#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    size_t rows;
    size_t cols;
    double *data;
} Matrix;
Matrix matrixA; 
Matrix matrixB;
Matrix matrixResult;

// Коды ошибок
enum Error{
    UNDENTIFY_ERROR,
    MEMORY_ALLOCATION_ERROR,
    INAVALID_INPUT,
    MATRIX_SIZE_IS_NOT_EQUAL,
    FIRST_MATCOLS_IS_NOT_EQUAL_SECOND_MATROWS,
    TOO_LARGE_MATRIX_SIZE,
    MATRIX_IS_NOT_SQUARE
};

void handle_error(enum Error error) {
    switch (error) {
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation error.\n");
            break;
        case INAVALID_INPUT:
            printf("Invalid input.\n");
            break;
        case MATRIX_SIZE_IS_NOT_EQUAL:
            printf("Matrix size.\n");
            break;
        case FIRST_MATCOLS_IS_NOT_EQUAL_SECOND_MATROWS:
            printf("Количество стобцов первой матрицы не равно количеству строк второй матрицы.\n");
            break;
        case TOO_LARGE_MATRIX_SIZE:
            printf("Матрица больше, чем ожидалось.\n");
            break;
        case MATRIX_IS_NOT_SQUARE:
            printf("Матрица не квадратная.\n");
            break;
        case UNDENTIFY_ERROR:
            printf("Неизвестная ошибка.\n");
            break;
    }
}

// Нулевая матрица
const Matrix MATRIX_NULL = {
    .cols = 0,
    .rows = 0,
    .data = NULL
};

// Выделение памяти
Matrix matrix_create(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0){
        return MATRIX_NULL;
    }

    if (cols >= SIZE_MAX / (sizeof(double) * rows)){
        handle_error(MEMORY_ALLOCATION_ERROR);
        return MATRIX_NULL;
    }

    Matrix mat;
    mat.data = (double*)malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        handle_error(MEMORY_ALLOCATION_ERROR);
        return MATRIX_NULL;
    }
    mat.cols = cols;
    mat.rows = rows;
    return mat;
}

// Освобождение памяти
void matrix_free(Matrix *mat){
    mat->cols = 0;
    mat->rows = 0;
    free(mat->data);
    mat->data = NULL;
}

void matrix_filling(Matrix *mat) {
    srand(time(NULL));
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            mat->data[i * mat->cols + j] = (double)rand();
        }
    }
}

// Функция для вычисления факториала
double factorial(int n) {
    double result = 1.0;
    for (size_t i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Единичная матрица
Matrix matrix_unit(size_t rows, size_t cols) {
    Matrix matrixUnit = matrix_create(rows, cols);
    if (matrixUnit.data == NULL) {
        handle_error(MEMORY_ALLOCATION_ERROR);
        return MATRIX_NULL;
    }
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (i == j){
                matrixUnit.data[i] = 1.0;
            } 
            else matrixUnit.data[i] = 0.0;
        }
    }

    return matrixUnit;
}

// Вывод матрицы на экран
void matrix_print(Matrix *mat){
    for (size_t rows = 0; rows < mat->rows; ++rows) {
        for (size_t cols = 0; cols < mat->cols; ++cols) {
            printf("%.1f\t", mat->data[mat->cols * rows + cols]);
        }
        printf("\n");
    }
}

// Умножение матрицы на число
void multiplication_by_number(Matrix *mat, double number){
    size_t size = mat->rows * mat->cols;
    for (size_t i = 0; i < size; i++) {
        mat->data[i] *= number;
    }
}

// Сложение матриц
void summation(Matrix *frst, Matrix *scnd){
    if (frst->rows != scnd->rows || frst->cols != scnd->cols){
        handle_error(3);
    }
    for (size_t i = 0; i < frst->rows; ++i) {
        for (size_t j = 0; j < frst->cols; ++j) {
            frst->data[i * frst->cols + j] = \
                frst->data[i * frst->cols + j] + scnd->data[i * scnd->cols + j];
        }
    }
}

// Вычитание матриц
void subtraction(Matrix *frst, Matrix *scnd){
    if (frst->rows != scnd->rows || frst->cols != scnd->cols){
        handle_error(3);
    }
    for (int i = 0; i < frst->rows; ++i) {
        for (int j = 0; j < frst->cols; ++j) {
            frst->data[i * frst->cols + j] = \
                frst->data[i * frst->cols + j] - scnd->data[i * scnd->cols + j];
        }
    }
}

// Умножение матриц
void multiplication(Matrix *frst, Matrix *scnd){
    if (frst->cols != scnd->rows){
        handle_error(4);
    }
    for (int frst_rows = 0; frst_rows < frst->rows; frst_rows++) {
        for (int scnd_mat_cols = 0; scnd_mat_cols < scnd->cols; scnd_mat_cols++) {
            double sum = 0;
            for (int elmnts = 0; elmnts < frst->rows; ++elmnts) {
                sum += frst->data[frst_rows * frst->cols + elmnts] * \
                    scnd->data[elmnts * scnd->cols + scnd_mat_cols];
            }
        }
    }
}

// Функция для вычисления экспоненты матрицы
Matrix matrix_exponential(Matrix *mat) {
    if (mat->rows == 0 || mat->cols == 0) {
        handle_error(MEMORY_ALLOCATION_ERROR);
        return MATRIX_NULL;
    }
    // Начальное значение единичная матрица
    Matrix result = matrix_unit(mat->rows, mat->cols);
    // Вспомогательная матрица
    Matrix temp = matrix_create(mat->rows, mat->cols);

    for (size_t k = 1; k <= 10; ++k) {
        double factor = 1.0 / factorial(k);

        // Добавляем факториал * на текущий член к текущему члену ряда
        for (size_t i = 0; i < result.rows; i++) {
            for (size_t j = 0; j < result.cols; j++) {
                result.data[i * result.cols + j] += factor * temp.data[i * temp.cols + j];
            }
        }

        // Умножаем матрицу-слагаемое на исходную матрицу
        multiplication(&temp, mat);
    }

    matrix_free(&temp);
    return result;
}

// Нахождение определителя
double determinant(Matrix *mat){
    double det;
    if (mat->rows != mat->cols){
        handle_error(MATRIX_IS_NOT_SQUARE);
        return nan("");
    }
    if (mat->rows == 1)
        det = mat->data[0];
    else if (mat->rows == 2)
        det = mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
    else if (mat->rows == 3){
        det = \
        mat->data[0] * mat->data[4] * mat->data[8] +\
        mat->data[1] * mat->data[2] * mat->data[5] +\
        mat->data[3] * mat->data[7] * mat->data[6] -\
        mat->data[6] * mat->data[4] * mat->data[2] -\
        mat->data[7] * mat->data[5] * mat->data[8] -\
        mat->data[3] * mat->data[1] * mat->data[0];
    }
    else{
        handle_error(TOO_LARGE_MATRIX_SIZE);
        return nan("");
    }

    return det;
}

int main(){
    matrixA = matrix_create(3, 3);
    matrix_filling(&matrixA);
    matrix_print(&matrixA);
    matrix_exponential(&matrixA);
    determinant(&matrixA);

    matrix_free(&matrixA);

    return 0;
}