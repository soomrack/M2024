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

#include <stdio.h>
#include <stddef.h>

typedef struct {
    int rows;
    int cols;
    int data[50][50];  // Вы можете выбрать максимальный размер матрицы
} Matrix;

Matrix matrixA = {3, 3, 
    {{1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}}
};
Matrix matrixB = {3, 3,
    {{1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}}
};
Matrix matrixResult;
// Matrix matrixResult = {3, 3, {{0}}};

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
    case 1: // TODO matrixA.data[0][3] универсальность кода
        printf("Вы выбрали %s.\n", task_name_multiplication_by_number);
        multiplication_by_number(&matrixA.data[0][0], &matrixResult.data[0][0], matrixA.rows, matrixA.cols, \
            address_difference(&matrixA.data[0][2], &matrixA.data[1][0]));
        break;
    case 2:
        printf("Вы выбрали %s.\n", task_name_summation);        
        break;
    case 3:
        printf("Вы выбрали %s.\n", task_name_subtraction);        
        break;
    case 4:
        printf("Вы выбрали %s.\n", task_name_multiplication);        
        break;
    case 5:
        printf("Вы выбрали %s.\n", task_name_determinant);        
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

// Вывод копии матрицы на экран
void copy_matrix_print(Matrix matrix){
    int *p;
    for (int i = 0; i < matrix.rows; ++i) {
        for (int j = 0; j < matrix.cols; ++j) {
            p = &matrix.data[i][j];
            printf("address=%p \t value=%d \n", (void*)p, *p);
        // printf("\n");
        }
    }
}

// Вывод оригинальной матрицы
void original_matrix_print(int *mtrx, int rows, int cols){

}

// Нахождение разницы адресов
ptrdiff_t address_difference(int *p_first_arr_element, int *p_second_arr_element){
        printf("ptrdiff_t: %lld\n", p_first_arr_element - p_second_arr_element);
        return p_first_arr_element - p_second_arr_element;
}

// Умножение матрицы на число
void multiplication_by_number(int *pold, int *pnew, int rows, int cols, ptrdiff_t p_addr_dif){
    printf("Пожалуйста, введите ЦЕЛОЕ число, на которое нужно умножить матрицу.\n");
    int number;
    scanf("%d", &number);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            *pnew = *pold * number;
            pnew++;
            pold++;
        }
        printf("pold: address=%p \t value=%d \n", (void*)pold, *pold);
        pold = pold + p_addr_dif;
        printf("pold: address=%p \t value=%d \n", (void*)pold, *pold);
    }
}

int main(){

    operation_choosing();

    int a = 10;
    int b = 23;
    int *pa = &a;
    int *pb = &b;
    ptrdiff_t c = pa - pb;  // разница между адресами
     
    printf("pa=%p \n", (void*)pa);
    printf("pb=%p \n", (void*)pb);
    printf("c=%lld \n", c);

    return 0;
}