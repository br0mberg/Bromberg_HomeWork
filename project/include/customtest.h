#ifndef PROJECT_INCLUDE_CUSTOMTEST_H_
#define PROJECT_INCLUDE_CUSTOMTEST_H_

#include <string.h>
#include <stdio.h>
#include "out_inputdata.h"

int write_to_file(FILE *source, credit_data_t *data);

int read_from_file(FILE *source, credit_data_t *data);

int record_struct_custest(credit_data_t *data);

int check_similar_data(credit_data_t *got_data, credit_data_t *expected_data);

#endif  // PROJECT_INCLUDE_CUSTOMTEST_H_
