#include "recordclientdata.h"

int record_client_data(const char* fname) {
    FILE *client_data_file = fopen(fname, "r+");

    credit_data_t client = {0};

    if (!client_data_file) {
        fprintf(stderr, "Not acess");
        return ERROR_ACTION_WITH_FILE;
    }

    while (input_data_client(&client) == CLIENT_STRUCT_FIELDS) {
        output_data_client(client_data_file, &client);
    }

    if (fclose(client_data_file)) {
        fprintf(stderr, "ERROR_CLOSE_FILE");
        return ERROR_ACTION_WITH_FILE;
    }

    return 0;
}
