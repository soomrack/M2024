#include <iostream>
#include <stdlib.h>
#include <time.h>


struct Matrix {
    size_t cols;
    size_t rows;
    double *data;
};


void error_log(const unsigned int number) {
    switch (number) {
    case 0:
        printf("\nno errors were found");
        return;
    
    case 1:
        printf("\nERROR: the matrix data weren't initialised");
        return;
    case 2:
        printf("\nERROR: the matrix size is zero or less");
        return;
    case 3:
        printf("\nERROR: the matrix is too big for an initialisation");
        return;
    case 4:
        printf("\nERROR: failed to initialize the matrix data");
        return;

    case 5:
        printf("\nERROR: failed to release the matrix data - no data were found");
        return;
    case 6:
        printf("\nERROR: failed to release the matrix data - the data size is zero or less");
        return;

    case 7:
        printf("\nERROR: matrix sizes are incomparable for this operation");
        return;
    case 8:
        printf("\nERROR: the matrix should be square");
        return;

    case 9:
        printf("\nERROR: the matrix minor indexing is incomparable for this operation");
        return;
    case 10:
        printf("\nERROR: the matrix minor size is zero or less");
        return;

    case 11:
        printf("\nERROR: the matrix determinant is equal to zero");
        return;
    case 12:
        printf("\nERROR: the inverse matrix cannot be caclulated");
        return;
    }
}


void matrix_init(struct Matrix* any_matrix, size_t cols, size_t rows) {
    any_matrix->cols = 0;
    any_matrix->rows = 0;
    
    if (cols * rows <= 0) {
        any_matrix->data = NULL;
        error_log(2);
        return;
    }
    long double size_check = SIZE_MAX / (cols * rows * sizeof(double));
    if (size_check < 1.0) {
        any_matrix->data = NULL;     
        error_log(3);
        return;
    }
    any_matrix->cols = cols;
    any_matrix->rows = rows;
    any_matrix->data = (double*)malloc(any_matrix->cols * any_matrix->rows * sizeof(double));

    if (any_matrix->data == NULL) {
        error_log(4);
        return;
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
    for (size_t index = 0; index < any_matrix->cols * any_matrix->rows; index++) {
        any_matrix->data[index] = double(rand() % 1000000) / 10000;
    }
}


void matrix_zeros_fill(struct Matrix* any_matrix) {
    memset(any_matrix->data, 0.0, any_matrix->cols * any_matrix->rows * sizeof(double));
}


void matrix_identity_fill(struct Matrix* any_matrix) {
    if (any_matrix->data == NULL) {
        error_log(1);
        return;
    }
    matrix_zeros_fill(&any_matrix);
    for (size_t col = 0; col < any_matrix->cols; col++) {
        any_matrix->data[col * any_matrix->rows + col] = 1.0;
    }
}


void matrix_print(struct Matrix any_matrix) {
    if (any_matrix.cols * any_matrix.rows <= 0) {
        error_log(2);
        return;
    }
    if (any_matrix.data == NULL) {
        error_log(1);
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
    if (any_matrix->data == NULL) {
        error_log(5);
        return;
    }
    if (any_matrix->cols * any_matrix->rows <= 0) {
        error_log(6);
        return;
    }
    any_matrix->cols = 0;
    any_matrix->rows = 0;
    free(any_matrix->data);   
}


struct Matrix matrix_summ(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix = matrix_create_empty_for_simple_calculus(first_matrix);

    if ((first_matrix.data == NULL) || (second_matrix.data == NULL)) {
        error_log(1);
        return new_matrix;
    }
    if ((first_matrix.rows != second_matrix.rows) || (first_matrix.cols != second_matrix.cols)) {
        error_log(7);
        return new_matrix;
    }

    if (new_matrix.cols * new_matrix.rows<=0) {
        error_log(2);
        return new_matrix;
    }

    for (size_t index = 0; index < first_matrix.rows * first_matrix.cols; index++) {
        new_matrix.data[index] = first_matrix.data[index] + second_matrix.data[index];
    }
    return new_matrix;
}


struct Matrix matrix_sub(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix = matrix_create_empty_for_simple_calculus(first_matrix);

    if ((first_matrix.data == NULL) || (second_matrix.data == NULL)) {
        error_log(1);
        return new_matrix;
    }
    if ((first_matrix.rows != second_matrix.rows) || (first_matrix.cols != second_matrix.cols)) {
        error_log(7);
        return new_matrix;
    }
    if (new_matrix.cols * new_matrix.rows <= 0) {
        error_log(2);
        return new_matrix;
    }

    for (size_t index = 0; index < first_matrix.rows * first_matrix.cols; index++) {
        new_matrix.data[index] = first_matrix.data[index] - second_matrix.data[index];
    }
    return new_matrix;
}


struct Matrix matrix_scalar_mult(struct Matrix any_matrix,double scalar) {
    struct Matrix new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);

    if (any_matrix.data == NULL) {
        error_log(1);
        return new_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        error_log(2);
        return new_matrix;
    }

    for (size_t index = 0; index < any_matrix.rows * any_matrix.cols; index++) {
        new_matrix.data[index] = any_matrix.data[index] * scalar;
    }
    return new_matrix;
}

    
struct Matrix matrix_mult(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix = matrix_create_empty_for_mult(first_matrix, second_matrix);
    
    if ((first_matrix.data == NULL) || (second_matrix.data == NULL)) {
        error_log(1);
        return new_matrix;
    }
    if (first_matrix.rows != second_matrix.cols) {
        error_log(7);
        return new_matrix;
    }
    
    if (new_matrix.cols * new_matrix.rows<=0) {
        error_log(2);
        return new_matrix;
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


struct Matrix matrix_power(struct Matrix any_matrix, const unsigned int power_number) {
    struct Matrix new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    
    if (any_matrix.data == NULL) {
        error_log(1);
        return new_matrix;
    }
    if (any_matrix.cols != any_matrix.rows) {
        error_log(8);
        return new_matrix;
    }
    if (any_matrix.cols * any_matrix.rows<=0) {
        error_log(2);
        return new_matrix;
    }
    
    if (power_number == 1) {
        return any_matrix;
    }
    if (power_number == 0) {
        new_matrix = matrix_create_empty_for_mult(any_matrix, any_matrix);
        matrix_identity_fill(&new_matrix);
        return new_matrix;
    }
    if (power_number > 1) {
        new_matrix = any_matrix;
        void* new_matrix_adress = new_matrix.data;  // адрес памяти записан

        for (int current_power = 2; current_power <= power_number; current_power++) {

            if (current_power > 2) {
                free(new_matrix_adress);
                new_matrix_adress = new_matrix.data;  // память освобождена, адрес перезаписан
            }
            new_matrix = matrix_mult(new_matrix, any_matrix);
            
        }
        return new_matrix;
    }
}


struct Matrix matrix_exp(struct Matrix any_matrix) {
    struct Matrix new_matrix = matrix_create_empty_for_mult(any_matrix, any_matrix);
    if (any_matrix.data == NULL) {
        error_log(1);
        return new_matrix;
    }
    if (any_matrix.cols != any_matrix.rows) {
        error_log(8);
        return new_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        error_log(2);
        return new_matrix;
    }

    const unsigned int accuracy = 50;
    matrix_identity_fill(&new_matrix);

    struct Matrix submatrix;
    matrix_init(&submatrix, new_matrix.cols, new_matrix.rows);
    matrix_identity_fill(&submatrix);

    void* submatrix_adress = submatrix.data;  // адрес памяти записан
    void* new_matrix_adress = new_matrix.data;  // адрес памяти записан

    for (int index = 1; index < accuracy; index++) {
        if (index >1) {
            free(new_matrix_adress);
            new_matrix_adress = new_matrix.data;  // память освобождена, адрес перезаписан
        }
        submatrix = matrix_mult(submatrix, any_matrix);

        free(submatrix_adress);
        submatrix_adress = submatrix.data;  // память освобождена, адрес перезаписан

        submatrix = matrix_scalar_mult(submatrix, 1.0/index);

        free(submatrix_adress);
        submatrix_adress = submatrix.data;  // память освобождена, адрес перезаписан

        new_matrix = matrix_summ(new_matrix, submatrix);
    }    
    return new_matrix;
}


struct Matrix matrix_create_for_minor(struct Matrix any_matrix, size_t col_number, size_t row_number) {
    struct Matrix minor_matrix;
    matrix_init(&minor_matrix, any_matrix.cols - 1, any_matrix.rows - 1);
    
    if (any_matrix.data == NULL) {
        error_log(1);
        return minor_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        error_log(2);
        return minor_matrix;
    }
    if (!((col_number < any_matrix.cols) && (row_number < any_matrix.rows))) {
        error_log(9);
        return minor_matrix;
    }
    if (minor_matrix.cols * minor_matrix.rows<=0) {
        error_log(10);
        return minor_matrix;
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
    if (any_matrix.data == NULL) {
        error_log(1);
        return NAN;
    }
    if (any_matrix.cols != any_matrix.rows) {
        error_log(8);
        return NAN;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        error_log(2);
        return NAN;
    }

    double determinant = 0;
    if (any_matrix.cols == 1) {
        determinant = any_matrix.data[0];
        return determinant;
    }
    struct Matrix matrix_M;
    for (size_t summ_index = 0; summ_index < any_matrix.rows; summ_index++) {
        matrix_M = matrix_create_for_minor(any_matrix, 0, summ_index);
        determinant += int(pow(-1, summ_index)) * any_matrix.data[summ_index] * matrix_determinant(matrix_M);
        matrix_free_memory(&matrix_M);
    }
    return determinant;
}


struct Matrix matrix_transposition(struct Matrix any_matrix) {
    struct Matrix new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    
    if (any_matrix.data == NULL) {
        error_log(1);
        return new_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        error_log(2);
        return new_matrix;
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
    struct Matrix inv_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    if (any_matrix.data == NULL) {
        error_log(1);
        return inv_matrix;
    }
    if (any_matrix.cols != any_matrix.rows) {
        error_log(8);
        return inv_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        error_log(2);
        return inv_matrix;
    }
    double det = matrix_determinant(any_matrix);
    if (det == 0.0) {
        error_log(11);
        return inv_matrix;
    }
    if (det == NAN) {
        error_log(12);
        return inv_matrix;
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
    
    free(inv_matrix_adresses[0]);
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
    

    matrix_free_memory(&matrix_A);
    matrix_free_memory(&matrix_B);


    return 0;
}

