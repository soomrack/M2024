#include <cstring>
#include <iostream>
#include <random>
#include <iomanip>

typedef double matrix_item;

const matrix_item RANDOM_LOW = -10., RANDOM_HIGH = 10.;
std::uniform_real_distribution<matrix_item> uniform(RANDOM_LOW, RANDOM_HIGH);
std::default_random_engine random_engine(42);

enum MatrixType {
    ZEROS, ONES, RANDOM, IDENTITY, UNFILLED
};


class MatrixException : public std::exception {
private:
    const char* exception_msg;
public:
    MatrixException(std::string msg) : exception_msg(msg.c_str()) {}
    const char* what() { return exception_msg; }
};


class Matrix {
private:
    size_t rows{ 0 };
    size_t cols{ 0 };
    matrix_item* data{ nullptr };
private:
    void fill(enum MatrixType matrix_type);
public:
    Matrix() = default;
    explicit Matrix(size_t n);
    Matrix(size_t rows_amount, size_t cols, MatrixType matrix_type);
    matrix_item get(size_t row, size_t col);
    void set(size_t row, size_t col, matrix_item item);
    void print();
    Matrix(const Matrix& M);
    Matrix(Matrix&& M) noexcept;
    Matrix& operator=(const Matrix& M);
    Matrix& operator=(Matrix&& M) noexcept;
    Matrix operator+(const Matrix& M) const;
    Matrix operator-(const Matrix& M) const;
    Matrix operator*(double scalar) const;
    Matrix operator*(const Matrix& M) const;
    Matrix& operator+=(const Matrix& M);
    Matrix& operator-=(const Matrix& M);
    Matrix& operator*=(double scalar);
    Matrix& operator*=(const Matrix& M);
    Matrix T();
    double det();
    Matrix exp(unsigned int n = 100) const;
    ~Matrix() { delete[] data; }
};


void Matrix::fill(enum MatrixType matrix_type) {
    switch (matrix_type) {
    case (ZEROS):
        memset(data, 0, cols * rows * sizeof(matrix_item));
        break;

    case (ONES):
        for (size_t idx = 0; idx < cols * rows; idx++) data[idx] = 1.;
        break;

    case (RANDOM):
        for (size_t idx = 0; idx < cols * rows; idx++) data[idx] = uniform(random_engine);
        break;

    case (IDENTITY):
        memset(data, 0, cols * rows * sizeof(matrix_item));
        if (cols != rows) throw MatrixException("Wrong number of columns or rows");
        for (size_t row = 0; row < rows; row++)
            for (size_t col = 0; col < cols; col++)
                if (row == col) data[row * cols + col] = 1.;
        break;

    case (UNFILLED):
        break;
    }
}


Matrix::Matrix(const size_t n) {
    if (n == 0) return;
    
    if (n >= SIZE_MAX / sizeof(matrix_item) / n) throw MatrixException("Memory allocation error");
        
    rows = n;
    cols = n;
    data = new matrix_item[n * n];
}


Matrix::Matrix(size_t rows_amount, size_t cols_amount, MatrixType matrix_type) {
    if (rows_amount == 0 || cols_amount == 0) throw MatrixException("Bad matrix error");

    if (rows_amount >= SIZE_MAX / sizeof(matrix_item) / cols_amount)
        throw MatrixException("Memory allocation error");
    
    rows = rows_amount;
    cols = cols_amount;
    data = new matrix_item[rows * cols];
    fill(matrix_type);
}


matrix_item Matrix::get(size_t row, size_t col) {
    if (row > rows || col > cols) throw MatrixException("Out of range");
    else return data[col + cols * row];
}


void Matrix::set(size_t row, size_t col, matrix_item item) {
    if (row > rows || col > cols) throw MatrixException("Out of range");
    else data[col + cols * row] = item;
}


void Matrix::print() {
    if (data == nullptr) throw MatrixException("Bad matrix error");

    std::cout.precision(2);
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) std::cout << std::fixed << data[row * cols + col] << "\t";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


Matrix::Matrix(const Matrix& M) {
    if (rows == 0 || cols == 0) throw MatrixException("Bad matrix error");
        
    if (rows >= SIZE_MAX / sizeof(matrix_item) / cols) throw MatrixException("Memory allocation error");

    rows = M.rows;
    cols = M.cols;
    data = new matrix_item[rows * cols];
    std::copy(M.data, M.data + rows * cols, data);
}


Matrix::Matrix(Matrix&& M) noexcept {
    rows = M.rows;
    cols = M.cols;
    data = M.data;
    M.rows = 0;
    M.cols = 0;
    M.data = nullptr;
}


Matrix& Matrix::operator=(const Matrix& M) {
    if (rows >= SIZE_MAX / sizeof(matrix_item) / cols) throw MatrixException("Memory allocation error");

    if (this == &M) return *this;

    delete[] data;
    rows = M.rows;
    cols = M.cols;

    if (M.data == nullptr) {
        data = nullptr;
        return *this;
    }

    data = new matrix_item[rows * cols];
    std::copy(M.data, M.data + rows * cols, data);
    return *this;
}


Matrix& Matrix::operator=(Matrix&& M) noexcept {
    delete[] data;
    rows = M.rows;
    cols = M.cols;
    data = M.data;
    M.rows = 0;
    M.cols = 0;
    M.data = nullptr;
    return *this;
}


Matrix Matrix::operator+(const Matrix& M) const {
    if (data == nullptr || M.data == nullptr) throw MatrixException("Bad matrix error");

    if (rows != M.rows || cols != M.cols) throw MatrixException("Matrix dimensions do not match");

    Matrix sum = { rows, cols, UNFILLED };
    for (size_t idx = 0; idx < rows * cols; idx++)
        sum.data[idx] = data[idx] + M.data[idx];
    return sum;
}


Matrix Matrix::operator-(const Matrix& M) const {
    if (data == nullptr || M.data == nullptr) throw MatrixException("Bad matrix error");

    if (rows != M.rows || cols != M.cols) throw MatrixException("Matrix dimensions do not match");

    Matrix sub = { rows, cols, UNFILLED };
    for (size_t idx = 0; idx < rows * cols; idx++)
        sub.data[idx] = data[idx] - M.data[idx];
    return sub;
}


Matrix Matrix::operator*(double scalar) const {
    if (data == nullptr) throw MatrixException("Bad matrix error");

    Matrix product = { rows, cols, UNFILLED };
    for (size_t idx = 0; idx < rows * cols; idx++) product.data[idx] = data[idx] * scalar;
    return product;
}


Matrix Matrix::operator*(const Matrix& M) const {
    if (data == nullptr || M.data == nullptr) throw MatrixException("Bad matrix error");

    if (cols != M.rows) throw MatrixException("Matrix outer dimensions do not match");

    Matrix product = { rows, M.cols, ZEROS };
    for (size_t this_row = 0; this_row < rows; this_row++)
        for (size_t M_col = 0; M_col < M.cols; M_col++)
            for (size_t idx = 0; idx < cols; idx++)
                product.data[this_row * cols + M_col] +=
                data[this_row * cols + idx] * M.data[idx * M.cols + M_col];
    return product;
}


Matrix& Matrix::operator+=(const Matrix& M) {
    if (data == nullptr || M.data == nullptr) throw MatrixException("Bad matrix error");

    if (rows != M.rows || cols != M.cols) throw MatrixException("Matrix dimensions do not match");

    for (size_t idx = 0; idx < rows * cols; idx++) data[idx] += M.data[idx];
    return *this;
}


Matrix& Matrix::operator-=(const Matrix& M) {
    if (data == nullptr || M.data == nullptr) throw MatrixException("Bad matrix error");

    if (rows != M.rows || cols != M.cols) throw MatrixException("Matrix dimensions do not match");

    for (size_t idx = 0; idx < rows * cols; ++idx) data[idx] -= M.data[idx];
    return *this;
}


Matrix& Matrix::operator*=(double scalar) {
    if (data == nullptr) throw MatrixException("Bad matrix error");

    for (size_t idx = 0; idx < rows * cols; idx++) data[idx] *= scalar;
    return *this;
}


Matrix& Matrix::operator*=(const Matrix& M) {
    if (data == nullptr || M.data == nullptr)
        throw MatrixException("Bad matrix error");

    if (cols != M.rows)
        throw MatrixException("Matrix dimensions do not match for multiplication");

    Matrix product{ rows, M.cols, ZEROS };

    for (size_t this_row = 0; this_row < rows; this_row++)
        for (size_t M_col = 0; M_col < M.cols; M_col++)
            for (size_t idx = 0; idx < cols; idx++)
                product.data[this_row * cols + M_col] +=
                data[this_row * cols + idx] * M.data[idx * M.cols + M_col];

    *this = product;
    return *this;
}


Matrix Matrix::T() {
    if (data == nullptr) throw MatrixException("Bad matrix error");

    Matrix transposed = { cols, rows, UNFILLED };
    for (size_t row = 0; row < rows; row++)
        for (size_t col = 0; col < cols; col++)
            transposed.data[row * rows + col] = data[col * cols + row];
    return transposed;
}


double Matrix::det() {
    if (data == nullptr) throw MatrixException("Bad matrix error");

    if (cols != rows) throw MatrixException("Matrix should be square");

    if (cols == 1)
        return (double)data[0];
    if (cols == 2)
        return (double)(data[0] * data[3] - data[1] * data[2]);
    if (cols == 3)
        return (double)(data[0] * data[4] * data[8]) + \
        (data[1] * data[5] * data[6]) + \
        (data[2] * data[3] * data[7]) - \
        (data[2] * data[4] * data[6]) - \
        (data[0] * data[5] * data[7]) - \
        (data[1] * data[3] * data[8]);
    throw MatrixException("Determinant can be only calculated for 1x1, 2x2 or 3x3 matrices");
}


Matrix Matrix::exp(unsigned int n) const {
    if (data == nullptr) throw MatrixException("Bad matrix error");

    if (cols != rows) throw MatrixException("Matrix should be square");

    Matrix exponent{ rows, cols, IDENTITY };
    Matrix summand{ rows, cols, IDENTITY };

    if (n == 0) return exponent;

    if (n == 1) {
        exponent += (*this);
        return exponent;
    }

    for (unsigned int idx = 1; idx <= n; idx++) {
        summand *= (*this);
        summand *= (1. / idx);
        exponent += summand;
    }

    return exponent;
}


int main() {
    Matrix A = {3, 3, RANDOM};
    Matrix B = {3, 3, IDENTITY};
    Matrix C = {3, 3, IDENTITY};
    Matrix D = {3, 3, IDENTITY};

    A.print();

    C = A*A;
    C = C*0.5;

    D = A*A;
    D = D*A;
    D = D*(1./6);


    B = B+A;
    B = B+C;
    B = B+D;


    A = A.exp(3);
    A.print();

    B.print();

    return 0;
}
