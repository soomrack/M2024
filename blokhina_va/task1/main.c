#include <stdio.h>
#include "./alice/alice_monthly_payment.h"
#include "./common/expenses.h"
#include "./variables.h"

// Запускать выполнение кода можно через скрипт ./build

int main() {
    // Вспомогательные переменные для читаемости кода, используются только в printf() 
    double alice_payment_for_credit_for_YEARS = alice_monthly_payment()* MONTHS;
    double alice_wastes_for_YEARS = \
    entire_period_expenses(WASTES, YEARS) + entire_period_expenses(FLAT_RENOVATION, FLAT_RENOVATION_YEARS);
    double alice_all_payments_for_YEARS = alice_monthly_payment()* MONTHS \
    + entire_period_expenses(WASTES, YEARS) + entire_period_expenses(FLAT_RENOVATION, FLAT_RENOVATION_YEARS);

    double bob_wastes_for_YEARS = entire_period_expenses((WASTES + FLAT_RENT), YEARS);
    double flat_price_after_YEARS = FLAT_PRICE * recursive((1 + INFLATION), YEARS);

    printf("---------ALICE---------\n");
    printf("Alice payment for credit for %d years: %.0f\n", YEARS, alice_payment_for_credit_for_YEARS);
    printf("Alice wastes for %d years without credit: %.0f\n", YEARS, alice_wastes_for_YEARS);
    printf("Alice payment for credit and wastes for 30 %d years: %.0f\n", YEARS, alice_all_payments_for_YEARS);

    printf("---------BOB---------\n");
    printf("Bob has spent money for %d years: %.0f\n", YEARS, bob_wastes_for_YEARS);
    printf("After %d years flat will cost: %.0f\n", YEARS, flat_price_after_YEARS);
    double bob_deposit_balance = entire_period_deposit_balance();

    printf("---------FINALLY---------\n");
    if (alice_all_payments_for_YEARS != bob_wastes_for_YEARS){
        if (alice_all_payments_for_YEARS < bob_wastes_for_YEARS)
            printf("Alice spent less money.\n");
        else
            printf("Bob spent less money.\n");
    }
    else
        printf("Strangely, they spent the same amount\n");

    if (bob_deposit_balance < flat_price_after_YEARS)
        printf("Unfortunately Bob didn't save enough money up for an apartment:(\n");
    else
        printf("Bob can buy a flat!\n");

    return 0;    
}