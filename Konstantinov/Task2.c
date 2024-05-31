#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


typedef double MatrixItem;

struct Matrix{
    size_t cols;
    size_t rows;
    MatrixItem* data;
};

const struct Matrix MATRIX_NULL = { 0, 0, NULL };


void error_message(const char str[]){
    printf("%s\n", str);
}


struct Matrix matrix_init(const size_t rows, const size_t cols){
    struct Matrix Serf = { .cols = cols, .rows = rows, .data = NULL };

    Serf.data = (MatrixItem*)malloc(Serf.cols * Serf.rows * sizeof(double));

    if (Serf.data == NULL) {
        return MATRIX_NULL;
        error_message("Matrix memory allocation error.");
    }

    if (rows == 0 || cols == 0) {
        return MATRIX_NULL;
        error_message("Zero number of rows or columns.");
    }

    if (cols * rows >= SIZE_MAX / sizeof(MatrixItem)){
        return MATRIX_NULL;
        error_message("Matrix is too large.");
    }

    return Serf;
}


struct Matrix matrix_make_ident(size_t rows, size_t cols){
    struct Matrix I = matrix_init(rows, cols);

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


struct Matrix matrix_create(const size_t rows, const size_t cols, const double* values){
    struct Matrix A = matrix_init(rows, cols);

    memcpy(A.data, values, rows * cols * sizeof(double));

    if (A.data == NULL) return MATRIX_NULL;

    return A;
}


void matrix_free(struct Matrix* A){
    if (A->data != NULL || A->cols != 0 || A->rows != 0) {
        A->cols = 0;
        A->rows = 0;
        free(A->data);
        A->data = NULL;
    }
}


void matrix_print(const struct Matrix A){
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
        error_message("Matrix structure error.");
    }
}


int matrix_add(const struct Matrix A, const struct Matrix B){        // A = А + B
    if (A.cols != B.cols || A.rows != B.rows) return 1;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx) {
        A.data[idx] += B.data[idx];
    }

    return 0;
}


struct Matrix matrix_sum(const struct Matrix A, const struct Matrix B){
    struct Matrix C = matrix_init(A.cols, A.rows);

    if (A.cols != B.cols || A.rows != B.rows) return MATRIX_NULL;

    for (size_t idx = 0; idx < C.cols * C.rows; ++idx) {
        C.data[idx] = A.data[idx] + B.data[idx];
    }

    return C;
}


struct Matrix matrix_sub(const struct Matrix A, const struct Matrix B){
    struct Matrix C = matrix_init(A.cols, A.rows);
    
    if (A.cols != B.cols || A.rows != B.rows) return MATRIX_NULL;

    for (size_t idx = 0; idx < C.cols * C.rows; ++idx) {
        C.data[idx] = A.data[idx] - B.data[idx];
    }

    return C;
}


struct Matrix matrix_mult(const struct Matrix A, const struct Matrix B){
    if (A.cols != B.rows) return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, B.rows);

    if (C.data == NULL) return MATRIX_NULL;

    for (size_t rowA = 0; rowA < A.rows; ++rowA) {
        for (size_t colsB = 0; colsB < B.cols; ++colsB) {
            C.data[rowA * (A.cols) + colsB] = 0;
            for (size_t idx = 0; idx < A.cols; ++idx) {
                C.data[rowA * (A.cols) + colsB] += (A.data[(rowA * A.cols) + idx]) * (B.data[(idx * B.cols) + colsB]);
            }
        }
    }

    return C;
}


struct Matrix matrix_transp(struct Matrix* A){
    struct Matrix C = matrix_init(A->rows, A->cols);

    if (C.data == NULL) {
        matrix_free(&C);
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
        error_message("Determinant only makes sense for square matrices.");
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
        double matr_det = (A->data[0]) * (A->data[4]) * (A->data[8]) + (A->data[1]) * (A->data[5]) * (A->data[6]) + (A->data[3]) * (A->data[7]) * (A->data[2]);
        matr_det -= ((A->data[2]) * (A->data[4]) * (A->data[6]) + (A->data[1]) * (A->data[3]) * (A->data[8]) + (A->data[0]) * (A->data[5]) * (A->data[7]));
        return matr_det;
    }

    return NAN;
}

strict Matrix matrix_copy(


struct Matrix sum_for_e(const size_t deg_acc, const struct Matrix A){
    struct Matrix E = matrix_init(A.rows, A.cols);

    if (E.data == NULL) {
        return MATRIX_NULL;
    }

    if (deg_acc == 1) {
        struct Matrix E = matrix_make_ident(A.cols, A.rows);
        return E;
    }
    
    E = matrix_copy(A);

    if (deg_acc == 2) {
        return E;
    }

    if (deg_acc > 2) {
        for (size_t id = 2; id < deg_acc; ++id) {
            struct Matrix buf = E;
            E = matrix_mult(buf, A);
            for (size_t idx = 0; idx < E.rows * E.cols; ++idx) {
                E.data[idx] /= (id);
            }
            matrix_free(&buf);
        }
    }
    return E;
}


struct Matrix matrix_exp(struct Matrix* A, const size_t accuracy){
    if (A->cols != A->rows) {
        error_message("Exponential function only make sense for square matrices.");
        return MATRIX_NULLNULL;
    }

    struct Matrix E = matrix_init(A->rows, A->cols);

    if (E.data == NULL) {
        matrix_free (&E);
        return MATRIX_NULL;
    }

    struct Matrix matrix_transfer;

    for (size_t deg_acc = 1; deg_acc <= accuracy; ++deg_acc) {
        matrix_transfer = sum_for_e(deg_acc, *A);
        struct Matrix buf1 = E;
        E = matrix_sum(buf1, matrix_transfer);
        matrix_free(&buf1);
        matrix_free(&matrix_transfer);
    }

    return E;
}


int main(){
    struct Matrix A, B, C, E, N;

    double deter;

    A = matrix_create(3, 3, (double[]) { 3., 2., 1., 1., 6., 7., 2., 6., 8. });
    B = matrix_create(3, 3, (double[]) { 1., 0., 0., 1., 7., 2., 6., 6., 8. });
    E = matrix_create(2, 2, (double[]) { 3., 2., 1., 1. });

    matrix_print(A);
    matrix_print(B);
    matrix_add(B, A);
    matrix_print(B);

    C = matrix_mult(A, B);
    matrix_print(C);

    C = matrix_transp(&C);
    matrix_print(C);

    deter = matrix_det(&C);
    printf("%f \n", deter);

    N = matrix_make_ident(3, 3);
    matrix_print(N);

    matrix_print(A);
    A = matrix_exp(&A, 3);
    matrix_print(A);


    matrix_free(&E);
    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);
    matrix_free(&N);

    return 0;
}
