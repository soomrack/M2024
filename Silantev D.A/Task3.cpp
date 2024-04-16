#include <iostream>
#include <cstdlib>
#include <cstring>


using namespace std;


class Matrix_Exception : public std::domain_error
{
public:
    Matrix_Exception(const char* const message) : std::domain_error(message) {}
};


class Matrix
{
private:
    size_t cols;
    size_t rows;
    double* value;

public:
    Matrix();
    Matrix(size_t col, size_t row);
    Matrix(const Matrix& matrix);
    Matrix(Matrix&& matrix);
    ~Matrix(); //вызывается после окончания программы

    void print();
    void fill_random(int max_value);
    Matrix& operator+(const Matrix& matrix) const; //переопределение операторов
    Matrix& operator-(const Matrix& matrix) const;
    Matrix& operator*(const Matrix& matrix) const;
    Matrix& operator*(double number) const;
    Matrix& operator=(Matrix& matrix);
    Matrix& operator=(Matrix&& matrix);
    Matrix& operator^(size_t number) const;
    Matrix& operator/(const double number) const;
    Matrix& exp(const size_t accuracy);
    Matrix& minor(Matrix& matrix1);
    Matrix& transp();
    void set_one();
    double& det(Matrix matrix);


};


Matrix::~Matrix() { //очистка памяти и матриц
    delete[] value;
    cols = 0;
    rows = 0;
    value = nullptr;
}


Matrix::Matrix() { //создание пустой матрицы
    cols = 0;
    rows = 0;
    value = nullptr;
}


Matrix::Matrix(size_t col, size_t row) { //вызывается этот конструктор, если передаём 2 значения
    cols = col;
    rows = row;
    if (rows > cols / sizeof(double) / SIZE_MAX) throw("Matrix overflow\n");
    value = new double[cols * rows * sizeof(double)];
}


Matrix::Matrix(const Matrix& matrix) {
    cols = matrix.cols;
    rows = matrix.rows;
    value = new double[rows * cols];
    memcpy(value, matrix.value, rows * cols * sizeof(double));
}


Matrix::Matrix(Matrix&& matrix) { //очищаем новую созданную матрицу
    cols = matrix.cols;
    rows = matrix.rows;
    value = matrix.value;
    matrix.value = nullptr;
    matrix.cols = 0;
    matrix.rows = 0;
}


void Matrix::print() {
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            cout << value[row * cols + col] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}


void Matrix::fill_random(int max_value = 10) {
    for (size_t index = 0; index < rows * cols; ++index) {
        value[index] = rand() % max_value;
    }
}


void Matrix::set_one() //создание единичной матрицы
{
    for (size_t row = 0; row < this->rows; row++) { //this относится к полю класса
        for (size_t col = 0; col < this->cols; col++) {
            value[row * this->cols + col] = (row == col) ? 1.0 : 0.0;
        }
    }
}


Matrix& Matrix::operator+ (const Matrix& matrix) const {

    if (rows != matrix.rows || cols != matrix.cols) throw ("Make matrix square\n");
    Matrix *result = new Matrix(matrix);

    for (size_t idx = 0; idx < matrix.cols * matrix.rows; idx++) {
        result->value[idx] += value[idx];
    }
    return *result;
}


Matrix& Matrix::operator- (const Matrix& matrix) const {
    if (rows != matrix.rows || cols != matrix.cols) throw ("Make matrix square\n");
    Matrix *result = new Matrix(matrix);

    for (size_t idx = 0; idx < matrix.cols * matrix.rows; idx++) {
        result->value[idx] -= value[idx];
    }
    return *result;
}


Matrix& Matrix::operator* (const Matrix& matrix) const {
    if (rows != matrix.rows || cols != matrix.cols) throw ("Make matrix square\n");
    Matrix *result = new Matrix(matrix);

    for (size_t row = 0; row < result->rows; row++) {
        for (size_t col = 0; col < result->cols; col++) {
            result->value[row * result->rows + col] = 0.00;
            for (size_t k = 0; k < result->cols; k++) {
                result->value[row * result->cols + col] += value[row * cols + k] * matrix.value[k * result->cols + col];
            }
        }
    }
    return *result;
}


Matrix& Matrix::operator* (const double coefficient) const {
    Matrix *result = new Matrix(*this);

    for (size_t idx = 0; idx < rows * cols; idx++) {
        result->value[idx] = value[idx] * coefficient;
    }
    return *result;
}


Matrix& Matrix::operator= (Matrix& matrix) {
    if (this == &matrix) {
        return *this;
    }
    rows = matrix.rows;
    cols = matrix.cols;
    if (rows * cols == matrix.rows * matrix.cols) {
        memcpy(value, matrix.value, rows * cols * sizeof(double));
        return *this;
    }
    delete[]value;
    value = new double[cols * rows];
    memcpy(value, matrix.value, rows * cols * sizeof(double));
    return *this;
}


Matrix& Matrix::operator= (Matrix&& matrix) {
    rows = matrix.rows;
    cols = matrix.cols;
    delete[]value;
    value = matrix.value;
    matrix.value = nullptr;
    return *this;
}


Matrix& Matrix::operator^ (size_t number) const {
    if (cols != rows) throw ("Make matrix square\n");
    Matrix *result = new Matrix(*this);

    if (number == 0) {
        result->set_one();
        return *result;
    }

    if (number == 1) {
        return *result;
    }

    const Matrix& start(*result);

    for (size_t idx = 0; idx < number; idx++) {
        *result = *result * *this;
    }
    return *result;
}


Matrix& Matrix::operator/(const double number) const {
    if (number == 0) throw ("Can't divide by zero\n");
    Matrix *result = new Matrix(*this);

    for (size_t idx = 0; idx < rows * cols; ++idx) {
        result->value[idx] = value[idx] / number;
    }
    return *result;
}


Matrix& Matrix::exp(const size_t accuracy = 30) {
    if (this->rows != this->cols) throw ("Make matrix square\n");
    
    Matrix *result = new Matrix(this->rows, this->cols);
    //Matrix *term = new Matrix(rows, cols);
    Matrix term(cols, rows);
    term.set_one();
    *result = term;

    for (int step = 1; step < accuracy; step++) {
        term = term * *this / step;
        *result = *result + term;
    }
    return *result;
}


Matrix& Matrix::minor(Matrix& A) {
   

    Matrix *result = new Matrix(A.rows, A.cols);
    size_t k = 0;

    for (size_t idx = 0; idx < rows * cols; idx++) {
        if ((idx % cols == A.cols) or (idx / cols == A.rows)) continue;
        result->value[k++] = A.value[idx];
    }

    return *result;
}

Matrix& Matrix::transp() {
    Matrix* result = new Matrix(this->rows, this->cols);

    for (size_t row = 0; row < result->rows; row++) {
        for (size_t col = 0; col < result->cols; col++) {
            result->value[row * result->cols + col] = value[col * result->cols + row];
        }
    }
    return *result;
}


int main() {
    Matrix matrix1(3, 3);
    matrix1.fill_random();
    matrix1.print();
    Matrix matrix2(3, 3);
    matrix2.fill_random();
    matrix2.print();
    Matrix sum = matrix1 + matrix2;
    sum.print();
    Matrix sub = matrix1 - matrix2;
    sub.print();
    Matrix mult = matrix1 * matrix2;
    mult.print();
    Matrix mult_number = matrix1 * 2;
    mult_number.print();
    Matrix pow = matrix1 ^ 2;
    pow.print();
    matrix1.exp(5);
    matrix1.print();
    return 0;
}  