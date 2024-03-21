#include <iostream>
#include <stdlib.h>
#include <time.h>

//через ^ и текстовую строку сделать операции V, T, -1

enum Errors {
    ATTEMPT_TO_REINIT, SIZE_ZERO, SIZE_THRESHOLD_EXCEEDED, 
    WRONG_SIZES, NOT_INIT, NOT_SQUARE,
    WRONG_MINOR_INDEX, MINOR_SIZE_ZERO,
    INV_MAT_CALC_ERROR, DET_EQUAL_ZERO
};

void matrix_error_log(enum Errors code) {
    switch (code) {
    case ATTEMPT_TO_REINIT:
        std::cout << "ERROR: the matrix has already been initialised\n";
        return;
    case SIZE_ZERO:
        std::cout << "WARNING: the matrix size equal to zero\n";
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
    case NOT_SQUARE:
        std::cout << "ERROR: the matrix size isn't square\n";
        return;
    case WRONG_MINOR_INDEX:
        std::cout << "ERROR: the minor index is incorrect\n";
        return;
    case MINOR_SIZE_ZERO:
        std::cout << "ERROR: the minor size equal to zero\n";
        return;
    case INV_MAT_CALC_ERROR:
        std::cout << "ERROR: the inverse matrix can't be caclulated\n";
        return;
    case DET_EQUAL_ZERO:
        std::cout << "WARNING: the matrix determinant is equal to zero\n";
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
        //std::cout << "Вызван конструктор инициализации пустой матрицы\n";
        cols = 0;
        rows = 0;
        data = nullptr;
    }


    // конструктор
    Matrix(const size_t init_cols, const size_t init_rows)
    {
        //std::cout << "Вызван конструктор матрицы\n";

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

        if (data == nullptr) {
            cols = 0;
            rows = 0;
            matrix_error_log(NOT_INIT);
            return;
        }
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


    Matrix minor(const size_t minor_col, const size_t minor_row)
    {
        if ((cols<=1)||(rows<=1)) {
            Matrix null_matrix;
            matrix_error_log(MINOR_SIZE_ZERO);
            return null_matrix;
        }
        if ((minor_col>=cols)||(minor_row>=rows)) {
            Matrix null_matrix;
            matrix_error_log(WRONG_MINOR_INDEX);
            return null_matrix;
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
                minor.data[current_col * minor.rows + current_row] = data[origin_index];
                origin_index += 1;
            }
            if (minor_row == minor.rows) {  // иначе не обрабатывается последний столбец
                origin_index += 1;
            }
        }
        return minor;
    }
    
    
    double determinant()
    {
        if (cols != rows) {
            matrix_error_log(NOT_SQUARE);
            return NAN;
        }
        if ((cols == 0) || (rows == 0)) {
            matrix_error_log(SIZE_ZERO);
            return NAN;
        }

        double determinant = 0.0;
        if (cols == 1) {
            determinant = data[0];
            return determinant;
        }
        Matrix matrix_copy(cols,rows);
        for (size_t index = 0; index < cols * rows; index++) {
            matrix_copy.data[index] = data[index];
        }

        Matrix minor;
        for (size_t summ_index = 0; summ_index < rows; summ_index++) {
            minor = matrix_copy.minor(0, summ_index);

            if (summ_index % 2 == 0) {
                determinant += data[summ_index] * minor.determinant();
            }
            else {
                determinant -= data[summ_index] * minor.determinant();
            }
        }

        return determinant;
    }


    Matrix transposition()
    {
        if (cols*rows==0) {
            Matrix null_matrix;
            matrix_error_log(SIZE_ZERO);
            return null_matrix;
        }
        Matrix new_matrix(rows, cols);
        for (size_t current_col = 0; current_col < new_matrix.cols; current_col++) {
            for (size_t current_row = 0; current_row < new_matrix.rows; current_row++) {

                new_matrix.data[new_matrix.rows * current_col + current_row] =
                    data[new_matrix.cols * current_row + current_col];
            }
        }
        return new_matrix;
    }
    

    Matrix inverse()
    {
        if (cols != rows) {
            Matrix matrix_null;
            matrix_error_log(NOT_SQUARE);
            return matrix_null;
        }
        if (cols * rows == 0) {
            Matrix matrix_null;
            matrix_error_log(NOT_SQUARE);
            return matrix_null;
        }

        Matrix matrix_copy(cols, rows);
        for (size_t index = 0; index < cols * rows; index++) {
            matrix_copy.data[index] = data[index];
        }

        double det = matrix_copy.determinant();
        if (det == NAN) {
            Matrix matrix_null;
            matrix_error_log(INV_MAT_CALC_ERROR);
            return matrix_null;
        }
        Matrix inv_matrix(cols, rows);

        if (fabs(det) < 0.00001) {
            inv_matrix.zeros_fill();
            matrix_error_log(DET_EQUAL_ZERO);
            return inv_matrix;
        }
        Matrix minor;
        for (size_t current_col = 0; current_col < inv_matrix.cols; current_col++) {
            for (size_t current_row = 0; current_row < inv_matrix.rows; current_row++) {
                
                minor=matrix_copy.minor(current_col, current_row);

                if ((current_col + current_row) % 2 == 0) {
                    inv_matrix.data[current_col * inv_matrix.cols + current_row] = minor.determinant();
                }
                else {
                    inv_matrix.data[current_col * inv_matrix.cols + current_row] = (-1) * minor.determinant();
                }
            }
        }
        inv_matrix /= det;
        inv_matrix = inv_matrix.transposition();
        return inv_matrix;
    }


// деструктор
    ~Matrix()
    {
        //std::cout << "Вызван деструктор матрицы\n";
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
    friend Matrix operator + (const Matrix& matrix, const double number);
    friend Matrix operator - (const Matrix& A, const Matrix& B);
    friend Matrix operator - (const Matrix& matrix, const double number);
    friend Matrix operator * (const Matrix& A, const Matrix& B);
    friend Matrix operator * (const Matrix& matrix, const double number);
    friend Matrix operator ^ (const double base, const Matrix& matrix);
    friend Matrix operator ^ (const Matrix& matrix, const unsigned int exponent);

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
    

    Matrix& operator *= (const Matrix& matrix) 
    {
        if (rows != matrix.cols) {
            matrix_error_log(WRONG_SIZES);
            return *this;
        }
        size_t new_cols = cols;
        size_t new_rows = matrix.rows;
        double* new_data = new double[new_cols * new_rows];
        if (new_data == nullptr) {
            cols = 0;
            rows = 0;
            matrix_error_log(NOT_INIT);
            return *this;
        }

        for (size_t current_col = 0; current_col < new_cols; current_col++) {
            for (size_t current_row = 0; current_row < new_rows; current_row++) {
                new_data[new_rows * current_col + current_row] = 0.0;
                
                for (size_t index = 0; index < rows; index++) {

                    new_data[rows * current_col + current_row] +=
                        data[rows * current_col + index] *
                        matrix.data[matrix.rows * index + current_row];
                }
            }
        }
        cols = new_cols;
        rows = new_rows;
        delete[] data;
        data = new_data;

        return *this;
    }


    Matrix& operator /= (const double number) {
        for (size_t index = 0; index < cols * rows; index++) {
            data[index] /= number;
        }
        return *this;
    }
   };

std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
{
    out << "\n";
    for (size_t col = 0; col < matrix.cols; col++) {
        for (size_t row = 0; row < matrix.rows; row++) {
            if (fabs(matrix.data[col * matrix.rows + row] <= 0.00001)) {
                out << 0;
            }
            else {
                out << matrix.data[col * matrix.rows + row];
            }
            out << " ";
        }
        out << "\n";
    }
    return out;
}


Matrix operator + (const Matrix& A, const Matrix& B)
{
    if ((A.cols != B.cols) || (A.rows != B.rows)) {
        Matrix null_matrix;
        matrix_error_log(WRONG_SIZES);
        return null_matrix;
    }

    Matrix new_matrix(A.cols, B.cols);
    new_matrix = A;
    new_matrix += B;
    return new_matrix;
}


Matrix operator + (const Matrix& matrix, const double number)
{
    Matrix new_matrix;
    new_matrix = matrix;
    Matrix identity_matrix(matrix.cols, matrix.rows);
    identity_matrix.identity_fill();

    identity_matrix *= number;
    new_matrix += identity_matrix;
    return new_matrix;
}


Matrix operator - (const Matrix& matrix, const double number)
{
    Matrix new_matrix;
    new_matrix = matrix;
    Matrix identity_matrix(matrix.cols, matrix.rows);
    identity_matrix.identity_fill();

    identity_matrix *= number;
    new_matrix -= identity_matrix;
    return new_matrix;
}


Matrix operator - (const Matrix& A, const Matrix& B)
{
    if ((A.cols != B.cols) || (A.rows != B.rows)) {
        Matrix null_matrix;
        matrix_error_log(WRONG_SIZES);
        return null_matrix;
    }

    Matrix new_matrix(A.cols, B.cols);
    new_matrix = A;
    new_matrix -= B;
    return new_matrix;
}


Matrix operator * (const Matrix& A, const Matrix& B) {
    if (A.rows != B.cols) {
        Matrix null_matrix;
        matrix_error_log(WRONG_SIZES);
        return null_matrix;
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


Matrix operator * (const Matrix& matrix, const double number)
{
    Matrix new_matrix;
    new_matrix = matrix;
    new_matrix *= number;
    return new_matrix;
}


Matrix operator ^ (const double base, const Matrix& matrix)
{
    const unsigned int accuracy = 15;

    if (matrix.rows != matrix.cols) {
        Matrix null_matrix;
        matrix_error_log(NOT_SQUARE);
        return null_matrix;
    }

    Matrix log_matrix;
    log_matrix = matrix;
    log_matrix *= log(base);

    Matrix new_matrix(matrix.cols, matrix.rows);
    new_matrix.identity_fill();

    Matrix submatrix(matrix.cols, matrix.rows);
    submatrix.identity_fill();

    for (unsigned int index = 1; index < accuracy; index++) {
        submatrix *= log_matrix;
        submatrix /= index;
        new_matrix += submatrix;
    }
    return new_matrix;
}


Matrix operator ^ (const Matrix& matrix, const unsigned int exponent)
{
    if (matrix.rows != matrix.cols) {
        Matrix null_matrix;
        matrix_error_log(NOT_SQUARE);
        return null_matrix;
    }

    Matrix new_matrix(matrix.cols, matrix.rows); 
    new_matrix.identity_fill(); 
    if (exponent == 0) {
        return new_matrix;
    }
    for (unsigned int index = 1; index <= exponent; index++) {
        new_matrix *= matrix;
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

    Matrix B(6, 6);
    B.random_fill();
    std::cout << B;

    Matrix C;
    C = A+B;
    C = C ^ 2;
    std::cout << C;

    C = C * C.inverse();

    std::cout << C;

}
