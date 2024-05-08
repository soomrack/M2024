#include <iostream>
#include <cmath>
#include <string>
#include <algorithm> // для std::copy

enum class ErrorType { ERROR };

class Matrix_Exception : public std::exception
{
private:
    std::string message;
public:
    Matrix_Exception(std::string msg) : message { msg } {}
    std::string get_message() const { return message; }
};

class Matrix 
{
private:
    size_t rows;
    size_t cols;
    double* data;

public:
    Matrix();
    Matrix(const size_t rows, const size_t cols, const double* values);
    Matrix(const size_t rows, const size_t cols);
    Matrix(const Matrix& A);
    Matrix(Matrix&& A);
    ~Matrix();

    Matrix& operator=(const Matrix& M);
    Matrix& operator=(Matrix&& M);
    size_t get_rows() const { return rows; }
    size_t get_cols() const { return cols; }

    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;

    Matrix operator+(const Matrix& M) const;
    Matrix operator-(const Matrix& M) const;
    Matrix operator*(double scalar) const;
    Matrix operator*(const Matrix& M) const;
    Matrix exp(size_t iterations) const;
    double determinant() const;

    void print() const;
};


Matrix::Matrix() : rows(0), cols(0), data(nullptr) {}


Matrix::Matrix(const size_t rows, const size_t cols) : rows(rows), cols(cols) {
    if (SIZE_MAX / cols / rows < sizeof(double)) throw Matrix_Exception("Ошибка выделения памяти");

    data = new double[rows * cols];
}


Matrix::Matrix(const size_t rows, const size_t cols, const double* values)
    : rows(rows), cols(cols) {
    if (SIZE_MAX / cols / rows < sizeof(double)) throw Matrix_Exception("Ошибка выделения памяти");
    data = new double[rows * cols];
    std::copy(values, values + (rows * cols), data);
}


Matrix::Matrix(const Matrix& M) : rows(M.rows), cols(M.cols) {
    data = new double[rows * cols];
    std::copy(M.data, M.data + (rows * cols), data);
}


Matrix::Matrix(Matrix&& A) : rows(A.rows), cols(A.cols), data(A.data) {
    A.rows = 0;
    A.cols = 0;
    A.data = nullptr;
}


Matrix::~Matrix() {
    delete[] data;
}


Matrix& Matrix::operator=(const Matrix& M) {
    if (this == &M) return *this;    
    if (data != nullptr) delete[] data;

    rows = M.rows;
    cols = M.cols;
    this->data = new double[rows * cols];
    std::copy(M.data, M.data + (rows * cols), data);

    return *this;
}


Matrix& Matrix::operator=(Matrix&& M) {
    delete[] data;

    rows = M.rows;
    cols = M.cols;
    data = M.data;

    M.rows = 0;
    M.cols = 0;
    M.data = nullptr;
    
    return *this;
}


double& Matrix::operator()(size_t row, size_t col) {
    return data[row * cols + col];
}


const double& Matrix::operator()(size_t row, size_t col) const {
    return data[row * cols + col];
}


Matrix Matrix::operator+(const Matrix& M) const {
    if (rows != M.rows || cols != M.cols) throw Matrix_Exception ("Невозможно сложить матрицы: неправильные размеры");

    Matrix result(rows, cols);
    for (size_t idx = 0; idx < (rows * cols); ++idx) {
        result.data[idx] = data[idx] + M.data[idx];
    }
    return result;
}


Matrix Matrix::operator-(const Matrix& M) const {
    if (rows != M.rows || cols != M.cols) throw Matrix_Exception ("Невозможно вычесть матрицы: неправильные размеры");

    Matrix result(rows, cols);
    for (size_t idx = 0; idx < (rows * cols); ++idx) {
        result.data[idx] = data[idx] - M.data[idx];
    }
    return result;
}


Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (size_t idx = 0; idx < (rows * cols); ++idx) {
        result.data[idx] = data[idx] * scalar;
    }
    return result;
}


Matrix Matrix::operator*(const Matrix& M) const {
    if (cols != M.rows) throw Matrix_Exception ("Невозможно перемножить матрицы: неправильные размеры");

    Matrix result(rows, M.cols);
    for (size_t rows_A = 0; rows_A < rows; ++rows_A) {
        for (size_t cols_B = 0; cols_B < M.cols; ++cols_B) {
            double sum = 0.0;
            for (size_t cols_A = 0; cols_A < cols; ++cols_A) {
                sum += (*this)(rows_A, cols_A) * M(cols_A, cols_B);
            }
            result(rows_A, cols_B) = sum;
        }
    }
    return result;
}


double Matrix::determinant() const {
    if (cols != rows) throw Matrix_Exception("Невозможно вычислить определитель: матрица не квадратная");
    
    size_t n = rows;
    if (n == 1) {
        return (*this)(0, 0);
    }

    if (n == 2) {
        return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
    }

    if (n == 3) {
        return (*this)(0, 0) * (*this)(1, 1) * (*this)(2, 2)
             + (*this)(0, 1) * (*this)(1, 2) * (*this)(2, 0)
             + (*this)(0, 2) * (*this)(1, 0) * (*this)(2, 1)
             - (*this)(0, 2) * (*this)(1, 1) * (*this)(2, 0)
             - (*this)(0, 1) * (*this)(1, 0) * (*this)(2, 2)
             - (*this)(0, 0) * (*this)(1, 2) * (*this)(2, 1);
    }

    return NAN;
}


Matrix Matrix::exp(size_t iterations) const {
    if (rows != cols) throw Matrix_Exception("Невозможно вычислить экспоненту матрицы: матрица не квадратная");

    Matrix result(rows, cols);
    Matrix term(rows, cols);

    double factorial = 1.0;

    for (size_t k = 0; k < iterations; ++k) {
        term = term + ((*this) * (1.0 / factorial));
        result = result + term;
        factorial *= (k + 1);
    }

    return result;
}


void Matrix::print() const {
    for (size_t rows_A = 0; rows_A < rows; ++rows_A) {
        for (size_t cols_B = 0; cols_B < cols; ++cols_B) {
            std::cout << (*this)(rows_A, cols_B) << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    // Создание матрицы 1
    Matrix matrixA(3, 3);
    matrixA(0, 0) = 1; matrixA(0, 1) = 2; matrixA(0, 2) = 3;
    matrixA(1, 0) = 4; matrixA(1, 1) = 5; matrixA(1, 2) = 6;
    matrixA(2, 0) = 7; matrixA(2, 1) = 8; matrixA(2, 2) = 9;

    // Создание матрицы 2
    Matrix matrixB(3, 3);
    matrixB(0, 0) = 9; matrixB(0, 1) = 8; matrixB(0, 2) = 7;
    matrixB(1, 0) = 6; matrixB(1, 1) = 5; matrixB(1, 2) = 4;
    matrixB(2, 0) = 3; matrixB(2, 1) = 2; matrixB(2, 2) = 1;

    // Выполнение операций
    Matrix result_sum = matrixA + matrixB;
    Matrix result_sub = matrixA - matrixB;
    Matrix result_scalar_mult = matrixA * 5;
    Matrix result_mult = matrixA * matrixB;
    double result_det = matrixA.determinant();
    Matrix result_exp = matrixA.exp(5);

    // Вывод результатов
    std::cout << "Матрица 1:\n";
    matrixA.print();
    std::cout << "\nМатрица 2:\n";
    matrixB.print();
    std::cout << "\nРезультат сложения матриц:\n";
    result_sum.print();
    std::cout << "\nРезультат вычитания матриц:\n";
    result_sub.print();
    std::cout << "\nРезультат умножения матрицы на скаляр:\n";
    result_scalar_mult.print();
    std::cout << "\nРезультат перемножения матриц:\n";
    result_mult.print();
    std::cout << "\nРезультат вычисления определителя:\n" << result_det << std::endl;
    std::cout << "\nРезультат нахождния экспоненты матрицы:\n";
    result_exp.print();
}
