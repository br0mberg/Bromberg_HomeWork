#include "../include/recordtransactiondata.h"

void recordtransactiondata(const char* fname) {
    FILE *transactionfile;
    Data transfer = {0};
    transactionfile = fopen(fname, "r+");

    if (transactionfile == NULL) {
       puts("Not acess");
    } else {
        while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) == 2) {
            output_data(TRANSFER_OUTMODE, transactionfile, transfer);
        }

    fclose(transactionfile);
    }
}
