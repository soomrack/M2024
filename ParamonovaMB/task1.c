#include<stdio.h>
#include<math.h>

const int FLAT_COST = 20 * 1000 * 1000;
const int FLAT_RENT = 25 * 1000;
const int SALARY = 150 * 1000;
const float INFLATION = 0.12; 

const int TEETH = 500 * 1000;
const int VACATION = 200 * 1000; 

const int FIRST_MONTH = 9;
const int LAST_MONTH = 8;   

const int FIRST_YEAR = 2024;
const int LAST_YEAR = 2054; 

float BANK_DEPOSIT = 0.16; 


typedef long long int Money;

struct Person{
    Money capital;
    Money salary;
    Money monthly_spendings;
};

struct Person Alice;
struct Person Bob;


void bob_initialization()
{
    Bob.capital = 2 * 1000 * 1000;
    Bob.monthly_spendings = 15 * 1000 + FLAT_RENT;
    Bob.salary = SALARY;
}


void alice_initialization()
{
    int number_of_years = LAST_YEAR - FIRST_YEAR;
    Alice.capital = 0;
    Alice.monthly_spendings = 15 * 1000 + ((FLAT_COST - 2000000) * (0.18 / 12) * (pow((1 + 0.18 / 12), 
    (number_of_years * 12)))) / (pow((1 + 0.18 / 12), (number_of_years * 12)) - 1);
Alice.salary = SALARY;
}


void bob_salary_income(int year, int month)
{
    Bob.capital += Bob.salary;
}


void alice_salary_income(int year, int month)
{
    Alice.capital += Alice.salary;
}


void bob_pay_for_life(int year, int month)
{
    if (month == 12) {
        Bob.capital -= Bob.monthly_spendings * 1.2;
    }
    Bob.capital -= Bob.monthly_spendings;
}


void alice_pay_for_life_and_flat_repair(int year, int month)
{
    if (month == 12) {
        Alice.capital -= Alice.monthly_spendings * 1.2;
    }
    if (year < 2) {
        Alice.capital -= 100*1000;
    }
    Alice.capital -= Alice.monthly_spendings;
}


void bob_salary_index(int year, int month)
{
    if(month == 12){
        Bob.salary *= 1.05;
    }
}


void alice_salary_index(int year, int month)
{
    if (month == 12) {
        Alice.salary *= 1.05;
    }
}


void life_inflation(int year, int month)
{
    if (month % 6 == 0 && month > 2) {
        Alice.monthly_spendings += Alice.monthly_spendings * INFLATION / 2.0;
        Bob.monthly_spendings += Bob.monthly_spendings * INFLATION / 2.0;
    }
    if (month == 12) {
        Alice.salary *= (1+INFLATION);
        Bob.salary *= (1+INFLATION);
    }
}


void teeth_repair(int year, int month)
{
    if((year - FIRST_YEAR) % 10 == 0 && year != FIRST_YEAR && month == 6){
        Alice.capital -= TEETH;
    }
}


void vacation_turkey(int year, int month)
{
    if((year - FIRST_YEAR) % 2 == 0 && year != FIRST_YEAR && month == 6){
        Alice.capital -= VACATION;
    }
}


void simulation()
{
    int month = FIRST_MONTH;
    int year = FIRST_YEAR;

    while(!(year == LAST_YEAR && month == LAST_MONTH)) {

        alice_salary_income(year, month);
        alice_pay_for_life_and_flat_repair(year, month);
        alice_salary_index(year, month);
        vacation_turkey(year, month);
        teeth_repair(year, month);

        bob_salary_income(year, month);
        bob_pay_for_life(year, month);
        bob_salary_index(year, month);

        life_inflation(year, month);
        
        month++;
        if (month > 12) {
            year++;
            month = 1;
        }  
    }
}


void print()
{
    printf("\nAlice capital: %lld\n", Alice.capital);
    printf("\nBob capital: %lld\n", Bob.capital);
}


main()
{
    alice_initialization();
    bob_initialization();
    
    simulation();
    
    print();
    
    return 0;
}