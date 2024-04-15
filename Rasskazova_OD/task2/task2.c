#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef double Matrix;

struct Matrix {
    size_t cols; 
    size_t rows; 
    double *data;
};

const struct Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL};

enum LogLevel {  
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

void log_message(enum LogLevel type, char* msg) {  
    switch (type) {  
        case DEBUG:
        printf("DEBUG: %s\n", msg);
        break;
    case INFO:
        printf("INFO: %s\n", msg);
        break;
    case WARNING:
        printf("WARNING: %s\n", msg);
        break;
    case ERROR:
        printf("ERROR: %s\n", msg);
        break;
    default:
        printf("DEFAULT: %s\n", msg);
        break;
    }
}

struct Matrix matrix_init(const size_t cols, const size_t rows)
{
    struct Matrix matrix;
    matrix.cols = cols;
    matrix.rows = rows;

    if(SIZE_MAX / cols / rows < sizeof(double)) {
        log_message(ERROR, "Матрица слишком большая");
        matrix.cols = 0;
        matrix.rows = 0;
        matrix.data = NULL;
        return matrix;
    }

    matrix.data = (double*)malloc(cols * rows * sizeof(double));

    if(matrix.data == NULL) {
        log_message(ERROR, "Ошибка выделения памяти");
        matrix.cols = 0;
        matrix.rows = 0;
    }
    return matrix;
}

struct Matrix matrix_create(const size_t rows, const size_t cols, const Matrix* values)
{
    struct Matrix A = matrix_init(rows, cols);
    if (A.data == NULL) {
        log_message(ERROR, "Ошибка выделения памяти");
        return MATRIX_NULL;
    }
    memcpy(A.data, values, rows * cols * sizeof(Matrix));
    return A;
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
    }else {
        log_message(ERROR, "Ошибка выделения памяти");
    }
}

void matrix_free(struct Matrix matrix)
{
    matrix.cols = 0;
    matrix.rows = 0;
    free(matrix.data);
    matrix.data = NULL;
}

int main()
{
    struct Matrix A, B, C;

    A = matrix_create(3, 3, (double[]) { 1., 2., 3., 4., 5., 6., 7. });
    B = matrix_create(3, 3, (double[]) { 10., 9., 8., 7., 6., 5., 4., 3., 2. });
    C = matrix_create(0, 0, (double[]) {});
    
    matrix_print(A);
    matrix_print(B);
    matrix_print(C);

    matrix_free(A);
    matrix_free(B);
    matrix_free(C);

    return 0;
}

