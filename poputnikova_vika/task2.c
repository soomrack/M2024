/*Matrix operations:
https://amkbook.net/mathbook/basic-operations-with-matrices-and-their-properties
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

struct Matrix
{
    size_t cols;
    size_t rows;
    double *data;
};

const struct Matrix MATRIX_NULL = {.cols = 0, .rows = 0, .data = NULL};


struct Matrix matrix_init(const size_t rows, const size_t cols)
{
    if (cols == 0 || rows == 0)
        return MATRIX_NULL;

    if (rows >= SIZE_MAX / sizeof(double) / cols)
        return MATRIX_NULL;

    struct Matrix A = {.cols = cols, .rows = rows, .data = NULL};

    A.data = (double *)malloc(A.cols * A.rows * sizeof(double));
    if (A.data == NULL)
    {
        return MATRIX_NULL;
    }

    return A;
}


void matrix_form(struct Matrix *A)
{
    // srand(time(NULL));
    for (size_t idx = 0; idx < A->cols * A->rows; idx++)
    {
        A->data[idx] = ((int)rand() % 10);
    }
}


void matrix_free(struct Matrix *A)
{
    A->cols = 0;
    A->rows = 0;
    free(A->data);
    *A = MATRIX_NULL;
}


// C = A + B
struct Matrix matrix_sum(const struct Matrix A, const struct Matrix B)
{
    if (A.rows != B.rows || A.cols != B.cols)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.rows, A.cols);
    if (C.data == NULL)
        return MATRIX_NULL;

    for (size_t idx = 0; idx < A.cols * A.rows; idx++)
    {
        C.data[idx] = A.data[idx] + B.data[idx];
    }

    return C;
}


// C = A - B
struct Matrix matrix_diff(const struct Matrix A, const struct Matrix B)
{
    if (A.rows != B.rows || A.cols != B.cols)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.rows, A.cols);
    if (C.data == NULL)
        return MATRIX_NULL;

    for (size_t idx = 0; idx < A.cols * A.rows; idx++)
    {
        C.data[idx] = A.data[idx] - B.data[idx];
    }

    return C;
}


// scalar * A
struct Matrix matrix_mult_scalar(const struct Matrix A, const double scalar)
{
    struct Matrix C = matrix_init(A.rows, A.cols);
    if (C.data == NULL)
        return MATRIX_NULL;

    for (size_t idx = 0; idx < A.cols * A.rows; idx++)
    {
        C.data[idx] = scalar * A.data[idx];
    }
    return C;
}


// C = A * B
struct Matrix matrix_mult(const struct Matrix A, const struct Matrix B)
{
    if (A.rows != B.rows)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.rows, B.cols);
    if (C.data == NULL)
        return MATRIX_NULL;

    for (size_t rowA = 0; rowA < A.rows; rowA++)
        for (size_t colB = 0; colB < B.cols; colB++)
        {
            C.data[rowA * B.cols + colB] = 0;

            for (size_t idx = 0; idx < A.cols; idx++)
                C.data[rowA * B.cols + colB] += A.data[rowA * A.cols + idx] * B.data[idx * B.cols + colB];
        }
    return C;
}


struct Matrix matrix_transp(const struct Matrix A)
{
    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return C;

    for (size_t row = 0; row < A.rows; ++row)
        for (size_t col = 0; col < A.cols; ++col)
            C.data[row * A.cols + col] = A.data[row + col * A.cols];

    return C;
}


double matrix_determinant(const struct Matrix A)
{
    if (A.rows != A.cols)
        return NAN;

    if (A.cols == 1)
        return A.data[0];
    else
    {
        double det = 0;
        for (size_t idx = 0; idx < A.cols; idx++)
        {
            struct Matrix minor = matrix_init(A.rows - 1, A.cols - 1);

            if (minor.data == NULL)
                return A.data[0];

            size_t minor_row = 0, minor_col = 0;
            for (size_t jdx = 1; jdx < A.rows; jdx++)
            {
                for (size_t k = 0; k < A.cols; k++)
                {
                    if (k == idx)
                        continue;
                    minor.data[minor_row * minor.cols + minor_col] = A.data[jdx * A.cols + k];
                    minor_col++;
                }
                minor_row++;
                minor_col = 0;
            }
            // Рекурсивно вычисляем детерминант минора и суммируем с учетом знака
            det += (idx % 2 == 0 ? 1 : -1) * A.data[0 * A.cols + idx] * matrix_determinant(minor);
            matrix_free(&minor);
        }
        return det;
    }
}


// C = e ^ (A) https://portal.tpu.ru/SHARED/k/KONVAL/Sites/Russian_sites/1/22.html
// https://www.wolframalpha.com/input?i2d=true&i=Power%5Be%2C%7B%7B1%2C7%7D%2C%7B4%2C0%7D%7D%5D
struct Matrix matrix_exp(struct Matrix A)
{
    if (A.rows != A.cols)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return C;

    for (size_t idx = 0; idx < A.cols * A.rows; idx++)
    {
        C.data[idx] += exp(A.data[idx]);
    }
    return C;
}


void matrix_print(const struct Matrix A)
{
    for (size_t row = 0; row < A.rows; ++row)
    {
        printf("[ ");
        for (size_t col = 0; col < A.cols; ++col)
        {
            printf("%4.2f ", A.data[A.cols * row + col]);
        }
        printf("]\n");
    }
    printf("\n");
}


int main()
{
    struct Matrix A, B, C, D;

    printf("\nFirst matrix\n");
    A = matrix_init(2, 2);
    matrix_form(&A);
    matrix_print(A);

    printf("Second matrix\n");
    B = matrix_init(2, 2);
    matrix_form(&B);
    matrix_print(B);

    printf("Sum of matrices\n");
    C = matrix_sum(A, B);
    matrix_print(C);

    printf("Sub of matrices\n");
    C = matrix_diff(A, B);
    matrix_print(C);

    printf("Multiplying the first matrix by a scalar\n");
    C = matrix_mult_scalar(A, 2);
    matrix_print(C);

    printf("Multiplying the first matrix by a second matrix\n");
    C = matrix_mult(A, B);
    matrix_print(C);

    printf("Transposed first matrix\n");
    C = matrix_transp(A);
    matrix_print(C);

    printf("Determinant of the first matrix\n");
    matrix_determinant(A);
    printf("%4.2f \n", matrix_determinant(A));

    printf("\nExponent of the first matrix\n");
    D = matrix_exp(A);
    matrix_print(D);

    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);
    matrix_free(&D);

    return 0;
}
