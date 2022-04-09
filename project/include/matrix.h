#ifndef PROJECT_INCLUDE_MATRIX_H_
#define PROJECT_INCLUDE_MATRIX_H_

#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>

#define ERROR_DETERMINANT "\nERROR - INCORRECT DET FOR OPERATION\n"
#define ERROR_MEM_MESSAGE "\nERROR WITH MEM FOR MATRIX\n"
#define ERROR_CREATE_MATRIX "\nERROR TO CREATE MATRIX\n"
#define ERROR_MATRIX_IS_NULL "\nERROR - MATRIX IS NULL\n"
#define ERROR_GET_VAL "\nERROR - CAN'T GET VALUE\n"
#define ERROR_SIZE_MATRIX "\nERROR - INCORRECT SIZE MATRIX\n"

#define MATRIX_FIELDS_SIZE_T 2

enum PROGRAM_CONST {
    ERROR = -1,
    FALSE,
    TRUE
};


typedef struct Matrix {
    size_t rows;
    size_t cols;
    double *values;
} Matrix;

// Assist operations
int check_order_matrix(const Matrix* l, const Matrix* r);
int checking_for_squariness(const Matrix* matrix);

// Init/release operations
Matrix* create_matrix_from_file(const char* path_file);
Matrix* create_matrix(size_t rows, size_t cols);
void free_matrix(Matrix* matrix);

// Basic operations
int get_rows(const Matrix* matrix, size_t* rows);
int get_cols(const Matrix* matrix, size_t* cols);
int get_elem(const Matrix* matrix, size_t row, size_t col, double* val);
int set_elem(Matrix* matrix, size_t row, size_t col, double val);


// Math operations
Matrix* mul_scalar(const Matrix* matrix, double val);
Matrix* transp(const Matrix* matrix);

Matrix* sum(const Matrix* l, const Matrix* r);
Matrix* sub(const Matrix* l, const Matrix* r);
Matrix* mul(const Matrix* l, const Matrix* r);

// Extra operations
int det(const Matrix* matrix, double* val);
Matrix* adj(const Matrix* matrix);
Matrix* inv(const Matrix* matrix);

#endif  // PROJECT_INCLUDE_MATRIX_H_
