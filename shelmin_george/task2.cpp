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
        for (int col = 0; col < any_matrix->cols; col++) {
            for (int row = 0; row < any_matrix->rows; row++) {
                any_matrix->data[col*any_matrix->rows+row] = double(rand() % 1000000) / 10000;
            }
        }
    }
    else{
        exit(0);
    }
}


void fill_identity_matrix(struct Matrix* any_matrix) {
    if (not(any_matrix->data == NULL)) {
        for (int col = 0; col < any_matrix->cols; col++) {
            for (int row = 0; row < any_matrix->rows; row++) {
                if (row == col) {
                    any_matrix->data[col * any_matrix->rows + row] = 1.0;
                }
                else {
                    any_matrix->data[col * any_matrix->rows + row] = 0.0;
                }
            }
        }
    }
    else {
        exit(0);
    }
}


void cout_matrix(struct Matrix* any_matrix) {
    printf("\n");
    for (int col = 0; col < any_matrix->cols; col++) {
        for (int row = 0; row < any_matrix->rows; row++) {
            printf("%f", any_matrix->data[col * any_matrix->rows + row]);
            printf(" ");
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
    else {
        printf("неверные размеры матриц");
        return *first_matrix;
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
    else {
        printf("неверные размеры матриц");
        return *first_matrix;
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
    else {
        printf("неверные размеры матриц");
        return *first_matrix;
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
            return *any_matrix;
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
    else {
        printf("неверный размер матрицы");
        return *any_matrix;
    }
}


// не нужна тк является частным случаем
/*double matrix_2x2_det(struct Matrix* any_matrix) {
    if ((any_matrix->cols == any_matrix->rows)&&(any_matrix->cols==2)) {
        double deterninant = any_matrix->data[0] * any_matrix->data[3] - 
            any_matrix->data[1] * any_matrix->data[2];
        return deterninant;
    }
}*/


struct Matrix create_matrix_for_minor(struct Matrix* any_matrix, const unsigned int col_number, const unsigned int row_number) {
    if ((col_number < any_matrix->cols) && (row_number < any_matrix->rows)) {
        struct Matrix minor_matrix;
        init_matrix(&minor_matrix, any_matrix->cols - 1, any_matrix->rows - 1);

        int minor_index = 0;
        int origin_index = 0;
        for (int current_col = 0; current_col < minor_matrix.cols; current_col++) {
            if (current_col == col_number) {
                origin_index += any_matrix->cols;
            }
            for (int current_row = 0; current_row < minor_matrix.rows; current_row++) {
                if (current_row == row_number) {
                    origin_index += 1;
                }

                minor_matrix.data[minor_index] = any_matrix->data[origin_index];

                minor_index += 1;
                origin_index += 1;
            }
            if (row_number == minor_matrix.rows) {  // иначе не обрабатывается последний столбец
                origin_index += 1;
            }
        }
        return minor_matrix;
    }
    else {
        printf("размер матрицы не соответствует индексам");
        return *any_matrix;
    }
}


double matrix_determinant(struct Matrix* any_matrix) {
    if ((any_matrix->cols == any_matrix->rows)&&(any_matrix->cols>0)) {
        double determinant = 0;
        if (any_matrix->cols == 1) {
            determinant = any_matrix->data[0];
        }
        else {
            for (int summ_index = 0; summ_index < any_matrix->rows; summ_index++) {
                struct Matrix matrix_M;
                matrix_M = create_matrix_for_minor(any_matrix, 0, summ_index);
                determinant += int(pow(-1, summ_index)) * any_matrix->data[summ_index] * matrix_determinant(&matrix_M);
                free_matrix_memory(&matrix_M);
            }
        }
        return determinant;
    }
    else {
    printf("определитель равен нулю");
    return *any_matrix;
    }
}


struct Matrix matrix_transposition(struct Matrix* any_matrix) {
    struct Matrix new_matrix;
    init_matrix(&new_matrix, any_matrix->rows, any_matrix->cols);

    for (int current_col = 0; current_col < new_matrix.cols; current_col++) {
        for (int current_row = 0; current_row < new_matrix.rows; current_row++) {
            new_matrix.data[new_matrix.rows * current_col + current_row] =
                any_matrix->data[new_matrix.cols*current_row+current_col];            
        }
    }
    return new_matrix;
}


struct Matrix inverse_matrix(struct Matrix* any_matrix) {
    double det = matrix_determinant(any_matrix);
    if (det != 0) {
        struct Matrix inv_matrix = create_empty_matrix_for_simple_calculus(any_matrix);

        for (int current_col = 0; current_col < inv_matrix.cols; current_col++) {
            for (int current_row = 0; current_row < inv_matrix.rows; current_row++) {
                struct Matrix matrix_M = create_matrix_for_minor(any_matrix, current_col, current_row);

                inv_matrix.data[current_col*inv_matrix.cols+current_row]=
                    int(pow(-1, current_col+current_row)) * matrix_determinant(&matrix_M);

                free_matrix_memory(&matrix_M);
            }
        }
        inv_matrix = matrix_transposition(&inv_matrix);
        inv_matrix = matrix_scalar_mult(&inv_matrix, 1 / det);
        return inv_matrix;
    }
    else {
        printf("определитель равен нулю");
        return *any_matrix;
    }
}


int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "rus");

    struct Matrix matrix_A;
    init_matrix(&matrix_A, 4, 4);
    random_fill_matrix(&matrix_A);
    cout_matrix(&matrix_A);

    struct Matrix matrix_B;
    init_matrix(&matrix_B, 4, 4);
    random_fill_matrix(&matrix_B);
    cout_matrix(&matrix_B);


    struct Matrix matrix_C=matrix_mult(&matrix_A, &matrix_B);
    //fill_identity_matrix(&matrix_C);
    cout_matrix(&matrix_C);
    struct Matrix matrix_inv = inverse_matrix(&matrix_C);
    cout_matrix(&matrix_inv);
    matrix_C = matrix_mult(&matrix_inv, &matrix_C);
    cout_matrix(&matrix_C);
    free_matrix_memory(&matrix_inv);
    //printf("\n%f", matrix_determinant(&matrix_C));



        
    free_matrix_memory(&matrix_A);
    free_matrix_memory(&matrix_B);
    free_matrix_memory(&matrix_C);
}

