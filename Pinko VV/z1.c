#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

typedef long long int Money;

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
    Money loan_payment;  // вычисляется в функции calculate_loan_payment
    Money extra_spendings;

    // в год
    double loan_rate;
    double deposit_rate;
};

struct Person Bob;
struct Person Alice;


// вычисление ежемесячной оплаты кредита
void calculate_loan_payment(struct Person* person)
{
    double numerator = pow(1 + person->loan_rate / 12, person->loan_period * 12);
    double denominator = numerator - 1;
    person->loan_payment = Money((person->apartment_price - person->savings) * person->loan_rate / 12 * numerator / denominator);

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
    Bob.apartment_price = 2 * 1000 * 1000;
    Bob.savings = 600 * 1000;
    Bob.loan_period = 0;
    Bob.salary = 200 * 1000;
    Bob.food_spending = 20 * 1000;
    Bob.rent = 30 * 1000;
    Bob.extra_spendings = 20 * 1000;
    Bob.loan_rate = 0;
    Bob.deposit_rate = 12 * 0.01;

    calculate_loan_payment(&Bob);
}


void init_Alice()
{
    Alice.if_has_apartment = 0;
    Alice.apartment_price = 2 * 1000 * 1000;
    Alice.savings = 600 * 1000;
    Alice.loan_period = 30;
    Alice.salary = 200 * 1000;
    Alice.food_spending = 20 * 1000;
    Alice.rent = 0;
    Alice.extra_spendings = 20 * 1000;
    Alice.loan_rate = 9 * 0.01;
    Alice.deposit_rate = 12 * 0.01;

    calculate_loan_payment(&Alice);
}


// ежемесячное начисление зарплаты
void Get_salary(const int month, const int year, struct Person* person)
{
    person->savings += person->salary;

    if (month == 12) {
        person->salary += person->salary * (INFLATION);  // индексация
    }
}


// ежемесячная покупка еды
void Buy_food(const int month, const int year, struct Person* person)
{
    person->savings -= person->food_spending;
}


// ежемесячная оплата аренды
void Pay_rent(const int month, const int year, struct Person* person)
{
    person->savings -= person->rent;
}


// ежемесячная плата по кредиту
void Pay_loan(const int month, const int year, struct Person* person)
{
    person->savings -= person->loan_payment;
}


// ежемесячный учет дополнительных расходов
void Pay_extra(const int month, const int year, struct Person* person)
{
    person->savings -= person->extra_spendings;
}


//  ежемесячная выплата процентов по вкладу
void Deposit_income(const int month, const int year, struct Person* person)
{
    long double income = person->savings * person->deposit_rate / 12;
    person->savings += income;
}


void Buy_apartment_if_has_not(struct Person* person)
{
    if (!person->if_has_apartment) {
        if (person->savings >= person->apartment_price * 2) {
            person->savings -= person->apartment_price;
            person->if_has_apartment = 1;
            person->rent = 0;
        }
    }
}


// ежемесячный рост цен
void inflate(const int month, const int year, struct Person* person)
{
    person->food_spending += person->food_spending * INFLATION / 12;
    person->extra_spendings += person->extra_spendings * INFLATION / 12;
    person->rent += person->rent * INFLATION / 12;
    person->apartment_price += person->apartment_price * INFLATION / 12;
}

void if_Broke_AF()
{
    if (Bob.savings < 0) {
        printf("Боб обанкротился");
        exit(0);
    }

    if (Alice.savings < 0) {
        printf("Алиса обанкротилась");
        exit(0);
    }
}

// Вывод результатов Боба
void Bob_print_results()
{
    Money Bob_savings_roubles = Bob.savings;
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

    if (Bob.if_has_apartment) {
        printf("\nБоб купил квартиру");
    }
}


// Вывод результатов Алисы
void Alice_print_results()
{
    Money Alice_savings_roubles = Alice.savings;
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

    if (Alice.if_has_apartment) {
        printf("\nАлиса купила квартиру");
    }
}

// вычисление результатов
void calculate()
{
    time_t time_ = time(NULL);
    struct tm time_now = *localtime(&time_);

    int STARTING_YEAR = time_now.tm_year + 1900;
    int STARTING_MONTH = time_now.tm_mon + 1;
    int year = STARTING_YEAR;
    int month = STARTING_MONTH;

    while (!(year == STARTING_YEAR + TOTAL_YEARS && month == STARTING_MONTH)) {
        Get_salary(year, month, &Bob);
        Buy_food(year, month, &Bob);
        Pay_rent(year, month, &Bob);
        Pay_loan(year, month, &Bob);
        Pay_extra(year, month, &Bob);
        Deposit_income(year, month, &Bob);
        Buy_apartment_if_has_not(&Bob);

        Get_salary(year, month, &Alice);
        Buy_food(year, month, &Alice);
        Pay_rent(year, month, &Alice);
        Pay_loan(year, month, &Alice);
        Pay_extra(year, month, &Alice);
        Deposit_income(year, month, &Alice);
        Buy_apartment_if_has_not(&Alice);

        inflate(year, month, &Bob);
        inflate(year, month, &Alice);

        if_Broke_AF();

        Bob_print_results();
        Alice_print_results();

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