#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>


typedef long long int Money; // rub
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
    Money carprice;
    Money caradds;
    Money TO;
};


struct Person Alice
{
    Alice.salary = 300 * 1000,
    Alice.food_spending = 15 * 1000,
    Alice.home_price = 20 * 1000 * 1000,
    Alice.rent = 0,
    Alice.additional_spendings = 50 * 1000,
    Alice.savings = 2 * 1000 * 1000,
    Alice.loan_years = 30,
    Alice.contribution = 2 * 1000 * 1000,
    Alice.appartment_repair = 150 * 1000,
    Alice.deposit = 0,
    Alice.carprice = 0,
    Alice.caradds = 0,
    Alice.TO = 0
};


struct Person Bob
{
    Bob.salary = 300 * 1000,
    Bob.food_spending = 15 * 1000,
    Bob.home_price = 0,
    Bob.rent = 25 * 1000,
    Bob.additional_spendings = 50 * 1000,
    Bob.savings = 2 * 1000 * 1000,
    Alice.loan_years = 30,
    Bob.contribution = 0,
    Bob.appartment_repair = 0,
    Bob.deposit = 0,
    Bob.carprice = 2 * 1000 * 1000,
    Bob.caradds = 20 * 1000,
    Bob.TO = 50 * 1000
};


void Alice_indexation() 
{
    Alice.salary += Alice.salary * INFLIATION;
    Alice.food_spending += Alice.food_spending * INFLIATION;
    Alice.home_price += Alice.home_price * INFLIATION;
    Alice.additional_spendings += Alice.additional_spendings * INFLIATION;
}


void Alice_mortgage() // raschet plati za ipoteku
{
    double monthly_rate = MORTGAGE_PERCENTAGE / 12; //ЕЖЕМЕСЯЧНАЯ_СТАВКА = ПРОЦЕНТНАЯ_СТАВКА_ГОДОВЫХ / 12
    double total_rate = pow(1 + monthly_rate, LOAN_YEARS * 12); // ОБЩАЯ_СТАВКА = (1 + ЕЖЕМЕСЯЧНАЯ_СТАВКА) ^ СРОК_ИПОТЕКИ_МЕСЯЦЕВ
    ALICE_PAYMENT = (Alice.home_price - Alice.savings) * monthly_rate * total_rate / (total_rate - 1);// ЕЖЕМЕСЯЧНЫЙ_ПЛАТЕЖ = СУММА_КРЕДИТА * ЕЖЕМЕСЯЧНАЯ_СТАВКА * ОБЩАЯ_СТАВКА / (ОБЩАЯ_СТАВКА - 1)
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


void Alice_rand_spendings()
{
   int rand_spendings = rand() % 10;

    if (rand_spendings == 5)
    {
        Alice_additional_spendings();
    }
}


void Alice_appartment_repair()
{
    Alice.savings -= Alice.appartment_repair;
}


void Alice_deposit() 
{
    int Alice_deposit_profit;
    Alice.savings -= ALICE_PAYMENT;
    Alice.deposit += ALICE_PAYMENT;
    Alice_deposit_profit = Alice.deposit * DEPOSIT_RATE / 365 * 30;
    Alice.deposit += Alice_deposit_profit;
}


void Bob_indexation()
{
    Bob.salary += Bob.salary * INFLIATION;
    Bob.food_spending += Bob.food_spending * INFLIATION;
    Bob.rent += Bob.rent * INFLIATION;
    Bob.carprice += Bob.carprice * INFLIATION;
    Bob.caradds += Bob.caradds * INFLIATION;
    Bob.TO += Bob.TO * INFLIATION;
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


void Bob_rand_spendings()
{
    int rand_spendings = rand() % 10;

    if (rand_spendings == 5)
    {
        Bob_additional_spendings();
    }
}


void Bob_deposit()
{
    int Bob_deposit_profit;
    Bob.deposit += (Bob.salary - Bob.home_price - Bob.additional_spendings - Bob.rent);
    Bob_deposit_profit = Bob.deposit * DEPOSIT_RATE / 365 * 30;
    Bob.deposit += Bob_deposit_profit;
}


void Bob_buyCar()
{
    Bob.savings -= Bob.carprice;
}


void CarTO()
{
    Bob.savings -= Bob.TO;
}


void Bob_caradds()
{
    Bob.savings -= Bob.caradds;
}


void Alice_simulation()  
{ 
    int month = 2;
    int year = 2024;
    Alice_mortgage();
    Alice_buy_home();
    while(year <= (year + LOAN_YEARS))
        {
            Alice_get_salary();
            Alice_get_food();
            Alice_mortgage_payment();
            Alice_rand_spendings();

            if (year == 2024 && (month == 2 || month == 3))
                Alice_appartment_repair();

            if (Alice.savings >= (ALICE_PAYMENT * 2))
                Alice_deposit();

            if (year == 2024 + LOAN_YEARS && month == 2) 
                break;

            month++;

            if (month == 13){
                month = 1;
                year++;
                Alice_indexation();
            }
        }
    Alice.savings += Alice.home_price;
    Alice.savings += Alice.deposit;
}


void Bob_simulation()
{
    int month = 2;
    int year = 2024;
    int f = 0;
    while (year <= (year + LOAN_YEARS))
    {
        Bob_get_salary();
        Bob_get_food();
        Bob_rand_spendings();
        Bob_deposit();
        if (year == (year + 10)) {
            Bob_buyCar();
            f = 1;
        }

        if (f != 0)
            Bob_caradds();

        if (year == 2024 + LOAN_YEARS && month == 2) {
            break;

        }

        month++;

        if (month == 13){
            if (f != 0)
                CarTO();

            month = 1;
            year++;
            Bob_indexation();
        }
    }
    Bob.savings += Bob.deposit;
    Bob.savings += Bob.carprice;
}


int main()
{
    Alice_simulation();
    Bob_simulation();
    setlocale(LC_ALL, "Rus");
    printf("Сбережения Элис = %lld руб\nСбережения Боба = %lld руб\n", Alice.savings, Bob.savings);
    if (Alice.savings > Bob.savings)
        printf("У Элис вышло больше денег");
    else
        printf("У Боба вышло больше денег");
    return 1;
}
