#include "matrix.h"
#include "exceptions.h"

namespace prep {
Matrix::Matrix(size_t rows, size_t cols): rows(rows), cols(cols) {
    values = new double[rows * cols];
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            values[i * cols + j] = 0;
        }
    }
}

Matrix::Matrix(const Matrix& rhs): rows(rhs.rows), cols(rhs.cols)  {
    values = new double[rows * cols];

    for (size_t i = 0; i < rows * cols; i++) {
        values[i] = rhs.values[i];
    }
}

Matrix& Matrix::operator=(const Matrix& rhs) {
    if (this == &rhs) {
        return (*this);
    }

    if (rows > 0 || cols > 0) {
        delete [] values;
    }

    rows = rhs.rows;
    cols = rhs.cols;

    values = new double[rows * cols];

    for (size_t i = 0; i < rows * cols; i++) {
        values[i] = rhs.values[i];
    }
    return *this;
}

Matrix::Matrix(std::istream& is) {
    is >> rows;
    is >> cols;
    values = new double[rows * cols];
    if (!is) {
        throw InvalidMatrixStream();
    }
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            is >> values[i * cols + j];
            if (!is) {
                throw InvalidMatrixStream();
            }
        }
    }
}

size_t Matrix::getRows() const {
    return (*this).rows;
}

size_t Matrix::getCols() const {
    return (*this).cols;
}

double Matrix::operator()(size_t i, size_t j) const {
    if (i >= rows || j >= cols) {
        throw OutOfRange(i, j, *this);
    }
    return values[i * cols + j];
}

double& Matrix::operator()(size_t i, size_t j) {
    if (i >= rows || j >= cols) {
        throw OutOfRange(i, j, *this);
    }
    return values[i * cols + j];
}

bool Matrix::operator==(const Matrix& rhs) const {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (std::abs(values[i*cols + j] - rhs(i, j)) > std::numeric_limits<float>::epsilon()) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& rhs) const {
    return !(*this == rhs);
}

Matrix Matrix::operator+(const Matrix& rhs) const {
    if (cols != rhs.getCols() || rows != rhs.getRows()) {
        throw DimensionMismatch(rhs);
    }

    Matrix new_matrix = Matrix(rows, cols);

    for (size_t i = 0; i < new_matrix.getRows(); ++i) {
        for (size_t j = 0; j < new_matrix.getCols(); ++j) {
            new_matrix(i, j) = values[i * cols + j] + rhs(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::operator-(const Matrix& rhs) const {
    if (cols != rhs.getCols() || rows != rhs.getRows()) {
        throw DimensionMismatch(rhs);
    }

    Matrix new_matrix = Matrix(rows, cols);

    for (size_t i = 0; i < new_matrix.getRows(); ++i) {
        for (size_t j = 0; j < new_matrix.getCols(); ++j) {
            new_matrix(i, j) = values[i * cols + j] - rhs(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::operator*(double val) const {
    Matrix new_matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            new_matrix(i, j) = values[i * cols + j] * val;
        }
    }
    return new_matrix;
}

Matrix operator*(double val, const Matrix& matrix) {
    size_t rows = matrix.getRows();
    size_t cols = matrix.getCols();
    Matrix new_matrix = Matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            new_matrix(i, j) = matrix(i, j) * val;
        }
    }
    return new_matrix;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    os << std::setprecision(std::numeric_limits<double>::max_digits10);
    os << matrix.getRows() << " " << matrix.getCols() << std::endl;
    size_t rows = matrix.getRows();
    size_t cols = matrix.getCols();
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            os << matrix(i, j) << " ";
        }
        os << std::endl;
    }
    return os;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
    if (cols != rhs.getRows()) {
        throw DimensionMismatch(rhs);
    }

    Matrix new_matrix = Matrix(rows, rhs.getCols());
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < rhs.getCols(); ++j) {
            for (size_t k = 0; k < cols; ++k) {
                new_matrix(i, j) += rhs(k, j) * (*this)(i, k);
            }
        }
    }
    return new_matrix;
}

Matrix Matrix::transp() const {
    Matrix transp_matrix = Matrix(cols, rows);

    for (size_t i = 0; i < transp_matrix.getRows(); ++i) {
        for (size_t j = 0; j < transp_matrix.getCols(); ++j) {
            transp_matrix(i, j) = values[j * transp_matrix.getRows() + i];
        }
    }
    return transp_matrix;
}

double Matrix::det() const {
    double val = 0;
    if ((*this).getCols() != rows || (*this).getRows() != cols) {
        throw DimensionMismatch((*this));
    }

    if (this->rows == 1) {
        val = this->values[0];
        return val;
    }

    if (this->rows == 2) {
        val += this->values[0] * this->values[3] - this->values[2] * this->values[1];
        return val;
    }

    for (size_t i = 0; i < cols; ++i) {
        double ratio = (double) i;
        Matrix matrix_norder(this->rows - 1, this->cols - 1);
        for (size_t j = 1, next_j = 0; j < this->rows; ++j, ++next_j) {
            for (size_t k = 0, next_k = 0; k < this->cols; ++k, ++next_k) {
                if (k != i) {
                    matrix_norder(next_j, next_k) = this->values[j * this->cols + k];
                } else {
                    --next_k;
                }
            }
        }
        val += pow(-1, ratio) * values[i] * matrix_norder.det();
    }
    return val;
}

Matrix Matrix::adj() const {
    if (rows != cols) {
        throw DimensionMismatch((*this));
    }

    if (!(*this).det()) {
        fprintf(stderr, "ERROR_DETERMINANT");
        throw false;
    }

    Matrix adj_matrix = Matrix(rows, cols);

    if (rows == 1) {
        adj_matrix(rows - 1, cols - 1) = 1;
        return adj_matrix;
    }
    Matrix transp_matrix = (*this).transp();
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
    if (!(*this).det()) {
        throw SingularMatrix();
    }

    if (rows != cols) {
        throw DimensionMismatch(*this);
    }
    Matrix inv_matrix = Matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            inv_matrix(i, j) = values[i * inv_matrix.getCols() + j];
        }
    }
    Matrix adj_matrix = inv_matrix.adj();
    double determinate = 0;
    determinate = inv_matrix.det();
    if (determinate == 0) {
        throw SingularMatrix();
    }
    Matrix res = adj_matrix.operator*(1. / determinate);
    return res;
}

Matrix::~Matrix() {
    delete [] values;
}
}  // namespace prep
