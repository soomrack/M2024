#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <iostream>
#include <math.h>

typedef double MatrixItem;

class Matrix {
private:
    size_t rows;
    size_t cols;
    MatrixItem* data;
public:
    Matrix() : rows(0), cols(0), data(nullptr) {};
    Matrix(const size_t rows, const size_t cols);
    Matrix(const size_t rows, const size_t cols, const MatrixItem* values);
    Matrix(Matrix& A);
    Matrix(Matrix&& A) noexcept;
    ~Matrix();
public:
    void print();
public:
    Matrix& operator=(const Matrix& A);
    Matrix& operator=(Matrix&& A) noexcept;
    Matrix operator+(const Matrix& A);
    Matrix& operator+=(const Matrix& A);
    Matrix operator-(const Matrix& A);
    Matrix operator*(const Matrix& A);
    Matrix operator*(const MatrixItem A);
    Matrix transposition();
    MatrixItem determinant();
    Matrix& exponent(unsigned int m);
    Matrix& set_zero();
    Matrix& set_one();
};


class MatrixException : public std::exception {
protected:
    std::string msg;
public:
    MatrixException(std::string message_of_error) : msg{ message_of_error } {};
};


Matrix::Matrix(const size_t rows, const size_t cols)
    : rows(rows), cols(cols)
{
    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols)
        throw MatrixException("too large object");
    data = new MatrixItem[rows * cols];
}


Matrix::Matrix(const size_t rows, const size_t cols, const MatrixItem* values)
    : rows(rows), cols(cols)
{
    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols)
        throw MatrixException("too large object");
    data = new MatrixItem[rows * cols];
    memcpy(data, values, rows * cols * sizeof(MatrixItem));
}


Matrix::~Matrix()
{
    delete[] data;
}


Matrix::Matrix(Matrix& A)
{
    rows = A.rows;
    cols = A.cols;
    if (A.data == nullptr) {
        data = nullptr;
        return;
    }
    data = new MatrixItem[rows * cols];
    memcpy(data, A.data, rows * cols * sizeof(MatrixItem));
}


Matrix::Matrix(Matrix&& A) noexcept
{
    rows = A.rows;
    cols = A.cols;
    data = A.data;
    A.rows = 0;
    A.cols = 0;
    A.data = nullptr;
}


void Matrix::print()
{
    printf("\n");
    for (size_t col = 0; col < rows; ++col) {
        printf("[ ");
        for (size_t row = 0; row < cols; ++row) {
            printf(" %4.2f", data[row + col * cols]);
        }
        printf("]\n");
    }
}


Matrix& Matrix::operator=(const Matrix& A)
{
    if (&A == this) return *this;

    if (A.data == nullptr) {
        delete[] data;
        data = nullptr;
        return *this;
    }

    if (A.cols * A.rows != cols * rows)
    {
        delete[] data;
        data = new MatrixItem[A.rows * A.cols];
    }
    cols = A.cols;
    rows = A.rows;
    memcpy(this->data, A.data, rows * cols * sizeof(MatrixItem));
    return *this;
}


Matrix& Matrix::operator=(Matrix&& A) noexcept
{
    if (this != &A) {
        delete[] data;

        cols = A.cols;
        rows = A.rows;
        data = A.data;

        A.rows = 0;
        A.cols = 0;
        A.data = nullptr;
    }
    return *this;
}


// A += B
Matrix& Matrix::operator+=(const Matrix& A) {
    if (A.cols != cols || A.rows != rows)
        throw MatrixException("Operator+=: Incorrect sizes");

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        data[idx] += A.data[idx];

    return *this;
}


// C = A + B
Matrix Matrix::operator+(const Matrix& A)
{
    if (A.cols != this->cols || A.rows != this->rows)
        throw MatrixException("Matrix A and B are not proportional");

    Matrix C(this->rows, this->cols);

    for (size_t idx = 0; idx < this->cols * this->rows; ++idx)
        C.data[idx] = A.data[idx] + this->data[idx];
    return C;
}



// C = A - B
Matrix Matrix::operator-(const Matrix& A) {
    if (A.cols != this->cols || A.rows != this->rows)
        throw MatrixException("Matrix A and B are not proportional");

    Matrix C(this->rows, this->cols);

    for (size_t idx = 0; idx < this->cols * this->rows; ++idx)
        C.data[idx] = this->data[idx] - A.data[idx];

    return C;
}


// C = A * B
Matrix Matrix::operator*(const Matrix& A)
{
    if (A.cols != this->rows)
        throw MatrixException("The rows of the matrix A are not equal to the columns of matrix B");

    Matrix C = Matrix(this->rows, A.cols);

    for (size_t C_col = 0; C_col < this->rows; ++C_col) {
        for (size_t C_row = 0; C_row < A.cols; ++C_row) {
            C.data[C_row + C_col * A.cols] = 0;
            for (size_t idx = 0; idx < this->cols; ++idx) {
                C.data[C_row + C_col * A.cols] += this->data[idx + (C_col * this->cols)]
                    * A.data[idx * A.cols + C_row];
            };
        };
    };
    return C;
}


Matrix Matrix::operator*(const MatrixItem A)
{

    Matrix C(this->rows, this->cols);

    for (unsigned int idx = 0; idx < cols * rows; ++idx)
        C.data[idx] = this->data[idx] / A;

    return C;
}


// C = A^T
Matrix Matrix::transposition()
{
    Matrix C(this->rows, this->cols);
    for (size_t C_row = 0; C_row < cols; ++C_row) {
        for (size_t C_col = 0; C_col < rows; ++C_col) {
            C.data[C_col + C_row * rows] = data[C_col * cols + C_row];
        };
    };
    return C;
}


// C = det (A)
MatrixItem Matrix::determinant()
{
    if (cols != rows)
        throw MatrixException("the numbers of columns and rows of the matrix do not match");
    MatrixItem det;
    switch (rows)
    {
    case 1:
        det = data[0];
        return det;
        break;
    case 2:
        det = data[0] * data[3] - data[1] * data[2];
        return det;
        break;
    case 3:
        det =
            +data[0] * data[4] * data[8]
            + data[6] * data[1] * data[5]
            + data[3] * data[7] * data[2]
            - data[2] * data[4] * data[6]
            - data[5] * data[7] * data[0]
            - data[3] * data[1] * data[8];
        return det;
        break;
    default:
        throw MatrixException("valid matrix size 1x1, 2x2, 3x3");
        break;
    };
}


Matrix& Matrix::set_zero()
{
    if (data == nullptr) return *this;
    memset(this->data, 0, sizeof(MatrixItem) * rows * cols);
    return *this;
}


Matrix& Matrix::set_one()
{
    set_zero();
    for (size_t i = 0; i < rows * cols; i += cols + 1)
        data[i] = 1.;
    return *this;
}


// C = e^A
Matrix& Matrix::exponent(unsigned int m)
{
    if ((rows == 0) || (cols == 0))
    {
        throw MatrixException("Can't find exponent matrix for zero matrix");
    }
    if (cols != rows)
        throw MatrixException("the numbers of columns and rows of the matrix do not match");

    Matrix* exp = new Matrix(rows, cols);
    exp->set_one();

    Matrix step(rows, cols);
    step.set_one();

    for (unsigned int k = 1; k <= m; ++k) {

        step = (step * *this) * (MatrixItem)(1. / k);
        *exp = *exp + step;
    };
    return *exp;
}


int main() {
    Matrix A, B, C, E;
    double deter;

    try
    {
        double array1[9]{ 3., 2., 1., 1., 6., 7., 2., 6., 8. };
        double array2[9]{ 1., 0., 0., 1., 7., 2., 6., 6., 8. };
        double array3[4]{ 3., 2., 1., 1. };

        A = Matrix(3, 3, array1);
        A.print();
        B = Matrix(3, 3, array2);
        C = Matrix(2, 2, array3);

        A.print();
        B.print();
        C = A + B;
        C.print();

        C = C.transposition();
        C.print();

        deter = C.determinant();
        printf("\n %f \n", deter);

        A.print();
        A = A.exponent(5);
        A.print();
    }
    catch (MatrixException error)
    {
        std::cerr << error.what() << std::endl;
    }
}
