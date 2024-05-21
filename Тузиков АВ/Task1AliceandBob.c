#include <stdio.h>
#include <math.h> 


typedef long long int Money;  // копейки 


struct Person
{
    Money capital;
    Money income;              // zp 
    Money life_spendings;      // expenses for life
    Money monthly_payment;     // expenses for monthly payment
    Money overpayment;         // overpayment 

};

struct Person Alice;
struct Person Bob;


const Money PRICE = 20 * 1000 * 1000 * 100;     // apartment cost
const Money SALARY = 200 * 1000 * 100;
const Money START_CAPITAL = 1000 * 1000 * 100;  // START_CAPITAL
const int YEARS = 30;
const double INFLATION = 0.07;                  // сота€ процента инфл€ции в год 
double BANK_RATE = 0.09;                        // сота€ годовой ставки в банке
const Money LIFE_SPENDINGS = 25 * 1000 * 100;
const int START_MONTH = 9;                      // начальные временные услови€
const int START_YEAR = 2023;
const int LAST_MONTH = 8;
const int LAST_YEAR = 2053;


void alice_initialization()
{
    Alice.income = SALARY;
    Alice.capital = START_CAPITAL;
    Alice.life_spendings = LIFE_SPENDINGS;
    Alice.monthly_payment = 40 * 1000 * 100;
    Alice.overpayment = PRICE * powf((1.0 + INFLATION), YEARS) - PRICE;
}


void alice_salary_income(const int year, const int month)
{
    Alice.capital += Alice.income;
}



void alice_deposite_income(const int year, const int month)
{
    if (year == START_YEAR && month == START_MONTH) {
        return;
    };
    if (year == 2033 && month == 1) {
        BANK_RATE = 0.1;
    };
    Alice.capital = Alice.capital * (1.0 + BANK_RATE / 12.0);
}


void alice_life_spending(const int year, const int month)
{
    Alice.capital -= Alice.life_spendings;
}


void alice_rent_payment(const int year, const int month)
{
    Alice.capital -= Alice.monthly_payment;
}


void bob_initialization()
{
    Bob.income = SALARY;
    Bob.capital = 0;
    Bob.life_spendings = LIFE_SPENDINGS;
    Bob.monthly_payment = (PRICE - START_CAPITAL) * (BANK_RATE / 12.0) / (1 - powf((1 + BANK_RATE / 12.0), (1.0 - YEARS * 12.0)));
    Bob.overpayment = Bob.monthly_payment * 12 * YEARS - PRICE;
}


void bob_salary_income(const int year, const int month)
{
    Bob.capital += Bob.income;

}


void bob_deposite_income(const int year, const int month)
{
    if (year == 2033 && month == 1) {
        BANK_RATE = 0.1;
    };
    Bob.capital = Bob.capital * (1.0 + BANK_RATE / 12.0);
}


void bob_life_spending(const int year, const int month)
{

    Bob.capital -= Bob.life_spendings;
}


void bob_mortgage_payment(const int year, const int month)
{
    Bob.capital -= Bob.monthly_payment;
}


void inflation(const int year, const int month)
{
    if (month % 3 == 0 && month > 2) {
        Alice.life_spendings += Alice.life_spendings * INFLATION / 4.0;
        Bob.life_spendings += Bob.life_spendings * INFLATION / 4.0;
    };
}




void simulation()
{
    int month = START_MONTH;
    int year = START_YEAR;
    int last_year = START_YEAR + YEARS;

    while (!(year == last_year && month == LAST_MONTH)) {

        alice_deposite_income(year, month);
        alice_salary_income(year, month);
        alice_life_spending(year, month);
        alice_rent_payment(year, month);

        bob_deposite_income(year, month);
        bob_salary_income(year, month);
        bob_life_spending(year, month);
        bob_mortgage_payment(year, month);

        inflation(year, month);


        month++;

    }
}


void print_persons()
{
    printf("A. Capital: %lld\t", Alice.capital);
    printf("A. Income: %lld\t", Alice.income);
    printf("A. Life spendings: %lld\t", Alice.life_spendings);
    printf("A. Monthly payment: %lld\t", Alice.monthly_payment);
    printf("A. Overpayment: %lld\n", Alice.overpayment);
    printf("B. Capital: %lld\t", Bob.capital);
    printf("B. Income: %lld\t", Bob.income);
    printf("B. Life spendings: %lld\t", Bob.life_spendings);
    printf("B. Monthly payment: %lld\t", Bob.monthly_payment);
    printf("B. Overpayment: %lld\n", Bob.overpayment);
}


int main()
{
    alice_initialization();
    bob_initialization();

    simulation();

    print_persons();

    return 0;
}
