#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

typedef double MatrixItem;

struct Matrix {
    size_t rows;
    size_t cols;
    MatrixItem* data;
};

const struct Matrix MATRIX_NULL = { 0, 0, NULL };

void matrix_error_message() {
    printf("Error!\n");
}

struct Matrix matrix_allocate(const size_t rows, const size_t cols) {
    if (rows == 0 || cols == 0) {
        return MATRIX_NULL;
    }

    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols) {
        return MATRIX_NULL;
    }

    struct Matrix A;
    A.rows = rows;
    A.cols = cols;
    A.data = (MatrixItem*)malloc(A.cols * A.rows * sizeof(MatrixItem));

    if (A.data == NULL) {
        matrix_error_message();
        return MATRIX_NULL;
    }
    return A;
}

struct Matrix matrix_create(const size_t rows, const size_t cols, const MatrixItem* values) {
    struct Matrix A = matrix_allocate(rows, cols);
    if (A.data == NULL) return MATRIX_NULL;

    memcpy(A.data, values, rows * cols * sizeof(MatrixItem));
    return A;
}

struct Matrix matrix_make_ident(size_t rows, size_t cols) {
    struct Matrix I = matrix_allocate(rows, cols);
    if (I.data == NULL) {
        return MATRIX_NULL;
    }
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            I.data[row * cols + col] = (row == col) ? 1.0 : 0.0;
        }
    }
    return I;
}

void matrix_free(struct Matrix* A) {
    if (A->data != NULL) {
        free(A->data);
        A->data = NULL;
    }
}

void matrix_print(const struct Matrix A) {
    if (A.data != NULL) {
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

int matrix_add(struct Matrix A, const struct Matrix B) {
    if (A.cols != B.cols || A.rows != B.rows) return 1;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx) {
        A.data[idx] += B.data[idx];
    }
    return 0;
}

int matrix_subtraction(struct Matrix A, const struct Matrix B) {
    if (A.cols != B.cols || A.rows != B.rows) return 1;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx) {
        A.data[idx] -= B.data[idx];
    }
    return 0;
}

struct Matrix matrix_sum(const struct Matrix A, const struct Matrix B) {
    if (A.cols != B.cols || A.rows != B.rows) return MATRIX_NULL;

    struct Matrix C = matrix_allocate(A.rows, A.cols);
    if (C.data == NULL) return MATRIX_NULL;

    for (size_t idx = 0; idx < C.cols * C.rows; ++idx) {
        C.data[idx] = A.data[idx] + B.data[idx];
    }
    return C;
}

MatrixItem matrix_sumdiag(const struct Matrix A) {
    if (A.cols != A.rows) {
        printf("матрица должна быть квадратной\n");
        return 0;
    }

    MatrixItem summary = 0;

    for (size_t i = 0; i < A.rows; ++i) {
        summary += A.data[i * A.cols + i];
    }

    return summary;
}
void matrix_free(struct Matrix* A) {
    if (A != NULL) {
        free(A->data);
        A->data = NULL;
        A->rows = 0;
        A->cols = 0;
    }
}

struct Matrix matrix_multiply(const struct Matrix A, const struct Matrix B) {
    if (A.cols != B.rows) return MATRIX_NULL;
    struct Matrix C = matrix_allocate(A.rows, B.cols);

    if (C.data == NULL) {
        return MATRIX_NULL;
    }

    for (size_t rowA = 0; rowA < A.rows; ++rowA) {
        for (size_t colB = 0; colB < B.cols; ++colB) {
            C.data[rowA * B.cols + colB] = 0;
            for (size_t idx = 0; idx < A.cols; ++idx) {
                C.data[rowA * B.cols + colB] += A.data[rowA * A.cols + idx] * B.data[idx * B.cols + colB];
            }
        }
    }

    return C;
}

struct Matrix matrix_transpose(const struct Matrix* A) {
    struct Matrix C = matrix_allocate(A->cols, A->rows);
    if (C.data == NULL) return MATRIX_NULL;

    for (size_t rowA = 0; rowA < A->rows; ++rowA) {
        for (size_t colA = 0; colA < A->cols; ++colA) {
            C.data[colA * A->rows + rowA] = A->data[rowA * A->cols + colA];
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
        return A->data[0] * A->data[3] - A->data[1] * A->data[2];
    }

    if (A->cols == 3) {
        return A->data[0] * A->data[4] * A->data[8] + A->data[1] * A->data[5] * A->data[6] + A->data[2] * A->data[3] * A->data[7] -
            (A->data[2] * A->data[4] * A->data[6] + A->data[1] * A->data[3] * A->data[8] + A->data[0] * A->data[5] * A->data[7]);
    }

    matrix_error_message();
    return NAN;
}

struct Matrix sum_for_e(const size_t deg_acc, const struct Matrix A) {
    struct Matrix E = matrix_make_ident(A.rows, A.cols);
    struct Matrix result = matrix_allocate(A.rows, A.cols);

    if (E.data == NULL || result.data == NULL) {
        matrix_free(&E);
        matrix_free(&result);
        return MATRIX_NULL;
    }

    if (deg_acc == 1) {
        matrix_free(&result);
        return E;
    }

    if (deg_acc == 2) {
        memcpy(result.data, A.data, A.rows * A.cols * sizeof(MatrixItem));
        matrix_free(&E);
        return result;
    }

    memcpy(result.data, E.data, E.rows * E.cols * sizeof(MatrixItem));
    matrix_free(&E);

    struct Matrix temp = result;

    for (size_t id = 2; id < deg_acc; ++id) {
        struct Matrix buf = matrix_multiply(temp, A);
        if (buf.data == NULL) {
            matrix_free(&result);
            matrix_free(&temp);
            return MATRIX_NULL;
        }

        for (size_t idx = 0; idx < buf.rows * buf.cols; ++idx) {
            buf.data[idx] /= id;
        }

        if (id > 2) {
            matrix_free(&temp);
        }

        temp = buf;
    }

    matrix_free(&result);
    return temp;
}


struct Matrix matrix_exp(struct Matrix* A, const size_t accuracy) {
    if (A->cols != A->rows) {
        matrix_error_message();
        return MATRIX_NULL;
    }

    struct Matrix E = matrix_allocate(A->rows, A->cols);
    if (E.data == NULL) {
        return MATRIX_NULL;
    }

    for (size_t deg_acc = 1; deg_acc <= accuracy; ++deg_acc) {
        struct Matrix matrix_transfer = sum_for_e(deg_acc, *A);
        struct Matrix buf1 = E;
        E = matrix_sum(buf1, matrix_transfer);
        matrix_free(&buf1);
        matrix_free(&matrix_transfer);
    }

    return E;
}

int main() {
    struct Matrix A, B, C, E, N;
    double deter;


    MatrixItem dataA[9] = { 3., 2., 1., 1., 6., 7., 2., 6., 8. };
    MatrixItem dataB[9] = { 1., 0., 0., 1., 7., 2., 6., 6., 8. };
    MatrixItem dataE[4] = { 3., 2., 1., 1. };

    A = matrix_create(3, 3, dataA);
    B = matrix_create(3, 3, dataB);
    E = matrix_create(2, 2, dataE);

    matrix_print(A);
    matrix_print(B);

    matrix_add(B, A);
    matrix_print(B);

    C = matrix_multiply(A, B);
    matrix_print(C);

    C = matrix_transpose(&C);
    matrix_print(C);

    deter = matrix_det(&C);
    printf("Determinant: %f\n", deter);

    N = matrix_make_ident(3, 3);
    matrix_print(N);

    matrix_print(A);
    struct Matrix expA = matrix_exp(&A, 3);
    matrix_print(expA);

    matrix_free(&E);
    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);
    matrix_free(&N);
    matrix_free(&expA);

    return 0;
}
