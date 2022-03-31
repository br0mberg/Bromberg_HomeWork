#include "../include/updclientdata.h"

void updclientdata(const char* fname_record, const char* fname_transaction, const char* fname_blackrecord) {
    FILE *recordfile, *transactionfile, *blackrecord;

    recordfile = fopen(fname_record, "r+");
    transactionfile = fopen(fname_transaction, "r+");
    blackrecord = fopen(fname_blackrecord, "w+");

    Data Client;
    Data transfer;

    if (recordfile == NULL || transactionfile == NULL || blackrecord == NULL) {
        puts("exit");
    } else {
        while (fscanf(recordfile, "%d%99s%99s%99s%99s%lf%lf%lf",
        &Client.Number, Client.Name, Client.Surname, Client.addres,
        Client.TelNumber, &Client.indebtedness, &Client.credit_limit,
        &Client.cash_payments) == 8) {
            while (fscanf(transactionfile, "%d %lf", &transfer.Number, &transfer.cash_payments) == 2) {
                if (Client.Number == transfer.Number && transfer.cash_payments != 0) {
                    Client.credit_limit += transfer.cash_payments;
                }
            }
            output_data(CLIENT_OUTMODE, blackrecord, Client);
            rewind(transactionfile);
        }

    fclose(recordfile);
    fclose(transactionfile);
    fclose(blackrecord);
    }
}
