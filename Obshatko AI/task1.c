#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

typedef long long int Money;

int TOTAL_YEARS = 30;
double INFLATION = 12 * 0.01;


struct Person 
{
    int if_has_apartment = 0;
    Money apartment_price;
    Money savings;  // сумма денег на вкладе
    int LOAN_PERIOD;  // в годах

    // в месяц
    Money salary;
    Money food_spending;
    Money RENT;
    Money LOAN_PAYMENT;  // вычисляется в функции compute_LOAN_payment
    Money additional_spendings;

    // в год
    Money APARTMENT_REPAIRS;
    double LOAN_RATE;
    double DEPOSIT_RATE;
};

struct Person Bob;
struct Person Alice;


// вычисление ежемесячной оплаты кредита
void calculate_loan_payment(struct Person *person)
{
    double numerator = pow(1 + person->LOAN_RATE / 12, person->LOAN_PERIOD * 12);
    double denominator = numerator - 1;
    person->LOAN_PAYMENT = Money((person->apartment_price - person->savings) * person->LOAN_RATE / 12 * numerator / denominator);

    if (person->LOAN_RATE == 0) {
        person->LOAN_PAYMENT = 0;
        person->if_has_apartment = 0;
    }
    else 
    {
        person->if_has_apartment = 1;
    }

    if (person->LOAN_PAYMENT > person->salary)
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
    Bob.LOAN_PERIOD = 0;
    Bob.salary = 300 * 1000 * 100;
    Bob.food_spending = 15000 * 100;
    Bob.RENT = 25000 * 100;
    Bob.additional_spendings = 0;
    Bob.APARTMENT_REPAIRS = 0;
    Bob.LOAN_RATE = 0;
    Bob.DEPOSIT_RATE = 16 * 0.01;

    calculate_loan_payment(&Bob);
}


void init_Alice()
{
    Alice.if_has_apartment = 0;
    Alice.apartment_price = 20 * 1000 * 1000 * 100;
    Alice.savings = 2 * 1000 * 1000 * 100;
    Alice.LOAN_PERIOD = 30;
    Alice.salary = 300 * 1000 * 100;
    Alice.food_spending = 15000 * 100;
    Alice.RENT = 0;
    Alice.additional_spendings = 0;
    Alice.APARTMENT_REPAIRS = 100 * 1000 * 100;
    Alice.LOAN_RATE = 18 * 0.01;
    Alice.DEPOSIT_RATE = 16 * 0.01;

    calculate_loan_payment(&Alice);
}


// ежемесячное начисление зарплаты
void get_salary_Bob(int month, int year)
{
    Bob.savings += Bob.salary;
}


void get_salary_Alice(int month, int year)
{
    Alice.savings += Alice.salary;
}


// ежемесячная покупка еды
void buy_food_Bob(int month, int year)
{
    Bob.savings -= Bob.food_spending;
}


void buy_food_Alice(int month, int year)
{
    Alice.savings -= Alice.food_spending;
}


// ежемесячная оплата аренды
void pay_rent_Bob(int month, int year)
{
    Bob.savings -= Bob.RENT;
}


void pay_rent_Alice(int month, int year)
{
    Alice.savings -= Alice.RENT;
}


// ежемесячная плата по кредиту
void pay_loan_Bob(int month, int year)
{
    Bob.savings -= Bob.LOAN_PAYMENT;
}


void pay_loan_Alice(int month, int year)
{
    Alice.savings -= Alice.LOAN_PAYMENT;
}


// ежемесячный учет дополнительных расходов
void pay_additional_Bob(int month, int year)
{
    Bob.savings -= Bob.additional_spendings;
}


void pay_additional_Alice(int month, int year)
{
    Alice.savings -= Alice.additional_spendings;
}


// ежегодный ремонт квартиры
void repair_apartment_Bob(int month, int year)
{
    Bob.savings -= Bob.APARTMENT_REPAIRS;
}


void repair_apartment_Alice(int month, int year)
{
    Alice.savings -= Alice.APARTMENT_REPAIRS;
}


//  ежемесячная выплата процентов по вкладу
void deposit_income_Bob(int month, int year)
{
    long double income = Bob.savings * Bob.DEPOSIT_RATE / 12;
    Bob.savings += income;
}


void deposit_income_Alice(int month, int year)
{
    long double income = Alice.savings * Alice.DEPOSIT_RATE / 12;
    Alice.savings += income;
}


// ежегодное повышение зарплаты
void indexation_Bob(int month, int year)
{
    Bob.salary += Bob.salary * INFLATION;
}


void indexation_Alice(int month, int year)
{
    Alice.salary += Alice.salary * INFLATION;
}


void buy_apartment_if_has_not()
{
    if (!Bob.if_has_apartment)
    {
        if (Bob.savings >= Bob.apartment_price)
        {
            Bob.savings -= Bob.apartment_price;
            Bob.if_has_apartment = 1;
        }
    }

    if (!Alice.if_has_apartment)
    {
        if (Alice.savings >= Alice.apartment_price)
        {
            Alice.savings -= Alice.apartment_price;
            Alice.if_has_apartment = 1;
        }
    }
}


// ежемесячный рост цен
void inflate(int month, int year)
{
    Bob.food_spending += Bob.food_spending * INFLATION / 12;
    Bob.additional_spendings += Bob.additional_spendings * INFLATION / 12;

    Alice.food_spending += Alice.food_spending * INFLATION / 12;
    Alice.additional_spendings += Alice.additional_spendings * INFLATION / 12;
}


// вычисление результатов
void calculate() 
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int STARTING_YEAR = tm.tm_year + 1900;
    int STARTING_MONTH = tm.tm_mon + 1;
    int year = STARTING_YEAR;
    int month = STARTING_MONTH;

    while(!(year == STARTING_YEAR + TOTAL_YEARS && month == STARTING_MONTH))
    {
        get_salary_Bob(year, month);
        get_salary_Alice(year, month);

        buy_food_Bob(year, month);
        buy_food_Alice(year, month);

        pay_rent_Bob(year, month);
        pay_rent_Alice(year, month);

        pay_loan_Bob(year, month);
        pay_loan_Alice(year, month);

        pay_additional_Bob(year, month);
        pay_additional_Alice(year, month);

        deposit_income_Bob(year, month);
        deposit_income_Alice(year, month);

        buy_apartment_if_has_not();

        inflate(year, month);


        if(month != 12)
        {
            month += 1;
        }
        else
        {
            repair_apartment_Bob(year, month);
            repair_apartment_Alice(year, month);

            indexation_Bob(year, month);
            indexation_Alice(year, month);

            month = 1;
            year += 1;
        }

    }
}

// Вывод результатов Боба
void print_results_Bob()
{
    Money Bob_savings_roubles = Bob.savings / 100;
    long long int count_digits = 1;
    while (count_digits < Bob_savings_roubles)
    {
        count_digits *= 1000;
    }
    count_digits /= 1000;

    printf("\nНакопления Боба: ");

    printf("%lld ", Bob_savings_roubles / count_digits);
    Bob_savings_roubles %= count_digits;
    count_digits /= 1000;

    for (count_digits; count_digits > 0; count_digits /= 1000)
    {
        int number_to_print = Bob_savings_roubles / count_digits;
        printf("%03d ", number_to_print);
        Bob_savings_roubles %= count_digits;
    }
    printf("рублей %02lld копеек", Bob.savings % 100);
}


// Вывод результатов Алисы
void print_results_Alice()
{
    Money Alice_savings_roubles = Alice.savings / 100;
    long long int count_digits = 1;
    while (count_digits < Alice_savings_roubles)
    {
        count_digits *= 1000;
    }
    count_digits /= 1000;

    printf("\nНакопления Алисы: ");

    printf("%lld ", Alice_savings_roubles / count_digits);
    Alice_savings_roubles %= count_digits;
    count_digits /= 1000;

    for (count_digits; count_digits > 0; count_digits /= 1000)
    {
        int number_to_print = Alice_savings_roubles / count_digits;
        printf("%03d ", number_to_print);
        Alice_savings_roubles %= count_digits;
    }
    printf("рублей %02lld копеек", Alice.savings % 100);
}


int main()
{
    setlocale(LC_ALL, "");

    init_Bob();
    init_Alice();

    calculate();

    print_results_Bob();
    print_results_Alice();
}
