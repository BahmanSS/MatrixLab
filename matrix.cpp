#include "matrix.hpp"

Matrix::Matrix(std::vector<std::vector<Rational>> data_) {
    rows = data_.size();
    cols = data_[0].size();

    for (int i = 1; i < rows; ++i) {
        if (data_[i].size() != cols) {
            throw std::invalid_argument("Error with format!");
        }
    }
    data = data_;
}

Matrix::Matrix(size_t rows_, size_t cols_, const Rational& el): rows(rows_), cols(cols_) {
    std::vector<Rational> row(cols_, el);
    data = std::vector<std::vector<Rational>>(rows, row);
}

Matrix Matrix::unit(size_t n) {
    Matrix result(n,n,Rational(0));
    for (int i = 0; i < n; ++i) {
        result.data[i][i] = Rational(1);
    }
    return result;
}
Matrix Matrix::linspace(const Rational& x_0, const Rational& x_1, const Rational& n) {
    if (n.to_int() < 1) {
        throw std::invalid_argument(" Can not do linspace for this n");
    }
    Rational jump = (x_1 - x_0 + Rational(1)) / n;
    std::vector<std::vector<Rational>> result_data(1);
    for (int i = 0; i < n.to_int(); ++i) {
        result_data[0].push_back(x_0 + Rational(i) * jump);
    }
    Matrix result(result_data);
    return result;
}
std::string Matrix::to_string() const {
    if (rows == 1 && cols == 1) {
        return data[0][0].to_string();
    }
    
    std::vector<size_t> max_widths(cols, 0);
    
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            size_t width = data[i][j].to_string().size();
            if (width > max_widths[j]) {
                max_widths[j] = width;
            }
        }
    }
    std::string result = "[\n";
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::string elem_str = data[i][j].to_string();
            size_t width = elem_str.size();
            
            size_t spaces_needed = max_widths[j] - width;
            for (size_t k = 0; k < spaces_needed; ++k) {
                result += ' ';
            }
            
            result += elem_str;
            if (j < cols - 1) {
                result += ' ';
            }
        }
        if (i < rows - 1) {
            result += '\n';
        }
    }
    
    result += "\n]";
    return result;
}

bool Matrix::operator==(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        return false;
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("different sizes of matrixs!");
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] += other.data[i][j];
        }
    }
    return *this;
}
Matrix& Matrix::operator-=(const Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("different sizes of matrixs!");
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] -= other.data[i][j];
        }
    }
    return *this;
}
Matrix& Matrix::operator*=(const Matrix& other) {
    if (cols != other.rows) {
        if (rows == 1 && cols == 1) {
            // случай с умножением числа на матрицу
            Matrix result(other.rows, other.cols, Rational(0));
            for (int i = 0; i < other.rows; ++i) {
                for (int j = 0; j < other.cols; ++j) {
                    result.data[i][j] = other.data[i][j] * data[0][0];
                }
            }
            
            *this = result;
            return *this;
        }
        if (other.rows == 1 && other.cols == 1){
            Matrix result(rows, cols, Rational(0));

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result.data[i][j] = other.data[0][0] * data[i][j];
                }
            }
            
            *this = result;
            return *this;
        } 
        throw std::invalid_argument("not valiable sizes of matrixs for multiplication!");
        
    }
    
    Matrix result(rows, other.cols, Rational(0));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            result.data[i][j] = Rational(0);
            for (int k = 0; k < cols; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    
    *this = result;
    return *this;
}

Matrix Matrix::inverse() const {
    if (rows != cols) {
        throw std::invalid_argument("Matrix must be square for inversion!");
    }

    size_t n = rows;
    Matrix augmented(n, 2 * n, Rational(0));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented.data[i][j] = data[i][j];
        }
    }
    
    for (int i = 0; i < n; ++i) {
        augmented.data[i][n + i] = Rational(1);
    }
    
    for (int i = 0; i < n; ++i) {

        int pivot = i;
        while (pivot < n && augmented.data[pivot][i] == Rational(0)) {
            ++pivot;
        }
        
        if (pivot == n) {
            throw std::invalid_argument("Matrix is singular, cannot be inverted!");
        }
        if (pivot != i) {
            for (int j = 0; j < 2 * n; ++j) {
                std::swap(augmented.data[i][j], augmented.data[pivot][j]);
            }
        }
        
        Rational divisor = augmented.data[i][i];
        for (int j = 0; j < 2 * n; ++j) {
            augmented.data[i][j] /= divisor;
        }
        
        for (int k = 0; k < n; ++k) {
            if (k != i) {
                Rational factor = augmented.data[k][i];
                for (int j = 0; j < 2 * n; ++j) {
                    augmented.data[k][j] -= factor * augmented.data[i][j];
                }
            }
        }
    }
    
    Matrix inverseMatrix(n, n, Rational(0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverseMatrix.data[i][j] = augmented.data[i][n + j];
        }
    }
    
    return inverseMatrix;
}

Matrix& Matrix::operator/=(const Matrix& other) {
    if (cols != other.rows) {
        throw std::invalid_argument("Invalid matrix sizes for division!");
    }
    Matrix inverseOther = other.inverse();
    *this *= inverseOther;
    return *this;
}

Matrix& Matrix::elemental_multiplication(const Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices must have the same size for elemental multiplication!");
    }
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] *= other.data[i][j];
        }
    }
    
    return *this;
}

Matrix& Matrix::elemental_division(const Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices must have the same size for elemental division!");
    }
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] /= other.data[i][j];
        }
    }
    
    return *this;
}

Matrix& Matrix::min(const Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("matrixs must have one size");
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = std::min(data[i][j], other.data[i][j]);
        }
    }
    return *this;
}
Matrix& Matrix::max(const Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("matrixs must have one size");
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = std::max(data[i][j], other.data[i][j]);
        }
    }
    return *this;
}

Matrix Matrix::operator-() const {
    Matrix result(rows, cols, Rational(0));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[i][j] = -data[i][j];
        }
    }
    
    return result;
}

Matrix Matrix::transpose() const {
    Matrix result(cols, rows, Rational(0));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            
            result.data[j][i] = data[i][j];
        }
    }
    
    return result;
}

Rational Matrix::sum() const {
    Rational total(0);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            total = total + data[i][j];
        }
    }
    return total;
}
Rational Matrix::product() const {
    if (rows == 0 || cols == 0) {
        return Rational(0);
    }
    
    Rational total(1);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            total = total * data[i][j];
        }
    }
    return total;
}

Rational Matrix::determinant() const {
    if (rows != cols) {
        throw std::invalid_argument("Determinant is defined only for square matrices");
    }
    
    std::vector<std::vector<Rational>> m = data;
    Rational det(1);
    //метод Гаусса
    for (int i = 0; i < rows; ++i) {
        int pivot = i;
        while (pivot < rows && m[pivot][i] == Rational(0)) {
            ++pivot;
        }
        
        if (pivot == rows) {
            return Rational(0);
        }
        
        if (pivot != i) {
            std::swap(m[i], m[pivot]);
            det = -det;
        }
        
        Rational pivot_val = m[i][i];
        det = det * pivot_val;
        
        if (pivot_val != Rational(1)) {
            for (int j = i + 1; j < cols; ++j) {
                m[i][j] = m[i][j] / pivot_val;
            }
            m[i][i] = Rational(1);
        }
        
        for (int k = i + 1; k < rows; ++k) {
            Rational factor = m[k][i];
            if (factor != Rational(0)) {
                for (int j = i + 1; j < cols; ++j) {
                    m[k][j] = m[k][j] - factor * m[i][j];
                }
                m[k][i] = Rational(0);
            }
        }
    }
    
    return det;
}


Matrix operator+(const Matrix& a, const Matrix& b) {
    Matrix tmp = a;
    tmp += b;
    return tmp;
}
Matrix operator-(const Matrix& a, const Matrix& b) {
    Matrix tmp = a;
    tmp -= b;
    return tmp;
}
Matrix operator*(const Matrix& a, const Matrix& b) {
    Matrix tmp = a;
    tmp *= b;
    return tmp;
}
Matrix operator/(const Matrix& a, const Matrix& b) {
    Matrix tmp = a;
    tmp /= b;
    return tmp;
}
Matrix elemental_multiplication(const Matrix& a, const Matrix& b) {
    Matrix tmp = a;
    tmp.elemental_multiplication(b);
    return tmp;
}
Matrix elemental_division(const Matrix& a, const Matrix& b) {
    Matrix tmp = a;
    tmp.elemental_division(b);
    return tmp;
}

Matrix min(const Matrix& a, const Matrix& b) {
    Matrix tmp = a;
    tmp.min(b);
    return tmp;
}
Matrix max(const Matrix& a, const Matrix& b) {
    Matrix tmp = a;
    tmp.max(b);
    return tmp;
}

Matrix& Matrix::horzcat(const Matrix& other) {
    if (rows != other.rows) {
        throw std::invalid_argument("Number of rows must be equal for horizontal concatenation");
    }
    
    std::vector<std::vector<Rational>> result_data(rows);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result_data[i].push_back(data[i][j]);
        }
        for (int j = 0; j < other.cols; ++j) {
            result_data[i].push_back(other.data[i][j]);
        }
    }
    
    *this = Matrix(result_data);
    return *this;
}
Matrix& Matrix::vertcat(const Matrix& other) {
    if (cols != other.cols) {
        throw std::invalid_argument("Number of columns must be equal for vertical concatenation");
    }
    
    std::vector<std::vector<Rational>> result_data(rows + other.rows);
    
    for (int i = 0; i < rows; ++i) {
        result_data[i] = data[i];
    }
    
    for (int i = 0; i < other.rows; ++i) {
        result_data[i + rows] = other.data[i];
    }
    
    *this = Matrix(result_data);
    return *this;
}

Rational Matrix::to_rational() const {
    if (rows != 1 || cols != 1) {
        throw std::invalid_argument("Can not do rational");
    }
    return data[0][0];
}