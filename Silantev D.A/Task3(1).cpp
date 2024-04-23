#include <iostream>
#include <cstdlib>
#include <cstring>




class Matrix_Exception : public std::domain_error
{
public:
    Matrix_Exception(const char* const message) : std::domain_error(message) {}
};


class Matrix
{
private:
    size_t cols;
    size_t rows;
    double* value;

public:
    Matrix();
    Matrix(size_t col, size_t row);
    Matrix(const Matrix& matrix);
    Matrix(Matrix&& matrix);
    ~Matrix(); 

    void print();
    void fill_random(int max_value);
    Matrix operator+(const Matrix& matrix) const;
    Matrix operator-(const Matrix& matrix) const;
    Matrix operator*(const Matrix& matrix) const;
    Matrix operator*(double number) const;
    Matrix& operator=(Matrix& matrix);
    Matrix& operator=(Matrix&& matrix);
    Matrix operator^(size_t number) const;
    Matrix operator/(const double number) const;
    Matrix exp(const size_t accuracy);
    Matrix transp();
    void set_one();


};


Matrix::~Matrix() { 
    delete[] value;
    cols = 0;
    rows = 0;
    value = nullptr;
}


Matrix::Matrix() { 
    cols = 0;
    rows = 0;
    value = nullptr;
}


Matrix::Matrix(size_t col, size_t row) {
    cols = col;
    rows = row;
    if (cols == 0 || rows == 0) {
        value = nullptr;
        return;
    }

    if (rows > SIZE_MAX / cols / sizeof(double) ) throw Matrix_Exception("Matrix overflow\n");
    value = new double[cols * rows * sizeof(double)];
}


Matrix::Matrix(const Matrix& matrix) {
    cols = matrix.cols;
    rows = matrix.rows;
    if (cols == 0 || rows == 0) {
        value = nullptr;
        return;
    }

    value = new double[rows * cols];
    memcpy(value, matrix.value, rows * cols * sizeof(double));
}


Matrix::Matrix(Matrix&& matrix) { 
    cols = matrix.cols;
    rows = matrix.rows;
    value = matrix.value;
    matrix.value = nullptr;
    matrix.cols = 0;
    matrix.rows = 0;
}


void Matrix::print() {
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            std::cout << value[row * cols + col] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


void Matrix::fill_random(int max_value = 10) {
    for (size_t index = 0; index < rows * cols; ++index) {
        value[index] = rand() % max_value;
    }
}


void Matrix::set_one()
{
    for (size_t row = 0; row < this->rows; row++) { 
        for (size_t col = 0; col < this->cols; col++) {
            value[row * this->cols + col] = (row == col) ? 1.0 : 0.0;
        }
    }
}


Matrix Matrix::operator+ (const Matrix& matrix) const {

    if (rows != matrix.rows || cols != matrix.cols) throw Matrix_Exception("Make matrix square\n");
    Matrix result(matrix);

    for (size_t idx = 0; idx < matrix.cols * matrix.rows; idx++) {
        result.value[idx] += value[idx];
    }
    return result;
}


Matrix Matrix::operator- (const Matrix& matrix) const {
    if (rows != matrix.rows || cols != matrix.cols) throw Matrix_Exception("Make matrix square\n");
    Matrix result(matrix);

    for (size_t idx = 0; idx < matrix.cols * matrix.rows; idx++) {
        result.value[idx] -= value[idx];
    }
    return result;
}


Matrix Matrix::operator* (const Matrix& matrix) const {
    if (rows != matrix.rows || cols != matrix.cols) throw Matrix_Exception("Make matrix square\n");
    Matrix result(matrix);

    for (size_t row = 0; row < result.rows; row++) {
        for (size_t col = 0; col < result.cols; col++) {
            result.value[row * result.rows + col] = 0.00;
            for (size_t k = 0; k < result.cols; k++) {
                result.value[row * result.cols + col] += value[row * cols + k] * matrix.value[k * result.cols + col];
            }
        }
    }
    return result;
}


Matrix Matrix::operator* (const double coefficient) const {
    Matrix result(*this);

    for (size_t idx = 0; idx < rows * cols; idx++) {
        result.value[idx] = value[idx] * coefficient;
    }
    return result;
}


Matrix& Matrix::operator= (Matrix& matrix) {
    if (this == &matrix) {
        return *this;
    }
    rows = matrix.rows;
    cols = matrix.cols;
    if (cols == 0 || rows == 0) {
        value = nullptr;
        return;
    }
    if (rows * cols == matrix.rows * matrix.cols) {
        memcpy(value, matrix.value, rows * cols * sizeof(double));
        return *this;
    }
    delete[]value;
    value = new double[cols * rows];
    memcpy(value, matrix.value, rows * cols * sizeof(double));
    return *this;
}


Matrix& Matrix::operator= (Matrix&& matrix) {
    rows = matrix.rows;
    cols = matrix.cols;
    delete[]value;
    value = matrix.value;
    matrix.value = nullptr;
    return *this;
}


Matrix Matrix::operator^ (size_t number) const {
    if (cols != rows) throw Matrix_Exception("Make matrix square\n");
    Matrix result(*this);

    if (number == 0) {
        result.set_one();
        return result;
    }

    if (number == 1) {
        return result;
    }

    for (size_t idx = 0; idx < number; idx++) {
        result = result * *this;
    }
    return result;
}


Matrix Matrix::operator/(const double number) const {
    if (number == 0) throw Matrix_Exception("Can't divide by zero\n");
    Matrix result(*this);

    for (size_t idx = 0; idx < rows * cols; ++idx) {
        result.value[idx] = value[idx] / number;
    }
    return result;
}


Matrix Matrix::exp(const size_t accuracy = 30) {
    if (this->rows != this->cols) throw Matrix_Exception("Make matrix square\n");
    
    Matrix result(this->rows, this->cols);
    Matrix term(cols, rows);
    term.set_one();
    result = term;

    for (int step = 1; step < accuracy; step++) {
        term = term * *this / step;
        result = result + term;
    }
    return result;
}


Matrix Matrix::transp() {
    Matrix result(this->rows, this->cols);

    for (size_t row = 0; row < result.rows; row++) {
        for (size_t col = 0; col < result.cols; col++) {
            result.value[row * result.cols + col] = value[col * result.cols + row];
        }
    }
    return result;
}


int main() {
    Matrix matrix1(3, 3);
    matrix1.fill_random();
    matrix1.print();
    Matrix matrix2(3, 3);
    matrix2.fill_random();
    matrix2.print();
    Matrix sum = matrix1 + matrix2;
    sum.print();
    Matrix sub = matrix1 - matrix2;
    sub.print();
    Matrix mult = matrix1 * matrix2;
    mult.print();
    Matrix mult_number = matrix1 * 2;
    mult_number.print();
    Matrix pow = matrix1 ^ 2;
    pow.print();
    matrix1.exp(5);
    matrix1.print();
    matrix1.transp();
    matrix1.print();
    return 0;
}  