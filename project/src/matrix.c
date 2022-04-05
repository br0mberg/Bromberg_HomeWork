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

int get_rows(const Matrix* matrix, size_t* rows) {
    rows = matrix->rows;
    return 0;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    cols = matrix->cols;
    return 0;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
    size_t cols = matrix->cols;
    size_t rows = matrix->rows;
    Matrix* result_matrix = create_matrix(rows, cols);
    for (size_t i=0; i < rows; ++i) {
        for (size_t j=0; j < cols; ++j) {
            double elem = 0;
            get_elem(matrix, i, j, &elem);
            set_elem(result_matrix, i, j, elem * val));
        }
    }
    return result_matrix;
}

Matrix* transp(const Matrix* matrix) {
    size_t rows = matrix->rows;
    size_t cols = matrix->cols;
    Matrix* trans_matrix = create_matrix(cols, rows);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            double elem = 0;
            get_elem(matrix, i, j, &elem);
            set_elem(trans_matrix, j, i, elem);
        }
    }
    return trans_matrix;
}

int check_order_matrix(const Matrix* l, const Matrix* r) {
    if (l->rows == r->rows && l->cols == r->cols) {
        return TRUE;
    }
    return FALSE;
}
Matrix* sum(const Matrix* l, const Matrix* r) {
    Matrix* sum_matrix = create_matrix(r->rows, r->cols);
    if (!check_order_matrix(const Matrix* l, const Matrix* r)) {
        printf("MATRICES OF DIFFERENT SIZE");
    } else {
        for (size_t i = 0; i < r->rows; ++i) {
            for (size_t j = 0; j < r->cols; ++j) {
                double a = 0;
                double b = 0;
                get_elem(l, i, j, &a);
                get_elem(r, i, j, &b);
                set_elem(sum_matrix, i, j, a + b);
            }
        }
    }
    return sum_matrix;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
    Matrix* sub_matrix = create_matrix(r->rows, r->cols);
    if (!check_order_matrix(const Matrix* l, const Matrix* r)) {
        printf("MATRICES OF DIFFERENT SIZE");
    } else {
        for (size_t i = 0; i < r->rows; ++i) {
            for (size_t j = 0; j < r->cols; ++j) {
                double a = 0;
                double b = 0;
                get_elem(l, i, j, &a);
                get_elem(r, i, j, &b);
                set_elem(sub_matrix, i, j, a - b);
            }
        }
    }
    return sub_matrix;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
    Matrix* mul_matrix = create_matrix(r->rows, r->cols);
    if (!check_order_matrix(const Matrix* l, const Matrix* r)) {
        printf("MATRICES OF DIFFERENT SIZE");
    } else {
        for (size_t i = 0; i < r->rows; ++i) {
            for (size_t j = 0; j < r->cols; ++j) {
                double a = 0;
                double b = 0;
                get_elem(l, i, j, &a);
                get_elem(r, i, j, &b);
                set_elem(mul_matrix, i, j, a * b);
            }
        }
    }
    return mul_matrix;
}

int det(const Matrix* matrix, double* val) {
    
}

void free_matrix(Matrix *matrix) {
    if (!matrix) {
        fprintf(stderr, "Matrix is null");
    }
    free(matrix->values);
    free(matrix);
}
