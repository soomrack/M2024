#include <stdio.h>
#include <math.h>

typedef long long int money;  // перевод всех денег в копейки

//---------------------------------------------------СТРУКТУРЫ---------------------------------------------------//
struct Person
{
    money capital;             // накопленный капитал
    money income;              // заработок
    money life_spendings;      // траты на жизнь
    money payment_monthly;     // проживание в квартире или погашение ипотеки в течение месяца
};                             

struct Person Alice;
struct Person Bob;

//---------------------------------------------------ПЕРЕМЕННЫЕ---------------------------------------------------//
const int START_MONTH = 9;                      // начальный месяц
const int START_YEAR = 2023;                    // начальный год
const int LAST_MONTH = 8;                       // конечный месяц
const money PRICE = 20 * 1000 * 1000 * 100;     // стоимость квартиры 
const money SALARY = 200 * 1000 * 100;          // зарплата Алисы и Боба
const money START_CAPITAL = 1000 * 1000 * 100;  // стартовый капитал Алисы, первоначальный взнос Боба
const int YEARS = 20;                           
const double INFLATION = 0.07;                  // инфляция в год 
double BANK_RATE = 0.09;                        // годовая ставка в банке
const money LIFE_SPENDINGS = 25 * 1000 * 100;   // обыкновенные траты на жизнь

//-------------------------------------------------ИНИЦИАЛИЗАЦИЯ---------------------------------------------------//
void alice_init()
{
    Alice.income = SALARY;
    Alice.capital = START_CAPITAL;
    Alice.life_spendings = LIFE_SPENDINGS;
    Alice.payment_monthly = 40 * 1000 * 100;
}

void bob_init()
{
    Bob.income = SALARY;
    Bob.capital = 0;
    Bob.life_spendings = LIFE_SPENDINGS;
    Bob.payment_monthly = (PRICE - START_CAPITAL) * (BANK_RATE / 12.0) / (1 - powf((1 + BANK_RATE / 12.0), (1.0 - YEARS * 12.0)));
}

//-----------------------------------------------------ЗАРПЛАТА-----------------------------------------------------//
void alice_income(const int year, const int month)
{
    if (month == 11) {
        return;
    }
    Alice.capital += Alice.income;
    if (month == 12) {
        Alice.income = Alice.income * (1.0 + INFLATION / 2.0);       // инфляция влияет и на зп тоже  :(
    }
}

void bob_income(const int year, const int month)
{
    if (month == 12) {
        Bob.capital += (money)(Bob.income * 1.2);
    }
    Bob.capital += Bob.income;
    if (month == 12) {
        Bob.income = Bob.income * (1.0 + INFLATION / 2.0);
    }
}

//-------------------------------------------ПРИБАВКА ПО ДЕПОЗИТУ В БАНКЕ------------------------------------------//
void alice_deposite_income(const int year, const int month)
{
    if (year == START_YEAR && month == START_MONTH) {
        return;
    }
    if (year == 2033 && month == 1) {
        BANK_RATE = 0.1;
    }
    Alice.capital = Alice.capital * (1.0 + BANK_RATE / 12.0);
}

void bob_deposite_income(const int year, const int month)
{
    if (year == 2033 && month == 1) {
        BANK_RATE = 0.1;
    }
    Bob.capital = Bob.capital * (1.0 + BANK_RATE / 12.0); 
}

//------------------------------------------------------РАСХОДЫ---------------------------------------------------//
void alice_life_spending(const int year, const int month)
{
    if (month == 12) {
        Alice.capital -= Alice.life_spendings * 0.5;
    }
    Alice.capital -= Alice.life_spendings;
}

void bob_life_spending(const int year, const int month)
{
    if (month == 12) {
        Bob.capital -= Bob.life_spendings * 0.5;
    }
    Bob.capital -= Bob.life_spendings;
}

//---------------------------------------------------ЕЖЕМЕСЯЧНЫЙ ПЛАТЕЖ---------------------------------------------------//
void alice_payment(const int year, const int month)
{
    Alice.capital -= Alice.payment_monthly;
}

void bob_payment(const int year, const int month)
{
    Bob.capital -= Bob.payment_monthly;
}

//------------------------------------------------ВЛИЯНИЕ ИНФЛЯЦИИ НА ТРАТЫ------------------------------------------------//
void life_inflation(const int year, const int month)
{
    if (month % 3 == 0 && month > 2) {
        Alice.life_spendings += Alice.life_spendings * INFLATION / 4.0;
        Bob.life_spendings += Bob.life_spendings * INFLATION / 4.0;
    }
}

//------------------------------------------------ВЛИЯНИЕ ИНФЛЯЦИИ НА АРЕНДУ------------------------------------------------//
void estate_inflation(const int year, const int month)
{
    if (month == 12) {
        Alice.payment_monthly = Alice.payment_monthly * (1.0 + INFLATION / 2.0);
    }
}

//------------------------------------------------------СИМУЛЯЦИЯ---------------------------------------------------//
void simulation()
{
    int month = START_MONTH;
    int year = START_YEAR;
    int last_year = START_YEAR + YEARS;

    while(!(year == last_year && month == LAST_MONTH)) {

        alice_deposite_income(year, month);
        alice_income(year, month);
        alice_life_spending(year, month);
        alice_payment(year, month);

        bob_deposite_income(year, month);
        bob_income(year, month);
        bob_life_spending(year, month);
        bob_payment(year, month);

        life_inflation(year, month);
        estate_inflation(year, month);
        
        month++;
        if (month > 12) {
            year++;
            month = 1;
        }
        
    }
}

//------------------------------------------------------ВЫВОД---------------------------------------------------//
void print()
{
    double alice_capital = Alice.capital;
    double bob_capital = Bob.capital;
    alice_capital /= 100;                                   // перевод обратно в целые деньги
    bob_capital /= 100;
    printf("Alice Capital: %f \n", (alice_capital));
    printf("Bob Capital: %f \n", (bob_capital)); 
}


int main() 
{
    alice_init();
    bob_init();

    simulation();

    print();

    return 0;
}