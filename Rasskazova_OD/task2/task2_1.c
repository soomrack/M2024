#include <stdio.h>
#include <stdlib.h>
#include <math.h>


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

Matrix create_matrix(size_t rows, size_t cols) {
    Matrix matrix;
    matrix.rows = rows;
    matrix.cols = cols;

    if (SIZE_MAX / cols / rows < sizeof(double)){
        return MATRIX_NULL;
    }

    if (rows * cols == 0) {
        matrix_error(ERROR, "Нулевые размеры матрицы"); 
        matrix.cols = 0;
        matrix.rows = 0;
        matrix.data = NULL; 
        return matrix; 
    }

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
void free_matrix(Matrix *matrix) {
    free(matrix->data);
    matrix->data = NULL;
    matrix->cols = 0;
    matrix->rows = 0;
}


Matrix sum(Matrix matrixA, Matrix matrixB) {
    if (matrixA.data == NULL || matrixB.data == NULL) {
         matrix_error(ERROR, "Невозможно сложить матрицы: матрицы пусты");
        return MATRIX_NULL;
    }

    if (matrixA.cols != matrixB.rows) {
        matrix_error(ERROR, "Невозможно сложить матрицы: неправильные размеры");
        return MATRIX_NULL;
    }

    Matrix result = create_matrix(matrixA.rows, matrixA.cols);

    for (size_t idx = 0; idx < matrixA.rows * matrixA.cols; idx++) {
        result.data[idx] = matrixA.data[idx] + matrixB.data[idx];
    }

    return result;
}


Matrix sub(Matrix matrixA, Matrix matrixB) {
    if (matrixA.data == NULL || matrixB.data == NULL) {
         matrix_error(ERROR, "Невозможно вычесть матрицы: матрицы пусты");
        return MATRIX_NULL;
    }

    if (matrixA.cols != matrixB.rows) {
        matrix_error(ERROR, "Невозможно вычесть матрицы: неправильные размеры");
        return MATRIX_NULL;
    }

    Matrix result = create_matrix(matrixA.rows, matrixA.cols);

    for (size_t idx = 0; idx < matrixA.rows * matrixA.cols; idx++) {
        result.data[idx] = matrixA.data[idx] - matrixB.data[idx];
    }

    return result;
}


Matrix mult_scalar(Matrix matrix, double scalar) {
    if (matrix.data == NULL) {
         matrix_error(ERROR, "Невозможно произвети умножение: матрица пуста");
        return MATRIX_NULL;
    }

    Matrix result = create_matrix(matrix.rows, matrix.cols);

    for (size_t idx = 0; idx < matrix.rows * matrix.cols; idx++) {
        result.data[idx] = matrix.data[idx ] * scalar;
    }

    return result;
}


Matrix mult(Matrix matrixA, Matrix matrixB) {
    if (matrixA.data == NULL || matrixB.data == NULL) {
         matrix_error(ERROR, "Невозможно перемножить матрицы: матрицы пусты");
        return MATRIX_NULL;
    }

    if (matrixA.cols != matrixB.rows) {
        matrix_error(ERROR, "Невозможно перемножить матрицы: неправильные размеры");
        return MATRIX_NULL;
    }

    Matrix result = create_matrix(matrixA.rows, matrixB.cols);

    for (size_t  rows_A= 0; rows_A < matrixA.rows; rows_A++) {
        for (size_t cols_B = 0; cols_B < matrixB.cols; cols_B++) {
            double sum = 0.0;
            for (size_t cols_A = 0; cols_A < matrixA.cols; cols_A++) {
                sum += matrixA.data[rows_A * matrixA.cols + cols_A] * matrixB.data[cols_A * matrixB.cols + cols_B];
            }
            result.data[rows_A * result.cols + cols_B] = sum;
        }
    }

    return result;
}


double det(Matrix matrix) {
    Matrix mat = create_matrix(matrix.rows, matrix.cols); 
    double result = 0;

    if (mat.rows != mat.cols) {
        matrix_error(ERROR, "Невозможно перемножить матрицы: неправильные размеры");
        return NAN;
    }

    if (matrix.cols == 1) {
        result = matrix.data[0];
        return result;
    }
    
    if (matrix.cols == 2) {
        double result = (matrix.data[0]) * (matrix.data[3]) - (matrix.data[1]) * (matrix.data[2]);
		return result;
    }

    if (matrix.cols == 3) {
        double result = (matrix.data[0]) * (matrix.data[4]) * (matrix.data[8]) + (matrix.data[1]) * (matrix.data[5]) * (matrix.data[6]) + (matrix.data[3]) * (matrix.data[7]) * (matrix.data[2]);
		result -= ((matrix.data[2]) * (matrix.data[4]) * (matrix.data[6]) + (matrix.data[1]) * (matrix.data[3]) * (matrix.data[8]) + (matrix.data[0]) * (matrix.data[5]) * (matrix.data[7]));
		return result;
    }

    return NAN;
}


// Функция для вывода матрицы 
void print_matrix(Matrix matrix) {
    for (size_t rows = 0; rows < matrix.rows; rows++) {
        for (size_t cols = 0; cols < matrix.cols; cols++) {
            printf("%0.1f ", matrix.data[rows * matrix.cols + cols]);
        }
        printf("\n");
    }
}


int main() {
    // Создание матрицы 1
    Matrix matrixA = create_matrix(3, 3);
    matrixA.data[0] = 1;
    matrixA.data[1] = 2;
    matrixA.data[2] = 3;
    matrixA.data[3] = 4;
    matrixA.data[4] = 5;
    matrixA.data[5] = 6;
    matrixA.data[6] = 7;
    matrixA.data[7] = 8;
    matrixA.data[8] = 9;

    // Создание матрицы 2
    Matrix matrixB = create_matrix(3, 3);
    matrixB.data[0] = 9;
    matrixB.data[1] = 8;
    matrixB.data[2] = 7;
    matrixB.data[3] = 6;
    matrixB.data[4] = 5;
    matrixB.data[5] = 4;
    matrixB.data[6] = 3;
    matrixB.data[7] = 2;
    matrixB.data[8] = 1;

    Matrix result_sum = sum(matrixA, matrixB);
    Matrix result_sub = sub(matrixA, matrixB);
    Matrix result_mult_scalar = mult_scalar(matrixA, 5);
    Matrix result_mult = mult(matrixA, matrixB);
    double result_det = det(matrixA);
    
    // Вывод результатов
    printf("Матрица 1:\n");
    print_matrix(matrixA);
    printf("\nМатрица 2:\n");
    print_matrix(matrixB);
    printf("\nРезультат сложения матриц:\n");
    print_matrix(result_sum);
    printf("\nРезультат вычитания матриц:\n");
    print_matrix(result_sub);
    printf("\nРезультат умножения матрицы на скаляр:\n");
    print_matrix(result_mult_scalar);
    printf("\nРезультат перемножения матриц:\n");
    print_matrix(result_mult);
    printf("\nРезультат вычисления определителя:\n%0.1f \n", result_det);
    
    // Освобождение памяти
    free_matrix(&matrixA);
    free_matrix(&matrixB);
    free_matrix(&result_sum);
    free_matrix(&result_sub);
    free_matrix(&result_mult);
    free_matrix(&result_mult_scalar);

    return 0;
}