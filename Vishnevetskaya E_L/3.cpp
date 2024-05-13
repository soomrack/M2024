#include <iostream>
#include <iomanip> // Для std::setprecision
#include <vector>
#include <stdexcept> // Для исключений

struct Matrix {
    size_t rows;
    size_t cols;
    std::vector<double> data;
};

unsigned int factorial(unsigned int n) {
    return (n == 0 || n == 1) ? 1 : n * factorial(n - 1);
}

void Matrix_fill_factorial(Matrix &mat) {
    for (size_t i = 0; i < mat.rows; ++i) {
        for (size_t j = 0; j < mat.cols; ++j) {
            unsigned int n = i * mat.cols + j + 1;
            double value = 1.0 / factorial(n);
            mat.data[i * mat.cols + j] = value;
        }
    }
}

void Matrix_print(const Matrix &mat) {
    for (size_t i = 0; i < mat.rows; ++i) {
        for (size_t j = 0; j < mat.cols; ++j) {
           std::cout << std::fixed << std::setprecision(3) << mat.data[i * mat.cols + j] << "\t";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix_Sum(const Matrix &mat1, const Matrix &mat2) {
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        throw std::invalid_argument("Matrix dimensions must be the same for addition.");
    }
    Matrix result{mat1.rows, mat1.cols, std::vector<double>(mat1.rows * mat1.cols)};
    for (size_t i = 0; i < mat1.rows * mat1.cols; ++i) {
    result.data[i] = mat1.data[i] + mat2.data[i];
    }
    return result;
}

Matrix Matrix_Difference(const Matrix &mat1, const Matrix &mat2) {
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        throw std::invalid_argument("Matrix dimensions must be the same for subtraction.");
    }
    Matrix result{mat1.rows, mat1.cols, std::vector<double>(mat1.rows * mat1.cols)};
    for (size_t i = 0; i < mat1.rows * mat1.cols; ++i) {
    result.data[i] = mat1.data[i] - mat2.data[i];
    }
    return result;
}

Matrix Matrix_Product(const Matrix &mat1, const Matrix &mat2) {
    if (mat1.cols != mat2.rows) {
        throw std::invalid_argument("Number of columns of the first matrix must be equal to the number of rows of the second matrix for matrix multiplication.");
    }
    Matrix result{mat1.rows, mat2.cols, std::vector<double>(mat1.rows * mat2.cols)};
    for (size_t i = 0; i < mat1.rows; ++i) {
        for (size_t j = 0; j < mat2.cols; ++j) {
            double sum = 0;
            for (size_t k = 0; k < mat1.cols; ++k) {
                sum += mat1.data[i * mat1.cols + k] * mat2.data[k * mat2.cols + j];
            }
            result.data[i * mat2.cols + j] = sum;
        }
    }
    return result;
}

Matrix Matrix_Scalar_Multiplication(const Matrix &mat, double scalar) {
    Matrix result{mat.rows, mat.cols, std::vector<double>(mat.rows * mat.cols)};
    for (size_t i = 0; i < mat.rows * mat.cols; ++i) {
    result.data[i] = mat.data[i] * scalar;
    }
    return result;
}

Matrix Matrix_Division(const Matrix &mat1, const Matrix &mat2) {
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        throw std::invalid_argument("Matrix dimensions must be the same for division.");
    }
    Matrix result{mat1.rows, mat1.cols, std::vector<double>(mat1.rows * mat1.cols)};
    for (size_t i = 0; i < mat1.rows; ++i) {
        for (size_t j = 0; j < mat1.cols; ++j) {
            if (mat2.data[i * mat1.cols + j] == 0) {
                throw std::invalid_argument("Division by zero encountered.");
            }
            result.data[i * mat1.cols + j] = mat1.data[i * mat1.cols + j] / mat2.data[i * mat1.cols + j];
        }
    }
    return result;
}

Matrix Matrix_Identity(size_t size) {
    Matrix identity{size, size, std::vector<double>(size * size, 0.0)};
    for (size_t i = 0; i < size; ++i) {
        identity.data[i * size + i] = 1.0;
    }
    return identity;
}

Matrix Matrix_Power(const Matrix &mat, int power) {
    if (mat.rows != mat.cols) {
        throw std::invalid_argument("Matrix must be square to be raised to a power.");
    }
    if (power == 0) {
        return Matrix_Identity(mat.rows);
    }
    if (power < 0) {
        throw std::invalid_argument("Matrix exponentiation with negative powers is not supported.");
    }
    Matrix result = mat;
    for (int i = 1; i < power; ++i) {
        result = Matrix_Product(result, mat);
    }
    return result;
}

Matrix Matrix_Exponential(const Matrix &mat, int power) {
    if (mat.rows != mat.cols) {
        throw std::invalid_argument("Matrix must be square to compute its exponential.");
    }
    if (power < 0) {
        throw std::invalid_argument("Matrix exponentiation with negative powers is not supported.");
    }
    // Используем ряд Тейлора для вычисления экспоненты матрицы
    Matrix result{mat.rows, mat.cols, std::vector<double>(mat.rows * mat.cols, 0.0)};
    Matrix term = mat;
    for (int i = 0; i <= power; ++i) {
        result = Matrix_Sum(result, term);
        term = Matrix_Product(term, mat);
        term = Matrix_Scalar_Multiplication(term, 1.0 / (i + 1));
    }
    return result;
}

double Matrix_Determinant(const Matrix &mat) {
    if (mat.rows != mat.cols) {
        throw std::invalid_argument("Matrix must be square to find its determinant.");
    }

    if (mat.rows == 1) {
        // Для матрицы 1x1 детерминант равен ее единственному элементу
        return mat.data[0];
    }

    double determinant = 0;
    int sign = 1;

    for (size_t i = 0; i < mat.cols; ++i) {
        // Создаем подматрицу без строки 0 и столбца i
        Matrix subMat{mat.rows - 1, mat.cols - 1, std::vector<double>((mat.rows - 1) * (mat.cols - 1), 0.0)};
        for (size_t row = 1; row < mat.rows; ++row) {
            for (size_t col = 0; col < mat.cols; ++col) {
                if (col < i) {
                    subMat.data[(row - 1) * (mat.cols - 1) + col] = mat.data[row * mat.cols + col];
                } else if (col > i) {
                    subMat.data[(row - 1) * (mat.cols - 1) + (col - 1)] = mat.data[row * mat.cols + col];
                }
            }
        }
        determinant += sign * mat.data[i] * Matrix_Determinant(subMat);
        sign = -sign;
    }

    return determinant;
}

int main() {
    size_t rows = 3;
    size_t cols = 3;

    Matrix mat1{rows, cols, std::vector<double>(rows * cols)};
    Matrix mat2{rows, cols, std::vector<double>(rows * cols)};
    Matrix mat3{2, 2, std::vector<double>(rows * cols)};

    Matrix_fill_factorial(mat1);
    Matrix_fill_factorial(mat2);
    Matrix_fill_factorial(mat3);

    std::cout << "matrix1:" << std::endl;
    Matrix_print(mat1);
    std::cout << std::endl;
    std::cout << "matrix2:" << std::endl;
    Matrix_print(mat2);
    std::cout << std::endl;

    try {
        Matrix sum = Matrix_Sum(mat1, mat2);
        std::cout << "Sum:" << std::endl;
        Matrix_print(sum);
        std::cout << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error during addition: " << e.what() << std::endl;
    }

    try {
        Matrix diff = Matrix_Difference(mat1, mat2);
        std::cout << "Difference:" << std::endl;
        Matrix_print(diff);
        std::cout << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error during subtraction: " << e.what() << std::endl;
    }

    try {
        Matrix prod = Matrix_Product(mat1, mat2);
        std::cout << "Product:" << std::endl;
        Matrix_print(prod);
        std::cout << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error during multiplication: " << e.what() << std::endl;
    }

    double scalar = 2.0;
    try {
        Matrix scaledMat = Matrix_Scalar_Multiplication(mat1, scalar);
        std::cout << "Scalar multiplication:" << std::endl;
        Matrix_print(scaledMat);
        std::cout << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error during scalar multiplication: " << e.what() << std::endl;
    }


    try {
        Matrix div = Matrix_Division(mat1, mat2);
        std::cout << "Division:" << std::endl;
        Matrix_print(div);
        std::cout << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error during division: " << e.what() << std::endl;
    }
    
    std::cout << "matrix3:" << std::endl;
    Matrix_print(mat3);
    std::cout << std::endl;
    try {
            Matrix matSquared = Matrix_Power(mat3, 2);
            std::cout << "Matrix power:" << std::endl;
            Matrix_print(matSquared);
            std::cout << std::endl;
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error during matrix exponentiation: " << e.what() << std::endl;
        }

    try {
            double determinant = Matrix_Determinant(mat3);
            std::cout << "Determinant of the matrix:" << determinant << std::endl;
            std::cout << std::endl;
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error during determinant calculation: " << e.what() << std::endl;
        }

    try {
            Matrix matExp = Matrix_Exponential(mat3, 3);
            std::cout << "Matrix exponential:" << std::endl;
            Matrix_print(matExp);
            std::cout << std::endl;
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error during matrix exponentiation: " << e.what() << std::endl;
        }

    return 0;
}
