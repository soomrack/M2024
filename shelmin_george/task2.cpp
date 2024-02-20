#include <iostream>
#include <stdlib.h>
#include <time.h>


struct matrix {
	size_t cols;
	size_t rows;
	double* data;
};



void init_matrix(struct matrix *any_matrix, int cols, int rows) {
	any_matrix->cols = cols;
	any_matrix->rows = rows;

	any_matrix->data=(double*)malloc(any_matrix->cols * any_matrix->rows * sizeof(double));
	if (any_matrix->data == NULL) {
		printf("Не удалось выделить память");
		exit(0);
	}
}


struct matrix create_empty_matrix_for_calculus(struct matrix* any_matrix) {
	struct matrix new_matrix;
	new_matrix.cols = any_matrix->cols;
	new_matrix.rows = any_matrix->rows;
	new_matrix.data = (double*)malloc(new_matrix.cols * new_matrix.rows * sizeof(double));
	return new_matrix;
}


void random_fill_matrix(struct matrix *any_matrix) {
	int current_index = 0;
	for (int col = 0; col < any_matrix->cols; col++) {
		for (int row = 0; row < any_matrix->rows; row++) {
			any_matrix->data[current_index] = double(rand()%1000000)/1000000;
			current_index+=1;
		}
	}
}


void cout_matrix(struct matrix* any_matrix) {
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


void free_matrix_memory(struct matrix *any_matrix) {
	free(any_matrix->data);
}


// проверка размерности матриц для сложения и вычитания
bool matrix_size_simple_check(struct matrix* first_matrix, struct matrix* second_matrix) {
	bool rows_check = (first_matrix->rows == second_matrix->rows);
	bool cols_check = (first_matrix->cols == second_matrix->cols);
	if (not(rows_check&&cols_check)){
		printf("размеры матриц не совпадают\n");
	}
	return rows_check * cols_check;
}


struct matrix matrix_summ(struct matrix* first_matrix, struct matrix* second_matrix) {
	
	if (matrix_size_simple_check(first_matrix, second_matrix)) {
		struct matrix new_matrix = create_empty_matrix_for_calculus(first_matrix);

		for (int index = 0; index < first_matrix->rows * first_matrix->cols;index++) {
			new_matrix.data[index] = first_matrix->data[index] + second_matrix->data[index];
		}
		return new_matrix;
	}
}


struct matrix matrix_sub(struct matrix* first_matrix, struct matrix* second_matrix) {

	if (matrix_size_simple_check(first_matrix, second_matrix)) {
		struct matrix new_matrix = create_empty_matrix_for_calculus(first_matrix);

		for (int index = 0; index < first_matrix->rows * first_matrix->cols; index++) {
			new_matrix.data[index] = first_matrix->data[index] - second_matrix->data[index];
		}
		return new_matrix;
	}
}


struct matrix matrix_scalar_mult(struct matrix* any_matrix,double scalar) {
	struct matrix new_matrix = create_empty_matrix_for_calculus(any_matrix);
	for (int index = 0; index < any_matrix->rows * any_matrix->cols; index++) {
		new_matrix.data[index] = any_matrix->data[index] * scalar;
	}
	return new_matrix;
}


// проверка размерности матриц для умножения
bool matrix_size_hard_check(struct matrix* first_matrix, struct matrix* second_matrix) {
	bool check = (first_matrix->rows == second_matrix->cols);
	if (not(check)) {
		printf("размеры матриц не совпадают\n");
	}
	return check;
}


int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "rus");

	struct matrix matrix_A;
	init_matrix(&matrix_A, 6, 5);
	random_fill_matrix(&matrix_A);
	cout_matrix(&matrix_A);

	struct matrix matrix_B;
	init_matrix(&matrix_B, 6, 5);
	random_fill_matrix(&matrix_B);
	cout_matrix(&matrix_B);

	struct matrix matrix_C=matrix_summ(&matrix_A, &matrix_B);
	cout_matrix(&matrix_C);
	matrix_C = matrix_scalar_mult(&matrix_C, 2);
	cout_matrix(&matrix_C);


	free_matrix_memory(&matrix_A);
	free_matrix_memory(&matrix_B);
	free_matrix_memory(&matrix_C);
}
