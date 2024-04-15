#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef double MatrixItem;

struct Matrix {
    size_t cols;
    size_t rows;
    MatrixItem* data;
};

const struct Matrix MATRIX_NULL = { 0, 0, NULL };

void matrix_error_message()
{
    printf("Error\n");
}


struct Matrix matrix_init(const size_t cols, const size_t rows)
{
    Matrix A;
    A.cols = cols;
    A.rows = rows;

    if (cols == 0 || rows == 0) {
    	return MATRIX_NULL;
    }

    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols)
    	return MATRIX_NULL;

    A.data = (MatrixItem*)malloc(A.cols * A.rows * sizeof(MatrixItem));

    if (A.data == NULL) {
    	return MATRIX_NULL;
    }
    return A;
}


struct Matrix matrix_make_ident(size_t rows, size_t cols)
{
    struct Matrix I = matrix_init(rows, cols);
    if (I.data == NULL) {
    	return MATRIX_NULL;
    }
    for (size_t idx = 0; idx < rows * cols; idx++) {
	    if (idx % (rows + 1) == 0) {
	    	I.data[idx] = 1;
	    }
	    else {
		    I.data[idx] = 0;
	    }
    }
    return I;
}

void matrix_free(struct Matrix* matrix)
{
    if (matrix->data == NULL)
    	return;

    free(matrix->data);
    *matrix = MATRIX_NULL;
    return;
}

void matrix_fill(Matrix* A)
{
    for (size_t idx = 0; idx < A->cols * A->rows; idx++) {
    	A->data[idx] = ((int)rand() % 10);
    }
}


void matrix_print(const struct Matrix A)
{
    printf("_____________________\n");
    for (size_t row = 0; row < A.rows; ++row) {
    	for (size_t col = 0; col < A.cols; ++col) {
    		printf("%4.2f \t", A.data[A.cols * row + col]);
		}
    	printf("\n");
	}
    printf("\n");
    return;
}


// A+B=C
struct Matrix matrix_sum(const struct Matrix A, const struct Matrix B)
{
    if (A.cols != B.cols || A.rows != B.rows)
    	return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
    	return C;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
		C.data[idx] = A.data[idx] + B.data[idx];
    return C;
}

//C=A-B
struct Matrix matrix_sub(const struct Matrix A, const struct Matrix B)
{
    if (A.cols != B.cols || A.rows != B.rows)
    	return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
    	return C;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
    	C.data[idx] = A.data[idx] - B.data[idx];
    return C;
}


// coeff * A
void matrix_mult_coeff(struct Matrix A, const double coeff)
{
    if (A.data == NULL)
    	return;

    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
    	A.data[idx] = A.data[idx] * coeff;

    return;
}


// C=A*B
struct Matrix matrix_mult(const struct Matrix A, const struct Matrix B)
{
    if (A.cols != B.rows)
    	return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
    {
		return MATRIX_NULL;
    }
    for (size_t rowA = 0; rowA < A.rows; ++rowA) {
    	for (size_t colB = 0; colB < B.cols; ++colB) {
    		C.data[rowA * (A.cols) + colB] = 0;
    		for (size_t idx = 0; idx < A.cols; ++idx) {
    			C.data[rowA * (A.cols) + colB] += (A.data[(rowA * A.cols) + idx]) * (B.data[(idx * B.cols) + colB]);
    		}
    	}
    }
	return C;
}


//A^T
struct Matrix matrix_transp(struct Matrix* A)
{
    struct Matrix C = matrix_init(A->cols, A->rows);

    if (C.data == NULL) {
		return MATRIX_NULL;
    }

    for (size_t rowA = 0; rowA < A->rows; ++rowA) {
	for (size_t colA = 0; colA < A->cols; ++colA) {
			C.data[(A->rows) * colA + rowA] = A->data[colA + rowA * A->cols];
		}
    }
    return C;
}


//determinant
double matrix_det(struct Matrix* A)
{
    if (A->cols != A->rows)
    	return NAN;

    struct Matrix C = matrix_init(A->cols, A->rows);
    if (C.data == NULL)
		return NAN;
    //matrix_copy(C, A);//

    if (A->cols == 1)
    {
    	matrix_free(&C);
    	return A->data[0];
    }

    if (A->cols == 2)
    {
    	double matr_det = (A->data[0]) * (A->data[3]) - (A->data[1]) * (A->data[2]);
    	return matr_det;
    }

    if (A->cols == 3)
    {
    	double matr_det = (A->data[0]) * (A->data[4]) * (A->data[8]) + (A->data[1]) * (A->data[5]) * (A->data[6]) + (A->data[3]) * (A->data[7]) * (A->data[2]);
    	matr_det -= ((A->data[2]) * (A->data[4]) * (A->data[6]) + (A->data[1]) * (A->data[3]) * (A->data[8]) + (A->data[0]) * (A->data[5]) * (A->data[7]));
    	return matr_det;
    }
    matrix_error_message();
    return NAN;
}


//e^A
struct Matrix matrix_exp(struct Matrix *A, const size_t accuracy)
{
    if (A->cols != A->rows) {
    	matrix_error_message();
    	return MATRIX_NULL;
    }

    struct Matrix E = matrix_init(A->rows, A->cols);

    if (E.data == NULL) {
    	return MATRIX_NULL;
    }
    struct Matrix matrix_transfer;

    for (size_t deg_acc = 1; deg_acc <= accuracy; ++deg_acc) {
    	matrix_transfer = sum_for_e(deg_acc, *A);
    	struct Matrix buf1 = E;
    	E = matrix_sum(buf1, matrix_transfer);
    	matrix_free(&buf1);
    	matrix_free(&matrix_transfer);
    }
    return E;
}


int main()
{
    struct Matrix A, B, C;

    double det;

    A = matrix_init(3, 3);
    matrix_fill(&A);
    matrix_print(A);

    B = matrix_init(3, 3);
    matrix_fill(&B);
    matrix_print(B);

    matrix_sum(B, A);
    matrix_print(B);

    C = matrix_mult(A, B);
    matrix_print(C);

    C = matrix_transp(&C);
    matrix_print(C);

    det = matrix_det(&C);
    printf("%f \n", det);

    A = matrix_exp(&A, 3);
    matrix_print(A);
	
    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);

    return 0;
}
