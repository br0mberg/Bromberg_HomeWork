#include "matrix.h"
#include "exceptions.h"

namespace prep {
Matrix::Matrix(size_t rows, size_t cols) {
    this->rows = rows;
    this->cols = cols;
    values.reserve(rows*cols);
}

Matrix::Matrix(std::istream& is) {
    is >> rows;
    is >> cols;
    double val = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            is >> val;
            values.push_back(val);
        }
    }
}

size_t Matrix::getRows() const {
    return rows;
}

size_t Matrix::getCols() const {
    return cols;
}

double Matrix::operator()(size_t i, size_t j) const {
    return values[i * cols + j];
}

double& Matrix::operator()(size_t i, size_t j) {
    return values[i * cols + j];
}

bool Matrix::operator==(const Matrix& rhs) const {
    bool res = true;
    if (cols != rhs.getCols() || rows != rhs.getRows()) {
        return false;
    }

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (values.at(i*cols + j) != rhs(i, j)) {
                res = false;
            }
        }
    }
    return res;
}

bool Matrix::operator!=(const Matrix& rhs) const {
    bool res = true;
    if (cols != rhs.getCols() || rows != rhs.getRows()) {
        throw false;
    }

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (values.at(i*cols + j) == rhs(i, j)) {
                res = false;
            }
        }
    }
    return res;
}

Matrix Matrix::operator+(const Matrix& rhs) const {
    if (cols != rhs.getCols() || rows != rhs.getRows()) {
        throw false;
    }

    Matrix new_matrix(rows, cols);

    for (size_t i = 0; i < new_matrix.getRows(); ++i) {
        for (size_t j = 0; j < new_matrix.getCols(); ++j) {
            new_matrix(i, j) = values.at(i * cols + j) + rhs(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::operator-(const Matrix& rhs) const {
    if (cols != rhs.getCols() || rows != rhs.getRows()) {
        throw false;
    }

    Matrix new_matrix(rows, cols);

    for (size_t i = 0; i < new_matrix.getRows(); ++i) {
        for (size_t j = 0; j < new_matrix.getCols(); ++j) {
            new_matrix(i, j) = values.at(i * cols + j) - rhs(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::operator*(double val) const {
    Matrix new_matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            new_matrix(i, j) = values.at(i * cols + j) * val;
        }
    }
    return new_matrix;
}

Matrix operator*(double val, const Matrix& matrix) {
    size_t rows = matrix.getRows();
    size_t cols = matrix.getCols();
    Matrix new_matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            new_matrix(i, j) = matrix(i, j) * val;
        }
    }
    return new_matrix;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    size_t rows = matrix.getRows();
    size_t cols = matrix.getCols();
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            os << matrix(i, j) << " ";
        }
    }
    return os;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
    if (cols != rhs.getCols() || rows != rhs.getRows()) {
        throw false;
    }
    size_t cols = rhs.getCols();
    Matrix new_matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            new_matrix(i, j) = rhs(i, j) * values.at(i * cols + j);
        }
    }
    return new_matrix;
}

Matrix Matrix::transp() const {
    Matrix transp_matrix(cols, rows);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            transp_matrix(i, j) = values.at(i * cols + rows);
        }
    }
    return transp_matrix;
}

double Matrix::det() const {
    double val = 0;
    if (this->rows == 1) {
        val = this->values.at(0);
        return val;
    }

    if (this->rows == 2) {
        val += this->values.at(0) * this->values.at(3) - this->values.at(2) * this->values.at(1);
        return val;
    }

    for (size_t i = 0; i < cols; ++i) {
        double ratio = (double) i;
        Matrix matrix_norder(this->rows - 1, this->cols - 1);
        for (size_t j = 1, next_j = 0; j < this->rows; ++j, ++next_j) {
            for (size_t k = 0, next_k = 0; k < this->cols; ++k, ++next_k) {
                if (k != i) {
                    matrix_norder(next_j, next_k) = this->values.at(j * this->cols + k);
                } else {
                    --next_k;
                }
            }
        }
        double res = 0;
        res = matrix_norder.det();
        val += pow(-1, ratio) * values.at(i) * res;
    }
    return val;
}

Matrix Matrix::adj() const {
    double deter = 0;
    deter = det();

    if (!deter) {
        fprintf(stderr, "ERROR_DETERMINANT");
        throw false;
    }

    Matrix adj_matrix(rows, cols);

    if (rows == 1) {
        adj_matrix(rows - 1, cols - 1) = 1;
        return adj_matrix;
    }

    Matrix transp_matrix = transp();

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            Matrix temp_matrix(rows - 1, cols - 1);
            for (size_t k = 0, k_next = 0; k < rows; ++k, ++k_next) {
                if (k == i) {
                    --k_next;
                } else {
                    for (size_t n = 0, n_next = 0; n < cols; ++n, ++n_next) {
                        if (n != j) {
                            temp_matrix(k_next, n_next) = transp_matrix(k, n);
                        } else {
                            --n_next;
                        }
                    }
                }
            }
            double minor = 0;
            minor = temp_matrix.det();
            minor*=pow(-1, (i + j) % 2);

            adj_matrix(i, j) = minor;
        }
    }
    return adj_matrix;
}

Matrix Matrix::inv() const {
    double determinant = 0;
    determinant = det();
    if (!determinant) {
        fprintf(stderr, "ERROR_DETERMINANT");
        throw false;
    }

    Matrix inv_matrix = adj();

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            double temp = 0;
            temp = inv_matrix(i, j);
            temp *= 1/determinant;
            inv_matrix(i, j) = temp;
        }
    }
    return inv_matrix;
}
}  // namespace prep
