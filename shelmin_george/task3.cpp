#include <iostream>
#include <stdlib.h>
#include <time.h>

// создать свои конструкторы и деструкторы
// переопределить математические операции, присваивание и оператор вывода <<
// переопределить ^ как возведение числа в степень матрицы (a^matrix = e^(matrix*ln(a))
enum Errors {
    ATTEMPT_TO_REINIT, SIZE_ZERO, SIZE_THRESHOLD_EXCEEDED, NOT_INIT,
    WRONG_SIZES
};

void matrix_error_log(enum Errors code) {
    switch (code) {
    case ATTEMPT_TO_REINIT:
        std::cout << "ERROR: the matrix has already been initialised\n";
        return;
    case SIZE_ZERO:
        std::cout << "WARNING: the matrix size is zero\n";
        return;
    case SIZE_THRESHOLD_EXCEEDED:
        std::cout << "ERROR: the matrix size too big\n";
        return;
    case WRONG_SIZES:
        std::cout << "ERROR: the matrix sizes are incomparable\n";
        return;
    case NOT_INIT:
        std::cout << "ERROR: the matrix wasn't initialised\n";
        return;
    }

};


class Matrix {
private:
    size_t cols;
    size_t rows;
    double* data = nullptr;

public:
    // конструктор пустой матрицы
    Matrix()
    {
        std::cout << "Вызван конструктор инициализации пустой матрицы\n";
        cols = 0;
        rows = 0;
        data = nullptr;
    }


    // конструктор
    Matrix(const size_t init_cols, const size_t init_rows)
    {
        std::cout << "Вызван конструктор матрицы\n";

        if (init_cols * init_rows == 0) {
            cols = 0;
            rows = 0;
            data = nullptr;
            matrix_error_log(SIZE_ZERO);
            return;
        }

        long double size_check = SIZE_MAX / init_cols / init_rows / sizeof(double);
        if (size_check < 1.0) {
            cols = 0;
            rows = 0;
            data = nullptr;
            matrix_error_log(SIZE_THRESHOLD_EXCEEDED);
            return;
        }

        cols = init_cols;
        rows = init_rows;
        data = new double[cols * rows];
    }


    void random_fill() {
        for (size_t index = 0; index < cols * rows; index++) {
            data[index] = double(rand() % 1000000) / 10000;
        }
    }


    void zeros_fill() {
        for (size_t index = 0; index < cols * rows; index++) {
            data[index] = 0.0;
        }
    }


    void identity_fill() {
        zeros_fill();
        for (size_t index = 0; index < rows * cols; index += rows + 1) {
            data[index] = 1.0;
        }
    }



// деструктор
    ~Matrix()
    {
        std::cout << "Вызван деструктор матрицы\n";
        if (data==nullptr){
            return;
        }
        cols = 0;
        rows = 0;
        delete[] data;
        data = nullptr;
    }
    

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    friend Matrix operator + (const Matrix& A, const Matrix& B);
    friend Matrix operator - (const Matrix& A, const Matrix& B);
    friend Matrix operator * (const Matrix& A, const Matrix& B);

    Matrix& operator = (const Matrix& matrix) 
    {
        if (matrix.cols * matrix.rows == 0) {
            cols = 0;
            rows = 0;
            data = nullptr;
            matrix_error_log(SIZE_ZERO);
            return *this;
        }

        delete[] data;
        data = nullptr;
        cols = matrix.cols;
        rows = matrix.rows;
        data = new double[cols * rows];
        for (size_t index = 0; index < cols * rows; index++) {
            data[index] = matrix.data[index];
        }
        return *this;
    };
    
    
    Matrix& operator += (const Matrix& matrix)
    {
        if ((cols!=matrix.cols)||(rows!=matrix.rows)) {
            matrix_error_log(WRONG_SIZES);
            return *this;
        }
        for (size_t index = 0; index < cols * rows; index++) {
            data[index] += matrix.data[index];
        }
        return *this;
    }
    
    
    Matrix& operator += (const double number) {
        for (size_t index = 0; index < cols * rows; index++) {
            data[index] += number;
        }
        return *this;
    }
    
    
    Matrix& operator -= (const Matrix& matrix)
    {
        if ((cols != matrix.cols) || (rows != matrix.rows)) {
            matrix_error_log(WRONG_SIZES);
            return *this;
        }
        for (size_t index = 0; index < cols * rows; index++) {
            data[index] -= matrix.data[index];
        }
        return *this;
    }
    
    
    Matrix& operator -= (const double number) {
        for (size_t index = 0; index < cols * rows; index++) {
            data[index] -= number;
        }
        return *this;
    }
    
    
    Matrix& operator *= (const double number) {
        for (size_t index = 0; index < cols * rows; index++) {
            data[index] *= number;
        }
        return *this;
    }
    


};

std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
{
    out << "\n";
    for (size_t col = 0; col < matrix.cols; col++) {
        for (size_t row = 0; row < matrix.rows; row++) {
            out << matrix.data[col * matrix.rows + row];
            out << " ";
        }
        out << "\n";
    }
    return out;
}


Matrix operator + (const Matrix& A, const Matrix& B)
{
    if ((A.cols != B.cols) || (A.rows != B.rows)) {
        Matrix new_matrix(0, 0);
        matrix_error_log(WRONG_SIZES);
        return new_matrix;
    }

    Matrix new_matrix(A.cols, B.cols);
    new_matrix = A;
    new_matrix += B;
    return new_matrix;
}


Matrix operator - (const Matrix& A, const Matrix& B)
{
    if ((A.cols != B.cols) || (A.rows != B.rows)) {
        Matrix new_matrix(0, 0);
        matrix_error_log(WRONG_SIZES);
        return new_matrix;
    }

    Matrix new_matrix(A.cols, B.cols);
    new_matrix = A;
    new_matrix -= B;
    return new_matrix;
}


Matrix operator * (const Matrix& A, const Matrix& B) {
    if (A.rows != B.cols) {
        Matrix new_matrix(0, 0);
        matrix_error_log(WRONG_SIZES);
        return new_matrix;
    }
    Matrix new_matrix(A.cols, B.rows);
    new_matrix.zeros_fill();
    for (size_t current_col = 0; current_col < new_matrix.cols; current_col++) {
        for (size_t current_row = 0; current_row < new_matrix.rows; current_row++) {

            for (size_t index = 0; index < A.rows; index++) {

                new_matrix.data[new_matrix.rows * current_col + current_row] +=
                    A.data[A.rows * current_col + index] *
                    B.data[B.rows * index + current_row];
            }
        }
    }
    return new_matrix;
}


int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "rus");

    Matrix A(6, 6);
    A.random_fill();
    std::cout << A;

    Matrix B(6, 8);
    B.identity_fill();
    std::cout << B;

    Matrix C(5, 6);
    C = A * B;

    std::cout << C;
}
