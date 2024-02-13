#include <iostream>

typedef long long int Money;  // копейки


const int CURRENT_YEAR = 2024;
const int credit_duration = 30;
const int inflation = 6;
const int duration = 30;
const int renovation_duration = 2;
const Money alice_capital = 2 * 1000 * 1000 * 100;
const Money bob_capital = 2 * 1000 * 1000 * 100;

int bob_deposit_interest = 8;
Money rent = 25 * 1000 * 100;
Money bob_salary = 150 * 1000 * 100;
Money bob_food_cost = 15 * 1000 * 100;
Money bob_deposit = bob_capital;
Money unexpected_waste = 0;


int loan_interest = 10;
int alice_deposit_interest = 8;
Money flat_cost = 20 * 1000 * 1000 * 100;
Money alice_salary = 150 * 1000 * 100;
Money alice_food_cost = 15 * 1000 * 100;
Money renovation_per_year = 100 * 1000 * 100;
Money credit = flat_cost;
Money alice_deposit = alice_capital;
Money alice_unexpected_waste = 0;
bool is_enough_money = true;


Money indexation(Money any_money)
{
    any_money = any_money * inflation / 100 + any_money;
    return any_money;
}


void bob_index_all()
{
    bob_salary = indexation(bob_salary);
    bob_food_cost = indexation(bob_food_cost);
    rent = indexation(rent);
}


void bob_deposit_add_per_month()
{
    Money balance = bob_salary - bob_food_cost - rent - unexpected_waste;
    if (balance >= 0) {
        bob_deposit += bob_deposit * bob_deposit_interest / 12 / 100;
        bob_deposit += balance;
    }
    else {
        bob_deposit += balance;
        bob_deposit += bob_deposit * bob_deposit_interest / 12 / 100;
    }
}


void bob_result()
{
    printf("Капитал Боба спустя 30 лет составил:  \n%lli %s %02lli %s",
        (bob_deposit / (100)), " рублей  ", bob_deposit % 100," копеек\n");
}


void alice_index_all()
{
    alice_salary = indexation(alice_salary);
    alice_food_cost = indexation(alice_food_cost);
    flat_cost = indexation(flat_cost);
}


Money credit_payment_compute()
{
    double numerator = pow(1 + double(loan_interest) / 100 / 12, double(credit_duration) * 12);
    double denominator = numerator - 1;
    Money payment = credit * loan_interest / 100 / 12 * numerator / denominator;
    return payment;
}


Money alice_balance(Money credit_payment) 
{
    Money balance = alice_salary - alice_food_cost - alice_unexpected_waste;
    balance -= credit_payment;
    return balance;
}


void payment_per_month(Money balance,Money payment)
{
    if (credit > 0) {
        credit -= credit / credit_duration / 12;
        if (alice_deposit < payment - balance) {
            is_enough_money = false;
        }
    }
}


void alice_deposit_add_per_month(Money balance)
{
    alice_deposit += balance;
    alice_deposit += alice_deposit * alice_deposit_interest / 100 / 12;
}


void renovation(int year)
{
    if (is_enough_money){
        if (year < renovation_duration+CURRENT_YEAR) {
            alice_deposit -= renovation_per_year;
            renovation_per_year = indexation(renovation_per_year);
        }
    }
}


void alice_result()
{
    if (is_enough_money) {
        printf("Капитал Алисы спустя 30 лет составил:  \n%lli %s %02lli %s",
            ((alice_deposit + flat_cost) / (100)), " рублей  ",
            (alice_deposit + flat_cost) % 100, " копеек\n");
    }
    else {
        printf("Алиса разорилась");
    }
}


void common_result()
{
    bob_result();
    printf("\n");
    alice_result();
}


int main()
{
    setlocale(LC_ALL, "rus");

    Money payment_a_month = credit_payment_compute();

    for (int year = CURRENT_YEAR; year < CURRENT_YEAR+duration; year ++)
    {
        for (int month = 1; month <= 12; month ++)
        {
            if (is_enough_money) {
                payment_per_month(alice_balance(payment_a_month), payment_a_month);
                alice_deposit_add_per_month(alice_balance(payment_a_month));
            }
            bob_deposit_add_per_month();
        }
        bob_index_all();
        alice_index_all();
        renovation(year);
    }
    common_result();
    return 0;
}
