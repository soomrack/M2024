#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structure for a matrix
typedef struct {
    int rows;       // number of rows
    int cols;       // number of columns
    double* data;   // pointer to data
} Matrix;

// Function to create a matrix
Matrix createMatrix(int rows, int cols) {
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

// Function to free the memory allocated for a matrix
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
    for (int i = 0; i < mat1.rows * mat1.cols; i++) {
        result.data[i] = mat1.data[i] + mat2.data[i];
    }
    return result;
}

// Function to print a matrix
void printMatrix(Matrix mat) {
    printf("Matrix:\n");
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            printf("%.2f\t", mat.data[i * mat.cols + j]);
        }
        printf("\n");
    }
}

// Function to fill a matrix (in progress)
void fillMatrix(Matrix mat) {
    for (int i = 0; i < mat.rows * mat.cols; i++) {
        mat.data[i] = i * 2 + 1;
    }
}

// Function to fill a matrix with random numbers
void fillMatrixRandom(Matrix mat) {
    srand(time(NULL)); // Seed the random number generator with current time
    for (int i = 0; i < mat.rows * mat.cols; i++) {
        mat.data[i] = (double)rand() / RAND_MAX; // Generate random double between 0 and 1
    }
}

int main() {
    // TODO temp
    int rows = 2;
    int cols = 3;

    Matrix mat1 = createMatrix(rows, cols);
    Matrix mat2 = createMatrix(rows, cols);

    fillMatrix(mat1);
    fillMatrixRandom(mat2);

    printMatrix(mat1);
    printf("\n");
    printMatrix(mat2);
    printf("\n");

    Matrix result = sumMatrices(mat1, mat2);

    printMatrix(result);

    // Free allocated memory
    freeMatrix(mat1);
    freeMatrix(mat2);
    freeMatrix(result);

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

Matrix:
1.86    3.25    5.49
7.62    9.94    11.67
*/
