#include <iostream>
#include <stdlib.h>
#include <time.h>
//функция которая проверяет вывели ли другие функции сообщения о коде ошибке
//в степени и экспоненте память выделяется но не освобождается
//проверить что выделенная память не превосходит size_t(size_max)
//mem_set для заполнения нулей
//return работает даже в void
//записывать в массив адрес каждой выделенной памяти чтобы потом освободить все(мб сделать init не void чтоб без него нельзя было создать матрицу)
// в цикле записывать адреса вплоть до n-1 элемента чтобы освободить весь массив не потеряв результат функции
// или (лучше) функции операций должны не выделять память, а работать с одной и той же уже выделенной памятью
//после очистки указатель приравнять к нулю, вернуть нулевую матрицу
//определитель в случае ошибки пусть возвращает NaN
//нужно сначала освободить массив через memset и только потом через free массив

struct Matrix {
    size_t cols;
    size_t rows;
    double *data;
};



void matrix_init(struct Matrix* any_matrix, size_t cols, size_t rows) {
    if (cols * rows <= 0) {
        return;
    }
    any_matrix->cols = cols;
    any_matrix->rows = rows;

    //проверка максимального размера
    any_matrix->data = (double*)malloc(any_matrix->cols * any_matrix->rows * sizeof(double));
    if (any_matrix->data == NULL) {
        printf("Не удалось выделить память");
    }
}


struct Matrix matrix_create_empty_for_simple_calculus(struct Matrix any_matrix) {
    struct Matrix new_matrix;
    new_matrix.cols = any_matrix.cols;
    new_matrix.rows = any_matrix.rows;
    matrix_init(&new_matrix, new_matrix.cols, new_matrix.rows);
    return new_matrix;
}


struct Matrix matrix_create_empty_for_mult(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix;
    new_matrix.cols = first_matrix.cols;
    new_matrix.rows = second_matrix.rows;
    matrix_init(&new_matrix, new_matrix.cols, new_matrix.rows);
    return new_matrix;
}


void matrix_random_fill(struct Matrix *any_matrix) {
    if (any_matrix->data == NULL) {
        printf("память не выделена");
        return;
    }
    for (size_t index = 0; index < any_matrix->cols * any_matrix->rows; index++) {
        any_matrix->data[index] = double(rand() % 1000000) / 10000;
    }
}


void matrix_zeros_fill(struct Matrix* any_matrix) {
    memset(any_matrix->data, 0.0, any_matrix->cols * any_matrix->rows * sizeof(double));
}


void matrix_identity_fill(struct Matrix* any_matrix) {
    if (any_matrix->data == NULL) {
        printf("память не выделена");
        return;
    }
    memset(any_matrix->data, 0.0, any_matrix->cols * any_matrix->rows * sizeof(double));
    for (size_t col = 0; col < any_matrix->cols; col++) {
        any_matrix->data[col * any_matrix->rows + col] = 1.0;
    }
}


void matrix_print(struct Matrix any_matrix) {
    if (any_matrix.cols * any_matrix.rows <= 0) {
        return;
    }
    printf("\n");
    for (size_t col = 0; col < any_matrix.cols; col++) {
        for (size_t row = 0; row < any_matrix.rows; row++) {
            printf("%f", any_matrix.data[col * any_matrix.rows + row]);
            printf(" ");
        }
        printf("\n");
    }
}


void matrix_free_memory(struct Matrix *any_matrix) {
    memset(any_matrix->data, 0.0, any_matrix->cols * any_matrix->rows * sizeof(double));
    free(any_matrix->data);   
}


struct Matrix matrix_summ(struct Matrix first_matrix, struct Matrix second_matrix) { 
    if ((first_matrix.rows != second_matrix.rows)||(first_matrix.cols != second_matrix.cols)) {
        printf("неверные размеры матриц");
        return first_matrix;
    }
    struct Matrix new_matrix = matrix_create_empty_for_simple_calculus(first_matrix);
    if (new_matrix.cols * new_matrix.rows<=0) {
        printf("подана нулевая матрица");
        return first_matrix;
    }

    for (int index = 0; index < first_matrix.rows * first_matrix.cols; index++) {
        new_matrix.data[index] = first_matrix.data[index] + second_matrix.data[index];
    }
    return new_matrix;
}


struct Matrix matrix_sub(struct Matrix first_matrix, struct Matrix second_matrix) {
    if ((first_matrix.rows != second_matrix.rows) || (first_matrix.cols != second_matrix.cols)) {
        printf("неверные размеры матриц");
        return first_matrix;
    }
    struct Matrix new_matrix = matrix_create_empty_for_simple_calculus(first_matrix);
    if (new_matrix.cols * new_matrix.rows <= 0) {
        printf("подана нулевая матрица");
        return first_matrix;
    }

    for (int index = 0; index < first_matrix.rows * first_matrix.cols; index++) {
        new_matrix.data[index] = first_matrix.data[index] - second_matrix.data[index];
    }
    return new_matrix;
}


struct Matrix matrix_scalar_mult(struct Matrix any_matrix,double scalar) {
    struct Matrix new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    if (new_matrix.cols * new_matrix.rows<=0) {
        printf("подана нулевая матрица");
        return any_matrix;
    }
    for (size_t index = 0; index < any_matrix.rows * any_matrix.cols; index++) {
        new_matrix.data[index] = any_matrix.data[index] * scalar;
    }
    return new_matrix;
}

    
struct Matrix matrix_mult(struct Matrix first_matrix, struct Matrix second_matrix) {
    if (first_matrix.rows != second_matrix.cols) {
        printf("неверные размеры матриц");
        return first_matrix;
    }
    struct Matrix new_matrix = matrix_create_empty_for_mult(first_matrix, second_matrix);
    
    if (new_matrix.cols * new_matrix.rows<=0) {
        printf("подана нулевая матрица");
        return first_matrix;
    }

    for (size_t current_col = 0; current_col < new_matrix.cols; current_col++) {
        for (size_t current_row = 0; current_row < new_matrix.rows; current_row++) {
            new_matrix.data[new_matrix.rows * current_col + current_row] = 0;

            for (size_t summ_index = 0; summ_index < first_matrix.rows; summ_index++) {

                new_matrix.data[new_matrix.rows * current_col + current_row] +=
                    first_matrix.data[current_col * new_matrix.cols + summ_index] *
                    second_matrix.data[summ_index * new_matrix.rows + current_row];
            }
        }
    }
    return new_matrix;
}


// освобождать память
struct Matrix matrix_power(struct Matrix any_matrix, const unsigned int power_number) {
    if (any_matrix.cols != any_matrix.rows) {
        printf("размер матрицы не подходит");
        return any_matrix;
    }
    if (any_matrix.cols * any_matrix.rows<=0) {
        printf("подана нулевая матрица");
        return any_matrix;
    }
    if (power_number == 1) {
        return any_matrix;
    }

    struct Matrix new_matrix = matrix_create_empty_for_mult(any_matrix, any_matrix);
    if (power_number == 0) {
        matrix_identity_fill(&new_matrix);
        return new_matrix;
    }
    if (power_number > 1) {
        new_matrix = any_matrix;


        for (int current_power = 2; current_power <= power_number; current_power++) {
            new_matrix = matrix_mult(new_matrix, any_matrix);
            
        }
        return new_matrix;
    }
}


struct Matrix matrix_exp(struct Matrix any_matrix) {
    if (any_matrix.cols != any_matrix.rows) {
        printf("неверный размер матрицы");
        return any_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        printf("подана нулевая матрица");
        return any_matrix;
    }
    const unsigned int accuracy = 50;

    struct Matrix new_matrix = matrix_create_empty_for_mult(any_matrix, any_matrix);

    matrix_identity_fill(&new_matrix);

    struct Matrix submatrix;
    matrix_init(&submatrix, new_matrix.cols, new_matrix.rows);
    matrix_identity_fill(&submatrix);

    void* submatrix_adresses[accuracy];
    void* new_matrix_adresses[accuracy];
    submatrix_adresses[0] = submatrix.data;
    new_matrix_adresses[0] = new_matrix.data;

    for (int index = 1; index < accuracy; index++) {
        submatrix = matrix_mult(submatrix, any_matrix);
        submatrix = matrix_scalar_mult(submatrix, 1.0/index);

        new_matrix = matrix_summ(new_matrix, submatrix);

        submatrix_adresses[index] = submatrix.data;
        if (index < accuracy - 1) {
            new_matrix_adresses[index] = new_matrix.data;
        }
    }

    for (int index = 0; index <accuracy; index++) {
        memset(submatrix_adresses[index], 0.0, submatrix.cols * submatrix.rows * sizeof(double));
        free(submatrix_adresses[index]);
        if (index != accuracy - 1) {
            memset(new_matrix_adresses[index], 0.0, new_matrix.cols * new_matrix.rows * sizeof(double));
            free(new_matrix_adresses[index]);
        }
    }
    
    return new_matrix;
}


struct Matrix matrix_create_for_minor(struct Matrix any_matrix, size_t col_number, size_t row_number) {
    if (!((col_number < any_matrix.cols) && (row_number < any_matrix.rows))) {
        printf("размер матрицы не соответствует индексам");
        return any_matrix;
    }
    struct Matrix minor_matrix;
    matrix_init(&minor_matrix, any_matrix.cols - 1, any_matrix.rows - 1);
    if (minor_matrix.cols * minor_matrix.rows<=0) {
        printf("подана нулевая матрица");
        return any_matrix;
    }

    size_t origin_index = 0;
    for (size_t current_col = 0; current_col < minor_matrix.cols; current_col++) {
        if (current_col == col_number) {
            origin_index += any_matrix.cols;
        }
        for (size_t current_row = 0; current_row < minor_matrix.rows; current_row++) {
            if (current_row == row_number) {
                origin_index += 1;
            }
            minor_matrix.data[current_col * minor_matrix.rows + current_row] = any_matrix.data[origin_index];
            origin_index += 1;
        }
        if (row_number == minor_matrix.rows) {  // иначе не обрабатывается последний столбец
            origin_index += 1;
        }
    }
    return minor_matrix;
}


double matrix_determinant(struct Matrix any_matrix) {
    if ((any_matrix.cols != any_matrix.rows) || (any_matrix.cols <= 0)) {
        printf("размер матрицы неверный");
        return 0.0;
    }
    double determinant = 0;
    if (any_matrix.cols == 1) {
        determinant = any_matrix.data[0];
        return determinant;
    }
    for (size_t summ_index = 0; summ_index < any_matrix.rows; summ_index++) {
        struct Matrix matrix_M;
        matrix_M = matrix_create_for_minor(any_matrix, 0, summ_index);
        determinant += int(pow(-1, summ_index)) * any_matrix.data[summ_index] * matrix_determinant(matrix_M);
        matrix_free_memory(&matrix_M);
    }
    return determinant;
}


struct Matrix matrix_transposition(struct Matrix any_matrix) {
    struct Matrix new_matrix;
    matrix_init(&new_matrix, any_matrix.rows, any_matrix.cols);
    if (new_matrix.cols* new_matrix.rows <=0) {
        printf("подана нулевая матрица");
        return any_matrix;
    }
    for (size_t current_col = 0; current_col < new_matrix.cols; current_col++) {
        for (size_t current_row = 0; current_row < new_matrix.rows; current_row++) {
            new_matrix.data[new_matrix.rows * current_col + current_row] =
                any_matrix.data[new_matrix.cols*current_row+current_col];            
        }
    }
    return new_matrix;
}


struct Matrix inverse_matrix(struct Matrix any_matrix) {
    double det = matrix_determinant(any_matrix);
    if (det == 0.0) {
        printf("определитель равен нулю");
        return any_matrix;
    }
    if (det == NAN) {
        printf("ошибка вычисления определителя");
        return any_matrix;
    }
    struct Matrix inv_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    if (inv_matrix.cols * inv_matrix.rows<=0) {
        printf("подана нулевая матрица");
        return any_matrix;
    }

    void* inv_matrix_adresses[] = { inv_matrix.data,0 };


    for (size_t current_col = 0; current_col < inv_matrix.cols; current_col++) {
        for (size_t current_row = 0; current_row < inv_matrix.rows; current_row++) {
            
            struct Matrix matrix_M = matrix_create_for_minor(any_matrix, current_col, current_row);

            inv_matrix.data[current_col * inv_matrix.cols + current_row] =
                int(pow(-1, current_col + current_row)) * matrix_determinant(matrix_M);

            matrix_free_memory(&matrix_M);
        }
    }
    inv_matrix = matrix_scalar_mult(inv_matrix, 1 / det);
    inv_matrix_adresses[1] = inv_matrix.data;
    inv_matrix = matrix_transposition(inv_matrix);
    
    memset(inv_matrix_adresses[0], 0.0, inv_matrix.cols * inv_matrix.rows * sizeof(double));
    free(inv_matrix_adresses[0]);
    memset(inv_matrix_adresses[1], 0.0, inv_matrix.cols * inv_matrix.rows * sizeof(double));
    free(inv_matrix_adresses[1]);

    return inv_matrix;
}


int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "rus");

    struct Matrix matrix_A;
    matrix_init(&matrix_A, 4, 4);
    matrix_random_fill(&matrix_A);
    matrix_print(matrix_A);

    struct Matrix matrix_B;
    matrix_init(&matrix_B, 4, 4);
    matrix_random_fill(&matrix_B);
    matrix_print(matrix_B);


    struct Matrix matrix_C=matrix_mult(matrix_A, matrix_B);
    //matrix_identity_fill(&matrix_C);
    matrix_print(matrix_C);

    /*
    matrix_free_memory(&matrix_C);

    matrix_C = matrix_exp(matrix_C);
    matrix_print(matrix_C);
    matrix_free_memory(&matrix_C);
    */
    
    struct Matrix matrix_inv = inverse_matrix(matrix_C);
    matrix_print(matrix_inv);
    //printf("\n%f", matrix_determinant(matrix_C));
    matrix_C = matrix_mult(matrix_inv, matrix_C);
    matrix_print(matrix_C);

    matrix_free_memory(&matrix_C);
    matrix_free_memory(&matrix_inv);
    

    //void* A_coords = matrix_A.data;
    //free(A_coords);
    matrix_free_memory(&matrix_A);
    matrix_free_memory(&matrix_B);

}

