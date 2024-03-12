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

    Matrix& multiply(Matrix& trg, const Matrix& A) const;
    
    void set_null();

public:
    Matrix();
    Matrix(const size_t a, const size_t b);
    Matrix(const Matrix& A);
    Matrix(Matrix&& A);

    void clear_items_to_zero();
    void set_as_identity();

    size_t get_rows() const;
    size_t get_cols() const;

    MatrixItem& operator[](const size_t idx);

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


    void set(size_t row, size_t col, MatrixItem value);
    MatrixItem get(size_t row, size_t col) const;


    Matrix transposed();
    double determinant() const;
    Matrix exponential(const int iterations = 100) const;

    MatrixItem get_maximum();

    ~Matrix();
};

Matrix operator+(const Matrix& A, const Matrix& B);
Matrix operator+(const Matrix& A, Matrix&& B);

Matrix operator-(const Matrix& A, const Matrix& B);
Matrix operator-(const Matrix& A, Matrix&& B);

std::ostream& operator<<(std::ostream& os, const Matrix& A);
