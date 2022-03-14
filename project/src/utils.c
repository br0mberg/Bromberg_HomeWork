#include "/mnt/c/Users/User/Desktop/Bromberg_HomeWork/project/include/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

size_t timer_from(unsigned char from, unsigned char add) {
    size_t counter = 0;
    int num = 0;

    if (add != 0) {
        from += 1;
    }

    for (unsigned char i = from; i > 0; --i) {
        ++counter;
        num = (int) i + (int) add;
        printf("%d ", num);
    }

    num -= 1;
    ++counter;
    printf("%d", num);

    for (unsigned char i = add; i > 0; --i) {
        ++counter;
        int number = (int) i -1;
        printf(" %d", number);
    }

    printf("\n");
    return counter;
}

int custom_pow(int base, int power) {
    int res = base;
    if (power != 0) {
        for (int i=1; i < power; ++i) {
        res *= base;
        }
    } else {
        res = 1;
    }
    return res;
}
// text
