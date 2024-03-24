// Задача:
// Необходимо производить с матрицами с ними следующие действия:
// складывать, вычитать, умножать, находить определитель, умножать на скаляр

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

    // size_t size_required = rows * cols * sizeof(double);
    // if (size_required > MAX_SIZE) {
    //     fprintf(stderr, "Error: Matrix size exceeds maximum allowed size.\n");
    //     exit(EXIT_FAILURE);
    // }

    // Выделение памяти под всю матрицу одним блоком
    double *matrix_data = (double *)malloc(rows * cols * sizeof(double));
    if (matrix_data == NULL) {
        // Обработка ошибки выделения памяти
        exit(1);
    }

    // Выделение памяти под массив указателей на строки
    mat.data = (double **)malloc(rows * sizeof(double *));
    if (mat.data == NULL) {
        free(matrix_data);
        exit(1);
    }

    // Проставление указателей на строки
    for (size_t i = 0; i < rows; ++i) {
        mat.data[i] = matrix_data + i * cols;
    }

    // Присваиваем значение
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            mat.data[i][j] = (double)((i * cols + j) * coeff);
        }
    }
    return mat;
}
// Функция для вычисления факториала
double factorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Функция для вычисления экспоненты матрицы
Matrix matrix_exponential(const Matrix A) {
    Matrix result = matrix_create(A.rows, A.cols,1 );
    Matrix term = matrix_create(A.rows, A.cols, 1);
    Matrix power = matrix_create(A.rows, A.cols, 1);
    int n;

    // Начальное значение матрицы результата - единичная матрица
    for (size_t i = 0; i < A.rows; ++i) {
        for (size_t j = 0; j < A.cols; ++j) {
            result.data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    // Считаем экспоненту матрицы с помощью ряда Тейлора
    for (n = 0; n < 10; ++n) { // Рассматриваем первые 10 членов ряда Тейлора
        // Вычисляем A^n
        power = matrix_create(A.rows, A.cols, 1);
        for (size_t i = 0; i < A.rows; ++i) {
            for (size_t j = 0; j < A.cols; ++j) {
                double sum = 0.0;
                for (size_t k = 0; k < A.cols; ++k) {
                    sum += result.data[i][k] * A.data[k][j];
                }
                power.data[i][j] = sum;
            }
        }

        // Вычисляем A^n / n!
        for (size_t i = 0; i < A.rows; ++i) {
            for (size_t j = 0; j < A.cols; ++j) {
                term.data[i][j] = power.data[i][j] / factorial(n);
            }
        }

        // Добавляем A^n / n! к результату
        for (size_t i = 0; i < A.rows; ++i) {
            for (size_t j = 0; j < A.cols; ++j) {
                result.data[i][j] += term.data[i][j];
            }
        }

        free_mem(&power);
    }

    free_mem(&term);
    return result;
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
    if (p_frst->rows != p_scnd->rows || p_frst->cols != p_scnd->cols){
        handle_error(3);
    }
    p_rslt->rows = p_frst->rows;
    p_rslt->cols = p_frst->cols;

    for (int i = 0; i < p_rslt->rows; ++i) {
        for (int j = 0; j < p_rslt->cols; ++j) {
            p_rslt->data[i][j] = p_frst->data[i][j] + p_scnd->data[i][j];
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
void multiplication(Matrix *p_frst, Matrix *p_scnd, Matrix *p_rslt){
    if (p_frst->cols != p_scnd->rows){
        handle_error(4);
    }
    for (int first_mat_row = 0; first_mat_row < p_frst->rows; ++first_mat_row) {
        for (int second_mat_col = 0; second_mat_col < p_scnd->cols; ++second_mat_col) {
            for (int elmnts_multi = 0; elmnts_multi < p_frst->rows; ++elmnts_multi) {
                p_rslt->data[first_mat_row][second_mat_col] += p_frst->data[first_mat_row][elmnts_multi] * p_scnd->data[elmnts_multi][second_mat_col];
            }
        }
    }
}

// Нахождение определителя
void determinant(Matrix *p_mtrx){
    double det;
    if (p_mtrx->rows != p_mtrx->cols){
        handle_error(6);
    }
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
    char task_name_exponent[] = "Нахождение экспоненты";

    int task_number;
    printf("Введите номер задания.\n\
        1.%s.\n\
        2.%s.\n\
        3.%s.\n\
        4.%s.\n\
        5.%s.\n\
        6.%s.\n",
        task_name_multiplication_by_number,
        task_name_summation,
        task_name_subtraction,
        task_name_multiplication,
        task_name_determinant,
        task_name_exponent);
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
    case 6:
      printf("Вы выбрали %s.\n", task_name_exponent);
      matrix_exponential(matrixA);
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
    matrix_print(&matrixA);
    printf("\n");
    matrixB = matrix_create(3, 3, 2);
    matrix_print(&matrixB);
    
    operation_choosing();

    matrix_print(&matrixResult);


    free_mem(&matrixA);
    free_mem(&matrixB);
    free_mem(&matrixResult);

    return 0;
}