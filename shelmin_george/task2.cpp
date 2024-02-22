#include <iostream>
#include <stdlib.h>
#include <time.h>


struct Matrix {
    size_t cols;
    size_t rows;
    double *data;
};


void init_matrix(struct Matrix* any_matrix, int cols, int rows) {
    any_matrix->cols = cols;
    any_matrix->rows = rows;

    any_matrix->data = (double*)malloc(any_matrix->cols * any_matrix->rows * sizeof(double));
    if (any_matrix->data == NULL) {
        printf("Не удалось выделить память");
        exit(0);
    }
}


struct Matrix create_empty_matrix_for_simple_calculus(struct Matrix* any_matrix) {
    struct Matrix new_matrix;
    new_matrix.cols = any_matrix->cols;
    new_matrix.rows = any_matrix->rows;
    new_matrix.data = (double*)malloc(new_matrix.cols * new_matrix.rows * sizeof(double));
    return new_matrix;
}


struct Matrix create_empty_matrix_for_mult(struct Matrix* first_matrix, struct Matrix* second_matrix) {
    struct Matrix new_matrix;
    new_matrix.cols = first_matrix->cols;
    new_matrix.rows = second_matrix->rows;
    new_matrix.data = (double*)malloc(new_matrix.cols * new_matrix.rows * sizeof(double));
    return new_matrix;
}


void random_fill_matrix(struct Matrix *any_matrix) {
    if (not(any_matrix->data == NULL)) {
        int current_index = 0;
        for (int col = 0; col < any_matrix->cols; col++) {
            for (int row = 0; row < any_matrix->rows; row++) {
                any_matrix->data[current_index] = double(rand() % 1000000) / 10000;
                current_index += 1;
            }
        }
    }
    else{
        exit(0);
    }
}


void fill_identity_matrix(struct Matrix* any_matrix) {
    if (not(any_matrix->data == NULL)) {
        int current_index = 0;
        for (int col = 0; col < any_matrix->cols; col++) {
            for (int row = 0; row < any_matrix->rows; row++) {
                if (row == col) {
                    any_matrix->data[current_index] = 1.0;
                }
                else {
                    any_matrix->data[current_index] = 0.0;
                }
                current_index += 1;
            }
        }
    }
    else {
        exit(0);
    }
}


void cout_matrix(struct Matrix* any_matrix) {
    int current_index = 0;
    printf("\n");
    for (int col = 0; col < any_matrix->cols; col++) {
        for (int row = 0; row < any_matrix->rows; row++) {
            printf("%f", any_matrix->data[current_index]);
            printf(" ");
            current_index += 1;
        }
        printf("\n");
    }
}


void free_matrix_memory(struct Matrix *any_matrix) {
    free(any_matrix->data);
}


// проверка размерности матриц для сложения и вычитания
bool matrix_size_simple_check(struct Matrix* first_matrix, struct Matrix* second_matrix) {
    bool rows_check = (first_matrix->rows == second_matrix->rows);
    bool cols_check = (first_matrix->cols == second_matrix->cols);
    if (not(rows_check&&cols_check)){
        printf("размеры матриц не совпадают\n");
    }
    return rows_check * cols_check;
}


struct Matrix matrix_summ(struct Matrix* first_matrix, struct Matrix* second_matrix) { 
    if (matrix_size_simple_check(first_matrix, second_matrix)) {
        struct Matrix new_matrix = create_empty_matrix_for_simple_calculus(first_matrix);

        for (int index = 0; index < first_matrix->rows * first_matrix->cols;index++) {
            new_matrix.data[index] = first_matrix->data[index] + second_matrix->data[index];
        }
        return new_matrix;
    }
}


struct Matrix matrix_sub(struct Matrix* first_matrix, struct Matrix* second_matrix) {
    if (matrix_size_simple_check(first_matrix, second_matrix)) {
        struct Matrix new_matrix = create_empty_matrix_for_simple_calculus(first_matrix);

        for (int index = 0; index < first_matrix->rows * first_matrix->cols; index++) {
            new_matrix.data[index] = first_matrix->data[index] - second_matrix->data[index];
        }
        return new_matrix;
    }
}


struct Matrix matrix_scalar_mult(struct Matrix* any_matrix,double scalar) {
    struct Matrix new_matrix = create_empty_matrix_for_simple_calculus(any_matrix);
    for (int index = 0; index < any_matrix->rows * any_matrix->cols; index++) {
        new_matrix.data[index] = any_matrix->data[index] * scalar;
    }
    return new_matrix;
}


// проверка размерности матриц для умножения
bool matrix_size_hard_check(struct Matrix* first_matrix, struct Matrix* second_matrix) {
    bool check = (first_matrix->rows == second_matrix->cols);
    if (not(check)) {
        printf("размеры матриц не совпадают\n");
    }
    return check;
}


struct Matrix matrix_mult(struct Matrix* first_matrix, struct Matrix* second_matrix) {
    if (matrix_size_hard_check(first_matrix, second_matrix)) {
        struct Matrix new_matrix=create_empty_matrix_for_mult(first_matrix, second_matrix);

        for (int current_col = 0; current_col < new_matrix.cols; current_col++) {
            for (int current_row = 0; current_row < new_matrix.rows; current_row++) {
                new_matrix.data[new_matrix.rows * current_col + current_row] = 0;

                for (int summ_index = 0; summ_index < first_matrix->rows; summ_index++) {

                    new_matrix.data[new_matrix.rows * current_col + current_row] += 
                        first_matrix->data[current_col * new_matrix.cols + summ_index] *
                        second_matrix->data[summ_index* new_matrix.rows+ current_row];
                }
            }
        }
        return new_matrix;
    }
}


struct Matrix matrix_power(struct Matrix* any_matrix, const unsigned int power_number) {
        if (any_matrix->cols == any_matrix->rows) {
            struct Matrix new_matrix = create_empty_matrix_for_mult(any_matrix, any_matrix);

            if (power_number == 0) {
                fill_identity_matrix(&new_matrix);
                return new_matrix;
            }
            if (power_number == 1) {
                return *any_matrix;
            }
            if (power_number > 1) {
                new_matrix = *any_matrix;
                for (int current_power = 2; current_power <= power_number; current_power++) {
                    new_matrix = matrix_mult(&new_matrix, any_matrix);
                }
                return new_matrix;
            }
        }
        else {
            printf("размер матрицы не подходит");
        }
}


struct Matrix matrix_exp(struct Matrix* any_matrix) {
    const unsigned int accuracy = 100;

    if (any_matrix->cols == any_matrix->rows) {
        struct Matrix new_matrix = create_empty_matrix_for_mult(any_matrix, any_matrix);
        fill_identity_matrix(&new_matrix);

        struct Matrix submatrix;
        init_matrix(&submatrix, new_matrix.cols,new_matrix.rows);
        submatrix = *any_matrix;

        double factorial = 1;
        for (int index = 1; index < accuracy + 1; index++) {
            factorial /= index;
            submatrix = matrix_power(any_matrix, index);
            submatrix = matrix_scalar_mult(&submatrix, factorial);
            
            new_matrix = matrix_summ(&new_matrix,&submatrix);
        }
        free_matrix_memory(&submatrix);

        return new_matrix;
    }
}


int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "rus");

    struct Matrix matrix_A;
    init_matrix(&matrix_A, 2, 2);
    random_fill_matrix(&matrix_A);
    //cout_matrix(&matrix_A);

    struct Matrix matrix_B;
    init_matrix(&matrix_B, 2, 2);
    random_fill_matrix(&matrix_B);
    //cout_matrix(&matrix_B);

    struct Matrix matrix_C=matrix_mult(&matrix_A, &matrix_B);
    cout_matrix(&matrix_C);
    //fill_identity_matrix(&matrix_C);
    //matrix_C = matrix_scalar_mult(&matrix_C, 10);
    matrix_C = matrix_exp(&matrix_C);
    cout_matrix(&matrix_C);

        
    free_matrix_memory(&matrix_A);
    free_matrix_memory(&matrix_B);
    free_matrix_memory(&matrix_C);
}
