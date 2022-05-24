#pragma once

#include <chrono>
#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <thread>
#include <tuple>
#include <vector>
#include <unistd.h>
#include <cstddef>

// #define DEBUG

#ifdef DEBUG
#include <algorithm>
#include <iostream>
#include <iterator>
#define LOGGER std::cerr
#define LOG_MESSAGE LOGGER << "[DEBUG] " << __FILE__ << ":" << __LINE__ << ": "
#define LOG_ERROR LOGGER << "[ERROR] " << __FILE__ << ":" << __LINE__ << ": "
#endif

enum class ErrorStatus {
    kNoError,
    kError
};

template <typename MessageTypeEnum>
struct Message {
    using Type = MessageTypeEnum;
    Message(Type type = Type::kUnknown, std::string_view content = "") : type(type), content(content) {}
    Type type;
    std::string content;
};

enum class RequestType {
    kUnknown,
    kPost,
    kGet
};

enum class ResponseType {
    kUnknown,
    kNew,
    kWork,
    kError,
    kDone
};

using Request = Message<RequestType>;
using Response = Message<ResponseType>;

template <typename MessageType>
struct NetworkMessage {
    static std::tuple<ErrorStatus, MessageType> Deserialize(const std::vector<std::byte> &network_bytes);
    static std::vector<std::byte> Serialize(const MessageType &message);
};

template <typename MessageType>
std::vector<std::byte> NetworkMessage<MessageType>::Serialize(const MessageType &message) {
    std::vector<std::byte> network_data;
    network_data[0] = sizeof(message.type);
    for (size_t i = 1; i < strlen(message.content) + 1; ++i) {
        network_data[i] = sizeof(message.content[i]);
    }
    // network_data[0] = sizeof(message.type);
    // пробежаться по строке и взять у каждого sizeof()
    // TODO - Implement me - нужно точно рассчитать количество байт и аккуратно скопировать их в массив
    return network_data;
}

template <typename MessageType>
std::tuple<ErrorStatus, MessageType> NetworkMessage<MessageType>::Deserialize(const std::vector<std::byte> &network_bytes) {
    // TODO - Implement me
    char* str = new char[network_bytes.size()];
    size_t i = 0;
    size_t type = network_bytes[0];
    for (i = 1; i < network_bytes.size(); ++i) {
        str[i] = network_bytes[i];
    }
    str[i + 1] = '\0';
    std::string out_str = std::string(str);
    return std::make_tuple(ErrorStatus::kNoError, MessageType{
        MessageType::Type type,
        std::string out_str;
    });
}

using NetworkRequest = NetworkMessage<Request>;

using NetworkResponse = NetworkMessage<Response>;

#ifdef DEBUG
inline std::ostream &operator<<(std::ostream &os, const Request::Type &type) {
    switch (type) {
        case Request::Type::kGet: { os << "Get"; break; }
        case Request::Type::kPost: { os << "Post"; break; }
        case Request::Type::kUnknown:
        default: { os << "Unknown"; break; }
    }
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Request &request) {
    os << "Request {type=" << request.type << ", content=" << request.content << "}";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Response::Type &type) {
    switch (type) {
        case Response::Type::kNew: { os << "New"; break; }
        case Response::Type::kWork: { os << "Work"; break; }
        case Response::Type::kDone: { os << "Done"; break; }
        case Response::Type::kError: { os << "Error"; break; }
        case Response::Type::kUnknown:
        default: { os << "Unknown"; break; }
    }
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Response &response) {
    os << "Response {type=" << response.type << ", content=" << response.content << "}";
    return os;
}
#endif

static constexpr int kBadFileDescriptor = -1;

struct AutoClosingFileDescriptor {
    AutoClosingFileDescriptor(int fd = kBadFileDescriptor) : fd_(fd) {}

    ~AutoClosingFileDescriptor() {
        if (fd_ != kBadFileDescriptor) {
#ifdef DEBUG
            LOG_MESSAGE << "Closing fd " << fd_ << "\n";
#endif
            close(fd_);
        }
    }

    operator int() { return fd_; }

  private:
    int fd_;
};

enum class ReadStatus {
    kNoData,
    kFailed,
    kSucceed
};

inline ReadStatus ReadFromNetwork(int socket_fd, std::vector<std::byte> &network_bytes,
                           bool retry_if_no_data = true,
                           const std::chrono::seconds &timeout = std::chrono::seconds(5),
                           const std::chrono::milliseconds &retry_time_interval = std::chrono::milliseconds(100)) {
    // unsigned int time_inteval = retry_time_interval.count();
    while (socket_fd == NULL && timeout > std::chrono::seconds(0) && retry_if_no_data == true) {
        sleep(retry_time_interval.count());
        if (timeout <= std::chrono::seconds(0)) {
            retry_if_no_data == false;
        }
    }
    if (socket_fd != NULL) {
        read(socket_fd, &network_bytes, sizeof(socket_fd));
        if (network_bytes.size() == 0) {
            return ReadStatus::kFailed;
        }
    } else {
        return ReadStatus::kNoData;
    }
    // ДЛЯ EXTRA SECONDS НАДО ПОМЕНЯТЬ = АСИНХРОН, не ждать данные вечность а с периодичностью
    // TODO - нужно считать данные из сокета в network_bytes
    // если данных нет - вернуть ReadStatus::kNoData (если не стоит retry_if_no_data и не вышел таймаут)
    // если произошла ошибка - вернуть ReadStatus::kFailed
    return ReadStatus::kSucceed
}

inline ErrorStatus SendToNetwork(int socket_fd, const std::vector<std::byte> &message_data) {
    if(write(socket_fd, &message_data, sizeof(socket_fd)) == -1) {
        return ErrorStatus::kError;
    }
    // TODO - нужно отправить массив байт через сокет
    // если произошла ошибка - вернуть
    return ErrorStatus::kNoError;
}

enum FileDescriptorOptions : int {
    kFileDescriptor_NoOptions = 0,
    kFileDescriptor_NonBlocking = O_NONBLOCK
};

inline ErrorStatus ConfigureFileDescriptor(int fd, FileDescriptorOptions options = kFileDescriptor_NoOptions) {
    int status = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | options);
    if (status == -1){
        return ErrorStatus::kError;
    }
    // FileDescriptorOptions options = kFileDescriptor_NoOptions - > 
    // TODO - Сконфигурируйте дескриптор через fcntl - например, чтобы сделать чтение неблокирующим
    return ErrorStatus::kNoError;
}

static constexpr std::string_view kFromNetPipeTag = "from_net";
static constexpr std::string_view kToNetPipeTag = "to_net";

struct NamedPipe {
    static constexpr std::string_view kNamedPipePrefix = "named-pipe-";

    NamedPipe(std::filesystem::path pipe_path = kUninitializedPath) : pipe_path_(pipe_path) {}

    static std::string GetPerProcessPipeName(std::string_view working_directory, std::string_view tag, pid_t process_id = getpid());
    static std::tuple<ErrorStatus, NamedPipe> Create(const std::filesystem::path &pipe_path);
    static std::tuple<ErrorStatus, int> GetReader(
        const std::filesystem::path &pipe_path,
        FileDescriptorOptions options = kFileDescriptor_NoOptions);
    static std::tuple<ErrorStatus, int> GetWriter(
        const std::filesystem::path &pipe_path);

    void Remove();

    std::tuple<ErrorStatus, int> GetReader(FileDescriptorOptions options = kFileDescriptor_NoOptions) const;
    std::tuple<ErrorStatus, int> GetWriter() const;

    template <typename ObjectType>
    ErrorStatus Write(const ObjectType &object) const;

    template <typename StructType>
    ReadStatus Read(
        StructType &read_object,
        FileDescriptorOptions options = kFileDescriptor_NoOptions,
        bool retry_if_no_data = true,
        const std::chrono::milliseconds &retry_time_interval = std::chrono::milliseconds(100)) const;

  private:
    static constexpr std::string_view kUninitializedPath = "";

    std::filesystem::path pipe_path_;
};

inline std::tuple<ErrorStatus, int> NamedPipe::GetReader(
        const std::filesystem::path &pipe_path,
        FileDescriptorOptions options) {
    int fifo = mkfifo(pipe_path.c_str(), S_IRUSR);
    if (fifo) {
        return std::make_tuple(ErrorStatus::kError, NULL);
    }
    int fd = open(reinterpret_cast<const char*>(fifo), options);
    if (fd < 0) {   
        return std::make_tuple(ErrorStatus::kError, NULL);
    }
    // TODO - нужно вернуть дескриптор для чтения (сконфигурированного с опциями) из именованного канала
    return std::make_tuple(ErrorStatus::kNoError, fd);
}

inline std::tuple<ErrorStatus, int> NamedPipe::GetWriter(
        const std::filesystem::path &pipe_path) {
    int fifo = mkfifo(pipe_path.c_str(), S_IWUSR);
    if (fifo) {
        return std::make_tuple(ErrorStatus::kError, NULL);
    }
    int fd = open(reinterpret_cast<const char*>(fifo), 0);
    if (fd < 0) {
        return std::make_tuple(ErrorStatus::kError, NULL);
    }
    // !TODO - нужно вернуть дескриптор для записи из именованного канала
    return std::make_tuple(ErrorStatus::kNoError, fd);
}

inline std::string NamedPipe::GetPerProcessPipeName(std::string_view working_directory, std::string_view tag, pid_t process_id) {
    return (std::filesystem::path{working_directory} / std::filesystem::path{
        std::string{kNamedPipePrefix} + std::string{tag} + std::to_string(process_id)
    }).string();
}

inline std::tuple<ErrorStatus, int> NamedPipe::GetReader(FileDescriptorOptions options) const {
    return GetReader(std::filesystem::path{pipe_path_}, options);
}

inline std::tuple<ErrorStatus, int> NamedPipe::GetWriter() const {
    return GetWriter(std::filesystem::path{pipe_path_});
}

inline std::tuple<ErrorStatus, NamedPipe> NamedPipe::Create(const std::filesystem::path &pipe_path) {
    // !TODO - нужно создать именованный pipe
    NamedPipe pip(pipe_path);
    return std::make_tuple(ErrorStatus::kNoError, pip);
}

template <typename ObjectType>
ErrorStatus WriteToFileDescriptor(int fd, const ObjectType &object) {
    ssize_t sucss_byt = write(fd, &object, object.size());
    if (sucss_byt == -1 || sucss_byt != object.size()) {
        return ErrorStatus::kError;
    }
    // TODO - запись цельного объекта (полезного набора байт) произвольного типа в дескриптор
    return ErrorStatus::kNoError;
}

inline ErrorStatus WriteToFileDescriptor(int fd, const std::vector<std::byte> &bytes) {
    ssize_t sucss_byt = write(fd, &bytes, bytes.size());
    if (sucss_byt == -1 || sucss_byt != bytes.size()) {
        return ErrorStatus::kError;
    }
    // TODO - запись массива байт в дескриптор
    return ErrorStatus::kNoError;
}

template <typename ObjectType>
ErrorStatus NamedPipe::Write(const ObjectType &object) const {
    // TODO - запись в инициализированный Named Pipe
    int fifo = mkfifo((this*)pipe_path.c_str(), S_IWUSR);
    if (fifo) {
        return std::make_tuple(ErrorStatus::kError, NULL);
    }
    int fd = open(reinterpret_cast<const char*>(fifo), 0);
    if (fd < 0) {
        return std::make_tuple(ErrorStatus::kError, NULL);
    }
    ssize_t sucss_wr = write(fd, &object, object.size());
    if (sucss_wr == -1) {
        return ErrorStatus::kError;
    }
    return ErrorStatus::kNoError;
}

inline void NamedPipe::Remove() {
    pipe_path_ = NULL;
    // TODO - удаление Named Pipe
}

struct AutoClosableNamedPipe : NamedPipe {
    AutoClosableNamedPipe(const NamedPipe &pipe_obj) : NamedPipe(pipe_obj), pipe_obj_(pipe_obj) {}
    ~AutoClosableNamedPipe() {
        pipe_obj_.Remove();
    }

    static std::tuple<ErrorStatus, AutoClosableNamedPipe> Create(const std::filesystem::path &pipe_path) {
        return NamedPipe::Create(pipe_path);
    }

  private:
    NamedPipe pipe_obj_;
};

struct Pipe {
    Pipe(int read_fd = kBadFileDescriptor, int write_fd = kBadFileDescriptor,
         FileDescriptorOptions read_options = kFileDescriptor_NoOptions) :
            read_fd_(read_fd), write_fd_(write_fd), read_options_(read_options) {}

    template <typename ObjectType>
    ErrorStatus Write(const ObjectType &object) const;

    template <typename StructType>
    ReadStatus Read(
        StructType &read_object,
        bool retry_if_no_data = true,
        const std::chrono::milliseconds &retry_time_interval = std::chrono::milliseconds(100)) const;

    inline ErrorStatus Close();

    inline static std::tuple<ErrorStatus, Pipe> Create(FileDescriptorOptions read_options = kFileDescriptor_NoOptions);

    int ReaderFd() const { return read_fd_; }
    int WriterFd() const { return write_fd_; }

#ifdef DEBUG
    friend std::ostream &operator<<(std::ostream &os, const Pipe &pipe) {
        os << "Pipe<read_fd=" << pipe.read_fd_ << ", write_fd=" << pipe.write_fd_ << ">";
        return os;
    }
#endif

  private:
    int read_fd_ = kBadFileDescriptor;
    int write_fd_ = kBadFileDescriptor;
    FileDescriptorOptions read_options_ = kFileDescriptor_NoOptions;
};

template <typename ObjectType>
ErrorStatus Pipe::Write(const ObjectType &object) const {
    return WriteToFileDescriptor(write_fd_, object);
}

template <typename StructType>
ReadStatus ReadFromFileDescriptor(
        int fd,
        StructType &read_object,
        bool retry_if_no_data = true,
        const std::chrono::milliseconds &retry_time_interval = std::chrono::milliseconds(100)) {
    // TODO - похоже на ReadFromNetwork, только для файлового дескриптора
    // read_object - цельный объект, который можно создать без параметров и заполнить единой операцией чтения
    const std::chrono::seconds &timeout = std::chrono::seconds(1);
    while (fd == NULL && timeout > std::chrono::seconds(0) && retry_if_no_data == true) {
        sleep(retry_time_interval.count());
        if (timeout <= std::chrono::seconds(0)) {
            retry_if_no_data == false;
        }
    }
    if (fd != NULL) {
        if (read(fd, &read_object, sizeof(fd))) {
            return ReadStatus::kFailed;
        }
    } else {
        return ReadStatus::kNoData;
    }
}

template <>
inline ReadStatus ReadFromFileDescriptor<std::vector<std::byte>>(
        int fd,
        std::vector<std::byte> &bytes_to_read,
        bool retry_if_no_data,
        const std::chrono::milliseconds &retry_time_interval) {
    const std::chrono::seconds &timeout = std::chrono::seconds(1);
    while (fd == NULL && timeout > std::chrono::seconds(0) && retry_if_no_data == true) {
        sleep(retry_time_interval.count());
        if (timeout <= std::chrono::seconds(0)) {
            retry_if_no_data == false;
        }
    }
    if (fd != NULL) {
        if (read(fd, &bytes_to_read, sizeof(fd))) {
            return ReadStatus::kFailed;
        }
    } else {
        return ReadStatus::kNoData;
    }
    // TODO - частный случай ReadFromFileDescriptor, когда объект не простой, а представляет из себя vector<std::byte>
}

template <typename StructType>
ReadStatus NamedPipe::Read(
        StructType &read_object,
        FileDescriptorOptions options,
        bool retry_if_no_data,
        const std::chrono::milliseconds &retry_time_interval) const {
    const std::chrono::seconds &timeout = std::chrono::seconds(1);
    int fifo = mkfifo((this*)pipe_path.c_str(), /*S_IWUSR*/ options);
    if (fifo) {
        return ErrorStatus::kError;
    }
    int fd = open(reinterpret_cast<const char*>(fifo), options);
    if (fd < 0) {
        return ErrorStatus::kError;
    }

    while (fd == NULL && timeout > std::chrono::seconds(0) && retry_if_no_data == true) {
        sleep(retry_time_interval.count());
        if (timeout <= std::chrono::seconds(0)) {
            retry_if_no_data == false;
        }
    }
    if (fd != NULL) {
        ssize_t sucss_wr = write(fd, &read_object, read_object.size());
        if (sucss_wr == -1) {
            return ErrorStatus::kError;
        }
    } else {
        return ReadStatus::kNoData;
    }
    return ErrorStatus::kNoError;
    // TODO - чтение из именованного канала
}

template <typename StructType>
ReadStatus Pipe::Read(
        StructType &read_object,
        bool retry_if_no_data,
        const std::chrono::milliseconds &retry_time_interval) const {
    const std::chrono::seconds &timeout = std::chrono::seconds(1);

    while (fd == NULL && timeout > std::chrono::seconds(0) && retry_if_no_data == true) {
        sleep(retry_time_interval.count());
        if (timeout <= std::chrono::seconds(0)) {
            retry_if_no_data == false;
        }
    }
    if (fd != NULL) {
        ssize_t sucss_r = 0;
        bool run = 1;
        while (run) {
        if (sucss_r < read_fd_.size()) {
            ssize_t r += read(read_fd_, &read_object, read_fd_.size());
            if (r == 0) {
                run = 0;
            }
            sucss_r += r;
        }
        }
        if (sucss_r == -1 || sucss_r != read_fd_.size()) {
            return ErrorStatus::kError;
        }
    } else {
        return ReadStatus::kNoData;
    }
    return ErrorStatus::kNoError;
    // TODO - чтение из неименованного канала (внимательно убедитесь на тестовых примерах, в чём разница неблокирующего чтения
    // из именованного и неименованного канала в зависимости от того, начато ли оно раньше, чем была запись, или если чтение
    // осуществляется меньшими "кусками", чем было записано, или если было несколько раз что-то записано, а потом за одну-несколько
    // итераций - считано
    // Эксперименты и понимание корректного чтения лишат вас огромного количества глупых ошибок
}

ErrorStatus Pipe::Close() {
    close(read_fd_);
    close(write_fd_);
    read_options_ = kFileDescriptor_NoOptions;
    // TODO - все ресурсы нужно освобождать после работы с ними. Pipe (обёртка над int fd[2]) - не исключение
}
/*void func_pipe(int *a = 0, int *b = 0) {
    int fd[2];
    if( pipe(fd[2]) == -1) {
        return ErrorStatus::kError;
    }
    a = fd[0];
    b = fd[1];
}*/
// std::function<void()> = f;
std::tuple<ErrorStatus, Pipe> Pipe::Create(FileDescriptorOptions read_options) {
    int fd[2];
    if (pipe(fd) == -1) {
        return std::make_tuple(ErrorStatus::kError, NULL);
    }
    Pipe pip(fd[0], fd[1], read_options);
    // TODO - создайте удобную обёртку над int fd[2] и необходимостью явно вызывать pipe во внешнем коде
    return std::make_tuple(ErrorStatus::kNoError, pip);
}

struct AutoClosingPipe : Pipe {
    AutoClosingPipe(const Pipe &pipe_obj) : Pipe(pipe_obj), pipe_obj_(pipe_obj) {}
    ~AutoClosingPipe() {
        pipe_obj_.Close();
    }

  private:
    Pipe pipe_obj_;
};
