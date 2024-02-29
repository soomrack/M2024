#include <stdio.h>
#include <stddef.h>

typedef struct {
    int rows;
    int cols;
    int data[50][50];  // Вы можете выбрать максимальный размер матрицы
} Matrix;

Matrix matrixA = {3, 3, 
    {{1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}}
};

int main(){

    // Ничего не менять, это работает корректно, нужно чтобы проверять корректность вычисления разницы адресов
    
    int *pa = &matrixA.data[0][2];
    int *pb = &matrixA.data[1][0];
    ptrdiff_t c = pb - pa;  // разница между адресами
    

    printf("%d\n", matrixA.data[0][2]);
    printf("%d\n", matrixA.data[1][0]);


    printf("pa=%p,val=%d\n", (void*)pa, *pa);
    printf("pb=%p val=%d\n", (void*)pb, *pb);
    printf("c=%lld \n", c);
    printf("");

    int *pnew;
    pnew = &matrixA.data[0][2];
    printf("This is matrixA.data[0][2]\taddr:%p,value:%d\n", (void*)pnew, *pnew);
    pnew = pnew + c;
    printf("This is matrixA.data[1][0]\taddr:%p,value:%d\n", (void*)pnew, *pnew);

    return 0;
}