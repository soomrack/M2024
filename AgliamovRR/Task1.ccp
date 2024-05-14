#include <stdio.h>
#include <math.h>

typedef long long int Money;

const double DEPOSIT_RATE = 0.16; // ставка по вкладу
const double MORTGAGE_RATE = 0.18; // ставка по ипотеке
const int YEARS = 30; // период
const int START_YEAR = 2024; // первый год
const int START_MONTH = 4; // первый месяц
const Money SALARY = 200 * 1000 * 100; // зарплата
const Money START_CAPITAL = 2000 * 1000 * 100; // стартовый капитал
const Money RENT_COST = 25 * 1000 * 100; // стоимость аренды квартиры
const double INFLATION = 0.12; // инфляция
const Money SPENDINGS = 20 * 1000 * 100; // ежемесячные траты
const Money FLAT_COST = 20 * 1000 * 1000 * 100; // стоимость квартиры
const Money cat = 10*1000; //  стоимость кота
const Money korm = 10*100; // стоимость корма коту
const Money TRATA = 10*100; // траты на котика

typedef struct {
    Money salary;
    Money savings;
    Money outlay;
    Money rent_payment;
    Money flat_payment;
    Money kormit;
} FinancialData;

FinancialData alice;
FinancialData bob;

void init_alice(FinancialData* alice) {
    double monthly_coef = MORTGAGE_RATE / 12.0;
    int total_payments = YEARS * 12;
    double discount_factor = (pow(1 + monthly_coef, total_payments) - 1) /
                             (monthly_coef * pow(1 + monthly_coef, total_payments));
    alice->savings = 0;
    alice->salary = SALARY;
    alice->outlay = SPENDINGS;
    alice->flat_payment = (FLAT_COST - START_CAPITAL) / discount_factor;
}

void init_bob(FinancialData* bob) {
    bob->savings = START_CAPITAL;
    bob->salary = SALARY;
    bob->outlay = SPENDINGS;
    bob->rent_payment = RENT_COST;
    bob->kormit = TRATA;
}

void add_salary(FinancialData* person, int year, int month) {
    person->savings += person->salary;
}

void update_savings(FinancialData* person, int year, int month) {
    person->savings *= (1.0 + DEPOSIT_RATE / 12.0);
}

void subtract_expenses(FinancialData* person, int year, int month) {
    person->savings -= (person->flat_payment + person->outlay);
}

void bob_cat (FinancialData* person, int year, int month) {

    if (year == 2024 && month == 2)
    bob.outlay -= cat;
}

void korm_cat (FinancialData* person, int year, int month) {
    if (year == 2024 && month == 2)
    bob.outlay -= korm;
}

void change_inflation(int year, int month) {
    if (month == 12) {
        alice.salary *= (1.0 + INFLATION);
        bob.salary *= (1.0 + INFLATION);
        bob.flat_payment *= (1.0 + INFLATION);
        alice.outlay *= (1.0 + INFLATION);
        bob.outlay *= (1.0 + INFLATION);
        bob.kormit *= (1.0 + INFLATION);
    }
}

void simulation() {
    int year = START_YEAR;
    int month = START_MONTH;
    while ((month != 1) || year != (YEARS + START_YEAR)) {
        update_savings(&alice, year, month);
        add_salary(&alice, year, month);
        subtract_expenses(&alice, year, month);
        update_savings(&bob, year, month);
        add_salary(&bob, year, month);
        subtract_expenses(&bob, year, month);
        korm_cat(&bob, year, month);
        change_inflation(year, month);

        month++;
        if (month > 12) {
            year++;
            month = 1;
        }
    }
}

void print_results() {
    double alice_final = (FLAT_COST + alice.savings);
    double bob_final = bob.savings;
    alice_final /= 100;
    bob_final /= 100;
    printf("Alice's capital = %f\n", alice_final);
    printf("Bob's capital = %f\n", bob_final);
}

int main() {
    init_alice(&alice);
    init_bob(&bob);
    simulation();
    print_results();
    return 0;
}
