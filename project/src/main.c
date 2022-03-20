#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/utils.h"
#include "../include/check_simplicity.h"
#include "../include/custom_recursion.h"
#include "../include/check_error_strtol.h"

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

int main(int argc, const char** argv) {
    if (argc < 3) {
        return ERR_ARGS_COUNT;
    }

	char* end = NULL;

    int test_case = (int) strtol(argv[1], &end, 0);
	check_error_strtol(&end);
	end = NULL;

    const char* data;
    data = argv[2];

    switch (test_case) {
        case TST_FOO_FIX: {
            int to = (int) strtol(data, &end, 0);
			check_error_strtol(&end);

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
                int base = (int) strtol(data, &end, 0);
				check_error_strtol(&end);

				end = NULL;

                int pow =  (int) strtol(argv[3], &end, 0);
				check_error_strtol(&end);

                printf("%d\n", custom_pow(base, pow));

                break;
            } else {
                return ERR_ARGS_COUNT;
            }
        }
        case TST_MOD_IMPL: {
            int num = (int) strtol(data, &end, 0);
			check_error_strtol(&end);

            printf("%d", custom_check_simplicity(num));

            break;
        }
        case TST_MOD_LASTIMPL: {
            int n = (int) strtol(data, &end, 0);
			check_error_strtol(&end);

            custom_recursion(n);

            break;
        }
        default: {
            return ERR_WRONG_FLG;
        }
    }
}
