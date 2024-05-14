#include <math.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>

typedef double MatrixItem;


class Matrix
{
private:
    size_t rows;
    size_t cols;
    MatrixItem* items;
   
public:
    Matrix();
    Matrix(const size_t a, const size_t b);
    Matrix(const Matrix& A);
    Matrix(Matrix&& A);
    ~Matrix();

public:
    void clear_items_to_zero();
    void set_as_identity();
    size_t get_rows() const;
    size_t get_cols() const;

    MatrixItem& operator[](const size_t idx); 

    Matrix& operator=(std::initializer_list<MatrixItem> lst); 
    Matrix& operator=(const Matrix& A);
    Matrix& operator=(Matrix&& A);

    Matrix operator+(const Matrix& A);
    Matrix operator-(const Matrix& A); 

    Matrix& operator+=(const Matrix& A);
    Matrix& operator-=(const Matrix& A);
    bool operator==(const Matrix& A) const;

    Matrix operator*(const Matrix& A) const;
    Matrix& operator*=(const Matrix& A);

    Matrix operator*(const MatrixItem& factor);
    Matrix& operator*=(const MatrixItem& factor);


    void set(size_t row, size_t col, MatrixItem value);
    MatrixItem get(size_t row, size_t col) const;


    Matrix transposed();
    Matrix minor(const size_t minor_row, const size_t minor_col);
    double determinant() const;
    int sled();
    Matrix exponential(const size_t iterations = 100) const;

    
};

std::ostream& operator<<(std::ostream& os, const Matrix& A);


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


Matrix::Matrix() : rows{ 0 }, cols{ 0 }, items{ nullptr } {}
const MatrixItem epsilon = 0.001;

Matrix::Matrix(const size_t a, const size_t b)
    : rows{ a }, cols{ b }, items{ nullptr }
{
     if (rows == 0 && cols == 0)
        return;

    if (rows == 0 || cols == 0)
        throw BAD_REQUEST;

    if (rows >= SIZE_MAX / cols / sizeof(MatrixItem))
        throw NO_MEMORY_ALLOCATED;

    items = new MatrixItem[rows * cols];
}


Matrix::Matrix(const Matrix& A)
    : rows{ A.rows }, cols{ A.cols }, items{ nullptr }
{
    if (A.items == nullptr) return;

    if (rows >= SIZE_MAX / cols / sizeof(MatrixItem))
        throw NO_MEMORY_ALLOCATED;

    items = new MatrixItem[rows * cols];

    memcpy(items, A.items, rows * cols * sizeof(MatrixItem));
}


Matrix::Matrix(Matrix&& A): rows{ A.rows }, cols{ A.cols }, items{ A.items } 
{
    A.items = nullptr;
    A.rows = 0;
    A.cols = 0;
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
        rows = A.rows;
        cols = A.cols;
        if (A.items == nullptr)
             return *this;

        items = new MatrixItem[A.rows * A.cols];


        memcpy(items, A.items, rows * cols * sizeof(MatrixItem));
        return *this;
    }

    if (rows * cols == A.cols * A.rows) {
        memcpy(items, A.items, rows * cols * sizeof(MatrixItem));
        return *this;
    }

    delete[] items;

    items = new MatrixItem[A.rows * A.cols];

    rows = A.rows;
    cols = A.cols;

    if (A.items == nullptr)
        return *this;

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

    A.items = nullptr;
    A.rows = 0;
    A.cols = 0;

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
    if (rows * cols == 0) throw BAD_REQUEST;
    if (idx >= rows * cols) throw OUT_OF_RANGE;
    if (idx < 0) throw OUT_OF_RANGE;

    return items[idx];
}


Matrix& Matrix::operator+=(const Matrix& A) 
{
    if ((rows != A.rows) || (cols != A.cols))
        throw BAD_REQUEST;

    for (size_t idx = 0; idx < (rows * cols); idx += cols + 1)
        items[idx] += A.items[idx];

    return *this;
}


Matrix Matrix::operator+(const Matrix& A) 
{
    Matrix sum = A;
    sum += *this;
    return sum;
}


Matrix& Matrix::operator-=(const Matrix& A) 
{
    if ((rows != A.rows) || (cols != A.cols))
        throw BAD_REQUEST;

    for (size_t idx = 0; idx < (rows * cols); idx += cols + 1)
        items[idx] -= A.items[idx];

    return *this;
}


Matrix Matrix::operator-(const Matrix& A) 
{
    Matrix sub = A;
    sub -= *this;
    return sub;
}


Matrix Matrix::operator*(const Matrix& A) const
{
    if (cols != A.rows)
        throw BAD_REQUEST;

    Matrix mult(rows, A.cols);
        for (size_t num_row = 0; num_row < rows; num_row++) {
         for (size_t num_col = 0; num_col < A.cols; num_col++) {
            MatrixItem sum = 0;

            for (size_t num_sum = 0; num_sum < A.rows; num_sum++) {
                sum += items[num_row * cols + num_sum] * A.items[num_sum * A.cols + num_col];
            }

            items[num_row * cols + num_col] = sum;
        }
    }
    return mult;
}


Matrix& Matrix::operator*=(const Matrix& A)
{
    *this = *this * A;
    return *this;
}


Matrix& Matrix::operator*=(const MatrixItem& factor) 
{
    for (size_t idx = 0; idx < (rows * cols); idx += cols + 1)
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


Matrix Matrix::minor(const size_t minor_row, const size_t minor_col)
{
    if ((cols <= 1) || (rows <= 1)) {
        throw BAD_REQUEST;
    }
    if ((minor_col >= cols) || (minor_row >= rows)) {
        throw BAD_REQUEST;
    }

    Matrix minor(cols - 1, rows - 1);
    size_t origin_index = 0;

    for (size_t current_col = 0; current_col < minor.cols; current_col++) {
        if (current_col == minor_col) {
            origin_index += cols;
        }
        for (size_t current_row = 0; current_row < minor.rows; current_row++) {
            if (current_row == minor_row) {
                origin_index += 1;
            }
            minor.items[current_col * minor.rows + current_row] = items[origin_index];
            origin_index += 1;
        }
        if (minor_row == minor.rows) { 
            origin_index += 1;
        }
    }
    return minor;
}



double Matrix::determinant() const
{
    if (cols != rows)
        throw BAD_REQUEST;

    if (items == nullptr)
        throw BAD_REQUEST;

    double determinant = 0.0;

    if (cols == 1) {
        determinant = items[0];
        return determinant;
    }

    Matrix matrix_copy(rows, cols); 
    for (size_t index = 0; index < matrix_copy.cols * matrix_copy.rows; index++) {
        matrix_copy.items[index] = items[index];
    }

    Matrix minor;
    for (size_t summ_index = 0; summ_index < matrix_copy.rows; summ_index++) {
        minor = matrix_copy.minor(0, summ_index);

        if (summ_index % 2 == 0) {
            determinant += items[summ_index] * minor.determinant();
        }
        else {
            determinant -= items[summ_index] * minor.determinant();
        }
    }

    return determinant;
}


Matrix Matrix::exponential(const size_t iterations) const 
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
        term *= (1.0 / count);

        sum += term;
    }

    return sum;
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

int get_rand_integer(int start = 0, int end = 99) { 

    if (start > end) {
        std::swap(start, end);
    }

    return rand() % (end - start + 1) + start;
}


void init_matrix_as_random(Matrix* M) { 
    size_t rows = M->get_rows();
    size_t cols = M->get_cols();

    for (size_t idx = 0; idx < rows * cols; idx++)
    {
        (*M)[idx] = get_rand_integer();
    }
}

int Matrix::sled () {
    int sled = 0;
        if (cols != rows)
        throw BAD_REQUEST;
        int size = rows * cols;
        for (size_t i = 0; i < size; i+= cols+1) {
        sled += items[i];    
        
    }
return sled ;
}

int main()
{
    srand(static_cast<unsigned int>(time_t(nullptr))); 
    setlocale(LC_CTYPE, "");
    
    try {
        Matrix A(2, 2);
        init_matrix_as_random(&A);
        std::cout << A << std::endl;

        Matrix B(2, 2);
        B.set_as_identity();
        std::cout << B << std::endl;

        Matrix C;
        C = A + B;
        std::cout << C << std::endl;

        std::cout << "----------" << std::endl;

        int sled = A.sled();
        std::cout << sled << std::endl;
                 
    }
    catch (MatrixException& ex)
    {
        std::cout << "ERROR = " << ex.what();
    }

    return 0;
}
