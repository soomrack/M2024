// Задача:
// Необходимо производить с матрицами с ними следующие действия:
// складывать, вычитать, умножать, находить определитель, умножать на скаляр

#include <stdio.h>
#include <stdlib.h>
// #include <stddef.h>

typedef struct {
    size_t rows;
    size_t cols;
    double **data;
} Matrix;
Matrix matrixA;
Matrix matrixB;
Matrix matrixResult;

// Коды ошибок
void handle_error(size_t error_code) {
    switch (error_code) {
        case 1:
            printf("Ошибка выделения памяти!\n");
            break;
        case 2:
            printf("Некорректный ввод.\n");
            break;
        case 3:
            printf("Размеры матриц не равны.\n");
            break;
        case 4:
            printf("Количество стобцов первой матрицы не равно количеству строк второй матрицы.\n");
            break;
        case 5:
            printf("Матрица больше, чем ожидалось.\n");
            break;
        case 6:
            printf("Матрица не квадратная.\n");
            break;
        default:
            printf("Неизвестная ошибка.\n");
            break;
    }
    exit(0);
}

// Выделение памяти
Matrix matrix_create(size_t rows, size_t cols, double coeff) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;

    // Выделяем память для массива указателей на строки
    mat.data = (double **)malloc(rows * sizeof(double *));
    if (mat.data == NULL) {
        handle_error(1);
    }

    // Выделяем память для каждой строки
    for (size_t i = 0; i < rows; ++i) {
        mat.data[i] = (double *)malloc(cols * sizeof(double));
        if (mat.data[i] == NULL) {
            handle_error(1);
        }
    }
    // Присваиваем значение
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            mat.data[i][j] = (double)((i * cols + j) * coeff);
        }
    }
    return mat;
}

// Вывод матрицы на экран
void matrix_print(Matrix *p_mtrx){
    for (size_t i = 0; i < p_mtrx->rows; ++i) {
        for (size_t j = 0; j < p_mtrx->cols; ++j) {
            printf("%.1f\t", p_mtrx->data[i][j]);
        }
        printf("\n");
    }
}

// Умножение матрицы на число
void multiplication_by_number(Matrix *p_old, Matrix *p_new, double number){
    for (int i = 0; i < p_new->rows; ++i) {
        for (int j = 0; j < p_new->cols; ++j) {
            p_new->data[i][j] = p_old->data[i][j] * number;
        }
    }
}

// Сложение матриц
void summation(Matrix *p_frst, Matrix *p_scnd, Matrix *p_rslt){
    if (p_frst->rows == p_scnd->rows && p_frst->cols == p_scnd->cols){
        p_rslt->rows = p_frst->rows;
        p_rslt->cols = p_frst->cols;

        for (int i = 0; i < p_rslt->rows; ++i) {
            for (int j = 0; j < p_rslt->cols; ++j) {
                p_rslt->data[i][j] = p_frst->data[i][j] + p_scnd->data[i][j];
            }
        }
    }
    else {
        handle_error(3);
    }
}

// Вычитание матриц
void subtraction(Matrix *p_frst, Matrix *p_scnd, Matrix *p_rslt){
    if (p_frst->rows == p_scnd->rows && p_frst->cols == p_scnd->cols){
        p_rslt->rows = p_frst->rows;
        p_rslt->cols = p_frst->cols;

        for (int i = 0; i < p_rslt->rows; ++i) {
            for (int j = 0; j < p_rslt->cols; ++j) {
                p_rslt->data[i][j] = p_frst->data[i][j] - p_scnd->data[i][j];
            }
        }
    }
    else {
        handle_error(3);
    }
}

// Умножение матриц
void multiplication(Matrix *p_frst, Matrix *p_scnd, Matrix *p_rslt){
    if (p_frst->cols != p_scnd->rows){
        handle_error(4);
    }
    for (int i = 0; i < p_frst->rows; ++i) {
        for (int j = 0; j < p_scnd->cols; ++j) {
            for (int k = 0; k < p_frst->rows; ++k) {
                p_rslt->data[i][j] += p_frst->data[i][k] * p_scnd->data[k][j];
            }
        }
    }
}

// Нахождение определителя
void determinant(Matrix *p_mtrx){
    double det;
    if (p_mtrx->rows == p_mtrx->cols){
        if (p_mtrx->rows == 2)
            det = p_mtrx->data[0][0] * p_mtrx->data[1][1] - p_mtrx->data[0][1] * p_mtrx->data[1][0];
        else if (p_mtrx->rows == 3){
            det = \
            p_mtrx->data[0][0] * p_mtrx->data[1][1] *p_mtrx->data[2][2] +\
            p_mtrx->data[1][0] * p_mtrx->data[2][1] *p_mtrx->data[0][2] +\
            p_mtrx->data[0][1] * p_mtrx->data[1][2] *p_mtrx->data[2][0] -\
            p_mtrx->data[2][0] * p_mtrx->data[1][1] *p_mtrx->data[0][2] -\
            p_mtrx->data[2][1] * p_mtrx->data[1][2] *p_mtrx->data[0][0] -\
            p_mtrx->data[1][0] * p_mtrx->data[0][1] *p_mtrx->data[2][2];
        }
        else
            handle_error(5);
    }
    else
        handle_error(6);
    printf("Определитель матрицы равен:%.1f\n", det);
    exit(0);
}


// Pre-действия: выбор операции
void operation_choosing(){
    char task_name_multiplication_by_number[] = "Умножение матрицы на число";
    char task_name_summation[] = "Сложение матриц";
    char task_name_subtraction[] = "Вычитание матриц";
    char task_name_multiplication[] = "Умножение матриц";
    char task_name_determinant[] = "Нахождение определителя";

    int task_number;
    printf("Введите номер задания.\n\
        1.%s.\n\
        2.%s.\n\
        3.%s.\n\
        4.%s.\n\
        5.%s.\n",
        task_name_multiplication_by_number,
        task_name_summation,
        task_name_subtraction,
        task_name_multiplication,
        task_name_determinant);
    scanf("%d", &task_number);
    printf("------------\n");

    switch (task_number){
    case 1:
        printf("Вы выбрали %s.\n", task_name_multiplication_by_number);
        matrixResult = matrix_create(matrixA.rows, matrixA.cols, 6);
        multiplication_by_number(&matrixA, &matrixResult, 5);
        break;
    case 2:
        printf("Вы выбрали %s.\n", task_name_summation);
        matrixResult = matrix_create(matrixA.rows, matrixA.cols, 0);
        summation(&matrixA, &matrixB, &matrixResult);
        break;
    case 3:
        printf("Вы выбрали %s.\n", task_name_subtraction);
        matrixResult = matrix_create(matrixA.rows, matrixA.cols, 0);
        subtraction(&matrixA, &matrixB, &matrixResult);
        break;
    case 4:
        printf("Вы выбрали %s.\n", task_name_multiplication);
        matrixResult = matrix_create(matrixA.cols, matrixB.rows, 0);
        multiplication(&matrixA, &matrixB, &matrixResult);
        break;
    case 5:
        printf("Вы выбрали %s.\n", task_name_determinant);   
        determinant(&matrixA);
        break;
    default:
        handle_error(2);
    }
    printf("------------\nРезультат:\n");
}

// Освобождение памяти
void free_mem(Matrix *mat){
    for (size_t i = 0; i < mat->rows; ++i) {
        free(mat->data[i]);
    }
    free(mat->data);
    mat->data = NULL;
}

int main(){
    matrixA = matrix_create(3, 3, 1.8);
    matrixB = matrix_create(3, 3, 2);
    
    operation_choosing();

    matrix_print(&matrixResult);


    free_mem(&matrixA);
    free_mem(&matrixB);
    free_mem(&matrixResult);

    return 0;
}