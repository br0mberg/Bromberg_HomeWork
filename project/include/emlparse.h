#ifndef PROJECT_INCLUDE_EMLPARSE_H_
#define PROJECT_INCLUDE_EMLPARSE_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR -1

int get_length_str(const char* path_to_eml, size_t* length);
int get_eml_str(const char* path_to_eml, char* buffer, size_t* length);

#endif  // PROJECT_INCLUDE_EMLPARSE_H_
