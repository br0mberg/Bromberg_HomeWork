#include "customtest.h"

int main() {
    const char* filename = "customtest.dat";
    FILE *custestfile = fopen(filename, "w+");

    CreditData Record_expected_data = {0};
    CreditData Record_got_data = {0};

    record_struct_custest(&Record_expected_data);

    if (!custestfile) {
        fprintf(stderr, "ERROR_OPEN_FILE_CUSTEST");
        if (fclose(custestfile)) {
            printf("ERROR_CLOSE_FILE");
            return ERROR_ACTION_WITH_FILE;
        }
        return ERROR_ACTION_WITH_FILE;
    }

    write_to_file(custestfile, &Record_expected_data);
    if (fclose(custestfile)) {
        printf("ERROR_CLOSE_FILE");
    }

    if (!(custestfile = fopen(filename, "r+"))) {
        fprintf(stderr, "ERROR_OPEN_FILE_CUSTEST");
        if (fclose(custestfile)) {
            printf("ERROR_CLOSE_FILE");
            return ERROR_ACTION_WITH_FILE;
        }
        return ERROR_ACTION_WITH_FILE;
    }

    read_from_file(custestfile, &Record_got_data);

    if (fclose(custestfile)) {
        printf("ERROR_CLOSE_FILE");
        return ERROR_ACTION_WITH_FILE;
    }

    check_similar_data(&Record_got_data, &Record_expected_data);
    return 0;
}
