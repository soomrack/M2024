#include<stdlib.h>
#include<math.h>

typedef long long int Kopeyka;


const Kopeyka APARTMENT_COST = 2000000000; //стоимость квартиры
const Kopeyka EXPENSES = 5000000; //расходы
const Kopeyka START_CAPITAL = 200000000; //начальный капитал
const Kopeyka RENOVATION = 100000000; //стоимость ремонта
const float MORTGAGE_RATE = 0.10; //процент по ипотеке
const float INFLATION_RATE = 0.09; //величина инфляции
const float DEPOSIT_RATE = 0.16; //процент по вкладу
const short int YEARS = 30; //расчетный период
const Kopeyka SALARY = 17000000; //зарплата (на первый год)
const Kopeyka RENT = 8500000; //стоимость аренды (на первый год)
int current_month = 2;
float annuity_coef = MORTGAGE_RATE / 12 * pow(1 + MORTGAGE_RATE / 12, YEARS * 12) / (pow(1 + MORTGAGE_RATE / 12, YEARS * 12) - 1);

struct Person
{
    Kopeyka payment;
    Kopeyka salary;
    Kopeyka savings;
    Kopeyka rent;
    Kopeyka expenses;
};

struct Person Alice;
struct Person Bob;

Kopeyka ap_cost = APARTMENT_COST;

void Alice_init()
{
    Alice.payment = APARTMENT_COST * annuity_coef;
    Alice.salary = SALARY;
    Alice.savings = 0;
    Alice.rent = 0;
    Alice.expenses = EXPENSES;
}


void Bob_init()
{
    Bob.salary = SALARY;
    Bob.rent = RENT;
    Bob.savings = START_CAPITAL;
    Bob.expenses = EXPENSES;
}


void alice_inflation()
{
    Alice.salary *= 1.0 + INFLATION_RATE;
    ap_cost *= 1 + INFLATION_RATE;
}


void bob_inflation()
{
    Bob.savings *= 1 + DEPOSIT_RATE;
    ap_cost *= 1 + INFLATION_RATE;
    Bob.rent *= 1 + INFLATION_RATE;
}


void bob_add_salary()
{
    Bob.salary *= (1 + INFLATION_RATE);
}


void alice_add_expenses()
{
    Alice.expenses = Alice.expenses * (1 + INFLATION_RATE / 12);
}


void bob_add_expenses()
{
    Bob.expenses = Bob.expenses * (1 + INFLATION_RATE / 12);
}


void alice()
{
    for (int year = 2024; year < year + YEARS + 1; year++) {
        while (current_month <= 12) {
            Alice.savings += Alice.salary - Alice.expenses - Alice.payment;
            current_month++;
            alice_add_expenses();
        }
        alice_inflation();
        current_month = 1;
    }
    printf("Alice: %lli", (Alice.savings + ap_cost) / 100);
}


void bob()
{
    current_month = 2;
    for (int year = 2024; year < year + YEARS + 1; year++) {
        while (current_month <= 12) {
            Bob.savings += Bob.salary - Bob.expenses - Bob.rent;
            current_month++;
            bob_add_expenses();
            if (current_month = 1) {
                bob_inflation();
            }
        }
        if ((year % 2) == 0) {
            bob_add_salary();
        }
        current_month = 1;
    }
    printf("Bob: %lli", Bob.savings / 100);


}
void main()
{
    Alice_init();
    Bob_init();
    alice();
    bob();
}
