#include "../include/recordclientdata.h"

void recordclientdata(const char* fname) {
    FILE *recordfile;
    recordfile = fopen(fname, "r+");

    Data Client;

    if (recordfile == NULL) {
        puts("Not acess");
    } else {
        while (scanf("%d%99s%99s%99s%99s%lf%lf%lf",
        &Client.Number, Client.Name, Client.Surname,
        Client.addres, Client.TelNumber, &Client.indebtedness,
        &Client.credit_limit, &Client.cash_payments) == 8) {
            output_data(CLIENT_OUTMODE, recordfile, Client);
        }

    fclose(recordfile);
    }
}
