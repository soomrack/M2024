#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef double MatrixItem; // Тип элемента матрицы

struct Matrix {
    size_t cols; // Количество столбцов
    size_t rows; // Количество строк
    MatrixItem* data; // Указатель на данные матриц
};

const struct Matrix MATRIX_NULL = { 0, 0, NULL };

void matrix_error_message(const char str[]) {
    printf("%s\n", str); // Сообщение об ошибке
}

// Инициализация матрицы заданных размеров
struct Matrix matrix_init(const size_t cols, const size_t rows) {
    struct Matrix A;
    A.cols = cols;
    A.rows = rows;
    A.data = NULL;

    // Проверка на нулевые размеры матрицы
    if (cols == 0 || rows == 0) {
        matrix_error_message("Ваша матрица не имеет ячеек");
        return A;
    }

    // Проверка на переполнение памяти при выделении памяти для хранения данных матрицы
    if (rows >= SIZE_MAX / sizeof(MatrixItem) / cols) {
        matrix_error_message("Недостачно места для хранения матрицы");
        return MATRIX_NULL;
    }

    // Выделение памяти под данные матрицы
    A.data = (MatrixItem*)malloc(A.cols * A.rows * sizeof(MatrixItem));

    // Проверка успешности выделения памяти
    if (A.data == NULL) {
        matrix_error_message("Ошибка выделения матрицы");
    }
    return MATRIX_NULL;
}

// Создание единичной матрицы заданных размеров
struct Matrix matrix_make_ident(size_t rows, size_t cols) {
    struct Matrix I = matrix_init(rows, cols); // Создание матрицы заданных размеров
    if (I.data == NULL) {
        return I; // Возвращение пустой матрицы в случае неудачи
    }

    // Заполнение элементов диагонали единицами, остальных элементов - нулями
    memset(I.data, 0, I.cols * I.rows * sizeof(MatrixItem));
    for (size_t idx = 0; idx < rows * cols; idx += cols + 1) {
        I.data[idx] = 1;
    }
    return I;
}

// Освобождение памяти, выделенной под матрицу
void matrix_free(struct Matrix* matrix) {
    free(matrix->data);
    *matrix.rows = 0;
    *matrix.cols = 0;
    *matrix = MATRIX_NULL;
}

// Заполнение матрицы случайными значениями
void matrix_random(struct Matrix* A) {
    for (size_t idx = 0; idx < A->cols * A->rows; idx++) {
        A->data[idx] = ((int)rand() % 10);
    }
}

// Вывод матрицы на экран
void matrix_print(const struct Matrix A) {
    printf("_____\n");
    for (size_t idx = 0; idx < A.rows * A.cols; idx++) {
        if (idx % A.cols == 0) {
            printf("\n");
        }
        printf("%4.2f \t", A.data[idx]);
    }
    printf("\n");
}

// A = A + B
void matrix_add(const struct Matrix A, const struct Matrix B) {
    if (A.cols != B.cols || A.rows != B.rows)
        return;

    if (A.data == NULL)
        return;

    // Поэлементное сложение матриц
    for (size_t idx = 0; idx < A.cols * A.rows; ++idx) {
        A.data[idx] += B.data[idx];
    }
    return;
}

// Вычисление разности матриц A и B
struct Matrix matrix_sub(const struct Matrix A, const struct Matrix B) {
    if (A.cols != B.cols || A.rows != B.rows)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return C;

    // Поэлементное вычитание матриц
    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        C.data[idx] = A.data[idx] - B.data[idx];
    return C;
}

// Умножение каждого элемента матрицы на заданный коэффициент
void matrix_mult_coeff(struct Matrix A, const double coeff) {
    if (A.data == NULL) {
        matrix_error_message("Матрица пустая. Невозможно умножение на коэффицент");
        return;
    }

    // Умножение каждого элемента матрицы на заданный коэффициент
    for (size_t idx = 0; idx < A.cols * A.rows; ++idx)
        A.data[idx] = A.data[idx] * coeff;
    return;
}

// Вычисление произведения матриц A и B
struct Matrix matrix_mult(const struct Matrix A, const struct Matrix B) {
    if (A.cols != B.rows)
        return MATRIX_NULL;

    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL)
        return C;

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
struct Matrix matrix_transp(struct Matrix* A) {
    struct Matrix C = matrix_init(A->cols, A->rows);

    if (C.data == NULL) {
        return C;
    }

    for (size_t rowA = 0; rowA < A->rows; ++rowA) {
        for (size_t colA = 0; colA < A->cols; ++colA) {
            C.data[(A->rows) * colA + rowA] = A->data[colA + rowA * A->cols];
        }
    }
    return C;
}

// Вычисление определителя матрицы
double matrix_det(struct Matrix* A) {
    if (A->cols != A->rows)
        return NAN;

    struct Matrix C = matrix_init(A->cols, A->rows);
    if (C.data == NULL) {
        matrix_free(&C);
        return NAN;
    }

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
    return NAN;
}

struct Matrix matrix_copy(const struct Matrix A) {
    struct Matrix C = matrix_init(A.cols, A.rows);
    if (C.data == NULL) {
        return C;
    }
    memcpy(C.data, A.data, A.cols * A.rows * sizeof(MatrixItem));
    return C;
}

// Возведение матрицы в степень
struct Matrix sum_for_e(const struct Matrix A, const size_t deg_acc) {
    if (deg_acc == 0)
    {
        return matrix_make_ident(A.cols, A.rows);
    }

    struct Matrix E = matrix_init(A.cols, A.rows);

    if (E.data == NULL) {
        return E;
    }

    E = matrix_copy(A);

    if (deg_acc == 1) {
        return E;
    }

    for (size_t id = 2; id < deg_acc; ++id)
    {
        struct Matrix power = E;
        E = matrix_mult(power, A);
        for (size_t idx = 0; idx < E.rows * E.cols; ++idx)
        {
            E.data[idx] /= (id);
        }
        matrix_free(&power);
    }
    return E;
}

// Вычисление экспоненты от матрицы
struct Matrix matrix_exp(struct Matrix* A, const size_t accuracy) {
    if (A->cols != A->rows) {
        matrix_error_message("У вас не квадратная матрица");
        return MATRIX_NULL;
    }

    struct Matrix E = matrix_init(A->rows, A->cols);

    if (E.data == NULL) {
        return E;
    }
    struct Matrix MatrixTransfer;

    for (size_t deg_acc = 1; deg_acc <= accuracy; ++deg_acc) {
        MatrixTransfer = sum_for_e(*A, deg_acc);
        struct Matrix buf1 = E;
        matrix_add(buf1, MatrixTransfer);
        E = matrix_copy(buf1);
        matrix_free(&buf1);
        matrix_free(&MatrixTransfer);
    }
    return E;
}

int main() {
    system("chcp 65001"); Ы
        struct Matrix A, B, C;

    double det;

    A = matrix_init(0, 3);
    matrix_random(&A);
    matrix_print(A);

    B = matrix_init(3, 3);
    matrix_fill(&B);
    matrix_print(B);

    matrix_add(B, A);
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
