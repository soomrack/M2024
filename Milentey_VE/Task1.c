#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef long long int cents; //kopeika

const int YEARS_COUNT = 30;
const int YEAR = 2024;
const int MONTH = 3;
const double INFLATION = 0.12;
const double DEPOSIT_PERCENT = 0.16;
const double BANK_PERCENT = 0.18;
const double INDEXATION = 0.08;
const cents START_SUM = 2 * 1000000 * 100;
const cents MONTH_SALARY = 300 * 1000 * 100;
const cents BOB_START_RENT = 25 * 1000 * 100;
const cents FLAT_COST = 20 * 1000 * 1000 * 100; 
const cents FOOD_MANTH = 15 * 1000 * 100; // first year

struct Person {
	cents bank_account;
	cents salary;
	cents food;
	cents rent;
};

struct Person Bob;
struct Person Alice;

cents flat_cost = 20 * 1000 * 1000 * 100;

void Bob_initially() {
	Bob.bank_account = START_SUM;
	Bob.salary = MONTH_SALARY;
	Bob.food = FOOD_MANTH;
	Bob.rent = BOB_START_RENT;
}

void Alice_initially() {
	Alice.bank_account = 0;
	Alice.salary = MONTH_SALARY;
	Alice.food = FOOD_MANTH;
}

cents Alice_month_pay() {
	double month_per = BANK_PERCENT / 12;
	cents bob_month_payment = (FLAT_COST - START_SUM) *
 (month_per + (month_per / (pow(1 + month_per, YEARS_COUNT * 12) - 1)));
	return bob_month_payment;
}
	
void Alice_add_salary(const int year, const int month) {


	if (month == 40 || month == 52) {
		Alice.bank_account += 0;
	}

	if (month >= 25 && month <= 29) {
		Alice.bank_account += 0;
	}

	if (month == 30) {
		Alice.salary *= 2;
	}

	else {
		Alice.bank_account += Alice.salary;
	}
	
	if (month % 12 == 0) {
		Alice.salary *= (1 + INDEXATION);
	}
}



void Bob_add_salary(const int month) {
    
    Bob.bank_account += Bob.salary; // добавляем зп на счёт

    if (month % 12 == 0) {
        Bob.salary *= (1 + INDEXATION);
    }	
}

void Bob_rent(const int year, const int month) {
    Bob.bank_account -= Bob.rent;
	Bob.rent *= (INFLATION / 12 + 1);
}

void Alice_payment(cents payment) {
	Alice.bank_account -= payment;

}

void Bob_spend_for_life(const int year, const int month) {
    Bob.bank_account -= Bob.food;
	Bob.food *= (INFLATION / 12 + 1);
}

void Alice_spend_for_life(const int year, const int month) {
	Alice.bank_account -= Alice.food;
	Alice.food *= (INFLATION / 12 + 1);
}

void Bob_bank_percent(const int year, const int month)
{
		Bob.bank_account *= (1 + DEPOSIT_PERCENT / 12);
	}

void Alice_bank_percent(const int year, const int month) {
	Alice.bank_account *= (1 + BANK_PERCENT / 12);
}
		

void Alice_inflation(const int year, const int month) {
	flat_cost *= (1 + INFLATION / 12);
}

void print_result() {
	double alice_sum = flat_cost + Alice.bank_account;
	double bob_sum = Bob.bank_account;
	alice_sum /= 100;
	bob_sum /= 100;
	printf("alice_capital = %f \n", (alice_sum));
	printf("bob_capital = %f \n", (bob_sum));
}

void simulation(const cents alice_month_payment) {
	int year = YEAR;
	int years_count = YEARS_COUNT;
	int month = MONTH;

	while (month != MONTH + 12 * years_count) {


			Bob_bank_percent(year, month);
			Bob_add_salary(month);
			Bob_rent(year, month);
			Bob_spend_for_life(year, month);
			
			Alice_add_salary(year, month);
			Alice_payment(alice_month_payment);
			Alice_spend_for_life(year, month);
			Alice_bank_percent(year, month);
			Alice_inflation(year, month);

			++month;

			if (month == 12) {
				++year;
			}
		}
}


int main() {
	
	Bob_initially();
	Alice_initially();

	cents alice_month_payment = Alice_month_pay();

	simulation(alice_month_payment);
	print_result();

	return 0;
}

