#include <stdio.h>
#include <math.h>

typedef long long int Money;  

int years = 30;
int years_reparation = 2;
int month_unemployment = 3;
double indexation_perc = 12;
double deposite_perc = 16;
double flat_rate_perc = 18;

struct Earnings {
    Money salary;
};

struct Expenses {
    Money flat_cost;
    Money reparation;
    Money rent;
    Money food;
};

struct Person {
    Money balance;
    struct Earnings earn;
    struct Expenses expen;
};

struct Person Alice;
struct Person Bob;

void Alice_finance()
{
    Alice.balance = 3 * 1000 * 1000 * 100;
    Alice.earn.salary = 300 * 1000 * 100;
    Alice.expen.flat_cost = 20 * 1000 * 1000 * 100;
    Alice.expen.reparation = 100 * 1000 * 100;
    Alice.expen.food = 15 * 1000 * 100;
};


void Bob_finance()
{
    Bob.balance = 3 * 1000 * 1000 * 100;
    Bob.earn.salary = 300 * 1000 * 100;
    Bob.expen.rent = 25 * 1000 * 100;
    Bob.expen.food = 15 * 1000 * 100;
}


void Alice_print()
{
    printf("%s %d %s\n", "Cherez ", years, "let ,balans Alisy budet raven:");
    printf("    %lld %s", Alice.balance / 100, "RUB");
    printf(" %02d %s\n", Alice.balance % 100, "KOP");
    printf("\n");
}


void Bob_print()
{
    printf("%s %d %s\n", "Cherez ", years, "let ,balans Boba budet raven:");
    printf("    %lld %s", Bob.balance / 100, "RUB");
    printf(" %02d %s\n", Bob.balance % 100, "KOP");
    printf("\n");
}


void Alice_bank()
{
    Alice.balance *= 1 + deposite_perc / 12 / 100;
}


void Alice_inflation(int month)
{
    if (month % 12 == 0) {
        Alice.expen.food *= 1 + indexation_perc / 100;
        Alice.earn.salary *= 1 + indexation_perc / 100;
        Alice.expen.reparation *= 1 + indexation_perc / 100;
    }
}


void Alice_mortgage ()
{
    double degree = pow (1 + flat_rate_perc / 1200, years * 12);
    double mortgage_month = Alice.expen.flat_cost * flat_rate_perc/ 1200 * degree / (degree - 1);
    Alice.balance -= mortgage_month;
}


void Alice_buying_flat()
{
    Alice.balance -= Alice.expen.flat_cost;
}


void Alice_salary(int month)
{
    int salary = Alice.earn.salary;

    Alice.balance += salary;
}


void Alice_food()
{
    Alice.balance -= Alice.expen.food;
}


void Alice_reparation(int month)
{
    if (month <= years_reparation * 12) {
        Alice.balance -= Alice.expen.reparation;
    }
}


void Alice_process()
{
    for (int month = 1; month <= years * 12; month++) {
       
        Alice_mortgage();
        Alice_food();
        
        Alice_reparation(month);
        
        Alice_bank();
        Alice_salary(month);

        Alice_inflation(month);
    }
};


void Bob_food()
{
    Bob.balance -= Bob.expen.food;
}


void Bob_rent()
{
    Bob.balance -= Bob.expen.rent;
}


void Bob_bank()
{
    Bob.balance *= 1 + deposite_perc / 12 / 100;
}


void Bob_salary()
{
    Bob.balance += Bob.earn.salary;
}


void Bob_inflation(int month)
{
    if (month % 12 == 0) {
        Bob.expen.food *= 1 + indexation_perc / 100;
        Bob.earn.salary *= 1 + indexation_perc / 100;
        Bob.expen.rent *= 1 + indexation_perc / 100;
    }
}


void Bob_process ()
{
    for (int month = 1; month <= years * 12; month++) {
       
        Bob_food();
        Bob_rent();

        Bob_bank();
        Bob_salary();

        Bob_inflation(month);
    };
}

int main()
{
    Alice_finance();
    Bob_finance();
    
    Alice_process();
    Bob_process();

    Alice_print();
    Bob_print();
    
    return 1;
}
