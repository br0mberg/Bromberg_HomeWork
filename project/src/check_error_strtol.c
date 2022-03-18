#include <stdio.h>
#include <stdlib.h>
#include "../include/check_error_strtol.h"

void check_error_strtol(char** end) {
    if (end == NULL) {
        printf("ERROR: INVALID STRING");
    }
}
