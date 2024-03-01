// Задача:
// Имеются две матрицы, необходимо производить с ними следующие действия:
// складывать, вычитать, умножать, находить определитель, умножать на число

// Краткое описание:
// В памяти выделено место под 3 матрицы. 2 из них matrixA и matrixВ инициализированы, матрица matrixResult только объявлена.
// Копий матриц нет, то есть работа исключительно с адресами и указателями.
// Каждый первый элемент новой строки в матрице хранится по адресу НЕ СЛЕДУЮЩЕМУ ДАЛЕЕ после последнего элемента предыдущей строки.
// Поэтому необходимо находить РАЗНИЦУ АДРЕСОВ (тип данных ptrdiff_t).
// Разница адресов необходима для корректного перемещения указателя. Просто pointer++; делать нельзя (почему - описано выше).
// Для нахождения разницы адресов используется функция address_difference().
// Или использовать линейный массив, к чему я скорее всего и приду :)
// Или не придумывать проблемы и использовать указатели на матрицы

#include <stdio.h>
#include <stddef.h>

typedef struct {
    int rows;
    int cols;
    int data[50][50];  // Вы можете выбрать максимальный размер матрицы
    char* name;
} Matrix;

Matrix matrixA = {3, 3, 
    {{-1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}},
    "Matrix A"
};
Matrix matrixB = {2, 2,
    {{1, 2, 3},
    {4, 5, 6},
    },
    "Matrix B"
};
Matrix matrixResult = {
    0,
    0,
    {{0}},
    "Matrix Result"};

Matrix *p_mtrxA = &matrixA;
Matrix *p_mtrxB = &matrixB;
Matrix *p_mtrxRslt = &matrixResult;


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
        multiplication_by_number(&matrixA, &matrixResult);
        break;
    case 2:
        printf("Вы выбрали %s.\n", task_name_summation);
        summation(&matrixA, &matrixB, &matrixResult);
        break;
    case 3:
        printf("Вы выбрали %s.\n", task_name_subtraction);
        subtraction(&matrixA, &matrixB, &matrixResult);
        break;
    case 4:
        printf("Вы выбрали %s.\n", task_name_multiplication);
        multiplication(&matrixA, &matrixB, &matrixResult); 
        break;
    case 5:
        printf("Вы выбрали %s.\n", task_name_determinant);   
        determinant(&matrixA);
        break;
    default:
        printf("Некорректный ввод.\n");
        exit(0);
    }
    printf("------------\nРезультат:\n");

}

// Наполнение матрицы
void matrix_fill(Matrix matrix){
    for (int i = 0; i < matrix.rows; ++i) {
        for (int j = 0; j < matrix.cols; ++j) {
            matrix.data[i][j] = i * matrix.cols + j + 1;
        }
    }
}

// Вывод матрицы на экран
void matrix_print(Matrix *p_mtrx){
    printf("------%s------\n", p_mtrx->name);
    for (int i = 0; i < p_mtrx->rows; ++i) {
        for (int j = 0; j < p_mtrx->cols; ++j) {
            // Пока матрица выводится будет в таком виде
            printf("address=%p \t value=%d \n", &p_mtrx->data[i][j], p_mtrx->data[i][j]);
        // printf("\n");
        }
    }
}

// Умножение матрицы на число
void multiplication_by_number(Matrix *p_old, Matrix *p_new){
    p_new->rows = p_old->rows;
    p_new->cols = p_old->cols;

    printf("Пожалуйста, введите ЦЕЛОЕ число, на которое нужно умножить матрицу.\n");
    int number;
    scanf("%d", &number);

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
        printf("Размеры матриц не равны.\n");
        exit(0);
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
        printf("Размеры матриц не равны.\n");
        exit(0);
    }
}

// Умножение матриц
void multiplication(Matrix *p_frst, Matrix *p_scnd, Matrix *p_rslt){
    if (p_frst->cols == p_scnd->rows){
        p_rslt->rows = p_frst->rows;
        p_rslt->cols = p_frst->cols;

        for (int i = 0; i < p_rslt->rows; ++i) {
            for (int j = 0; j < p_rslt->cols; ++j) {
                p_rslt->data[i][j] = p_frst->data[i][j] * p_scnd->data[i][j];
            }
        }
    }
    else {
        printf("Количество стобцов первой матрицы не равно количеству строк второй матрицы.\n");
        exit(0);
    }
}

// Нахождение определителя
void determinant(Matrix *p_mtrx){
    int det;
    if (p_mtrx->rows == p_mtrx->cols){
        if (p_mtrx->rows == 2)
            det = p_mtrx->data[0][0] * p_mtrx->data[1][1] - p_mtrx->data[0][1] * p_mtrx->data[1][0];
        else if (p_mtrx->rows == 3){
            // printf("Требует доработки.")
            det = \
            p_mtrx->data[0][0] * p_mtrx->data[1][1] *p_mtrx->data[2][2] +\
            p_mtrx->data[1][0] * p_mtrx->data[2][1] *p_mtrx->data[0][2] +\
            p_mtrx->data[0][1] * p_mtrx->data[1][2] *p_mtrx->data[2][0] -\
            p_mtrx->data[2][0] * p_mtrx->data[1][1] *p_mtrx->data[0][2] -\
            p_mtrx->data[2][1] * p_mtrx->data[1][2] *p_mtrx->data[0][0] -\
            p_mtrx->data[1][0] * p_mtrx->data[0][1] *p_mtrx->data[2][2];
        }
        else
            printf("Матрица больше, чем ожидалось.");
    }
    else
        printf("Матрица не квадратная.");
    printf("Определитель %s равен:%d\n", p_mtrx->name, det);
    exit(0);
}

int main(){
    operation_choosing();    
    matrix_print(&matrixResult);
    return 0;
}