#ifndef PROJECT_INCLUDE_CUSTOMTEST_H_
#define PROJECT_INCLUDE_CUSTOMTEST_H_

#include <string.h>
#include <stdio.h>
#include "out_inputdata.h"

int write_to_file(FILE *source, CreditData *data);

int read_from_file(FILE *source, CreditData *data);

int record_struct_custest(CreditData *data);

int check_similar_data(CreditData *got_data, CreditData *expected_data);

#endif  // PROJECT_INCLUDE_CUSTOMTEST_H_
