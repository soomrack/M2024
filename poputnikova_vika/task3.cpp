/*Matrix operations on C++:
https://amkbook.net/mathbook/basic-operations-with-matrices-and-their-properties
*/

#include <iostream>
#include <string>
#include <cstring>
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
    Matrix& operator*=(double scalar);
    Matrix transposition();
    MatrixItem determinant();
    Matrix exponent(unsigned int N);
    Matrix& set_zero();
    Matrix& set_one();
};


class Matrix_Exception : public std::exception
{
private:
    std::string message;
public:
    Matrix_Exception(std::string message) : message { message } {}
    std::string get_message() const { return message; }
};

Matrix_Exception INCORRECT_INPUT_MATRIX("Incorrect input matrix\n");
Matrix_Exception INCORRECT_DIMENTION_OF_MATRIX("The matrix has an incorrect dimension\n");
Matrix_Exception NULL_MATRIX("Your matrix is empty\n");
Matrix_Exception OTHER_ERROR("An unfamiliar error\n");

Matrix::Matrix(const size_t rows, const size_t cols)
    : rows(rows), cols(cols)
{
    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols)
        throw INCORRECT_DIMENTION_OF_MATRIX;
    data = new MatrixItem[rows * cols];
}


Matrix::Matrix(const size_t rows, const size_t cols, const MatrixItem* values)
    : rows(rows), cols(cols)
{
    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols)
        throw INCORRECT_DIMENTION_OF_MATRIX;
    data = new MatrixItem[rows * cols];
    memcpy(data, values, rows * cols * sizeof(MatrixItem));
}


Matrix::~Matrix()
{
    delete[] data;
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
        throw INCORRECT_DIMENTION_OF_MATRIX;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        data[idx] += A.data[idx];

    return *this;
}


// C = A + B
Matrix Matrix::operator+(const Matrix& A)
{
    if (A.cols != this->cols || A.rows != this->rows)
        throw INCORRECT_DIMENTION_OF_MATRIX;

    Matrix C(this->rows, this->cols);

    for (size_t idx = 0; idx < this->cols * this->rows; ++idx)
        C.data[idx] = A.data[idx] + this->data[idx];
    return C;
}



// C = A - B
Matrix Matrix::operator-(const Matrix& A) {
    if (A.cols != this->cols || A.rows != this->rows)
        throw INCORRECT_DIMENTION_OF_MATRIX;

    Matrix C(this->rows, this->cols);

    for (size_t idx = 0; idx < this->cols * this->rows; ++idx)
        C.data[idx] = this->data[idx] - A.data[idx];

    return C;
}


// C = A * B
Matrix Matrix::operator*(const Matrix& A)
{
    if (A.cols != this->rows)
        throw INCORRECT_INPUT_MATRIX;

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


// C = A * B
Matrix Matrix::operator*(const MatrixItem A)
{

    Matrix C(this->rows, this->cols);

    for (unsigned int idx = 0; idx < cols * rows; ++idx)
        C.data[idx] = this->data[idx] * A;

    return C;
}


// C = scalar * A
Matrix& Matrix::operator*=(double scalar) {
    if (data == nullptr) throw NULL_MATRIX;
    for (size_t idx = 0; idx < rows * cols; idx++) 
    data[idx] *= scalar;
    return *this;
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
    MatrixItem det;
	if (cols == 1)
    {
        det = data[0];
        return det;
    }
	
    if (cols == 2) {
        det = data[0] * data[3] - data[1] * data[2];
        return det;
    }

    if (cols == 3) {
        det =
            +data[0] * data[4] * data[8]
            + data[6] * data[1] * data[5]
            + data[3] * data[7] * data[2]
            - data[2] * data[4] * data[6]
            - data[5] * data[7] * data[0]
            - data[3] * data[1] * data[8];
        return det;
    }

    else throw INCORRECT_INPUT_MATRIX;
}


// C = e^A
Matrix Matrix::exponent(unsigned int N)
{
    if ((rows == 0) || (cols == 0))
    {
        throw NULL_MATRIX;
    }
    if (cols != rows)
        throw INCORRECT_INPUT_MATRIX;

    Matrix exp = Matrix(rows, cols);
    exp.set_one();

    Matrix step(rows, cols);
    step.set_one();

    for (unsigned int idx = 1; idx <= N; idx++) {

        step = (step * (*this))*(1. / idx);
        exp += step;
    };
    return exp;
}


void Matrix::print()
{
    printf("\n");
    for (size_t col = 0; col < rows; ++col) {
        printf("[ ");
        for (size_t row = 0; row < cols; ++row) {
            printf("%4.2f ", data[row + col * cols]);
        }
        printf("]\n");
    }
}


int main() {
    Matrix A, B, C, E, F;
    double det;

    try
    {
        double array1[4]{ 7., 4., 2., 0. };
        double array2[4]{ 4., 1., 9., 8. };

        A = Matrix(2, 2, array1);
        B = Matrix(2, 2, array2);

        A.print();
        B.print();

        C = A + B;
        C.print();

        C = A - B;
        C.print();

        C = A;
        C *= 2;
        C.print();
        
        C = A * B;
        C.print();
        
        F = A.transposition();
        F.print();

        det = A.determinant();
        printf("\n %4.2f \n", det);

        C = A.exponent(3);
        C.print();
    }
    catch (const Matrix_Exception& exception)
    {
        std::cout << exception.get_message() << std::endl;
    }
}
