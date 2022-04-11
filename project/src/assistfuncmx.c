#include <matrix.h>

int create_minor(const Matrix* matrix, Matrix* transp_matrix, Matrix* temp_matrix, size_t  i, size_t j) {
    for (size_t k = 0, k_next = 0; k < matrix->rows; ++k, ++k_next) {
        if (k == i) {
            --k_next;
        } else {
            for (size_t n = 0, n_next = 0; n < matrix->cols; ++n, ++n_next) {
                if (n != j) {
                    double elem = 0;
                    get_elem(transp_matrix, k, n, &elem);
                    set_elem(temp_matrix, k_next, n_next, elem);
                } else {
                    --n_next;
                }
            }
        }
    }
    return 0;
}
