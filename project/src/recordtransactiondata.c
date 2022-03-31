#include "../include/recordtransactiondata.h"

void recordtransactiondata(const char* fname) {
    FILE *ofPtr;
    ofPtr = fopen(fname, "r+");

    if (ofPtr == NULL) {
       puts("Not acess");
    } else {
        while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) == 2) {
            output_data(TRANSFER_OUTMODE, ofPtr, transfer);
        }

    fclose(ofPtr);
    }
}
