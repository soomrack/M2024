#include <stdlib.h>
#include <iostream>


using namespace std;


typedef long long cash;


const int credit_duration = 30;
const int inflation = 12;
const int duration = 30;
const int renovation_duration = 2;


cash rent = 25 * 1000;
int loan_percent = 18;
cash flat_cost = 20 * 1000 * 1000;
cash renovation_per_year = 100 * 1000 * 12;
int current_year = 2024;


struct Person {
    cash capital = 2 * 1000 * 1000;
    cash salary = 150 * 1000;
    cash food_cost = 15 * 1000;
    cash additional_costs = 0;
    cash deposit = 2 * 1000 * 1000;
    int deposit_percent = 16;
};


cash indexation_per_year(cash money)
{
    money = money * (1 + inflation / 100);
    return money;
}


void bob_index_all_per_year(struct Person* person)
{

    (*person).salary = indexation_per_year((*person).salary);
    (*person).food_cost = indexation_per_year((*person).food_cost);
    (*person).additional_costs = indexation_per_year((*person).additional_costs);
    rent = indexation_per_year(rent);
}


void bob_deposit_add_per_month(struct Person* person)
{
    cash balance = (*person).salary - (*person).food_cost - rent - (*person).additional_costs;
    (*person).deposit *= (1 + (*person).deposit_percent / (12 * 100));
    (*person).deposit += balance;
}


void bob_result(struct Person* person)
{
    printf("Капитал Боба спустя 30 лет составил: %d\n", int((*person).deposit));
}


void alice_index_all_per_year(struct Person* person)
{
    (*person).salary = indexation_per_year((*person).salary);
    (*person).food_cost = indexation_per_year((*person).food_cost);
    flat_cost = indexation_per_year(flat_cost);
}


cash payment_per_month()
{
    cash credit_payment = flat_cost * (1 + loan_percent / (100 * 12)) / (1 - powf((1 + (loan_percent / (100 * 12))), -duration * 12 - 1));
    return credit_payment;
}


cash alice_balance(cash credit_payment, struct Person* person)
{
    cash balance = (*person).salary - (*person).food_cost - (*person).additional_costs;
    balance -= credit_payment;
    return balance;
}


void renovation(int year, cash balance, struct Person* person)
{
    if (balance >= renovation_per_year / 12) {
        if (year < current_year + renovation_duration) {
        balance -= renovation_per_year / 12;
        renovation_per_year = indexation_per_year(renovation_per_year);
        }
    }
}


void alice_result(struct Person* person) {
    printf("Капитал Алисы спустя 30 лет составил: %d\n", int((*person).deposit + flat_cost));
}


void overall_result(struct Person* person1, struct Person* person2)
{
    bob_result(person1);
    printf("\n");
    alice_result(person2);
}



void alice_deposit_add_per_month(cash balance, struct Person* person)
{
    if (balance > 0) {
        (*person).deposit *= (1 + (*person).deposit_percent / (12 * 100));
        (*person).deposit += balance;
    }
    else {
        if ((*person).deposit > 0) {
        (*person).deposit *= (1 + (*person).deposit_percent / (12 * 100));
        (*person).deposit += balance;
        }
        else {
            (*person).deposit += balance;
        }
    }
}



int main()
{
    setlocale(LC_ALL, "rus");

    Person Bob;
    Person Alice;
    for (int year = current_year; year < duration + current_year; year++)
    {
        for (int month = 1; month <= 12; month++)
        {
            payment_per_month();
            alice_deposit_add_per_month(alice_balance(payment_per_month(), &Alice), &Alice);
            bob_deposit_add_per_month(&Bob);
            printf("%d: ", int(Bob.salary));
        }
        bob_index_all_per_year(&Bob); 
        alice_index_all_per_year(&Alice); 
        renovation(year, (alice_balance(payment_per_month(), &Alice)), &Alice); 
    }
    overall_result(&Bob, &Alice);
    return 0;
}
