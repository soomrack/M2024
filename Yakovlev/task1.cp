#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


typedef long int Money;
double INFLIATION = 0.12, INFLATION_MONTH = INFLIATION / 12;
double DEPOSIT_RATE = 0.16, DEPOSIT_MONTH = DEPOSIT_RATE / 12;
double LOAN_RATE = 0.18, LOAN_MONTH = 0.18 / 12;
int LOAN_YEARS = 30;
Money MORTGAGE_PAYMENT;


bool IS_RANDOM_INITIALIZED = false;


double random_double(double min, double max) {

    if (!IS_RANDOM_INITIALIZED){
        srand(time(NULL));
        IS_RANDOM_INITIALIZED = true;
        }

    double range = max - min;
    double div = RAND_MAX / range;
    return min + (rand() / div);
}


struct Person
{
    Money salary;
    Money food_spending;
    Money home_price;
    Money rent;
    Money additional_spendings;
    Money savings;
    int loan_years;
    Money cost_random_min;
    Money cost_random_max;
    Money initial_payment;
    Money appartment_repair;
    Money deposit;
};


struct Person Alice
{
    Alice.salary = 150000,
    Alice.food_spending = 15000,
    Alice.home_price = 10000000,
    Alice.rent = 0,
    Alice.additional_spendings = 40000,
    Alice.savings = 0,
    Alice.loan_years = 30,
    Alice.cost_random_min = 5000,
    Alice.cost_random_max = 20000,
    Alice.initial_payment = 2000000,
    Alice.appartment_repair = 150000,
    Alice.deposit = 0,
};


struct Person Bob
{
    Bob.salary = 150000,
    Bob.food_spending = 15000,
    Bob.home_price = 0,
    Bob.rent = 30000,
    Bob.additional_spendings = 40000,
    Bob.savings = 2000000,
    Bob.loan_years = 0,
    Bob.cost_random_min = 5000,
    Bob.cost_random_max = 20000,
    Bob.initial_payment = 0,
    Bob.appartment_repair = 0,
    Bob.deposit = 0,
};


void Alice_indexation()
{
    Alice.salary += Alice.salary * INFLIATION;
    Alice.food_spending += Alice.food_spending * INFLIATION;
    Alice.home_price += Alice.home_price * INFLIATION;
    Alice.additional_spendings += Alice.additional_spendings * INFLIATION;
    Alice.cost_random_min += Alice.cost_random_min * INFLIATION;
    Alice.cost_random_max += Alice.cost_random_max * INFLIATION;
}


void Alice_mortgage()
{
    MORTGAGE_PAYMENT = (Alice.home_price - Alice.initial_payment) * (LOAN_MONTH + LOAN_MONTH /
        (pow((1 + LOAN_MONTH), LOAN_YEARS * 12) - 1));
}


void Alice_get_salary()
{
    Alice.savings += Alice.salary;
}


void Alice_get_food()
{
    Alice.savings -= Alice.food_spending;
}


void Alice_mortgage_payment()
{
    Alice.savings -= MORTGAGE_PAYMENT;
}


void Alice_additional_spendings()
{
    Alice.savings -= Alice.additional_spendings;
}


void Alice_rand_spendings()
{
    double cost_random = random_double(Alice.cost_random_min, Alice.cost_random_max);

    Alice.savings -= cost_random;
}


void Alice_appartment_repair(int year)
{
    if (year == 2026)
        Alice.savings -= Alice.appartment_repair;
}


void Alice_deposit()
{
    if (Alice.savings >= (MORTGAGE_PAYMENT * 2)) {
        int Alice_deposit_profit;
        Alice.savings -= MORTGAGE_PAYMENT;
        Alice.deposit += MORTGAGE_PAYMENT;
        Alice_deposit_profit = Alice.deposit * DEPOSIT_MONTH;
        Alice.deposit += Alice_deposit_profit;
    }
}


void Alice_savings()
{
    Alice.savings += Alice.home_price;
    Alice.savings += Alice.deposit;
}
void Alice_inflation_march()
{
    if (yaer == 2028 && month == 3){
        INFLIATION*=2;
        Alice.home_price*=2;
        
    }
}

void Bob_indexation()
{
    Bob.salary += Bob.salary * INFLIATION;
    Bob.food_spending += Bob.food_spending * INFLIATION;
    Bob.rent += Bob.rent * INFLIATION;
    Bob.cost_random_min += Bob.cost_random_min * INFLIATION;
    Bob.cost_random_max += Bob.cost_random_max * INFLIATION;
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
    double cost_random = random_double(Bob.cost_random_min, Bob.cost_random_max);

    Bob.savings -= cost_random;
}

void Bob_deposit()
{
    int Bob_deposit_profit;
    Bob.deposit += (Bob.salary - Bob.home_price - Bob.additional_spendings - Bob.rent);
    Bob_deposit_profit = Bob.deposit * DEPOSIT_MONTH;
    Bob.deposit += Bob_deposit_profit;
}



    

void Bob_savings()
{
    Bob.savings += Bob.deposit;
}

void Bob_inflation_march()
{
    if (yaer == 2028 && month == 3){
        INFLIATION*=2;
        Bob.home_price*=2;
        
    }
        
}


void Alice_simulation()
{
    int month = 2;
    int year = 2024;
    Alice_mortgage();
    while (year <= (year + LOAN_YEARS))
    {
        Alice_get_salary();
        Alice_get_food();
        Alice_mortgage_payment();
        Alice_rand_spendings();
        Alice_appartment_repair(year);
        Alice_deposit();
        Alice_inflation_march();

        if (year == 2024 + LOAN_YEARS && month == 2)
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
    int month = 2;
    int year = 2024;
    while (year <= (year + LOAN_YEARS))
    {
        Bob_get_salary();
        Bob_get_food();
        Bob_rand_spendings();
        Bob_deposit();
        Bob_inflation_march();

        if (year == 2024 + LOAN_YEARS && month == 2)
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
    printf("Alice savings = %ld \nBob savings = %ld\n", Alice.savings, Bob.savings);
    
}
