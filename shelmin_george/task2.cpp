#include <iostream>
#include <stdlib.h>
#include <time.h>


struct Matrix {
    size_t cols;
    size_t rows;
    double *data;
};

enum Errors {
    NO_ERRORS,
    NOT_INIT, SIZE_ZERO, TOO_BIG, FAILED_INIT,
    FAILID_RELEASE_NO_DATA, FAILID_RELEASE_SIZE_ZERO,
    WRONG_SIZES, NOT_SQUARE,
    WRONG_MINOR_INDEX, MINOR_SIZE_ZERO,
    DET_EQUAL_ZERO, INV_MAT_CALC_ERROR
};

void matrix_error_log(enum Errors code) {
    if (code == NO_ERRORS) {
        printf("\nno errors were found");
        return;
    }

    if (code == NOT_INIT) {
        printf("\nERROR: the matrix data weren't initialised");
        return;
    }
    if (code == SIZE_ZERO) {
        printf("\nERROR: the matrix size is zero or less");
        return;
    }
    if (code == TOO_BIG) {
        printf("\nERROR: the matrix is too big for an initialisation");
        return;
    }
    if (code == FAILED_INIT) {
        printf("\nERROR: failed to initialize the matrix data");
        return;
    }
 
    if (code == FAILID_RELEASE_NO_DATA) {
        printf("\nERROR: failed to release the matrix data - no data were found");
        return;
    }
    if (code == FAILID_RELEASE_SIZE_ZERO) {
        printf("\nERROR: failed to release the matrix data - the data size is zero or less");
        return;
    }

    if (code == WRONG_SIZES) {
        printf("\nERROR: matrix sizes are incomparable for this operation");
        return;
    }
    if (code == NOT_SQUARE) {
        printf("\nERROR: the matrix should be square");
        return;
    }

    if (code == WRONG_MINOR_INDEX) {
        printf("\nERROR: the matrix minor indexing is incomparable for this operation");
        return;
    }
    if (code == MINOR_SIZE_ZERO) {
        printf("\nERROR: the matrix minor size is zero or less");
        return;
    }

    if (code == DET_EQUAL_ZERO) {
        printf("\nWARNING: the matrix determinant is equal to zero");
        return;
    }
    if (code == INV_MAT_CALC_ERROR) {
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

    if ((cols==0) || (rows == 0)) {
        new_matrix = matrix_null();
        matrix_error_log(SIZE_ZERO);
        return new_matrix;
    }
    long double size_check = SIZE_MAX / cols / rows / sizeof(double);
    if (size_check < 1.0) {
        new_matrix = matrix_null();
        matrix_error_log(TOO_BIG);
        return new_matrix;
    }
    new_matrix.cols = cols;
    new_matrix.rows = rows;
    new_matrix.data = (double*)malloc(new_matrix.cols * new_matrix.rows * sizeof(double));

    if (new_matrix.data == NULL) {
        new_matrix = matrix_null();
        matrix_error_log(FAILED_INIT);
        return new_matrix;
    }
    return new_matrix;
}


void matrix_zeros_fill(struct Matrix* any_matrix) {
    memset(any_matrix->data, 0, any_matrix->cols * any_matrix->rows * sizeof(double));
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

    if ((any_matrix.cols == 0) || (any_matrix.rows == 0)) {
        new_matrix.data = NULL;
        matrix_error_log(SIZE_ZERO);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    memcpy(new_matrix.data, any_matrix.data, any_matrix.cols * any_matrix.rows * sizeof(double));
    return new_matrix;
}


void matrix_random_fill(struct Matrix *any_matrix) {
    for (size_t index = 0; index < any_matrix->cols * any_matrix->rows; index++) {
        any_matrix->data[index] = double(rand() % 1000000) / 10000;
    }
}


void matrix_identity_fill(struct Matrix* any_matrix) {
    matrix_zeros_fill(any_matrix);
    for (size_t index = 0; index < any_matrix->rows*any_matrix->cols; index+=any_matrix->rows+1) {
        any_matrix->data[index] = 1.0;
    }
}


void matrix_print(struct Matrix any_matrix) {
    if ((any_matrix.cols == 0) || (any_matrix.rows == 0)) {
        matrix_error_log(SIZE_ZERO);
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
        matrix_error_log(FAILID_RELEASE_NO_DATA);
        return;
    }

    any_matrix->cols = 0;
    any_matrix->rows = 0;
    free(any_matrix->data);  
    any_matrix->data = NULL;
}


struct Matrix matrix_summ(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix;

    if ((first_matrix.rows != second_matrix.rows) || (first_matrix.cols != second_matrix.cols)) {
        new_matrix = matrix_null();
        matrix_error_log(WRONG_SIZES);
        return new_matrix;
    }
    if ((first_matrix.cols == 0) || (second_matrix.rows == 0)) {
        new_matrix = matrix_null();
        matrix_error_log(SIZE_ZERO);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_simple_calculus(first_matrix);
    for (size_t index = 0; index < new_matrix.rows * new_matrix.cols; index++) {
        new_matrix.data[index] = first_matrix.data[index] + second_matrix.data[index];
    }
    return new_matrix;
}


struct Matrix matrix_sub(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix;

    if ((first_matrix.rows != second_matrix.rows) || (first_matrix.cols != second_matrix.cols)) {
        new_matrix = matrix_null();
        matrix_error_log(WRONG_SIZES);
        return new_matrix;
    }
    if ((first_matrix.cols == 0) || (second_matrix.rows == 0)) {
        new_matrix = matrix_null();
        matrix_error_log(SIZE_ZERO);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_simple_calculus(first_matrix);
    for (size_t index = 0; index < new_matrix.rows * new_matrix.cols; index++) {
        new_matrix.data[index] = first_matrix.data[index] - second_matrix.data[index];
    }
    return new_matrix;
}


struct Matrix matrix_scalar_mult(struct Matrix any_matrix,double scalar) {
    struct Matrix new_matrix;

    if ((any_matrix.cols == 0) || (any_matrix.rows == 0)) {
        new_matrix = matrix_null();
        matrix_error_log(SIZE_ZERO);
        return new_matrix;
    }
    new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    for (size_t index = 0; index < new_matrix.rows * new_matrix.cols; index++) {
        new_matrix.data[index] = any_matrix.data[index] * scalar;
    }
    return new_matrix;
}

    
struct Matrix matrix_mult(struct Matrix first_matrix, struct Matrix second_matrix) {
    struct Matrix new_matrix;
    
    if ((first_matrix.data == NULL) || (second_matrix.data == NULL)) {
        new_matrix = matrix_null();
        matrix_error_log(NOT_INIT);
        return new_matrix;
    }
    if (first_matrix.rows != second_matrix.cols) {
        new_matrix = matrix_null();
        matrix_error_log(WRONG_SIZES);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_mult(first_matrix, second_matrix);
 
    for (size_t current_col = 0; current_col < new_matrix.cols; current_col++) {
        for (size_t current_row = 0; current_row < new_matrix.rows; current_row++) {
            
            for (size_t index = 0; index < first_matrix.rows; index++) {  

                new_matrix.data[new_matrix.rows * current_col + current_row] += 
                    first_matrix.data[first_matrix.rows*current_col+index] *
                    second_matrix.data[second_matrix.rows*index+current_row];
            }
        }
    }
    return new_matrix;
}


struct Matrix matrix_exp(struct Matrix any_matrix, const unsigned int accuracy) {
    struct Matrix new_matrix;

    if (any_matrix.data == NULL) {
        new_matrix = matrix_null();
        matrix_error_log(NOT_INIT);
        return new_matrix;
    }
    if (any_matrix.cols != any_matrix.rows) {
        new_matrix = matrix_null();
        matrix_error_log(NOT_SQUARE);
        return new_matrix;
    }

    new_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    matrix_identity_fill(&new_matrix);

    struct Matrix submatrix;
    matrix_init(new_matrix.cols, new_matrix.rows);
    matrix_identity_fill(&submatrix);

    struct Matrix submatrix_shadow=submatrix;
    struct Matrix new_matrix_shadow=new_matrix;

    for (unsigned int index = 1; index < accuracy; index++) {
        if (index >1) {
            matrix_free_memory(&new_matrix_shadow);  // память освобождена, адрес перезаписан
            new_matrix_shadow = new_matrix;
        }
        submatrix = matrix_mult(submatrix, any_matrix);

        matrix_free_memory(&submatrix_shadow);
        submatrix_shadow = submatrix;  // память освобождена, адрес перезаписан

        submatrix = matrix_scalar_mult(submatrix, 1.0/index);

        matrix_free_memory(&submatrix_shadow);
        submatrix_shadow = submatrix;  // память освобождена, адрес перезаписан

        new_matrix = matrix_summ(new_matrix, submatrix);
    }    
    return new_matrix;
}


struct Matrix matrix_create_for_minor(struct Matrix any_matrix, size_t col_number, size_t row_number) {
    struct Matrix minor_matrix;
    
    if (any_matrix.data == NULL) {
        minor_matrix = matrix_null();
        matrix_error_log(NOT_INIT);
        return minor_matrix;
    }
    if ((any_matrix.cols == 0) || (any_matrix.rows == 0)) {
        minor_matrix = matrix_null();
        matrix_error_log(SIZE_ZERO);
        return minor_matrix;
    }
    if (!((col_number < any_matrix.cols) && (row_number < any_matrix.rows))) {
        minor_matrix = matrix_null();
        matrix_error_log(WRONG_MINOR_INDEX);
        return minor_matrix;
    }
    if ((any_matrix.cols-1 == 0) || (any_matrix.rows-1 == 0)) {
        minor_matrix = matrix_null();
        matrix_error_log(MINOR_SIZE_ZERO);
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
        matrix_error_log(NOT_INIT);
        return NAN;
    }
    if (any_matrix.cols != any_matrix.rows) {
        matrix_error_log(NOT_SQUARE);
        return NAN;
    }
    if ((any_matrix.cols == 0) || (any_matrix.rows == 0)) {
        matrix_error_log(SIZE_ZERO);
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
        matrix_error_log(NOT_INIT);
        return new_matrix;
    }
    if ((any_matrix.cols == 0) || (any_matrix.rows == 0)) {
        new_matrix = matrix_null();
        matrix_error_log(SIZE_ZERO);
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
        matrix_error_log(NOT_INIT);
        return inv_matrix;
    }
    if (any_matrix.cols != any_matrix.rows) {
        inv_matrix = matrix_null();
        matrix_error_log(NOT_SQUARE);
        return inv_matrix;
    }
    if ((any_matrix.cols == 0) || (any_matrix.rows == 0)) {
        inv_matrix = matrix_null();
        matrix_error_log(SIZE_ZERO);
        return inv_matrix;
    }

    double det = matrix_determinant(any_matrix);
    if (det == NAN) {
        inv_matrix = matrix_null();
        matrix_error_log(INV_MAT_CALC_ERROR);
        return inv_matrix;
    }

    inv_matrix = matrix_create_empty_for_simple_calculus(any_matrix);
    if (fabs(det) <= 0.001) {
        matrix_zeros_fill(&inv_matrix);
        matrix_error_log(DET_EQUAL_ZERO);
        return inv_matrix;
    }

    void* inv_matrix_addresses[] = { inv_matrix.data,0 };
    struct Matrix inv_matrix_shadow = inv_matrix;

    for (size_t current_col = 0; current_col < inv_matrix.cols; current_col++) {
        for (size_t current_row = 0; current_row < inv_matrix.rows; current_row++) {
            
            struct Matrix matrix_M = matrix_create_for_minor(any_matrix, current_col, current_row);

            inv_matrix.data[current_col * inv_matrix.cols + current_row] =
                int(pow(-1, current_col + current_row)) * matrix_determinant(matrix_M);

            matrix_free_memory(&matrix_M);
        }
    }
    inv_matrix = matrix_scalar_mult(inv_matrix, 1 / det);

    matrix_free_memory(&inv_matrix_shadow);
    inv_matrix_shadow = inv_matrix;

    inv_matrix = matrix_transposition(inv_matrix);
    
    matrix_free_memory(&inv_matrix_shadow);

    return inv_matrix;
}


int main()
{
    srand(time(NULL));
    // setlocale(LC_ALL, "rus");

    struct Matrix matrix_A = matrix_empty_init(5, 5);
    matrix_random_fill(&matrix_A);
    matrix_print(matrix_A);

    struct Matrix matrix_B = matrix_empty_init(5, 5);
    matrix_random_fill(&matrix_B);
    matrix_print(matrix_B);


    struct Matrix matrix_C = matrix_mult(matrix_A, matrix_B);
    //matrix_identity_fill(&matrix_C);
    matrix_print(matrix_C);
    /*
    matrix_C = matrix_exp(matrix_C, 50);
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

