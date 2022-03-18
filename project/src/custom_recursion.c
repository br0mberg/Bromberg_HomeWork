#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/custom_recursion.h"

void custom_recursion(int n) {
    if (n > 1) {
        custom_recursion(n - 1);

        printf(" ");
    }
    if (n < 1) {
        custom_recursion(n + 1);
        printf(" ");
    }

    printf("%d", n);
}
