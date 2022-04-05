#include "recordclientdata.h"

int record_client_data(const char* fname) {
    FILE *clientDatafile = fopen(fname, "r+");

    CreditData Client;

    if (!clientDatafile) {
        fprintf(stderr, "Not acess");
        return ERROR_ACTION_WITH_FILE;
    }

    while (scanf("%d%99s%99s%99s%99s%lf%lf%lf",
    &Client.Number, Client.Name, Client.Surname,
    Client.addres, Client.TelNumber, &Client.indebtedness,
    &Client.credit_limit, &Client.cash_payments) == CLIENT_STRUCT_FIELDS) {
        output_data_client(clientDatafile, &Client);
    }

    if (fclose(clientDatafile)) {
        fprintf(stderr, "ERROR_CLOSE_FILE");
        return ERROR_ACTION_WITH_FILE;
    }

    return 0;
}
