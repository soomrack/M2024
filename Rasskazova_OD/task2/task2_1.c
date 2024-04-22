#include <stdio.h>
#include <stdlib.h>

enum ErrorType { ERROR };

void matrix_error(enum ErrorType type, const char *message) 
{
    if (type == ERROR) {
        printf("ОШИБКА! %s\n", message);
    }
}

struct Matrix 
{
    size_t cols;
    size_t rows;
    double *data;
};

typedef struct Matrix Matrix;

const struct Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL }; 

Matrix createMatrix(size_t rows, size_t cols) {
    Matrix matrix;
    matrix.rows = rows;
    matrix.cols = cols;

    // Выделение памяти под данные матрицы
    matrix.data = (double *)malloc(rows * cols * sizeof(double));

    if (matrix.data == NULL) {
        matrix_error(ERROR, "Ошибка выделения памяти");
        matrix.cols = 0;
        matrix.rows = 0;
    }

    return matrix;
}

// Функция для освобождения памяти, выделенной под матрицу
void freeMatrix(Matrix matrix) {
    free(matrix.data);
}

Matrix sum(Matrix matrix1, Matrix matrix2) {
    Matrix result = createMatrix(matrix1.rows, matrix1.cols);

    for (size_t i = 0; i < matrix1.rows * matrix1.cols; i++) {
        result.data[i] = matrix1.data[i] + matrix2.data[i];
    }

    return result;
}

Matrix sub(Matrix matrix1, Matrix matrix2) {
    Matrix result = createMatrix(matrix1.rows, matrix1.cols);

    for (size_t i = 0; i < matrix1.rows * matrix1.cols; i++) {
        result.data[i] = matrix1.data[i] - matrix2.data[i];
    }

    return result;
}

Matrix mult_scalar(Matrix matrix, double scalar) {
    Matrix result = createMatrix(matrix.rows, matrix.cols);

    for (size_t i = 0; i < matrix.rows; i++) {
        for (size_t j = 0; j < matrix.cols; j++) {
            result.data[i * result.cols + j] = matrix.data[i * matrix.cols + j] * scalar;
        }
    }

    return result;
}

Matrix mult(Matrix matrix1, Matrix matrix2) {
    if (matrix1.cols != matrix2.rows) {
        matrix_error(ERROR, "Невозможно перемножить матрицы: неправильные размеры");
        return MATRIX_NULL;
    }

    Matrix result = createMatrix(matrix1.rows, matrix2.cols);

    for (size_t i = 0; i < matrix1.rows; i++) {
        for (size_t j = 0; j < matrix2.cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < matrix1.cols; k++) {
                sum += matrix1.data[i * matrix1.cols + k] * matrix2.data[k * matrix2.cols + j];
            }
            result.data[i * result.cols + j] = sum;
        }
    }

    return result;
}

// Функция для вывода матрицы 
void printMatrix(Matrix matrix) {
    for (size_t i = 0; i < matrix.rows; i++) {
        for (size_t j = 0; j < matrix.cols; j++) {
            printf("%0.1f ", matrix.data[i * matrix.cols + j]);
        }
        printf("\n");
    }
}

int main() {
    // Создание матрицы 1
    Matrix matrix1 = createMatrix(3, 3);
    matrix1.data[0] = 1;
    matrix1.data[1] = 2;
    matrix1.data[2] = 3;
    matrix1.data[3] = 4;
    matrix1.data[4] = 5;
    matrix1.data[5] = 6;
    matrix1.data[6] = 7;
    matrix1.data[7] = 8;
    matrix1.data[8] = 9;

    // Создание матрицы 2
    Matrix matrix2 = createMatrix(3, 3);
    matrix2.data[0] = 9;
    matrix2.data[1] = 8;
    matrix2.data[2] = 7;
    matrix2.data[3] = 6;
    matrix2.data[4] = 5;
    matrix2.data[5] = 4;
    matrix2.data[6] = 3;
    matrix2.data[7] = 2;
    matrix2.data[8] = 1;

    Matrix result_sum = sum(matrix1, matrix2);
    Matrix result_sub = sub(matrix1, matrix2);
    Matrix result_mult_scalar = mult_scalar(matrix1, 5);
    Matrix result_mult = mult(matrix1, matrix2);
    
    // Вывод результатов
    printf("Матрица 1:\n");
    printMatrix(matrix1);
    printf("\nМатрица 2:\n");
    printMatrix(matrix2);
    printf("\nРезультат сложения матриц:\n");
    printMatrix(result_sum);
    printf("\nРезультат вычитания матриц:\n");
    printMatrix(result_sub);
    printf("\nРезультат умножения матрицы на скаляр:\n");
    printMatrix(result_mult_scalar);
    printf("\nРезультат перемножения матриц:\n");
    printMatrix(result_mult);

    // Освобождение памяти
    freeMatrix(matrix1);
    freeMatrix(matrix2);
    freeMatrix(result_sum);
    freeMatrix(result_sub);
    freeMatrix(result_mult);
    freeMatrix(result_mult_scalar);

    return 0;
}