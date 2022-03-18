#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/check_simplicity.h"

int custom_check_simplicity(int num) {
    if (num < 2) {
        return 0;
    }

    if (num > 2) {
        for (int i = 2; i <= num/2; ++i) {
            if (num % i == 0) {
                return 0;
            }
        }
    }
    return 1;
}
