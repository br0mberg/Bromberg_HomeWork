#include "recordtransactiondata.h"

int record_transaction_data(const char* fname) {
    FILE *transaction_file = fopen(fname, "r+");
    credit_data_t transfer = {0};

    if (!transaction_file) {
       fprintf(stderr, "Not acess");
       return ERROR_ACTION_WITH_FILE;
    }

    while (input_data_transfer(&transfer) == TRANSACTION_STRUCT_FIELDS) {
        output_data_transaction(transaction_file, &transfer);
    }

    if (fclose(transaction_file)) {
        fprintf(stderr, "ERROR_CLOSE_FILE");
        return ERROR_ACTION_WITH_FILE;
    }

    return 0;
}
