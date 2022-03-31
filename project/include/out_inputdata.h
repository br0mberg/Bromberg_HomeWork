#ifndef PROJECT_INCLUDE_OUT_INPUTDATA_H_
#define PROJECT_INCLUDE_OUT_INPUTDATA_H_

#include "../include/structs.h"

void output_data(int outmode, FILE* file, Data data);

void read_from_file(FILE* got_data);

void write_to_file(FILE* expected_data);

#endif  // PROJECT_INCLUDE_OUT_INPUTDATA_H_
