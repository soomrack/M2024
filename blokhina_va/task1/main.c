#include <stdio.h>
#include "./alice/alice_monthly_payment.h"
#include "./common/expenses.h"

// Alice
float credit = 18*1000*1000;
float credit_rate = 0.11;
int months = 30 * 12;

// Bob
float salary = 150*1000;

// Common Expenses
float wastes = 30*1000;
float flat_renovation = 100*1000;
int flat_rent = 25*1000;
float inflation = 0.04;

int main() {
    printf("Bob has spent money for 30 years: %.0f\n",\
    expenses((wastes * 12 + flat_rent * 12), (months / 12)));

    printf("Alice monthly credit payment: %.0f\n", alice_monthly_payment());
    printf("Alice payment for credit for 30 years: %.0f\n", alice_monthly_payment()* months);
    printf("Alice payment for credit and wastes for 30 years: %.0f\n", alice_monthly_payment()* months + inflation_calculation(wastes * 12, months/12)+inflation_calculation(flat_renovation * 12, 2));
    printf("Alice overpayment: %.0f\n", alice_monthly_payment()* months - credit - 2*1000*1000);
    return 0;
}