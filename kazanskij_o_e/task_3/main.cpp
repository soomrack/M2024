#include <iostream>
#include <cstring>
#include <cmath>


using namespace std;

class Matrix{
    public:
        /*constract class*/
        Matrix(uint16_t cols, uint16_t rows){
            this->cols = cols;
            this->rows = rows;
            this->matrix_init();
        }
        /*destract class*/
        ~Matrix(){
            free(this->data);
            this->data = NULL; 
            cout << "deleted" << endl;
        }
        /*convert to ident matrix*/
        void create_ident_matrix(){
            if(this->data == NULL){
                this->error_message("В матрице отсуствует данные");
                return;
            }

            if(this->cols != this->rows){
                this->error_message("Ваша матрица не квадратичная");
                return;
            }

            memset(this->data, 0, this->cols * this->rows * sizeof(MatrixItem));
            for(size_t i = 0; i < this->rows * this->cols; i += this->cols + 1){
                this->data[i] = 1;
            }
        }
        /*fill the matrix random values*/
        void rand_fill(){
            if(this->data == NULL){
                this->error_message("В матрице нет данных");
                return;
            }
            for(size_t i = 0; i < this->rows * this->cols; i++){
                this->data[i] = (int)rand() % 10;
            }
        }
        /*swap cols and rows in the matrix*/
        void transperant(){
            this->buffer_init(this->rows, this->cols);

            if(this->data == NULL || this->buf == NULL){
                this->error_message("Произошла ошибка транспонирования");
                return;
            }

            for(size_t rowA = 0; rowA < this->rows; rowA++){
                for(size_t colA = 0; colA < this->cols; colA++){
                    this->buf[this->rows * colA + rowA] = this->data[colA + rowA * this->cols];
                }
            }
            this->copy(this->cols * this->rows * sizeof(MatrixItem));
        }
        /*get the determinate of matrix*/
        double get_det(){
            if(this->cols != this->rows){
                printf("Вычисление определителя матрицы: Невозможно\n Ваша матрица не квадратичная");
                return NAN;
            }

            switch (this->cols)
            {
            case 1:{
                return this->data[0];
            }

            case 2:{
                return this->data[0] * this->data[3] - this->data[1] * this->data[2];
            }

            case 3:{ 
                double matr_det = this->data[0] * this->data[4] * this->data[8] + this->data[1] * this->data[5] * this->data[6] + this->data[3] * this->data[7] * this->data[2];
                matr_det -= ((this->data[2]) * (this->data[4]) * (this->data[6]) + (this->data[1]) * (this->data[3]) * (this->data[8]) + (this->data[0]) * (this->data[5]) * (this->data[7]));
                return matr_det;
            }

            default:{
                return NAN;
            }
            }

        }      
        /*print the matrix*/
        void print(){
            cout << "=====================\n" << endl;
            if(this->data == NULL){
                this->error_message("Пусто");
                return;
            }

            for(size_t i = 0; i < this->rows * this->cols; i++){
                cout << this->data[i] << "\t";
                if((i + 1) % this->cols == 0){cout << "\n";}
            }
            printf("\n");
        }
        /* A + B */
        void operator+ (const Matrix& B){
            if(this->data == NULL || B.data == NULL){
                this->error_message("Невозожно сложить матрицу.\nПроверьте матрицы на наличие данных");
                return;
            }
            if(this->cols != B.cols || this->rows != B.rows){
                this->error_message("Ваши матрицы разных размеров");
                return;
            }
            for(size_t i = 0; i < this->cols * this->rows; i++){
                this->data[i] += B.data[i];
            }
        }
        void operator+= (const Matrix& B){
            this->operator+(B);
        }
        /* A - B */
        void operator- (const Matrix& B){
            if(this->data == NULL || B.data == NULL){
                this->error_message("Невозожно сложить матрицу.\nПроверьте матрицы на наличие данных");
                return;
            }
            if(this->cols != B.cols || this->rows != B.rows){
                this->error_message("Ваши матрицы разных размеров");
                return;
            }
            for(size_t i = 0; i < this->cols * this->rows; i++){
                this->data[i] -= B.data[i];
            }
        }
        void operator-= (const Matrix& B){
            this->operator-(B);
        }
        /* A * k */
        void operator* (const double k){
            if(this->data == NULL){
                this->error_message("в вашей матрице нет данных");
                return;
            }
            for(size_t i = 0; i < this->cols * this->rows; i++){
                this->data[i] *= k;
            }
        }
        void operator*= (const double k){
            this->operator*(k);
        }
        /* A * B */
        void operator* (const Matrix& B){
            if(this->cols != B.rows){
                this->error_message("Ваши матрицы не верной размерности");
                return;
            }

            if(this->data == NULL || B.data == NULL){
                this->error_message("Ваши матрицы пустые");
                return;
            }

            this->buffer_init(B.rows, this->cols);

            if(this->buf == NULL){
                this->error_message("Произошла ошибка умножение матриц");
            }

            for(size_t rowsA = 0; rowsA < this->rows; rowsA++){
                for(size_t colsB = 0; colsB < B.cols; colsB++){
                    for(size_t i = 0; i < this->cols; i++){
                        this->buf[rowsA * this->cols + colsB] += this->data[rowsA * this->cols + i] * B.data[i * B.cols + colsB];
                    }
                }
            }
            this->copy(this->cols * B.rows * sizeof(MatrixItem));
            
        }
        void operator*= (const Matrix& B){
            this->operator*(B);
        }
        /*left = rigth*/
        Matrix& operator= (const Matrix& rigth){
            if(&rigth == this) return *this;
            this->cols = rigth.cols;
            this->rows = rigth.rows;
            this->data = rigth.data;
        }

    private:
        typedef double MatrixItem;
        uint16_t cols = 0, rows = 0;
        MatrixItem *data = NULL, *buf = NULL;
        void error_message(const char msg[]){printf("%s\n", msg);}
        void matrix_init(){
            if(this->cols == 0 || this->rows == 0){
                this->error_message("Ваша матрица не имеет ячеек");
                return;
            }

            if(rows >= SIZE_MAX / sizeof(MatrixItem) / cols){
                this->error_message("Недостаточно места для хранения данных");
                return;
            }

            this->data = (MatrixItem*)malloc(this->cols * this->rows * sizeof(MatrixItem));
            if(this->data == NULL){
                this->error_message("Ошибка выделения памяти для хранения матрицы");
            }
            memset(this->data, 0, this->cols * this->rows * sizeof(MatrixItem));
        }
        void buffer_init(uint16_t rows, uint16_t cols){
            this->buf = (MatrixItem*)malloc(cols * rows * sizeof(MatrixItem));
            if(this->buf == NULL) return;
            memset(this->buf, 0, cols * rows * sizeof(MatrixItem));
        }
        void copy(size_t size){
            memcpy(this->data, this->buf, size);
            free(this->buf);
        }

};


int main(int argc, char **argv){
    system("chcp 65001");
    Matrix m(2, 2), b(2, 2), c(2,2);
    m.rand_fill();
    b.rand_fill();
    m.print();
    b.print();
    m += b;
    m.print();
    b.print();
    b *= m;
    b.print();
    b.transperant();
    printf("b\n");
    b.print();
    c = b;
    printf("c\n");
    c.print();
    return 0;
}
