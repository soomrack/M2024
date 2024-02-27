// Liubinetskii V. (@liubvlad)
// 
//
// ☑ Добавить базовые операции с матрицей
//
// ☑ Расчёт определителя
// (+-) ☑Экспонента матрицы (https://ru.wikipedia.org/wiki/%D0%AD%D0%BA%D1%81%D0%BF%D0%BE%D0%BD%D0%B5%D0%BD%D1%82%D0%B0_%D0%BC%D0%B0%D1%82%D1%80%D0%B8%D1%86%D1%8B)
//
// ☑snake_case
// ☑size_t check
// ☑free on *
// ☑для цикла переменные idx, row, col.. etc
// ☑consts params
// ☑названия переменых массивов передаваемых в функцию A и B (M - для одной)
// ☑exit(1) - нельзя (NULL matrix)
//
// 
// ☑проверять что создаются матрицы
// ☑избавитья от указателя матрицы
// ☑copy_ptr в экспоненте, вычещать
// ☑убрать проверку при сложении и тд для сложении Матрица(0,5)
// 
// ☑не считать факториал
// ☑переименовать функции
// ☑матриц дада тип . МатрикцДада
// ☑Переменная для макс итераций
// 
// ☑Логгер

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logger.h"

typedef double MatrixData; // possible: float, double, long double

typedef struct Matrix {
    size_t rows;
    size_t cols;
    MatrixData *data;
} Matrix;


const struct Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL };


Matrix matrix_create(const size_t rows, const size_t cols) {

    if (rows == 0 || cols == 0) {
        return MATRIX_NULL;
    };

    // Verify on overflow
    if (rows >= SIZE_MAX / sizeof(MatrixData) / cols) {
        log(ERROR, "Required memory amount went beyond the data type's capacity.");
        return MATRIX_NULL;
    }

    // Allocate memory for the matrix data
    Matrix M;
    M.data = (MatrixData*)malloc(rows * cols * sizeof(MatrixData));
    if (M.data == NULL) {
        log(ERROR, "Memory allocation for data failed.");
        return MATRIX_NULL;
    }

    M.rows = rows;
    M.cols = cols;

    return M;
}


Matrix matrix_create_square(const size_t size) {
    return matrix_create(size, size);
}


void matrix_free(Matrix *M) {
    M->rows = 0;
    M->cols = 0;
    free(M->data);
    free(M);
}


// Function to sum two matrices
Matrix matrix_sum(const Matrix A, const Matrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        log(WARNING, "Matrices have different dimensions.");
        return MATRIX_NULL;
    }

    Matrix result = matrix_create(A.rows, A.cols);
    if (result.data == NULL) {
        log(ERROR, "Watchout! Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    for (size_t idx = 0; idx < A.rows * A.cols; idx++) {
        result.data[idx] = A.data[idx] + B.data[idx];
    }

    return result;
}


// Function to subtract two matrices
Matrix matrix_subtract(const Matrix A, const Matrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        log(WARNING, "Matrices have different dimensions.");
        return MATRIX_NULL;
    }

    Matrix result = matrix_create(A.rows, A.cols);
    if (result.data == NULL) {
        log(ERROR, "Watchout! Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    for (size_t idx = 0; idx < A.rows * A.cols; idx++) {
        result.data[idx] = A.data[idx] - B.data[idx];
    }

    return result;
}


// Function to multiply two matrices
Matrix matrix_multiply(const Matrix A, const Matrix B) {
    if (A.cols != B.rows) {
        log(WARNING, "Matrices cannot be multiplied due to incompatible dimensions.");
        return MATRIX_NULL;
    }

    Matrix result = matrix_create(A.rows, B.cols);
    if (result.data == NULL) {
        log(ERROR, "Watchout! Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    for (size_t row = 0; row < A.rows; row++) {
        for (size_t col = 0; col < B.cols; col++) {
            MatrixData sum = 0;
            for (size_t offset = 0; offset < A.cols; offset++) {
                sum += A.data[row * A.cols + offset] * B.data[offset * B.cols + col];
            }
            result.data[row * B.cols + col] = sum;
        }
    }
    return result;
}


// Function to multiply a matrix by a scalar
Matrix matrix_multiply_by_scalar(const Matrix M, double scalar) {
    if (M.rows == 0 || M.cols == 0) {
        log(ERROR, "One of dimensions of multiply matrix on scalar has value 0.");
        return MATRIX_NULL;
    };

    Matrix result = matrix_create(M.rows, M.cols);
    if (result.data == NULL) {
        log(ERROR, "Watchout! Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    for (size_t idx = 0; idx < M.rows * M.cols; idx++) {
        result.data[idx] = M.data[idx] * scalar;
    }
    return result;
}


Matrix matrix_exponential(const Matrix M, int iteration_count) {
    if (M.rows == 0 || M.cols == 0) {
        log(ERROR, "Matrix should be size more then zero!");
        return M;
    }

    if (M.rows != M.cols) {
        log(WARNING, "Exponential can only be calculated for sqare matrices.");
        return M;
    }

    size_t size = M.rows;

    Matrix result = matrix_create_square(size);
    if (result.data == NULL) {
        log(ERROR, "Watchout! Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    Matrix temp = matrix_create_square(size);
    if (temp.data == NULL) {
        log(ERROR, "Watchout! Data matrix of 'temp' is NULL!");
        matrix_free(&result);
        return MATRIX_NULL;
    }
    
    // Инициализируем как единичные матрицы
    size_t allo_mem_size = size * size * sizeof(MatrixData);
    memset(result.data, 0, allo_mem_size);
    memset(temp.data, 0, allo_mem_size);
    for (size_t idx_diag = 0; idx_diag < size; idx_diag++) {
        result.data[idx_diag * size + idx_diag] = 1.;
        temp.data[idx_diag * size + idx_diag] = 1.;
    }

    // Расчёт матричного экспоненциального ряда
    for (int iteration = 1; iteration <= iteration_count; iteration++) {
        Matrix copy_ptr = temp;
        temp = matrix_multiply(temp, M);
        matrix_free(&copy_ptr);

        Matrix copy_ptr = temp;
        temp = matrix_multiply_by_scalar(temp, 1.0 / iteration);
        matrix_free(&copy_ptr);

        Matrix copy_ptr = result;
        result = matrix_sum(result, temp);
        matrix_free(&copy_ptr);
        matrix_free(&temp);
    }

    matrix_free(&temp);

    return result;
}


// Function to calculate the determinant of matrix by minors
MatrixData matrix_determinant_of_by_minors(const Matrix M) {
    if (M.rows == 0 || M.cols == 0) {
        log(ERROR, "Matrix should be size more then zero!");
        return 0;
    }

    if (M.rows != M.cols) {
        log(WARNING, "Determinant can only be calculated for sqare matrices.");
        return 0;
    }

    if (M.rows > 7) {
        log(WARNING, "Can't be calculated for a matrix of dimensions greater than 7.");
        return 0;
    }

    size_t size = M.rows;
    if (size == 1) {
        return M.data[0];
    }

    MatrixData det = 0;
    Matrix submat = matrix_create_square(size - 1);
    if (submat.data == NULL) {
        log(ERROR, "Watchout! Data matrix of 'submat' is NULL!");
        return 0;
    }

    for (size_t idx = 0; idx < size; idx++) {
        size_t sub_row = 0;
        for (size_t row = 1; row < size; row++) {
            size_t sub_col = 0;
            for (size_t col = 0; col < size; col++) {
                if (col == idx) {
                    continue;
                }

                submat.data[sub_row * submat.cols + sub_col] = M.data[row * M.cols + col];
                sub_col++;
            }

            sub_row++;
        }

        short sign = (idx % 2 == 0) ? 1 : -1;
        MatrixData subdet = matrix_determinant_of_by_minors(submat);
        det += sign * M.data[idx] * subdet;
    }

    matrix_free(&submat);

    return det;
}


void matrix_print(const Matrix M) {
    log(INFO, "Matrix:");

    if (M.rows == 0 || M.cols == 0) {
        log(ERROR, "Matrix is null.\n");
        return;
    }

    for (size_t row = 0; row < M.rows; row++) {
        for (size_t col = 0; col < M.cols; col++) {
            printf("%.2f\t", M.data[row * M.cols + col]);
        }

        printf("\n\n");
    }
}


// Function to fill a matrix (in progress)
void matrix_fill(Matrix M) {
    for (size_t idx = 0; idx < M.rows * M.cols; idx++) {
        M.data[idx] = idx * 2 + 1;
    }
}


// Function to fill a matrix with random numbers
void matrix_fill_random(Matrix M) {
    srand(time(NULL));
    for (size_t idx = 0; idx < M.rows * M.cols; idx++) {
        M.data[idx] = (MatrixData)rand() / RAND_MAX;
    }
}

// *************************

void case_sum_and_print() {
    size_t rows = 2;
    size_t cols = 3;

    Matrix mat1 = matrix_create(rows, cols);
    Matrix mat2 = matrix_create(rows, cols);

    matrix_fill(mat1);
    matrix_fill_random(mat2);

    matrix_print(mat1);
    matrix_print(mat2);

    Matrix result = matrix_sum(mat1, mat2);
    matrix_print(result);

    matrix_free(&mat1);
    matrix_free(&mat2);
    matrix_free(&result);
}


void case_prepair_matrix_and_get_determinant() {
    size_t size = 3;

    Matrix matrix = matrix_create_square(size);
    matrix.data[0] = 1; matrix.data[1] = 2; matrix.data[2] = 3;
    matrix.data[3] = 4; matrix.data[4] = 5; matrix.data[5] = 6;
    matrix.data[6] = 7; matrix.data[7] = 8; matrix.data[8] = 9;

    matrix_print(matrix);

    MatrixData det = matrix_determinant_of_by_minors(matrix);
    printf("Determinant of the matrix: %.2f\n", det);

    matrix_free(&matrix);
}


void case_get_matrix_determinant() {
    size_t size = 3;

    Matrix matrix = matrix_create_square(size);
    matrix_fill_random(matrix);
    matrix_print(matrix);

    MatrixData det = matrix_determinant_of_by_minors(matrix);
    printf("Determinant of the matrix: %.2f\n", det);

    matrix_free(&matrix);
}


void case_exp() {
    int size = 2; 
    Matrix matrix = matrix_create_square(size);

    matrix.data[0] = 1; matrix.data[1] = 2;
    matrix.data[2] = 3; matrix.data[3] = 4;

    Matrix expMat = matrix_exponential(matrix, 8);

    log(INFO, "Exponential of the matrix:");
    matrix_print(expMat);

    matrix_free(&matrix);
    matrix_free(&expMat);
}


// TODO OBSOLETE
void case_test_exp() {

    size_t size = 3;

    Matrix A = matrix_create_square(size);
    matrix_fill(A);
    matrix_print(A);

    Matrix Exp1 = matrix_exponential(A, 8);
    matrix_print(Exp1);
}


int main() {
    
    case_test_exp();

    return 0;
}
