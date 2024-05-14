#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdint.h>
#include <cstring>

typedef double MatrixItem;
using namespace std;
enum MatrixType { ZERO, ONES, RANDOM, IDENTITY };

class Matrix {
private:
    size_t rows;
    size_t cols;
    MatrixItem* data;
public:
    Matrix() : rows(0), cols(0), data(nullptr) {};
    Matrix(const size_t cols, const size_t rows);
    Matrix(const Matrix& A);
    Matrix(Matrix&& A);
public:
    Matrix& operator=(const Matrix& A);
    Matrix& operator=(Matrix&& A);
    Matrix& operator+(const Matrix& A);
    Matrix& operator+=(const Matrix& A);
    Matrix& operator-(const Matrix& A);
    Matrix& operator-=(const Matrix& A);
    Matrix& operator*(const Matrix& B);
    Matrix& operator*=(const Matrix& B);
    Matrix& operator*(const double& coeff);
    Matrix& operator*=(const double& coeff);
public:
    void fill(enum MatrixType mat_type);
    void print();
    static Matrix transpose(const Matrix& A);
    static Matrix exponent(const unsigned int level, const Matrix& A);
    double determinant() const;

    ~Matrix() {
        cols = 0;
        rows = 0;
        delete[] data;
    }
};

class MatrixException : public exception {
private:
    string message;
public:
    MatrixException(string message) : message{ message } {}
    string get_message() const { return message; }
};

MatrixException SQUARE_ERROR("\nОшибка: матрица должна быть квадратной!\n\n");
MatrixException SIZE_ERROR("\nОшибка: матрицы должны иметь одинаковый размер\n\n");
MatrixException SHAPE_ERROR("\nОшибка: количество столбцов первой матрицы не совпадает с количеством строк во второй матрице\n\n");
MatrixException NULL_MATRIX("\nОшибка: матрица пуста\n\n");
MatrixException OTHER_ERROR("\nОшибка: неизвестная ошибка\n\n");

void Matrix::print() {
    if (data == nullptr) throw NULL_MATRIX;
    cout << "\n";
    for (size_t row = 0; row < rows; row++) {
        cout << "[";
        for (size_t column = 0; column < cols; column++) {
            cout << data[column + row * cols];
            if (column != cols - 1) cout << "\t"; // исправлено условие
        }
        cout << "]\n";
    }
    cout << "\n";
}

Matrix::Matrix(const size_t cols, const size_t rows)
    : cols(cols), rows(rows), data(nullptr) {
    if (cols == 0 || rows == 0) {
        data = nullptr;
        return;
    };

    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols) {
        return;
    };

    data = new MatrixItem[cols * rows];
}

Matrix::Matrix(const Matrix& A) {
    if (A.data == nullptr) {
        cols = A.cols;
        rows = A.rows;
        data = nullptr;
        return;
    };
    cols = A.cols;
    rows = A.rows;
    data = new MatrixItem[rows * cols];
    memcpy(data, A.data, sizeof(MatrixItem) * A.rows * A.cols);
}

Matrix::Matrix(Matrix&& A) {
    cols = A.cols;
    rows = A.rows;
    data = A.data;
    A.cols = 0;
    A.rows = 0;
    A.data = nullptr;
}

void Matrix::fill(enum MatrixType mat_type) {
    switch (mat_type) {
    case (ZERO):
        memset(data, 0, cols * rows * sizeof(MatrixItem));
        break;

    case (ONES):
        for (size_t idx = 0; idx < cols * rows; idx++)
            data[idx] = 1.0;
        break;

    case (RANDOM):
        for (size_t idx = 0; idx < cols * rows; idx++)
            data[idx] = (MatrixItem)(rand() % 10);
        break;

    case (IDENTITY):
        this->fill(ZERO);
        for (size_t row_col = 0; row_col < rows; row_col++)
            data[row_col * cols + row_col] = 1;
        break;
    }
}

//Оператор присваивания
Matrix& Matrix::operator=(const Matrix& A) {
    if (this == &A) return *this;
    if (cols != A.cols || rows != A.rows) {
        delete[] data;
        cols = A.cols;
        rows = A.rows;
        data = A.data;
    } else {
        memcpy(data, A.data, cols * rows * sizeof(MatrixItem));
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& A) {
    if (this == &A) return *this;

    cols = A.cols;
    rows = A.rows;
    delete[] data;
    data = A.data;

    A.data = nullptr;

    return *this;
}
Matrix& Matrix::operator+(const Matrix& A) {
    if (A.cols != cols || A.rows != rows) throw SIZE_ERROR;

    Matrix result(*this);

    for (size_t idx = 0; idx < cols * rows; ++idx)
        result.data[idx] += A.data[idx];

    return result;
}

Matrix& Matrix::operator+=(const Matrix& A) {
    if (A.cols != cols || A.rows != rows) throw SIZE_ERROR;

    for (size_t idx = 0; idx < cols * rows; ++idx)
        data[idx] += A.data[idx];

    return *this;
}

Matrix& Matrix::operator-(const Matrix& A) {
    if (A.cols != cols || A.rows != rows) throw SIZE_ERROR;

    Matrix result(*this);

    for (size_t idx = 0; idx < cols * rows; ++idx)
        result.data[idx] -= A.data[idx];

    return result;
}

Matrix& Matrix::operator-=(const Matrix& A) {
    if (A.cols != cols || A.rows != rows) throw SIZE_ERROR;

    for (size_t idx = 0; idx < cols * rows; ++idx)
        data[idx] -= A.data[idx];

    return *this;
}

Matrix& Matrix::operator*(const Matrix& B) {
    if (cols != B.rows) throw SHAPE_ERROR;

    Matrix result(rows, B.cols);

    for (size_t rowA = 0; rowA < rows; ++rowA)
        for (size_t colB = 0; colB < B.cols; ++colB) {
            result.data[result.cols * rowA + colB] = 0.0;
            for (size_t colA = 0; colA < cols; ++colA)
                result.data[result.cols * rowA + colB] += data[colA + rowA * cols] * B.data[B.cols * colA + colB];
        };

    return result;
}

Matrix& Matrix::operator*=(const Matrix& B) {
    *this = *this * B;

    return *this;
}

Matrix& Matrix::operator*(const double& coeff) {
    Matrix result(*this);

    for (size_t idx = 0; idx < cols * rows; ++idx)
        result.data[idx] = data[idx] * coeff;

    return result;
}

Matrix& Matrix::operator*=(const double& coeff) {
    for (size_t idx = 0; idx < cols * rows; ++idx)
        data[idx] = data[idx] * coeff;

    return *this;
}

Matrix Matrix::transpose(const Matrix& A) {
    Matrix result(A.cols, A.rows);

    for (size_t row = 0; row < A.rows; ++row)
        for (size_t col = 0; col < A.cols; ++col)
            result.data[col * A.rows + row] = A.data[row * A.cols + col];

    return result;
}

Matrix Matrix::exponent(const unsigned int level, const Matrix& A) {
    if (A.cols != A.rows) throw SQUARE_ERROR;
    if (A.data == nullptr) throw NULL_MATRIX;

    Matrix R(A.rows, A.cols);
    R.fill(IDENTITY);

    Matrix temp = A;
    for (unsigned int idx = 1; idx <= level; idx++) {
        temp *= A;
        R += (temp * (1.0 / idx));
    }
    return R;
}

double Matrix::determinant() const {
    if (cols != rows) throw SQUARE_ERROR;

    if (cols == 1) {
        return data[0];
    }
    else if (cols == 2) {
        return (data[0] * data[3] - data[1] * data[2]);
    }
    else if (cols == 3) {
        return (data[0] * data[4] * data[8]) + \
            (data[1] * data[5] * data[6]) + \
            (data[2] * data[3] * data[7]) - \
            (data[2] * data[4] * data[6]) - \
            (data[0] * data[5] * data[7]) - \
            (data[1] * data[3] * data[8]);
    }
    else throw OTHER_ERROR;
}

int main() {
    Matrix A(3, 3);
    A.fill(RANDOM);
    A.print();
}
