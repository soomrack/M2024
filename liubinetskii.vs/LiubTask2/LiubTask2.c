// Liubinetskii V. (@liubvlad)
// 
//
// ☑ Добавить базовые операции с матрицей
//
// ☑ Расчёт определителя
// □  Экспонента матрицы (https://ru.wikipedia.org/wiki/%D0%AD%D0%BA%D1%81%D0%BF%D0%BE%D0%BD%D0%B5%D0%BD%D1%82%D0%B0_%D0%BC%D0%B0%D1%82%D1%80%D0%B8%D1%86%D1%8B)
//
// ☑snake_case
// size_t check
// ☑free on *
// для цикла переменные idx, row, col.. etc
// ☑consts params
// ☑названия переменых массивов передаваемых в функцию A и B (M - для одной)
// ☑exit(1) - нельзя (NULL matrix)
// 
// 
// □  Логгер

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
        printf("Error: Required memory amount went beyond the data type's capacity.\n");
        return &MATRIX_NULL;
    }

    // Allocate memory for the matrix
    Matrix* M = (Matrix*)malloc(sizeof(Matrix));
    if (M == NULL) {
        printf("Error: Memory allocation for matrix failed.\n");
        return &MATRIX_NULL;
    }

    // Allocate memory for the matrix data
    M->data = (Matrix_data_type*)malloc(rows * cols * sizeof(Matrix_data_type));
    if (M->data == NULL) {
        printf("Error: Memory allocation for data failed.\n");
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
        printf("Error: One of dimensions of sum matrices has value 0.\n");
        return &MATRIX_NULL;
    };

    if (A->rows != B->rows || A->cols != B->cols) {
        printf("Error: Matrices have different dimensions.\n");
        return &MATRIX_NULL;
    }

    Matrix *result = create_matrix(A->rows, A->cols);
    for (size_t i = 0; i < A->rows * A->cols; i++) {
        result->data[i] = A->data[i] + B->data[i];
    }

    return result;
}


// Function to subtract two matrices
Matrix* subtract_matrices(const Matrix *A, const Matrix *B) {
    if (A->rows == 0 || B->rows == 0 || A->cols == 0 || B->cols == 0) {
        printf("Error: One of dimensions of subtract matrices has value 0.\n");
        return &MATRIX_NULL;
    };

    if (A->rows != B->rows || A->cols != B->cols) {
        printf("Error: Matrices have different dimensions.\n");
        return &MATRIX_NULL;
    }

    Matrix *result = create_matrix(A->rows, A->cols);
    for (size_t i = 0; i < A->rows * A->cols; i++) {
        result->data[i] = A->data[i] - B->data[i];
    }

    return result;
}


// Function to multiply two matrices
Matrix* multiply_matrices(const Matrix *A, const Matrix *B) {
    if (A->rows == 0 || B->rows == 0 || A->cols == 0 || B->cols == 0) {
        printf("Error: One of dimensions of multiply matrices has value 0.\n");
        return &MATRIX_NULL;
    };

    if (A->cols != B->rows) {
        printf("Error: Matrices cannot be multiplied due to incompatible dimensions.\n");
        return &MATRIX_NULL;
    }

    Matrix *result = create_matrix(A->rows, B->cols);
    for (size_t i = 0; i < A->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            Matrix_data_type sum = 0;
            for (size_t k = 0; k < A->cols; k++) {
                sum += A->data[i * A->cols + k] * B->data[k * B->cols + j];
            }
            result->data[i * B->cols + j] = sum;
        }
    }
    return result;
}


// Function to multiply a matrix by a scalar
Matrix* multiply_matrix_by_scalar(const Matrix *M, double scalar) {
    if (M->rows == 0 || M->cols == 0) {
        printf("Error: One of dimensions of multiply matrix on scalar has value 0.\n");
        return &MATRIX_NULL;
    };

    Matrix *result = create_matrix(M->rows, M->cols);
    for (size_t i = 0; i < M->rows * M->cols; i++) {
        result->data[i] = M->data[i] * scalar;
    }
    return result;
}


Matrix* exponential_matrix(const Matrix *M) {
    if (M->rows == 0 || M->cols == 0) {
        printf("Error: Matrix should be size more then zero!\n");
        return M;
    }

    if (M->rows != M->cols) {
        printf("Error: Exponential can only be calculated for sqare matrices.\n");
        return M;
    }

    size_t size = M->rows;

    size_t factorial = 1;

    Matrix *result = create_square_matrix(size);
    Matrix *power = create_square_matrix(size);

    // Инициализируем как единичную матрицу
    for (size_t row = 0; row < size; row++) {
        for (size_t col = 0; col < size; col++) {
            result->data[row * size + col] = (row == col) ? 1 : 0;
        }
    }

    // Расчёт матричного экспоненциального ряда
    for (int iteration = 1; iteration <= EXPONENT_ITERATION_COUNT; iteration++) {






        // Расчёт степени матрицы
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
            }
        }

        factorial *= iteration;

    }

    free_matrix(power);

    return result;
}


// Function to calculate the determinant of matrix by minors
Matrix_data_type determinant_of_matrix(const Matrix *M) {
    if (M->rows == 0 || M->cols == 0) {
        printf("Error: Matrix should be size more then zero!\n");
        return 0;
    }

    if (M->rows != M->cols) {
        printf("Error: Determinant can only be calculated for sqare matrices.\n");
        return 0;
    }

    if (M->rows > 7) {
        printf("Error: Can't be calculated for a matrix of dimensions greater than 7.\n");
        return 0;
    }

    size_t size = M->rows;
    if (size == 1) {
        return M->data[0];
    }

    Matrix_data_type det = 0;
    Matrix *submat = create_square_matrix(size - 1);

    for (size_t i = 0; i < size; i++) {
        size_t sub_row = 0;
        for (size_t row = 1; row < size; row++) {
            size_t sub_col = 0;
            for (size_t col = 0; col < size; col++) {
                if (col == i) {
                    continue;
                }

                submat->data[sub_row * submat->cols + sub_col] = M->data[row * M->cols + col];
                sub_col++;
            }

            sub_row++;
        }

        short sign = (i % 2 == 0) ? 1 : -1;
        Matrix_data_type subdet = determinant_of_matrix(submat);
        det += sign * M->data[i] * subdet;
    }

    free_matrix(submat);

    return det;
}


void print_matrix(const Matrix *M) {
    printf("Matrix:\n");

    if (M->rows == 0 || M->cols == 0) {
        printf("Matrix is null.\n\n");
        return;
    }

    for (size_t i = 0; i < M->rows; i++) {
        for (size_t j = 0; j < M->cols; j++) {
            printf("%.2f\t", M->data[i * M->cols + j]);
        }

        printf("\n\n");
    }
}


// Function to fill a matrix (in progress)
void fill_matrix(Matrix *M) {
    for (size_t i = 0; i < M->rows * M->cols; i++) {
        M->data[i] = i * 2 + 1;
    }
}


// Function to fill a matrix with random numbers
void fill_matrix_random(Matrix *M) {
    srand(time(NULL));
    for (size_t i = 0; i < M->rows * M->cols; i++) {
        M->data[i] = (Matrix_data_type)rand() / RAND_MAX;
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

    Matrix_data_type det = determinant_of_matrix(matrix);
    printf("Determinant of the matrix: %.2f\n", det);

    free_matrix(matrix);
}


void case_get_matrix_determinant() {
    size_t size = 3;

    Matrix *matrix = create_square_matrix(size);
    fill_matrix_random(matrix);
    print_matrix(matrix);

    Matrix_data_type det = determinant_of_matrix(matrix);
    printf("Determinant of the matrix: %.2f\n", det);

    free_matrix(matrix);
}


void case_exp() {
    // Example usage of matrix exponential calculation

    // Create a square matrix
    int size = 2; // Change size as needed
    Matrix *matrix = create_square_matrix(size);

    // Fill matrix with values (example)
    matrix->data[0] = 1; matrix->data[1] = 2;
    matrix->data[2] = 3; matrix->data[3] = 4;

    // Calculate matrix exponential
    Matrix *expMat = exponential_matrix(matrix);

    // Print matrix exponential
    printf("Exponential of the matrix:\n");
    print_matrix(expMat);

    // Free allocated memory
    free_matrix(matrix);
    free_matrix(expMat);
}


// TODO OBSOLETE
void case_test_exp() {

    size_t size = 3;

    Matrix *A = create_square_matrix(size);
    fill_matrix(A);
    print_matrix(A);

    Matrix *B = create_square_matrix(size);
    for (size_t row = 0; row < size; row++) {
        for (size_t col = 0; col < size; col++) {
            B->data[row * size + col] = 1;
        }
    }
    print_matrix(B);

    Matrix* C1 = multiply_matrices(A, B);
    print_matrix(C1);

    Matrix* C2 = multiply_matrices(B, A);
    print_matrix(C2);

}


int main() {
    
    //case_test_exp();

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
