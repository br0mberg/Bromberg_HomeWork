#include "matrix.h"

Matrix* create_matrix_from_file(const char* path_file) {
    FILE* matrixfile = fopen(path_file, "r+");
    if (!matrixfile) {
        fprintf(stderr, "ERROR - FILE WITH MATRIX IS NULL");
        return NULL;
    }
    size_t cols = 0;
    size_t rows = 0;
    if (fscanf(matrixfile, "%zu %zu", &rows, &cols) != MATRIX_FIELDS_SIZE_T) {
        fprintf(stderr, "ERROR READING COLS AND ROWS FROM FILE");
        fclose(matrixfile);
        return NULL;
    }

    Matrix* matrix = create_matrix(rows, cols);
    if (!matrix) {
        fprintf(stderr, "ERROR CALLOC FOR MATRIX");
        fclose(matrixfile);
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            double val = 0;
            if (fscanf(matrixfile, "%lf", &val) != 1) {
                fprintf(stderr, "ERROR SCAN VAL FROM FILE");
                fclose(matrixfile);
                free_matrix(matrix);
                return NULL;
            }
            set_elem(matrix, i, j, val);
        }
    }

    if (fclose(matrixfile)) {
        fprintf(stderr, "ERROR CLOSE FILE");
        return NULL;
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
    *rows = matrix->rows;
    return 0;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    *cols = matrix->cols;
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
            set_elem(result_matrix, i, j, elem * val);
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
    if (!check_order_matrix(l, r)) {
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
    if (!check_order_matrix(l, r)) {
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
    if (!check_order_matrix(l, r)) {
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
    if (matrix->rows != matrix->cols) {
        printf("MATRIX IS NOT QUADRATIC");
        return ERROR;
    }

    if (matrix->rows == 1) {
        *val = matrix->values[0];
    }

    if (matrix->rows == 2) {
        *val = matrix->values[0] * matrix->values[3] - matrix->values[2] * matrix->values[1];
        return 0;
    }

    for (size_t i = 0; i < matrix->cols; ++i) {
        double ratio = (double) i;

        Matrix* matrix_norder = create_matrix(matrix->rows - 1, matrix->cols - 1);
        for (size_t j = 1; j < matrix->rows; ++j) {
            for (size_t k = 0; k < matrix->cols; ++k) {
                if (k != i) {
                    double elem = 0;
                    get_elem(matrix, j, k, &elem);
                    set_elem(matrix_norder, j, k, elem);
                }
            }
        }
        det(matrix_norder, val);
        *val += pow(-1, ratio) * matrix->values[i] * det(matrix_norder, val);
        free(matrix_norder);
    }
    return 0;
}

Matrix* adj(const Matrix* matrix) {
    size_t rows = matrix->rows;
    size_t cols = matrix->cols;
    Matrix* adj_matrix = create_matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            Matrix* temp_matrix = create_matrix(rows - 1, cols - 1);
            for (size_t k = 0; k < rows - 1; ++k) {
                for (size_t n = 0; n < cols - 1; ++n) {
                    if (k != i && n != j) {
                        double elem = 0;
                        get_elem(matrix, k, n, &elem);
                        set_elem(temp_matrix, k, n, elem);
                    }
                }
            }
            double minor = 0;
            det(temp_matrix, &minor);
            set_elem(adj_matrix, i, j, minor);
            free(temp_matrix);
        }
    }
    return adj_matrix;
}

Matrix* inv(const Matrix* matrix) {
    double determinant = 0;
    det(matrix, &determinant);
    Matrix* inv_matrix = create_matrix(matrix->rows, matrix->cols);
    if (!determinant) {
        fprintf(stderr, "DET MATRIX == 0\n It is impossible to find inv-matrix");
        return inv_matrix;
    }
    Matrix* adj_matrix = adj(matrix);
    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            double temp = 0;
            get_elem(adj_matrix, i, j, &temp);
            temp /= determinant;
            set_elem(inv_matrix, i, j, temp);
        }
    }
    free(adj_matrix);
    return inv_matrix;
}

void free_matrix(Matrix *matrix) {
    if (!matrix) {
        fprintf(stderr, "Matrix is null");
    }
    free(matrix->values);
    free(matrix);
}

// Вынести отдельно проверку на квадратность матрицы
