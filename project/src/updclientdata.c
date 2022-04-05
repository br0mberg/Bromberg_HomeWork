#include "updclientdata.h"

int upd_client_data(const char* fname_client, const char* fname_transaction,
const char* fname_update) {
    FILE *clientDataFile = fopen(fname_client, "r+");

    FILE *transactionDataFile = fopen(fname_transaction, "r+");

    FILE *updateClientDataFile = fopen(fname_update, "w+");

    CreditData Client;
    CreditData transfer;

    if (!clientDataFile || !transactionDataFile || !updateClientDataFile) {
        fprintf(stderr, "Not acess");
        if (clientDataFile) {
            fclose(clientDataFile);
        }
        if (transactionDataFile) {
            fclose(transactionDataFile);
        }
        if (updateClientDataFile) {
            fclose(updateClientDataFile);
        }
        return ERROR_ACTION_WITH_FILE;
    }

    while (input_data(clientDataFile, &Client) == CLIENT_STRUCT_FIELDS) {
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
