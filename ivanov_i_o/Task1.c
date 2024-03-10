#include <stdio.h>
#include <math.h>


typedef long long int Money;


struct Person
{
    Money capital;
    Money income;
    Money life_spendings; 
    Money monthly_spendings;
    Money overpayment;
} Alice, Bob;

 
const Money FLAT_PRICE = 20 * 1000 * 1000 * 100;
const Money SALARY = 200 * 1000 * 100;
const Money START_CAPITAL = 2000 * 1000 * 100;
const int YEARS = 25;
const double INFLATION = 0.09;
double BANK_RATE = 0.08;
const Money LIFE_SPENDINGS = 30 * 1000 * 100;
const int START_MONTH = 3;
const int START_YEAR = 2024;


void Alice_initiation()
{
    Alice.income = SALARY;
    Alice.capital = 0;
    Alice.life_spendings = LIFE_SPENDINGS;
    Alice.monthly_spendings = (FLAT_PRICE - START_CAPITAL) * (BANK_RATE / 12.0) / (1 - powf((1.0 + BANK_RATE / 12.0), (1.0 - YEARS * 12.0)));
    Alice.overpayment = Alice.monthly_spendings * 12 * YEARS - FLAT_PRICE;
}


void Alice_salary_income(int year, int month)
{
   if (month == 12) 
   {
       Alice.capital += (Money)(Alice.income * 1.2);
   } 
   Alice.capital += Alice.income;
}


void Alice_deposite_income(int year, int month)
{
     if (year == 2034 && month == 1)
    {
        BANK_RATE = 0.1;
    }
    Alice.capital = Alice.capital * (1.0 + BANK_RATE / 12.0);
}


void Alice_usual_spendings (int years, int month)
{
    if (month == 12) 
    {
        Alice.capital -= Alice.life_spendings * 1.5;
    }
    Alice.capital -= Alice.life_spendings + Alice.monthly_spendings;
}


void Bob_initiation()
{
    Bob.income = SALARY;
    Bob.capital = START_CAPITAL;
    Bob.life_spendings = LIFE_SPENDINGS;
    Bob.monthly_spendings = 40 * 1000 * 100;
    Bob.overpayment = FLAT_PRICE * powf((1.0 + INFLATION), YEARS) - FLAT_PRICE;
}


void Bob_salary_income(int year, int month)
{
   if (month == 12) 
   {
       return;
   } 
   Bob.capital += Bob.income;
}


void Bob_deposite_income(int year, int month)
{
    if (year == 2034 && month == 1)
    {
        BANK_RATE = 0.1;
    }
   Bob.capital = Bob.capital * (1.0 + BANK_RATE / 12.0);
}


void Bob_usual_spendings (int years, int month)
{
    if (month == 12) 
    {
        Bob.capital -= Bob.life_spendings * 1.5;
    }
    Bob.capital -=  Bob.life_spendings + Bob.monthly_spendings;
}


void inflation(int year, int month)
{
    if (month % 3 == 0 && month > 2) 
    {
        Alice.life_spendings += Alice.life_spendings * INFLATION / 4.0;
        Bob.life_spendings += Bob.life_spendings * INFLATION / 4.0;
    }
    if (month == 12) 
    {
        Alice.income = Alice.income * (1.0 + INFLATION / 2.0);
        Bob.income = Bob.income * (1.0 + INFLATION / 2.0);
        Bob.monthly_spendings = Bob.monthly_spendings * (1.0 + INFLATION / 2.0);
    }
}


void simulation()
{
    int month = START_MONTH;
    int year = START_YEAR;

    while(year != START_YEAR + YEARS || month != START_MONTH ) 
    {
        Alice_deposite_income(year, month);
        Alice_salary_income(year, month);
        Alice_usual_spendings(year, month);
                
        Bob_deposite_income(year, month);
        Bob_salary_income(year, month);
        Bob_usual_spendings(year, month);

        inflation(year, month);

        month++;
        if (month > 12) 
        {
            year++;
            month -= 12;
        }
    }
}


void print_result()
{
    printf("Alice Capital: %lld\n",Alice.capital / 100);
    printf("Alice Income: %lld\n", Alice.income / 100); 
    printf("Alice Life spendings: %lld\n", Alice.life_spendings / 100); 
    printf("Alice Monthly spendings: %lld\n", Alice.monthly_spendings / 100);
    printf("Alice Overpayment: %lld\n", Alice.overpayment / 100);

    printf("Bob Capital: %lld\n", Bob.capital / 100);
    printf("Bob Income: %lld\n", Bob.income / 100);
    printf("Bob Life spendings: %lld\n", Bob.life_spendings / 100);
    printf("Bob Monthly spendings: %lld\n", Bob.monthly_spendings / 100);
    printf("Bob Overpayment: %lld\n", Bob.overpayment / 100);

    printf("* all values presented above are in rubles \n");
}


int main()
{
    Alice_initiation();
    Bob_initiation();

    simulation();

    print_result();

    return 0;
}