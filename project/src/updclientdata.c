#include "updclientdata.h"

void Upd_client_data(const char* fname_client_data, const char* fname_transaction_data,
const char* fname_update_client_data) {
    FILE *ClientDataFile;
    ClientDataFile = fopen(fname_client_data, "r+");

    FILE *TransactionDataFile;
    TransactionDataFile = fopen(fname_transaction_data, "r+");

    FILE *UpdateClientDataFile;
    UpdateClientDataFile = fopen(fname_update_client_data, "w+");

    CreditData Client;
    CreditData transfer;

    if (ClientDataFile == NULL || TransactionDataFile == NULL || UpdateClientDataFile == NULL) {
        puts("Not acess");
    } else {
        while (fscanf(ClientDataFile, "%d%99s%99s%99s%99s%lf%lf%lf",
        &Client.Number, Client.Name, Client.Surname, Client.addres,
        Client.TelNumber, &Client.indebtedness, &Client.credit_limit,
        &Client.cash_payments) == 8) {
            while (fscanf(TransactionDataFile, "%d %lf", &transfer.Number, &transfer.cash_payments) == 2) {
                if (Client.Number == transfer.Number && transfer.cash_payments != 0) {
                    Client.credit_limit += transfer.cash_payments;
                }
            }
            output_data(CLIENT_OUTMODE, UpdateClientDataFile, &Client);
            rewind(TransactionDataFile);
        }

    if (fclose(ClientDataFile) != 0 || fclose(TransactionDataFile) != 0
    || fclose(UpdateClientDataFile) !=0) {
        printf("ERROR_CLOSE_FILE");
        }
    }
}
