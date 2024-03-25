#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Second_Task_log.c"



typedef float MatrixData;

typedef struct Matrix {
    size_t rows;                    //Строка
    size_t cols;                    //Столбец
    MatrixData *data;
} Matrix;

const struct Matrix NULL_MATRIX = { .cols = 0, .rows = 0, .data = NULL };      //Нулевая матрица

Matrix matrix_create(const size_t rows, const size_t cols) {

    if (rows == 0 || cols == 0) {
        return NULL_MATRIX;
    };

    //Проверяем переполнение памяти
    if (rows >= SIZE_MAX / sizeof(MatrixData) / cols) {
        log(ERROR, "Error. The amount of memory is exceeded!");
        return NULL_MATRIX;
    }

    // Выделение памяти
    Matrix Mtrx;
    Mtrx.data = (MatrixData*)malloc(rows * cols * sizeof(MatrixData));
    if (Mtrx.data == NULL) {
         log(ERROR, "Error. No memory allocated!");
        return NULL_MATRIX;
    }

    Mtrx.rows = rows;
    Mtrx.cols = cols;

    return Mtrx;
}

//Создание квадратичной матрицы
Matrix square_matrix(const size_t size) {
    return matrix_create(size, size);
}

//Очищение памяти
void free_matrix(Matrix *Mtrx) {
    Mtrx->rows = 0;
    Mtrx->cols = 0;
    free(Mtrx->data);
}

//Вывод матрицы
void print_matrix(const Matrix Mtrx) {

    if (Mtrx.rows == 0 || Mtrx.cols == 0) {
        log(ERROR,  "Error. The matrix is 0!");
        return;
    }

    for (size_t row = 0; row < Mtrx.rows; row++) {
        for (size_t col = 0; col < Mtrx.cols; col++) {
            printf("%.1f\t", Mtrx.data[row * Mtrx.cols + col]);
        }
        printf("\n");
    }
    printf("\n");
}

//Заполнение матрицы 
void filling_matrix(Matrix Mtrx) {
    for (size_t i = 0; i < Mtrx.rows * Mtrx.cols; i++) {
        Mtrx.data[i] = (i * 3 + 2) * 0.5;
    }
}

//Рандомное заполение матрицы
void random_matrix(Matrix Mtrx) {
    srand(time(NULL));
    for (size_t i = 0; i < Mtrx.rows * Mtrx.cols; i++) {
        Mtrx.data[i] = (MatrixData)rand() / RAND_MAX;
    }
}








//Сложение матриц
Matrix sum_matrix(const Matrix A, const Matrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {                    //Складываются только  матрицы одинакового размера
        log(WARNING, "Error. Adding the result only for a matrix of the same size!"); 
        return NULL_MATRIX;
    }

    Matrix result = matrix_create(A.rows, A.cols);
    if (result.data == NULL) {
        log(ERROR, "Error. No memory allocated!");
        return NULL_MATRIX;
    }

    for (size_t i = 0; i < A.rows * A.cols; i++) {
        result.data[i] = A.data[i] + B.data[i];
    }

    return result;
}

//Вывод матрицы А, B, A+B
void sum_print() {
    size_t rows = 3;
    size_t cols = 3;

    Matrix Mtrx1 = matrix_create(rows, cols);
    Matrix Mtrx2 = matrix_create(rows, cols);

    filling_matrix(Mtrx1);
    random_matrix(Mtrx2);

    print_matrix(Mtrx1);
    print_matrix(Mtrx2);

    Matrix result = sum_matrix(Mtrx1, Mtrx2);
    print_matrix(result);

    free_matrix(&Mtrx1);
    free_matrix(&Mtrx2);
    free_matrix(&result);
}





//Вычитание
Matrix sub_matrix(const Matrix A, const Matrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {                    
        log(WARNING, "Error. Adding the result only for a matrix of the same size!"); 
        return NULL_MATRIX;
    }

    Matrix result = matrix_create(A.rows, A.cols);
    if (result.data == NULL) {
        log(ERROR, "Error. No memory allocated!");
        return NULL_MATRIX;
    }

    for (size_t i = 0; i < A.rows * A.cols; i++) {
        result.data[i] = A.data[i] - B.data[i];
    }

    return result;
}

//Вывод матрицы А, B, A-B
void sub_print() {
    size_t rows = 3;
    size_t cols = 3;

    Matrix Mtrx1 = matrix_create(rows, cols);
    Matrix Mtrx2 = matrix_create(rows, cols);

    filling_matrix(Mtrx1);
    random_matrix(Mtrx2);

    print_matrix(Mtrx1);
    print_matrix(Mtrx2);

    Matrix result = sub_matrix(Mtrx1, Mtrx2);
    print_matrix(result);

    free_matrix(&Mtrx1);
    free_matrix(&Mtrx2);
    free_matrix(&result);
}





// Умножение матриц
Matrix mult_matrix(const Matrix A, const Matrix B) {
    if (A.cols != B.rows) {
        log(WARNING, "Error.Incorrect dimensions!");                  //Столбцы первой матрицы, должны быть равны строкам второй матрицы
        return NULL_MATRIX;
    }

    Matrix result = matrix_create(A.rows, B.cols);
    if (result.data == NULL) {
        log(ERROR, "Error. No memory allocated!");
        return NULL_MATRIX;
    }

    for (size_t row = 0; row < A.rows; row++) {
        for (size_t col = 0; col < B.cols; col++) 
        {
            MatrixData multi_sum = 0;
            for (size_t k = 0; k < A.cols; k++) {
                multi_sum += A.data[row * A.cols + k] * B.data[k * B.cols + col];
            }   
            result.data[row * B.cols + col] = multi_sum;
        }
    }
    return result;
}

//Вывод матриц A,B, A*B
void mult_print() {
    size_t Mtrx1_rows = 3;
    size_t Mtrx1_cols = 2;

    size_t Mtrx2_rows = 2;
    size_t Mtrx2_cols = 3;

    Matrix Mtrx1 = matrix_create(Mtrx1_rows, Mtrx1_cols);
    Matrix Mtrx2 = matrix_create(Mtrx2_rows, Mtrx2_cols);

    filling_matrix(Mtrx1);
    random_matrix(Mtrx2);

    print_matrix(Mtrx1);
    print_matrix(Mtrx2);

    Matrix result = mult_matrix(Mtrx1, Mtrx2);
    print_matrix(result);

    free_matrix(&Mtrx1);
    free_matrix(&Mtrx2);
    free_matrix(&result);
}



// Умножение матрицу на скаляр
Matrix mult_scalar_matrix(const Matrix Mtrx, float scalar) {
    if (Mtrx.rows == 0 || Mtrx.cols == 0) {
       log(ERROR, "Error. The row or column is 0!");
        return NULL_MATRIX;
    };

    Matrix result = matrix_create(Mtrx.rows, Mtrx.cols);
    if (result.data == NULL) {
        log(ERROR, "Error. No memory allocated!");
        return NULL_MATRIX;
    }

    for (size_t i = 0; i < Mtrx.rows * Mtrx.cols; i++) {
        result.data[i] = Mtrx.data[i] * scalar;
    }
    return result;
}

//Вывод матриц A, A*scalar
void mult_scalar_print() {
    size_t rows = 3;
    size_t cols = 3;
    float scalar = 2;

    Matrix Mtrx1 = matrix_create(rows, cols);

    filling_matrix(Mtrx1);

    print_matrix(Mtrx1);

    Matrix result = mult_scalar_matrix(Mtrx1, scalar);
    print_matrix(result);

    free_matrix(&Mtrx1);
    free_matrix(&result);
}



//Вычисление определителя матрицы
MatrixData determinant_matrix(const Matrix Mtrx) {
    if (Mtrx.rows == 0 || Mtrx.cols == 0) {
        log(ERROR, "Error. The matrix is 0!");
        return 0;
    }

    if (Mtrx.rows != Mtrx.cols) {
        log(WARNING, "Error. The matrix must be square!");
        return 0;
    }

    size_t size = Mtrx.rows;
    if (size == 1) {
        return Mtrx.data[0];
    }

    float determinant = 0;

    Matrix new_mtrx = square_matrix(size - 1);
    if (new_mtrx.data == NULL) {
        log(ERROR, "Error. No memory allocated!");
        return 0;
    }

    for (size_t i = 0; i < size; i++) {
        size_t new_row = 0;
        for (size_t row = 1; row < size; row++) {
            size_t new_col = 0;
            for (size_t col = 0; col < size; col++) {
                if (col == i) {
                    continue;
                }
                new_mtrx.data[new_row * new_mtrx.cols + new_col] = Mtrx.data[row * Mtrx.cols + col];
                new_col++;
            }
            new_row++;
        }

        short sign = (i % 2 == 0) ? 1 : -1;
        MatrixData minor = determinant_matrix(new_mtrx);
        determinant += sign * Mtrx.data[i] * minor;
    }

    free_matrix(&new_mtrx);

    return determinant;
}

//Вывод матрицы и ее определителя
void determinant_print() {
    size_t size = 3;

    Matrix matrix = square_matrix(size);
    random_matrix(matrix);
    print_matrix(matrix);

    MatrixData determinant = determinant_matrix(matrix);
    printf("Determinant = %.1f\n", determinant);

    free_matrix(&matrix);
}



//Экспон. матрица
Matrix exp_matrix(const Matrix Mtrx, int N_iteration) {
    if (Mtrx.rows == 0 || Mtrx.cols == 0) {
        log(ERROR, "Error. The matrix is 0!");
        return Mtrx;
    }

    if (Mtrx.rows != Mtrx.cols) {
        log(WARNING, "The matrix must be square!");
        return Mtrx;
    }

    size_t size = Mtrx.rows;

    Matrix result = square_matrix(size);
    if (result.data == NULL) {
        log(ERROR, "Error. Result is NULL!");
        return NULL_MATRIX;
    }

    Matrix temp = square_matrix(size);
    if (temp.data == NULL) {
        log(ERROR, "Error. Temp is NULL!");
        free_matrix(&result);
        return NULL_MATRIX;
    }

    size_t allo_mem_size = size * size * sizeof(MatrixData);
    memset(result.data, 0, allo_mem_size);
    memset(temp.data, 0, allo_mem_size);
    for (size_t i = 0; i < size; i++) {
        result.data[i * size + i] = 1.;
        temp.data[i * size + i] = 1.;
    }

    for (int iteration = 1; iteration <= N_iteration; iteration++) {
        Matrix copy_ptr = temp;
        temp = mult_matrix(temp, Mtrx);
        free_matrix(&copy_ptr);

        copy_ptr = temp;
        temp = mult_scalar_matrix(temp, 1.0 / iteration);
        free_matrix(&copy_ptr);

        copy_ptr = result;
        result = sum_matrix(result, temp);
        free_matrix(&copy_ptr);
        free_matrix(&temp);
    }

    free_matrix(&temp);

    return result;
}


void case_exp() {
    int size = 2; 
    Matrix matrix = square_matrix(size);

    matrix.data[0] = 1; matrix.data[1] = 2;
    matrix.data[2] = 3; matrix.data[3] = 4;

    Matrix expMat = exp_matrix(matrix, 8);

    log(INFO, "Exponential of the matrix:");
    print_matrix(expMat);

    free_matrix(&matrix);
    free_matrix(&expMat);
}


// TODO OBSOLETE
void case_test_exp() {

    size_t size = 3;

    Matrix A = square_matrix(size);
    filling_matrix(A);
    print_matrix(A);

    Matrix Exp1 = exp_matrix(A, 8);
    print_matrix(Exp1);
}



int main() {
    
    sum_print();

    return 0;
}