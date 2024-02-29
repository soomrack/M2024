// Описание
// Имеются две матрицы, необходимо производить с ними следующие действия:
// складывать, вычитать, умножать, находить определитель, умножать на число

#include <stdio.h>

struct Matrix {
    int rows;
    int cols;
    int data[100][100];  // Вы можете выбрать максимальный размер матрицы
};

struct Matrix matrix1 = {3, 3, {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
struct Matrix matrix2 = {3, 3, {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};

// Вывод матрицы на экран
void matrix_print(Matrix matrix){
    printf("Матрица:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d\t", matrix[i][j]);}
        printf("\n");}
}

// Наполнение матрицы
void matrix_fill(Matrix matrix){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = i * cols + j + 1;
        }
    }
}

int main(){
    return 0;
}