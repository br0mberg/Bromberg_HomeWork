#include "updclientdata.h"

int Upd_client_data(const char* fname_client_data, const char* fname_transaction_data,
const char* fname_update_client_data) {
    FILE *clientDataFile = fopen(fname_client_data, "r+");

    FILE *transactionDataFile = fopen(fname_transaction_data, "r+");

    FILE *updateClientDataFile = fopen(fname_update_client_data, "w+");

    CreditData Client;
    CreditData transfer;

    if (!clientDataFile || !transactionDataFile || !updateClientDataFile) {
        fprintf(stderr, "Not acess");
        return ERROR_ACTION_WITH_FILE;
    }

    while (fscanf(clientDataFile, "%d%99s%99s%99s%99s%lf%lf%lf",
    &Client.Number, Client.Name, Client.Surname, Client.addres,
    Client.TelNumber, &Client.indebtedness, &Client.credit_limit,
    &Client.cash_payments) == CLIENT_STRUCT_FIELDS) {
        while (fscanf(transactionDataFile, "%d %lf", &transfer.Number, &transfer.cash_payments)
        == TRANSACTION_STRUCT_FIELDS) {
            if (Client.Number == transfer.Number && transfer.cash_payments != 0) {
                Client.credit_limit += transfer.cash_payments;
            }
        }
    output_data_client(updateClientDataFile, &Client);
    rewind(transactionDataFile);
    }

    if (fclose(clientDataFile) || fclose(transactionDataFile)
    || fclose(updateClientDataFile)) {
        fprintf(stderr, "ERROR_CLOSE_FILE");
        return ERROR_ACTION_WITH_FILE;
    }

    return 0;
}
