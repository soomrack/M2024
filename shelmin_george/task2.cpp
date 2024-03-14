#include <iostream>
#include <stdlib.h>
#include <time.h>


struct Matrix {
    size_t cols;
    size_t rows;
    double *data;
};

enum Errors {
    no_errors,
    not_init, size_zero, too_big, failed_init,
    failed_release_no_data, failed_release_size_zero,
    wrong_sizes, not_square,
    wrong_minor_index, minor_size_zero,
    det_equal_zero, inv_mat_cacl_error
};

void error_log(enum Errors code) {
    if (code == no_errors) {
        printf("\nno errors were found");
        return;
    }

    if (code == not_init) {
        printf("\nERROR: the matrix data weren't initialised");
        return;
    }
    if (code == size_zero) {
        printf("\nERROR: the matrix size is zero or less");
        return;
    }
    if (code == too_big) {
        printf("\nERROR: the matrix is too big for an initialisation");
        return;
    }
    if (code == failed_init) {
        printf("\nERROR: failed to initialize the matrix data");
        return;
    }
 
    if (code == failed_release_no_data) {
        printf("\nERROR: failed to release the matrix data - no data were found");
        return;
    }
    if (code == failed_release_size_zero) {
        printf("\nERROR: failed to release the matrix data - the data size is zero or less");
        return;
    }

    if (code == wrong_sizes) {
        printf("\nERROR: matrix sizes are incomparable for this operation");
        return;
    }
    if (code == not_square) {
        printf("\nERROR: the matrix should be square");
        return;
    }

    if (code == wrong_minor_index) {
        printf("\nERROR: the matrix minor indexing is incomparable for this operation");
        return;
    }
    if (code == minor_size_zero) {
        printf("\nERROR: the matrix minor size is zero or less");
        return;
    }

    if (code == det_equal_zero) {
        printf("\nWARNING: the matrix determinant is equal to zero");
        return;
    }
    if (code == inv_mat_cacl_error) {
        printf("\nERROR: the inverse matrix cannot be caclulated");
        return;
    }
}


struct Matrix matrix_null() {
    struct Matrix null_matrix;
    null_matrix.cols = 0;
    null_matrix.rows = 0;
    null_matrix.data = NULL;
    return null_matrix;
}


struct Matrix matrix_init(size_t cols, size_t rows) {
    struct Matrix new_matrix;

    if (cols * rows <= 0) {
        new_matrix = matrix_null();
        error_log(size_zero);
        return new_matrix;
    }
    long double size_check = SIZE_MAX / (cols * rows * sizeof(double));
    if (size_check < 1.0) {
        new_matrix = matrix_null();
        error_log(too_big);
        return new_matrix;
    }
    new_matrix.cols = cols;
    new_matrix.rows = rows;
    new_matrix.data = (double*)malloc(new_matrix.cols * new_matrix.rows * sizeof(double));

    if (new_matrix.data == NULL) {
        new_matrix = matrix_null();
        error_log(failed_init);
        return new_matrix;
    }
    return new_matrix;
}


void matrix_zeros_fill(struct Matrix* any_matrix) {
    memset(any_matrix->data, 0.0, any_matrix->cols * any_matrix->rows * sizeof(double));
}


struct Matrix matrix_empty_init(size_t cols, size_t rows) {
    struct Matrix new_matrix = matrix_init(cols, rows);
    matrix_zeros_fill(&new_matrix);
    return new_matrix;
}


struct Matrix matrix_create_empty_for_simple_calculus(struct Matrix any_matrix) {
    struct Matrix new_matrix = matrix_empty_init(any_matrix.cols, any_matrix.rows);
    return new_matrix;
}


struct Matrix matrix_create_empty_for_mult(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix = matrix_empty_init(first_matrix.cols, second_matrix.rows);
    return new_matrix;
}


struct Matrix matrix_copy(struct Matrix any_matrix) {
    struct Matrix new_matrix;

    if (any_matrix.cols * any_matrix.rows <= 0) {
        new_matrix.data = NULL;
        error_log(size_zero);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    for (size_t index = 0; index < any_matrix.cols * any_matrix.rows; index++) {
        new_matrix.data[index] = any_matrix.data[index];
    }
    return new_matrix;
}


void matrix_random_fill(struct Matrix *any_matrix) {
    for (size_t index = 0; index < any_matrix->cols * any_matrix->rows; index++) {
        any_matrix->data[index] = double(rand() % 1000000) / 10000;
    }
}


void matrix_identity_fill(struct Matrix* any_matrix) {
    matrix_zeros_fill(any_matrix);
    if (any_matrix->cols < any_matrix->rows) {
        for (size_t col = 0; col < any_matrix->cols; col++) {
            any_matrix->data[col * any_matrix->rows + col] = 1.0;
        }
    }
    else {
        for (size_t row = 0; row < any_matrix->rows; row++) {
            any_matrix->data[row * any_matrix->cols + row] = 1.0;
        }
    }
}


void matrix_print(struct Matrix any_matrix) {
    if (any_matrix.cols * any_matrix.rows <= 0) {
        error_log(size_zero);
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
        error_log(failed_release_no_data);
        return;
    }
    if (any_matrix->cols * any_matrix->rows <= 0) {
        error_log(failed_release_size_zero);
        return;
    }
    any_matrix->cols = 0;
    any_matrix->rows = 0;
    free(any_matrix->data);   
}


struct Matrix matrix_summ(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix;

    if ((first_matrix.rows != second_matrix.rows) || (first_matrix.cols != second_matrix.cols)) {
        new_matrix = matrix_null();
        error_log(wrong_sizes);
        return new_matrix;
    }
    if (first_matrix.cols * second_matrix.rows<=0) {
        new_matrix = matrix_null();
        error_log(size_zero);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_simple_calculus(first_matrix);
    for (size_t index = 0; index < first_matrix.rows * first_matrix.cols; index++) {
        new_matrix.data[index] = first_matrix.data[index] + second_matrix.data[index];
    }
    return new_matrix;
}


struct Matrix matrix_sub(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix;

    if ((first_matrix.rows != second_matrix.rows) || (first_matrix.cols != second_matrix.cols)) {
        new_matrix = matrix_null();
        error_log(wrong_sizes);
        return new_matrix;
    }
    if (first_matrix.cols * first_matrix.rows <= 0) {
        new_matrix = matrix_null();
        error_log(size_zero);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_simple_calculus(first_matrix);
    for (size_t index = 0; index < first_matrix.rows * first_matrix.cols; index++) {
        new_matrix.data[index] = first_matrix.data[index] - second_matrix.data[index];
    }
    return new_matrix;
}


struct Matrix matrix_scalar_mult(struct Matrix any_matrix,double scalar) {
    struct Matrix new_matrix;

    if (any_matrix.cols * any_matrix.rows <= 0) {
        new_matrix = matrix_null();
        error_log(size_zero);
        return new_matrix;
    }
    new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    for (size_t index = 0; index < any_matrix.rows * any_matrix.cols; index++) {
        new_matrix.data[index] = any_matrix.data[index] * scalar;
    }
    return new_matrix;
}

    
struct Matrix matrix_mult(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix;
    
    if ((first_matrix.data == NULL) || (second_matrix.data == NULL)) {
        new_matrix = matrix_null();
        error_log(not_init);
        return new_matrix;
    }
    if (first_matrix.rows != second_matrix.cols) {
        new_matrix = matrix_null();
        error_log(wrong_sizes);
        return new_matrix;
    }
    if (first_matrix.cols * first_matrix.rows<=0) {
        new_matrix = matrix_null();
        error_log(size_zero);
        return new_matrix;
    }
    if (second_matrix.cols * second_matrix.rows <= 0) {
        new_matrix = matrix_null();
        error_log(size_zero);
        return new_matrix;
    }

    new_matrix = matrix_init(first_matrix.cols, second_matrix.rows);

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


struct Matrix matrix_exp(struct Matrix any_matrix) {
    struct Matrix new_matrix;

    if (any_matrix.data == NULL) {
        new_matrix = matrix_null();
        error_log(not_init);
        return new_matrix;
    }
    if (any_matrix.cols != any_matrix.rows) {
        new_matrix = matrix_null();
        error_log(not_square);
        return new_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        new_matrix = matrix_null();
        error_log(size_zero);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    const unsigned int accuracy = 50;
    matrix_identity_fill(&new_matrix);

    struct Matrix submatrix;
    matrix_init(new_matrix.cols, new_matrix.rows);
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
    
    if (any_matrix.data == NULL) {
        minor_matrix = matrix_null();
        error_log(not_init);
        return minor_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        minor_matrix = matrix_null();
        error_log(size_zero);
        return minor_matrix;
    }
    if (!((col_number < any_matrix.cols) && (row_number < any_matrix.rows))) {
        minor_matrix = matrix_null();
        error_log(wrong_minor_index);
        return minor_matrix;
    }
    if ((any_matrix.cols-1) * (any_matrix.rows - 1 )<=0) {
        minor_matrix = matrix_null();
        error_log(minor_size_zero);
        return minor_matrix;
    }

    minor_matrix = matrix_init(any_matrix.cols - 1, any_matrix.rows - 1);
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
        error_log(not_init);
        return NAN;
    }
    if (any_matrix.cols != any_matrix.rows) {
        error_log(not_square);
        return NAN;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        error_log(size_zero);
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
    struct Matrix new_matrix;
    
    if (any_matrix.data == NULL) {
        new_matrix = matrix_null();
        error_log(not_init);
        return new_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        new_matrix = matrix_null();
        error_log(size_zero);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    for (size_t current_col = 0; current_col < new_matrix.cols; current_col++) {
        for (size_t current_row = 0; current_row < new_matrix.rows; current_row++) {
            new_matrix.data[new_matrix.rows * current_col + current_row] =
                any_matrix.data[new_matrix.cols*current_row+current_col];            
        }
    }
    return new_matrix;
}


struct Matrix inverse_matrix(struct Matrix any_matrix) {
    struct Matrix inv_matrix;

    if (any_matrix.data == NULL) {
        inv_matrix = matrix_null();
        error_log(not_init);
        return inv_matrix;
    }
    if (any_matrix.cols != any_matrix.rows) {
        inv_matrix = matrix_null();
        error_log(not_square);
        return inv_matrix;
    }
    if (any_matrix.cols * any_matrix.rows <= 0) {
        inv_matrix = matrix_null();
        error_log(size_zero);
        return inv_matrix;
    }

    double det = matrix_determinant(any_matrix);
    if (det == NAN) {
        inv_matrix = matrix_null();
        error_log(inv_mat_cacl_error);
        return inv_matrix;
    }

    inv_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    if (fabs(det) <= 0.001) {
        matrix_zeros_fill(&inv_matrix);
        error_log(det_equal_zero);
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
    // setlocale(LC_ALL, "rus");

    struct Matrix matrix_A = matrix_empty_init(6, 5);
    matrix_random_fill(&matrix_A);
    matrix_print(matrix_A);

    struct Matrix matrix_B = matrix_empty_init(5, 6);
    matrix_random_fill(&matrix_B);
    matrix_print(matrix_B);


    struct Matrix matrix_C = matrix_mult(matrix_A, matrix_B);
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


}

