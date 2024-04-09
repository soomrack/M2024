#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h> // Библиотека для функции рандома

typedef struct Matrix {  // Структура с параметрами матрицы
    size_t cols; // size_t - тип данных, который может уместить больше всего, целочисленный // столбцы
    size_t rows;  // строки
    double *data;
} Matrix;


const struct Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL };  // Создание нулевой матрицы для выхода при ошибке  


enum LogLevel {  // Уровни логирования, enum - перечисление констант
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

void log_message(enum LogLevel type, char* message) {  // Функция выводящая сообщения с указанием уровня логирования (enum)
    switch (type) {  // оператор множественного выбора
    case DEBUG:
        printf("DEBUG: %s\n", message);
        break;
    case INFO:
        printf("INFO: %s\n", message);
        break;
    case WARNING:
        printf("WARNING: %s\n", message);
        break;
    case ERROR:
        printf("ERROR: %s\n", message);
        break;
    default:
        printf("DEFAULT: %s\n", message);
        break;
    }
}


// Создание матриц и их проверка
Matrix matrix_create(const size_t rows, const size_t cols) {
    if (rows == 0 || cols == 0) {  // Проверка на нуль
        return MATRIX_NULL;
    };

    // Проверка на переполнение (*)
    if (cols >= __SIZE_MAX__ / (sizeof(double) * rows)) {  // Макс число (количество строк или столбцов) делим на количество строк или столбцов, если значение больше, то выходит за рамки памяти (частное меньше единицы)
        log_message(ERROR, "Out of size");  // Логирование с указанием уровня логирования
        return MATRIX_NULL;
    }

    // Выделение памяти для матрицы
    Matrix M;
    M.data = (double*)malloc(rows * cols * sizeof(double));
    if (M.data == NULL) {  // Проверка выделения памяти на нуль
        log_message(ERROR, "Memory not allocated");
        return MATRIX_NULL;
    }
    M.rows = rows;
    M.cols = cols;
    return M;
}


void matrix_print(const Matrix M) {  // Вывод матрицы
    log_message(INFO, "Matrix:");

    if (M.rows == 0 || M.cols == 0) {
        log_message(ERROR, "Matrix is null.\n");
        return;
    }

    for (size_t row = 0; row < M.rows; row++) {
        for (size_t col = 0; col < M.cols; col++) {
            printf("%.2f\t", M.data[row * M.cols + col]);
        }

        printf("\n\n");
    }
}


void matrix_fill_random(Matrix M) {  // Заполнение  матрицы рандомными числами
    srand(time(NULL));
    for (size_t idx = 0; idx < M.rows * M.cols; idx++) {
        M.data[idx] = (double)rand() / RAND_MAX;
    }
}

void matrix_free(Matrix *M) {  // Очистка памяти, при ненадобности матрицы
    M->rows = 0;
    M->cols = 0;
    free(M->data); 
    M = NULL;
  }


// Сумма двух матриц
Matrix matrix_sum(const Matrix A, const Matrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {  // Если количество строк или столбцов у матриц не равны, предупреждение
        log_message(WARNING, "Matrices have different dimensions!");  // Предупреждение
        return MATRIX_NULL;
    }

    Matrix result = matrix_create(A.rows, A.cols);  // Создаем матрицу, тк они одного размера нет разницы
    if (result.data == NULL) { // Если сумма равна нулю, ошибка
        log_message(ERROR, "Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    for (size_t idx = 0; idx < A.rows * A.cols; idx++) {  // Суммирование (Сумма соответствующих элементов)
        result.data[idx] = A.data[idx] + B.data[idx];
    }

    return result;
}


// Разность двух матриц
Matrix matrix_subtract(const Matrix A, const Matrix B) {  // Если количество строк или столбцов у матриц не равны, предупреждение
    if (A.rows != B.rows || A.cols != B.cols) {
        log_message(WARNING, "Matrices have different dimensions!");
        return MATRIX_NULL;
    }

    Matrix result = matrix_create(A.rows, A.cols);
    if (result.data == NULL) {  // Если разность равна нулю
        log_message(ERROR, "Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    for (size_t idx = 0; idx < A.rows * A.cols; idx++) {  // Вычитание
        result.data[idx] = A.data[idx] - B.data[idx];
    }

    return result;
}


// Произведение двух матриц
Matrix matrix_multiply(const Matrix A, const Matrix B) {
    if (A.cols != B.rows) {  // Если количество столбцов матрицы А не равно числу строк матрицы Б, предупреждение
        log_message(WARNING, "Matrices cannot be multiplied due to incompatible dimensions!");
        return MATRIX_NULL;
    }

    Matrix result = matrix_create(A.rows, B.cols); // Создание матрицы А строк, Б столбцов
    if (result.data == NULL) {  // Если результат нулевой
        log_message(ERROR, "Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    for (size_t row = 0; row < A.rows; row++) {  // Перемножение строк на столбцы (*), size_t - размер строк или столбцов
        for (size_t col = 0; col < B.cols; col++) {  
            double sum = 0;  // Временная переменная для сложения
            for (size_t offset = 0; offset < A.cols; offset++) {
                sum += A.data[row * A.cols + offset] * B.data[offset * B.cols + col];
            }
            result.data[row * B.cols + col] = sum;
        }
    }
    return result;
}


// Умножение матрицы на скаляр
Matrix matrix_multiply_by_scalar(const Matrix M, const double scalar) {
    if (M.rows == 0 || M.cols == 0) {  // Если количество строк или столбцов рано 0, ошибка
        log_message(ERROR, "One of dimensions of multiply matrix on scalar has value 0.");
        return MATRIX_NULL;
    };

    Matrix result = matrix_create(M.rows, M.cols);
    if (result.data == NULL) {  // Если результат нулевой
        log_message(ERROR, "Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    for (size_t idx = 0; idx < M.rows * M.cols; idx++) {  // Умножение на скаляр, каждый элемент умножается на скаляр
        result.data[idx] = M.data[idx] * scalar;
    }
    return result;
}


//Создание единичной матрицы
Matrix matrix_identity(const size_t rows, const size_t cols) {
    
    Matrix identity = matrix_create(rows, cols);  // Создание матрицы
    if (identity.data == NULL) {  // Проверка памяти для матрицы на нуль
        log_message(ERROR, "Watchout! Data matrix of 'identity' is NULL!");
        return MATRIX_NULL;
    }
    
    size_t allo_mem_size = rows * cols * sizeof(double);
    memset(identity.data, 0, allo_mem_size);  // Заполнение памяти байтами на указанную длину

    for (size_t idx_diag = 0; idx_diag < rows; idx_diag++) {  // Заполнение главной диагонали единицами //тут
        identity.data[idx_diag * rows + idx_diag] = 1.;
    }

    return identity;
} 


// Экспонента
Matrix matrix_exponential(const Matrix M, int iteration_count) {
    if (M.rows == 0 || M.cols == 0) {  // Проверка на нуль
        log_message(ERROR, "Matrix should be size more then zero!");
        return MATRIX_NULL;
    }

    if (M.rows != M.cols) {  // Проверка на квадратность, тк будет умножаться друг на друга
        log_message(WARNING, "Exponential can only be calculated for sqare matrices!");
        return MATRIX_NULL;
    }


    Matrix result = matrix_identity(result.rows, result.cols);  // Создание матрицы резулт
    if (result.data == NULL) {  // Проверка памяти для матрицы на нуль
        log_message(ERROR, "Watchout! Data matrix of 'result' is NULL!");
        return MATRIX_NULL;
    }

    Matrix temp = matrix_identity(temp.rows, temp.cols);  // Создание матрицы темп
    if (temp.data == NULL) {   // Проверка памяти для матрицы на нуль
        log_message(ERROR, "Watchout! Data matrix of 'temp' is NULL!");
        matrix_free(&result);  // Освобождение данных из-под матрицы резулт
        return MATRIX_NULL;
    }

    // Расчёт экспоненциального ряда
    for (int iteration = 1; iteration <= iteration_count; iteration++) {  // по формуле матричной экспоненты повтор от 1 до n! 
        Matrix copy_ptr = temp;  // Временная переменная, которая еденичная матрица, запоминаем, чтобы потом убрать память
        temp = matrix_multiply(temp, M);  
        matrix_free(&copy_ptr);  // Чистка памяти

        copy_ptr = temp;
        temp = matrix_multiply_by_scalar(temp, 1. / iteration); // Реализация степени матрицы (умножение на еденичную м и деление на факториал)
        matrix_free(&copy_ptr);  // Чистка памяти

        copy_ptr = result;
        result = matrix_sum(result, temp);   
        matrix_free(&copy_ptr);  // Чистка памяти
    }

    matrix_free(&temp);  // Чистка памяти

    return result;
}


// Определитель (по минорам)
double matrix_determinant_of_by_minors(const Matrix M) {
    if (M.rows == 0 || M.cols == 0) {  // Проверка на нуль
        log_message(ERROR, "Matrix should be size more then zero!");
        return NAN;
    }

    if (M.rows != M.cols) {  // Проверка на квадратность
        log_message(WARNING, "Determinant can only be calculated for sqare matrices.");
        return NAN;
    }

    if (M.rows > 10) {  // Слишком долго будет считать если будем брать матрицу больше 10х10
        log_message(WARNING, "Can't be calculated for a matrix of dimensions greater than 10.");
        return NAN;
    }

    size_t size = M.rows;
    if (size == 1) {  // Если матрица 1х1, ее определитель это единственный элемент
        return M.data[0];
    }

    double det = 0;
    Matrix submat = matrix_create(size - 1, size - 1); // Создание квадратной под матрицы, которая меньше предыдущей
    if (submat.data == NULL) {  // Проверка выделения памяти
        log_message(ERROR, "Watchout! Data matrix of 'submat' is NULL!");
        return NAN;
    }

    for (size_t idx = 0; idx < size; idx++) {  //(*)
        size_t sub_row = 0;
        for (size_t row = 1; row < size; row++) { // Проходим по сторокам
            size_t sub_col = 0;
            for (size_t col = 0; col < size; col++) {   // Проходим по столбцам строки
                if (col == idx) {
                    continue;
                }

                submat.data[sub_row * submat.cols + sub_col] = M.data[row * M.cols + col];
                sub_col++;
            }

            sub_row++;
        }

        short sign = (idx % 2 == 0) ? 1 : -1; // знак перед определителем (idx - индекс, остаток от деления на 2 , упрощенное иф элс
        double subdet = matrix_determinant_of_by_minors(submat);  // Рекурсия
        det += sign * M.data[idx] * subdet;
    }

    matrix_free(&submat); // Освобождение памяти

    return det;
}


void tack_exp() {  // Пример работы экспоненты матрицы 2х2
    int size = 2; 
    Matrix matrix = matrix_create(size, size);

    matrix.data[0] = 1; matrix.data[1] = 2;  // Заполнение ячеек матрицы
    matrix.data[2] = 3; matrix.data[3] = 4;

    Matrix expMat = matrix_exponential(matrix, 8);  // 8 = бит

    log_message(INFO, "Exponential of the matrix:");
    matrix_print(expMat);

    matrix_free(&matrix);
    matrix_free(&expMat);
}

int main() {
    tack_exp(); 
    return 0;
}

