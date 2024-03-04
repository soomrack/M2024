/*Alice takes mortgage. Bob rents an apartament. Whose strategy is better? */

#include <stdio.h>
#include <math.h>

const int FLAT_PRICE = 10 * 1000 * 1000;
const int BUDGET = 3 * 1000 * 1000;
const int FLAT_RENT = 40 * 1000;
const int SALARY = 150 * 1000;
const float INFLATION = 0.07;

const int FIRST_MONTH = 3;
const int LAST_MONTH = 2;

const int FIRST_YEAR = 2024;
const int LAST_YEAR = 2054;

float DEPOSIT_RATE = 0.14 / 12;
float MORTGAGE_RATE = 0.17 / 12;

typedef long long int Money;

struct Person {
    Money balance;
    Money income;
    Money expenses;
};

struct Person bob;
struct Person alice;


void bob_init() {
    bob.balance = BUDGET;
    bob.expenses = 40 * 1000 + FLAT_RENT;
    bob.income = SALARY;
}


void alice_init() {
    int years = LAST_YEAR - FIRST_YEAR;
    alice.balance = 0;
    alice.expenses = 40 * 1000;
    alice.income = SALARY;
}


void bob_income(int year, int month) {
    bob.balance += bob.income;
}


void alice_income(int year, int month) {
    alice.balance += alice.income;
}


void bob_expenses_index(int year, int month) {
    if (month == 12) {
        bob.balance -= bob.expenses * 1.1;
    }
    bob.balance -= bob.expenses;
}


void alice_expenses_index(int year, int month) {
    if (month == 12) {
        alice.balance -= alice.expenses * 1.1;
    }
    alice.balance -= alice.expenses;
}

void alice_mortgage() {
    int years = LAST_YEAR - FIRST_YEAR;
    alice.balance -=
        (FLAT_PRICE - BUDGET) * MORTGAGE_RATE * pow(1 + MORTGAGE_RATE, years * 12) / (pow(1 + MORTGAGE_RATE, years * 12) - 1);
    //  https://mortgage-calculator.ru/формула-расчета-ипотеки/
}


void bob_deposite(int year, int month) {
    bob.balance *= (1 + DEPOSIT_RATE);
}


void alice_deposite(int year, int month) {
    alice.balance *= (1 + DEPOSIT_RATE);
}


void bob_promotion(int year, int month) {
    if (month == 12) {
        bob.income *= 1.1;
    }
}


void alice_promotion(int year, int month) {
    if (month == 12) {
        alice.income *= 1.1;
    }
}


void inflation(int year, int month) {
    if (month == 12) {
        bob.income *= (1 + INFLATION / 1.5);
        alice.income *= (1 + INFLATION / 1.5);
    }
}


void buy_car(int year, int month) {
    if (year == 2034 && month == 2) {
        alice.balance -= 2 * 1000 * 1000;
    }
}


void simulation() {
    int month = FIRST_MONTH;
    int year = FIRST_YEAR;

    while (!(year == LAST_YEAR && month == LAST_MONTH)) {

        bob_deposite(year, month);
        bob_income(year, month);
        bob_expenses_index(year, month);
        bob_promotion(year, month);
        // buy_horse(year, month);

        alice_deposite(year, month);
        alice_income(year, month);
        alice_expenses_index(year, month);
        alice_promotion(year, month);
        alice_mortgage();
        buy_car(year, month);

        inflation(year, month);

        month++;
        if (month > 12) {
            year++;
            month = 1;
        }
    }
}


void print() {
    printf("\nБаланс Боба: %lld %s", bob.balance/100, "рублей");
    printf(" %02d %s\n", bob.balance % 100, "копеек");
    printf("\nБаланс Алисы: %lld %s", alice.balance/100, "рублей");
    printf(" %02d %s\n", alice.balance % 100, "копеек");
}


int main() {
    bob_init();
    alice_init();

    simulation();

    print();

    return 0;
}
