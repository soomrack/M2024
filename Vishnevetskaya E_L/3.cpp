#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip> 


class Matrix {
private:
    size_t numCols;
    size_t numRows;
    double* matrixData = nullptr;

public:
    Matrix();
    Matrix(const size_t initCols, const size_t initRows);       

    void randomFill();
    void fillFactorial();
    void zerosFill();
    void identityFill();
    Matrix getMinor(const size_t minorCol, const size_t minorRow);
    double determinant();
    Matrix transpose();
    Matrix inverse();

    ~Matrix();

    Matrix(const Matrix& other);  // конструтор копирования
    Matrix(Matrix&& other) noexcept;  // конструктор перемещения

    Matrix& operator=(const Matrix& matrix);  // оператор копирования
    Matrix& operator=(Matrix&& matrix) noexcept;  // оператор перемещения

    Matrix& operator+=(const Matrix& matrix);
    Matrix& operator+=(const double number);
    Matrix& operator-=(const Matrix& matrix);
    Matrix& operator-=(const double number);
    Matrix& operator*=(const double number);
    Matrix& operator*=(const Matrix& matrix);
    Matrix& operator/=(const double number);
    Matrix operator+(const Matrix& matrix);
    Matrix operator+(const double number);
    Matrix operator-(const Matrix& matrix);
    Matrix operator-(const double number);
    Matrix operator*(const Matrix& matrix);
    Matrix operator*(const double number);
    Matrix operator^(const unsigned int exponent);
    friend Matrix operator^(const double base, const Matrix& matrix);

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
};

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.numRows; ++i) {
        for (size_t j = 0; j < matrix.numCols; ++j) {
            os << std::fixed << std::setprecision(3) << matrix.matrixData[i * matrix.numCols + j] << "\t";
        }
        os << std::endl;
    }
    return os;
}

Matrix::Matrix() : numCols{0}, numRows{0}, matrixData{nullptr} {}

Matrix::Matrix(const size_t initCols, const size_t initRows) {
    if (initCols == 0 || initRows == 0) {
        throw std::runtime_error("WARNING: the matrix size is zero\n");
    }

    if (initRows >= SIZE_MAX / sizeof(double) / initCols) {
        throw std::runtime_error("WARNING: the matrix size is too big");
    };

    numCols = initCols;
    numRows = initRows;
    matrixData = new double[numCols * numRows];
}

// конструктор перемещения
Matrix::Matrix(Matrix&& other) noexcept : numCols(other.numCols), numRows(other.numRows), matrixData(other.matrixData) {
    other.numCols = 0;
    other.numRows = 0;
    other.matrixData = nullptr;
}

// конструктор копирования
Matrix::Matrix(const Matrix& other) : numCols(other.numCols), numRows(other.numRows) {
    if (other.matrixData != nullptr) {
        matrixData = new double[numCols * numRows];
        std::copy(other.matrixData, other.matrixData + numCols * numRows, matrixData);
    } else {
        matrixData = nullptr;
    }
}

//Оператор копирования
Matrix& Matrix::operator=(const Matrix& matrix) {
    if (this == &matrix) {
        return *this;
    }

    if (numCols * numRows != matrix.numRows*matrix.numCols) {
        delete[] matrixData;
        numCols = matrix.numCols;
        numRows = matrix.numRows;
    }

    if (matrix.matrixData != nullptr) {
        matrixData = new double[numCols * numRows];
        std::copy(matrix.matrixData, matrix.matrixData + numCols * numRows, matrixData);
    } else {
        matrixData = nullptr;
    }

    return *this;
}

// Оператор перемещения
Matrix& Matrix::operator=(Matrix&& matrix) noexcept {
    if (this == &matrix) {
        return *this;
    }

    delete[] matrixData;

    numCols = matrix.numCols;
    numRows = matrix.numRows;
    matrixData = matrix.matrixData;

    matrix.numCols = 0;
    matrix.numRows = 0;
    matrix.matrixData = nullptr;

    return *this;
}


Matrix::~Matrix() {
    delete[] matrixData;
}

void Matrix::randomFill() {
    for (size_t index = 0; index < numCols * numRows; ++index) {
        matrixData[index] = static_cast<double>(std::rand() % 10000) / 100.0;
    }
}

void Matrix::zerosFill() {
    for (size_t index = 0; index < numCols * numRows; ++index) {
        matrixData[index] = 0.0;
    }
}

void Matrix::identityFill() {
    zerosFill();
    for (size_t index = 0; index < numRows; ++index) {
        matrixData[index * numRows + index] = 1.0;
    }
}

Matrix Matrix::getMinor(const size_t minorCol, const size_t minorRow) {
    if (numCols <= 1 || numRows <= 1) {
        throw std::runtime_error("ERROR: the minor size is zero\n");
    }
    if (minorCol >= numCols || minorRow >= numRows) {
        throw std::runtime_error("ERROR: the minor index is incorrect\n");
    }

    Matrix minorMatrix(numCols - 1, numRows - 1);
    size_t originIndex = 0;

    for (size_t currentCol = 0; currentCol < minorMatrix.numCols; ++currentCol) {
        if (currentCol == minorCol) {
            originIndex += numCols;
        }
        for (size_t currentRow = 0; currentRow < minorMatrix.numRows; ++currentRow) {
            if (currentRow == minorRow) {
                originIndex += 1;
            }
            minorMatrix.matrixData[currentCol * minorMatrix.numRows + currentRow] = matrixData[originIndex];
            originIndex += 1;
        }
        if (minorRow == minorMatrix.numRows) {
            originIndex += 1;
        }
    }
    return minorMatrix;
}

unsigned int factorial(unsigned int n) {
    return (n == 0 || n == 1) ? 1 : n * factorial(n - 1);
}

void Matrix::fillFactorial() {
    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < numCols; ++j) {
            unsigned int n = i * numCols + j + 1;
            double value = 1.0 / factorial(n);
            matrixData[i * numCols + j] = value;
        }
    }
}

double Matrix::determinant() {
    if (numCols != numRows) {
        throw std::runtime_error("ERROR: the matrix size isn't square\n");
    }
    if (numCols == 0 || numRows == 0) {
        throw std::runtime_error("ERROR: the matrix size is zero\n");
    }

    double determinant = 0.0;
    if (numCols == 1) {
        determinant = matrixData[0];
        return determinant;
    }

    Matrix matrixCopy(numCols, numRows);
    for (size_t index = 0; index < matrixCopy.numCols * matrixCopy.numRows; ++index) {
        matrixCopy.matrixData[index] = matrixData[index];
    }

    Matrix minor;
    for (size_t summIndex = 0; summIndex < matrixCopy.numRows; ++summIndex) {
        minor = matrixCopy.getMinor(0, summIndex);

        if (summIndex % 2 == 0) {
            determinant += matrixData[summIndex] * minor.determinant();
        } else {
            determinant -= matrixData[summIndex] * minor.determinant();
        }
    }
    return determinant;
}

Matrix Matrix::transpose() {
    Matrix newMatrix(numRows, numCols);
    for (size_t currentCol = 0; currentCol < newMatrix.numCols; ++currentCol) {
        for (size_t currentRow = 0; currentRow < newMatrix.numRows; ++currentRow) {
            newMatrix.matrixData[newMatrix.numRows * currentCol + currentRow] =
                matrixData[newMatrix.numCols * currentRow + currentCol];
        }
    }
    return newMatrix;
}

Matrix Matrix::inverse() {
    if (numCols != numRows) {
        throw std::runtime_error("ERROR: the matrix size isn't square\n");
    }
    if (numCols == 0 || numRows == 0) {
        throw std::runtime_error("ERROR: the matrix size is zero\n");
    }

    Matrix matrixCopy(numCols, numRows);
    for (size_t index = 0; index < matrixCopy.numCols * matrixCopy.numRows; ++index) {
        matrixCopy.matrixData[index] = matrixData[index];
    }

    double det = matrixCopy.determinant();
    if (det == 0) {
        throw std::runtime_error("ERROR: the matrix determinant is zero, inverse matrix cannot be calculated\n");
    }

    Matrix invMatrix(numCols, numRows);

    Matrix minor;
    for (size_t currentCol = 0; currentCol < invMatrix.numCols; ++currentCol) {
        for (size_t currentRow = 0; currentRow < invMatrix.numRows; ++currentRow) {

            minor = matrixCopy.getMinor(currentCol, currentRow);

            if ((currentCol + currentRow) % 2 == 0) {
                invMatrix.matrixData[currentCol * invMatrix.numCols + currentRow] = minor.determinant();
            } else {
                invMatrix.matrixData[currentCol * invMatrix.numCols + currentRow] = (-1) * minor.determinant();
            }
        }
    }
    invMatrix /= det;
    invMatrix = invMatrix.transpose();
    return invMatrix;
}



Matrix& Matrix::operator+=(const Matrix& matrix) {
    if ((numCols != matrix.numCols) || (numRows != matrix.numRows)) {
        throw std::runtime_error("ERROR: the matrix sizes are incomparable\n");
    }
    for (size_t index = 0; index < numCols * numRows; ++index) {
        matrixData[index] += matrix.matrixData[index];
    }
    return *this;
}

Matrix& Matrix::operator+=(const double number) {
    for (size_t index = 0; index < numCols * numRows; ++index) {
        matrixData[index] += number;
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& matrix) {
    if ((numCols != matrix.numCols) || (numRows != matrix.numRows)) {
        throw std::runtime_error("ERROR: the matrix sizes are incomparable\n");
    }
    for (size_t index = 0; index < numCols * numRows; ++index) {
        matrixData[index] -= matrix.matrixData[index];
    }
    return *this;
}

Matrix& Matrix::operator-=(const double number) {
    for (size_t index = 0; index < numCols * numRows; ++index) {
        matrixData[index] -= number;
    }
    return *this;
}

Matrix& Matrix::operator*=(const double number) {
    for (size_t index = 0; index < numCols * numRows; ++index) {
        matrixData[index] *= number;
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& matrix) {
    if (numRows != matrix.numCols) {
        throw std::runtime_error("ERROR: the matrix sizes are incomparable\n");
    }
    size_t newCols = numCols;
    size_t newRows = matrix.numRows;
    double* newData = new double[newCols * newRows];
    if (newData == nullptr) {
        numCols = 0;
        numRows = 0;
        throw std::runtime_error("ERROR: the matrix wasn't initialised\n");
    }

    for (size_t currentCol = 0; currentCol < newCols; ++currentCol) {
        for (size_t currentRow = 0; currentRow < newRows; ++currentRow) {
            newData[newRows * currentCol + currentRow] = 0.0;

            for (size_t index = 0; index < numRows; ++index) {
                newData[numRows * currentCol + currentRow] +=
                    matrixData[numRows * currentCol + index] *
                    matrix.matrixData[matrix.numRows * index + currentRow];
            }
        }
    }
    numCols = newCols;
    numRows = newRows;
    delete[] matrixData;
    matrixData = newData;

    return *this;
}

Matrix& Matrix::operator/=(const double number) {
    for (size_t index = 0; index < numCols * numRows; ++index) {
        matrixData[index] /= number;
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix& matrix) {
    if ((numCols != matrix.numCols) || (numRows != matrix.numRows)) {
        throw std::runtime_error("ERROR: the matrix sizes are incomparable\n");
    }

    Matrix newMatrix(numCols, numRows);
    newMatrix = *this;
    newMatrix += matrix;
    return newMatrix;
}

Matrix Matrix::operator+(const double number) {
    Matrix newMatrix;
    newMatrix = *this;
    Matrix identityMatrix(numCols, numRows);
    identityMatrix.identityFill();

    identityMatrix *= number;
    newMatrix += identityMatrix;
    return newMatrix;
}

Matrix Matrix::operator-(const double number) {
    Matrix newMatrix;
    newMatrix = *this;
    Matrix identityMatrix(numCols, numRows);
    identityMatrix.identityFill();

    identityMatrix *= number;
    newMatrix -= identityMatrix;
    return newMatrix;
}

Matrix Matrix::operator-(const Matrix& matrix) {
    if ((numCols != matrix.numCols) || (numRows != matrix.numRows)) {
        throw std::runtime_error("ERROR: the matrix sizes are incomparable\n");
    }

    Matrix newMatrix(numCols, numRows);
    newMatrix = *this;
    newMatrix -= matrix;
    return newMatrix;
}

Matrix Matrix::operator*(const Matrix& matrix) {
    if (numRows != matrix.numCols) {
        throw std::runtime_error("ERROR: the matrix sizes are incomparable\n");
    }

    Matrix newMatrix(numCols, matrix.numRows);
    newMatrix.zerosFill();
    for (size_t currentCol = 0; currentCol < newMatrix.numCols; ++currentCol) {
        for (size_t currentRow = 0; currentRow < newMatrix.numRows; ++currentRow) {

            for (size_t index = 0; index < numRows; ++index) {

                newMatrix.matrixData[newMatrix.numRows * currentCol + currentRow] +=
                    matrixData[numRows * currentCol + index] *
                    matrix.matrixData[matrix.numRows * index + currentRow];
            }
        }
    }
    return newMatrix;
}

Matrix Matrix::operator*(const double number) {
    Matrix newMatrix;
    newMatrix = *this;
    newMatrix *= number;
    return newMatrix;
}

Matrix operator^(const double base, const Matrix& matrix) {
    const unsigned int accuracy = 15;

    if (matrix.numRows != matrix.numCols) {
        throw std::runtime_error("ERROR: the matrix size isn't square\n");
    }

    Matrix logMatrix;
    logMatrix = matrix;
    logMatrix *= log(base);

    Matrix newMatrix(matrix.numCols, matrix.numRows);
    newMatrix.identityFill();

    Matrix subMatrix(matrix.numCols, matrix.numRows);
    subMatrix.identityFill();

    for (unsigned int index = 1; index < accuracy; ++index) {
        subMatrix *= logMatrix;
        subMatrix /= index;
        newMatrix += subMatrix;
    }
    return newMatrix;
}

Matrix Matrix::operator^(const unsigned int exponent) {
    if (numRows != numCols) {
        throw std::runtime_error("ERROR: the matrix size isn't square\n");
    }

    Matrix newMatrix(numCols, numRows);
    newMatrix.identityFill();
    if (exponent == 0) {
        return newMatrix;
    }
    for (unsigned int index = 1; index <= exponent; ++index) {
        newMatrix *= *this;
    }
    return newMatrix;
}

int main() {
    srand(time(NULL));

try {
    // Create two matrices A and B
    Matrix A(3, 3);
    A.fillFactorial();
    std::cout << "Matrix A:\n" << A << std::endl;

    Matrix B(3, 3);
    B.fillFactorial();
    std::cout << "Matrix B:\n" << B << std::endl;

    // Matrix sum
    Matrix C = A + B;
    std::cout << "Matrix C (sum of A and B):\n" << C << std::endl;

    // Matrix difference
    Matrix D = A - B;
    std::cout << "Matrix D (difference of A and B):\n" << D << std::endl;

    // Matrix product
    Matrix E = A * B;
    std::cout << "Matrix E (product of A and B):\n" << E << std::endl;

    // Scalar multiplication
    double scalar = 2.5;
    Matrix F = A * scalar;
    std::cout << "Matrix F (A multiplied by " << scalar << "):\n" << F << std::endl;

    // Matrix raised to power
    int exponent = 2;
    Matrix G = A ^ exponent;
    std::cout << "Matrix G (A raised to power " << exponent << "):\n" << G << std::endl;

    // Matrix exponential
    int terms = 10;
    Matrix H; // Assuming you have a function to compute matrix exponential
    // H = A.exp(terms); // Uncomment this line when you implement exp() function
    std::cout << "Matrix H (exponential of A with " << terms << " terms):\n" << H << std::endl;

    // Determinant of matrix B
    double det = B.determinant();
    std::cout << "Determinant of matrix B: " << det << std::endl;
} catch (const std::invalid_argument& e) {
    std::cerr << "Invalid argument: " << e.what() << std::endl;
} catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}

    return 0;
}
