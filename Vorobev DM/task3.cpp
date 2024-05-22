#include <iostream>
#include <string>
#include <cstring>
#include <math.h>

typedef double Matrixoject;

class Matrix {
public:
    Matrix operator+(const Matrix& M) const;
    Matrix operator-(const Matrix& M) const;
    Matrix operator*(const Matrix& M) const;
    Matrix operator*(double k) const;

private:
    size_t rows;
    size_t columns;
    Matrixoject* data;
    int countzero;

public:
    Matrix();
    Matrix(const size_t n);
    Matrix(const size_t row, const size_t col);
    Matrix(const Matrix& M);
    Matrix(Matrix&& M);
    ~Matrix();

public:
    Matrix& operator= (const Matrix& M);
    Matrix& operator= (Matrix&& M);
    Matrix& operator+= (const Matrix& M);
    Matrix& operator-= (const Matrix& M);
    Matrix& operator*= (const double k);
    Matrix& operator*= (const Matrix& M);
    Matrix operator ^ (const double base, const Matrix& matrix);

public:
    void print();
    void transposition(const Matrix& M);
    double determinant(void);
    void random_fill();
    void identity_fill();
    void zeros_fill();
    int zerocount();
    int max_el();
};

class Matrix_Exception : public std::exception
{
private:
    std::string message;
public:
    Matrix_Exception(std::string message) : message{ message } {}
    std::string get_message() const { return message; }
};

Matrix_Exception ERROR_INPUT_MATRIX("Error input matrix\n");
Matrix_Exception INCORRECT_DIMENTION_OF_MATRIX("The matrix has an incorrect dimension\n");
Matrix_Exception NULL_MATRIX("Your matrix is empty\n");
Matrix_Exception ANOTHER_WARNING("An unfamiliar error\n");


Matrix::Matrix() {
    rows = 0;
    columns = 0;
    data = nullptr;
}


Matrix::Matrix(const size_t n) {
    if (n > std::numeric_limits<size_t>::max()) {
        throw std::invalid_argument("Matrix size is too large");
    }

    rows = n;
    columns = n;

    data = new Matrixoject[n * n];
}

Matrix::Matrix(const size_t row, const size_t col) {
    if (row > std::sqrt(std::numeric_limits<size_t>::max()) || col > std::sqrt(std::numeric_limits<size_t>::max())) {
        throw std::invalid_argument("Matrix size is too large");
    }

    rows = row;
    columns = col;

    data = new Matrixoject[row * col];
}


Matrix::Matrix(const Matrix& M) {

    if (rows * columns != M.rows * M.columns) {
        for (unsigned int idx = 0; idx < rows * columns; idx++) {
        data[idx] = M.data[idx];
        return;
        }
    }

    rows = M.rows;
    columns = M.columns;


    delete[] data;
    data = new Matrixoject[rows * columns];

    for (unsigned int idx = 0; idx < rows * columns; idx++) {
        data[idx] = M.data[idx];
    }
}


Matrix::Matrix(Matrix&& M) {
    rows = M.rows;
    columns = M.columns;
    data = M.data;

    M.rows = 0;
    M.columns = 0;
    M.data = nullptr;
}


Matrix::~Matrix() {
    rows = 0;
    columns = 0;
    delete[] data;
}


void Matrix::zeros_fill()
{
    for (size_t index = 0; index < columns * rows; index++) {
        data[index] = 0.0;
    }
}


void Matrix::random_fill()
{
    for (size_t index = 0; index < columns * rows; index++) {
        data[index] = double(rand() % 1000000) / 10000;
    }
}


void Matrix::identity_fill()
{
    zeros_fill();
    for (size_t index = 0; index < rows * columns; index += rows + 1) {
        data[index] = 1.0;
    }
}


void Matrix::print()
{
    if (data == nullptr) throw NULL_MATRIX;

    std::cout << "\n";
    for (size_t r = 0; r < rows; r++) {
        std::cout << "[";
        for (size_t c = 0; c < columns; c++) {
            std::cout << data[r * columns + c];
            if (columns != columns - 1)
                std::cout << "\t";
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}


void Matrix::transposition(const Matrix& M)
{
    if (data == nullptr) throw NULL_MATRIX;

    for (size_t row = 0; row < M.rows; row++)
        for (size_t col = 0; col < M.columns; col++)
            data[row * M.rows + col] = M.data[col * M.columns + row];
}


double Matrix::determinant(void)
{
    if (columns == 1)
    {
        return data[0];
    }
    if (columns == 2) {
        return data[0] * data[3] - data[1] * data[2];
    }

    if (columns == 3) {
        return (data[0] * data[4] * data[8]) +
            (data[1] * data[5] * data[6]) +
            (data[2] * data[3] * data[7]) -
            (data[2] * data[4] * data[6]) -
            (data[0] * data[5] * data[7]) -
            (data[1] * data[3] * data[8]);
    }

    double det = 0;
    Matrix submatrix(rows - 1, columns - 1);

    for (size_t col = 0; col < columns; col++)
    {
        for (size_t row = 1; row < rows; row++)
        {
            size_t submatrix_row = 0;
            for (size_t submatrix_col = 0; submatrix_col < columns; submatrix_col++)
            {
                if (submatrix_col == col)
                    continue;

                submatrix.data[submatrix_row * columns + submatrix_col] = data[(row - 1) * columns + submatrix_col];
                submatrix_row++;
            }
        }

        det += (col % 2 == 0 ? 1 : -1) * data[col] * submatrix.determinant();
    }

    return det;
}

Matrix& Matrix::operator = (const Matrix& M) {
    if (this == &M) {
        return *this;
    }

    if ((M.cols == 0) || (M.rows == 0)) {
        cols = 0;
        rows = 0;
        data = nullptr;
        throw std::runtime_error("Zero matrix");
        return *this;
    }

    if (cols * rows == M.cols * M.rows) {
        cols = M.cols;
        rows = M.rows;
        for (size_t idx = 0; idx < cols * rows; idx++) {
            data[idx] = M.data[idx];
        }
    }
    else {
        if (data != nullptr) {
            delete[] data;
        cols = M.cols;
        rows = M.rows;
        data = new double[cols * rows];
        for (size_t idx = 0; idx < cols * rows; idx++) {
            data[idx] = M.data[idx];
        }
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& M) noexcept {
    if (this != &M) {
        delete[] data;

        rows = M.rows;
        columns = M.columns;
        data = M.data;

        M.data = nullptr;
        M.rows = 0;
        M.columns = 0;
    }

    return *this;
}


Matrix& Matrix::operator+=(const Matrix& M) {
    if (rows != M.rows || columns != M.columns) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }
    if (rows == 0 || columns == 0) {
        throw std::invalid_argument("Cannot add to a matrix with zero dimensions");
    }
    for (size_t idx = 0; idx < rows * columns; idx++) {
        data[idx] += M.data[idx];
    }
    return *this;
}


Matrix& Matrix::operator-=(const Matrix& M) {
    if (rows != M.rows || columns != M.columns) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }
    if (rows == 0 || columns == 0) {
        throw std::invalid_argument("Cannot subtract from a matrix with zero dimensions");
    }
    for (size_t idx = 0; idx < rows * columns; idx++) {
        data[idx] -= M.data[idx];
    }
    return *this;
}


Matrix& Matrix::operator*= (const double k) {
    if (data == nullptr) throw NULL_MATRIX;

    for (size_t idx = 0; idx < rows * columns; idx++) {
        data[idx] *= k;
    }
    return *this;
}


Matrix& Matrix::operator*= (const Matrix& M) {
    if (columns != M.rows && columns != 0 && M.rows != 0)
        throw INCORRECT_DIMENTION_OF_MATRIX;

    Matrix R(rows, M.columns);
    for (size_t row = 0; row < R.rows; row++)
        for (size_t col = 0; col < R.columns; col++)
            for (size_t idx = 0; idx < M.rows; idx++)
                R.data[row * R.columns + col] += data[row * columns + idx] * M.data[idx * M.columns + col];

    columns = R.columns;
    rows = R.rows;
    data = R.data;
    R.data = nullptr;
    return *this;
}


Matrix operator ^ (const double base, const Matrix& matrix)
{
    const unsigned int accuracy = 15;

    if (matrix.rows != matrix.columns) {
        throw std::runtime_error("ERROR: the matrix size isn't square\n");
        Matrix null_matrix;
        return null_matrix;
    }

    Matrix log_matrix(matrix);
    log_matrix *= log(base);

    Matrix new_matrix(matrix.columns, matrix.rows);
    new_matrix.identity_fill();

    Matrix submatrix(matrix.columns, matrix.rows);
    submatrix.identity_fill();

    for (unsigned int index = 1; index < accuracy; index++) {
        term = term * matrix * (1 / index);
        exp += term;
    }
    return new_matrix;
}

Matrix Matrix::operator + (const Matrix& matrix)
{
    if ((cols != matrix.cols) || (rows != matrix.rows)) {
        throw std::runtime_error("ERROR: the matrix sizes are incomparable\n");
    }

    Matrix new_matrix(cols, rows);
    new_matrix = *this;
    new_matrix += matrix;
    return new_matrix;
}

Matrix Matrix::operator - (const Matrix& matrix)
{
    if ((cols != matrix.cols) || (rows != matrix.rows)) {
        throw std::runtime_error("ERROR: the matrix sizes are incomparable\n");
    }

    Matrix new_matrix(cols, rows);
    new_matrix = *this;
    new_matrix -= matrix;
    return new_matrix;
}

Matrix Matrix::operator * (const Matrix& matrix) {
    if (rows != matrix.cols) {
        throw std::runtime_error("Alarm: the matrix sizes are incomparable\n");
    }

    Matrix new_matrix(cols, matrix.rows);
    new_matrix.zeros_fill();
    for (size_t current_col = 0; current_col < new_matrix.cols; current_col++) {
        for (size_t current_row = 0; current_row < new_matrix.rows; current_row++) {

            for (size_t index = 0; index < rows; index++) {

                new_matrix.data[new_matrix.rows * current_col + current_row] +=
                    data[rows * current_col + index] *
                    matrix.data[matrix.rows * index + current_row];
            }
        }
    }
    return new_matrix;
}


Matrix Matrix::operator * (const double number)
{
    Matrix new_matrix;
    new_matrix = *this;
    new_matrix *= number;
    return new_matrix;
}

double DiagonalSum(const Matrix& M) {
    double summdiag = 0;
    if (M.rows != M.cols) {
        throw std::invalid_argument("Matrix is not square/n");
    }
    for (int i = 0; i < M.cols; ++i) {
        summdiag += M.data[i * M.cols + i];
    }
    return summdiag;
}

int Matrix::zerocount() {
    if (data == nullptr) throw NULL_MATRIX;
    countzero = 0;
    for (size_t i = 0, i <= rows * columns, i++) {
        if (data[i] == 0)
            countzero++;
    }
    return countzero;
}


 Matrixobject Matrix::max_el() {
     Matrixobject max = data[0];
    for (size_t i = 0, i <= rows * columns, i++) {
        if (data[i] > max)
            max = data[i];
    }
    return max;
}

int main(void)
{
    Matrix A, B, C, N;
    try {
        A = Matrix(3, 3);
        A.random_fill();
        B = Matrix(3, 3);
        B.random_fill();
        C = Matrix(2, 2);
        C.random_fill();

        A.print();

        B.print();

        C.print();

        C = A;
        C.print();

        C += A;
        C.print();

        C -= A;
        C.print();

        C *= 6.0;
        C.print();

        C *= B;
        C.print();

        C = A + B;
        C.print();

        C = A - B;
        C.print();

        C = A * 2;
        C.print();

        C = A * B;
        C.print();

        C.transposition(A);
        C.print();

        C.determinant();
        C.print();

        return 0;
    }
    catch (const Matrix_Exception& exception)
    {
        std::cout << exception.get_message() << std::endl;
    }

}
