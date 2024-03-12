#include <iostream>
#include "Matrix.hpp"

int get_rand_integer(int start = 0, int end = 20) {

    if (start > end) {
        std::swap(start, end);
    }

    return rand() % (end - start + 1) + start;
}


void init_matrix_as_random(Matrix* M) {
    size_t rows = M->get_rows();
    size_t cols = M->get_cols();

    for (size_t idx = 0; idx < rows * cols; idx++)
    {
        (*M)[idx] = get_rand_integer();
    }
}


int main()
{
    std::cout << "step by step I'm growing :)\n";

    srand(time(NULL));

    Matrix A(6, 4);
    init_matrix_as_random(&A);
    
    std::cout << A << std::endl;


    Matrix B(6, 4);
    B.set_as_identity();
    std::cout << B << std::endl;

    Matrix C;
    C = A + B;
    std::cout << C << std::endl;

    return 0;
}
