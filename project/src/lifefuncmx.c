#include <matrix.h>

Matrix *create_matrix(size_t rows, size_t cols) {
    Matrix *matrix = malloc(sizeof(Matrix));
    if (!matrix) {
        fprintf(stderr, ERROR_MEM_MESSAGE);
        free(matrix);
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;
    if (rows == 0 || cols == 0) {
        fprintf(stderr, ERROR_MEM_MESSAGE);
        free(matrix);
        return NULL;
    }

    matrix->values = calloc(1, rows * cols * sizeof(double));
    if (!matrix->values) {
        fprintf(stderr, ERROR_MEM_MESSAGE);
        free(matrix);
        return NULL;
    }

    return matrix;
}

Matrix* create_matrix_from_file(const char* path_file) {
    FILE* matrixfile = fopen(path_file, "r+");
    if (!matrixfile) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
        return NULL;
    }

    size_t cols = 0;
    size_t rows = 0;
    if (fscanf(matrixfile, "%zu %zu", &rows, &cols) != MATRIX_FIELDS_SIZE_T) {
        fprintf(stderr, ERROR_GET_VAL);
        fclose(matrixfile);
        return NULL;
    }

    Matrix* matrix = create_matrix(rows, cols);
    if (!matrix) {
        fprintf(stderr, ERROR_MEM_MESSAGE);
        fclose(matrixfile);
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            double val = 0;
            if (fscanf(matrixfile, "%lf", &val) != 1) {
                fprintf(stderr, ERROR_GET_VAL);
                fclose(matrixfile);
                free_matrix(matrix);
                return NULL;
            }
            set_elem(matrix, i, j, val);
        }
    }

    if (fclose(matrixfile)) {
        fprintf(stderr, "ERROR CLOSE FILE");
        free_matrix(matrix);
        return NULL;
    }
    return matrix;
}

void free_matrix(Matrix *matrix) {
    if (!matrix) {
        fprintf(stderr, ERROR_MATRIX_IS_NULL);
    } else {
        free(matrix->values);
        free(matrix);
    }
}
