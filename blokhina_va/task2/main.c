#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    size_t rows;
    size_t cols;
    double **data;
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
const Matrix matrixNull = {
    .cols = 0,
    .rows = 0,
    .data = NULL
};

// Выделение памяти
Matrix matrix_create(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0){
        return matrixNull;
    }

    if (cols >= SIZE_MAX / (sizeof(double) * rows)){
        handle_error(MEMORY_ALLOCATION_ERROR);
        return matrixNull;
    }

    Matrix mat;
    mat.data = (double**)malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        handle_error(MEMORY_ALLOCATION_ERROR);
        return matrixNull;
    }
    mat.cols = cols;
    mat.rows = rows;
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
        mat->data[i] = (double *)malloc(mat->cols * sizeof(double));
        for (size_t j = 0; j < mat->cols; j++) {
            mat->data[i][j] = (double)rand() / RAND_MAX;
        }
    }
}
// Функция для вычисления факториала
double factorial(int n) {
    double result = 1.0;
    for (unsigned i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Единичная матрица
Matrix matrix_unit(size_t rows, size_t cols) {
    Matrix matrixUnit = matrix_create(rows, cols);
    if (matrixUnit.data == NULL) {
        handle_error(MEMORY_ALLOCATION_ERROR);
        return matrixNull;
    }
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (i == j){
                matrixUnit.data[i][j] = 1.0;
            } 
            else matrixUnit.data[i][j] = 0.0;
        }
    }

    return matrixUnit;
}

// Вывод матрицы на экран
void matrix_print(Matrix *mtrx){
    for (size_t i = 0; i < mtrx->rows; ++i) {
        for (size_t j = 0; j < mtrx->cols; ++j) {
            printf("%.1f\t", mtrx->data[i][j]);
        }
        printf("\n");
    }
}

// Умножение матрицы на число
void multiplication_by_number(Matrix *mat, double number){
    for (int i = 0; i < mat->rows; ++i) {
        for (int j = 0; j < mat->cols; ++j) {
            mat->data[i][j] = mat->data[i][j] * number;
        }
    }
}

// Сложение матриц
void summation(Matrix *frst, Matrix *scnd){
    if (frst->rows != scnd->rows || frst->cols != scnd->cols){
        handle_error(3);
    }
    for (int i = 0; i < frst->rows; ++i) {
        for (int j = 0; j < frst->cols; ++j) {
            frst->data[i][j] = frst->data[i][j] + scnd->data[i][j];
        }
    }
}

// Вычитание матриц
void subtraction(Matrix *p_frst, Matrix *p_scnd, Matrix *p_rslt){
    if (p_frst->rows != p_scnd->rows || p_frst->cols != p_scnd->cols){
        handle_error(3);
    }
    p_rslt->rows = p_frst->rows;
    p_rslt->cols = p_frst->cols;
    for (int i = 0; i < p_rslt->rows; ++i) {
        for (int j = 0; j < p_rslt->cols; ++j) {
            p_rslt->data[i][j] = p_frst->data[i][j] - p_scnd->data[i][j];
        }
    }
}

// Умножение матриц
void multiplication(Matrix *frst, Matrix *scnd){
    if (frst->cols != scnd->rows){
        handle_error(4);
    }
    for (int first_mat_row = 0; first_mat_row < frst->rows; ++first_mat_row) {
        for (int second_mat_col = 0; second_mat_col < scnd->cols; ++second_mat_col) {
            frst->data[first_mat_row][second_mat_col] = 0;
            for (int elmnts_multi = 0; elmnts_multi < frst->rows; ++elmnts_multi) {
                frst->data[first_mat_row][second_mat_col] += frst->data[first_mat_row][elmnts_multi] * scnd->data[elmnts_multi][second_mat_col];
            }
        }
    }
}

// Функция для вычисления экспоненты матрицы
Matrix matrix_exponential(Matrix *mat) {
    if (mat->rows == 0 || mat->cols == 0) {
        handle_error(MEMORY_ALLOCATION_ERROR);
        return matrixNull;
    }
    // Начальное значение единичная матрица
    Matrix result = matrix_unit(mat->rows, mat->cols);
    Matrix temp = matrix_unit(mat->rows, mat->cols);

    for (int iteration = 1; iteration <= 10; iteration++){
        Matrix copy_ptr = temp;
        multiplication(&temp, mat);
        matrix_free(&copy_ptr);

        copy_ptr = temp;
        multiplication_by_number(&temp, 1.0/iteration);
        matrix_free(&copy_ptr);

        copy_ptr = result;
        summation(&result, &temp);
        matrix_free(&copy_ptr);
    }

    matrix_free(&temp);
    return result;
}

// Нахождение определителя
double determinant(Matrix *mtrx){
    double det;
    if (mtrx->rows != mtrx->cols){
        handle_error(6);
        return nan("");
    }
    if (mtrx->rows == 2)
        det = mtrx->data[0][0] * mtrx->data[1][1] - mtrx->data[0][1] * mtrx->data[1][0];
    else if (mtrx->rows == 3){
        det = \
        mtrx->data[0][0] * mtrx->data[1][1] *mtrx->data[2][2] +\
        mtrx->data[1][0] * mtrx->data[2][1] *mtrx->data[0][2] +\
        mtrx->data[0][1] * mtrx->data[1][2] *mtrx->data[2][0] -\
        mtrx->data[2][0] * mtrx->data[1][1] *mtrx->data[0][2] -\
        mtrx->data[2][1] * mtrx->data[1][2] *mtrx->data[0][0] -\
        mtrx->data[1][0] * mtrx->data[0][1] *mtrx->data[2][2];
    }
    else{
        handle_error(5);
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