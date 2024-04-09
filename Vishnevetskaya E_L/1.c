
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef long long int money; // считаем в копейках

// Структура, описывающая человека
struct Person {
    money income;           // Зарплата
    money expenses;         // Расходы
    money deposit;          // Вклад
    money flat_pay;         // Оплата по ипотеке/аренды
    money capital;
} Alice, Bob;

double MONTH_INFLATION_RENT = 0.12/12;
money APARTMENT_PRICE = 2000000000;
money START_CAP = 2000000000;
double IPOTEKA_RATE = 0.12;     // Процентная ставка ипотеки
double DEPOSIT_RATE = 0.16;     // Процентная ставка вклада
static money SALARY = 30000000;

int YEARS = 30;
int START_YEEAR = 2024;
int START_MONTH = 2;

// Создаем человека и инициализируем его данные
void AliceInit() {
    Alice.income = SALARY;
    Alice.capital = 0;
    Alice.expenses = 12000000;
}

void BobInit() {
    Bob.income = SALARY;
    Bob.capital = 0;
    Bob.expenses = 4500000;
}


// Функция для оплаты ипотеки
//void payIpoteka() {
   //Alice.savings = Alice.income - Alice.expenses;
   //Alice.ipoteka_dolg -= Alice.savings;
   //Alice.ipoteka_dolg *= 1 + Alice.ipoteka_rate/12;
   //Alice.savings = 0;
//}

void payIpoteka() {
    Alice.flat_pay = (APARTMENT_PRICE - START_CAP) * (((IPOTEKA_RATE / 12.0) * pow((1 + IPOTEKA_RATE / 12.0), (YEARS * 12.0)))
        / (pow((1 + IPOTEKA_RATE / 12.0), (YEARS * 12.0)) - 1));
}


void AliceSpendings() {
    Alice.expenses += Alice.flat_pay;
}

void AliceSalary() {
    Alice.capital += Alice.income;
}


void BobSalary() {
    Bob.capital += Bob.income;
}


// Функция для инвестирования денег на вклад
void investDeposit() {
   Bob.deposit += Bob.capital;
   Bob.deposit *= 1 + DEPOSIT_RATE/12;
   Alice.deposit += Alice.capital;
   Alice.deposit *= 1 + DEPOSIT_RATE/12;
}

void inflation(){
    Alice.expenses *= (1 + MONTH_INFLATION_RENT);
    Alice.income *= (1 + MONTH_INFLATION_RENT);
    Bob.expenses *= (1 + MONTH_INFLATION_RENT);
    Bob.income *= (1 + MONTH_INFLATION_RENT);
    APARTMENT_PRICE *= (1 + MONTH_INFLATION_RENT);
    
}

void PrintAlice() {   
    printf("Alice money ost: %lld rub\n", Alice.capital);
}

void PrintBob() {   
    printf("Bob money ost: %lld rub\n", Bob.capital);
}


//void SomeBad(int month,int year, int bad_month, int bad_year){
//    if (month == bad_month && year == bad_year)
//    {
//        
//    }
//}

// Функция 
void Simulation(int start_year, int end_year) {

    // Цикл по годам и месяцам
    for (int year = start_year; year <= end_year; ++year) {

        for (int month = 1; month <= 12; ++month) {
            //SomeBad(month,year,9,2045)

            if (Alice.flat_pay>0){
                payIpoteka();
            }
            else{
                Alice.flat_pay = 0;
            }

            AliceSalary ();
            AliceSpendings();
            BobSalary();
            investDeposit();
            inflation();
        }
    }

    // Боб купил квартиру
   // Bob.capital = Bob.deposit- APARTMENT_PRICE;

}



int main() {


    AliceInit();
    BobInit();

    Simulation(2024,2054);
   
    PrintAlice();
    PrintBob();

    return 0;
}
