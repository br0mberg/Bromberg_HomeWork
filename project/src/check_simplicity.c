#include "../include/check_simplicity.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int custom_check_simplicity(int num) {
    int value = num;
    int flag = 0;
    if (value > 2) {
        for (int i = 2; i <= value/2 && flag == 0; ++i) {
            if (value % i == 0) {
                flag = 1;
            }
        }
    } else {
        return 0;
    }
    if (flag == 0) {
        return 1;
    } else {
        return 0;
    }
}
