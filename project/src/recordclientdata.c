#include "recordclientdata.h"

void Record_client_data(const char* fname) {
    FILE *ClientDatafile;
    ClientDatafile = fopen(fname, "r+");

    CreditData Client;

    if (ClientDatafile == NULL) {
        puts("Not acess");
    } else {
        while (scanf("%d%99s%99s%99s%99s%lf%lf%lf",
        &Client.Number, Client.Name, Client.Surname,
        Client.addres, Client.TelNumber, &Client.indebtedness,
        &Client.credit_limit, &Client.cash_payments) == CLIENT_STRUCT_FIELDS) {
            output_data(CLIENT_OUTMODE, ClientDatafile, &Client);
        }

    if (fclose(ClientDatafile) != 0) {
        printf("ERROR_CLOSE_FILE");
    }
    }
}
