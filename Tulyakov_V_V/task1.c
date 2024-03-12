#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>


typedef long long int Money; // рубли
double INFLIATION = 12 * 0.01;
double DEPOSIT_RATE = 16 * 0.01;
double MORTGAGE_PERCENTAGE = 18 * 0.01;
int LOAN_YEARS = 30;
Money ALICE_PAYMENT;

struct Person
{
    Money salary;
    Money food_spending;
    Money home_price;
    Money rent;
    Money additional_spendings;
    Money savings;
    int loan_years;
    Money contribution;
    Money appartment_repair;
    Money deposit;
};


struct Person Alice
{
    Alice.salary = 250 * 1000,
        Alice.food_spending = 20 * 1000,
        Alice.home_price = 20 * 1000 * 1000,
        Alice.rent = 0,
        Alice.additional_spendings = 30 * 1000,
        Alice.savings = 2 * 1000 * 1000,
        Alice.loan_years = 30,
        Alice.contribution = 2 * 1000 * 1000,
        Alice.appartment_repair = 150 * 1000,
        Alice.deposit = 0,
};


struct Person Bob
{
    Bob.salary = 300 * 1000,
        Bob.food_spending = 20 * 1000,
        Bob.home_price = 0,
        Bob.rent = 30 * 1000,
        Bob.additional_spendings = 50 * 1000,
        Bob.savings = 2 * 1000 * 1000,
        Alice.loan_years = 30,
        Bob.contribution = 0,
        Bob.appartment_repair = 0,
        Bob.deposit = 0,
};


void Alice_indexation()
{
    Alice.salary += Alice.salary * INFLIATION;
    Alice.food_spending += Alice.food_spending * INFLIATION;
    Alice.home_price += Alice.home_price * INFLIATION;
    Alice.additional_spendings += Alice.additional_spendings * INFLIATION;
}


void Alice_mortgage() // расчет ипотеки
{
    double monthly_rate = MORTGAGE_PERCENTAGE / 12;
    double total_rate = pow(1 + monthly_rate, LOAN_YEARS * 12);
    ALICE_PAYMENT = (Alice.home_price - Alice.savings) * monthly_rate * total_rate / (total_rate - 1);
}


void Alice_get_salary()
{
    Alice.savings += Alice.salary;
}


void Alice_get_food()
{
    Alice.savings -= Alice.food_spending;
}


void Alice_buy_home()
{
    Alice.savings -= Alice.contribution;
}


void Alice_mortgage_payment()
{
    Alice.savings -= ALICE_PAYMENT;
}


void Alice_additional_spendings()
{
    Alice.savings -= Alice.additional_spendings;
}


void Alice_appartment_repair(int year, int month)
{
    if (year == 2024 && (month == 3 || month == 4))
        Alice.savings -= Alice.appartment_repair;
}


void Alice_deposit()
{
    if (Alice.savings >= (ALICE_PAYMENT * 2)) {
        int Alice_deposit_profit;
        Alice.savings -= ALICE_PAYMENT;
        Alice.deposit += ALICE_PAYMENT;
        Alice_deposit_profit = Alice.deposit * DEPOSIT_RATE / 365 * 30;
        Alice.deposit += Alice_deposit_profit;
    }
}


void Alice_savings()
{
    Alice.savings += Alice.home_price;
    Alice.savings += Alice.deposit;
}


void Bob_indexation()
{
    Bob.salary += Bob.salary * INFLIATION;
    Bob.food_spending += Bob.food_spending * INFLIATION;
    Bob.rent += Bob.rent * INFLIATION;
}


void Bob_get_salary()
{
    Bob.savings += Bob.salary;
}


void Bob_get_food()
{
    Bob.savings -= Bob.food_spending;
}


void Bob_additional_spendings()
{
    Bob.savings -= Bob.additional_spendings;
}


void Bob_deposit()
{
    int Bob_deposit_profit;
    Bob.deposit += (Bob.salary - Bob.home_price - Bob.additional_spendings - Bob.rent);
    Bob_deposit_profit = Bob.deposit * DEPOSIT_RATE / 365 * 30;
    Bob.deposit += Bob_deposit_profit;
}


void Bob_savings()
{
    Bob.savings += Bob.deposit;
}


void Alice_simulation()
{
    int month = 3;
    int year = 2024;
    Alice_mortgage();
    Alice_buy_home();
    while (year <= (year + LOAN_YEARS))
    {
        Alice_get_salary();
        Alice_get_food();
        Alice_mortgage_payment();
        Alice_appartment_repair(year, month);
        Alice_deposit();

        if (year == 2024 + LOAN_YEARS && month == 3)
            break;

        month++;

        if (month == 13) {
            month = 1;
            year++;
            Alice_indexation();
        }
    }
    Alice_savings();
}


void Bob_simulation()
{
    int month = 3;
    int year = 2024;
    while (year <= (year + LOAN_YEARS))
    {
        Bob_get_salary();
        Bob_get_food();
        Bob_deposit();

        if (year == 2024 + LOAN_YEARS && month == 3)
            break;

        month++;

        if (month == 13) {
            month = 1;
            year++;
            Bob_indexation();
        }
    }
    Bob_savings();
}


int main()
{
    Alice_simulation();
    Bob_simulation();
    setlocale(LC_ALL, "Rus");
    printf("Сбережения Элис: %lld руб\nСбережения Боба: %lld руб\n", Alice.savings, Bob.savings);
    if (Alice.savings > Bob.savings)
        printf("Элис в большей выгоде.");
    else if (Alice.savings < Bob.savings)
        printf("Боб в большей выгоде.");
    else
        printf("Элис и Боб остались с одинаковым количеством капитала.");
    return 1;
}
