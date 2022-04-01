#include "recordtransactiondata.h"

void Record_transaction_data(const char* fname) {
    FILE *transactionfile;
    CreditData transfer = {0};
    transactionfile = fopen(fname, "r+");

    if (transactionfile == NULL) {
       puts("Not acess");
    } else {
        while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) == 2) {
            output_data(TRANSFER_OUTMODE, transactionfile, &transfer);
        }

    if (fclose(transactionfile) != 0) {
        printf("ERROR_CLOSE_FILE");
    }
    }
}
