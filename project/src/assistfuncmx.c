#include <matrix.h>

int checking_for_squariness(const Matrix* matrix) {
    if (matrix->rows != matrix->cols) {
        fprintf(stderr, ERROR_SIZE_MATRIX);
        return ERROR;
    }
    return 0;
}

int check_order_matrix(const Matrix* l, const Matrix* r) {
    if (l->rows == r->rows && l->cols == r->cols) {
        return TRUE;
    }
    return FALSE;
}
