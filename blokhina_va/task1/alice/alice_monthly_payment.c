#include "../variables.h"

double recursive(double base, int exponent){
    if (exponent == 0)
        return 1;
    else
        return base * recursive(base, exponent - 1);
}

// K ratio for monthly alice payment
// TODO если вызывать функцию в main.c, то возвращает 0.000000
double K_ratio(void){
    double monthly_credit_rate = CREDIT_RATE / 12;

    double all_credit_time_inflation = \
    recursive((1 + monthly_credit_rate), MONTHS);

    double K = (monthly_credit_rate * all_credit_time_inflation)/\
    (all_credit_time_inflation - 1);

    return K;
}

// Alice monthly payment
double alice_monthly_payment(){
    double monthly_payment = CREDIT * K_ratio();
    return monthly_payment;
}