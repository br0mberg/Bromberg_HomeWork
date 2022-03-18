#include "../include/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

size_t timer_from(unsigned char from, int add) {
    size_t counter = 0;
    int num = 0;

    if (add != 0) {
        from += 1;
    }

    for (size_t i = from + add; i > 0; --i) {
        ++counter;
        num = (int) i;
        printf("%d ", num);
    }

    num -= 1;
    ++counter;

    printf("%d", num);
    printf("\n");
    return counter;
}

int custom_pow(int base, int power) {
    int res = base;
    if (!power) {
        return 1;
    } else {
        for (int i = 1; i < power; ++i) {
        res *= base;
        }
    }
    return res;
}
