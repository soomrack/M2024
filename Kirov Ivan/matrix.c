#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

struct Matrix {
    unsigned int rows;
    unsigned int cols;
    double *data;
};
 

const struct Matrix NULL_MATRIX = { .rows = 0, .cols = 0, .data = NULL };


struct Matrix matrix_creation(const unsigned int rows, const unsigned int cols)
{
    if (cols == 0 || rows == 0) {
        struct Matrix A = { .rows = rows, .cols = cols, .data = NULL };
        return A;
    };

    if (rows >= SIZE_MAX / sizeof(double) / cols)
        return NULL_MATRIX;

    struct Matrix A = { .rows = rows, .cols = cols, .data = NULL };
    A.data = (double*)malloc(A.cols * A.rows * sizeof(double));
    if (A.data == NULL) {
        return NULL_MATRIX;
    }
    return A;
}

void delete_matrix(struct Matrix* A)
{
    A->rows = 0;
    A->cols = 0;
    free(A->data);
    A->data = NULL;
}


void matrix_error()
{
    printf("Error: different number of columns or rows\n");
}


void zero_matrix(struct Matrix A)
{
    if (A.data == NULL) return;
    memset(A.data, 0, sizeof(double) * A.rows * A.cols);
}


void matrix_set_one(struct Matrix A)
{
    zero_matrix(A);
    for (unsigned int idx = 0; idx < A.rows * A.cols; idx += A.cols + 1)
        A.data[idx] = 1;
}



// A += B
int matrix_add(struct Matrix A, struct Matrix B)
{
    if (A.cols != B.cols || A.rows != B.rows) return 1;

    for (unsigned int idx = 0; idx < A.cols * A.rows; ++idx)
        A.data[idx] += B.data[idx];
    return 0;
}

// С = A + B
void matrix_sum(const struct Matrix A, const struct Matrix B, const struct Matrix C)
{
    if (A.cols != B.cols || A.rows != B.rows) {
        matrix_error();
        return;
    }
    for (unsigned int idx = 0; idx < A.cols * A.rows; ++idx)
    {
        C.data[idx] = A.data[idx] + B.data[idx];
    }
}



// C = A - B
void matrix_sub(const struct Matrix A, const struct Matrix B, const struct Matrix C)
{
    if (A.cols != B.cols || A.rows != B.rows) {
        matrix_error();
        return;
    }
    for (unsigned int idx = 0; idx < A.cols * A.rows; ++idx)
    {
        C.data[idx] = A.data[idx] - B.data[idx];
    }
}


// diff = A - B
struct Matrix matrix_diff(struct Matrix A, struct Matrix B)
{
    if (A.cols != B.cols || A.rows != B.rows) return NULL_MATRIX;

    struct Matrix diff = matrix_creation(A.cols, A.rows);
    if (diff.data == NULL) return NULL_MATRIX;

    memcpy(diff.data, A.data, sizeof(double) * A.cols * A.rows);
    matrix_add(diff, B);

    return diff;
}

// scalar * A
struct Matrix matrix_mult_scalar(const struct Matrix A, const double scalar)
{
    struct Matrix S = matrix_creation(A.rows, A.cols);
    if (S.data == NULL) return NULL_MATRIX;

    for (size_t idx = 0; idx < A.cols * A.rows; idx++) {
        S.data[idx] = scalar * A.data[idx];
    }
    return S;
}

// mult = A * B
struct Matrix matrix_mult(struct Matrix A, struct Matrix B)
{
    if (A.cols != B.rows) return NULL_MATRIX;

    struct Matrix mult = matrix_creation(A.rows, B.cols);
    if (mult.data == NULL) return NULL_MATRIX;

    for (unsigned int rowA = 0; rowA < A.rows; ++rowA) {
        for (unsigned int colB = 0; colB < B.cols; ++colB) {
            double sum = 0;
            for (unsigned int idx = 0; idx < A.cols; ++idx) {
                sum += A.data[rowA * A.cols + idx] * B.data[idx * B.cols + colB];
            }
            mult.data[rowA * mult.cols + colB] = sum;
        }
    }
    return mult;
}
// det

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

// A /= k
void matrix_div_k(struct Matrix A, const double k)
{
    for (unsigned int idx = 0; idx < A.cols * A.rows; ++idx) {
        A.data[idx] /= k;
    }
    return;
}

// trans = A^T

struct Matrix matrix_trans(struct Matrix A)
{
    struct Matrix trans = matrix_creation(A.rows, A.cols);
    if (trans.data == NULL) return trans;

    for (unsigned int row = 0; row < trans.rows; row++) {
        for (unsigned int col = 0; col < trans.cols; col++) {
            trans.data[col * trans.cols + row] = A.data[row * A.cols + col];
        }
    }
    return trans;
}

// exp = e^A
struct Matrix matrix_exp(struct Matrix A)
{
    if (A.cols != A.rows) return NULL_MATRIX;
    if (A.cols == 0) return NULL_MATRIX;

    struct Matrix exp = matrix_creation(A.rows, A.cols);
    if (exp.data == NULL) return NULL_MATRIX;
    matrix_set_one(exp);

    struct Matrix term_prev = matrix_creation(A.rows, A.cols);
    if (term_prev.data == 0) {
        delete_matrix(&exp);
        return NULL_MATRIX;
    };
    matrix_set_one(term_prev);

    struct Matrix term_next;

    for (int idx = 1; idx < 100; ++idx) {

        term_next = matrix_mult(term_prev, A);
        if (term_next.data == NULL) {
            delete_matrix(&term_prev);
            delete_matrix(&exp);
            return NULL_MATRIX;
        }
        matrix_div_k(term_next, idx);
        memcpy(term_prev.data, term_next.data, sizeof(double) * term_prev.cols * term_prev.rows);
        delete_matrix(&term_next);
        matrix_add(exp, term_prev);

    }
    delete_matrix(&term_prev);
    return exp;
}


void matrix_fill(struct Matrix A, const double* values)
{
    memcpy(A.data, values, sizeof(double) * A.cols * A.rows);
    return;
}


void matrix_print(const struct Matrix A, const char* text)
{
    printf("%s\n", text);
    for (unsigned int col = 0; col < A.cols; ++col)
    {
        printf("[");

        for (unsigned int row = 0; row < A.rows; ++row)
        {
            printf("%4.2f ", A.data[A.cols * row + col]);
        }
        printf("]\n");
    }
    printf("\n");
}


int main()
{
    double values_A[] =
    { 2., 1., 2.,
      1., 3., 0.,
      3., 0., 3.
    };

    struct Matrix A = matrix_creation(3, 3);
    matrix_set_one(A);
    struct Matrix B = matrix_creation(3, 3);
    matrix_set_one(B);
    struct Matrix C = matrix_creation(3, 3);
    struct Matrix D = matrix_creation(3, 3);
    struct Matrix S = matrix_creation(3, 3);

    matrix_fill(A, values_A);
    matrix_fill(B, values_A);

    matrix_sum(A, B, C);

    matrix_sub(A, B, D);
    
    matrix_mult_scalar(S, 3);

    matrix_mult(A, B);

    matrix_exp(A);
    det(A);

    struct Matrix MULT = matrix_mult(A, B);
    struct Matrix EXP = matrix_trans(matrix_exp(A));
    struct Matrix T = matrix_trans(A);
    
    

    matrix_print(A, "Matrix A");
    matrix_print(B, "Matrix B");
    matrix_print(C, "SUM: A + B");
    matrix_print(D, "SUB: A - B");
    matrix_print(S, "Mult: scalar * A");
    printf ("Determinant A = %4.2f  \n", det(A));
    printf ("\n");
    matrix_print(MULT, "MULT: A * B");
    matrix_print(T, "TRANS: A^T");
    matrix_print(EXP, "EXP: A ^ e");
   
    delete_matrix(&A);
    delete_matrix(&B);
    delete_matrix(&C);
    delete_matrix(&D);
    delete_matrix(&S);
    delete_matrix(&MULT);
    delete_matrix(&T);
    delete_matrix(&EXP);

    return(0);
}
