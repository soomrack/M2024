#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <math.h>

typedef double MatrixItem;

class Matrix {
private:
    size_t rows;
    size_t cols;
    MatrixItem* data;

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
    Matrix Matrix::operator+(const Matrix& M); //
    Matrix Matrix::operator-(const Matrix& M);
    Matrix Matrix::operator*(const Matrix& M);
    Matrix Matrix::operator*(const Matrix& M);
	Matrix Matrix::exponent(const unsigned int accuracy);

public:
    void print();
    void transposition(const Matrix& M);
    double determinant(void);
    void random_fill();
	void identity_fill();
	void zeros_fill();
};


class Matrix_Exception : public std::exception
{
private:
    std::string message;
public:
    Matrix_Exception(std::string msg) : message { msg } {}
    std::string get_message() const { return message; }
};

Matrix_Exception INCORRECT_INPUT_MATRIX("Incorrect input matrix\n");
Matrix_Exception INCORRECT_DIMENTION_OF_MATRIX("The matrix has an incorrect dimension\n");
Matrix_Exception NULL_MATRIX("Your matrix is empty\n");
Matrix_Exception OTHER_ERROR("An unfamiliar error\n");


Matrix::Matrix() 
{
    rows = 0;
    cols = 0;
    data = nullptr;
}


Matrix::Matrix(const size_t n) 
{
    rows = n;
    cols = n;

    data = new MatrixItem[n * n];
}


Matrix::Matrix(const size_t row, const size_t col)
{
    rows = row;
    cols = col;

    data = new MatrixItem[row * col];
}


Matrix::Matrix(const Matrix& M)
{
	
	if (rows * cols == M.rows * M.cols) {
        std::memcpy(data, M.data, M.rows * M.cols * sizeof(MatrixItem)); 
        return;
	}
	
    rows = M.rows;
    cols = M.cols;

    data = new MatrixItem[rows * cols];
    std::memcpy(data, M.data, M.rows * M.cols * sizeof(MatrixItem));
}


Matrix::Matrix(Matrix&& M)
{
    rows = M.rows;
    cols = M.cols;
    data = M.data;

    M.rows = 0;
    M.cols = 0;
    M.data = nullptr;
}


Matrix::~Matrix()
{
    rows = 0;
    cols = 0;
    delete[] data;
}


void Matrix::zeros_fill()
{
   std::memset(data, 0, cols * rows * sizeof(MatrixItem));
}


void Matrix::random_fill()
{
    for (size_t index = 0; index < cols * rows; index++) {
        data[index] = rand() % 100;
    }
}


void Matrix::identity_fill()
{
    this->zeros_fill();
    for (size_t index = 0; index < rows * cols; index += rows + 1) {
        data[index] = 1.0;
    }
}


void Matrix::print()
{
    if (data == nullptr) throw NULL_MATRIX;

    std::cout << "\n";
    for (size_t r = 0; r < rows; r++) {
        std::cout << "[";
        for (size_t c = 0; c < cols; c++) {
            std::cout << std::setprecision(2) << data[r * cols + c] << std::fixed;
            if (cols != cols - 1)
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
        for (size_t col = 0; col < M.cols; col++)
            data[row * M.rows + col] = M.data[col * M.cols + row];
}


double Matrix::determinant(void)
{
	if (cols == 1)
    {
        return data[0];
    }
	
    if (cols == 2) {
        return data[0] * data[3] - data[1] * data[2];
    }

    if (cols == 3) {
        return (data[0] * data[4] * data[8]) + \
            (data[1] * data[5] * data[6]) + \
            (data[2] * data[3] * data[7]) - \
            (data[2] * data[4] * data[6]) - \
            (data[0] * data[5] * data[7]) - \
            (data[1] * data[3] * data[8]);
    }

    else throw INCORRECT_INPUT_MATRIX;
}


Matrix& Matrix::operator= (const Matrix& M)
{
    if (this == &M) return *this;    
    if (data != nullptr) delete[] data;

    rows = M.rows;
    cols = M.cols;

    this->data = new MatrixItem[rows * cols];
    memcpy(data, M.data, cols * rows * sizeof(MatrixItem));

    return *this;
}


Matrix& Matrix::operator= (Matrix&& M)
{
    delete[] data;

    rows = M.rows;
    cols = M.cols;
    data = M.data;

    M.rows = 0;
    M.cols = 0;
    M.data = nullptr;

    return *this;
}


Matrix& Matrix::operator+= (const Matrix& M)
{ 
    if ((rows != M.rows) || (cols != M.cols)) 
        throw INCORRECT_DIMENTION_OF_MATRIX;

    for (size_t idx = 0; idx < rows * cols; idx++) {
        data[idx] += M.data[idx];
    }
    return *this;
}


Matrix& Matrix::operator-= (const Matrix& M)
{
    if ((rows != M.rows) || (cols != M.cols))
        throw INCORRECT_DIMENTION_OF_MATRIX;

    for (size_t idx = 0; idx < rows * cols; idx++) {
        data[idx] -= M.data[idx];
    }
    return *this;
}


Matrix& Matrix::operator*= (const double k)
{
    if (data == nullptr) throw NULL_MATRIX;

    for (size_t idx = 0; idx < rows * cols; idx++) {
        data[idx] *= k;
    }
    return *this;
}


Matrix& Matrix::operator*= (const Matrix& M)
{
    if (cols != M.rows)
        throw INCORRECT_DIMENTION_OF_MATRIX;

    Matrix R(rows, M.cols);
    for (size_t row = 0; row < R.rows; row++)
        for (size_t col = 0; col < R.cols; col++)
            for (size_t idx = 0; idx < M.rows; idx++)
                R.data[row * R.cols + col] += data[row * cols + idx] * M.data[idx * M.cols + col]; 
    
    if (data != nullptr){
        delete[] data;
    }

    cols = R.cols;
    rows = R.rows;
    data = R.data;

    R.rows = 0;
    R.cols = 0;
    R.data = nullptr;
    return *this;
}


Matrix Matrix::exponent(const unsigned int accuracy)
{
    if (rows != cols) {
        throw std::runtime_error("ERROR: the matrix size isn't square\n");
        Matrix null_matrix;
        return null_matrix;
    }

    Matrix new_matrix(cols, rows);
    new_matrix.identity_fill();

    Matrix submatrix(cols, rows);
    submatrix.identity_fill();

    for (unsigned int index = 1; index <= accuracy; index++) {
		new_matrix = new_matrix * (*this) * (1.0 / index);
		submatrix += new_matrix;
    }
    return submatrix;
}


Matrix Matrix::operator+(const Matrix& M)
{
    Matrix rez = Matrix(M.rows, M.cols);
    rez = M;
    rez += *this;
    return rez;
}


Matrix Matrix::operator-(const Matrix& M)
{
    Matrix rez = Matrix(M.rows, M.cols);
    rez = M;
    rez -= *this;
    return rez;
}


Matrix Matrix::operator*(const Matrix& M)
{
    Matrix rez = Matrix(M.rows, M.cols);
    rez = M;
    rez *= *this;
    return rez;
}


Matrix Matrix::operator*(const Matrix& M)
{
    Matrix rez = Matrix(M.rows, M.cols);
    rez = M;
    rez *= *this;
    return rez;
}


int main(void)
{ 
    Matrix A, B, C;
    try {
        A = Matrix(3, 3);
        B = Matrix(3, 3);
        C = Matrix(2, 2);
        
        A.random_fill();
        B.random_fill();
        C.random_fill();

        A.print();
        B.print();
        C.print();
        
        C = A;
        C.print();

        C += A;
        C.print();

        C -= B;
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
    
        double det = A.determinant();
        std::cout << det << '\n';
        
        C = A.exponent(3);
        C.print();

        return 0;
    }
    catch (const Matrix_Exception& exception)
    {
        std::cout << exception.get_message() << std::endl;
    }
}