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
    MatrixItem* data;

    void set_null();
     
public: // конструкторы
    Matrix(); // пустой массив, с размерами, как у прошлой матрицы, следующая матрица отдаст данные новой и освобождается, деструктор = освобождение опер памяти
    Matrix(const size_t a, const size_t b);
    Matrix(const Matrix& A);
    Matrix(Matrix&& A);
    ~Matrix();

public:
    void clear_data_to_zero();
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

    Matrix operator*(const MatrixItem& coefficient); //  матрица*коэффициент = новые значения в старой матрице
    Matrix& operator*=(const MatrixItem& coefficient); 


    void set(size_t row, size_t col, MatrixItem value); //установить значения
    MatrixItem get(size_t row, size_t col) const; // забрать значения


    Matrix transposed(); 
    Matrix minor(const size_t minor_row, const size_t minor_col);
    double determinant_classic() const;
    Matrix exponential(const int iterations = 100) const;


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
MatrixException ERROR_MATRIX("Error matrix!");   //класс с экземплярами 
MatrixException OUT_OF_SIZE("Out of size!"); //выход за пределы массива
MatrixException NO_MEMORY_ALLOCATED("No memory allocated!"); //не выделяется память
MatrixException NULL_POINTER_REFERENCE("Null pointer!");


Matrix::Matrix() : rows{ 0 }, cols{ 0 }, data{ nullptr } {}  //нулевая матрица
const MatrixItem epsilon = 0.001; //погрешность в экспоненциальной матрице

Matrix::Matrix(const size_t a, const size_t b) //создание матрицы
    : rows{ a }, cols{ b }, data{ nullptr }
{
    if (rows == 0 && cols == 0) // если всё 0, пустая матрица - возврат назад
        return;

    if (rows == 0 || cols == 0)
        throw ERROR_MATRIX;

    if (rows >= _CRT_SIZE_MAX / cols / sizeof(MatrixItem)) // проверка размерности, нельзя выделить оперативную память, тк размерность больше
        throw NO_MEMORY_ALLOCATED;

    data = new MatrixItem[rows * cols];  // выделяем память new
}

Matrix::Matrix(const Matrix& A) //конструктор копирования
    : rows{ A.rows }, cols{ A.cols }, data{ nullptr }
{
    if (A.data == nullptr) return;

    if (rows >= _CRT_SIZE_MAX / cols / sizeof(MatrixItem))
        throw NO_MEMORY_ALLOCATED;

    data = new MatrixItem[rows * cols];

    memcpy(data, A.data, rows* cols * sizeof(MatrixItem)); //копирование матрицы
}


Matrix::Matrix(Matrix&& A) : rows{ A.rows }, cols{ A.cols }, data{ A.data } // конструктор переноса - копирование ссылки, прошлой матрицы нет
{
    A.set_null(); // обнуление матрицы, которую передали
}


void Matrix::set_null()
{
    data = nullptr;
    rows = 0;
    cols = 0;
}


Matrix& Matrix::operator=(std::initializer_list<MatrixItem> lst) //присваивание из списка
{
    if (lst.size() != rows * cols)
        throw OUT_OF_SIZE;

    std::copy(lst.begin(), lst.end(), data);

    return *this; // this = матрица слева А*=В, А= 1 2 3 4 
}


Matrix& Matrix::operator=(const Matrix& A) // копирование данных из матрицы А в матрицу В
{
    if (this == &A) return *this;

    if (data == nullptr) {
        rows = A.rows;
        cols = A.cols;
        if (A.data == nullptr)
            return *this;

        data = new MatrixItem[A.rows * A.cols];


        memcpy(data, A.data, rows * cols * sizeof(MatrixItem));
        return *this;
    }

    if (rows * cols == A.cols * A.rows) {
        memcpy(data, A.data, rows * cols * sizeof(MatrixItem));
        return *this;
    }

    delete[] data; //если А не пустая, то очистить 

    data = new MatrixItem[A.rows * A.cols];  //выделение памяти, если есть ланные - копировать

    rows = A.rows;
    cols = A.cols;

    if (A.data == nullptr) // возврат матрицы
        return *this;

    memcpy(data, A.data, rows * cols * sizeof(MatrixItem)); //обе матрицы копируются, перестаёт существовать
     
    return *this; //возврат матрицы слева
}


Matrix& Matrix::operator=(Matrix&& A) // перенос справа влево, и справа зачистка
{
    if (data != nullptr)
        delete[] data; // если что-то было слева, то очистка. всё справа перенесём влево

    rows = A.rows;
    cols = A.cols;
    data = A.data;

    A.set_null(); // очистка справа. все данные справа перешли в лево

    return *this; // возврат из лева
}


void Matrix::clear_data_to_zero() //очистка всей опер памяти = обнуление матрицы = заполнение нулями
{
    memset(data, 0, sizeof(MatrixItem) * cols * rows);
}


void Matrix::set_as_identity() //после обнуление заполнить матрицу как единичную - поиск минимальной размерности и заполнение 1
{
    clear_data_to_zero(); 

    size_t min_size = std::min(rows, cols);

    for (size_t idx = 0; idx < min_size; idx++)
        data[idx * cols + idx] = 1.;
}


size_t Matrix::get_rows() const { return rows; } //возврат количества строк, столбцов


size_t Matrix::get_cols() const { return cols; }


MatrixItem& Matrix::operator[](const size_t idx) // проверка индекса
{
    if (idx >= rows * cols)
        throw OUT_OF_SIZE;

    if (idx < 0)
        throw OUT_OF_SIZE;

    return data[idx];
}


Matrix& Matrix::operator+=(const Matrix& A) // суммирование с присваиванием
{
    if ((rows != A.rows) || (cols != A.cols))
        throw ERROR_MATRIX;

    for (size_t idx = 0; idx < (rows * cols); idx += cols + 1)
        data[idx] += A.data[idx];

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
        throw ERROR_MATRIX;

    for (size_t idx = 0; idx < (rows * cols); idx += cols + 1)
        data[idx] -= A.data[idx];

    return *this;
}


Matrix operator-(const Matrix& A, const Matrix& B)
{
    Matrix sub = A;
    sub -= B;
    return sub;
}



Matrix Matrix::operator*(const Matrix& A) const
{
    if (cols != A.rows)
        throw ERROR_MATRIX;

    Matrix mult(rows, A.cols);
    for (size_t num_row = 0; num_row < rows; num_row++) {
        for (size_t num_col = 0; num_col < A.cols; num_col++) {
            MatrixItem sum = 0;

            for (size_t num_sum = 0; num_sum < A.rows; num_sum++) {
                sum += data[num_row * cols + num_sum] * A.data[num_sum * A.cols + num_col];
            }

            data[num_row * cols + num_col] = sum;
        }
    }
    return mult;
}


Matrix& Matrix::operator*=(const Matrix& A)
{
    *this = *this * A;
    return *this;
}


Matrix& Matrix::operator*=(const MatrixItem& coefficient) //умножение на коэффициент с присваиванием
{
    for (size_t idx = 0; idx < (rows * cols); idx += cols + 1)
        data[idx] *= coefficient;

    return *this;
}


Matrix Matrix::operator*(const MatrixItem& coefficient)
{
    Matrix mult = *this;
    mult *= coefficient;
    return mult;
}


Matrix Matrix::transposed() // транспонирование - строки столбцы меняем местами и заполняем
{
    Matrix trn(cols, rows);

    for (size_t num_row = 0; num_row < cols; num_row++)
        for (size_t num_col = 0; num_col < rows; num_col++)
            trn.data[num_row * trn.cols + num_col] = data[num_col * cols + num_row];

    return trn;
}

Matrix Matrix::minor(const size_t minor_row, const size_t minor_col)
{
    if ((cols <= 1) || (rows <= 1)) {
        throw ERROR_MATRIX;
       
    }
    if ((minor_col >= cols) || (minor_row >= rows)) {
        throw ERROR_MATRIX;
       
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
            minor.data[current_col * minor.rows + current_row] = data[origin_index];
            origin_index += 1;
        }
        if (minor_row == minor.rows) {  // иначе не обрабатывается последний столбец
            origin_index += 1;
        }
    }
    return minor;
}


double Matrix::determinant_classic() const
{
    if (cols != rows)
        throw ERROR_MATRIX;

    if (data == nullptr)
        throw ERROR_MATRIX;

    double determinant = 0.0;

    if (cols == 1) {
        determinant = data[0];
        return determinant;
    }

    if (cols == 2) {
        determinant = data[0] * data[3] - data[1] * data[2];
        return determinant;
    }

    if (cols == 3) {
        determinant = (data[0] * data[4] * data[8]) + (data[1] * data[5] * data[6]) + (data[2] * data[3] * data[7]) - 
            (data[2] * data[4] * data[6]) - (data[0] * data[5] * data[7]) - (data[1] * data[3] * data[8]);
        return determinant;
    }

}


Matrix Matrix::exponential(const int iterations) const
{
    if (cols != rows)
        throw ERROR_MATRIX;

    if (data == nullptr)
        throw ERROR_MATRIX;

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


void Matrix::set(size_t row, size_t col, MatrixItem value)
{
    if (row >= rows || col >= cols)
        throw OUT_OF_SIZE;

    data[row * cols + col] = value;
}


MatrixItem Matrix::get(size_t row, size_t col) const
{
    if (row >= rows || col >= cols)
        throw OUT_OF_SIZE;

    return data[row * cols + col];
}


bool Matrix::operator==(const Matrix& A) const
{
    if ((cols != A.cols) || (rows != A.rows))
        return false;

    for (size_t idx = 0; idx < (rows * cols); idx++) {
        if (std::abs(data[idx] - A.data[idx]) > epsilon)
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
    if (data != nullptr) { 
        delete[] data;
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

        Matrix A(2, 2);
        init_matrix_as_random(&A);
        std::cout << A << std::endl;

        Matrix B(2, 2);
        B.set_as_identity();
        std::cout << B << std::endl;

        Matrix C;
        C = A + B;
        std::cout << C << std::endl;

        std::cout << "----------" << std::endl;

    }
    catch (MatrixException& ex)
    {
        std::cout << "ERROR = " << ex.what();
    }

    return 0;
}
