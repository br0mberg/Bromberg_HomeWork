#include "../include/utils.h"
#include "../include/check_simplicity.h"
#include "../include/custom_recursion.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum MAIN_ERRORS {
    ERR_ARGS_COUNT = -1,
    ERR_WRONG_FLG = -2
};

enum MAIN_CONST {
    TST_FOO_FIX = 1,
    TST_FOO_IMPL,
    TST_MOD_IMPL,
    TST_MOD_LASTIMPL
};

/* NOTE(stitaevskiy):
 * We use `atoi` function just for simplification and code reducing.
 * This function doesn't report conversation errors.
 * For safety program we recommend using `strtol` and its analogs.
 * (See `man atoi` and `man strtol` for more info).
 * &&
 * const char str_num[] = "1234";
 * char* end = NULL;
 * int val = (int) strtol(str_num, &end, 0);
 * if (end != '\0') {
 *     //ERROR
 * }
 * ??
 * */

int main(int argc, const char** argv) {
    if (argc < 3) {
        return ERR_ARGS_COUNT;
    }

    int Test_case = (int) strtol(argv[1], NULL, 0);
    const char* data;
    data = argv[2];

    switch (Test_case) {
        case TST_FOO_FIX: {
            int to = (int) strtol(data, NULL, 0);
            int add = 0;
            if (to > 255) {
				add = 255;
			}
            size_t ticks_count = 0;
            if (to > 0) {
                ticks_count = timer_from(to, add);
            }
            printf("%zu\n", ticks_count);
            break;
        }
        case TST_FOO_IMPL: {
            if (argc == 4) {
                int base = (int) strtol(data, NULL, 0);
                int pow =  (int) strtol(argv[3], NULL, 0);
                int res = custom_pow(base, pow);
                printf("%d\n", res);
                break;
            } else {
                return ERR_ARGS_COUNT;
            }
        }
        case TST_MOD_IMPL: {
            int num = (int) strtol(data, NULL, 0);
            int res = custom_check_simplicity(num);
            printf("%d", res);
            break;
        }
        case TST_MOD_LASTIMPL: {
            int n = (int) strtol(data, NULL, 0);
            custom_recursion(n);
            break;
        }
        default: {
            return ERR_WRONG_FLG;
        }
    }
}
