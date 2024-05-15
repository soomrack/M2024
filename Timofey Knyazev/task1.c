#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

typedef long long int money;  // êîïåéêè
int boat_price = 30000;
bool has_boat = false; //флаг наличия лодки
int TOTAL_YEARS = 30;
double INFLATION = 12 * 0.01;

struct Person {
	int if_has_apartment;
	money apartment_price;
	money savings;  // ñóììà äåíåã íà âêëàäå
	int loan_period;  // â ãîäàõ

	// â ìåñÿö
	money salary;
	money food_spending;
	money rent;
	money loan_payment;  // âû÷èñëÿåòñÿ â ôóíêöèè compute_loan_payment
	money additional_spendings;

	// â ãîä
	money apartment_repairs;
	double loan_rate;
	double deposit_rate;
};

struct Person Bob;
struct Person Alice;

void calculation_loan_payment(struct Person*);  // âû÷èñëåíèå åæåìåñÿ÷íîé îïëàòû êðåäèòà

void init_BOB();
void BOBs_salary_increase(const int, const int);  // åæåãîäíîå ïîâûøåíèå çàðïëàòû
void BOBs_salary_getting(const int, const int);  // åæåìåñÿ÷íîå íà÷èñëåíèå çàðïëàòû
void BOB_buy_food(const int, const int);  // åæåìåñÿ÷íàÿ ïîêóïêà åäû
void BOB_pay_rent(const int, const int);  // åæåìåñÿ÷íàÿ îïëàòà àðåíäû
void BOB_pay_loan(const int, const int);  // åæåìåñÿ÷íàÿ ïëàòà ïî êðåäèòó
void BOBs_unexpectable_spends(const int, const int);  // åæåìåñÿ÷íûé ó÷åò äîïîëíèòåëüíûõ ðàñõîäîâ
void BOB_apartment_renovation(const int, const int);  // åæåãîäíûé ðåìîíò êâàðòèðû
void BOBs_percents_by_deposit(const int, const int);  //  åæåìåñÿ÷íàÿ âûïëàòà ïðîöåíòîâ ïî âêëàäó
void BOB_has_house();
void BOB_print_results();  // Âûâîä ðåçóëüòàòîâ Áîáà

void init_ALICE();
void ALICEs_salary_increase(const int, const int);  // åæåãîäíîå ïîâûøåíèå çàðïëàòû
void ALICEs_salary_getting(const int, const int);  // åæåìåñÿ÷íîå íà÷èñëåíèå çàðïëàòû
void ALICE_buy_food(const int, const int);  // åæåìåñÿ÷íàÿ ïîêóïêà åäû
void ALICE_pay_rent(const int, const int);  // åæåìåñÿ÷íàÿ îïëàòà àðåíäû
void ALICE_pay_loan(const int, const int);  // åæåìåñÿ÷íàÿ ïëàòà ïî êðåäèòó
void ALICEs_unexpectable_spends(const int, const int);  // åæåìåñÿ÷íûé ó÷åò äîïîëíèòåëüíûõ ðàñõîäîâ
void ALICE_apartment_renovation(const int, const int);  // åæåãîäíûé ðåìîíò êâàðòèðû
void ALICEs_percents_by_deposit(const int, const int);  //  åæåìåñÿ÷íàÿ âûïëàòà ïðîöåíòîâ ïî âêëàäó
void ALICE_has_house();
void ALICE_print_results();  // Âûâîä ðåçóëüòàòîâ Àëèñû

void inflate(const int, const int);  // åæåìåñÿ÷íûé ðîñò öåí
void calculation();  // âû÷èñëåíèå ðåçóëüòàòîâ


void calculation_loan_payment(struct Person* person)
{
	double numerator = pow(1 + person->loan_rate / 12, person->loan_period * 12);
	double denominator = numerator - 1;
	person->loan_payment = (money)((person->apartment_price - person->savings)
		* person->loan_rate / 12 * numerator / denominator);

	if (person->loan_rate == 0) {
		person->loan_payment = 0;
		person->if_has_apartment = 0;
	}
	else {
		person->if_has_apartment = 1;
	}

	if (person->loan_payment > person->salary) {
		printf("Ïðîãðàììà îñòàíîâëåíà: Ïëàòà ïî êðåäèòó áîëüøå çàðàáîòíîé ïëàòû");
		exit(0);
	}
}


void init_BOB()
{
	Bob.if_has_apartment = 0;
	Bob.apartment_price = 20 * 1000 * 1000 * 100;
	Bob.savings = 2 * 1000 * 1000 * 100;
	Bob.loan_period = 0;
	Bob.salary = 300 * 1000 * 100;
	Bob.food_spending = 15000 * 100;
	Bob.rent = 25000 * 100;
	Bob.additional_spendings = 0;
	Bob.apartment_repairs = 0;
	Bob.loan_rate = 0;
	Bob.deposit_rate = 16 * 0.01;

	calculation_loan_payment(&Bob);
}

void BOBs_salary_increase(const int month, const int year)
{
	Bob.salary += Bob.salary * INFLATION;
}


void BOBs_salary_getting(const int month, const int year)
{
	Bob.savings += Bob.salary;

	if (month == 12) {
		BOBs_salary_increase(year, month);
	}
}


void BOB_buy_food(const int month, const int year)
{
	Bob.savings -= Bob.food_spending;
}


void BOB_pay_rent(const int month, const int year)
{
	Bob.savings -= Bob.rent;
}


void BOB_pay_loan(const int month, const int year)
{
	Bob.savings -= Bob.loan_payment;
}


void BOBs_unexpectable_spends(const int month, const int year)
{
	Bob.savings -= Bob.additional_spendings;
}


void BOB_apartment_renovation(const int month, const int year)
{
	Bob.savings -= Bob.apartment_repairs;
}


void BOBs_percents_by_deposit(const int month, const int year)
{
	long double income = Bob.savings * Bob.deposit_rate / 12;
	Bob.savings += income;
}


void BOB_has_house()
{
	if (!Bob.if_has_apartment) {
		if (Bob.savings >= Bob.apartment_price) {
			Bob.savings -= Bob.apartment_price;
			Bob.if_has_apartment = 1;
		}
	}
}

void BOB_buy_boat(int year, int month) {
	if (year == 2026 && month == 1) {
		Bob.savings -= boat_price;
		has_boat = true;
	}
}

void BOB_pay_boat() {
	if (has_boat == true)
		Bob.savings -= boat_price;
}

void BOB_print_results()
{
	money Bob_savings_roubles = Bob.savings / 100;
	long long int count_digits = 1;
	while (count_digits < Bob_savings_roubles) {
		count_digits *= 1000;
	}
	count_digits /= 1000;

	printf("\nÍàêîïëåíèÿ Áîáà: ");

	printf("%lld ", Bob_savings_roubles / count_digits);
	Bob_savings_roubles %= count_digits;
	count_digits /= 1000;

	for (count_digits; count_digits > 0; count_digits /= 1000) {
		int number_to_print = Bob_savings_roubles / count_digits;
		printf("%03d ", number_to_print);
		Bob_savings_roubles %= count_digits;
	}
	printf("ðóáëåé %02lld êîïååê", Bob.savings % 100);
}
void init_ALICE()
{
	Alice.if_has_apartment = 0;
	Alice.apartment_price = 20 * 1000 * 1000 * 100;
	Alice.savings = 2 * 1000 * 1000 * 100;
	Alice.loan_period = 30;
	Alice.salary = 300 * 1000 * 100;
	Alice.food_spending = 15000 * 100;
	Alice.rent = 0;
	Alice.additional_spendings = 0;
	Alice.apartment_repairs = 100 * 1000 * 100;
	Alice.loan_rate = 18 * 0.01;
	Alice.deposit_rate = 16 * 0.01;

	calculation_loan_payment(&Alice);
}


void ALICEs_salary_increase(const int month, const int year)
{
	Alice.salary += Alice.salary * INFLATION;
}


void ALICEs_salary_getting(const int month, const int year)
{
	Alice.savings += Alice.salary;

	if (month == 12) {
		ALICEs_salary_increase(year, month);
	}
}

void ALICE_buy_food(const int month, const int year)
{
	Alice.savings -= Alice.food_spending;
}


void ALICE_pay_rent(const int month, const int year)
{
	Alice.savings -= Alice.rent;
}

void ALICE_pay_loan(const int month, const int year)
{
	Alice.savings -= Alice.loan_payment;
}


void ALICEs_unexpectable_spends(const int month, const int year)
{
	Alice.savings -= Alice.additional_spendings;
}

void ALICE_apartment_renovation(const int month, const int year)
{
	Alice.savings -= Alice.apartment_repairs;
}

void ALICEs_percents_by_deposit(const int month, const int year)
{
	long double income = Alice.savings * Alice.deposit_rate / 12;
	Alice.savings += income;
}


void ALICE_has_house()
{
	if (!Alice.if_has_apartment) {
		if (Alice.savings >= Alice.apartment_price) {
			Alice.savings -= Alice.apartment_price;
			Alice.if_has_apartment = 1;
		}
	}
}

void ALICE_print_results()
{
	money Alice_savings_roubles = Alice.savings / 100;
	long long int count_digits = 1;
	while (count_digits < Alice_savings_roubles) {
		count_digits *= 1000;
	}
	count_digits /= 1000;

	printf("\nÍàêîïëåíèÿ Àëèñû: ");

	printf("%lld ", Alice_savings_roubles / count_digits);
	Alice_savings_roubles %= count_digits;
	count_digits /= 1000;

	for (count_digits; count_digits > 0; count_digits /= 1000) {
		int number_to_print = Alice_savings_roubles / count_digits;
		printf("%03d ", number_to_print);
		Alice_savings_roubles %= count_digits;
	}
	printf("ðóáëåé %02lld êîïååê", Alice.savings % 100);
}



void inflate(const int month, const int year)
{
	Bob.food_spending += Bob.food_spending * INFLATION / 12;
	Bob.additional_spendings += Bob.additional_spendings * INFLATION / 12;

	Alice.food_spending += Alice.food_spending * INFLATION / 12;
	Alice.additional_spendings += Alice.additional_spendings * INFLATION / 12;
}



void calculation()
{
	time_t time_ = time(NULL);
	struct tm time_now = *localtime(&time_);

	int starting_year = time_now.tm_year + 1900;
	int starting_month = time_now.tm_mon + 1;
	int year = starting_year;
	int month = starting_month;

	while (!(year == starting_year + TOTAL_YEARS && month == starting_month)) {
		BOBs_salary_getting(year, month);
		BOB_buy_food(year, month);
		BOB_pay_rent(year, month);
		BOB_pay_loan(year, month);
		BOBs_unexpectable_spends(year, month);
		BOBs_percents_by_deposit(year, month);
		BOB_has_house();
		BOB_buy_boat(year, month);
		BOB_pay_boat();

		ALICEs_salary_getting(year, month);
		ALICE_buy_food(year, month);
		ALICE_pay_rent(year, month);
		ALICE_pay_loan(year, month);
		ALICEs_unexpectable_spends(year, month);
		ALICEs_percents_by_deposit(year, month);
		ALICE_has_house();

		inflate(year, month);

		if (month == 12) {
			BOB_apartment_renovation(year, month);
			ALICE_apartment_renovation(year, month);
		}

		if (month != 12) {
			month += 1;
		}
		else {
			month = 1;
			year += 1;
		}

	}
}


int main()
{
	setlocale(LC_ALL, "");

	init_BOB();
	init_ALICE();

	calculation();

	BOB_print_results();
	ALICE_print_results();
}

