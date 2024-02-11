#include <stdio.h>
#include "./alice/alice_monthly_payment.h"
#include "./common/expenses.h"

// Alice
int credit = 18*1000*1000;
float credit_rate = 0.18;
int months = 30 * 12;

//Common Expenses
int food = 15*1000;
int flat_renovation = 100*1000;
int flat_rent = 25*1000;
float inflation = 0.12;

int main(void) {
    printf("Bob has spent money for 30 years: %.0f\n",\
    all_time_expenses((food * 12 + flat_rent * 12), (months / 12)));
    printf("Alice monthly credit payment: %.0f\n", alice_monthly_payment());
    return 0;
}