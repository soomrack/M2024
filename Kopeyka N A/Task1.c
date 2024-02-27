#include <stdio.h>
#include <math.h>

typedef long long int money; // считаем в копейках
struct Person {
    money capital;
    money income;
    money spendings;
    money flat_payment;
    money overpayment;
} Alice, Bob;

const money FLAT_RENT_PRICE = 25 * 1000 * 100;
const money SALARY = 200 * 1000 * 100;
const money START_CAPITAL = 2000 * 1000 * 100;

money FLAT_PRICE = 20 * 1000 * 1000 * 100;
money LIFE_SPENDING = 20 * 1000 * 100;

const int YEARS = 30;
const int START_MONTH = 2;
const int START_YEAR = 2024;

double INFLATION = 0.12;    // ежегодная инфляция
double BANK_RATE = 0.16;    // годовая ставка по вкладу
double MORTGAGE_RATE = 0.10;    // годовая ставка по ипотеке

const money YEAR_BUY_CAR = 2028;    // Стоимость машины
const money CAR_PRICE = 2000 * 1000 * 100;
const money CAR_SPENDINGS = 10 * 1000;
const money CAR_TO = 100 * 1000;


void alice_init() {
    Alice.income = SALARY;
    Alice.capital = 0;
    Alice.spendings = LIFE_SPENDING;
    Alice.flat_payment = (FLAT_PRICE - START_CAPITAL) * (((MORTGAGE_RATE / 12.0) * pow((1 + MORTGAGE_RATE / 12.0), (YEARS * 12.0)))
        / (pow((1 + MORTGAGE_RATE / 12.0), (YEARS * 12.0)) - 1));
}

void bob_init() {
    Bob.income = SALARY;
    Bob.capital = START_CAPITAL;
    Bob.spendings = LIFE_SPENDING;
    Bob.flat_payment = FLAT_RENT_PRICE;
}

void alice_salary(int year, int month) {
    Alice.capital += Alice.income;
}

void alice_deposite(int year, int month) {
    Alice.capital *= (1.0 + BANK_RATE / 12.0);
}

void alice_spendings(int year, int month) {
    Alice.capital -= (Alice.spendings + Alice.flat_payment);
}

void alice_buying_car(int year, int month) {
    if ((year == YEAR_BUY_CAR) && (month == START_MONTH))  // Покупка машины
    {
        Alice.capital -= CAR_PRICE;
        Alice.spendings += CAR_SPENDINGS;
    }
}

void alice_car_TO(int year, int month) {
    if ((year - YEAR_BUY_CAR) % 3 == 0)     // Тех обслуживание раз в 3 года
    {
        Alice.capital -= CAR_TO;
    }
}

void bob_salary(int year, int month) {
    Bob.capital += Bob.income;
}

void bob_deposite(int year, int month) {
    Bob.capital *= (1.0 + BANK_RATE / 12.0);
}

void bob_spendings(int year, int month) {
    Bob.capital -= (Bob.spendings + Bob.flat_payment);
}

void inflation(int year, int month) {
    if (month == 12) {
        //INFLATION *= 1.05;
        Alice.income *= (1.0 + INFLATION);
        Bob.income *= (1.0 + INFLATION);
        Bob.flat_payment *= (1.0 + INFLATION);
        Alice.spendings *= (1.0 + INFLATION);
        Bob.spendings *= (1.0 + INFLATION);
        FLAT_PRICE *= (1.0 + INFLATION);
    }
}

void simulation() {
    int month = START_MONTH;
    int year = START_YEAR;
    while ((year != (START_YEAR + YEARS)) || month != 1)
    {
        alice_deposite(year, month);
        alice_salary(year, month);
        alice_spendings(year, month);

        alice_buying_car(year, month);
        alice_car_TO(year, month);

        bob_deposite(year, month);
        bob_salary(year, month);
        bob_spendings(year, month);

        inflation(year, month);
        month++;

        
        if (month > 12)
        {
            year++;
            month = 1;
        }
    }
}

void print_bob()
{
    printf("Bob Capital: %lld\n", Bob.capital);
    printf("Bob Income: %lld\n", Bob.income);
    printf("Bob Life spendings: ""%lld\n", Bob.spendings);
    printf("Bob Monthly payment: %lld\n", Bob.flat_payment);
}

void print_alice()
{
    printf("Alice Capital: %lld\n", Alice.capital + FLAT_PRICE + CAR_PRICE);
    printf("Alice Income: %lld\n", Alice.income);
    printf("Alice Life spendings: ""%lld\n", Alice.spendings);
    printf("Alice Monthly payment: %lld\n", Alice.flat_payment);
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
