#ifndef PROJECT_INCLUDE_CUSTOMTEST_H_
#define PROJECT_INCLUDE_CUSTOMTEST_H_

#include <string.h>
#include <stdio.h>
#include "out_inputdata.h"

void write_to_file(FILE *source, CreditData *data);

void read_from_file(FILE *source, CreditData *data);


#endif  // PROJECT_INCLUDE_CUSTOMTEST_H_
