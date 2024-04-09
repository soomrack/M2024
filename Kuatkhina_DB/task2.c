#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h> // Библиотека для функции рандома

typedef struct Matrix {  // Структура с параметрами матрицы
    size_t cols; // size_t - тип данных, который может уместить больше всего, целочисленный // столбцы
    size_t rows;  // строки
    double* data;
} Matrix;


const struct Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL };  // Создание нулевой матрицы для выхода при ошибке  


enum LogLevel {  // Уровни логирования, enum - перечисление констант
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

void log_message(enum LogLevel type, char* message) {  // Функция выводящая сообщения с указанием уровня логирования (enum)
    switch (type) {  // оператор множественного выбора - пример: если type=debug, то принт "debug..."
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

    // Проверка на переполнение (*) - работа с массивом ************************************
    if (cols >= SIZE_MAX / (sizeof(double) * rows)) {  // SIZE_MAX - макс допустимое значение типа size_t ,Макс число (количество строк или столбцов) делим на количество строк или столбцов, если значение больше, то выходит за рамки памяти (частное меньше единицы)
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

    for (size_t row = 0; row < M.rows; row++) {  // for (инициализация перед циклом; условие True цикла; приращение) - для строки
        for (size_t col = 0; col < M.cols; col++) { // для столбца каждого ряда
            printf("%.2f\t", M.data[row * M.cols + col]); // вещественное число с 2мя знаками после точки ******************
        }

        printf("\n\n");
    }
}


void matrix_fill_random(Matrix* M) {  // Заполнение матрицы рандомными числами
    srand(time(NULL)); // в () значение базы, чтоб каждый раз был рандом
    for (size_t idx = 0; idx < M->rows * M->cols; idx++) {
        M->data[idx] = (double)rand() / RAND_MAX;
    }
}

void matrix_free(Matrix* M) {  // Очистка памяти, при ненадобности матрицы указатель 0 
   
    M->rows = 0; // через указатели
    M->cols = 0;
    free(M->data);
    M->data = NULL;
}


// Сумма двух матриц
Matrix matrix_sum(const Matrix A, const Matrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {  // Если количество строк или столбцов у матриц не равны, предупреждение
        log_message(WARNING, "Matrices have different dimensions!");  // Предупреждение
        return MATRIX_NULL;
    }

    Matrix result = matrix_create(A.rows, A.cols);  // Создаем матрицу, тк они одного размера нет разницы - переделать комментарий - нулевая матрица = ничего не сделать 
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

    for (size_t row = 0; row < A.rows; row++) {  // Перемножение строк на столбцы, size_t - размер строк или столбцов
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


// Умножение матрицы на скаляр - проверка выдаёт ошибку
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


//Единичная матрица для экспоненты
Matrix matrix_E_create(const size_t rows, const size_t cols) {

    Matrix matrix_E = matrix_create(rows, cols); // Создание "нулевой" матрицы
    if (matrix_E.data == NULL) {  // Проверка памяти для матрицы на нуль
        log_message(ERROR, " Data matrix of 'matrix_E' is NULL!");
        return MATRIX_NULL;
    }

    memset(matrix_E.data, 0, rows * cols * sizeof(double));  // Заполнение памяти байтами на указанную длину, обнуление
    for (size_t idx_diag = 0; idx_diag < matrix_E.rows; idx_diag++) {  // Заполнение главной диагонали единицами 
        matrix_E.data[idx_diag * matrix_E.rows + idx_diag] = 1.;
    }

    return matrix_E;
}


// Экспонента
Matrix matrix_exponential(const Matrix M, int iteration_count) {
    if (M.rows == 0 || M.cols == 0) {  // Проверка на нуль
        log_message(ERROR, "Matrix should be size more then zero!");
        return M;
    }

    if (M.rows != M.cols) {  // Проверка на квадратность, тк будет умножаться друг на друга
        log_message(WARNING, "Exponential can only be calculated for sqare matrices!");
        return M;
    }

    Matrix result = matrix_E_create(M.rows, M.cols); // Приравнивание к единичной матрице
    Matrix temp = matrix_E_create(M.rows, M.cols);


    // Расчёт экспоненциального ряда
    for (int iteration = 1; iteration <= iteration_count; iteration++) {  // по формуле матричной экспоненты повтор от 1 до n! 
        Matrix copy_ptr = temp;  // Временная перемення (единичная матрица) чтобы убрать память
        temp = matrix_multiply(temp, M);
        matrix_free(&copy_ptr);  // Чистка памяти

        copy_ptr = temp;
        temp = matrix_multiply_by_scalar(temp, 1. / iteration); // Реализация степени матрицы (умножение на единичную м и деление на факториал) !!!!!!!!!!!!!!!
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
        log_message(ERROR, " Data matrix of 'submat' is NULL!");
        return NAN;
    }

    for (size_t idx = 0; idx < size; idx++) {  
        size_t sub_row = 0;
        for (size_t row = 1; row < size; row++) { // По сторокам
            size_t sub_col = 0;
            for (size_t col = 0; col < size; col++) {   // По столбцам строки
                if (col == idx) {
                    continue;
                }

                submat.data[sub_row * submat.cols + sub_col] = M.data[row * M.cols + col];
                sub_col++;
            }

            sub_row++;
        }

        short sign = (idx % 2 == 0) ? 1 : -1; // знак перед определителем (idx - индекс, остаток от деления на 2 , упрощенное if else
        double subdet = matrix_determinant_of_by_minors(submat);  // Рекурсия
        det += sign * M.data[idx] * subdet;
    }

    matrix_free(&submat); // Освобождение памяти

    return det;
}




void task_exp() {  // Пример работы экспоненты матрицы 2х2
    size_t size = 2;
    Matrix matrix = matrix_create(size, size);

    matrix.data[0] = 1; matrix.data[1] = 2;  // Заполнение ячеек матрицы
    matrix.data[2] = 3; matrix.data[3] = 4;

    Matrix expMat = matrix_exponential(matrix, 8);  // 8 = бит

    log_message(INFO, "Exponential of the matrix:");
    matrix_print(expMat);

    matrix_free(&matrix);
    matrix_free(&expMat);
}

void matrix_determinant_by_classic() {  // Вычисление определителя матрицы 3х3 методом треугольника 
    size_t size = 3;
    Matrix matrix = matrix_create(size, size);
    matrix_fill_random(&matrix);
    

    if (matrix.rows == 0 || matrix.cols == 0) {  // Проверка на нуль
        log_message(ERROR, "Matrix should be size more then zero!");
        return NAN;
    }


 
        double Matrix_determinant = matrix.data[0] * matrix.data[4] * matrix.data[8] + matrix.data[1] * matrix.data[5] * matrix.data[6]
        + matrix.data[3] * matrix.data[7] * matrix.data[2] - matrix.data[6] * matrix.data[4] * matrix.data[2]
        - matrix.data[3] * matrix.data[1] * matrix.data[8] - matrix.data[7] * matrix.data[5] * matrix.data[0];

    log_message(INFO, "3*3 Rand Matrix:");
    matrix_print(matrix);

    log_message(INFO, "Matrix determinant classic:");
    printf("%f\n", Matrix_determinant);
    matrix_free(&matrix);

}


int main() {
    matrix_determinant_by_classic();

    Matrix M = matrix_create(3, 3);

    matrix_fill_random(&M);
    matrix_print(M);

    Matrix M_E = matrix_exponential(M, 7);
    matrix_print(M_E);

    return 0;

}
