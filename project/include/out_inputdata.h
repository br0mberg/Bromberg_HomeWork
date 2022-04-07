#ifndef PROJECT_INCLUDE_OUT_INPUTDATA_H_
#define PROJECT_INCLUDE_OUT_INPUTDATA_H_

#include "structs.h"
#include <string.h>

int output_data_client(FILE* file, credit_data_t *data);

int output_data_transaction(FILE *file, credit_data_t *data);

int input_data_to_file(FILE* file, credit_data_t *client);

int input_data(credit_data_t *client);

#endif  // PROJECT_INCLUDE_OUT_INPUTDATA_H_
