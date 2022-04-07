#include "customtest.h"

int main() {
    const char* filename = "customtest.dat";
    FILE *custestfile = fopen(filename, "w+");

    credit_data_t record_expected_data = {0};
    credit_data_t record_got_data = {0};

    record_struct_custest(&record_expected_data);

    if (!custestfile) {
        fprintf(stderr, "ERROR_OPEN_FILE_CUSTEST");
        return ERROR_ACTION_WITH_FILE;
    }

    write_to_file(custestfile, &record_expected_data);
    if (fclose(custestfile)) {
        printf("ERROR_CLOSE_FILE");
    }

    if (!(custestfile = fopen(filename, "r+"))) {
        fprintf(stderr, "ERROR_OPEN_FILE_CUSTEST");
        return ERROR_ACTION_WITH_FILE;
    }

    read_from_file(custestfile, &record_got_data);

    if (fclose(custestfile)) {
        printf("ERROR_CLOSE_FILE");
    }

    check_similar_data(&record_got_data, &record_expected_data);
    return 0;
}
