#include "../include/updclientdata.h"

void updclientdata(const char* fname_record, const char* fname_transaction, const char* fname_blackrecord) {
    FILE *ofPTR, *ofPTR_2, *blackrecord;

    ofPTR = fopen(fname_record, "r+");
    ofPTR_2 = fopen(fname_transaction, "r+");
    blackrecord = fopen(fname_blackrecord, "w+");

    if (ofPTR == NULL || ofPTR_2 == NULL || blackrecord == NULL) {
        puts("exit");
    } else {
        while (fscanf(ofPTR, "%d%99s%99s%99s%99s%lf%lf%lf",
        &Client.Number, Client.Name, Client.Surname, Client.addres,
        Client.TelNumber, &Client.indebtedness, &Client.credit_limit,
        &Client.cash_payments) == 8) {
            while (fscanf(ofPTR_2, "%d %lf", &transfer.Number, &transfer.cash_payments) == 2) {
                if (Client.Number == transfer.Number && transfer.cash_payments != 0) {
                    Client.credit_limit += transfer.cash_payments;
                }
            }
            output_data(CLIENT_OUTMODE, blackrecord, Client);
            rewind(ofPTR_2);
        }

    fclose(ofPTR);
    fclose(ofPTR_2);
    fclose(blackrecord);
    }
}
