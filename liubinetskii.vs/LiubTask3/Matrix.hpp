/// 
/// перегразка инедексирования
/// разобраться с выводом
/// 

typedef double MatrixItem;

class Matrix
{
private:
    size_t rows;
    size_t cols;
    MatrixItem* items;

    MatrixItem* ptr_start();
    MatrixItem* ptr_end();
    const MatrixItem* ptr_start() const;
    const MatrixItem* ptr_end() const;

    Matrix& multiply(Matrix& trg, const Matrix& A) const;

    void set_null();

public:
    Matrix();
    Matrix(const size_t a, const size_t b);
    Matrix(const Matrix& A);
    Matrix(Matrix&& A);

    void set_all_zero();
    void set_all_one();

    size_t get_rows() const;
    size_t get_cols() const;

    Matrix& operator=(std::initializer_list<MatrixItem> lst);
    Matrix& operator=(const Matrix& A);
    Matrix& operator=(Matrix&& A);

    Matrix& operator+=(const Matrix& A);
    Matrix& operator-=(const Matrix& A);
    bool operator==(const Matrix& A) const;

    Matrix operator*(const Matrix& A) const;
    Matrix& operator*=(const Matrix& A);

    Matrix operator*(const MatrixItem& factor);
    Matrix& operator*=(const MatrixItem& factor);

    Matrix transposed();
    double determinant() const;
    Matrix exponential(const MatrixItem& accuracy) const;

    MatrixItem get_maximum();

    ~Matrix();
};

Matrix operator+(const Matrix& A, const Matrix& B);
Matrix operator+(const Matrix& A, Matrix&& B);

Matrix operator-(const Matrix& A, const Matrix& B);
Matrix operator-(const Matrix& A, Matrix&& B);

std::ostream& operator<<(std::ostream& os, const Matrix& A);
