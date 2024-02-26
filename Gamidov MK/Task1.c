#include <stdio.h>
#include <math.h>

// Считаем деньги в копейках
typedef long long int money;

struct Person
{
    money capital;          // общий капитал
    money income;           // зарплата
    money spendings;        // бытовые траты
    money flat_payment;     // траты на квартиру
    money overpayment;      // переплата при покупке квартиры
} Alice, Bob;


const money PRICE = 20 * 1000 * 1000 * 100; // стоимость квартиры
const money SALARY = 200 * 1000 * 100;       // зарплата
const money START_CAPITAL = 1000 * 1000 * 100;      // стартовый капитал
const int YEARS = 20;
const money SPENDINGS = 25 * 1000 * 100;        // траты на жизнь
const int START_MONTH = 2;                      // начальные временные условия
const int START_YEAR = 2024;

double inflation = 0.07;                   // инфляция
double bank_rate = 0.09;                   // ставка в банке


void alice_init()
{
    Alice.income = SALARY;
    Alice.capital = START_CAPITAL;
    Alice.spendings = SPENDINGS;
    Alice.flat_payment = 40 * 1000 * 100;
    Alice.overpayment = PRICE * pow((1.0 + inflation), YEARS) - PRICE;
}

// Прибавка к капиталу от зарплаты
void alice_salary_income(int year, int month)
{
    Alice.capital += Alice.income;
}

// Ежемесячная прибавка к капиталу от депозита в банке
void alice_deposite_income(int year, int month)
{
    Alice.capital = Alice.capital * (1.0 + bank_rate / 12.0);
}

// Расходы в месяц (жизнь + квартира)
void alice_all_spendings(int year, int month)
{
    Alice.capital = Alice.capital - Alice.spendings - Alice.flat_payment;
}


void bob_init()
{
    Bob.income = SALARY;
    Bob.capital = 0;
    Bob.spendings = SPENDINGS;
    Bob.flat_payment = ((PRICE - START_CAPITAL) * (bank_rate / 12.0) * pow((1 + bank_rate / 12.0), (YEARS * 12.0)))
        / (pow((1 + bank_rate / 12.0), (YEARS * 12.0)) - 1);
    Bob.overpayment = Bob.flat_payment * 12 * YEARS - PRICE;
}

// Прибавка к капиталу от зарплаты
void bob_salary_income(int year, int month)
{
    Bob.capital += Bob.income;
}

// Прибавка к капиталу от депозита в банке
void bob_deposite_income(int year, int month)
{
    Bob.capital = Bob.capital * (1.0 + bank_rate / 12.0);
}

// Расходы в месяц (жизнь + квартира)
void bob_usual_spendings(int year, int month)
{
    Bob.capital = Bob.capital - Bob.spendings - Bob.flat_payment;
}

// Влияние инфляции на зарплату и траты
void annual_inflation(int year, int month)
{
    if (month == 12)
    {
        inflation *= 1.05;  // Предположим, что инфляция растет каждый год на 5 процентов
        Alice.income = Alice.income * (1.0 + inflation);
        Alice.flat_payment = Alice.flat_payment * (1.0 + inflation);
        Bob.income = Bob.income * (1.0 + inflation);
        Bob.spendings = Bob.spendings * (1.0 + inflation);
        Alice.spendings = Alice.spendings* (1.0 + inflation);
    }
}


void simulation()
{
    int month = START_MONTH;
    int year = START_YEAR;

    while ((year != (START_YEAR + YEARS)) || month != 1) {

        alice_deposite_income(year, month);
        alice_salary_income(year, month);
        alice_all_spendings(year, month);

        bob_deposite_income(year, month);
        bob_salary_income(year, month);
        bob_usual_spendings(year, month);

        annual_inflation(year, month);

        month++;
        if (month > 12) {
            year++;
            month -= 12;
        }

    }
}

void print_bob()
{
    printf("Bob Capital: %lld\n", Bob.capital);
    printf("Bob Income: %lld\n", Bob.income);
    printf("Bob Life spendings: ""%lld\n", Bob.spendings);
    printf("Bob Monthly payment: %lld\n", Bob.flat_payment);
    printf("Bob Overpayment: ""%lld\n", Bob.overpayment);
}

void print_alice()
{
    printf("Alice Capital: %lld\n", Alice.capital);
    printf("Alice Income: %lld\n", Alice.income);
    printf("Alice Life spendings: ""%lld\n", Alice.spendings);
    printf("Alice Monthly payment: %lld\n", Alice.flat_payment);
    printf("Alice Overpayment: ""%lld\n", Alice.overpayment);
    printf("\n");
}

int main()
{
    alice_init();
    bob_init();

    simulation();

    print_alice();
    print_bob();

    return 0;
}
