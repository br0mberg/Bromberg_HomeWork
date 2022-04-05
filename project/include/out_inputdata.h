#ifndef PROJECT_INCLUDE_OUT_INPUTDATA_H_
#define PROJECT_INCLUDE_OUT_INPUTDATA_H_

#include "structs.h"
#include <string.h>

int output_data_client(FILE* file, CreditData *data);

int output_data_transaction(FILE *file, CreditData *data);

int input_data(FILE* file, CreditData *Client);

#endif  // PROJECT_INCLUDE_OUT_INPUTDATA_H_
