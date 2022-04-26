#include "../include/emlparse.h"

int get_length_str(const char* path_to_eml, size_t* length) {
    FILE* strfile = fopen(path_to_eml, "r");
    if (!strfile) {
        fprintf(stderr, "ERROR - CAN'T OPEN FILE");
        return ERROR;
    }

    fseek(strfile, 0, SEEK_END);
    *length = ftell(strfile);
    fseek(strfile, 0, SEEK_SET);
    if (fclose(strfile)) {
        fprintf(stderr, "\nERROR - CAN'T CLOSE FILE\n");
        return ERROR;
    }
    return 0;
}

int get_eml_str(const char* path_to_eml, char* buffer, size_t* length) {
    FILE* strfile = fopen(path_to_eml, "r");
    if (!strfile) {
        fprintf(stderr, "ERROR - CAN'T OPEN FILE");
        return ERROR;
    }
    fseek(strfile, 0, SEEK_SET);
    fread(buffer, 1, *length, strfile);
    buffer[*length] = '\0';
    if (fclose(strfile)) {
        fprintf(stderr, "\nERROR - CAN'T CLOSE FILE\n");
        return ERROR;
    }
    return 0;
}
