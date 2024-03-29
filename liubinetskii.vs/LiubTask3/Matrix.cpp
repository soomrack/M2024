#include <math.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include "matrix.hpp"

class MatrixException : public std::exception {
private:
    const char* message;

public:
    MatrixException(std::string msg) : message(msg.c_str()) {}
    const char* what() { return message; }
};
MatrixException BAD_REQUEST("bad_request");
MatrixException OUT_OF_RANGE("out_of_range");
MatrixException NO_MEMORY_ALLOCATED("no_memory_allocated");
MatrixException NULL_POINTER_REFERENCE("null_pointer_reference");

Matrix::Matrix() : rows{ 0 }, cols{ 0 }, items{ nullptr } {}
const MatrixItem epsilon = 0.001;

Matrix::Matrix(const size_t a, const size_t b)
    : rows{ a }, cols{ b }, items{ nullptr }
{
    // TODO except ���� �� ����
    if (rows == 0 && cols == 0)
        return;

    if (rows == 0 || cols == 0)
        throw BAD_REQUEST;

    if (rows >= _CRT_SIZE_MAX / cols / sizeof(MatrixItem))
        throw NO_MEMORY_ALLOCATED;

    items = new MatrixItem[rows * cols];
}


Matrix::Matrix(const Matrix& A)
    : rows{ A.rows }, cols{ A.cols }, items{ nullptr }
{
    if (A.items == nullptr) return;

    if (rows >= _CRT_SIZE_MAX / cols / sizeof(MatrixItem))
        throw NO_MEMORY_ALLOCATED;

    items = new MatrixItem[rows * cols];

    memcpy(items, A.items, rows * cols * sizeof(MatrixItem));
}


Matrix::Matrix(Matrix&& A) : rows{ A.rows }, cols{ A.cols }, items{ A.items }
{
    A.set_null();
}


void Matrix::set_null()
{
    items = nullptr;
    rows = 0;
    cols = 0;
}


Matrix& Matrix::operator=(std::initializer_list<MatrixItem> lst)
{
    if (lst.size() != rows * cols)
        throw OUT_OF_RANGE;

    std::copy(lst.begin(), lst.end(), items);

    return *this;
}


Matrix& Matrix::operator=(const Matrix& A)
{
    if (this == &A) return *this;

    if (items == nullptr) {
        items = new MatrixItem[A.rows * A.cols];
        rows = A.rows;
        cols = A.cols;

        if (A.items == nullptr)
            throw NULL_POINTER_REFERENCE;

        memcpy(items, A.items, rows * cols * sizeof(MatrixItem));
        return *this;
    }

    if (rows * cols == A.cols * A.rows) {
        if (A.items == nullptr)
            throw NULL_POINTER_REFERENCE;

        memcpy(items, A.items, rows * cols * sizeof(MatrixItem));
        return *this;
    }

    delete[] items;

    items = new MatrixItem[A.rows * A.cols];

    rows = A.rows;
    cols = A.cols;

    if (A.items == nullptr)
        throw NULL_POINTER_REFERENCE;

    memcpy(items, A.items, rows * cols * sizeof(MatrixItem));

    return *this;
}


Matrix& Matrix::operator=(Matrix&& A)
{
    if (items != nullptr)
        delete[] items;

    rows = A.rows;
    cols = A.cols;
    items = A.items;

    A.set_null();

    return *this;
}


void Matrix::clear_items_to_zero()
{
    memset(items, 0, sizeof(MatrixItem) * cols * rows);
}


void Matrix::set_as_identity()
{
    clear_items_to_zero();

    size_t min_size = std::min(rows, cols);

    for (size_t idx = 0; idx < min_size; idx++)
        items[idx * cols + idx] = 1.;
}


size_t Matrix::get_rows() const { return rows; }


size_t Matrix::get_cols() const { return cols; }


MatrixItem& Matrix::operator[](const size_t idx)
{
    if (idx >= rows * cols)
        throw OUT_OF_RANGE;

    return items[idx];
}


Matrix& Matrix::operator+=(const Matrix& A)
{
    if ((rows != A.rows) || (cols != A.cols))
        throw BAD_REQUEST;

    for (size_t idx = 0; idx < (rows * cols); idx++)
        items[idx] += A.items[idx];

    return *this;
}


Matrix operator+(const Matrix& A, const Matrix& B)
{
    Matrix sum = A;
    sum += B;
    return sum;
}


Matrix operator+(const Matrix& A, Matrix&& B)
{
    Matrix sum = B;
    sum += A;
    return sum;
}


Matrix& Matrix::operator-=(const Matrix& A)
{
    if ((rows != A.rows) || (cols != A.cols))
        throw BAD_REQUEST;

    for (size_t idx = 0; idx < (rows * cols); idx++)
        items[idx] -= A.items[idx];

    return *this;
}


Matrix operator-(const Matrix& A, const Matrix& B)
{
    Matrix sub = A;
    sub -= B;
    return sub;
}


Matrix operator-(const Matrix& A, Matrix&& B)
{
    Matrix sub = B;
    sub -= A;
    return sub;
}


Matrix& Matrix::multiply(Matrix& trg, const Matrix& A) const
{
    for (size_t num_row = 0; num_row < rows; num_row++) {
        for (size_t num_col = 0; num_col < A.cols; num_col++) {
            MatrixItem sum = 0;

            for (size_t num_sum = 0; num_sum < A.rows; num_sum++) {
                sum += items[num_row * cols + num_sum] * A.items[num_sum * A.cols + num_col];
            }

            trg.items[num_row * trg.cols + num_col] = sum;
        }
    }

    return trg;
}


Matrix Matrix::operator*(const Matrix& A) const
{
    if (cols != A.rows)
        throw BAD_REQUEST;

    Matrix mult(rows, A.cols);
    multiply(mult, A);
    return mult;
}


Matrix& Matrix::operator*=(const Matrix& A)
{
    *this = *this * A;
    return *this;
}


Matrix& Matrix::operator*=(const MatrixItem& factor)
{
    for (size_t idx = 0; idx < (rows * cols); idx++)
        items[idx] *= factor;

    return *this;
}


Matrix Matrix::operator*(const MatrixItem& factor)
{
    Matrix mult = *this;
    mult *= factor;
    return mult;
}


Matrix Matrix::transposed()
{
    Matrix trn(cols, rows);

    for (size_t num_row = 0; num_row < cols; num_row++)
        for (size_t num_col = 0; num_col < rows; num_col++)
            trn.items[num_row * trn.cols + num_col] = items[num_col * cols + num_row];

    return trn;
}


double Matrix::determinant() const
{
    if (cols != rows)
        throw BAD_REQUEST;

    if (items == nullptr)
        throw BAD_REQUEST;

    Matrix mat = *this;

    double det = 1.0;
    int pivot = 0;
    double factor = 0;

    for (size_t num_col = 0; num_col < mat.cols; num_col++) {
        pivot = num_col;

        for (size_t num_row = num_col + 1; num_row < mat.rows; num_row++) {
            if (fabs(mat.items[num_row * mat.cols + num_col]) > fabs(mat.items[pivot * mat.cols + num_col]))
                pivot = num_row;
        }

        if (pivot != num_col) {
            for (size_t idx = 0; idx < mat.rows; idx++)
                std::swap(mat.items[num_col * mat.cols + idx], mat.items[pivot * mat.cols + idx]);

            det *= -1;
        }

        if (mat.items[num_col * mat.cols + num_col] == 0) {
            det = 0;
            break;
        }

        det *= mat.items[num_col * mat.cols + num_col];

        for (size_t num_row = num_col + 1; num_row < mat.rows; num_row++) {
            factor = mat.items[num_row * mat.cols + num_col] / mat.items[num_col * mat.cols + num_col];

            for (size_t idx = num_col + 1; idx < mat.cols; idx++)
                mat.items[num_row * mat.cols + idx] -= factor * mat.items[num_col * mat.cols + idx];
        }
    }

    return det;
}


Matrix Matrix::exponential(const int iterations) const
{
    if (cols != rows)
        throw BAD_REQUEST;

    if (items == nullptr)
        throw BAD_REQUEST;

    Matrix sum(rows, cols);
    Matrix term(rows, cols);

    term.set_as_identity();
    sum.set_as_identity();

    for (size_t count = 1; count < iterations; count++) {
        term *= *this;
        term *= (1. / count);

        sum += term;
    }

    return sum;
}


MatrixItem Matrix::get_maximum()
{
    MatrixItem max = 0;
    MatrixItem num = 0;

    for (size_t idx = 0; idx < (rows * cols); idx++) {
        num = std::fabs(items[idx]);
        if (num > max) max = num;
    }

    return max;
}


void Matrix::set(size_t row, size_t col, MatrixItem value)
{
    if (row >= rows || col >= cols)
        throw OUT_OF_RANGE;

    items[row * cols + col] = value;
}


MatrixItem Matrix::get(size_t row, size_t col) const
{
    if (row >= rows || col >= cols)
        throw OUT_OF_RANGE;

    return items[row * cols + col];
}


bool Matrix::operator==(const Matrix& A) const
{
    if ((cols != A.cols) || (rows != A.rows))
        return false;

    for (size_t idx = 0; idx < (rows * cols); idx++) {
        if (std::abs(items[idx] - A.items[idx]) > epsilon)
            return false;
    }

    return true;
}


std::ostream& operator<<(std::ostream& os, const Matrix& A)
{
    for (size_t row = 0; row < A.get_rows(); row++) {
        for (size_t col = 0; col < A.get_cols(); col++) {
            os << A.get(row, col) << "\t";
        }
        os << std::endl;
    }

    os << std::endl;

    return os;
}


Matrix::~Matrix()
{
    if (items != nullptr) {
        delete[] items;
    }
}
