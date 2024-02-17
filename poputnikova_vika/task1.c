/*Alice takes mortgage.
Bob rents an apartament and keeps money in deposit.
Whose strategy is better? */

#include <stdio.h>
#include <math.h>

typedef long long int Money; // money

const int BUDGET = 3 * 1000 * 1000;
const int FLAT_PRICE = 10 * 1000 * 1000;
const int FLAT_RENT = 40 * 1000;
const int PERIOD = 30 * 12; // months
const float DEPOSIT_RATE = 0.14 / 12;
const float MORTGAGE_RATE = 0.17 / 12;
const float INFLATION = 0.07 / 12;
const int CURRENT_YEAR = 2024;
const int CURRENT_MONTH = 2;

typedef struct Person
{
    Money balance;
    Money income;
    Money expenses;
} Person;

Person alice = {BUDGET, 150 * 1000, 40 * 1000};
Person bob = {BUDGET, 150 * 1000, 40 * 1000};

Money deposit(const Money bank_balance, const Money monthly_income) {
    double deposit_income = bank_balance * (1 + DEPOSIT_RATE);
    for (int month = 1; month < PERIOD - 1; month++) {
        if (month == 10 * 12) {
            const int alice_promotion = 80 * 1000;
            const int bob_promotion = 80 * 1000;
            alice.income += alice_promotion;
            bob.income += bob_promotion;
        }
        deposit_income = (deposit_income + monthly_income * (1 + INFLATION)) * (1 + DEPOSIT_RATE);
    }
    return deposit_income;
}

Money mortgage() {
    Money monthly_payment =
        (FLAT_PRICE - BUDGET) * MORTGAGE_RATE * pow(1 + MORTGAGE_RATE, PERIOD) / (pow(1 + MORTGAGE_RATE, PERIOD) - 1);
    // https://mortgage-calculator.ru/формула-расчета-ипотеки/
    // printf("Ежемесячный платеж по ипотеке %d\n", monthly_payment);
    return monthly_payment;
}

Money calculate_Alice() {
    alice.balance -= BUDGET;
    alice.income -= mortgage() + alice.expenses; // monthly income
    // printf("Доход Alise за месяц %d\n", alice.income);
    alice.balance = deposit(alice.balance, alice.income);
    return alice.balance;
}

Money calculate_Bob() {
    bob.income -= FLAT_RENT + bob.expenses; // monthly income
    // printf("Доход Boba за месяц %d\n", bob.income)
    bob.balance = deposit(bob.balance, bob.income);
    return bob.balance;
}

int calculate_Alice_Bob() {
    if (CURRENT_MONTH == 2 && CURRENT_YEAR == 2024)
    {
        calculate_Alice();
        calculate_Bob();
    }
    else
        printf("Вне периода. Расчет не проводился. Введите 2 месяц 2024 год\n");
}

int show_print() {
    printf("Баланс в банке Alice: %d\n", alice.balance);
    printf("Баланс в банке Bob: %d\n", bob.balance);
}

int main() {
    calculate_Alice_Bob();
    show_print();
    return 0;
}
