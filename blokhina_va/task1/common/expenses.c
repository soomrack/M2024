#include <stdio.h>
#include "../alice/alice_monthly_payment.h"
#include "../variables.h"

// Функция считает сумму всех значений, повышенных на инфляцию
double percentage_calculation(double value, int time_unit){
    double sum = 0;
    for (int i=0; i<time_unit; i++){
        value = value * (1 + INFLATION);
        sum = sum + value;
    }
    return sum;
}

// Функция считает сколько потратит денег за YEARS лет с учетом инфляции при неизменном базом числе
double entire_period_expenses(double expenses, int years){
    double full_expenses = percentage_calculation(expenses * 12, years);
    return full_expenses;
}

double entire_period_deposit_balance(){
    double margin = (SALARY - WASTES - FLAT_RENT);
    double balance = margin * 12 * recursive((1 + DEPOSIT_RATE),YEARS);
    printf("On deposit for %d years: %.0f\n", YEARS, balance);

    return balance;
}