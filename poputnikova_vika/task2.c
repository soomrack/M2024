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
            for (size_t row = 1; row < A.rows; row++)
            {
                for (size_t col = 0; col < A.cols; col++)
                {
                    if (col == idx)
                        continue;
                    minor.data[minor_row * minor.cols + minor_col] = A.data[row * A.cols + col];
                    minor_col++;
                }
                minor_row++;
                minor_col = 0;
            }
            // Рекурсивно вычисляем детерминант минора и суммируем с учетом знака
            det += (idx % 2 == 0 ? 1 : -1) * A.data[idx] * matrix_determinant(minor);
            matrix_free(&minor);
        }
        return det;
    }
}


struct Matrix matrix_identity(size_t rows, size_t cols)
{
    struct Matrix identity = matrix_init(rows, cols);
    if (identity.data == NULL)
    {
        return MATRIX_NULL;
    }
    for (size_t idx = 0; idx < rows * cols; idx++)
    {
        if (idx % (rows + 1) == 0)
        {
            identity.data[idx] = 1.;
        }
        else
        {
            identity.data[idx] = 0;
        }
    }
    return identity;
}


struct Matrix matrix_power(struct Matrix A, const size_t pow)
{
    struct Matrix C = matrix_init(A.rows, A.cols);

    memcpy(C.data, A.data, A.rows * A.cols * sizeof(double));

    if (C.data == NULL)
    {
        return MATRIX_NULL;
    }
    if (pow == 0)
    {
        return matrix_identity(A.rows, A.cols);
    }
    if (pow == 1)
    {
        return C;
    }
    for (size_t idx = 1; idx < pow; ++idx)
    {
        struct Matrix temp = C;
        C = matrix_mult(temp, A);
        matrix_free(&temp);
    }
    return C;
}


int factorial(int index)
{
    int F = 1;

    for (int idx = 1; idx <= index; ++idx)
    {
        F *= idx;
    }
    return F;
}


// C = e ^ (A) https://portal.tpu.ru/SHARED/k/KONVAL/Sites/Russian_sites/1/22.htm
struct Matrix matrix_exp(struct Matrix A, size_t N)
{
    if (A.rows != A.cols)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    struct Matrix C_power = matrix_init(A.cols, A.rows);

    for (size_t idx = 0; idx < N; ++idx)
    {
        C_power = matrix_power(A, idx);
        double factor = 1.0 / factorial(idx);

        for (size_t row = 0; row < A.rows; ++row)
        {
            for (size_t col = 0; col < A.cols; ++col)
            {
                double value = C_power.data[row * C_power.cols + col] * factor;
                C.data[row * A.cols + col] = C.data[row * A.cols + col] + value;
            }
        }
        matrix_free(&C_power);
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
    struct Matrix A, B, C, D, G;

    srand(time(NULL));

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

    // printf("\nPower of the first matrix\n");
    // G = matrix_power(A, 3);
    // matrix_print(G);

    printf("\nExponent of the first matrix\n");
    D = matrix_exp(A, 3);
    matrix_print(D);

    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);
    // matrix_free(&G);
    matrix_free(&D);

    return 0;
}
