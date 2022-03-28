#include "../include/recordclientdata.h"

void recordclientdata(const char* fname, Data data) {
    FILE *ofPTR;
    ofPTR = fopen(fname, "r+");

    if (ofPTR == NULL) {
        puts("Not acess");
    } else {
        while (scanf("%d%99s%99s%99s%99s%lf%lf%lf",
        &data.Number, data.Name, data.Surname,
        data.addres, data.TelNumber, &data.indebtedness,
        &data.credit_limit, &data.cash_payments) == 8) {
            output_data(CLIENT_OUTMODE, ofPTR, data);
        }

    fclose(ofPTR);
    }
}
