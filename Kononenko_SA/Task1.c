#include <stdio.h>
#include <math.h>

typedef long long int Money; 

int years = 30;
int years_reparation = 2;
double indexation = 0.12;
double deposite = 0.16;
double flat_rate = 0.18;


struct Earnings {
    Money salary;
};

struct Expenses {
    Money flat_cost;
    Money reparation;
    Money rent;
    Money food;
    Money car_repair;
};

struct Person {
    Money balance;
    struct Earnings earn;
    struct Expenses expen;
};

struct Person Alice;
struct Person Bob;

void Alice_salary(int month)
{
    int salary = Alice.earn.salary;

    Alice.balance += salary;
    
}

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
    Bob.expen.car_repair = 50 * 1000*100;
}


void Alice_print()
{
    printf("%s %d %s\n", "Через ", years, "лет баланс Алисы будет составлять:");
    printf("    %lld %s", Alice.balance / 100, "рублей");
    printf(" %lld %s\n", Alice.balance % 100, "копеек");
    printf("\n");
}


void Bob_print()
{
    printf("%s %d %s\n", "Через ", years, "лет баланс Боба будет составлять:");
    printf("    %lld %s", Bob.balance / 100, "рублей");
    printf(" %lld %s\n", Bob.balance % 100, "копеек");
    printf("\n");
}


void Alice_bank()
{
    Alice.balance *= 1 + deposite / 12 ;
}


void Alice_inflation(int month)
{
    if (month % 12 == 0) {
        Alice.expen.food *= 1 + indexation ;
        Alice.earn.salary *= 1 + indexation ;
        Alice.expen.reparation *= 1 + indexation ;
    }
}



void Alice_mortgage ()
{
    double degree = pow (1 + flat_rate / 12, years * 12);
    double mortgage_month = Alice.expen.flat_cost * flat_rate/ 12 * degree / (degree - 1);
    Alice.balance -= mortgage_month;
}


void Alice_buying_flat()
{
    Alice.balance -= Alice.expen.flat_cost;
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
    Bob.balance *= 1 + deposite / 12 ;
}


void Bob_salary()
{
    Bob.balance += Bob.earn.salary;
}

void Bob_car_repair(int month)
{
    if (month %6 == 0) {
        Bob.balance -= Bob.expen.car_repair;
    }
}

void Bob_inflation(int month)
{
    if (month % 12 == 0) {
        Bob.expen.food *= 1 + indexation ;
        Bob.earn.salary *= 1 + indexation ;
        Bob.expen.rent *= 1 + indexation ;
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
        Bob_car_repair(month);
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
