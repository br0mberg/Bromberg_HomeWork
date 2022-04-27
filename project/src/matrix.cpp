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
    if (rows == 1) {
        val = values.at(0);
        return val;
    }

    if (rows == 2) {
        val += values.at(0) * values.at(3) - values.at(2) * values.at(1);
        return 0;
    }

    for (size_t i = 0; i < cols; ++i) {
        double ratio = (double) i;
        Matrix matrix_norder(rows - 1, cols - 1);
        for (size_t j = 1, next_j = 0; j < rows; ++j, ++next_j) {
            for (size_t k = 0, next_k = 0; k < cols; ++k, ++next_k) {
                if (k != i) {
                    matrix_norder(next_j, next_k) = values.at(j * cols + k);
                } else {
                    --next_k;
                }
            }
        }
        double res = 0;
        res = det(matrix_norder);
        val += pow(-1, ratio) * values.at(i) * res;
    }
    return val;
}
}  // namespace prep
