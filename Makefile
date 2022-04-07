TARGET = ./main.out
HDRS_DIR = project/include

SRCS = \
	project/src/main.c \
	project/src/updclientdata.c \
	project/src/recordclientdata.c \
	project/src/recordtransactiondata.c \
	project/src/out_inputdata.c
	
TEST_TARGET = ./test.out

TEST_SRCS = \
	project/src/customtest.c \
	project/src/out_inputdata.c

.PHONY: all build rebuild check test memtest clean

all: clean check test memtest

$(TARGET): $(SRCS)
	$(CC) -Wpedantic -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

$(TEST_TARGET): $(TEST_SRCS)
	$(CC) -Wpedantic -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TEST_TARGET) $(CFLAGS) $(TEST_SRCS)

build: $(TARGET) $(TEST_TARGET)

rebuild: clean build

check:
	./run_linters.sh

test: $(TARGET) $(TEST_TARGET)
	./btests/run.sh $(TARGET) && ./test.out 
	
memtest: $(TARGET)
	./btests/run.sh $(TARGET) --memcheck

clean:
	rm -rf $(TARGET) $(TEST_TARGET) *.dat

