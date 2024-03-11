#include <stdio.h>
#include <math.h>

typedef long long int kopeika;

const double DEPOSIT_RATE = 0.16; // ставка по вкладу
const double MORTGAGE_RATE = 0.18; // ставка по ипотеке
const int YEARS = 30; // период
const int START_YEAR = 2024; // первый год
const int START_MONTH = 2; // первый месяц
const kopeika SALARY = 200*1000*100; // зарплата
const kopeika START_CAPITAL = 2000*1000*100; // стартовый капитал
const kopeika RENT_COST = 25*1000*100; // стоимость аренды квартиры
const double INFLATION = 0.12; // инфляция
kopeika SPENDINGS = 20*1000*100; // траты ежемесячные
kopeika FLAT_COST = 20*1000*1000*100; // стоимость квартиры

struct Person {
kopeika salary; 
kopeika savings; 
kopeika outlay; 
kopeika rent_payment;
kopeika flat_payment;
};

struct Person Alice;
struct Person Bob;

void Alice_init()
{
    double monthly_coef = MORTGAGE_RATE / 12.0;
    int total_payments = YEARS * 12;
    double discount_factor = (pow(1 + monthly_coef, total_payments) - 1) / 
 (monthly_coef * pow(1 + monthly_coef, total_payments));
    Alice.savings = 0;
    Alice.salary = SALARY;
    Alice.outlay = SPENDINGS;
    Alice.flat_payment = (FLAT_COST - START_CAPITAL) / discount_factor;
};

void Bob_init()
{
    Bob.savings = START_CAPITAL;
    Bob.salary = SALARY;
    Bob.outlay = SPENDINGS;
    Bob.rent_payment = RENT_COST;
};

void Alice_add_salary(int year, int month) 
{
    Alice.savings += Alice.salary;
};

void Bob_add_salary(int year, int month) 
{
    Bob.savings += Bob.salary;
};

void Alice_account(int year, int month) 
{
    Alice.savings *= (1.0 + DEPOSIT_RATE / 12.0);
};

void Bob_account(int year, int month) 
{
    Bob.savings *= (1.0 + DEPOSIT_RATE / 12.0);
};

void Alice_expenses(int year, int month)
{
    Alice.savings -= (Alice.flat_payment + Alice.outlay);
};

void Bob_expenses(int year, int month)
{
    Bob.savings -= (Bob.rent_payment + Bob.outlay);
};

void change_inflation(int year, int month)
{
    if (month == 12) {
        Alice.salary *= (1.0 + INFLATION);
        Bob.salary *= (1.0 + INFLATION);
        Bob.flat_payment *= (1.0 + INFLATION);
        Alice.outlay *= (1.0 + INFLATION);
        Bob.outlay *= (1.0 + INFLATION);
        FLAT_COST *= (1.0 + INFLATION);
    }
};

void simulation() 
{
    int year = START_YEAR;
    int month = START_MONTH;
    while((month != 1) || year != (YEARS + START_YEAR) ) {
        Alice_account(year, month);
        Alice_add_salary(year, month);
        Alice_expenses(year, month);
        Bob_account(year, month);
        Bob_add_salary(year, month);
        Bob_expenses(year, month);
        change_inflation(year, month);
    
       month++;
        if (month > 12)
        {
            year++;
            month = 1;
        }
    }
};

void print_results()
{
    double Alice_final = (FLAT_COST + Alice.savings);
    double Bob_final = Bob.savings;
    Alice_final /= 100; 
    Bob_final /= 100;
    printf("Alice's capital = %f\n", Alice_final);
    printf("Bob's capital = %f\n", Bob_final);
};

int main() 
{
    Alice_init();
    Bob_init();
    simulation();
    print_results();
    return 0;
}
