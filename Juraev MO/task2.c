#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>


typedef double MatrixItem;

struct Matrix 
{
    size_t cols;  // столбцы
    size_t rows;  // строки
    MatrixItem* data;
};

const struct Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL };


enum MatrixErrorsType { 
    NO_MEMORY,  // нехватка памяти
    INCOMPATIBLE_SIZES  // несовместимые размеры
};

void matrix_error_message(enum MatrixErrorsType error_type)  // вывод сообщений об ошибках 
{ 
    switch (error_type) {
    case NO_MEMORY:
        printf("No memory available!\n");
        break;
    case INCOMPATIBLE_SIZES:
        printf("Incompatible sizes of matrices!\n");
        break;
    default:
        printf("Unknown error!\n");
        break;
    }
}


struct Matrix matrix_init(const size_t rows, const size_t cols)  // инициализация матрицы
{
    if (rows == 0 || cols == 0) {
        return MATRIX_NULL; 
    }

    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols) {  
        matrix_error_message(NO_MEMORY);
        return MATRIX_NULL;  
    }

    struct Matrix A = { .cols = cols, .rows = rows, .data = NULL };

    A.data = (MatrixItem*)malloc(A.cols * A.rows * sizeof(MatrixItem));

    if (A.data == NULL) {
        matrix_error_message(NO_MEMORY);
        return MATRIX_NULL;
    }

    return A;
}


struct Matrix matrix_make_ones(size_t rows, size_t cols)  // создание единичной матрицы
{
    struct Matrix O = matrix_init(rows, cols);

    if (O.data == NULL) {
        matrix_error_message(NO_MEMORY);
        return MATRIX_NULL;
    }

    memset(O.data, 0, sizeof(O.data));

    for (size_t idx = 0; idx < rows * cols; idx++) {
        if (idx % (rows + 1) == 0) {
            O.data[idx] = 1.;
        }
    }

    return O;
}


struct Matrix matrix_create(const size_t rows, const size_t cols, const MatrixItem* values) // создание матрицы с заданными параметрами
{
    if (rows == 0 && cols == 0) 
    {
        return MATRIX_NULL;
    }
    
    struct Matrix A = matrix_init(rows, cols);

    if (A.data == NULL) {
        matrix_error_message(NO_MEMORY);
        return MATRIX_NULL;
    }

    memcpy(A.data, values, rows * cols * sizeof(MatrixItem));

    return A;
}


void matrix_print(const struct Matrix A)  // вывод матрицы 
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
        matrix_error_message(NO_MEMORY);
    }
}


void matrix_free(struct Matrix *A)  // освобождение памяти
{
    free(A->data);
    *A = MATRIX_NULL;
}


struct Matrix matrix_sum(const struct Matrix A, const struct Matrix B)  // сложение
{
    if (A.cols != B.cols || A.rows != B.rows) {
        matrix_error_message(INCOMPATIBLE_SIZES);
        return MATRIX_NULL;
    }
 
    struct Matrix C = matrix_init(A.cols, A.rows);

    for (size_t idx = 0; idx < C.cols * C.rows; ++idx) {
        C.data[idx] = A.data[idx] + B.data[idx];
    }

    return C;
}


struct Matrix matrix_sub(const struct Matrix A, const struct Matrix B)  // вычитание
{
    if (A.cols != B.cols || A.rows != B.rows) {
        matrix_error_message(INCOMPATIBLE_SIZES);
        return MATRIX_NULL;
    }

    struct Matrix C = matrix_init(A.cols, A.rows);

    for (size_t idx = 0; idx < C.cols * C.rows; ++idx) {
        C.data[idx] = A.data[idx] - B.data[idx];
    }

    return C;
}


struct Matrix matrix_mult(const struct Matrix A, const struct Matrix B)  // умножение
{
    if (A.cols != B.rows) {
        matrix_error_message(NO_MEMORY);
        return MATRIX_NULL;
    }

     struct Matrix C = matrix_init(A.cols, B.rows);

    if (C.data == NULL) {
        matrix_error_message(NO_MEMORY);
        return MATRIX_NULL;
    }

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


struct Matrix matrix_mult_scalar(const double scalar, struct Matrix B)  // умножение на скаляр
{
    for (size_t idx = 0; idx < B.cols * B.rows; ++idx) {
        B.data[idx] *= scalar;
    }

    return B;
}


double matrix_det(struct Matrix* A)  // определитель матрицы
{
    if (A->cols != A->rows) {
        matrix_error_message(NO_MEMORY);
        return NAN;
    }

    if (A->cols == 1) {
        return A->data[0];
    }

    if (A->cols == 2) {
        MatrixItem matr_det = (A->data[0]) * (A->data[3]) - (A->data[1]) * (A->data[2]);
        return matr_det;
    }

    if (A->cols == 3) {
        MatrixItem matr_det = (A->data[0]) * (A->data[4]) * (A->data[8]) + (A->data[1]) * (A->data[5]) * (A->data[6]) + (A->data[3]) * (A->data[7]) * (A->data[2]);
        matr_det -= ((A->data[2]) * (A->data[4]) * (A->data[6]) + (A->data[1]) * (A->data[3]) * (A->data[8]) + (A->data[0]) * (A->data[5]) * (A->data[7]));
        return matr_det;
    } 
    
    return NAN;
}


struct Matrix sum_for_e(const size_t deg_acc, const struct Matrix A)  // сумма ряда Эйлера
{
    struct Matrix E = matrix_init(A.rows, A.cols);

    if (E.data == NULL) {
        matrix_error_message(NO_MEMORY);
        return MATRIX_NULL;
    }

    if (deg_acc == 1) {
        struct Matrix E = matrix_make_ones(A.cols, A.rows);
        return E;
    }

    if (deg_acc == 2) {
        return A;
    }

    if (deg_acc > 2) {
        E = A;
        for (size_t step = 2; step < deg_acc; ++step) {
            struct Matrix buf = E;
            E = matrix_mult(buf, A);
            matrix_mult_scalar(1 / step, E);
            matrix_free(&buf);
        }
    }
    return E;
}


struct Matrix matrix_exp(struct Matrix* A, const size_t accuracy)  // экспонента
{
    if (A->cols != A->rows) {
        matrix_error_message(NO_MEMORY);
        return MATRIX_NULL;
    }

    struct Matrix E = matrix_init(A->rows, A->cols);

    if (E.data == NULL) {
        matrix_error_message(NO_MEMORY);
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


int main()
{
    struct Matrix A, B, C, E, N;

    A = matrix_create(3, 3, (double[]) { 1., 2., 3., 4., 5., 6., 7., 8., 9. });
    B = matrix_create(3, 3, (double[]) { 10., 11., 12., 13., 14., 15., 16., 18., 19. });
    E = matrix_create(2, 2, (double[]) { 3., 2., 1., 1. });

    matrix_print(A);
    matrix_print(B);

    C = matrix_mult(A, B);
    matrix_print(C);

    N = matrix_make_ones(3, 3);
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
