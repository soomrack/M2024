// Liubinetskii V. (@liubvlad)
// 
//
// ☑ Добавить базовые операции с матрицей
//
// ☑ Расчёт определителя
// (+-) ☑Экспонента матрицы (https://ru.wikipedia.org/wiki/%D0%AD%D0%BA%D1%81%D0%BF%D0%BE%D0%BD%D0%B5%D0%BD%D1%82%D0%B0_%D0%BC%D0%B0%D1%82%D1%80%D0%B8%D1%86%D1%8B)
//
// ☑snake_case
// ☑size_t check
// ☑free on *
// ☑для цикла переменные idx, row, col.. etc
// ☑consts params
// ☑названия переменых массивов передаваемых в функцию A и B (M - для одной)
// ☑exit(1) - нельзя (NULL matrix)
// 
// 
// ☑Логгер

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logger.h"

typedef double Matrix_data_type;

typedef struct Matrix {
    size_t rows;
    size_t cols;
    Matrix_data_type *data;
} Matrix;


const struct Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL };
const int EXPONENT_ITERATION_COUNT = 10;


Matrix* create_matrix(const size_t rows, const size_t cols) {

    if (rows == 0 || cols == 0) {
        return &MATRIX_NULL;
    };

    // Verify on overflow
    if (rows >= SIZE_MAX / sizeof(Matrix_data_type) / cols) {
        log(ERROR, "Required memory amount went beyond the data type's capacity.");
        return &MATRIX_NULL;
    }

    // Allocate memory for the matrix
    Matrix* M = (Matrix*)malloc(sizeof(Matrix));
    if (M == NULL) {
        log(ERROR, "Memory allocation for matrix failed.");
        return &MATRIX_NULL;
    }

    // Allocate memory for the matrix data
    M->data = (Matrix_data_type*)malloc(rows * cols * sizeof(Matrix_data_type));
    if (M->data == NULL) {
        log(ERROR, "Memory allocation for data failed.");
        return &MATRIX_NULL;
    }

    M->rows = rows;
    M->cols = cols;

    return M;
}


Matrix* create_square_matrix(const size_t size) {
    return create_matrix(size, size);
}


void free_matrix(Matrix *M) {
    // ??
    M->rows = 0;
    M->cols = 0;
    free(M->data);
    free(M);
}


// Function to sum two matrices
Matrix* sum_matrices(const Matrix *A, const Matrix *B) {
    if (A->rows == 0 || B->rows == 0 || A->cols == 0 || B->cols == 0) {
        log(ERROR, "One of dimensions of sum matrices has value 0.");
        return &MATRIX_NULL;
    };

    if (A->rows != B->rows || A->cols != B->cols) {
        log(WARNING, "Matrices have different dimensions.");
        return &MATRIX_NULL;
    }

    Matrix *result = create_matrix(A->rows, A->cols);
    for (size_t idx = 0; idx < A->rows * A->cols; idx++) {
        result->data[idx] = A->data[idx] + B->data[idx];
    }

    return result;
}


// Function to subtract two matrices
Matrix* subtract_matrices(const Matrix *A, const Matrix *B) {
    if (A->rows == 0 || B->rows == 0 || A->cols == 0 || B->cols == 0) {
        log(ERROR, "One of dimensions of subtract matrices has value 0.");
        return &MATRIX_NULL;
    };

    if (A->rows != B->rows || A->cols != B->cols) {
        log(WARNING, "Matrices have different dimensions.");
        return &MATRIX_NULL;
    }

    Matrix *result = create_matrix(A->rows, A->cols);
    for (size_t idx = 0; idx < A->rows * A->cols; idx++) {
        result->data[idx] = A->data[idx] - B->data[idx];
    }

    return result;
}


// Function to multiply two matrices
Matrix* multiply_matrices(const Matrix *A, const Matrix *B) {
    if (A->rows == 0 || B->rows == 0 || A->cols == 0 || B->cols == 0) {
        log(ERROR, "One of dimensions of multiply matrices has value 0.");
        return &MATRIX_NULL;
    };

    if (A->cols != B->rows) {
        log(WARNING, "Matrices cannot be multiplied due to incompatible dimensions.");
        return &MATRIX_NULL;
    }

    Matrix *result = create_matrix(A->rows, B->cols);
    for (size_t row = 0; row < A->rows; row++) {
        for (size_t col = 0; col < B->cols; col++) {
            Matrix_data_type sum = 0;
            for (size_t offset = 0; offset < A->cols; offset++) {
                sum += A->data[row * A->cols + offset] * B->data[offset * B->cols + col];
            }
            result->data[row * B->cols + col] = sum;
        }
    }
    return result;
}


// Function to multiply a matrix by a scalar
Matrix* multiply_matrix_by_scalar(const Matrix *M, double scalar) {
    if (M->rows == 0 || M->cols == 0) {
        log(ERROR, "One of dimensions of multiply matrix on scalar has value 0.");
        return &MATRIX_NULL;
    };

    Matrix *result = create_matrix(M->rows, M->cols);
    for (size_t idx = 0; idx < M->rows * M->cols; idx++) {
        result->data[idx] = M->data[idx] * scalar;
    }
    return result;
}


Matrix* exponential_matrix(const Matrix *M) {
    if (M->rows == 0 || M->cols == 0) {
        log(ERROR, "Matrix should be size more then zero!");
        return M;
    }

    if (M->rows != M->cols) {
        log(WARNING, "Exponential can only be calculated for sqare matrices.");
        return M;
    }

    size_t size = M->rows;
    size_t factorial = 1;

    Matrix *result = create_square_matrix(size);
    Matrix *power = create_square_matrix(size);

    // Инициализируем как единичные матрицы
    for (size_t row = 0; row < size; row++) {
        for (size_t col = 0; col < size; col++) {
            result->data[row * size + col] = (row == col) ? 1 : 0;
            power->data[row * size + col] = (row == col) ? 1 : 0;
        }
    }

    // Расчёт матричного экспоненциального ряда
    for (int iteration = 1; iteration <= EXPONENT_ITERATION_COUNT; iteration++) {

        factorial *= iteration;

        power = multiply_matrices(power, M);
        // ?? what about memory?
        Matrix* temp = multiply_matrix_by_scalar(power, 1.0 / factorial);
        result = sum_matrices(result, temp);

        free_matrix(temp);
    }

    free_matrix(power);

    return result;
}


// Function to calculate the determinant of matrix by minors
Matrix_data_type determinant_of_matrix_by_minors(const Matrix *M) {
    if (M->rows == 0 || M->cols == 0) {
        log(ERROR, "Matrix should be size more then zero!");
        return 0;
    }

    if (M->rows != M->cols) {
        log(WARNING, "Determinant can only be calculated for sqare matrices.");
        return 0;
    }

    if (M->rows > 7) {
        log(WARNING, "Can't be calculated for a matrix of dimensions greater than 7.");
        return 0;
    }

    size_t size = M->rows;
    if (size == 1) {
        return M->data[0];
    }

    Matrix_data_type det = 0;
    Matrix *submat = create_square_matrix(size - 1);

    for (size_t idx = 0; idx < size; idx++) {
        size_t sub_row = 0;
        for (size_t row = 1; row < size; row++) {
            size_t sub_col = 0;
            for (size_t col = 0; col < size; col++) {
                if (col == idx) {
                    continue;
                }

                submat->data[sub_row * submat->cols + sub_col] = M->data[row * M->cols + col];
                sub_col++;
            }

            sub_row++;
        }

        short sign = (idx % 2 == 0) ? 1 : -1;
        Matrix_data_type subdet = determinant_of_matrix_by_minors(submat);
        det += sign * M->data[idx] * subdet;
    }

    free_matrix(submat);

    return det;
}


void print_matrix(const Matrix *M) {
    log(INFO, "Matrix:");

    if (M->rows == 0 || M->cols == 0) {
        log(ERROR, "Matrix is null.\n");
        return;
    }

    for (size_t row = 0; row < M->rows; row++) {
        for (size_t col = 0; col < M->cols; col++) {
            printf("%.2f\t", M->data[row * M->cols + col]);
        }

        printf("\n\n");
    }
}


// Function to fill a matrix (in progress)
void fill_matrix(Matrix *M) {
    for (size_t idx = 0; idx < M->rows * M->cols; idx++) {
        M->data[idx] = idx * 2 + 1;
    }
}


// Function to fill a matrix with random numbers
void fill_matrix_random(Matrix *M) {
    srand(time(NULL));
    for (size_t idx = 0; idx < M->rows * M->cols; idx++) {
        M->data[idx] = (Matrix_data_type)rand() / RAND_MAX;
    }
}

// *************************

void case_sum_and_print() {
    size_t rows = 2;
    size_t cols = 3;

    Matrix *mat1 = create_matrix(rows, cols);
    Matrix *mat2 = create_matrix(rows, cols);

    fill_matrix(mat1);
    fill_matrix_random(mat2);

    print_matrix(mat1);
    print_matrix(mat2);

    Matrix *result = sum_matrices(mat1, mat2);
    print_matrix(result);

    free_matrix(mat1);
    free_matrix(mat2);
    free_matrix(result);
}


void case_prepair_matrix_and_get_determinant() {
    size_t size = 3;

    Matrix *matrix = create_square_matrix(size);
    matrix->data[0] = 1; matrix->data[1] = 2; matrix->data[2] = 3;
    matrix->data[3] = 4; matrix->data[4] = 5; matrix->data[5] = 6;
    matrix->data[6] = 7; matrix->data[7] = 8; matrix->data[8] = 9;

    print_matrix(matrix);

    Matrix_data_type det = determinant_of_matrix_by_minors(matrix);
    printf("Determinant of the matrix: %.2f\n", det);

    free_matrix(matrix);
}


void case_get_matrix_determinant() {
    size_t size = 3;

    Matrix *matrix = create_square_matrix(size);
    fill_matrix_random(matrix);
    print_matrix(matrix);

    Matrix_data_type det = determinant_of_matrix_by_minors(matrix);
    printf("Determinant of the matrix: %.2f\n", det);

    free_matrix(matrix);
}


void case_exp() {
    int size = 2; 
    Matrix *matrix = create_square_matrix(size);

    matrix->data[0] = 1; matrix->data[1] = 2;
    matrix->data[2] = 3; matrix->data[3] = 4;

    Matrix *expMat = exponential_matrix(matrix);

    log("Exponential of the matrix:");
    print_matrix(expMat);

    free_matrix(matrix);
    free_matrix(expMat);
}


// TODO OBSOLETE
void case_test_exp() {

    size_t size = 3;

    Matrix *A = create_square_matrix(size);
    fill_matrix(A);
    print_matrix(A);

    Matrix* Exp1 = exponential_matrix(A);
    print_matrix(Exp1);
}


int main() {
    
    case_test_exp();

    return 0;
}


/*
* Output:

    Matrix:
    1.00    3.00    5.00
    7.00    9.00    11.00

    Matrix:
    0.86    0.25    0.49
    0.62    0.94    0.67

    Matrix: (sum)
    1.86    3.25    5.49
    7.62    9.94    11.67

****
* Determinant
    Matrix:
    0.76    0.91    0.09

    0.89    0.25    0.96

    0.83    0.67    0.93

    Determinant of the matrix: -0.31

Det(Matrix) = 0.76·0.25·0.93 + 0.91·0.96·0.83 + 0.09·0.89·0.67 - 0.09·0.25·0.83 - 0.76·0.96·0.67 - 0.91·0.89·0.93 =
            = 0.1767 + 0.725088 + 0.053667 - 0.018675 - 0.488832 - 0.753207 = -0.305259

*/

/*
exp:

1.00    3.00    5.00
7.00    9.00    11.00
13.00   15.00   17.00

_____________________________________________
2.72    5.15    8.59
12.03   16.46   18.90
22.34   25.77   30.21

Matrix:
1.00    3.00    5.00

7.00    9.00    11.00

13.00   15.00   17.00

Matrix:
19033382.71     23904323.93     28775266.14

48967356.61     61498873.95     74030389.28

78901331.50     99093421.96     119285513.43
*/
