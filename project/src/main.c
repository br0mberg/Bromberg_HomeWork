#include "/mnt/c/Users/User/Desktop/Bromberg_HomeWork/project/include/utils.h"
#include "/mnt/c/Users/User/Desktop/Bromberg_HomeWork/project/include/check_simplicity.h"
#include "/mnt/c/Users/User/Desktop/Bromberg_HomeWork/project/include/custom_recursion.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int atoi(const char *str);
enum MAIN_CONST {
    ERR_ARGS_COUNT = -1,
    ERR_WRONG_FLG = -2,
    TST_FOO_FIX = 1,
    TST_FOO_IMPL,
    TST_MOD_IMPL,
    TST_MOD_LASTIMPL
};

//  val-grint
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

    int Test_case = atoi(argv[1]);
    const char* data;
    data = argv[2];

    switch (Test_case) {
        case TST_FOO_FIX: {
            int to = atoi(data);
            unsigned char add = 0;
            if (to > 255) add = 255;
            size_t ticks_count = 0;
            if (to > 0) {
                ticks_count = timer_from(to, add);
            }
            printf("%zu\n", ticks_count);
            break;
        }
        case TST_FOO_IMPL: {
            if (argc == 4) {
                int base = atoi(data);
                int pow =  atoi(argv[3]);
                int res = custom_pow(base, pow);
                printf("%d\n", res);
                break;
            } else {
                return ERR_ARGS_COUNT;
            }
        }
        case TST_MOD_IMPL: {
            int num = atoi(data);
            int res = custom_check_simplicity(num);
            printf("%d", res);
            break;
        }
        case TST_MOD_LASTIMPL: {
            int n = atoi(data);
            custom_recursion(n);
            break;
        }
        default: {
            return ERR_WRONG_FLG;
        }
    }
}
