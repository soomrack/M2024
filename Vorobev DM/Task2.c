#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>


typedef double MatrixObject;

struct Matrices {
    size_t columns;
    size_t rows;
    MatrixObject* data;
};


const struct Matrices MATRIX_NULL = { .columns = 0, .rows = 0, .data = NULL };


void Matrix_warning()
{
    printf("Something is wrong, please check!\n");
}


struct Matrices matrix_initialization(const size_t rows, const size_t columns)
{
    if (rows == 0 || columns == 0) {
        return MATRIX_NULL;
    }

    if (rows >= SIZE_MAX / sizeof(MatrixObject) / columns) {
        return MATRIX_NULL;
    }

    struct Matrices A = { .columns = columns, .rows = rows, .data = NULL };
    A.data = (MatrixObject*)malloc(A.columns * A.rows * sizeof(MatrixObject));

    if (A.data == NULL) {
        Matrix_warning();
        return MATRIX_NULL;
    }
    return A;
}


struct Matrices matrix_create_random(const size_t rows, const size_t columns)
{
    struct Matrices A = matrix_initialization(rows, columns);
    for (size_t idx = 0; idx < A.columns * A.rows; idx++)
        A.data[idx] = (MatrixObject)(rand() % 10);

    if (A.data == NULL) {
        Matrix_warning();
        return MATRIX_NULL;
    }
    return A;
}


void matrix_free(struct Matrices* A)
{
    A->columns = 0;
    A->rows = 0;
    free(A->data);
    A->data = NULL;
}


void matrix_print(const struct Matrices A)
{
    if (A.data != NULL || A.columns != 0 || A.rows != 0) {
        for (size_t row = 0; row < A.rows; ++row) {
            printf("[ ");
            for (size_t col = 0; col < A.columns; ++col) {
                printf("%4.2f ", A.data[A.columns * row + col]);
            }
            printf("]\n");
        }
        printf("\n");
    }
    else {
        Matrix_warning();
    }
}


struct Matrices matrix_sum(const struct Matrices A, const struct Matrices B)
{
    if (A.columns != B.columns || A.rows != B.rows) {
        return MATRIX_NULL;
    }

    struct Matrices C = matrix_initialization(A.columns, A.rows);
    for (size_t idx = 0; idx < C.columns * C.rows; ++idx) {
        C.data[idx] = A.data[idx] + B.data[idx];
    }
    return C;
}


struct Matrices matrix_sub(const struct Matrices A, const struct Matrices B)
{
    if (A.columns != B.columns || A.rows != B.rows) {
        return MATRIX_NULL;
    }

    struct Matrices C = matrix_initialization(A.columns, A.rows);
    for (size_t idx = 0; idx < C.columns * C.rows; ++idx) {
        C.data[idx] = A.data[idx] - B.data[idx];
    }
    return C;
}


struct Matrices matrix_mult(const struct Matrices A, const struct Matrices B)
{
    if (A.columns != B.rows) {
        return MATRIX_NULL;
    }
    struct Matrices C = matrix_initialization(A.columns, B.rows);

    if (C.data == NULL) {
        return MATRIX_NULL;
    }

    for (size_t rowA = 0; rowA < A.rows; ++rowA) {
        for (size_t columnsB = 0; columnsB < B.columns; ++columnsB) {
            C.data[rowA * (A.columns) + columnsB] = 0;
            for (size_t idx = 0; idx < A.columns; ++idx) {
                MatrixObject W = A.data[(rowA * A.columns) + idx];
                MatrixObject V = B.data[(idx * B.columns) + columnsB];
                C.data[(rowA * A.columns) + columnsB] += W * V;
            }
        }
    }

    return C;
}


struct Matrices matrix_transp(struct Matrices* A)
{
    struct Matrices C = matrix_initialization(A->rows, A->columns);

    if (C.data == NULL) {
        return MATRIX_NULL;
    }

    for (size_t rowA = 0; rowA < A->rows; ++rowA) {
        for (size_t columnsA = 0; columnsA < A->columns; ++columnsA) {
            C.data[(A->rows) * columnsA + rowA] = A->data[columnsA + rowA * A->columns];
        }
    }
    return C;
}


double matrix_det(struct Matrices* A) {
    if (A->columns != A->rows) {
        Matrix_warning();
        return NAN;
    }

    if (A->columns == 1) {
        return A->data[0];
    }

    if (A->columns == 2) {
        double matr_det = (A->data[0]) * (A->data[3]) - (A->data[1]) * (A->data[2]);
        return matr_det;
    }

    if (A->columns == 3) {
        double matr_det = (A->data[0]) * (A->data[4]) * (A->data[8]);
        matr_det += (A->data[1]) * (A->data[5]) * (A->data[6]);
        matr_det += (A->data[3]) * (A->data[7]) * (A->data[2]);
        matr_det -= (A->data[1]) * (A->data[3]) * (A->data[8]);
        matr_det -= (A->data[0]) * (A->data[5]) * (A->data[7]);
        matr_det -= (A->data[2]) * (A->data[4]) * (A->data[6]);
        return matr_det;
    }
    Matrix_warning();
    return NAN;
}


struct Matrices matrix_make_ident(size_t rows, size_t columns)
{
    struct Matrices I = matrix_initialization(rows, columns);
    if (I.data == NULL) {
        return MATRIX_NULL;
    }
    for (size_t idx = 0; idx < rows * columns; idx++) {
        if (idx % (rows + 1) == 0) {
            I.data[idx] = 1.;
        }
        else {
            I.data[idx] = 0;
        }
    }
    return I;
}


struct Matrices pow_matrix(struct Matrices A, const size_t p) {
    struct Matrices M = matrix_initialization(A.rows, A.columns);
    if (M.data == NULL) {
        return MATRIX_NULL;
    }
    memcpy(M.data, A.data, A.rows * A.columns * sizeof(MatrixObject));
    if (p == 0) {
        matrix_free(&M);
        return matrix_make_ident(A.columns, A.rows);
    }
    if (p == 1) {
        return M;
    }
    for (size_t i = 1; i < p; ++i) {
        struct Matrices ptr = M;
        M = matrix_mult(ptr, A);
        matrix_free(&ptr); // free ptr explicitly
    }
    return M;
}


struct Matrices el_sum_for_e(const struct Matrices A, const size_t k)
{
    struct Matrices P = pow_matrix(A, k);
    if (P.data == NULL) {
        return MATRIX_NULL;
    }
    for (size_t i = 1; i <= k; ++i) {
        for (size_t id = 0; id < P.rows * P.columns; ++id) {
            P.data[id] /= (double)i;
        }
    }

    return P;
}


struct Matrices exponent(const struct Matrices A, const size_t deg_acc)
{
    struct Matrices G = matrix_initialization(A.rows, A.columns);

    for (size_t id = 0; id < G.rows * G.columns; ++id) {
        G.data[id] = 0;
    }

    for (size_t i = 0; i <= deg_acc; ++i) {
        struct Matrices ptr = G;
        struct Matrices temp = el_sum_for_e(A, i);
        G = matrix_sum(ptr, temp);
        matrix_free(&ptr);
        matrix_free(&temp);
    }

    return G;
}


int main(void)
{
    struct Matrices A, B, C, N, E, R, D;

    double deter;

    A = matrix_create_random(3, 3);
    B = matrix_create_random(3, 3);

    matrix_print(A);
    matrix_print(B);

    R = matrix_sum(A, B);
    matrix_print(R);
    E = matrix_sub(B, A);
    matrix_print(E);

    C = matrix_mult(A, B);
    matrix_print(C);

    C = matrix_transp(&C);
    matrix_print(C);

    deter = matrix_det(&C);
    printf("%f\n\n", deter);

    N = matrix_make_ident(3, 3);
    matrix_print(N);

    D = exponent(A, 3);
    matrix_print(D);

    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);
    matrix_free(&N);
    matrix_free(&E);
    matrix_free(&R);
    matrix_free(&D);

    return 0;
}
