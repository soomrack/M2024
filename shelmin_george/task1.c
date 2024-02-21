#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

typedef long long int Money;  // копейки

int TOTAL_YEARS = 30;
double INFLATION = 12 * 0.01;

struct Person {
    int if_has_apartment = 0;
    Money apartment_price;
    Money savings;  // сумма денег на вкладе
    int loan_period;  // в годах

    // в месяц
    Money salary;
    Money food_spending;
    Money rent;
    Money loan_payment;  // вычисляется в функции compute_loan_payment
    Money additional_spendings;

    // в год
    Money apartment_repairs;
    double loan_rate;
    double deposit_rate;
};

struct Person Bob;
struct Person Alice;

void calculate_loan_payment(struct Person*);  // вычисление ежемесячной оплаты кредита

void init_Bob();
void Bob_indexation(int, int);  // ежегодное повышение зарплаты
void Bob_get_salary(int, int);  // ежемесячное начисление зарплаты
void Bob_buy_food(int, int);  // ежемесячная покупка еды
void Bob_pay_rent(int, int);  // ежемесячная оплата аренды
void Bob_pay_loan(int, int);  // ежемесячная плата по кредиту
void Bob_pay_additional(int, int);  // ежемесячный учет дополнительных расходов
void Bob_repair_apartment(int, int);  // ежегодный ремонт квартиры
void Bob_deposit_income(int, int);  //  ежемесячная выплата процентов по вкладу
void Bob_buy_apartment_if_has_not();
void Bob_print_results();  // Вывод результатов Боба

void init_Alice();
void Alice_indexation(int, int);  // ежегодное повышение зарплаты
void Alice_get_salary(int, int);  // ежемесячное начисление зарплаты
void Alice_buy_food(int, int);  // ежемесячная покупка еды
void Alice_pay_rent(int, int);  // ежемесячная оплата аренды
void Alice_pay_loan(int, int);  // ежемесячная плата по кредиту
void Alice_pay_additional(int, int);  // ежемесячный учет дополнительных расходов
void Alice_repair_apartment(int, int);  // ежегодный ремонт квартиры
void Alice_deposit_income(int, int);  //  ежемесячная выплата процентов по вкладу
void Alice_buy_apartment_if_has_not();  
void Alice_print_results();  // Вывод результатов Алисы

void inflate(int, int);  // ежемесячный рост цен
void calculate();  // вычисление результатов


void calculate_loan_payment(struct Person* person)
{
    double numerator = pow(1 + person->loan_rate / 12, person->loan_period * 12);
    double denominator = numerator - 1;
    person->loan_payment = (Money)((person->apartment_price - person->savings) 
        * person->loan_rate / 12 * numerator / denominator);

    if (person->loan_rate == 0) {
        person->loan_payment = 0;
        person->if_has_apartment = 0;
    }
    else
    {
        person->if_has_apartment = 1;
    }

    if (person->loan_payment > person->salary)
    {
        printf("Программа остановлена: Плата по кредиту больше заработной платы");
        exit(0);
    }
}


void init_Bob()
{
    Bob.if_has_apartment = 0;
    Bob.apartment_price = 20 * 1000 * 1000 * 100;
    Bob.savings = 2 * 1000 * 1000 * 100;
    Bob.loan_period = 0;
    Bob.salary = 300 * 1000 * 100;
    Bob.food_spending = 15000 * 100;
    Bob.rent = 25000 * 100;
    Bob.additional_spendings = 0;
    Bob.apartment_repairs = 0;
    Bob.loan_rate = 0;
    Bob.deposit_rate = 16 * 0.01;

    calculate_loan_payment(&Bob);
}


void init_Alice()
{
    Alice.if_has_apartment = 0;
    Alice.apartment_price = 20 * 1000 * 1000 * 100;
    Alice.savings = 2 * 1000 * 1000 * 100;
    Alice.loan_period = 30;
    Alice.salary = 300 * 1000 * 100;
    Alice.food_spending = 15000 * 100;
    Alice.rent = 0;
    Alice.additional_spendings = 0;
    Alice.apartment_repairs = 100 * 1000 * 100;
    Alice.loan_rate = 18 * 0.01;
    Alice.deposit_rate = 16 * 0.01;

    calculate_loan_payment(&Alice);
}


void Bob_indexation(int month, int year)
{
    Bob.salary += Bob.salary * INFLATION;
}


void Alice_indexation(int month, int year)
{
    Alice.salary += Alice.salary * INFLATION;
}


void Bob_get_salary(int month, int year)
{
    Bob.savings += Bob.salary;

    if (month == 12) {
        Bob_indexation(year, month);
    }
}


void Alice_get_salary(int month, int year)
{
    Alice.savings += Alice.salary;

    if (month == 12) {
        Alice_indexation(year, month);
    }
}


void Bob_buy_food(int month, int year)
{
    Bob.savings -= Bob.food_spending;
}


void Alice_buy_food(int month, int year)
{
    Alice.savings -= Alice.food_spending;
}


void Bob_pay_rent(int month, int year)
{
    Bob.savings -= Bob.rent;
}


void Alice_pay_rent(int month, int year)
{
    Alice.savings -= Alice.rent;
}


void Bob_pay_loan(int month, int year)
{
    Bob.savings -= Bob.loan_payment;
}


void Alice_pay_loan(int month, int year)
{
    Alice.savings -= Alice.loan_payment;
}


void Bob_pay_additional(int month, int year)
{
    Bob.savings -= Bob.additional_spendings;
}


void Alice_pay_additional(int month, int year)
{
    Alice.savings -= Alice.additional_spendings;
}


void Bob_repair_apartment(int month, int year)
{
    Bob.savings -= Bob.apartment_repairs;
}


void Alice_repair_apartment(int month, int year)
{
    Alice.savings -= Alice.apartment_repairs;
}


void Bob_deposit_income(int month, int year)
{
    long double income = Bob.savings * Bob.deposit_rate / 12;
    Bob.savings += income;
}


void Alice_deposit_income(int month, int year)
{
    long double income = Alice.savings * Alice.deposit_rate / 12;
    Alice.savings += income;
}


void Bob_buy_apartment_if_has_not()
{
    if (!Bob.if_has_apartment) {
        if (Bob.savings >= Bob.apartment_price) {
            Bob.savings -= Bob.apartment_price;
            Bob.if_has_apartment = 1;
        }
    }
}


void Alice_buy_apartment_if_has_not()
{
    if (!Alice.if_has_apartment) {
        if (Alice.savings >= Alice.apartment_price) {
            Alice.savings -= Alice.apartment_price;
            Alice.if_has_apartment = 1;
        }
    }
}


void inflate(int month, int year)
{
    Bob.food_spending += Bob.food_spending * INFLATION / 12;
    Bob.additional_spendings += Bob.additional_spendings * INFLATION / 12;

    Alice.food_spending += Alice.food_spending * INFLATION / 12;
    Alice.additional_spendings += Alice.additional_spendings * INFLATION / 12;
}


void Bob_print_results()
{
    Money Bob_savings_roubles = Bob.savings / 100;
    long long int count_digits = 1;
    while (count_digits < Bob_savings_roubles) {
        count_digits *= 1000;
    }
    count_digits /= 1000;

    printf("\nНакопления Боба: ");

    printf("%lld ", Bob_savings_roubles / count_digits);
    Bob_savings_roubles %= count_digits;
    count_digits /= 1000;

    for (count_digits; count_digits > 0; count_digits /= 1000) {
        int number_to_print = Bob_savings_roubles / count_digits;
        printf("%03d ", number_to_print);
        Bob_savings_roubles %= count_digits;
    }
    printf("рублей %02lld копеек", Bob.savings % 100);
}


void Alice_print_results()
{
    Money Alice_savings_roubles = Alice.savings / 100;
    long long int count_digits = 1;
    while (count_digits < Alice_savings_roubles) {
        count_digits *= 1000;
    }
    count_digits /= 1000;

    printf("\nНакопления Алисы: ");

    printf("%lld ", Alice_savings_roubles / count_digits);
    Alice_savings_roubles %= count_digits;
    count_digits /= 1000;

    for (count_digits; count_digits > 0; count_digits /= 1000) {
        int number_to_print = Alice_savings_roubles / count_digits;
        printf("%03d ", number_to_print);
        Alice_savings_roubles %= count_digits;
    }
    printf("рублей %02lld копеек", Alice.savings % 100);
}


void calculate()
{
    time_t time_ = time(NULL);
    struct tm time_now = *localtime(&time_);

    int starting_year = time_now.tm_year + 1900;
    int starting_month = time_now.tm_mon + 1;
    int year = starting_year;
    int month = starting_month;

    while (!(year == starting_year + TOTAL_YEARS && month == starting_month)) {
        Bob_get_salary(year, month);
        Bob_buy_food(year, month);
        Bob_pay_rent(year, month);
        Bob_pay_loan(year, month);
        Bob_pay_additional(year, month);
        Bob_deposit_income(year, month);
        Bob_buy_apartment_if_has_not();

        Alice_get_salary(year, month);
        Alice_buy_food(year, month);
        Alice_pay_rent(year, month);
        Alice_pay_loan(year, month);
        Alice_pay_additional(year, month);
        Alice_deposit_income(year, month);
        Alice_buy_apartment_if_has_not();

        inflate(year, month);

        if (month == 12) {
            Bob_repair_apartment(year, month);
            Alice_repair_apartment(year, month);
        }

        if (month != 12) {
            month += 1;
        }
        else {
            month = 1;
            year += 1;
        }

    }
}


int main()
{
    setlocale(LC_ALL, "");

    init_Bob();
    init_Alice();

    calculate();

    Bob_print_results();
    Alice_print_results();
}
