#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

size_t counter = 0;

class Matrix {
    private:
    size_t cols;
    size_t rows;
    double* data = nullptr;
    size_t id;

    public:
    Matrix()
    {
        id = ++counter;
        std::cout << "вызван конструктор нулевой матрицы с id " << id << std::endl;
        cols = 0;
        rows = 0;
    }


    Matrix(const size_t received_cols, const size_t received_rows)
    {
        id = ++counter;
        std::cout << "вызван конструктор матрицы с id " << id << std::endl;
        if(SIZE_MAX / received_cols < received_rows) {
            //matrix_log(ERROR, "Матрица слишком большая");
            cols = 0;
            rows = 0;
            data = NULL;
            return;
        }

        cols = received_cols;
        rows = received_rows;
        if(cols * rows != 0) {
            data = new double[cols * rows];
        } else {
            data = nullptr;
        }
    }


    Matrix(const Matrix &matrix)
    {
        id = ++counter;
        std::cout << "вызван конструктор копирования для матрицы с id " << id << std::endl;
        cols = matrix.cols;
        rows = matrix.rows;
        if(cols * rows != 0) {
            data = new double[cols * rows];
            memcpy(data, matrix.data, sizeof(double) * matrix.cols * matrix.rows);
        }
    }


    Matrix(Matrix &&matrix)  // Matrix A(C+B);
    {
        id = ++counter;
        std::cout << "вызван конструктор переноса для матрицы с id " << id << std::endl;
        cols = matrix.cols;
        rows = matrix.rows;
        data = matrix.data;
        matrix.data = nullptr;
    }


    ~Matrix()
    {
        std::cout << "вызван деструктор для матрицы с id " << id << std::endl;
        delete data;
    }


    void fill_random(size_t);
    void fill_zeros();
    void unit();
    Matrix& operator=(const Matrix&);
    Matrix& operator=(Matrix&&);
    Matrix& operator+(const Matrix&);
    Matrix& operator-(const Matrix&);
    Matrix& operator*(const Matrix&);
    Matrix& operator*(const double);
    Matrix& exponential(unsigned int);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
};


void Matrix::fill_random(size_t max_value)
{
    for(size_t index = 0; index < rows * cols; index++) {
        double random_number = round(((float)rand()/RAND_MAX)*(float)(max_value * 10)) / 10.0;
        data[index] = random_number;
    }
}


void Matrix::fill_zeros()
{
    for(size_t index = 0; index < rows * cols; index++) {
        data[index] = 0;
    }
}


void Matrix::unit()
{
    fill_zeros();
    for(size_t index = 0; index < rows * cols; index += cols + 1) {
        data[index] = 1.;
    }
}


Matrix& Matrix::operator=(const Matrix& matrix) {
    std::cout << "вызван оператор присваивания с копированием для матрицы с id " << id << std::endl;
    if(data != matrix.data) {
        cols = matrix.cols;
        rows = matrix.rows;
        if(cols * rows != 0) {
            delete data;
            memcpy(data, matrix.data, sizeof(double) * matrix.cols * matrix.rows);
        }
    }
    return *this;
}


Matrix& Matrix::operator=(Matrix&& matrix) {
    std::cout << "вызван оператор присваивания с переносом для матрицы с id " << id << std::endl;
    cols = matrix.cols;
    rows = matrix.rows;
    delete data;
    data = matrix.data;
    matrix.data = nullptr;
    return *this;
}


Matrix& Matrix::operator+(const Matrix& matrix_to_add) {
    std::cout << "вызван оператор сложения" << std::endl;
    // проверка равности размерноти матриц
    if(cols != matrix_to_add.cols || rows != matrix_to_add.rows) {
        throw std::invalid_argument("Для выполнения операции сложения матрицы должны иметь одинаковые размерности");
    }

    Matrix *result_matrix = new Matrix(cols, rows);

    for(size_t index = 0; index < rows * cols; index++) {
        result_matrix->data[index] = data[index] + matrix_to_add.data[index];
    }
    return *result_matrix;
}


Matrix& Matrix::operator-(const Matrix& matrix_to_add) {
    std::cout << "вызван оператор вычитания" << std::endl;
    // проверка равности размерноти матриц
    if(cols != matrix_to_add.cols || rows != matrix_to_add.rows) {
        throw std::invalid_argument("Для выполнения операции вычитания матрицы должны иметь одинаковые размерности");
    }

    Matrix *result_matrix = new Matrix(cols, rows);

    for(size_t index = 0; index < rows * cols; index++) {
        result_matrix->data[index] = data[index] - matrix_to_add.data[index];
    }
    return *result_matrix;
}


Matrix& Matrix::operator*(const Matrix& matrix_to_multiply) {
    std::cout << "вызван оператор умножения" << std::endl;
    // проверка равности размерноти матриц
    if(cols != matrix_to_multiply.rows) {
        throw std::invalid_argument("Для выполнения операции умножения матрицы должны иметь согласованные размерности");
    }

    Matrix *result_matrix = new Matrix(matrix_to_multiply.cols, rows);

    size_t cols_of_A = cols;  // количество столбцов первой матрицы
    size_t operations_per_element = cols_of_A;  // количество операций умножения на каждый элемент результирующей матрицы

    for(size_t row_A = 0; row_A < rows; row_A++) {
        for(size_t row_B = 0; row_B < matrix_to_multiply.cols; row_B++) {
            double current_element = 0;
            for(size_t operation = 0; operation < operations_per_element; operation++) {
                double A_element = data[row_A * cols_of_A + operation];
                double B_element = matrix_to_multiply.data[row_B + matrix_to_multiply.cols * operation];
                current_element += A_element * B_element;
            }
            size_t index = row_A * matrix_to_multiply.cols + row_B;
            result_matrix->data[index] = current_element;
        }
    }
    return *result_matrix;
}


Matrix& Matrix::operator*(const double multiplier) {
    std::cout << "вызван оператор умножения на скаляр" << std::endl;

    Matrix *result_matrix = new Matrix(cols, rows);

    for(size_t index = 0; index < rows * cols; index++) {
        result_matrix->data[index] = data[index] * multiplier;
    }
    return *result_matrix;
}


Matrix& Matrix::exponential(unsigned int total_iterations) 
{
    std::cout << "вызвана операция экспененты" << std::endl;
    // проверка что матрица квадратная
    if(cols != rows) {
        throw std::invalid_argument("Для выполнения операции возведения в степень матрица должна быть квадратной");
    }
    
    Matrix *result_matrix = new Matrix(cols, rows);
    result_matrix->unit();  // результирующая матрица инициализируется как единичная
    Matrix *prev_iteration_matrix = new Matrix(cols, rows);  // матрица для хранения предыдущего слагаемого
    prev_iteration_matrix->unit();

    for(unsigned int iteration = 1; iteration < total_iterations; iteration++) {
        Matrix intermediate_matrix = *prev_iteration_matrix * *this;
        std::cout << intermediate_matrix << std::endl;
        *prev_iteration_matrix = intermediate_matrix * (1/(float)iteration);
        *result_matrix = *result_matrix + *prev_iteration_matrix;
    }
    return *result_matrix;
}


std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for(size_t index_row = 0; index_row < matrix.rows; index_row++) {
        for(size_t index_col = 0; index_col < matrix.cols; index_col++) {
            os << matrix.data[index_row * matrix.cols + index_col] << "  ";
        }
        os << std::endl;
    }
    os << std::endl;
    return os;
}


int main()
{
    setlocale(LC_CTYPE, "");
    std::cout << std::setprecision(2) << std::fixed;

    Matrix matrix_A(3, 3);
    Matrix matrix_B(3, 3);

    matrix_A.fill_random(10);
    matrix_B.fill_random(10);

    Matrix matrix_C = matrix_A.exponential(30);
    std::cout << std::endl;

    std::cout << matrix_A;
    std::cout << matrix_B;
    std::cout << matrix_C;

    return 0;
}
