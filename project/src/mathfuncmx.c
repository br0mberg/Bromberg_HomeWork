#include <matrix.h>

Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (!matrix) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
        return NULL;
    }

    size_t cols = matrix->cols;
    size_t rows = matrix->rows;
    Matrix* result_matrix = create_matrix(rows, cols);
    if (!result_matrix) {
        fprintf(stderr, ERROR_CREATE_MATRIX);
        return NULL;
    }

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            double elem = 0;
            get_elem(matrix, i, j, &elem);
            set_elem(result_matrix, i, j, elem * val);
        }
    }
    return result_matrix;
}

Matrix* transp(const Matrix* matrix) {
    if (!matrix) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
        return NULL;
    }

    size_t rows = matrix->rows;
    size_t cols = matrix->cols;
    Matrix* trans_matrix = create_matrix(cols, rows);

    if (!trans_matrix) {
        fprintf(stderr, ERROR_CREATE_MATRIX);
        return NULL;
    }
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            double elem = 0;
            get_elem(matrix, i, j, &elem);
            set_elem(trans_matrix, j, i, elem);
        }
    }
    return trans_matrix;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
    if (!l) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
        return NULL;
    }

    if (!r) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
        return NULL;
    }

    if (l->rows != r->rows || l->cols != r->cols) {
        fprintf(stderr, ERROR_SIZE_MATRIX);
        return NULL;
    }

    Matrix* sum_matrix = create_matrix(r->rows, r->cols);
    if (!sum_matrix) {
        fprintf(stderr, ERROR_CREATE_MATRIX);
        return NULL;
    }

    for (size_t i = 0; i < r->rows; ++i) {
        for (size_t j = 0; j < r->cols; ++j) {
            double a = 0;
            double b = 0;
            get_elem(l, i, j, &a);
            get_elem(r, i, j, &b);
            set_elem(sum_matrix, i, j, a + b);
        }
    }
    return sum_matrix;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
    if (!l) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
        return NULL;
    }

    if (!r) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
        return NULL;
    }

    if (l->rows != r->rows || l->cols != r->cols) {
        fprintf(stderr, ERROR_SIZE_MATRIX);
        return NULL;
    }

    Matrix* sub_matrix = create_matrix(r->rows, r->cols);
    if (!sub_matrix) {
        fprintf(stderr, ERROR_CREATE_MATRIX);
        return NULL;
    }

    for (size_t i = 0; i < r->rows; ++i) {
        for (size_t j = 0; j < r->cols; ++j) {
            double a = 0;
            double b = 0;
            get_elem(l, i, j, &a);
            get_elem(r, i, j, &b);
            set_elem(sub_matrix, i, j, a - b);
            // for sum sub this 3 string on another func
        }
    }
    return sub_matrix;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
    if (!l) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
        return NULL;
    }

    if (!r) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
        return NULL;
    }

    if (l->cols != r->rows) {
        fprintf(stderr, ERROR_SIZE_MATRIX);
        return NULL;
    }

    Matrix* mul_matrix = create_matrix(l->rows, r->cols);
    if (!mul_matrix) {
        fprintf(stderr, ERROR_CREATE_MATRIX);
        return NULL;
    }

    double sum_els = 0;
    double lval = 0;
    double rval = 0;
    for (size_t i = 0; i < l->rows; ++i) {
        for (size_t j = 0; j < r->cols; ++j) {
            for (size_t k = 0; k < l->cols; k++) {
                get_elem(l, i, k, &lval);
                get_elem(r, k, j, &rval);
                sum_els += lval * rval;
            }
            set_elem(mul_matrix, i, j, sum_els);
            sum_els = 0;
        }
    }
    return mul_matrix;
}
