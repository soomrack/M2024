#include "../variables.h"

double recursive(double base, int exponent){
    printf("base: %.0f\n", base);
    printf("exponent: %d\n", exponent);
    if (exponent == 0)
        return 1;
    else
        return base * recursive(base, exponent - 1);
}

// K ratio for monthly alice payment
float K_ratio(void){
    float monthly_credit_rate = CREDIT_RATE / 12;
    printf("1step: %.f\n", monthly_credit_rate);

    float all_credit_time_inflation = \
    recursive((1 + monthly_credit_rate), MONTHS);
    printf("2step: %.0f\n", all_credit_time_inflation);

    float K = (monthly_credit_rate * all_credit_time_inflation)/\
    (all_credit_time_inflation - 1);
    printf("K ratio: %.0f\n", K);

    // return K;
    return 2;
}

// Alice monthly payment
float alice_monthly_payment (){
    float monthly_payment = CREDIT * K_ratio();
    return monthly_payment;
}