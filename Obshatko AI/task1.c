#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

typedef long long int Money;

int TOTAL_YEARS = 30;
double INFLATION = 12 * 0.01;


struct Person 
{
    Money apartment_price;

    Money savings;  // сумма денег на вкладе

    int LOAN_PERIOD;  // в годах

    // в месяц
    Money salary;
    Money food_spending;
    Money RENT;
    Money LOAN_PAYMENT;  // вычисляется в функции compute_LOAN_payment
    Money additional_spendings;

    // в год
    Money APARTMENT_REPAIRS;
    double LOAN_RATE;
    double DEPOSIT_RATE;
};


// вычисление ежемесячной оплаты кредита
void calculate_loan_payment(struct Person *person)
{
    //int payment = apartment_price * LOAN / 100 / 12;
    double numerator = pow(1 + person->LOAN_RATE / 12, person->LOAN_PERIOD * 12);
    double denominator = numerator - 1;
    person->LOAN_PAYMENT = Money((person->apartment_price - person->savings) * person->LOAN_RATE / 12 * numerator / denominator);

    if (person->LOAN_RATE == 0) {
        person->LOAN_PAYMENT = 0;
    }
    
    if (person->LOAN_PAYMENT > person->salary)
    {
        printf("Программа остановлена: Плата по кредиту больше заработной платы");
        exit(0);
    }
}


// ежемесячное начисление зарплаты
void get_salary(struct Person *person)
{
    person->savings += person->salary;
}


// ежемесячная покупка еды
void buy_food(struct Person *person)
{
    person->savings -= person->food_spending;
}


// ежемесячная оплата аренды
void pay_rent(struct Person *person)
{
    person->savings -= person->RENT;
}


// ежемесячная плата по кредиту
void pay_loan(struct Person *person)
{
    //printf("%f", person->savings);
    //printf("\n");
    person->savings -= person->LOAN_PAYMENT;
    //printf("%f", person->savings);
    //printf("\n");
    //printf("\n");
}


// ежемесячный учет дополнительных расходов
void pay_additional(struct Person *person)
{
    person->savings -= person->additional_spendings;
}


// ежегодный ремонт квартиры
void repair_apartment(struct Person *person)
{
    person->savings -= person->APARTMENT_REPAIRS;
}


//  ежемесячная выплата процентов по вкладу
void deposit_income(struct Person *person)
{
    long double income = person->savings * person->DEPOSIT_RATE / 12;
    person->savings += income;
    //printf("%d", Money(person->savings * person->DEPOSIT_RATE / 12));
    //printf("\n");
}


// ежемесячный рост цен
void inflate(struct Person *person) 
{
    person->food_spending += person->food_spending * INFLATION / 12;
    person->additional_spendings += person->additional_spendings * INFLATION / 12;
}


// ежегодное повышение зарплаты
void indexation(struct Person *person)
{
    person->salary += person->salary * INFLATION;
}


// вычисление всех ежемесячных операций
void monthly_calculations(struct Person *person)
{
    //printf("%d", person->savings);
    //printf("\n");
    get_salary(person);
    buy_food(person);
    pay_rent(person);
    pay_loan(person);
    pay_additional(person);
    deposit_income(person);
    //printf("%d", person->savings);
    //printf("\n");
    //printf("\n");
    inflate(person);

}


// вычисление всех ежегодных операций
void yearly_calculations(struct Person *person)
{
    repair_apartment(person);
    indexation(person);
}


// вычисление результатов
void calculate(struct Person *first_person, struct Person *second_person) 
{
    calculate_loan_payment(first_person);
    calculate_loan_payment(second_person);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int STARTING_YEAR = tm.tm_year + 1900;
    int STARTING_MONTH = tm.tm_mon + 1;
    int year = STARTING_YEAR;
    int month = STARTING_MONTH;


    while(year != STARTING_YEAR + TOTAL_YEARS)
    {
        while (month <= 12)
        {
            monthly_calculations(first_person);
            monthly_calculations(second_person);

            month += 1;
        }
        month = 1;

        yearly_calculations(first_person);
        yearly_calculations(second_person);

        year += 1;
    }

    while (month < STARTING_MONTH)
    {
        monthly_calculations(first_person);
        monthly_calculations(second_person);

        month += 1;
    }
}


int main()
{
    setlocale(LC_ALL, "");

    struct Person Bob;
    struct Person Alice;

    Bob.apartment_price = 20 * 1000 * 1000 * 100;
    Alice.apartment_price = 20 * 1000 * 1000 * 100;

    Bob.savings = 2 * 1000 * 1000 * 100;
    Alice.savings = 2 * 1000 * 1000 * 100;

    Bob.LOAN_PERIOD = 0;
    Alice.LOAN_PERIOD = 30;

    Bob.salary = 300 * 1000 * 100;
    Alice.salary = 300 * 1000 * 100;

    Bob.food_spending = 15000 * 100;
    Alice.food_spending = 15000 * 100;

    Bob.RENT = 25000 * 100;
    Alice.RENT = 0;

    Bob.additional_spendings = 0;
    Alice.additional_spendings = 0;

    Bob.APARTMENT_REPAIRS = 0;
    Alice.APARTMENT_REPAIRS = 100 * 1000 * 100;

    Bob.LOAN_RATE = 0;
    Alice.LOAN_RATE = 18 * 0.01;

    Bob.DEPOSIT_RATE = 16 * 0.01;
    Alice.DEPOSIT_RATE = 16 * 0.01;



    calculate(&Bob, &Alice);



    printf("Bob %d", Bob.savings);
    printf("\n");
    printf("Alice %d", Alice.savings);
    printf("\n");

    //printf("%d", Alice.LOAN_PAYMENT);
}
