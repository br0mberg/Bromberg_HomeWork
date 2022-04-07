#include "updclientdata.h"

int upd_client_data(const char* fname_client, const char* fname_transaction,
const char* fname_update) {
    FILE *client_data_file = fopen(fname_client, "r+");
    if (!client_data_file) {
        fprintf(stderr, "Not acess");
        return ERROR_ACTION_WITH_FILE;
    }

    FILE *transaction_data_file = fopen(fname_transaction, "r+");
    if (!transaction_data_file) {
        fprintf(stderr, "Not acess");
        fclose(client_data_file);
        return ERROR_ACTION_WITH_FILE;
    }

    FILE *update_client_data_file = fopen(fname_update, "w+");
    if (!update_client_data_file) {
        fprintf(stderr, "Not acess");
        fclose(transaction_data_file);
        fclose(client_data_file);
        return ERROR_ACTION_WITH_FILE;
    }

    credit_data_t client;
    credit_data_t transfer;

    while (input_data_to_file(client_data_file, &client) == CLIENT_STRUCT_FIELDS) {
        while (fscanf(transaction_data_file, "%d %lf", &transfer.Number, &transfer.cash_payments)
        == TRANSACTION_STRUCT_FIELDS) {
            if (client.Number == transfer.Number && transfer.cash_payments != 0) {
                client.credit_limit += transfer.cash_payments;
            }
        }
    output_data_client(update_client_data_file, &client);
    rewind(transaction_data_file);
    }

    if (fclose(client_data_file) || fclose(transaction_data_file)
    || fclose(update_client_data_file)) {
        fprintf(stderr, "ERROR_CLOSE_FILE");
        return ERROR_ACTION_WITH_FILE;
    }

    return 0;
}
