#include <iostream>
#include <stdlib.h>

using namespace std;

typedef long long int money;//копейки


const short int credit_duration = 20;
const short int inflation = 12;
const short int duration = 30;
const short int renovation_duration = 2;
const money alice_capital = 2 * 1000 * 1000 * 100;
const money bob_capital = 2 * 1000 * 1000 * 100;

money indexation(money any_money)
{
    any_money = any_money * inflation / 100 + any_money;
    return any_money;
}


short int bob_deposit_interest = 16;
money rent = 25 * 1000 * 100;
money bob_salary = 150 * 1000 * 100;
money bob_food_cost = 15 * 1000 * 100;
money bob_deposit = bob_capital;
money unexpected_waste = 0;

void bob_index_all()
{
    bob_salary = indexation(bob_salary);
    bob_food_cost = indexation(bob_food_cost);
    rent = indexation(rent);
}


void bob_deposit_add_per_month()
{
    money balance = bob_salary - bob_food_cost - rent - unexpected_waste;
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
    printf("Капитал Боба спустя 30 лет составил:  \n%lli %s %02lli %s", (bob_deposit / (100)), " рублей  ", bob_deposit % 100," копеек\n");
}


short int loan_interest = 18;
short int alice_deposit_interest = 16;
money flat_cost = 20 * 1000 * 1000 * 100;
money alice_salary = 150 * 1000 * 100;
money alice_food_cost = 15 * 1000 * 100;
money renovation_per_year = 100 * 1000 * 100;
money credit = flat_cost;
money alice_deposit = alice_capital;
money alice_unexpected_waste = 0;
bool is_enough_money = true;


void alice_index_all()
{
    alice_salary = indexation(bob_salary);
    alice_food_cost = indexation(bob_food_cost);
    flat_cost = indexation(flat_cost);
}


money payment_per_month()//возвращает остаток после оплаты кредита 
{
    money balance = alice_salary - alice_food_cost - alice_unexpected_waste;
    if (credit > 0) {
        money credit_payment = credit * loan_interest / 12 / 100;
        balance -= credit_payment;
        credit -= credit / credit_duration / 12;
        if (alice_deposit < credit_payment - balance) {//срабатывает только когда проценты больше чем баланс и на вкладе нет денег
            is_enough_money = false;
        }
    }
    return balance;
}


void alice_deposit_add_per_month(money alice_balance)
{
    if (is_enough_money) {
        alice_deposit += alice_balance;
        alice_deposit += alice_deposit * alice_deposit_interest / 12 / 100;
    }
}


void renovation(short int year)
{
    if (is_enough_money){
        if (year < renovation_duration) {
            alice_deposit -= renovation_per_year;
            renovation_per_year = indexation(renovation_per_year);
        }
    }
}


void alice_result()
{
    if (is_enough_money) {
        printf("Капитал Алисы спустя 30 лет составил:  \n%lli %s %02lli %s", ((alice_deposit + flat_cost) / (100)), " рублей  ", (alice_deposit + flat_cost) % 100, " копеек\n");
    }
    else {
        printf("Алиса разорилась");
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    for (short int year = 0; year < duration; year += 1)
    {
        for (short int month = 0; month < 12; month += 1)
        {
            alice_deposit_add_per_month(payment_per_month());
            bob_deposit_add_per_month();
        }
        bob_index_all();
        alice_index_all();
        renovation(year);
    }
    bob_result();
    printf("\n");
    alice_result();
    return 0;
}
