#ifndef PROJECT_INCLUDE_CUSTOMTEST_H_
#define PROJECT_INCLUDE_CUSTOMTEST_H_

#include <string.h>
#include <stdio.h>
#include "../include/out_inputdata.h"

Data Record_got_data;
Data Record_expected_data;

void write_to_file(FILE *ofPTR);

void read_from_file(FILE *ofPTR);


#endif  // PROJECT_INCLUDE_CUSTOMTEST_H_
