#include "matrix.h"

Matrix* create_matrix_from_file(const char* path_file) {
    FILE* matrixfile = fopen(path_file, "r+");
    if (!matrixfile) {
        fprintf(stderr, "ERROR - FILE WITH MATRIX IS NULL");
    }

    Matrix* matrix = {0};
    if (fscanf(matrixfile, "%zu %zu", &matrix->rows, &matrix->cols) != MATRIX_FIELDS_SIZE_T) {
        fprintf(stderr, "ERROR READING COLS AND ROWS FROM FILE");
    }

    matrix->values = calloc(1, matrix->rows * matrix->cols * sizeof(double));
    if (!matrix->values) {
        fprintf(stderr, "ERROR CALLOC FOR MATRIX");
    }

    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            double val = 0;
            if (fscanf(matrixfile, "%lf", &val) != 1) {
                fprintf(stderr, "ERROR SCAN VAL FROM FILE");
            }
            set_elem(matrix, i, j, val);
        }
    }

    if (fclose(matrixfile)) {
        fprintf(stderr, "ERROR CLOSE FILE");
    }
    return matrix;
}

Matrix *create_matrix(size_t rows, size_t cols) {
    Matrix *matrix = malloc(sizeof(Matrix));
    if (!matrix) {
        fprintf(stderr, "ERORR MALLOC MEM FOR MATRIX");
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->values = calloc(1, rows * cols * sizeof(double));

    if (!matrix->values) {
        fprintf(stderr, "ERROR CALLOC FOR MATRIX");
    }

    return matrix;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    *(matrix->values + row * matrix->cols + col) = val;  // matrix->rows ? matrix->cols
    return 0;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    *val = *(matrix->values + row * matrix->cols + col);
    return 0;
}

void free_matrix(Matrix *matrix) {
    if (!matrix) {
        fprintf(stderr, "Matrix is null");
    }
    free(matrix->values);
    free(matrix);
}


