#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef double matrix_item; // Тип элемента матрицы

struct matrix {
    size_t cols; // Количество столбцов
    size_t rows; // Количество строк
    matrix_item* data; // Указатель на данные матрицы
};

const struct matrix MATRIX_NULL = { 0, 0, NULL }; // Пустая матрица

void matrix_error_message() {
    printf("Error! Check your actions!\n"); // Сообщение об ошибке
}

// Инициализация матрицы заданных размеров
struct matrix matrix_init(const size_t cols, const size_t rows) {
    struct matrix a;
    a.cols = cols;
    a.rows = rows;

    // Проверка на нулевые размеры матрицы
    if (cols == 0 || rows == 0) {
        return MATRIX_NULL;
    }

    // Проверка на переполнение памяти при выделении памяти для хранения данных матрицы
    if (rows >= SIZE_MAX / sizeof(matrix_item) / cols)
        return MATRIX_NULL;

    // Выделение памяти под данные матрицы
    a.data = (matrix_item*)malloc(a.cols * a.rows * sizeof(matrix_item));

    // Проверка успешности выделения памяти
    if (a.data == NULL) {
        return MATRIX_NULL;
    }
    return a;
}

// Создание единичной матрицы заданных размеров
struct matrix matrix_make_ident(size_t rows, size_t cols) {
    struct matrix I = matrix_init(rows, cols); // Создание матрицы заданных размеров
    if (I.data == NULL) {
        return MATRIX_NULL; // Возвращение пустой матрицы в случае неудачи
    }

    // Заполнение элементов диагонали единицами, остальных элементов - нулями
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

// Освобождение памяти, выделенной под матрицу
void matrix_free(struct matrix* matrix) {
    if (matrix->data == NULL)
        return;

    free(matrix->data);
    *matrix = MATRIX_NULL;
    return;
}

// Заполнение матрицы случайными значениями
void matrix_fill(struct matrix* A) {
    for (size_t idx = 0; idx < A->cols * A->rows; idx++) {
        A->data[idx] = ((int)rand() % 10);
    }
}

// Вывод матрицы на экран
void matrix_print(const struct matrix A) {
    printf("_____\n");
    for (size_t row = 0; row < A.rows; ++row) {
        for (size_t col = 0; col < A.cols; ++col) {
            printf("%4.2f \t", A.data[A.cols * row + col]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

// Сложение матриц A и B
void matrix_add(const struct matrix A, const struct matrix B) {
    if (A.cols != B.cols || A.rows != B.rows)
        return;

    if (A.data == NULL || B.data == NULL)
        return;

    // Поэлементное сложение матриц
    for (size_t idx = 0; idx < A.cols * A.rows; ++idx) {
        A.data[idx] += B.data[idx];
    }
    return;
}

// Вычисление суммы матриц A и B
struct matrix matrix_sum(const struct matrix A, const struct matrix B) {
    if (A.cols != B.cols || A.rows != B.rows)
        return MATRIX_NULL;

    struct matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return C;

    // Поэлементное сложение матриц
    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        C.data[idx] = A.data[idx] + B.data[idx];
    return C;
}

// Вычисление разности матриц A и B
struct matrix matrix_sub(const struct matrix A, const struct matrix B) {
    if (A.cols != B.cols || A.rows != B.rows)
        return MATRIX_NULL;

    struct matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return C;

    // Поэлементное вычитание матриц
    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        C.data[idx] = A.data[idx] - B.data[idx];
    return C;
}

// Умножение каждого элемента матрицы на заданный коэффициент
void matrix_mult_coeff(struct matrix A, const double coeff) {
    if (A.data == NULL)
        return;

    // Умножение каждого элемента матрицы на заданный коэффициент
    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        A.data[idx] = A.data[idx] * coeff;

    return;
}
// Вычисление произведения матриц A и B
struct matrix matrix_mult(const struct matrix A, const struct matrix B) {
    if (A.cols != B.rows)
        return MATRIX_NULL;

    struct matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return MATRIX_NULL;

    // Умножение матриц
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

// Транспонирование матрицы
struct matrix matrix_transp(struct matrix* A) {
    struct matrix C = matrix_init(A->cols, A->rows);

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

// Вычисление определителя матрицы
double matrix_det(struct matrix* A) {
    if (A->cols != A->rows)
        return NAN;

    struct matrix C = matrix_init(A->cols, A->rows);
    if (C.data == NULL)
        return NAN;

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

// Возведение матрицы в степень
struct matrix sum_for_e(const size_t deg_acc, const struct matrix A) {
    struct matrix E = matrix_init(A.cols, A.rows);

    if (E.data == NULL) return MATRIX_NULL;

    if (deg_acc == 1)
    {
        struct matrix E = matrix_make_ident(A.cols, A.rows);
        return E;
    }

    if (deg_acc == 2) return A;

    if (deg_acc > 2)
    {
        E = A;
        for (size_t id = 2; id < deg_acc; ++id)
        {
            struct matrix buf = E;
            E = matrix_mult(buf, A);
            for (size_t idx = 0; idx < E.rows * E.cols; ++idx)
            {
                E.data[idx] /= (id);
            }
            matrix_free(&buf);
        }
    }
    return E;
}

// Вычисление экспоненты от матрицы
struct matrix matrix_exp(struct matrix* A, const size_t accuracy) {
    if (A->cols != A->rows) {
        matrix_error_message();
        return MATRIX_NULL;
    }

    struct matrix E = matrix_init(A->rows, A->cols);

    if (E.data == NULL) {
        return MATRIX_NULL;
    }
    struct matrix matrix_transfer;

    for (size_t deg_acc = 1; deg_acc <= accuracy; ++deg_acc) {
        matrix_transfer = sum_for_e(deg_acc, *A);
        struct matrix buf1 = E;
        E = matrix_sum(buf1, matrix_transfer);
        matrix_free(&buf1);
        matrix_free(&matrix_transfer);
    }
    return E;
}

int main() {
    struct matrix A, B, C;

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
