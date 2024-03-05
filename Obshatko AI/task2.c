#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

struct Matrix {
    size_t cols;
    size_t rows;
    double *data;
};

enum LogLevel {ERROR, WARNING};


void matrix_log(enum LogLevel level, char* msg) 
{
    switch (level)
    {
    case ERROR:
        printf("ERROR: ");
        break;
    case WARNING:
        printf("WARNING: ");
        break;
    default:
        break;
    }
    printf("%s\n", msg);
}


void matrix_print(struct Matrix matrix) 
{
    size_t cols = matrix.cols;
    size_t rows = matrix.rows;

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            printf("%.2lf  ", matrix.data[index_row * cols + index_col]);
        }
        printf("\n");
    }
    printf("\n");
}


struct Matrix matrix_init(const size_t cols, const size_t rows)
{
    struct Matrix matrix;
    matrix.cols = cols;
    matrix.rows = rows;

    if(SIZE_MAX / cols / rows < sizeof(double)) {
        matrix_log(ERROR, "Матрица слишком большая");
        matrix.cols = 0;
        matrix.rows = 0;
        matrix.data = NULL;
        return matrix;
    }

    matrix.data = (double*)malloc(cols * rows * sizeof(double));

    if(matrix.data == NULL) {
        matrix_log(ERROR, "Ошибка выделения памяти");
        matrix.cols = 0;
        matrix.rows = 0;
    }
    return matrix;
}


struct Matrix matrix_init_zeros(const size_t cols, const size_t rows)
{
    struct Matrix matrix = matrix_init(cols, rows);
    //memset(matrix.data, 0.0, sizeof(double));
    for(size_t index = 0; index < matrix.rows * matrix.cols; index++) {
        matrix.data[index] = 0.0;
    }
    return matrix;
}


// инициализация матрицы, заполненной случайными числами от 0 до max_value
struct Matrix matrix_init_random(const size_t cols, const size_t rows, size_t max_value)
{
    struct Matrix matrix = matrix_init(cols, rows);

    for(size_t index = 0; index < matrix.rows * matrix.cols; index++) {
        double random_number = round(((float)rand()/RAND_MAX)*(float)(max_value * 10)) / 10.0;
        matrix.data[index] = random_number;
    }
    return matrix;
}


// инициализация единичной матрицы
struct Matrix matrix_init_unit(const size_t cols, const size_t rows)
{
    struct Matrix matrix = matrix_init_zeros(cols, rows);

    for(size_t index = 0; index < matrix.rows * matrix.cols; index += matrix.cols + 1) {
        matrix.data[index] = 1.;
    }
    return matrix;
}


void matrix_free(struct Matrix matrix)
{
    matrix.cols = 0;
    matrix.rows = 0;
    free(matrix.data);
    matrix.data = NULL;
}


void matrix_row_add(struct Matrix matrix, size_t result_row, size_t adding_row, double multiplyer)
{
    size_t cols = matrix.cols;
    for(int col = 0; col < cols; col++) {
        matrix.data[result_row * cols + col] += matrix.data[adding_row * cols + col] * multiplyer;
    }
}


void matrix_row_substract(struct Matrix matrix, size_t result_row, size_t substracting_row, double multiplyer)
{
    size_t cols = matrix.cols;
    for(int col = 0; col < cols; col++) {
        matrix.data[result_row * cols + col] -= matrix.data[substracting_row * cols + col] * multiplyer;
    }
}


struct Matrix matrix_sum(struct Matrix A, struct Matrix B) 
{
    // проверка равности размерноти матриц
    if(A.cols != B.cols || A.rows != B.rows) {
        matrix_log(ERROR, "Для выполнения операции вычитания матрицы должны иметь одинаковые размерности");
        struct Matrix result_matrix = {0, 0, NULL};
        return result_matrix;
    }

    struct Matrix result_matrix = matrix_init_zeros(A.cols, A.rows);
    size_t cols = result_matrix.cols;
    size_t rows = result_matrix.rows;

    for(size_t index = 0; index < rows * cols; index++) {
        result_matrix.data[index] = A.data[index] + B.data[index];
    }
    return result_matrix;
}


struct Matrix matrix_substract(struct Matrix A, struct Matrix B) 
{
    // проверка равности размерноти матриц
    if(A.cols != B.cols || A.rows != B.rows) {
        matrix_log(ERROR, "Для выполнения операции вычитания матрицы должны иметь одинаковые размерности");
        struct Matrix result_matrix = {0, 0, NULL};
        return result_matrix;
    }

    struct Matrix result_matrix = matrix_init_zeros(A.cols, A.rows);
    size_t cols = result_matrix.cols;
    size_t rows = result_matrix.rows;

    for(size_t index = 0; index < rows * cols; index++) {
        result_matrix.data[index] = A.data[index] - B.data[index];
    }
    return result_matrix;
}


struct Matrix matrix_multiplication(struct Matrix A, struct Matrix B) 
{
    // проверка согласованности размерности матриц
    if(A.cols != B.rows) {
        matrix_log(ERROR, "Для выполнения операции умножения матрицы должны иметь согласованные размерности");
        struct Matrix result_matrix = {0, 0, NULL};
        return result_matrix;
    }

    struct Matrix result_matrix = matrix_init_zeros(B.cols, A.rows);
    size_t cols = result_matrix.cols;  // количество столбцов второй матрицы
    size_t rows = result_matrix.rows;  // количество строк второй матрицы
    size_t cols_of_A = A.cols;  // количество столбцов первой матрицы
    size_t operations_per_element = cols_of_A;  // количество операций умножения на каждый элемент результирующей матрицы

    for(size_t row_A = 0; row_A < rows; row_A++) {
        for(size_t row_B = 0; row_B < cols; row_B++) {
            double current_element = 0;
            for(size_t operation = 0; operation < operations_per_element; operation++) {
                double A_element = A.data[row_A * cols_of_A + operation];
                double B_element = B.data[row_B + cols * operation];
                current_element += A_element * B_element;
            }
            size_t index = row_A * cols + row_B;
            result_matrix.data[index] = current_element;
        }
    }
    return result_matrix;
}


struct Matrix matrix_scalar_multiplication(struct Matrix matrix, double multiplier) 
{
    struct Matrix result_matrix = matrix_init_zeros(matrix.cols, matrix.rows);
    size_t cols = result_matrix.cols;
    size_t rows = result_matrix.rows;

    for(size_t index = 0; index < rows * cols; index++) {
        result_matrix.data[index] = matrix.data[index] * multiplier;
    }
    return result_matrix;
}


struct Matrix matrix_power(struct Matrix matrix, const int power) 
{
    // проверка что матрица квадратная
    if(matrix.cols != matrix.rows) {
        matrix_log(ERROR, "Для выполнения операции возведения в степень матрица должна быть квадратной");
        struct Matrix result_matrix = {0, 0, NULL};
        return result_matrix;
    }

    struct Matrix result_matrix = matrix_init_unit(matrix.cols, matrix.rows);  // результирующая матрица инициализируется как единичная

    for(int iteration = 0; iteration < power; iteration++) {
        struct Matrix intermediate_matrix = matrix_multiplication(result_matrix, matrix);
        matrix_free(result_matrix);
        result_matrix = intermediate_matrix;
    }
    return result_matrix;
}


struct Matrix matrix_exponential(struct Matrix matrix, size_t total_iterations) 
{
    // проверка что матрица квадратная
    if(matrix.cols != matrix.rows) {
        matrix_log(ERROR, "Для выполнения операции возведения в степень матрица должна быть квадратной");
        struct Matrix result_matrix = {0, 0, NULL};
        return result_matrix;
    }
    
    struct Matrix result_matrix = matrix_init_unit(matrix.cols, matrix.rows);  // результирующая матрица инициализируется как единичная
    struct Matrix prev_iteration_matrix = matrix_init_unit(matrix.cols, matrix.rows);  // матрица для хранения предыдущего слагаемого

    for(int iteration = 1; iteration < total_iterations; iteration++) {
        struct Matrix intermediate_matrix = matrix_multiplication(prev_iteration_matrix, matrix);

        matrix_free(prev_iteration_matrix);
        prev_iteration_matrix = matrix_scalar_multiplication(intermediate_matrix, (1/(float)iteration));

        matrix_free(intermediate_matrix);
        intermediate_matrix = matrix_sum(result_matrix, prev_iteration_matrix);

        matrix_free(result_matrix);
        result_matrix = intermediate_matrix;
    }
    matrix_free(prev_iteration_matrix);
    return result_matrix;
}


double matrix_det(struct Matrix initial_matrix, double accuracy) 
{
    // проверка что матрица квадратная
    if(initial_matrix.cols != initial_matrix.rows) {
        matrix_log(ERROR, "Для вычисления определителя матрица должна быть квадратной");
        return NAN;
    }

    if(initial_matrix.data == NULL) {
        matrix_log(ERROR, "Для вычисления определителя матрица не должна быть пустой");
        return NAN;
    }

    struct Matrix matrix = matrix_init_zeros(initial_matrix.cols, initial_matrix.rows);
    size_t matrix_size = matrix.cols;

    for(size_t index = 0; index < matrix_size * matrix_size; index++) {  // создание копии исходной матрицы
        matrix.data[index] = initial_matrix.data[index];
    }

    double det = 1.;

    for(size_t diagonal_index = 0; diagonal_index < matrix_size; diagonal_index++) {  // прибавление к одним строкам других, чтобы на главной диагонали не было нулей
        if(fabs(matrix.data[(matrix_size + 1) * diagonal_index]) < accuracy) {
            for(size_t row = 0; row < matrix_size; row++) {
                if(matrix.data[diagonal_index + row * matrix_size] != 0) {
                    matrix_row_add(matrix, diagonal_index, row, 1);
                    break;
                }
                if(row == matrix_size - 1) {  // если в столбце все элементы равны 0, то определитель равен 0
                    matrix_free(matrix);
                    return 0;
                }
            }
        }
    }

    for(size_t col = 0; col < matrix_size - 1; col++) {  // преобразование матрицы к треугольнику
        for(size_t row = col + 1; row < matrix_size; row++) {
            double multiplyer = matrix.data[row * matrix_size + col] / matrix.data[col * matrix_size + col];
            matrix_row_substract(matrix, row, col, multiplyer);
        }
    }

    for(size_t index = 0; index < matrix.rows; index++) {  // вычисление определителя
        det *= matrix.data[index * (matrix.rows + 1)];
    }

    if(fabs(det) < accuracy) {
        det = 0.0;
    }

    matrix_free(matrix);
    return det;
}


int main()
{
    setlocale(LC_CTYPE, "");

    struct Matrix matrix_A = matrix_init_random(4, 4, 10);
    struct Matrix matrix_B = matrix_init_unit(6, 3);

    //struct Matrix matrix_C = matrix_sum(matrix_A, matrix_B, matrix_C);
    //struct Matrix matrix_C = matrix_substract(matrix_A, matrix_B);
    //struct Matrix matrix_C = matrix_multiplication(matrix_A, matrix_B);
    //struct Matrix matrix_C = matrix_scalar_multiplication(matrix_A, 2);
    //struct Matrix matrix_C = matrix_power(matrix_A, 3);
    //struct Matrix matrix_C = matrix_exponential(matrix_A, 30);
    double det = matrix_det(matrix_A, 0.00001);

    matrix_print(matrix_A);
    matrix_print(matrix_B);
    //matrix_print(matrix_C);
    printf("%g\n", det);

    matrix_free(matrix_A);
    matrix_free(matrix_B);
    //matrix_free(matrix_C);
    return 0;
}
