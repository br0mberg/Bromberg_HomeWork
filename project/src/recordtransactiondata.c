#include "recordtransactiondata.h"

int record_transaction_data(const char* fname) {
    FILE *transactionfile;
    CreditData transfer = {0};
    transactionfile = fopen(fname, "r+");

    if (!transactionfile) {
       fprintf(stderr, "Not acess");
       return ERROR_ACTION_WITH_FILE;
    }
    while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) == TRANSACTION_STRUCT_FIELDS) {
        output_data_transaction(transactionfile, &transfer);
    }

    if (fclose(transactionfile)) {
        fprintf(stderr, "ERROR_CLOSE_FILE");
        return ERROR_ACTION_WITH_FILE;
    }

    return 0;
}
