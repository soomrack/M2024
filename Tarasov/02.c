#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

enum ErrorType { ERROR };

void matrix_error(enum ErrorType type, const char* message)
{
    if (type == ERROR) {
        printf("ОШИБКА! %s\n", message);
    }
}

struct Matrix {
    size_t cols;
    size_t rows;
    double* data;
};

typedef struct Matrix Matrix;

void matrix_locate(Matrix* matrix, size_t rows, size_t cols) // выделение памяти
{
    matrix->rows = rows;
    matrix->cols = cols;

    if (rows * cols == 0) {
        matrix_error(ERROR, "Нулевые размеры матрицы");
        matrix->data = NULL;
        return;
    }

    if (SIZE_MAX / cols / rows < sizeof(double)) {
        matrix_error(ERROR, "Матрица превышает допустимый размер");
        matrix->data = NULL;
        return;
    }

    matrix->data = (double*)malloc(rows * cols * sizeof(double));

    if (matrix->data == NULL) {
        matrix_error(ERROR, "Ошибка выделения памяти");
    }
}

// освобождение памяти для хранения матрицы
void matrix_free(Matrix* matrix)
{
    free(matrix->data);
    matrix->data = NULL;
    matrix->cols = 0;
    matrix->rows = 0;
}

void matrix_print(Matrix matrix)
{
    for (size_t row = 0; row < matrix.rows; row++) {
        for (size_t col = 0; col < matrix.cols; col++) {
            printf("%f ", matrix.data[row * matrix.cols + col]);
        }
        printf("\n");
    }
}

void matrix_add(Matrix A, Matrix B, Matrix* result)
{
    if (A.rows != B.rows || A.cols != B.cols) {
        matrix_error(ERROR, "Размеры матриц не совпадают");
        matrix_locate(result, 0, 0);
        return;
    }

    matrix_locate(result, A.rows, A.cols);

    if (result->data == NULL) {
        return;
    }

    for (size_t ind = 0; ind < A.rows * A.cols; ind++) {
        result->data[ind] = A.data[ind] + B.data[ind];
    }
}

void matrix_sub(Matrix A, Matrix B, Matrix* result)
{
    if (A.rows != B.rows || A.cols != B.cols) {
        matrix_error(ERROR, "Размеры матриц не совпадают");
        matrix_locate(result, 0, 0);
        return;
    }

    matrix_locate(result, A.rows, A.cols);

    if (result->data == NULL) {
        return;
    }

    for (size_t ind = 0; ind < A.rows * A.cols; ind++) {
        result->data[ind] = A.data[ind] - B.data[ind];
    }
}

void matrix_mul(Matrix A, Matrix B, Matrix* result)
{
    if (A.data == NULL || B.data == NULL) {
        matrix_error(ERROR, "Одна из матриц пустая");
        matrix_locate(result, 0, 0);
        return;
    }

    if (A.cols != B.rows) {
        matrix_error(ERROR, "Количество столбцов матрицы A отличается от количества строк матрицы B");
        matrix_locate(result, 0, 0);
        return;
    }

    matrix_locate(result, A.rows, B.cols);

    for (size_t row_A = 0; row_A < A.rows; row_A++) {
        for (size_t col_B = 0; col_B < B.cols; col_B++) {
            double sum = 0;
            for (size_t col_A = 0; col_A < A.cols; col_A++) {
                sum += A.data[row_A * A.cols + col_A] * B.data[col_A * B.cols + col_B];
            }
            result->data[row_A * result->cols + col_B] = sum;
        }
    }
}

void matrix_mul_scalar(Matrix matrix, double scalar, Matrix* result)
{
    matrix_locate(result, matrix.rows, matrix.cols);

    for (size_t ind = 0; ind < matrix.rows * matrix.cols; ind++) {
        result->data[ind] = matrix.data[ind] * scalar;
    }
}

// единичная матрица
void matrix_identity(Matrix* matrix, size_t rows, size_t cols)
{
    matrix_locate(matrix, rows, cols);

    if (matrix->data == NULL) {
        return;
    }

    if (matrix->cols != matrix->rows) {
        matrix_error(ERROR, "Матрица должна быть квадратной");
        matrix_free(matrix);
        return;
    }

    for (size_t row = 0; row < matrix->rows; ++row) {
        for (size_t col = 0; col < matrix->cols; ++col) {
            if (row == col) {
                matrix->data[row * matrix->cols + col] = 1.0;
            }
            else {
                matrix->data[row * matrix->cols + col] = 0.0;
            }
        }
    }
}

// e^A
void matrix_exp(Matrix matrix, const size_t iteration, Matrix* result)
{
    if (matrix.data == NULL) {
        matrix_error(ERROR, "Матрица не должна быть пустой");
        matrix_locate(result, 0, 0);
        return;
    }

    if (matrix.cols != matrix.rows) {
        matrix_error(ERROR, "Матрица должна быть квадратной");
        matrix_locate(result, 0, 0);
        return;
    }

    matrix_identity(result, matrix.rows, matrix.cols);

    Matrix B;
    matrix_identity(&B, matrix.rows, matrix.cols);

    if (result->data == NULL || B.data == NULL) {
        matrix_error(ERROR, "Не удалось создать матрицу");
        matrix_free(result);
        matrix_free(&B);
        matrix_locate(result, 0, 0);
        return;
    }

    for (size_t k = 1; k <= iteration; ++k) {
        Matrix B_shadow = B;

        matrix_mul(B_shadow, matrix, &B);
        matrix_free(&B_shadow);

        matrix_mul_scalar(B, 1.0 / k, &B_shadow);
        matrix_free(&B);
        B = B_shadow;

        Matrix result_shadow = *result;
        matrix_add(result_shadow, B, result);
        matrix_free(&result_shadow);
    }

    matrix_free(&B);
}

double matrix_det(Matrix matrix)
{
    if (matrix.data == NULL) {
        matrix_error(ERROR, "Матрица не должна быть пустой");
        return NAN;
    }

    if (matrix.cols == 0) {
        matrix_error(ERROR, "Количество столбцов матрицы равно нулю");
        return NAN;
    }

    if (matrix.cols != matrix.rows) {
        matrix_error(ERROR, "Матрица должна быть квадратной");
        return NAN;
    }

    if (matrix.cols == 1) {
        return matrix.data[0];
    }

    double det = 0;

    if (matrix.cols == 2) {
        det = (matrix.data[0]) * (matrix.data[3]) - (matrix.data[1]) * (matrix.data[2]);
        return det;
    }

    if (matrix.cols == 3) {
        det = (matrix.data[0]) * (matrix.data[4]) * (matrix.data[8]) + (matrix.data[1]) * (matrix.data[5]) * (matrix.data[6]) + (matrix.data[3]) * (matrix.data[7]) * (matrix.data[2]);
        det -= ((matrix.data[2]) * (matrix.data[4]) * (matrix.data[6]) + (matrix.data[1]) * (matrix.data[3]) * (matrix.data[8]) + (matrix.data[0]) * (matrix.data[5]) * (matrix.data[7]));
        return det;
    }

    return NAN;
}

void matrix_from_array(Matrix* matrix, double* array)
{
    if (matrix == NULL || array == NULL) {
        matrix_error(ERROR, "Переданы некорректные параметры");
        return;
    }

    matrix_locate(matrix, matrix->rows, matrix->cols);

    if (matrix->data == NULL) {
        return;
    }

    for (size_t i = 0; i < matrix->rows * matrix->cols; i++) {
        matrix->data[i] = array[i];
    }
}

double matrix_min_elem(Matrix matrix) {
    if (matrix.data == NULL) {
        matrix_error(ERROR, "Матрица не должна быть пустой");
        return NAN;
    }

    if (matrix.rows == 0 || matrix.cols == 0) {
        matrix_error(ERROR, "Неверный размер матрицы");
        return NAN; 
    }

    double min_elem = matrix.data[0];

    for (size_t i = 1; i < matrix.rows * matrix.cols; ++i) {
        if (matrix.data[i] < min_elem) {
            min_elem = matrix.data[i];
        }
    }

    return min_elem;
}

int main()
{
    Matrix A;
    matrix_locate(&A, 3, 3);
    matrix_from_array(&A, (double[]) { 1, 1, 1, 1, 1, 1, 1, 1, 1 });

    Matrix B;
    matrix_locate(&B, 3, 3);
    matrix_from_array(&B, (double[]) { 5, -2, 1, 0, 2, 3, -1, 4, 7 });

    Matrix C;
    matrix_locate(&C, 3, 3);

    double det_A = matrix_det(A);
    double det_B = matrix_det(B);
    double min_A = matrix_min_elem(A);
    double min_B = matrix_min_elem(B);

    printf("Matrix A:\n");
    matrix_print(A);
    printf("\nMatrix B:\n");
    matrix_print(B);

    printf("\nMatrix addition A and B:\n");
    matrix_add(A, B, &C);
    matrix_print(C);

    printf("\nMatrix subtraction A and B:\n");
    matrix_sub(A, B, &C);
    matrix_print(C);

    printf("\nMatrix multiply A and B:\n");
    matrix_mul(A, B, &C);
    matrix_print(C);

    printf("\nMatrix A multiply by number 3:\n");
    matrix_mul_scalar(A, 3, &C);
    matrix_print(C);

    printf("\nMatrix expo A:\n");
    matrix_exp(A, 10, &C);
    matrix_print(C);

    printf("\nMatrix det A: %f\n", det_A);
    printf("Matrix det B: %f\n", det_B);

    printf("\nMatrix A min elem: %f\n", min_A);
    printf("Matrix B min elem: %f\n", min_B);

    // Освобождение памяти
    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);

    return 0;
}
