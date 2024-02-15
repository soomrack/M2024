#include <iostream>

typedef long long int Money;  // копейки


const int CURRENT_YEAR = 2024;
const int CREDIT_DURATION = 30;
const int INFLATION = 6;
const int DURATION = 30;
const int RENOVATION_DURATION = 2;

Money rent = 25 * 1000 * 100;
int LOAN_INTEREST = 10;
Money FLAT_COST = 20 * 1000 * 1000 * 100;
Money RENOVATION_PER_YEAR = 100 * 1000 * 100;
Money CREDIT = FLAT_COST;
bool IS_ENOUGH_MONEY = true;

struct Person {
    const Money capital = 2 * 1000 * 1000 * 100;
    Money salary = 150 * 1000 * 100;
    Money food_cost = 15 * 1000 * 100;
    Money unexpected_waste = 0;
    Money deposit = 2 * 1000 * 1000 * 100;
    int deposit_interest = 8;
};


Money indexation(Money any_money)
{
    any_money = any_money * INFLATION / 100 + any_money;
    return any_money;
}


void bob_index_all(struct Person *person)
{
    person->salary = indexation(person->salary);
    person->food_cost = indexation(person->food_cost);
    person->unexpected_waste = indexation(person->unexpected_waste);
    rent = indexation(rent);
}


void bob_deposit_add_per_month(struct Person *person)
{
    Money balance = person->salary - person->food_cost - rent - person->unexpected_waste;
    if (balance >= 0) {
        person->deposit += (person->deposit * person->deposit_interest) / 100 / 12;
        person->deposit += balance;
    }
    else {
        person->deposit += balance;
        person->deposit += (person->deposit * person->deposit_interest) / 100 / 12;
    }
}


void bob_result(struct Person *person)
{
    printf("Капитал Боба спустя 30 лет составил:  \n%lli %s %02lli %s",
        (person->deposit / (100)), " рублей  ", person->deposit % 100," копеек\n");
}


void alice_index_all(struct Person* person)
{
    person->salary = indexation(person->salary);
    person->food_cost = indexation(person->food_cost);
    FLAT_COST = indexation(FLAT_COST);
}


Money credit_payment_compute()
{
    double numerator = pow(1 + double(LOAN_INTEREST) / 100 / 12, double(CREDIT_DURATION) * 12);
    double denominator = numerator - 1;
    Money payment = CREDIT * LOAN_INTEREST / 100 / 12 * numerator / denominator;
    return payment;
}


Money alice_balance(Money credit_payment, struct Person* person)
{
    Money balance = person->salary - person->food_cost - person->unexpected_waste;
    balance -= credit_payment;
    return balance;
}


void payment_per_month(Money balance,Money payment, struct Person* person)
{
    if (CREDIT > 0) {
        CREDIT -= CREDIT / CREDIT_DURATION / 12;
        if (person->deposit < payment - balance) {
            IS_ENOUGH_MONEY = false;
        }
    }
}


void alice_deposit_add_per_month(Money balance, struct Person* person)
{
    if (balance >= 0) {
        person->deposit += (person->deposit * person->deposit_interest) / 100 / 12;
        person->deposit += balance;
    }
    else {
        person->deposit += balance;
        person->deposit += (person->deposit * person->deposit_interest) / 100 / 12;
    }
}


void renovation(int year, struct Person* person)
{
    if (IS_ENOUGH_MONEY){
        if (year < RENOVATION_DURATION+CURRENT_YEAR) {
            person->deposit -= RENOVATION_PER_YEAR;
            RENOVATION_PER_YEAR = indexation(RENOVATION_PER_YEAR);
        }
    }
}


void alice_result(struct Person* person)
{
    if (IS_ENOUGH_MONEY) {
        printf("Капитал Алисы спустя 30 лет составил:  \n%lli %s %02lli %s",
            ((person->deposit + FLAT_COST) / (100)), " рублей  ",
            (person->deposit + FLAT_COST) % 100, " копеек\n");
    }
    else {
        printf("Алиса разорилась");
    }
}


void common_result(struct Person *person1, struct Person *person2)
{
    bob_result(person1);
    printf("\n");
    alice_result(person2);
}


int main()
{
    setlocale(LC_ALL, "rus");

    Person Bob;
    Person Alice;
    Money payment_a_month = credit_payment_compute();

    for (int year = CURRENT_YEAR; year < CURRENT_YEAR+DURATION; year ++)
    {
        for (int month = 1; month <= 12; month ++)
        {
            if (IS_ENOUGH_MONEY) {
                payment_per_month(alice_balance(payment_a_month, &Alice), payment_a_month, &Alice);
                alice_deposit_add_per_month(alice_balance(payment_a_month, &Alice), &Alice);
            }
            bob_deposit_add_per_month(&Bob);
        }
        bob_index_all(&Bob);
        alice_index_all(&Alice);
        renovation(year, &Alice);
    }
    common_result(&Bob, &Alice);
    return 0;
}
