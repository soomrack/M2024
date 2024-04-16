#include <stdio.h>
#include <math.h>


typedef long long int kpk; // для расчёта в копейках

kpk potentialsalary;

const int PERIOD_YEARS = 20;
const int START_YEAR = 2024;
const int START_MONTH = 3;
const double INFLATION = 0.07;
const double BANK_PERCENT = 0.11;
const double MORTGAGE_PERCENT = 0.09;
const kpk START_SUM = 2000 * 1000 * 100;
const kpk START_MONTH_SALARY = 140 * 1000 * 100;
const kpk START_EXPENSES = 60 * 1000 * 100;
const kpk BOB_START_RENT = 50 * 1000 * 100;
const kpk ALICE_START_FLAT_COST = 10 * 1000 * 1000 * 100;


kpk mortgage_payment() {
	double month_percent = MORTGAGE_PERCENT / 12;
	double total_rate = pow((1 + month_percent), PERIOD_YEARS * 12);
	kpk mrtg_payment = (ALICE_START_FLAT_COST - START_SUM) * month_percent * total_rate / (total_rate - 1); 
	return mrtg_payment;
}


struct Person {
	kpk savings;
	kpk salary;
	kpk expenses;
	kpk flat;
};


struct Person Alice;
struct Person Bob;


void Alice_init() {
	Alice.savings = 0;
	Alice.salary = START_MONTH_SALARY;
	Alice.expenses = START_EXPENSES;
	Alice.flat = ALICE_START_FLAT_COST;
}


void Bob_init() {
	Bob.savings = START_SUM;
	Bob.salary = START_MONTH_SALARY;
	Bob.expenses = START_EXPENSES;
	Bob.flat = BOB_START_RENT;
}


void indexations(int month) {
	if (month % 12 == 0) {
		Alice.salary *= 1 + INFLATION;
		Bob.salary *= 1 + INFLATION;
		potentialsalary *= 1 + INFLATION;
	}
}


void inflations(int month) {
	if(month % 12 == 0){
		Alice.expenses *= 1 + INFLATION;
		Bob.expenses *= 1 + INFLATION;
		Alice.flat *= 1 + INFLATION;
		Bob.flat *= 1 + INFLATION;
	}
}


void savings() {
	Alice.savings *= 1 + BANK_PERCENT / 12;
	Bob.savings *= 1 + BANK_PERCENT / 12;
	Alice.savings += Alice.salary - Alice.expenses - mortgage_payment();
	Bob.savings += Bob.salary - Bob.expenses - Bob.flat;
}

void life_changes(int month) {
	if (month == (START_MONTH + (2026 - START_YEAR) * 12 + 2)) {
		potentialsalary = Bob.salary;
		Bob.salary = 0;
	}
	if (month == (START_MONTH + (2026 - START_YEAR) * 12 + 4)) {
		Bob.salary = potentialsalary * 2;
	}
}

void bob_purchase_yacht(int month) {                                                   // покупка лодки
	if (month == (START_MONTH + (2028 - START_YEAR) * 12)) {
		Bob.savings -= 600 * 1000 * 100;
	}
}

void bob_yacht_TO(int month) {                                                          //ТО лодки на год после покупки
	if (month < ((2029 - START_YEAR) * 12) && month > ((2028 - START_YEAR) * 12)){
		Bob.savings -= 30 * 1000 * 100;
	}
}

void bob_vacation(int month) {
	if ((month % 12) == 7) {
		Bob.salary == 0;
	}
	else {
		Bob.salary == potentialsalary;
	}
}

void lifecycle() {
	int month = START_MONTH;
	while (month != START_MONTH + 12 * PERIOD_YEARS) {
		life_changes(month);
		indexations(month);
		inflations(month);
		savings();
		bob_purchase_yacht(month);
		bob_yacht_TO(month);
		bob_vacation(month);
		++month;
	}
}

void print_result() {
	int Alice_capital = (Alice.savings + Alice.flat) / 100;
	printf("Alice's capital = %i RUB\n", Alice_capital);
	int Bob_capital = Bob.savings / 100;
	printf("Bob's capital = %i RUB\n", Bob_capital);
}


void main() {
	Alice_init();
	Bob_init();
	lifecycle();
	print_result();
}
