#include "../include/customtest.h"

int main() {
    const char* filename = "customtest.dat";
    FILE *custestfile;

    Data Record_expected_data = {0};
    Data Record_got_data = {0};

    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
        "1 Number account: ",
        "2 Client name: ",
        "3 Surname: ",
        "4 Addres client: ",
        "5 Client Telnum: ",
        "6 Client indebtedness: ",
        "7 Client credit limit: ",
        "8 Client cash payments: ");

    scanf("%d%99s%99s%99s%99s%lf%lf%lf",
        &Record_expected_data.Number, Record_expected_data.Name, Record_expected_data.Surname,
        Record_expected_data.addres, Record_expected_data.TelNumber, &Record_expected_data.indebtedness,
        &Record_expected_data.credit_limit, &Record_expected_data.cash_payments);

    custestfile = fopen(filename, "w+");

    write_to_file(custestfile);
    fclose(custestfile);

    custestfile = fopen(filename, "r+");

    read_from_file(custestfile);
    fclose(custestfile);

    if (Record_got_data.Number != Record_expected_data.Number
    || strcmp(Record_expected_data.Name, Record_got_data.Name) != 0
    || strcmp(Record_got_data.Surname, Record_expected_data.Surname) !=0
    || strcmp(Record_got_data.addres, Record_expected_data.addres) !=0
    || strcmp(Record_got_data.TelNumber, Record_expected_data.TelNumber) != 0
    || Record_got_data.indebtedness != Record_expected_data.indebtedness
    || Record_got_data.credit_limit != Record_expected_data.credit_limit
    || Record_got_data.cash_payments != Record_expected_data.cash_payments) {
        printf("\nMY TEST ERROR\n");
    } else {
        printf("\n***** MY TEST SUCCEED *****\n");
    }
}
