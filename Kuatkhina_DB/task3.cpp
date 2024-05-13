#include <math.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>

typedef double MatrixItem;

class Matrix  //класс с характеристиками
{
private:
    size_t rows;
    size_t cols;
    MatrixItem* items;

    Matrix& multiply(Matrix& trg, const Matrix& A) const;  //метод сложения
    void set_null();
     
public: // конструкторы
    Matrix(); // пустой массив, с размерами, как у прошлой матрицы, следующая матрица отдаст данные новой и освобождается, деструктор = освобождение опер памяти
    Matrix(const size_t a, const size_t b);
    Matrix(const Matrix& A);
    Matrix(Matrix&& A);
    ~Matrix();

public:
    void clear_items_to_zero();
    void set_as_identity();
    size_t get_rows() const;
    size_t get_cols() const;

    MatrixItem& operator[](const size_t idx); //обращение к индексу элемента (не обращаясь к матрице) 

    Matrix& operator=(std::initializer_list<MatrixItem> lst); // создание матрицы по введённым числам
    Matrix& operator=(const Matrix& A); // копирование данных из одной матрицы в другую
    Matrix& operator=(Matrix&& A); // перемещение данных в новую матрицу

    Matrix& operator+=(const Matrix& A); // А += В  ----  А = А+В
    Matrix& operator-=(const Matrix& A);
    bool operator==(const Matrix& A) const; // оператор сравнения - если одинаковые, то true

    Matrix operator*(const Matrix& A) const; // матрица*матрица = новая матрица
    Matrix& operator*=(const Matrix& A); 

    Matrix operator*(const MatrixItem& factor); //  матрица*коэффициент = новые значения в старой матрице
    Matrix& operator*=(const MatrixItem& factor); 


    void set(size_t row, size_t col, MatrixItem value); //установить значения
    MatrixItem get(size_t row, size_t col) const; // забрать значения


    Matrix transposed(); 
    Matrix minor(const size_t minor_row, const size_t minor_col);
    double determinant() const;
    Matrix exponential(const int iterations = 100) const;

    MatrixItem get_maximum();


};


Matrix operator+(const Matrix& A, const Matrix& B);
Matrix operator-(const Matrix& A, const Matrix& B);

std::ostream& operator<<(std::ostream& os, const Matrix& A); //оператор вывода - матрица в консоль в виде текста


class MatrixException : public std::exception { //если непредвиденное действие = как деление на 0, отработка ошибки
private:
    const char* message;

public:
    MatrixException(std::string msg) : message(msg.c_str()) {}
    const char* what() { return message; }
};
MatrixException BAD_REQUEST("bad_request");   //класс с экземплярами 
MatrixException OUT_OF_RANGE("out_of_range"); //выход за пределы массива
MatrixException NO_MEMORY_ALLOCATED("no_memory_allocated"); //не выделяется память
MatrixException NULL_POINTER_REFERENCE("null_pointer_reference");


Matrix::Matrix() : rows{ 0 }, cols{ 0 }, items{ nullptr } {}  //нулевая матрица
const MatrixItem epsilon = 0.001; //погрешность в экспоненциальной матрице

Matrix::Matrix(const size_t a, const size_t b) //создание матрицы
    : rows{ a }, cols{ b }, items{ nullptr }
{
    if (rows == 0 && cols == 0) // если всё 0, пустая матрица - возврат назад
        return;

    if (rows == 0 || cols == 0)
        throw BAD_REQUEST;

    if (rows >= _CRT_SIZE_MAX / cols / sizeof(MatrixItem)) // проверка размерности, нельзя выделить оперативную память, тк размерность больше
        throw NO_MEMORY_ALLOCATED;

    items = new MatrixItem[rows * cols];  // выделяем память new
}

Matrix::Matrix(const Matrix& A) //конструктор копирования
    : rows{ A.rows }, cols{ A.cols }, items{ nullptr }
{
    if (A.items == nullptr) return;

    if (rows >= _CRT_SIZE_MAX / cols / sizeof(MatrixItem))
        throw NO_MEMORY_ALLOCATED;

    items = new MatrixItem[rows * cols];

    memcpy(items, A.items, rows* cols * sizeof(MatrixItem)); //копирование матрицы
}


Matrix::Matrix(Matrix&& A) : rows{ A.rows }, cols{ A.cols }, items{ A.items } // конструктор переноса - копирование ссылки, прошлой матрицы нет
{
    A.set_null(); // обнуление матрицы, которую передали
}


void Matrix::set_null()
{
    items = nullptr;
    rows = 0;
    cols = 0;
}


Matrix& Matrix::operator=(std::initializer_list<MatrixItem> lst) //присваивание из списка
{
    if (lst.size() != rows * cols)
        throw OUT_OF_RANGE;

    std::copy(lst.begin(), lst.end(), items);

    return *this; // this = матрица слева А*=В, А= 1 2 3 4 
}


Matrix& Matrix::operator=(const Matrix& A) // копирование данных из матрицы А в матрицу В
{
    if (this == &A) return *this;

    if (items == nullptr) {
        rows = A.rows;
        cols = A.cols;
        if (A.items == nullptr)
            return *this;

        items = new MatrixItem[A.rows * A.cols];


        memcpy(items, A.items, rows * cols * sizeof(MatrixItem));
        return *this;
    }

    if (rows * cols == A.cols * A.rows) {
        memcpy(items, A.items, rows * cols * sizeof(MatrixItem));
        return *this;
    }

    delete[] items; //если А не пустая, то очистить 

    items = new MatrixItem[A.rows * A.cols];  //выделение памяти, если есть ланные - копировать

    rows = A.rows;
    cols = A.cols;

    if (A.items == nullptr) // возврат матрицы
        return *this;

    memcpy(items, A.items, rows * cols * sizeof(MatrixItem)); //обе матрицы копируются, перестаёт существовать
     
    return *this; //возврат матрицы слева
}


Matrix& Matrix::operator=(Matrix&& A) // перенос справа влево, и справа зачистка
{
    if (items != nullptr)
        delete[] items; // если что-то было слева, то очистка. всё справа перенесём влево

    rows = A.rows;
    cols = A.cols;
    items = A.items;

    A.set_null(); // очистка справа. все данные справа перешли в лево

    return *this; // возврат из лева
}


void Matrix::clear_items_to_zero() //очистка всей опер памяти = обнуление матрицы = заполнение нулями
{
    memset(items, 0, sizeof(MatrixItem) * cols * rows);
}


void Matrix::set_as_identity() //после обнуление заполнить матрицу как единичную - поиск минимальной размерности и заполнение 1
{
    clear_items_to_zero(); 

    size_t min_size = std::min(rows, cols);

    for (size_t idx = 0; idx < min_size; idx++)
        items[idx * cols + idx] = 1.;
}


size_t Matrix::get_rows() const { return rows; } //возврат количества строк, столбцов


size_t Matrix::get_cols() const { return cols; }


MatrixItem& Matrix::operator[](const size_t idx) // проверка индекса
{
    if (idx >= rows * cols)
        throw OUT_OF_RANGE;

    if (idx < 0)
        throw OUT_OF_RANGE;

    return items[idx];
}


Matrix& Matrix::operator+=(const Matrix& A) // суммирование с присваиванием
{
    if ((rows != A.rows) || (cols != A.cols))
        throw BAD_REQUEST;

    for (size_t idx = 0; idx < (rows * cols); idx += cols + 1)
        items[idx] += A.items[idx];

    return *this;
}


Matrix operator+(const Matrix& A, const Matrix& B)
{
    Matrix sum = A;
    sum += B;
    return sum;
}



Matrix& Matrix::operator-=(const Matrix& A)
{
    if ((rows != A.rows) || (cols != A.cols))
        throw BAD_REQUEST;

    for (size_t idx = 0; idx < (rows * cols); idx += cols + 1)
        items[idx] -= A.items[idx];

    return *this;
}


Matrix operator-(const Matrix& A, const Matrix& B)
{
    Matrix sub = A;
    sub -= B;
    return sub;
}


Matrix& Matrix::multiply(Matrix& trg, const Matrix& A) const // матрица*матрица для использования далее
{
    for (size_t num_row = 0; num_row < rows; num_row++) {
        for (size_t num_col = 0; num_col < A.cols; num_col++) {
            MatrixItem sum = 0;

            for (size_t num_sum = 0; num_sum < A.rows; num_sum++) {
                sum += items[num_row * cols + num_sum] * A.items[num_sum * A.cols + num_col];
            }

            trg.items[num_row * trg.cols + num_col] = sum;
        }
    }

    return trg;
}


Matrix Matrix::operator*(const Matrix& A) const
{
    if (cols != A.rows)
        throw BAD_REQUEST;

    Matrix mult(rows, A.cols);
    multiply(mult, A);
    return mult;
}


Matrix& Matrix::operator*=(const Matrix& A)
{
    *this = *this * A;
    return *this;
}


Matrix& Matrix::operator*=(const MatrixItem& factor) //умножение на коэффициент с присваиванием
{
    for (size_t idx = 0; idx < (rows * cols); idx += cols + 1)
        items[idx] *= factor;

    return *this;
}


Matrix Matrix::operator*(const MatrixItem& factor)
{
    Matrix mult = *this;
    mult *= factor;
    return mult;
}


Matrix Matrix::transposed() // транспонирование - строки столбцы меняем местами и заполняем
{
    Matrix trn(cols, rows);

    for (size_t num_row = 0; num_row < cols; num_row++)
        for (size_t num_col = 0; num_col < rows; num_col++)
            trn.items[num_row * trn.cols + num_col] = items[num_col * cols + num_row];

    return trn;
}

Matrix Matrix::minor(const size_t minor_row, const size_t minor_col)
{
    if ((cols <= 1) || (rows <= 1)) {
        throw BAD_REQUEST;
       
    }
    if ((minor_col >= cols) || (minor_row >= rows)) {
        throw BAD_REQUEST;
       
    }

    Matrix minor(cols - 1, rows - 1);
    size_t origin_index = 0;

    for (size_t current_col = 0; current_col < minor.cols; current_col++) {
        if (current_col == minor_col) {
            origin_index += cols;
        }
        for (size_t current_row = 0; current_row < minor.rows; current_row++) {
            if (current_row == minor_row) {
                origin_index += 1;
            }
            minor.items[current_col * minor.rows + current_row] = items[origin_index];
            origin_index += 1;
        }
        if (minor_row == minor.rows) {  // иначе не обрабатывается последний столбец
            origin_index += 1;
        }
    }
    return minor;
}



double Matrix::determinant() const
{
    if (cols != rows)
        throw BAD_REQUEST;

    if (items == nullptr)
        throw BAD_REQUEST;

    double determinant = 0.0;

    if (cols == 1) {
        determinant = items[0];
        return determinant;
    }

    Matrix matrix_copy(rows, cols);
    for (size_t index = 0; index < matrix_copy.cols * matrix_copy.rows; index++) {
        matrix_copy.items[index] = items[index];
    }

    Matrix minor;
    for (size_t summ_index = 0; summ_index < matrix_copy.rows; summ_index++) {
        minor = matrix_copy.minor(0, summ_index);

        if (summ_index % 2 == 0) {
            determinant += items[summ_index] * minor.determinant();
        }
        else {
            determinant -= items[summ_index] * minor.determinant();
        }
    }

    return determinant;
}



Matrix Matrix::exponential(const int iterations) const
{
    if (cols != rows)
        throw BAD_REQUEST;

    if (items == nullptr)
        throw BAD_REQUEST;

    Matrix sum(rows, cols);
    Matrix term(rows, cols);

    term.set_as_identity(); // переменная наращивания (с факториалами и тд)
    sum.set_as_identity(); // для суммирования

    for (size_t count = 1; count < iterations; count++) {
        term *= *this;
        term *= (1. / count);

        sum += term;
    }

    return sum;
}


MatrixItem Matrix::get_maximum() // максимальный элемент
{
    MatrixItem max = 0;
    MatrixItem num = 0;

    for (size_t idx = 0; idx < (rows * cols); idx++) {
        num = std::fabs(items[idx]);
        if (num > max) max = num;
    }

    return max;
}


void Matrix::set(size_t row, size_t col, MatrixItem value)
{
    if (row >= rows || col >= cols)
        throw OUT_OF_RANGE;

    items[row * cols + col] = value;
}


MatrixItem Matrix::get(size_t row, size_t col) const
{
    if (row >= rows || col >= cols)
        throw OUT_OF_RANGE;

    return items[row * cols + col];
}


bool Matrix::operator==(const Matrix& A) const
{
    if ((cols != A.cols) || (rows != A.rows))
        return false;

    for (size_t idx = 0; idx < (rows * cols); idx++) {
        if (std::abs(items[idx] - A.items[idx]) > epsilon)
            return false;
    }

    return true;
}


std::ostream& operator<<(std::ostream& os, const Matrix& A)  //вывод данных во встроенную функцию, вывод с табуляцией
{
    for (size_t row = 0; row < A.get_rows(); row++) {
        for (size_t col = 0; col < A.get_cols(); col++) {
            os << A.get(row, col) << "\t";
        }
        os << std::endl; // перенос с новой строки
    }

    os << std::endl;

    return os;
}


Matrix::~Matrix() //очищение
{
    if (items != nullptr) { 
        delete[] items;
    }
}

int get_rand_integer(int start = 0, int end = 20) { //генерация цифр для рандома

    if (start > end) {
        std::swap(start, end);
    }

    return rand() % (end - start + 1) + start;
}


void init_matrix_as_random(Matrix* M) { //заполнение матрицы рандомом
    size_t rows = M->get_rows();
    size_t cols = M->get_cols();

    for (size_t idx = 0; idx < rows * cols; idx++)
    {
        (*M)[idx] = get_rand_integer();
    }
}


int main()
{
    srand(time(NULL)); 
    setlocale(LC_CTYPE, "");
    ///std::cout << std::setprecision(2) << std::fixed;

    try {
        Matrix A(6, 4);
        init_matrix_as_random(&A);
        std::cout << A << std::endl;

        Matrix B(6, 4);
        B.set_as_identity();
        std::cout << B << std::endl;

        Matrix C;
        C = A + B;
        std::cout << C << std::endl;

        std::cout << "----------" << std::endl;

       /* Matrix&& Bb = std::move(B);
        Matrix result = A + Bb;
        std::cout << A << std::endl;
        std::cout << B << std::endl;
        std::cout << result << std::endl;
        */

    }
    catch (MatrixException& ex)
    {
        std::cout << "Oops *-* = " << ex.what();
    }

    return 0;
}
