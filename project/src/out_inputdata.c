#include "../include/out_inputdata.h"

void output_data(int outmode, FILE* file, Data data) {
    switch (outmode) {
        case CLIENT_OUTMODE:
            printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
                   "1 Number account: ",
                   "2 Client name: ",
                   "3 Surname: ",
                   "4 Addres client: ",
                   "5 Client Telnum: ",
                   "6 Client indebtedness: ",
                   "7 Client credit limit: ",
                   "9 Client cash payments:");
            fprintf(file, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
            data.Number, data.Name, data.Surname,
            data.addres, data.TelNumber, data.indebtedness,
            data.credit_limit, data.cash_payments);

            break;
        case TRANSFER_OUTMODE:
            printf("%s\n%s\n", "1 Number account:", "2 Client cash payments: ");
            fprintf(file, "%-3d%-6.2f\n", data.Number, data.cash_payments);
            break;
        default:
            printf("INCORRECT_OUTMODE");
    }
}

void read_from_file(FILE *got_data) {
    Data Record_got_data;

    if (got_data == NULL) {
        puts("Not acess");
    } else {
        fscanf(got_data, "%d%99s%99s%99s%99s%lf%lf%lf",
        &Record_got_data.Number, Record_got_data.Name, Record_got_data.Surname, Record_got_data.addres,
        Record_got_data.TelNumber, &Record_got_data.indebtedness, &Record_got_data.credit_limit,
        &Record_got_data.cash_payments);
    }
}

void write_to_file(FILE* expected_data) {
    Data Record_expected_data = {0};
    if (expected_data == NULL) {
        puts("Not acess");
    } else {
        output_data(CLIENT_OUTMODE, expected_data, Record_expected_data);
    }
}
