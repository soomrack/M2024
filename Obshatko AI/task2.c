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


// инициализация матрицы, заполненной случайными числами от 0 до 10
void init_matrix(struct Matrix *matrix, size_t cols, size_t rows)
{
    matrix->cols = cols;
    matrix->rows = rows;
    matrix->data = (double*)malloc(cols * rows * sizeof(double));

    if(matrix->data == NULL) {
        printf("Ошибка выделения памяти");
        exit(0);
    }

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            double random_number = round(((float)rand()/RAND_MAX)*(float)(100.0)) / 10;
            matrix->data[index_row * cols + index_col] = random_number;
        }
    }
}


// инициализация единичной матрицы
void init_unit_matrix(struct Matrix *matrix, size_t size)
{
    matrix->cols = size;
    matrix->rows = size;
    matrix->data = (double*)malloc(size * size * sizeof(double));

    if(matrix->data == NULL) {
        printf("Ошибка выделения памяти");
        exit(0);
    }

    for(size_t index_row = 0; index_row < size; index_row++) {
        for(size_t index_col = 0; index_col < size; index_col++) {
            size_t index = index_row * size + index_col;
            if(index_row == index_col) {
                matrix->data[index] = 1;
            } else {
                matrix->data[index] = 0;
            }
        }
    }
}


void free_initial_matrices() {
    free(matrix_A.data);
    free(matrix_B.data);
    free(matrix_C.data);
}


// проверка размерностей матриц для сложения и вычитания
int if_equal_dimensions(struct Matrix *matrix_1, struct Matrix *matrix_2) {
    int check;
    if(matrix_1->cols == matrix_2->cols && matrix_1->rows == matrix_2->rows) {
        check = 1;
    } else {
        check = 0;
    }
    return check;
}


// проверка согласованности матриц для умножения
int if_matching_dimensions(struct Matrix *matrix_1, struct Matrix *matrix_2) {
    int check;
    if(matrix_1->cols == matrix_2->rows) {
        check = 1;
    } else {
        check = 0;
    }
    return check;
}


// проверка равности количества строк и столбцов для возведения в степень
int if_square_matrix(struct Matrix *matrix) {
    int check;
    if(matrix->cols == matrix->rows) {
        check = 1;
    } else {
        check = 0;
    }
    return check;
}


void sum_matrices(struct Matrix *matrix_1, struct Matrix *matrix_2, struct Matrix *result_matrix) {
    int condition = if_equal_dimensions(matrix_1, matrix_2);
    if(!condition) {
        printf("Для выпонлнения операции сложения размерости матриц должны совпадать");
        free_initial_matrices();
        exit(0);
    }

    size_t cols = matrix_1->cols;
    size_t rows = matrix_1->rows;
    init_matrix(result_matrix, cols, rows);

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            size_t index = index_row * cols + index_col;
            result_matrix->data[index] = matrix_1->data[index] + matrix_2->data[index];
        }
    }
}


void substract_matrices(struct Matrix *matrix_1, struct Matrix *matrix_2, struct Matrix *result_matrix) {
    int condition = if_matching_dimensions(matrix_1, matrix_2);
    if(!condition) {
        printf("Для выпонлнения операции вычитания размерости матриц должны совпадать");
        free_initial_matrices();
        exit(0);
    }

    size_t cols = matrix_1->cols;
    size_t rows = matrix_1->rows;
    init_matrix(result_matrix, cols, rows);

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            size_t index = index_row * cols + index_col;
            result_matrix->data[index] = matrix_1->data[index] - matrix_2->data[index];
        }
    }
}


void multiply_matrices(struct Matrix *matrix_1, struct Matrix *matrix_2, struct Matrix *result_matrix) {
    int condition = if_matching_dimensions(matrix_1, matrix_2);
    if(!condition) {
        printf("Для выпонлнения операции умножения размерости матриц должны быть согласованными");
        free_initial_matrices();
        exit(0);
    }

    size_t cols = matrix_2->cols;  // количество столбцов результирующей матрицы
    size_t rows = matrix_1->rows;  // количетсво строк результирующей матрицы
    size_t cols_of_matrix_1 = matrix_1->cols;  // количество столбцов первой исходной матрицы
    size_t operations_per_element = cols_of_matrix_1;  // количество операций умножения на каждый элемент результирующей матрицы
    init_matrix(result_matrix, cols, rows);

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            double current_element = 0;
            for(size_t operation = 0; operation < operations_per_element; operation++) {
                double matrix_1_element = matrix_1->data[index_row * cols_of_matrix_1 + operation];
                double matrix_2_element = matrix_2->data[index_col + cols * operation];
                current_element += matrix_1_element * matrix_2_element;
            }
            size_t index = index_row * cols + index_col;
            result_matrix->data[index] = current_element;
        }
    }
}


void scalar_multiplication(struct Matrix *matrix_1, double multiplier, struct Matrix *result_matrix) {
    size_t cols = matrix_1->cols;
    size_t rows = matrix_1->rows;
    init_matrix(result_matrix, cols, rows);

    for(size_t index_row = 0; index_row < rows; index_row++) {
        for(size_t index_col = 0; index_col < cols; index_col++) {
            size_t index = index_row * cols + index_col;
            result_matrix->data[index] = matrix_1->data[index] * multiplier;
        }
    }
}


void matrix_power(struct Matrix *matrix, int power, struct Matrix *result_matrix) {
    int condition = if_square_matrix(matrix);
    if(!condition) {
        printf("Для выпонлнения операции возведения в степень матрица должна быть квадратной");
        free_initial_matrices();
        exit(0);
    }

    size_t size = matrix->cols;
    init_unit_matrix(result_matrix, size);  // результирующая матрица инициализируется как единичная

    struct Matrix intermediate_matrix;  // промежуточная матрица, необходима, чтобы в ходе вычислений результирующей матрицы не менялась исходная

    for(int iteration = 0; iteration < power; iteration++) {
        multiply_matrices(result_matrix, matrix, &intermediate_matrix);
        *result_matrix = intermediate_matrix;
    }
    free(intermediate_matrix.data);
}


void matrix_exponential(struct Matrix *matrix, struct Matrix *result_matrix) {
    int condition = if_square_matrix(matrix);
    if(!condition) {
        printf("Для выпонлнения операции возведения в степень матрица должна быть квадратной");
        free_initial_matrices();
        exit(0);
    }
    
    size_t size = matrix->cols;
    init_unit_matrix(result_matrix, size);  // результирующая матрица инициализируется как единичная

    struct Matrix intermediate_matrix_1;  // первая промежуточная матрица
    struct Matrix intermediate_matrix_2;  // вторая промежуточная матрица
    struct Matrix intermediate_matrix_3;  // третья промежуточная матрица

    double coefficient = 1.0;
    for(int iteration = 1; iteration < 30; iteration++) {
        matrix_power(matrix, iteration, &intermediate_matrix_1);
        coefficient /= iteration;
        scalar_multiplication(&intermediate_matrix_1, coefficient, &intermediate_matrix_2);
        sum_matrices(result_matrix, &intermediate_matrix_2, &intermediate_matrix_3);
        *result_matrix = intermediate_matrix_3;
        //print_matrix(result_matrix);
    }
    free(intermediate_matrix_1.data);
    free(intermediate_matrix_2.data);
    free(intermediate_matrix_3.data);
}


double det_matrix() {

}


void print_matrix(struct Matrix *matrix) {
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


int main()
{
    init_matrix(&matrix_A, 3, 3);
    init_matrix(&matrix_B, 1, 3);

    //sum_matrices(&matrix_A, &matrix_B, &matrix_C);
    //substract_matrices(&matrix_A, &matrix_B, &matrix_C);
    //multiply_matrices(&matrix_A, &matrix_B, &matrix_C);
    //scalar_multiplication(&matrix_A, 2, &matrix_C);
    //matrix_power(&matrix_A, 1, &matrix_C);
    matrix_exponential(&matrix_A, &matrix_C);

    print_matrix(&matrix_A);
    print_matrix(&matrix_B);
    print_matrix(&matrix_C);

    free_initial_matrices();

    return 0;
}
