#include<stdio.h>
#include<stdlib.h>
#include <math.h>

typedef long long int Money;
const int YEAR_NOW = 2024; 
const int MONTH_NOW = 3; 
const int YEARS_COUNT = 30; 
const Money START_CAPITAL = 2*1000*1000;          //Изначальный капитал
const Money APARTMENT_COST = 20*1000*1000;        //Стоимость квартиры
const Money MONTH_SALARY = 2*100*1000;              //Зарплата
const Money MONTH_FOOD = 15*1000;                 //Затраты на еду в месяц
const Money MONTH_RENT = 25*1000;                 //Аренда за квартиру в месяц
const float INFLATION = 0.12;                     //Индексация и инфляция
const float I_PERCENT = 0.18;                     //Годовой процент по ипотеки
const float V_PERCENT = 0.16;                     //Процент по вкладу

int month;
int year;
float flat_cost;

float annuity_coef()
{
	return ((I_PERCENT/12 * pow(1 + I_PERCENT, YEARS_COUNT * 12)) / (pow(1 + I_PERCENT, YEARS_COUNT * 12) - 1));

}

float ipoteka()
{
	return((APARTMENT_COST - START_CAPITAL) * annuity_coef());
}

float dolar (float rubli)
{
	return( rubli / 100);
}

struct Person
{
    Money salary; 
    Money food; 
    Money rent; 
    Money capital; //Накопленная сумма за весь промежуток врем
};

struct Person Bob;
struct Person Alice;



void Alice_initially() {
    Alice.capital = 0;
	Alice.salary = MONTH_SALARY;
	Alice.food = MONTH_FOOD;
}

void Bob_initially() {
	Bob.capital = START_CAPITAL;
	Bob.salary = MONTH_SALARY;
	Bob.food = MONTH_FOOD;
    Bob.rent = MONTH_RENT;
}

void alice_capital()
{
	Alice.capital += Alice.salary - ipoteka() - Alice.food;
}

void alice_increase_food()
{
	Alice.food *= 1 + INFLATION / 12;
}

void alice_increase_salary(int m)
{
	if (m == 12) {
		Alice.salary *= 1 + INFLATION;
	}
}

void ap_cost(int m)
{
	if (m == 12) {
		flat_cost *= 1 + INFLATION;
	}
}

void bob_increase_food()
{
	Bob.food *= 1 + INFLATION / 12;	
}

void bob_increase_salary(int m)
{
	if (m == 12) {
		Bob.salary *= 1 + INFLATION;
	}
}

void bob_increase_rent(int m)
{
	if (m == 12) {
		Bob.rent *= 1 + INFLATION;
	}
}

void bob_deposit()
{
	Bob.capital *= 1 + V_PERCENT / 12;  
}

void bob_capital()
{
	Bob.capital += Bob.salary - Bob.food - Bob.rent;
}

void print_alice()
{
	printf("Alice: %.0llf\n", dolar(Alice.capital));
}


void print_bob()
{
	printf("Bob: %.0llf\n", dolar(Bob.capital));
}       

void alice()
{
	month = MONTH_NOW;
	year = YEAR_NOW;
	while ((year != YEAR_NOW + YEARS_COUNT) || (month != MONTH_NOW)) {
		alice_capital();
		alice_increase_food();
		alice_increase_salary(month);
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
	month = MONTH_NOW;
	year = YEAR_NOW;
    flat_cost = APARTMENT_COST; 
	while ((year != YEAR_NOW + YEARS_COUNT) || (month!= MONTH_NOW)) {
		bob_capital();
        bob_deposit();
		bob_increase_food();
		bob_increase_salary(month);
		bob_increase_rent(month);
        ap_cost(month);
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
	Alice_initially();
	Bob_initially();
	alice();
	bob();
    system("pause");
}