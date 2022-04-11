#include "matrix.h"

int det(const Matrix* matrix, double* val) {
    if (matrix->rows != matrix->cols) {
        fprintf(stderr, ERROR_SIZE_MATRIX);
        return ERROR;
    }

    if (matrix->rows == 1) {
        *val = matrix->values[0];
        return 0;
    }

    if (matrix->rows == 2) {
        *val += matrix->values[0] * matrix->values[3] - matrix->values[2] * matrix->values[1];
        return 0;
    }

    for (size_t i = 0; i < matrix->cols; ++i) {
        double ratio = (double) i;
        Matrix* matrix_norder = create_matrix(matrix->rows - 1, matrix->cols - 1);
        for (size_t j = 1, next_j = 0; j < matrix->rows; ++j, ++next_j) {
            for (size_t k = 0, next_k = 0; k < matrix->cols; ++k, ++next_k) {
                if (k != i) {
                    double elem = 0;
                    get_elem(matrix, j, k, &elem);
                    set_elem(matrix_norder, next_j, next_k, elem);
                } else {
                    --next_k;
                }
            }
        }
        double res = 0;
        det(matrix_norder, &res);
        *val += pow(-1, ratio) * matrix->values[i] * res;

        free_matrix(matrix_norder);
    }
    return 0;
}

Matrix* adj(const Matrix* matrix) {
    if (matrix->rows != matrix->cols) {
        fprintf(stderr, ERROR_SIZE_MATRIX);
        return NULL;
    }

    double deter = 0;
    det(matrix, &deter);

    if (!deter) {
        fprintf(stderr, ERROR_DETERMINANT);
        return NULL;
    }

    Matrix* adj_matrix = create_matrix(matrix->rows, matrix->cols);
    if (!adj_matrix) {
        fprintf(stderr, ERROR_CREATE_MATRIX);
        return NULL;
    }

    if (matrix->rows == 1) {
        set_elem(adj_matrix, matrix->rows - 1, matrix->cols - 1, 1);
        return adj_matrix;
    }

    Matrix* transp_matrix = transp(matrix);
    if (!transp_matrix) {
        fprintf(stderr, ERROR_CREATE_MATRIX);
        free_matrix(adj_matrix);
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            Matrix* temp_matrix = create_matrix(matrix->rows - 1, matrix->cols - 1);
            create_minor(matrix, transp_matrix, temp_matrix, i, j);

            double minor = 0;
            det(temp_matrix, &minor);
            minor*=pow(-1, (i + j) % 2);

            set_elem(adj_matrix, i, j, minor);
            free_matrix(temp_matrix);
        }
    }
    free_matrix(transp_matrix);
    return adj_matrix;
}

Matrix* inv(const Matrix* matrix) {
    double determinant = 0;
    det(matrix, &determinant);
    if (!determinant) {
        fprintf(stderr, ERROR_DETERMINANT);
        return NULL;
    }

    Matrix* inv_matrix = adj(matrix);
    if (!inv_matrix) {
        fprintf(stderr, "\nERR TO GET ADJ MATRIX\n");
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            double temp = 0;
            get_elem(inv_matrix, i, j, &temp);
            temp *= 1/determinant;
            set_elem(inv_matrix, i, j, temp);
        }
    }
    return inv_matrix;
}
