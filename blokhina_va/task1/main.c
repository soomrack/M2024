#include <stdio.h>
#include "./alice/alice_monthly_payment.h"
#include "./common/expenses.h"
#include "./variables.h"

int main() {
    // printf("K ratio: %.0f", K_ratio());
    // printf("Bob has spent money for 30 YEARS: %.0f\n",\
    // expenses((wastes * 12 + FLAT_RENT * 12), (MONTHS / 12)));
    // printf("Alice payment for credit for 30 YEARS: %.0f\n", alice_monthly_payment()* MONTHS);
    // printf("Alice payment for credit and wastes for 30 YEARS: %.0f\n", alice_monthly_payment()* MONTHS + inflation_calculation(WASTES * 12, MONTHS/12, 0) + inflation_calculation(flat_renovation * 12, 2, 0));
    // printf("Alice overpayment: %.0f\n", alice_monthly_payment()* MONTHS - credit - 2*1000*1000);
    // printf("Recursice: %f\n", recursive(1.009, 30));

    printf("Recursice: %f\n", recursive(a, b));

    return 0;
}