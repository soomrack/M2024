#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    size_t rows;
    size_t cols;
    double *data;
} Matrix;

const Matrix MATRIX_NULL = { .cols = 0, .rows = 0, .data = NULL };

Matrix Matrix_create(size_t rows, size_t cols) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    size_t maxSize = __SIZE_MAX__;
    
    if (rows >= SIZE_MAX / sizeof(double) / cols) {
        printf("Matrix size error.\n");
        return MATRIX_NULL;
    }

    mat.data = (double *)malloc(rows * cols * sizeof(double));

    if (mat.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return MATRIX_NULL;
    }
    return mat;
}

void Matrix_free(Matrix *mat) {
    free(mat->data);
    free(mat->cols);
    free(mat->rows);
}


void Matrix_fill_random(Matrix *mat) {
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            double random_value = (double)(rand() % 100);
            mat->data[i * mat->cols + j] = random_value;
        }
    }
}


unsigned long long factorial(unsigned int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    unsigned long long result = 1;
    for (unsigned int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}


void Matrix_fill_factorial(Matrix *mat) {
    for (size_t i = 0; i < mat->rows; ++i) {
        for (size_t j = 0; j < mat->cols; ++j) {
            unsigned int n = i * mat->cols + j + 1;
            double value = 1.0 / factorial(n);
            mat->data[i* mat->cols + j] = value;
        }
    }
}

void Matrix_print(const Matrix *mat) {
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%.6f\t", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }

}

Matrix Matrix_sum(const Matrix *mat1, const Matrix *mat2) {
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        fprintf(stderr, "Matrix dimensions do not match\n");
        return MATRIX_NULL;
    }

    Matrix result = Matrix_create(mat1->rows, mat1->cols);

    if (result.data == NULL) return MATRIX_NULL;

    for (size_t i = 0; i < mat1->rows*mat1->cols; i++) {
        double sum = mat1->data[i] + mat2->data[i];
            result.data[i] = sum;
    }

    return result;
}

Matrix Matrix_subtract(const Matrix *mat1, const Matrix *mat2) {
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        fprintf(stderr, "Matrix dimensions do not match\n");
        return MATRIX_NULL;
    }

    Matrix result = Matrix_create(mat1->rows, mat1->cols);

    if (result.data == NULL) return MATRIX_NULL;
    
    for (size_t i = 0; i < mat1->rows*mat1->cols; i++) {
        double diff = mat1->data[i] - mat2->data[i];
            result.data[i] = diff;
    }
    return result;
}


Matrix Matrix_multiply(const Matrix *mat1, const Matrix *mat2) {
    if (mat1->cols != mat2->rows) {
        fprintf(stderr, "Cannot multiply matrices: dimensions do not match\n");
        return MATRIX_NULL;
    }

    size_t rows = mat1->rows;
    size_t cols = mat2->cols;
    size_t common_dim = mat1->cols;

    Matrix result = Matrix_create(rows, cols);

    if (result.data == NULL) return MATRIX_NULL;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double sum = 0;
            for (size_t k = 0; k < common_dim; k++) {
                sum += mat1->data[i * mat1->cols + k] * mat2->data[k * mat2->cols + j];
            }
            result.data[i * result.cols + j] = sum;
        }
    }

    return result;
}

Matrix Matrix_scalar_mult(const Matrix *mat, double scalar) {
    Matrix result = Matrix_create(mat->rows, mat->cols);

    if (result.data == NULL) return MATRIX_NULL;

    for (size_t i = 0; i < mat->rows*mat->cols; i++) {
        result.data[i] = mat->data[i] * scalar;
    }

    return result;
}


double Matrix_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        fprintf(stderr, "Matrix must be square to compute determinant\n");
        return 0;
    }
    if (mat->rows == 1) {
        return mat->data[0 * mat->cols + 0];
    } else {
        double det = 0;
        for (size_t i = 0; i < mat->cols; i++) {
            // Получаем минор для каждого элемента первой строки
            Matrix minor = Matrix_create(mat->rows - 1, mat->cols - 1);

            if (minor.data == NULL) return MATRIX_NULL;
            
            size_t minor_row = 0, minor_col = 0;
            for (size_t j = 1; j < mat->rows; j++) {
                for (size_t k = 0; k < mat->cols; k++) {
                    if (k == i) // Пропускаем столбец, содержащий текущий элемент
                        continue;
                    minor.data[minor_row * minor.cols + minor_col] = mat->data[j * mat->cols + k];
                    minor_col++;
                }
                minor_row++;
                minor_col = 0;
            }
            // Рекурсивно вычисляем детерминант минора и суммируем с учетом знака
            det += (i % 2 == 0 ? 1 : -1) * mat->data[0 * mat->cols + i] * Matrix_determinant(&minor);
            Matrix_free(&minor);
        }
        return det;
    }
}

Matrix Matrix_identity(const Matrix *mat){

    Matrix identity = Matrix_create(mat->rows, mat->cols);
    
    if (result.data == NULL) return MATRIX_NULL;
    
        for (size_t i = 0; i < mat->rows; ++i) {
            for (size_t j = 0; j < mat->cols; ++j) {
                identity.data[i * mat->cols + j] = i == j ? 1 : 0;
            }
        }
        return identity;
}



Matrix Matrix_power(const Matrix *mat, unsigned int power) {
    if (mat->rows != mat->cols) {
        fprintf(stderr, "Matrix must be square for matrix power\n");
        return MATRIX_NULL;
    }
    if (power == 0) {
      return Matrix_identity(mat);
    }
    Matrix result = *mat;
    for (unsigned int i = 1; i < power; ++i) {
        result = Matrix_multiply(&result, mat);
    }
    return result;
}


Matrix Matrix_exponential(const Matrix *mat, unsigned int terms) {
    if (mat->rows != mat->cols) {
        fprintf(stderr, "Matrix must be square for matrix exponential\n");
        return MATRIX_NULL;
    }
    Matrix result = Matrix_create(mat->rows, mat->cols);
    Matrix term = Matrix_create(mat->rows, mat->cols);
    for (unsigned int i = 0; i < terms; ++i) {
        term = Matrix_power(mat, i);
        double factor = 1.0 / factorial(i);
        for (size_t j = 0; j < mat->rows; ++j) {
            for (size_t k = 0; k < mat->cols; ++k) {
                double value = term.data[j * term.cols + k] * factor;
                result.data[j * mat->cols + k] = result.data[j * result.cols + k] + value;
            }
        }
        Matrix_free(&term);
    }
    return result;
}


int main() {
    //srand(time(NULL));
    size_t rows = 3;
    size_t cols = 3;
    size_t k = 1;
    Matrix mat1 = Matrix_create(rows, cols);
    Matrix mat2 = Matrix_create(rows, cols);

    Matrix_fill_factorial(&mat1);
    Matrix_fill_factorial(&mat2);


    printf("\nMatrix 1\n");
    Matrix_print(&mat1);
    printf("\nMatrix 2\n");
    Matrix_print(&mat2);
   

    Matrix sum = Matrix_sum(&mat1, &mat2);
    Matrix diff = Matrix_subtract(&mat1, &mat2);
    
    printf("\nSum matrix 1 2\n");
    Matrix_print(&sum);
    printf("\nDiff matrix 1 2\n");
    Matrix_print(&diff);

    Matrix_free(&mat1);
    Matrix_free(&sum);
    Matrix_free(&diff);

    Matrix mat3 = Matrix_create(2, 3);
    Matrix mat4 = Matrix_create(3, 3);

    Matrix_fill_factorial(&mat3);
    Matrix_fill_factorial(&mat4);

    printf("\nMatrix 3\n");
    Matrix_print(&mat3);
    printf("\nMatrix 4\n");
    Matrix_print(&mat4);


    Matrix multi = Matrix_multiply(&mat3,&mat4);
    printf("\nMultiply matrix 3 4\n");
    Matrix_print(&multi);

    Matrix scalar = Matrix_scalar_mult(&mat3, 2.5);
    printf("\nMatrix 3 multi scalar 2,5 \n");
    Matrix_print(&scalar);


    printf("\nDeterminant matrix 2: %.6f\n", Matrix_determinant(&mat2));

    // Вычисление экспоненты матрицы
    Matrix exp_A = Matrix_exponential(&mat2, 10);

    // Вывод экспоненты матрицы в консоль
    printf("\nExponential of matrix A:\n");
    Matrix_print(&exp_A);

    return 0;
}
