#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

typedef double MatrixItem;

struct Matrix {
    size_t cols;
    size_t rows;
    MatrixItem* data;
} Matrix;

const struct Matrix MATRIX_NULL = {.cols = 0, .rows = 0, .data = NULL };

struct Matrix matrix_init(const size_t cols, const size_t rows) {
    if (cols == 0 || rows == 0) {
        struct Matrix A = { .cols = cols, .rows = rows, .data = NULL };
        return A;
    }

    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols)
        return MATRIX_NULL;

    struct Matrix A = { .cols = cols, .rows = rows, .data = NULL };
    A.data = (MatrixItem*)malloc(A.cols * A.rows * sizeof(MatrixItem));
    if (A.data == NULL) {
        return MATRIX_NULL;
    }
    return A;

}

void matrix_free(struct Matrix* matrix) {
    if (matrix->data == NULL)
        return;

    free(matrix->data);
    matrix->data = NULL;
    matrix->cols = 0;
    matrix->rows = 0;
}


void print_matrix(const struct Matrix A) {
    printf("_____________________________________________ \n");
    for (size_t idx = 0; idx < A.cols * A.rows; ++idx) {
        printf("%4.2f \t", A.data[idx]);
        if ((idx + 1) % A.cols == 0)
            printf("\n");
    }
    printf("\n");
}

// Функция копирования данных из одной матрицы в другую
void matrix_copy(struct Matrix dest, const struct Matrix src) {
    if (dest.data == NULL || src.data == NULL || dest.cols != src.cols || dest.rows != src.rows)
        return;

    size_t size = src.cols * src.rows * sizeof(MatrixItem);
    memcpy(dest.data, src.data, size);
}

// Функция умножения матрицы на скаляр
void matrix_mult_by_coeff(struct Matrix A, const double coefficient)
{
    if (A.data == NULL)
        return;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        A.data[idx] *= coefficient; // Умножаем значение на коэффициент
}

// Функция обнуления матрицы
void matrix_zero(struct Matrix A)
{
    if (A.data == NULL)
        return;

    memset(A.data, 0, A.cols * A.rows * sizeof(MatrixItem));
}

// Функция сложения двух матриц
struct Matrix matrix_sum(const struct Matrix A, const struct Matrix B)
{
    if (A.cols != B.cols || A.rows != B.rows)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return C;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        C.data[idx] = A.data[idx] + B.data[idx];

    return C;
}

// Функция добавления одной матрицы к другой
void matrix_add(const struct Matrix A, const struct Matrix B)
{
    if (A.cols != B.cols || A.rows != B.rows || A.data == NULL || B.data == NULL)
        return;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        A.data[idx] += B.data[idx];
}

// Функция вычитания одной матрицы из другой
struct Matrix matrix_substr(const struct Matrix A, const struct Matrix B)
{
    if (A.cols != B.cols || A.rows != B.rows)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return C;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        C.data[idx] = A.data[idx] - B.data[idx];

    return C;
}

// Функция умножения двух матриц
struct Matrix matrix_mult(const struct Matrix A, const struct Matrix B)
{
    if (A.cols != B.rows)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(B.cols, A.rows);
    if (C.data == NULL)
        return C;

    matrix_zero(C);

    for (size_t rowA = 0; rowA < A.rows; ++rowA)
        for (size_t colB = 0; colB < B.cols; ++colB)
            for (size_t colA = 0; colA < A.cols; ++colA)
                C.data[C.cols * rowA + colB] += A.data[colA + rowA * A.cols] * B.data[B.cols * colA + colB];

    return C;
}

// Функция вычисления экспоненты матрицы
struct Matrix matrix_exponent(const struct Matrix A, const double accuracy)
{
    if (A.cols != A.rows)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return C;
    matrix_zero(C);

    struct Matrix B = matrix_init(A.cols, A.rows);
    matrix_copy(B, A);

    int degree = (int)(ceil(1.0 / accuracy));

    for (int trm = 2; trm <= degree; ++trm) {
        matrix_add(B, A);
        matrix_mult_by_coeff(B, 1.0 / trm);
        matrix_add(C, B);
    }

    matrix_add(C, A);

    for (size_t diag = 0; diag < C.rows; ++diag)
        C.data[diag * C.cols + diag] += 1;

    matrix_free(&B);

    return C;
}

// Функция проверки, равен ли определитель нулю
int matrix_det_if_zero(const struct Matrix A)
{
    size_t count;

    for (size_t row = 0; row < A.rows; ++row) {
        count = 0;
        for (size_t col = 0; col < A.cols; ++col) {
            count += 1;
            if (A.data[row * A.cols + col] != 0.0)
                break;
        }

        if (count == A.cols)
            return 0;
    }

    for (size_t col = 0; col < A.cols; ++col) {
        count = 0;
        for (size_t row = 0; row < A.rows; ++row) {
            count += 1;
            if (A.data[row * A.cols + col] != 0.0)
                break;
        }

        if (count == A.rows)
            return 0;
    }

    return 1;
}

// Функция приведения к верхнетреугольному виду
void matrix_det_prep(const struct Matrix A, size_t diag, double* coeff)
{
    size_t buff_one = diag;

    if (A.data[diag * A.cols + diag] == 0.0) {
        for (size_t row = diag; row < A.rows; ++row) {
            buff_one += 1;
            if (A.data[row * A.cols + diag] != 0.0)
                break;
        }

        double buff_two = 0;

        for (size_t col = diag; col < A.cols; ++col) {
            buff_two = A.data[diag * A.cols + col];
            A.data[diag * A.cols + col] = A.data[buff_one * A.cols + col];
            A.data[buff_one * A.cols + col] = buff_two;
        }

        *coeff *= -1;
    }
}

// Функция вычисления определителя матрицы
double matrix_det(const struct Matrix A)
{
    if (A.cols != A.rows)
        return NAN;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return NAN;
    matrix_copy(C, A);

    if (A.cols == 1) {
        double result = A.data[0];
        matrix_free(&C);
        return result;
    }

    if (matrix_det_if_zero(C) == 0) {
        matrix_free(&C);
        return 0.0;
    }

    double coeff = 1.0;
    double diagonal = 1.0;
    double buff_one, buff_two;

    for (size_t diag = 0; diag < A.rows - 1; ++diag) {
        matrix_det_prep(C, diag, &coeff);
        coeff *= C.data[diag * A.cols + diag];
        buff_one = C.data[diag * A.cols + diag];
        for (size_t col = diag; col < A.cols; ++col)
            C.data[diag * A.cols + col] /= buff_one;

        for (size_t row = diag + 1; row < A.rows; ++row) {
            buff_two = C.data[row * A.cols + diag];
            for (size_t col = diag; col < A.cols; ++col) {
                C.data[row * A.cols + col] -= C.data[diag * A.cols + col] * buff_two;
            }
        }
    }

    for (size_t diag = 0; diag < A.rows; ++diag)
        diagonal *= C.data[diag * A.cols + diag];

    matrix_free(&C);

    double result = coeff * diagonal;

    return result;
}

int main() {
    struct Matrix A, B, F;

    A = matrix_init(3, 3);
    B = matrix_init(3, 3);
    F = matrix_init(2, 2);

    for (int k = 0; k <= A.cols * A.rows - 1; ++k)
        A.data[k] = k + 1;
    print_matrix(A);

    for (int k = 0; k <= B.cols * B.rows - 1; ++k)
        B.data[k] = B.cols * B.rows - k;
    print_matrix(B);

    double a = matrix_det(A);
    printf("det A = %lf \n", a);

    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&F);

    return 0;
}
