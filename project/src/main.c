#include "../include/emlparse.h"

int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "All is not ok");
        return ERROR;
    }
    const char *path_to_eml = argv[1];

    size_t length = 0;
    get_length_str(path_to_eml, &length);

    char* str = malloc(length);
    if (!str) {
        fprintf(stderr, "\nERROR - CAN'T REALLOC\n");
        return ERROR;
    }

    get_eml_str(path_to_eml, str, &length);

    free(str);
    return 0;
}
