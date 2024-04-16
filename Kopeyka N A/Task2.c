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
};

const struct Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL };


// Function returns MATRIX_NULL if fail
struct Matrix matrix_allocate(const size_t rows, const size_t cols)
{
    if (cols == 0 || rows == 0) {
        return MATRIX_NULL;
    }
    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols) return MATRIX_NULL;  // rows * cols < MAX_SIZE / sizeof(MatrixItem)

    struct Matrix A = { .cols = cols, .rows = rows, .data = NULL };
    A.data = (MatrixItem*)malloc(A.cols * A.rows * sizeof(MatrixItem));
    if (A.data == NULL) {
        return MATRIX_NULL;
    }
    return A;
}


struct Matrix matrix_create(const size_t rows, const size_t cols, const MatrixItem* values)
{
    struct Matrix A = matrix_allocate(rows, cols);
    if (A.data == NULL) return A;

    memcpy(A.data, values, rows * cols * sizeof(MatrixItem));
    return A;
}


void matrix_free(struct Matrix* A)
{
    free(A->data);
    *A = MATRIX_NULL;
}


// A += B
int matrix_add(const struct Matrix A, const struct Matrix B)
{
    if (A.rows != B.rows || A.cols != B.cols) return 1;

    for (size_t idx = 0; idx < A.cols * A.rows; idx++) {
        A.data[idx] += B.data[idx];
    }
    return 0;
}


// C = A + B
struct Matrix matrix_sum(const struct Matrix A, const struct Matrix B)
{
    if (A.rows != B.rows || A.cols != B.cols) return MATRIX_NULL;

    struct Matrix C = matrix_allocate(A.rows, A.cols);
    if (C.data == NULL) return MATRIX_NULL;

    memcpy(C.data, A.data, C.cols * C.rows * sizeof(MatrixItem));
    matrix_add(C, B);
    return C;
}


// A -= B
int matrix_subtraction(const struct Matrix A, const struct Matrix B)
{
    if (A.rows != B.rows || A.cols != B.cols) return 1;

    for (size_t idx = 0; idx < A.cols * A.rows; idx++) {
        A.data[idx] -= B.data[idx];
    }
    return 0;
}


// C = A - B 
struct Matrix matrix_diff(const struct Matrix A, const struct Matrix B)
{
    if (A.rows != B.rows || A.cols != B.cols) return MATRIX_NULL;

    struct Matrix C = matrix_allocate(A.rows, A.cols);
    if (C.data == NULL) return C;


    memcpy(C.data, A.data, C.cols * C.rows * sizeof(MatrixItem));
    matrix_subtraction(C, B);

    return C;
}


// coeff * A
struct Matrix matrix_mult_scalar(const struct Matrix A, const double scalar)
{
    struct Matrix C = matrix_allocate(A.rows, A.cols);
    if (C.data == NULL) return MATRIX_NULL;

    for (size_t idx = 0; idx < A.cols * A.rows; idx++) {
        C.data[idx] = scalar * A.data[idx];
    }
    return C;
}

// (A / coeff)
struct Matrix matrix_div_scalar(const struct Matrix A, const double scalar)
{
    struct Matrix C = matrix_allocate(A.rows, A.cols);
    if (C.data == NULL) return MATRIX_NULL;

    for (size_t idx = 0; idx < A.cols * A.rows; idx++) {
        C.data[idx] = A.data[idx] / scalar;
    }
    return C;
}


// C = A * B
struct Matrix matrix_mult(const struct Matrix A, const struct Matrix B)
{
    if (A.cols != B.rows) return MATRIX_NULL;

    struct Matrix C = matrix_allocate(A.rows, B.cols);
    if (C.data == NULL) return MATRIX_NULL;

    for (size_t row = 0; row < A.rows; row++)
        for (size_t col = 0; col < B.cols; col++) {
            C.data[row * B.cols + col] = 0;

            for (size_t idx = 0; idx < A.cols; idx++)
                C.data[row * B.cols + col] += A.data[row * A.cols + idx] * B.data[idx * B.cols + col];
        }
    return C;
}



double det(const struct Matrix A)
{
    if (A.rows != A.cols) return NAN;

    if (A.cols == 1) return A.data[0];

    if (A.cols == 2) return (A.data[0] * A.data[3] - A.data[1] * A.data[2]);

    if (A.cols == 3) {
        return (A.data[0] * A.data[4] * A.data[8]) + \
            (A.data[1] * A.data[5] * A.data[6]) + \
            (A.data[2] * A.data[3] * A.data[7]) - \
            (A.data[2] * A.data[4] * A.data[6]) - \
            (A.data[0] * A.data[5] * A.data[7]) - \
            (A.data[1] * A.data[3] * A.data[8]);
    }
    return NAN;
}


struct Matrix matrix_E(const struct Matrix A)
{
    struct Matrix E = matrix_allocate(A.cols, A.rows);
    if (E.data == NULL) return MATRIX_NULL;

    for (int idx = 0; idx < A.rows * A.cols; idx++) E.data[idx] = 0;
    for (int idx = 0; idx < A.rows; idx++) E.data[idx + idx * A.cols] = 1.;

    return E;
}

 //C = e ^ (A)
struct Matrix matrix_exp(struct Matrix A, unsigned long int level)
{
    struct Matrix sumexp;

    if (A.rows != A.cols) return MATRIX_NULL;

    sumexp = matrix_E(A);
    matrix_add(sumexp, A);

    struct Matrix C = matrix_allocate(A.rows, A.cols);
    if (C.data == NULL) {
        matrix_free(&sumexp);
        return MATRIX_NULL;
    }

    for (unsigned int count = 2; count <= level; count++) {
        matrix_free(&C);
        C = A;

        for (unsigned int idx = 0; idx < count - 1; idx++) {
            struct Matrix t = C;
            C = matrix_mult(A, C);
            matrix_free(&t);

            struct Matrix t = C;
            C = matrix_div_scalar(C, count);
            matrix_free(&t);
        }


        if (C.data == NULL) {
            matrix_free(&sumexp);
            return MATRIX_NULL;
        }
        matrix_add(sumexp, C);
        matrix_free(&C);
    }


    return sumexp;
}


void matrix_print(const struct Matrix A)
{
    for (size_t row = 0; row < A.rows; ++row) {
        printf("[ ");
        for (size_t col = 0; col < A.cols; ++col) {
            printf("%4.2f ", A.data[A.cols * row + col]);
        }
        printf("]\n");
    }
    printf("\n");
}


int main()
{
    struct Matrix A, B, C, C3;

    printf("\nFirst matrix\n");
    A = matrix_create(2, 2, (double[]) { 2., 1., 1., 1. });
    matrix_print(A);

    printf("Second matrix\n");
    B = matrix_create(2, 2, (double[]) { 1., 0., 0., 1. });
    matrix_print(B);


    printf("MAtrix E\n");
    matrix_print(matrix_E(A));

    printf("Sum1 of matrices\n");
    matrix_add(A, B);
    matrix_print(A);

    printf("Sum2 of matrices\n");
    C = matrix_sum(A, B);
    matrix_print(C);


    printf("Sub1 of matrices\n");
    matrix_subtraction(A, B);
    matrix_print(A);

    printf("Sub2 of matrices\n");
    C = matrix_diff(A, B);
    matrix_print(C);

    printf("Multiplying the first matrix by a scalar\n");
    C = matrix_mult_scalar(A, 2);
    matrix_print(C);

    printf("Multiplying the first matrix by a second matrix\n");
    C = matrix_mult(A, B);
    matrix_print(C);

    printf("Determinant of the first matrix\n");
    det(A);
    printf("%4.2f \n", det(A));

    printf("\nExponent of the first matrix\n");
    C3 = matrix_exp(A, 10);
    matrix_print(C3);


    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);
    matrix_free(&C3);

    return 0;
}
