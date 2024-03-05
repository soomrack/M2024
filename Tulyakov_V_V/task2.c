#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows;
    int cols;
    double** data;
} Matrix;

// Функция для инициализации матрицы
Matrix matrix_init(int rows, int cols) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        mat.data[i] = (double*)malloc(cols * sizeof(double));
    }
    return mat;
}

// Функция для освобождения памяти, выделенной под матрицу
void matrix_free(Matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        free(mat.data[i]);
    }
    free(mat.data);
}

// Функция для вывода матрицы на экран
void matrix_print(Matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            printf("%.2f\t", mat.data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Функция для сложения матриц
Matrix matrix_add(Matrix A, Matrix B) {
    Matrix result = matrix_init(A.rows, A.cols);
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            result.data[i][j] = A.data[i][j] + B.data[i][j];
        }
    }
    return result;
}

// Функция для вычитания матриц
Matrix matrix_subtract(Matrix A, Matrix B) {
    Matrix result = matrix_init(A.rows, A.cols);
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            result.data[i][j] = A.data[i][j] - B.data[i][j];
        }
    }
    return result;
}

// Функция для умножения матриц
Matrix matrix_multiply(Matrix A, Matrix B) {
    Matrix result = matrix_init(A.rows, B.cols);
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.cols; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < A.cols; k++) {
                result.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    return result;
}

// Функция для транспонирования матрицы
Matrix matrix_transpose(Matrix A) {
    Matrix result = matrix_init(A.cols, A.rows);
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            result.data[j][i] = A.data[i][j];
        }
    }
    return result;
}

// Функция для нахождения определителя матрицы (для квадратных матриц)
double matrix_determinant(Matrix A) {
    if (A.rows != A.cols) {
        printf("Matrix is not square\n");
        return 0;
    }

    if (A.rows == 1) {
        return A.data[0][0];
    }

    if (A.rows == 2) {
        return A.data[0][0] * A.data[1][1] - A.data[0][1] * A.data[1][0];
    }

    double det = 0;
    for (int j = 0; j < A.cols; j++) {
        Matrix minor = matrix_init(A.rows - 1, A.cols - 1);
        for (int k = 1; k < A.rows; k++) {
            for (int l = 0; l < A.cols; l++) {
                if (l < j) {
                    minor.data[k - 1][l] = A.data[k][l];
                }
                else if (l > j) {
                    minor.data[k - 1][l - 1] = A.data[k][l];
                }
            }
        }
        double sign = (j % 2 == 0) ? 1 : -1;
        det += sign * A.data[0][j] * matrix_determinant(minor);
        matrix_free(minor);
    }
    return det;
}

// Пример использования программы
int main() {
    Matrix A = matrix_init(2, 2);
    A.data[0][0] = 1; A.data[0][1] = 2;
    A.data[1][0] = 3; A.data[1][1] = 4;

    Matrix B = matrix_init(2, 2);
    B.data[0][0] = 5; B.data[0][1] = 6;
    B.data[1][0] = 7; B.data[1][1] = 8;

    printf("Matrix A:\n");
    matrix_print(A);

    printf("Matrix B:\n");
    matrix_print(B);

    Matrix C = matrix_add(A, B);
    printf("Matrix A + B:\n");
    matrix_print(C);

    Matrix D = matrix_subtract(A, B);
    printf("Matrix A - B:\n");
    matrix_print(D);

    Matrix E = matrix_multiply(A, B);
    printf("Matrix A * B:\n");
    matrix_print(E);

    Matrix F = matrix_transpose(A);
    printf("Transpose of Matrix A:\n");
    matrix_print(F);

    double det_A = matrix_determinant(A);
    printf("Determinant of Matrix A: %.2f\n", det_A);

    matrix_free(A);
    matrix_free(B);
    matrix_free(C);
    matrix_free(D);
    matrix_free(E);
    matrix_free(F);

    return 0;
}
