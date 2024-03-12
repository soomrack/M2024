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
int year;
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


void alice_savings()
{
	Alice.savings += Alice.salary - mortgage() - Alice.expenses;
}


void alice_add_expenses()
{
	Alice.expenses *= 1 + INFLATION_RATE / 12;
}


void alice_add_salary(int m)
{
	if (m == 12) {
		Alice.salary *= 1 + INFLATION_RATE;
	}
}


void apart_cost(int m)
{
	if (m == 12) {
		ap_cost *= 1 + INFLATION_RATE;
	}
}


void bob_deposit()
{
	Bob.savings *= 1 + DEPOSIT_RATE / 12;
}


void bob_add_salary(int m)
{
	if (m == 12) {
		Bob.salary *= 1 + INFLATION_RATE;
	}
}


void bob_add_rent(int m)
{
	if (m == 12) {
		Bob.rent *= 1 + INFLATION_RATE;
	}
}


void bob_savings(int m, int y)
{
	static Kopeyka new_salary;
	if ((m == 2) && (y == 2028)) {
		new_salary = Bob.salary;
		Bob.salary = 0;
	}
	if ((m == 4) && (y == 2028)) {
		Bob.salary = 1.5 * new_salary;
	}
	Bob.savings += Bob.salary - Bob.expenses - Bob.rent;
}


void bob_add_expenses()
{
	Bob.expenses *= 1 + INFLATION_RATE / 12;	
}


void print_alice()
{
	printf("Alice: %.0llf \n", rubel(Alice.savings + ap_cost));
}


void print_bob()
{
	printf("Bob: %.0llf", rubel(Bob.savings));
}


void alice()
{
	month = START_MONTH;
	year = START_YEAR;
	ap_cost = APARTMENT_COST;
	while ((year != START_YEAR + YEARS) || (month != START_MONTH)) {
		alice_savings();
		alice_add_expenses();
		alice_add_salary(month);
		apart_cost(month);
		if (month == 12) {
			month = 1;
			year++;
		}
		else
		{
			month++;
		}
	}
	print_alice();
}


void bob()
{
	month = START_MONTH;
	year = START_YEAR;
	while ((year != START_YEAR + YEARS) || (month!= START_MONTH)) {
		bob_savings(month, year);
		bob_add_expenses();
		bob_deposit();
		bob_add_salary(month);
		bob_add_rent(month);
		if (month == 12)
		{
			month = 1;
			year++;
		}
		else
		{
			month++;
		}
	}
	print_bob();
}


void main()
{
	Alice_init();
	Bob_init();
	alice();
	bob();
}
