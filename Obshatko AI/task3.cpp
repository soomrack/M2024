#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <locale.h>


class matrix_size_error: public std::exception
{
public:
    matrix_size_error(const std::string& message): message{message}
    {}
    const char* what() const noexcept override
    {
        return message.c_str();
    }
private:
    std::string message;
};


class matrix_data_error: public std::exception
{
public:
    matrix_data_error(const std::string& message): message{message}
    {}
    const char* what() const noexcept override
    {
        return message.c_str();
    }
private:
    std::string message;
};


class Matrix {
private:
    size_t cols;
    size_t rows;
    double* data = nullptr;
    size_t id;  // для разработчика
    static int counter;  // для разработчика

public:
    Matrix();
    Matrix(const size_t received_cols, const size_t received_rows);
    Matrix(Matrix &matrix);
    Matrix(Matrix &&matrix);
    ~Matrix();

    void fill_random(size_t);
    void fill_zeros();
    void unit();
    void matrix_row_add(size_t result_row, size_t adding_row, double multiplyer);
    void matrix_row_substract(size_t result_row, size_t adding_row, double multiplyer);
    Matrix& operator=(const Matrix&);
    Matrix& operator=(Matrix&&);
    Matrix operator+(const Matrix&);
    Matrix operator-(const Matrix&);
    Matrix operator*(const Matrix&);
    Matrix operator*(const double);
    Matrix exponential(unsigned int);
    double det(double accuracy);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
};
int Matrix::counter = 0;  // для разработчика


Matrix::Matrix() 
{
    id = ++counter;  // для разработчика
    std::cout << "вызван конструктор нулевой матрицы с id " << id << std::endl;  // для разработчика
    cols = 0;
    rows = 0;
}


Matrix::Matrix(const size_t received_cols, const size_t received_rows) : cols {received_cols}, rows {received_rows}
{
    id = ++counter;  // для разработчика
    std::cout << "вызван конструктор матрицы с id " << id << std::endl;  // для разработчика
    data = nullptr;
    if(SIZE_MAX / received_cols < received_rows) {
        throw matrix_size_error("Матрица слишком большая");
        cols = 0;
        rows = 0;
        return;
    }
    
    if(cols != 0 && rows != 0) {
        data = new double[cols * rows];
    }
}


Matrix::Matrix(Matrix &matrix)
{
    id = ++counter;  // для разработчика
    std::cout << "вызван конструктор копирования для матрицы с id " << id << std::endl;  // для разработчика
    cols = matrix.cols;
    rows = matrix.rows;
    if(cols != 0 && rows != 0) {
        data = new double[cols * rows];
        memcpy(data, matrix.data, sizeof(double) * matrix.cols * matrix.rows);
    }
}


Matrix::Matrix(Matrix &&matrix)  // Matrix A(C+B);
{
    id = ++counter;  // для разработчика
    std::cout << "вызван конструктор переноса для матрицы с id " << id << std::endl;  // для разработчика
    cols = matrix.cols;
    rows = matrix.rows;
    data = matrix.data;
    matrix.data = nullptr;
}


Matrix::~Matrix()
{
    std::cout << "вызван деструктор для матрицы с id " << id << std::endl;  // для разработчика
    delete data;
}


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


void Matrix::matrix_row_add(size_t result_row, size_t adding_row, double multiplyer)
{
    for(int col = 0; col < cols; col++) {
        data[result_row * cols + col] += data[adding_row * cols + col] * multiplyer;
    }
}


void Matrix::matrix_row_substract(size_t result_row, size_t adding_row, double multiplyer)
{
    for(int col = 0; col < cols; col++) {
        data[result_row * cols + col] -= data[adding_row * cols + col] * multiplyer;
    }
}


Matrix& Matrix::operator=(const Matrix& matrix) {
    std::cout << "вызван оператор присваивания с копированием для матрицы с id " << id << std::endl;  // для разработчика
    if(data != matrix.data) {
        cols = matrix.cols;
        rows = matrix.rows;
        if(cols != 0 && rows != 0) {
            delete data;
            memcpy(data, matrix.data, sizeof(double) * matrix.cols * matrix.rows);
        }
    }
    return *this;
}


Matrix& Matrix::operator=(Matrix&& matrix) {
    std::cout << "вызван оператор присваивания с переносом для матрицы с id " << id << std::endl;  // для разработчика
    cols = matrix.cols;
    rows = matrix.rows;
    delete data;
    data = matrix.data;
    matrix.data = nullptr;
    return *this;
}


Matrix Matrix::operator+(const Matrix& matrix_to_add) {
    std::cout << "вызван оператор сложения" << std::endl;  // для разработчика
    // проверка равности размерноти матриц
    if(cols != matrix_to_add.cols || rows != matrix_to_add.rows) {
        throw matrix_size_error("Для выполнения операции сложения матрицы должны иметь одинаковые размерности");
    }

    Matrix result_matrix = Matrix(cols, rows);

    for(size_t index = 0; index < rows * cols; index++) {
        result_matrix.data[index] = data[index] + matrix_to_add.data[index];
    }
    return result_matrix;
}


Matrix Matrix::operator-(const Matrix& matrix_to_add) {
    std::cout << "вызван оператор вычитания" << std::endl;  // для разработчика
    // проверка равности размерноти матриц
    if(cols != matrix_to_add.cols || rows != matrix_to_add.rows) {
        throw matrix_size_error("Для выполнения операции вычитания матрицы должны иметь одинаковые размерности");
    }

    Matrix result_matrix = Matrix(cols, rows);

    for(size_t index = 0; index < rows * cols; index++) {
        result_matrix.data[index] = data[index] - matrix_to_add.data[index];
    }
    return result_matrix;
}


Matrix Matrix::operator*(const Matrix& matrix_to_multiply) {
    std::cout << "вызван оператор умножения" << std::endl;  // для разработчика
    // проверка равности размерноти матриц
    if(cols != matrix_to_multiply.rows) {
        throw matrix_size_error("Для выполнения операции умножения матрицы должны иметь согласованные размерности");
    }

    Matrix result_matrix = Matrix(matrix_to_multiply.cols, rows);

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
            result_matrix.data[index] = current_element;
        }
    }
    return result_matrix;
}


Matrix Matrix::operator*(const double multiplier) {
    std::cout << "вызван оператор умножения на скаляр" << std::endl;  // для разработчика

    Matrix result_matrix = Matrix(cols, rows);

    for(size_t index = 0; index < rows * cols; index++) {
        result_matrix.data[index] = data[index] * multiplier;
    }
    return result_matrix;
}


Matrix Matrix::exponential(unsigned int total_iterations) 
{
    std::cout << "вызвана операция экспененты" << std::endl;  // для разработчика
    // проверка что матрица квадратная
    if(cols != rows) {
        throw matrix_size_error("Для выполнения операции возведения в степень матрица должна быть квадратной");
    }
    
    Matrix result_matrix = Matrix(cols, rows);
    result_matrix.unit();  // результирующая матрица инициализируется как единичная
    Matrix prev_iteration_matrix = Matrix(cols, rows);  // матрица для хранения предыдущего слагаемого
    prev_iteration_matrix.unit();

    for(unsigned int iteration = 1; iteration < total_iterations; iteration++) {
        Matrix intermediate_matrix = prev_iteration_matrix * *this;
        prev_iteration_matrix = intermediate_matrix * (1/(float)iteration);
        result_matrix = result_matrix + prev_iteration_matrix;
    }
    return result_matrix;
}


double Matrix::det(double accuracy) 
{
    // проверка что матрица квадратная
    if(cols != rows) {
        throw matrix_size_error("Для вычисления определителя матрица должна быть квадратной");
        return NAN;
    }

    if(data == NULL) {
        throw matrix_data_error("Для вычисления определителя матрица не должна быть пустой");
        return NAN;
    }

    Matrix matrix = Matrix(cols, rows);
    size_t matrix_size = matrix.cols;

    memcpy(matrix.data, data, sizeof(double) * cols * rows);

    double det = 1.;

    for(size_t col = 0; col < matrix_size - 1; col++) {  // преобразование матрицы к треугольнику
        if(fabs(matrix.data[col * matrix_size + col]) < accuracy) {
            for(size_t row_to_add = col + 1; row_to_add < matrix_size; row_to_add++) {
                if(fabs(matrix.data[row_to_add * matrix_size + col])  > accuracy) {
                    matrix.matrix_row_add(col, row_to_add, 1);
                    break;
                }
                if(row_to_add == matrix_size - 1) {  // если в столбце все элементы равны 0, то определитель равен 0
                    return 0;
                }
            }
        }
        for(size_t row = col + 1; row < matrix_size; row++) {
            double multiplyer = matrix.data[row * matrix_size + col] / matrix.data[col * matrix_size + col];
            matrix.matrix_row_substract(row, col, multiplyer);
        }
    }

    for(size_t index = 0; index < matrix.rows; index++) {  // вычисление определителя
        det *= matrix.data[index * (matrix.rows + 1)];
    }

    if(fabs(det) < accuracy) {
        det = 0.0;
    }

    return det;
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

    Matrix matrix_C = matrix_A + matrix_B;
    //double det = matrix_A.det(0.00001);
    std::cout << std::endl;

    std::cout << matrix_A;
    std::cout << matrix_B;
    std::cout << matrix_C;
    //std::cout << det;
    //std::cout << std::endl;

    return 0;
}
