#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>

enum class ErrorType { ERROR };

class System_Exception : public std::exception
{
private:
    std::string message;
public:
    System_Exception(std::string msg) : message { msg } {}
    std::string get_message() const { return message; }
};

System_Exception ERROR_LOCATE("Ошибка выделения памяти\n");
System_Exception ERROR_SIZE("Размеры матриц не совпадают\n");
System_Exception ERROR_SQUARE("Матрица должна быть квадратной\n");


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

    void matrix_identity();
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;

    Matrix operator+(const Matrix& M) const;
    Matrix operator-(const Matrix& M) const;
    Matrix operator*(double scalar) const;
    Matrix operator*(const Matrix& M) const;
    Matrix exp(size_t iterations=_10) const; //ВВЕЛА
    double det() const;

    void print() const;
};


Matrix::Matrix() : rows(0), cols(0), data(nullptr) {}


Matrix::Matrix(const size_t rows, const size_t cols) : rows(rows), cols(cols) // фигурные cols
{
    if (SIZE_MAX / cols / rows < sizeof(double)) {
        throw ERROR_LOCATE;
    }

    data = new double[rows * cols];
}


Matrix::Matrix(const size_t rows, const size_t cols, const double* values)
    : rows(rows), cols(cols) 
{
    if (SIZE_MAX / cols / rows < sizeof(double)) {
        throw ERROR_LOCATE;
    }

    data = new double[rows * cols];
    std::copy(values, values + (rows * cols), data);
}


Matrix::Matrix(const Matrix& M) : rows(M.rows), cols(M.cols) 
{
    data = new double[rows * cols];
    std::copy(M.data, M.data + (rows * cols), data);
}


Matrix::Matrix(Matrix&& A) : rows(A.rows), cols(A.cols), data(A.data) 
{
    A.rows = 0;
    A.cols = 0;
    A.data = nullptr;
}


Matrix::~Matrix() 
{
    delete[] data;
}


//копирование
Matrix& Matrix::operator=(const Matrix& M) 
{
    if (this == &M) {
        return *this;
    }   
    
    if (rows != M.cols || cols != M.cols) {
        if (data != nullptr) {
            delete[] data;
        }
        data = new double[rows * cols];
    }


    rows = M.rows;
    cols = M.cols;
    std::copy(M.data, M.data + (rows * cols), data);

    return *this;
}

//перемещение
Matrix& Matrix::operator=(Matrix&& M) 
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


double& Matrix::operator()(size_t row, size_t col) 
{
    return data[row * cols + col];
}


const double& Matrix::operator()(size_t row, size_t col) const 
{
    return data[row * cols + col];
}

//вывод
void Matrix::print() const {
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            std::cout << (*this)(row, col) << " ";
        }
        std::cout << std::endl;
    }
}


//сложение
Matrix Matrix::operator+(const Matrix& M) const 
{
    if (rows != M.rows || cols != M.cols) {
        throw ERROR_SIZE;
    }

    Matrix result(rows, cols);
    for (size_t ind = 0; ind < (rows * cols); ind++) {
        result.data[ind] = data[ind] + M.data[ind];
    }
    return result;
}

//вычитание
Matrix Matrix::operator-(const Matrix& M) const 
{
    if (rows != M.rows || cols != M.cols) {
        throw ERROR_SIZE;
    }

    Matrix result(rows, cols);
    for (size_t ind = 0; ind < (rows * cols); ind++) {
        result.data[ind] = data[ind] - M.data[ind];
    }
    return result;
}

//умножение матриц
Matrix Matrix::operator*(const Matrix& M) const 
{
    if (cols != M.rows) {
        throw ERROR_SIZE;
    }

    Matrix result(rows, M.cols);

    for (size_t row_A = 0; row_A < rows; row_A++) {
        for (size_t col_B = 0; col_B < M.cols; col_B++) {
            double sum = 0;
            for (size_t cols_A = 0; cols_A < cols; cols_A++) {
                sum += (*this)(row_A, cols_A) * M(cols_A, col_B);
            }
            result(row_A, col_B) = sum;
        }
    }
    return result;
}

//умножение на скаляр
Matrix Matrix::operator*(double scalar) const 
{
    Matrix result(rows, cols);

    for (size_t ind = 0; ind < (rows * cols); ind++) {
        result.data[ind] = data[ind] * scalar;
    }

    return result;
}

//единичная матрица
void Matrix::matrix_identity()
{
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            if (row == col) {
                data[row * cols + col] = 1.0;
            }
            else {
                data[row * cols + col] = 0.0;
            }
        }
    }
}

//e^
Matrix Matrix::exp(size_t iterations) const 
{
    if(cols != rows) {
        throw ERROR_SQUARE;
    }

    Matrix result_matrix = Matrix(cols, rows);
    result_matrix.matrix_identity();  //заполнение единичной матрицей
    Matrix B = Matrix(cols, rows);  //хранение предыдущего слагаемого
    B.matrix_identity();

    for(unsigned int k = 1; k < iterations; k++) {
        Matrix intermediate_matrix = B * *this;
        B = intermediate_matrix * (1/(float)k);
        result_matrix = result_matrix + B;
    }
    return result_matrix;
}

//определитель
double Matrix::det() const 
{
    if (cols != rows) {
        throw ERROR_SQUARE;
    }
    
    if (cols == 1) {
        return data[0];
    }

    if (cols == 2) {
        return (data[0]*data[3]- data[1] * data[2]);
    }

    if (cols == 3) {
        return (data[0] * data[4] * data[8]) + \
            (data[1] * data[5] * data[6]) + \
            (data[2] * data[3] * data[7]) - \
            (data[2] * data[4] * data[6]) - \
            (data[0] * data[5] * data[7]) - \
            (data[1] * data[3] * data[8]);
    }

    return NAN;
}

int main() 
{
    Matrix A, B, C;
    try {
        double arr_A[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	    double arr_B[9] = {5, -2, 1, 0, 2, 3, -1, 4, 7};
	    double arr_C[9] = {3, 1, -4, 0, 1, 2, -1, 0, 1};
    
        Matrix A = Matrix(3, 3, arr_A);
        Matrix B = Matrix(3, 3, arr_B);
        Matrix C = Matrix(3, 3, arr_C);
        double det = 0.0;

        std::cout << "Матрица A:" << std::endl;
		A.print();
		std::cout << "Матрица B:" << std::endl;
		B.print();

        C = A + B;
	    std::cout << "Сложение матриц A и B:" << std::endl;
	    C.print();

        C = A - B;
	    std::cout << "Вычитание матриц A и B:" << std::endl;
	    C.print();

	    C = A * B;
	    std::cout << "Умножение матриц A и B:" << std::endl;
	    A.print();

        C = A * 3;
	    std::cout << "Умножения матрицы A на скаляр 3:" << std::endl;
	    A.print();

	    A = A.exp(3);
	    std::cout << "Экспонента матрицы А:" << std::endl;
	    A.print();

        det = A.det();
	    std::cout << "Определитель матрицы A:" << std::endl;
	    printf("%.f\n", det);
	    std::cout << std::endl;
    
        return 0;
    }
    catch (const System_Exception& exception)
    {
        std::cout << exception.get_message() << std::endl;
    }
}