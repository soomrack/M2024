#include <stdio.h>
#include <math.h>

typedef long long int Kopeyka;

const Kopeyka APARTMENT_COST = 20 * 1000 * 1000 * 100;
const Kopeyka START_MONEY = 2 * 1000 * 1000 * 100;
const Kopeyka SALARY = 210 * 1000 * 100;
const Kopeyka EXPENSES = 50 * 1000 * 100;
const Kopeyka RENT = 90 * 1000 * 100;
const float MORTGAGE_RATE = 0.10;
const float INFLATION_RATE = 0.09;
const float DEPOSIT_RATE = 0.16;
const int START_MONTH = 3;
const int START_YEAR = 2024;
const int YEARS = 30;

//const float annuity_coef = (MORTGAGE_RATE / 12 * pow(1 + MORTGAGE_RATE, YEARS * 12) / (pow(1 + MORTGAGE_RATE, YEARS * 12) - 1);

int month;
double ap_cost;

float annuity_coef()
{
	return ((MORTGAGE_RATE / 12 * pow(1 + MORTGAGE_RATE, YEARS * 12)) / (pow(1 + MORTGAGE_RATE, YEARS * 12) - 1));

}


float mortgage()
{
	return((APARTMENT_COST - START_MONEY) * annuity_coef());
}


float rubel(double kopeechka)
{
	return(kopeechka / 100);
}


struct Person
{
	Kopeyka salary;
	Kopeyka expenses;
	Kopeyka savings;
	Kopeyka rent;
};

struct Person Alice;
struct Person Bob;


void Alice_init()
{
	Alice.salary = SALARY;
	Alice.expenses = EXPENSES;
}


void Bob_init()
{
	Bob.salary = SALARY;
	Bob.expenses = EXPENSES;
	Bob.rent = RENT;
	Bob.savings = START_MONEY;
}

float inflation(double param, float rate)
{
	return(param *= 1 + rate);
}


float deposit(double param, float rate)
{
	return(param *= 1 + rate/12);
}


void alice()
{
	month = START_MONTH;
	ap_cost = APARTMENT_COST;
	for (int year = START_YEAR; year < START_YEAR + YEARS + 1; year++) {
		while (month <= 12) {
			Alice.savings += Alice.salary - mortgage() - Alice.expenses;
			Alice.expenses = inflation(Alice.expenses, INFLATION_RATE / 12);
			if (month == 12)
			{
				Alice.salary = inflation(Alice.salary, INFLATION_RATE);
				ap_cost = inflation(ap_cost, INFLATION_RATE);
			}
			month++;
		}
		month = 1;
	}
	printf("Alice: %llf \n", rubel(Alice.savings + ap_cost));
}


void bob()
{
	month = START_MONTH;
	for (int year = START_YEAR; year < START_YEAR + YEARS + 1; year++) {
		while (month <= 12) {
			Bob.savings += Bob.salary - Bob.expenses - Bob.rent;
			Bob.savings = deposit(Bob.savings, DEPOSIT_RATE);
			Bob.expenses = inflation(Bob.expenses, INFLATION_RATE / 12);
			if (month == 12)
			{
				Bob.salary = inflation(Bob.salary, INFLATION_RATE);
				if (year % 2 != 0)
				{
					Bob.rent = inflation(Bob.rent, INFLATION_RATE);
				}
			}
			month++;
		}
		month = 1;
	}
	printf("Bob: %llf", rubel(Bob.savings));
}


void main()
{
	Alice_init();
	Bob_init();
	alice();
	bob();
}
