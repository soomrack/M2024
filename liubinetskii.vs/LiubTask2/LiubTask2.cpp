// Liubinetskii V. (@liubvlad)
// 
//
// ☑ Добавить базовые операции с матрицей
//
// ☑ Расчёт определителя
// □  Экспонента матрицы (https://ru.wikipedia.org/wiki/%D0%AD%D0%BA%D1%81%D0%BF%D0%BE%D0%BD%D0%B5%D0%BD%D1%82%D0%B0_%D0%BC%D0%B0%D1%82%D1%80%D0%B8%D1%86%D1%8B)
//
// □  Логгер

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    size_t rows;
    size_t cols;
    double* data;
} Matrix;


Matrix createMatrix(size_t rows, size_t cols) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;

    // Allocate memory for the matrix data
    mat.data = (double*)malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    return mat;
}


Matrix createMatrix(size_t size) {
    return createMatrix(size, size);
}


void freeMatrix(Matrix mat) {
    free(mat.data);
}


// Function to sum two matrices
Matrix sumMatrices(Matrix mat1, Matrix mat2) {
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        printf("Error: Matrices have different dimensions.\n");
        exit(1);
    }

    Matrix result = createMatrix(mat1.rows, mat1.cols);
    for (size_t i = 0; i < mat1.rows * mat1.cols; i++) {
        result.data[i] = mat1.data[i] + mat2.data[i];
    }

    return result;
}


// Function to subtract two matrices
Matrix subtractMatrices(Matrix mat1, Matrix mat2) {
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        printf("Error: Matrices have different dimensions.\n");
        exit(1);
    }

    Matrix result = createMatrix(mat1.rows, mat1.cols);
    for (size_t i = 0; i < mat1.rows * mat1.cols; i++) {
        result.data[i] = mat1.data[i] - mat2.data[i];
    }

    return result;
}


// Function to multiply two matrices
Matrix multiplyMatrices(Matrix mat1, Matrix mat2) {
    if (mat1.cols != mat2.rows) {
        printf("Error: Matrices cannot be multiplied due to incompatible dimensions.\n");
        exit(1);
    }

    Matrix result = createMatrix(mat1.rows, mat2.cols);
    for (size_t i = 0; i < mat1.rows; i++) {
        for (size_t j = 0; j < mat2.cols; j++) {
            double sum = 0;
            for (size_t k = 0; k < mat1.cols; k++) {
                sum += mat1.data[i * mat1.cols + k] * mat2.data[k * mat2.cols + j];
            }

            result.data[i * mat2.cols + j] = sum;
        }
    }

    return result;
}


// Function to multiply a matrix by a scalar
Matrix multiplyMatrixByScalar(Matrix mat, double scalar) {
    Matrix result = createMatrix(mat.rows, mat.cols);
    for (size_t i = 0; i < mat.rows * mat.cols; i++) {
        result.data[i] = mat.data[i] * scalar;
    }

    return result;
}


void matrixExponential(Matrix mat) {
    // Validate

    // Some magic

    // return result;
}


// Function to calculate the determinant of matrix
double determinant(Matrix mat) {
    if (mat.rows == 0 || mat.cols == 0) {
        printf("Error: Matrix should be size more then zero!\n");
        return 0;
    }

    if (mat.rows != mat.cols) {
        printf("Error: Determinant can only be calculated for sqare matrices.\n");
        return 0;
    }

    size_t mat_size = mat.rows;
    if (mat_size == 1) {
        return mat.data[0];
    }

    double det = 0;
    Matrix submat = createMatrix(mat_size - 1);

    for (size_t i = 0; i < mat_size; i++) {
        size_t sub_i = 0;
        for (size_t row = 1; row < mat_size; row++) {
            size_t sub_j = 0;
            for (size_t col = 0; col < mat_size; col++) {
                if (col == i) {
                    continue;
                }

                submat.data[sub_i * submat.cols + sub_j] = mat.data[row * mat.cols + col];
                sub_j++;
            }

            sub_i++;
        }

        short sign = (i % 2 == 0) ? 1 : -1;
        double subdet = determinant(submat);
        det += sign * mat.data[i] * subdet;
    }

    freeMatrix(submat);

    return det;
}


void printMatrix(Matrix mat) {
    printf("Matrix:\n");
    for (size_t i = 0; i < mat.rows; i++) {
        for (size_t j = 0; j < mat.cols; j++) {
            printf("%.2f\t", mat.data[i * mat.cols + j]);
        }

        printf("\n\n");
    }
}


// Function to fill a matrix (in progress)
void fillMatrix(Matrix mat) {
    for (size_t i = 0; i < mat.rows * mat.cols; i++) {
        mat.data[i] = i * 2 + 1;
    }
}


// Function to fill a matrix with random numbers
void fillMatrixRandom(Matrix mat) {
    srand(time(NULL));
    for (size_t i = 0; i < mat.rows * mat.cols; i++) {
        mat.data[i] = (double)rand() / RAND_MAX;
    }
}


void case_sum_and_print() {
    size_t rows = 2;
    size_t cols = 3;

    Matrix mat1 = createMatrix(rows, cols);
    Matrix mat2 = createMatrix(rows, cols);

    fillMatrix(mat1);
    fillMatrixRandom(mat2);

    printMatrix(mat1);
    printMatrix(mat2);

    Matrix result = sumMatrices(mat1, mat2);
    printMatrix(result);

    freeMatrix(mat1);
    freeMatrix(mat2);
    freeMatrix(result);
}


void case_prepair_matrix_and_get_determinant() {
    size_t size = 3;

    Matrix mat = createMatrix(size);
    mat.data[0] = 1; mat.data[1] = 2; mat.data[2] = 3;
    mat.data[3] = 4; mat.data[4] = 5; mat.data[5] = 6;
    mat.data[6] = 7; mat.data[7] = 8; mat.data[8] = 9;

    printMatrix(mat);

    double det = determinant(mat);
    printf("Determinant of the matrix: %.2f\n", det);

    freeMatrix(mat);
}


void case_get_matrix_determinant() {
    size_t size = 3;

    Matrix mat = createMatrix(size);
    fillMatrixRandom(mat);
    printMatrix(mat);

    double det = determinant(mat);
    printf("Determinant of the matrix: %.2f\n", det);

    freeMatrix(mat);
}


int main() {
    
    case_get_matrix_determinant();

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
