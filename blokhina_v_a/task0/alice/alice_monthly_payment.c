extern int credit;
extern float credit_rate;
extern int months;

float recursive(float base, int exponent){
    if (exponent == 0)
        return 1;
    else
        return base * recursive(base, exponent - 1);
}
// K ratio for monthly alice payment
float K_ratio(void){
    float monthly_credit_rate = credit_rate / 12;

    float all_credit_time_inflation = \
    recursive((1 + monthly_credit_rate), months);

    float K = (monthly_credit_rate * all_credit_time_inflation)/\
    (all_credit_time_inflation - 1);

    return K;
}
// Alice monthly payment
float alice_monthly_payment (){
    float monthly_payment = credit * K_ratio();
    return monthly_payment;
}