#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

struct Matrix {
    size_t cols;
    size_t rows;
    double *data;
};

struct Matrix matrix_A;
struct Matrix matrix_B;
struct Matrix matrix_C;

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
    printf(msg);
    printf("\n");
}


struct Matrix matrix_create(const size_t cols, const size_t rows)
{
    struct Matrix matrix;
    matrix.cols = cols;
    matrix.rows = rows;
    matrix.data = (double*)malloc(cols * rows * sizeof(double));

    if(matrix.data == NULL) {
        matrix_log(ERROR, "Ошибка выделения памяти");
        matrix.cols = 0;
        matrix.rows = 0;
    }
    return matrix;
}


// инициализация матрицы, заполненной случайными числами от 0 до 10
struct Matrix matrix_init(const size_t cols, const size_t rows)
{
    struct Matrix matrix = matrix_create(cols, rows);

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            double random_number = round(((float)rand()/RAND_MAX)*(float)(100.0)) / 10;
            matrix.data[index_row * cols + index_col] = random_number;
        }
    }
    return matrix;
}


// инициализация единичной матрицы
struct Matrix matrix_init_unit(const size_t size)
{
    struct Matrix matrix = matrix_create(size, size);

    for(size_t index_row = 0; index_row < size; index_row++) {
        for(size_t index_col = 0; index_col < size; index_col++) {
            size_t index = index_row * size + index_col;
            if(index_row == index_col) {
                matrix.data[index] = 1;
            } else {
                matrix.data[index] = 0;
            }
        }
    }
    return matrix;
}


void matrix_print(struct Matrix *matrix) 
{
    size_t cols = matrix->cols;
    size_t rows = matrix->rows;

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            printf("%.2lf  ", matrix->data[index_row * cols + index_col]);
        }
        printf("\n");
    }
    printf("\n");
}


// проверка равности размерноти матриц для сложения и вычитания
int if_equal_dimensions(struct Matrix *A, struct Matrix *B) 
{
    int check;
    if(A->cols == B->cols && A->rows == B->rows) {
        check = 1;
    } else {
        check = 0;
    }
    return check;
}


// проверка согласованности размерности матриц для умножения
int if_matching_dimensions(struct Matrix *A, struct Matrix *B) 
{
    int check;
    if(A->cols == B->rows) {
        check = 1;
    } else {
        check = 0;
    }
    return check;
}


// проверка что матрица квадратная для операции возведения в степень
int if_square_matrix(struct Matrix *matrix) 
{
    int check;
    if(matrix->cols == matrix->rows) {
        check = 1;
    } else {
        check = 0;
    }
    return check;
}


struct Matrix matrix_sum(struct Matrix *A, struct Matrix *B) 
{
    int condition = if_equal_dimensions(A, B);
    if(!condition) {
        matrix_log(ERROR, "Для выполнения операции сложения матицы должны иметь одинаковые размерности");
        struct Matrix result_matrix = matrix_init(0, 0);
        return result_matrix;
    }

    size_t cols = A->cols;
    size_t rows = A->rows;
    struct Matrix result_matrix = matrix_init(cols, rows);

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            size_t index = index_row * cols + index_col;
            result_matrix.data[index] = A->data[index] + B->data[index];
        }
    }
    return result_matrix;
}


struct Matrix matrix_substract(struct Matrix *A, struct Matrix *B) 
{
    int condition = if_matching_dimensions(A, B);
    if(!condition) {
        matrix_log(ERROR, "Для выполнения операции вычитания матицы должны иметь одинаковые размерности");
        struct Matrix result_matrix = matrix_init(0, 0);
        return result_matrix;
    }

    size_t cols = A->cols;
    size_t rows = A->rows;
    struct Matrix result_matrix = matrix_init(cols, rows);

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            size_t index = index_row * cols + index_col;
            result_matrix.data[index] = A->data[index] - B->data[index];
        }
    }
    return result_matrix;
}


struct Matrix matrix_multiplication(struct Matrix *A, struct Matrix *B) 
{
    int condition = if_matching_dimensions(A, B);
    if(!condition) {
        matrix_log(ERROR, "Для выполнения операции умножения матрицы должны иметь согласованные размерности");
        struct Matrix result_matrix = matrix_init(0, 0);
        return result_matrix;
    }

    size_t cols = B->cols;  // количество столбцов второй матрицы
    size_t rows = A->rows;  // количество строк второй матрицы
    size_t cols_of_A = A->cols;  // количество столбцов первой матрицы
    size_t operations_per_element = cols_of_A;  // количество операций умножения на каждый элемент результирующей матрицы
    struct Matrix result_matrix = matrix_init(cols, rows);

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            double current_element = 0;
            for(size_t operation = 0; operation < operations_per_element; operation++) {
                double A_element = A->data[index_row * cols_of_A + operation];
                double B_element = B->data[index_col + cols * operation];
                current_element += A_element * B_element;
            }
            size_t index = index_row * cols + index_col;
            result_matrix.data[index] = current_element;
        }
    }
    return result_matrix;
}


struct Matrix matrix_scalar_multiplication(struct Matrix *matrix, double multiplier) 
{
    size_t cols = matrix->cols;
    size_t rows = matrix->rows;
    struct Matrix result_matrix = matrix_init(cols, rows);

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            size_t index = index_row * cols + index_col;
            result_matrix.data[index] = matrix->data[index] * multiplier;
        }
    }
    return result_matrix;
}


struct Matrix matrix_power(struct Matrix *matrix, const int power) 
{
    int condition = if_square_matrix(matrix);
    if(!condition) {
        matrix_log(ERROR, "Для выполнения операции возведения в степень матрица должна быть квадратной");
        struct Matrix result_matrix = matrix_init(0, 0);
        return result_matrix;
    }

    size_t size = matrix->cols;
    struct Matrix result_matrix = matrix_init_unit(size);  // результирующая матрица инициализируется как единичная

    for(int iteration = 0; iteration < power; iteration++) {
        result_matrix = matrix_multiplication(&result_matrix, matrix);
    }
    return result_matrix;
}


struct Matrix matrix_exponential(struct Matrix *matrix) 
{
    int condition = if_square_matrix(matrix);
    if(!condition) {
        matrix_log(ERROR, "Для выполнения операции возведения в степень матрица должна быть квадратной");
        struct Matrix result_matrix = matrix_init(0, 0);
        return result_matrix;
    }
    
    size_t size = matrix->cols;
    struct Matrix result_matrix = matrix_init_unit(size);  // результирующая матрица инициализируется как единичная

    struct Matrix intermediate_matrix;  // промежуточная матрица для последовательного выполнения операций

    double coefficient = 1.0;
    for(int iteration = 1; iteration < 30; iteration++) {
        intermediate_matrix = matrix_power(matrix, iteration);
        coefficient /= iteration;
        intermediate_matrix = matrix_scalar_multiplication(&intermediate_matrix, coefficient);
        result_matrix = matrix_sum(&result_matrix, &intermediate_matrix);
        //matrix_print(result_matrix);
    }
    free(intermediate_matrix.data);
    return result_matrix;
}


int main()
{
    setlocale(LC_CTYPE, "");
    struct Matrix matrix_A = matrix_init(3, 3);
    struct Matrix matrix_B = matrix_init(3, 3);

    struct Matrix matrix_C = matrix_sum(&matrix_A, &matrix_B);
    //struct Matrix matrix_C = matrix_substract(&matrix_A, &matrix_B);
    //struct Matrix matrix_C = matrix_multiplication(&matrix_A, &matrix_B);
    //struct Matrix matrix_C = matrix_scalar_multiplication(&matrix_A, 2);
    //struct Matrix matrix_C = matrix_power(&matrix_A, 2);
    //struct Matrix matrix_C = matrix_exponential(&matrix_A);

    matrix_print(&matrix_A);
    matrix_print(&matrix_B);
    matrix_print(&matrix_C);

    free(matrix_A.data);
    free(matrix_B.data);
    free(matrix_C.data);
    return 0;
}
