#include "out_inputdata.h"

int output_data_client(FILE* file, credit_data_t *data) {
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
           "1 Number account: ",
           "2 Client name: ",
           "3 Surname: ",
           "4 Addres client: ",
           "5 Client Telnum: ",
           "6 Client indebtedness: ",
           "7 Client credit limit: ",
           "9 Client cash payments:");

    if (fprintf(file, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
    data->Number, data->Name, data->Surname,
    data->addres, data->TelNumber, data->indebtedness,
    data->credit_limit, data->cash_payments) < 0) {
        fprintf(stderr, "ERROR WRITE TO FILE");
        return ERROR_ACTION_WITH_FILE;
    }

    return 0;
}

int output_data_transaction(FILE* file, credit_data_t *data) {
    printf("%s\n%s\n", "1 Number account:", "2 Client cash payments: ");
    if (fprintf(file, "%-3d%-6.2f\n", data->Number, data->cash_payments) < 0) {
        fprintf(stderr, "ERROR WRITE TO FILE");
        return ERROR_ACTION_WITH_FILE;
    }

    return 0;
}

int read_from_file(FILE *got_data, credit_data_t *data) {
    if (!got_data) {
        fprintf(stderr, "Not acess");
        return ERROR_ACTION_WITH_FILE;
    }
    input_data_client_to_file(got_data, data);

    return 0;
}

int write_to_file(FILE* expected_data, credit_data_t *data) {
    if (!expected_data) {
        fprintf(stderr, "Not acess");
        return ERROR_ACTION_WITH_FILE;
    }

    output_data_client(expected_data, data);

    return 0;
}

int input_data_client_to_file(FILE* file, credit_data_t *client) {
    return fscanf(file, "%d%99s%99s%99s%99s%lf%lf%lf",
    &client->Number, client->Name, client->Surname, client->addres,
    client->TelNumber, &client->indebtedness, &client->credit_limit,
    &client->cash_payments);
}

int input_data_client(credit_data_t *client) {
    return scanf("%d%99s%99s%99s%99s%lf%lf%lf",
        &client->Number, client->Name, client->Surname,
        client->addres, client->TelNumber, &client->indebtedness,
        &client->credit_limit, &client->cash_payments);
}

int input_data_transfer_to_file(FILE* file, credit_data_t *transfer) {
    return fscanf(file, "%d %lf", &transfer->Number, &transfer->cash_payments);
}

int input_data_transfer(credit_data_t *transfer) {
    return scanf("%d %lf", &transfer->Number, &transfer->cash_payments);
}

int record_struct_custest(credit_data_t *data) {
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
        "1 Number account: ",
        "2 Client name: ",
        "3 Surname: ",
        "4 Addres client: ",
        "5 Client Telnum: ",
        "6 Client indebtedness: ",
        "7 Client credit limit: ",
        "8 Client cash payments: ");

    if (input_data_client(data) != CLIENT_STRUCT_FIELDS) {
            fprintf(stderr, "ERROR_INPUT_CUSTOM_TEST_DATA");
            return ERROR_ACTION_WITH_FILE;
        }

    return 0;
}

int check_similar_data(credit_data_t *got_data, credit_data_t *expected_data) {
    if (got_data->Number != expected_data->Number
    || strcmp(expected_data->Name, got_data->Name) != 0
    || strcmp(got_data->Surname, expected_data->Surname) !=0
    || strcmp(got_data->addres, expected_data->addres) !=0
    || strcmp(got_data->TelNumber, expected_data->TelNumber) != 0
    || got_data->indebtedness != expected_data->indebtedness
    || got_data->credit_limit != expected_data->credit_limit
    || got_data->cash_payments != expected_data->cash_payments) {
        fprintf(stderr, "MY TEST ERROR");
        return ERROR_ACTION_WITH_FILE;
    }
    printf("\n***** MY TEST SUCCEED *****\n");

    return 0;
}
