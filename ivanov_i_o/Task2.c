#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>


typedef double MatrixItem;

struct Matrix {
    size_t cols;
    size_t rows;
    MatrixItem* data;
};


const struct Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL }; 


void matrix_error_message()
{
    printf("Check your actions!\n");
}


struct Matrix matrix_initiation(const size_t rows, const size_t cols)
{
    if (rows == 0 || cols == 0) {
        return MATRIX_NULL;
    }

    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols) {
        return MATRIX_NULL;
    }

    struct Matrix A = { .cols = cols, .rows = rows, .data = NULL };
    A.data = (MatrixItem*)malloc(A.cols * A.rows * sizeof(MatrixItem));

    if (A.data == NULL) {
        matrix_error_message();
        return MATRIX_NULL;
    }
    return A;
}


struct Matrix matrix_create_random(const size_t rows, const size_t cols) 
{
    struct Matrix A = matrix_initiation(rows, cols);
    for (size_t idx = 0; idx < A.cols * A.rows; idx++)
            A.data[idx] = (MatrixItem)(rand() % 10);
    return A;
}


void matrix_free(struct Matrix* A)
{
   A->cols = 0;
   A->rows = 0;
   free(A->data);
   A->data = NULL;
}


void matrix_print(const struct Matrix A)
{
    if (A.data != NULL || A.cols != 0 || A.rows != 0) {
        for (size_t row = 0; row < A.rows; ++row) {
            printf("[ ");
            for (size_t col = 0; col < A.cols; ++col) {
                printf("%4.2f ", A.data[A.cols * row + col]);
            }
            printf("]\n");
        }
        printf("\n");
    }
    else {
        matrix_error_message();
    }
}


struct Matrix matrix_sum(const struct Matrix A, const struct Matrix B) 
{
    if (A.cols != B.cols || A.rows != B.rows) {
        return MATRIX_NULL;
    }

    struct Matrix C = matrix_initiation(A.cols, A.rows);
    for (size_t idx = 0; idx < C.cols * C.rows; ++idx) {
        C.data[idx] = A.data[idx] + B.data[idx];
    }
    return C;
}


struct Matrix matrix_sub(const struct Matrix A, const struct Matrix B) 
{
    if (A.cols != B.cols || A.rows != B.rows) {
      return MATRIX_NULL;  
    } 

    struct Matrix C = matrix_initiation(A.cols, A.rows);
    for (size_t idx = 0; idx < C.cols * C.rows; ++idx) {
        C.data[idx] = A.data[idx] - B.data[idx];
    }
    return C;
}


struct Matrix matrix_mult(const struct Matrix A, const struct Matrix B) 
{
    if (A.cols != B.rows) {
        return MATRIX_NULL;
    }
    struct Matrix C = matrix_initiation(A.cols, B.rows);

    if (C.data == NULL) {
        return MATRIX_NULL;
    }

    for (size_t rowA = 0; rowA < A.rows; ++rowA) {
        for (size_t colsB = 0; colsB < B.cols; ++colsB) {
            C.data[rowA * (A.cols) + colsB] = 0;
            for (size_t idx = 0; idx < A.cols; ++idx) {
                MatrixItem W = A.data[(rowA * A.cols) + idx]; 
                MatrixItem V = B.data[(idx * B.cols) + colsB];
                C.data[(rowA * A.cols) + colsB] += W * V;    
            }
        }
    }

    return C;
}


struct Matrix matrix_transp(struct Matrix* A)
{
    struct Matrix C = matrix_initiation(A->rows, A->cols);

    if (C.data == NULL) {
        return MATRIX_NULL;
    }

    for (size_t rowA = 0; rowA < A->rows; ++rowA) {
        for (size_t colsA = 0; colsA < A->cols; ++colsA) {
            C.data[(A->rows) * colsA + rowA] = A->data[colsA + rowA * A->cols];
        }
    }
    return C;
}


double matrix_det(struct Matrix* A) {
    if (A->cols != A->rows) {
        matrix_error_message();
        return NAN;
    }

    if (A->cols == 1) {
        return A->data[0];
    }

    if (A->cols == 2) {
        double matr_det = (A->data[0]) * (A->data[3]) - (A->data[1]) * (A->data[2]);
        return matr_det;
    }

    if (A->cols == 3) {
        double matr_det = (A->data[0]) * (A->data[4]) * (A->data[8]);
        matr_det += (A->data[1]) * (A->data[5]) * (A->data[6]);
        matr_det += (A->data[3]) * (A->data[7]) * (A->data[2]);
        matr_det -= (A->data[1]) * (A->data[3]) * (A->data[8]);
        matr_det -= (A->data[0]) * (A->data[5]) * (A->data[7]);
        matr_det -= (A->data[2]) * (A->data[4]) * (A->data[6]);
        return matr_det;
    }
    matrix_error_message();
    return NAN;
}


struct Matrix matrix_make_ident(size_t rows, size_t cols)
{
    struct Matrix I = matrix_initiation(rows, cols);
    if (I.data == NULL) {
        return MATRIX_NULL;
    }
    for (size_t idx = 0; idx < rows * cols; idx++) {
        if (idx % (rows + 1) == 0) {
            I.data[idx] = 1.;
        }
        else {
            I.data[idx] = 0;
        }
    }
    return I;
}


struct Matrix pow_matrix(struct Matrix A, const size_t p) {
    struct Matrix M = matrix_initiation(A.rows, A.cols);

    memcpy(M.data, A.data, A.rows * A.cols * sizeof(MatrixItem));

    if (M.data == NULL) {
        return MATRIX_NULL;
    }
    if (p == 0) {
        return matrix_make_ident(A.cols, A.rows); 
    }
    if (p == 1) {
        return M;
    }
    for (size_t i = 1; i < p; ++i) {
        struct Matrix ptr = M;
        M = matrix_mult(ptr, A); 
        matrix_free(&ptr);
    }
    return M;
}


struct Matrix el_sum_for_e(const struct Matrix A, const size_t k)  
{
    struct Matrix P = pow_matrix(A, k);

    for (size_t i = 1; i <= k; ++i) {
        for (size_t id = 0; id < P.rows * P.cols; ++id) {
            P.data[id] /= (double)i;
        }
    }

    return P;
}


struct Matrix exponent(const struct Matrix A, const size_t deg_acc)  
{
    struct Matrix G = matrix_initiation(A.rows, A.cols);

    for (size_t id = 0; id < G.rows * G.cols; ++id) {
        G.data[id] = 0;
    }

    for (size_t i = 0; i <= deg_acc; ++i) {
        struct Matrix ptr = G;
        struct Matrix temp = el_sum_for_e(A, i);
        G = matrix_sum(ptr, temp); 
        matrix_free(&ptr);
        matrix_free(&temp);
    }

    return G;
}


int main(void)
{
    struct Matrix A, B, C, N, E, R, D;

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