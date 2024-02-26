#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>


typedef long long int Money; // kopeiki
double inflation = 12 * 0.01;
double deposit_rate = 16 * 0.01;
double mortgage_percentage = 18 * 0.01;
int loan_years = 30;
Money Alice_payment;

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
};


struct Person Alice
{
    Alice.salary = 300 * 1000 * 100,
    Alice.food_spending = 15 * 1000 * 100,
    Alice.home_price = 20 * 1000 * 1000 * 100,
    Alice.rent = 0,
    Alice.additional_spendings = 50 * 1000 * 100,
    Alice.savings = 2 * 1000 * 1000 * 100,
    Alice.loan_years = 30,
    Alice.contribution = 2 * 1000 * 1000 * 100,
    Alice.appartment_repair = 150 * 1000 * 100
};


struct Person Bob
{
    Bob.salary = 300 * 1000 * 100,
    Bob.food_spending = 15 * 1000 * 100,
    Bob.home_price = 0,
    Bob.rent = 25 * 1000 * 100,
    Bob.additional_spendings = 50 * 1000 * 100,
    Bob.savings = 2 * 1000 * 1000 * 100,
    Alice.loan_years = 30,
    Bob.contribution = 0,
    Bob.appartment_repair = 0
};


void Bob_indexation(const int month, const int year) // indexacia
{
    Bob.salary += Bob.salary * inflation;
    Bob.food_spending += Bob.food_spending * inflation;
    Bob.home_price += Bob.home_price * inflation;
}


void Alice_indexation() // indexacia
{
    Alice.salary += Alice.salary * inflation;
    Alice.food_spending += Alice.food_spending * inflation;
    Alice.home_price += Alice.home_price * inflation;
    Alice.additional_spendings += Alice.additional_spendings * inflation;
}


void Alice_mortgage() // raschet plati za ipoteku
{
    double monthly_rate = mortgage_percentage / 12; //ЕЖЕМЕСЯЧНАЯ_СТАВКА = ПРОЦЕНТНАЯ_СТАВКА_ГОДОВЫХ / 12
    double total_rate = pow(1 + monthly_rate, loan_years * 12); // ОБЩАЯ_СТАВКА = (1 + ЕЖЕМЕСЯЧНАЯ_СТАВКА) ^ СРОК_ИПОТЕКИ_МЕСЯЦЕВ
    Alice_payment = (Alice.home_price - Alice.savings) * monthly_rate * total_rate / (total_rate - 1);// ЕЖЕМЕСЯЧНЫЙ_ПЛАТЕЖ = СУММА_КРЕДИТА * ЕЖЕМЕСЯЧНАЯ_СТАВКА * ОБЩАЯ_СТАВКА / (ОБЩАЯ_СТАВКА - 1)
}


void Alice_get_salary() //zp
{
    Alice.savings += Alice.salary;
}


void Bob_get_salary() //zp
{
    Bob.savings += Bob.salary;
}


void Alice_get_food() //eda
{
    Alice.savings -= Alice.food_spending;
}


void Bob_get_food() //eda
{
    Bob.savings -= Bob.food_spending;
}

void Alice_buy_home() //pokupka hati
{
    Alice.savings -= Alice.contribution;
}


void Alice_mortgage_payment() //platej po ipoteke
{
    Alice.savings -= Alice_payment;
}


void Alice_additional_spendings() //dop rashodi
{
    Alice.savings -= Alice.additional_spendings;
}


void Alice_deposit() // deposit 
{
    ;
}
void Alice_savings() // sberejenia 
{ 
    int month = 2;
    int rand_spendings;
    Alice_mortgage();
    Alice_buy_home();
    for (int year = 2024; year <= year + 30; year++)
    {
        
        for (; month <= 12; month++)
        {
            Alice_get_salary();
            Alice_get_food();
            Alice_mortgage_payment();
            rand_spendings = rand() % 10;

            if (rand_spendings == 5)
            {
                Alice_additional_spendings();
            }

            if (year == 2024 && (month == 2 || month == 3))
            {
                Alice.savings -= Alice.appartment_repair;
            }

            if (month == 12)
            {
                break;
            }

            if (year == 2024 + 30 && month == 2) 
            {
                goto exit_loop;
            }

            if (Alice.savings > 0)
            {
                ;
            }

        }
        Alice_indexation();
        month = 1;
        //printf("month= %d, year= %d, AS=%lld\n", month, year, Alice.salary / 100);
    }
exit_loop: 
    //printf("month= %d, year= %d, AS=%lld\n", month, year1, Alice.salary / 100);
    Alice.savings += Alice.home_price;
}


int main()
{
    Alice_savings();
    printf("AS=%lld\n", Alice.savings / 100);
    return 1;
}
