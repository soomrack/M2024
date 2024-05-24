#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>


typedef double MatrixData;


struct Matrix {
    size_t cols;
    size_t rows;
    MatrixData *data;
};


const struct Matrix MATRIX_NULL = {.cols = 0, .rows = 0, .data = NULL}; 


enum ErrorType { MEM_ALLOC_ERROR, FALSE_MATRIX, FALSE_ROWS_COLS, EMPTY_MATRIX };


void matrix_error(enum ErrorType error)
{
    switch (error)
    {
        case MEM_ALLOC_ERROR:
            printf("memory wasn't allocated\n");
            break;
        case FALSE_MATRIX:
            printf("matrix input is incorrect or difficult to calculate\n");
            break;
        case FALSE_ROWS_COLS:
            printf("the number of rows or columns in the matrices is incorrect\n");
            break;
        case EMPTY_MATRIX:
            printf("Matrix are empty\n");
            break;
        default:
            printf("undeclared error\n");
            break;
    }
}


struct Matrix matrix_allocation(const size_t cols, const size_t rows)
{
    if (cols == 0, rows == 0) {
    struct Matrix A = {.cols = cols, .rows = rows, .data = NULL};
    return A;
    }

    if (rows >= SIZE_MAX / sizeof(MatrixData) / cols) {
        matrix_error(MEM_ALLOC_ERROR);
        return MATRIX_NULL; 
    }

    struct Matrix A = {.cols = cols, .rows = rows, .data = NULL};
    
    A.data = malloc(A.cols * A.rows * sizeof(MatrixData));
    
    if (A.data == NULL) {
        matrix_error(MEM_ALLOC_ERROR);
        matrix_free(&A);
        return MATRIX_NULL;
    }
    
    return A;
}

struct Matrix matrix_create(const size_t cols, const size_t rows)
{
    struct Matrix A = matrix_allocation(cols, rows);
    return A;
}


struct Matrix zero_matrix(const size_t cols, const size_t rows)
{
    struct Matrix A = matrix_create(cols, rows);
    for (size_t idx = 0; idx < A.cols * A.rows; ++idx) {
        A.data[idx] = 0;
    }

    return A;
}


struct Matrix matrix_fill(const size_t cols, size_t rows, const MatrixData *data)
{
    struct Matrix A = matrix_create(cols, rows);
    
    if (A.data == NULL) {
        matrix_error(EMPTY_MATRIX);
        matrix_free(&A);
        return MATRIX_NULL;
    }

    memcpy(A.data, data, rows * cols * sizeof(MatrixData));

    return A;
}


void matrix_free(struct Matrix *A)
{
    free(A->data);
    *A = MATRIX_NULL;
}


// A = A + B
struct Matrix matrix_add(struct Matrix A, struct Matrix B) 
{
    if (A.cols != B.cols ||  A.rows != B.rows) {
        matrix_error(FALSE_ROWS_COLS);
        return MATRIX_NULL;
    }

    for (size_t pos = 0; pos != A.rows * A.cols; ++pos) {
        A.data[pos]+=B.data[pos];
    }

}


// C = A + B
struct Matrix matrix_sum(struct Matrix A, struct Matrix B) 
{
    struct Matrix C;

    if (A.cols != B.cols ||  A.rows != B.rows) { 
        matrix_error(FALSE_ROWS_COLS);
        return MATRIX_NULL; 
    }
    C = matrix_create(A.cols, A.rows);

    if (C.data == NULL) { 
        matrix_error(EMPTY_MATRIX);
        matrix_free(&C);
        return MATRIX_NULL;
    }

    for (size_t pos = 0; pos < A.cols * A.rows; ++pos) {
        C.data[pos] = A.data[pos] + B.data[pos]; 
    }
    
    return C;
}


// A = A - B
struct Matrix matrix_substraction(struct Matrix A, struct Matrix B) 
{
    if (A.cols != B.cols ||  A.rows != B.rows) {   
        matrix_error(FALSE_ROWS_COLS);
        return MATRIX_NULL;
    }

    for (size_t pos = 0; pos < A.cols * B.cols; ++pos) {
        A.data[pos]-=B.data[pos];
    }
}


// C = A - B
struct Matrix matrix_difference(struct Matrix A, struct Matrix B) 
{
    struct Matrix C;

    C = matrix_create(A.cols, A.rows);

    if (C.data == NULL) { 
        matrix_error(EMPTY_MATRIX);
        matrix_free(&C);
        return MATRIX_NULL;
    }


if (A.cols != B.cols ||  A.rows != B.rows) {
        matrix_error(FALSE_ROWS_COLS);
        return MATRIX_NULL;
    }

    for (int pos = 0; pos < A.cols * A.rows; ++pos) {
        C.data[pos] = A.data[pos] - B.data[pos];
    }

    return C;   
}


// A = A * scalar
struct Matrix matrix_scalar_multiplication(const struct Matrix A, const double scalar)
{
  for (size_t idx = 0; idx < A.cols * A.rows; idx++) {
    A.data[idx] *= scalar;
  }
  return A;    
}


// C = A * B
struct Matrix matrix_multiplication(const struct Matrix A, const struct Matrix B) 
{
    struct Matrix C;

    C = matrix_create(B.cols, B.rows);

    if (C.data == NULL) { 
        matrix_error(EMPTY_MATRIX);
        matrix_free(&C);
        return MATRIX_NULL;
    }
    
    if (A.rows != B.cols) {
        matrix_error(FALSE_ROWS_COLS);
        return MATRIX_NULL;
    }

    if (C.cols != B.cols ||  C.rows != B.rows) {
        matrix_error(FALSE_ROWS_COLS);
        return MATRIX_NULL;
    }
  
    for (size_t row = 0; row < C.rows; row++) {
        for (size_t col = 0; col < C.cols; col++) {
            C.data[row * C.cols + col] = 0;
            for (size_t idx = 0; idx < A.cols; idx++) {
                C.data[row * C.cols + col] += A.data[row * A.cols + idx] * B.data[idx * B.cols + col];
                };
        }
    }
    return C;
}


double matrix_determinant(const struct Matrix A)
{
  if (A.cols != A.rows) {
        matrix_error(FALSE_ROWS_COLS);
        return 1;
    }

    if (A.cols == 1) return A.data[0];

  if (A.cols == 2) return (A.data[0] * A.data[3] - A.data[1] * A.data[2]);

  if (A.cols == 3) {
    return ((A.data[0] * A.data[4] * A.data[8]) 
        + (A.data[1] * A.data[5] * A.data[6]) 
        + (A.data[3] * A.data[7] * A.data[2])
        - (A.data[2] * A.data[4] * A.data[6])
        - (A.data[1] * A.data[3] * A.data[8])
        - (A.data[0] * A.data[5] * A.data[7]));
  }

    return NAN;

}


struct Matrix matrix_transposition(const struct Matrix A)
{
    struct Matrix C;

    C = matrix_create(A.cols, A.rows);

    if (C.cols != A.rows ||  C.rows != A.cols) { 
        matrix_error(FALSE_MATRIX);
        matrix_free(&C);
        return MATRIX_NULL;
    }

  for (size_t rows = 0; rows < A.rows; rows++)
      for (size_t cols = 0; cols < A.cols; cols++)
      C.data[rows * A.rows + cols] = A.data[cols * A.cols + rows];
  return C;
}


struct Matrix matrix_E(const struct Matrix A)
{
    struct Matrix E;
    E = matrix_create(A.cols, A.rows);
    if (E.data == NULL) { 
        matrix_error(EMPTY_MATRIX);
        matrix_free(&E);
        return MATRIX_NULL;
    }

    for (size_t idx = 0; idx < A.rows * A.cols; idx++) E.data[idx] = 0;
    for (size_t idx = 0; idx < A.rows; idx++) E.data[idx + idx * A.cols] = 1.;

    return E;
}


struct Matrix matrix_exp(const struct Matrix A, const int order)
{
    struct Matrix A_exp;
    struct Matrix assist;
    struct Matrix buf;
    struct Matrix buf2;

    assist = matrix_E(A);
    A_exp = matrix_E(A);
    buf = matrix_E(A);

    if (A_exp.data == NULL ||  assist.data == NULL || buf.data == NULL) {
        matrix_error(EMPTY_MATRIX);
        matrix_free(&A_exp);
        matrix_free(&assist);
        matrix_free(&buf);
        return MATRIX_NULL;
    } 
    
    double n = 1.;

    for (int counter = 1; counter <= order; ++counter) {
        n *= (double)counter;

        buf = matrix_multiplication(A, assist);
        buf2 = matrix_multiplication(A, assist);
            
        if (buf.data == NULL) {
            matrix_error(EMPTY_MATRIX); 
            matrix_free(&buf);
            matrix_free(&A_exp);
            matrix_free(&assist);
            return MATRIX_NULL;
        }

        assist = buf;

        matrix_scalar_multiplication(assist, 1 / n);
        matrix_add(A_exp, assist);
        matrix_free(&buf);

     
    }

    matrix_free(&assist);
    matrix_free(&buf);

    return(A_exp);
}   


void matrix_print(const struct Matrix A)
{
    for (size_t row = 0; row < A.rows; ++row) {
        printf("["); 
        for (size_t col = 0; col < A.cols; ++col) {
            printf("%f ", A.data[A.cols * row + col]);
        }
        printf("]\n");
    }
}

int main()
{
    struct Matrix A, B, C, EXP;

    // printf("\nFirst matrix\n");
    A = matrix_fill(2, 2, (double[]) { 2., 4., 2., 4. });
    // matrix_print(A);

    // printf("Second matrix\n");
    // B = matrix_fill(2, 2, (double[]) { 1., 0., 0., 1. });
    // matrix_print(B);

    // printf("Sum1 of matrices\n");
    // matrix_add(A, B);
    // matrix_print(A);

    // printf("Sum2 of matrices\n");
    // C = matrix_sum(A, B);
    // matrix_print(C);

    // // printf("Sub1 of matrices\n");
    // // matrix_substraction(A, B);
    // // matrix_print(A);

    // printf("Sub2 of matrices\n");
    // C = matrix_difference(A, B);
    // matrix_print(C);

    // printf("Multiplying the first matrix by a scalar\n");
    // C = matrix_scalar_multiplication(A, 2);
    // matrix_print(C);

    // printf("Multiplying the first matrix by a second matrix\n");
    // C = matrix_multiplication(A, B);
    // matrix_print(C);

    // printf("Transposed first matrix\n");
    // C = matrix_transposition(A);
    // matrix_print(C);

    // printf("Determinant of the first matrix\n");
    // matrix_determinant(A);
    // printf("%4.2f \n", matrix_determinant(A));
    
    //printf("\nExponent of the first matrix\n");
    
    EXP = matrix_exp(A, 2);
    matrix_print(A);

    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);
    matrix_free(&EXP);

    return 0;
}
