#include <stdio.h>
#include <math.h>

typedef long long int Shekel;  //рассчеты ведутся в копейках

int years = 30;
int years_no_job = 4;
int month_tunejad = 3;
int years_reparation = 2;

double deposite = 16;
double flat_rate = 18;
double indexation = 12;

//***********************************************//

struct Earnings 
{
    Shekel poluchka;
};

struct Expenses 
{
    Shekel flat_cost;
    Shekel reparation;
    Shekel rent;
    Shekel food;
};

struct Person 
{
    Shekel balance;
    struct Earnings earn;
    struct Expenses expen;
};

struct Person Alice;
struct Person Bob;

//***********************************************//

void Alice_finance()
{
    Alice.balance = 3*1000*1000*100;
    Alice.earn.poluchka = 300*1000*100;
    Alice.expen.flat_cost = 20*1000*1000*100;
    Alice.expen.reparation = 100*1000*100;
    Alice.expen.food = 15*1000*100;
};

void Bob_finance()
{
    Bob.balance = 3*1000*1000*100;
    Bob.earn.poluchka = 300*1000*100;
    Bob.expen.rent = 25*1000*100;
    Bob.expen.food = 15*1000*100;
}

void Bob_print()
{
    printf("%s %d %s\n", "Спустя", years, "лет баланс Боба:");
    printf("    %lld %s", Bob.balance / 100, "рублей");
    printf(" %02d %s\n", Bob.balance % 100, "копеек");
    printf("\n");
}

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
    Bob.balance *= 1 + deposite / 12 / 100;
}

void Bob_poluchka()
{
    Bob.balance += Bob.earn.poluchka;
}

void Bob_inflation(int month)
{
    if (month % 12 == 0) 
    {
        Bob.expen.food *= 1 + indexation / 100;
        Bob.earn.poluchka *= 1 + indexation / 100;
        Bob.expen.rent *= 1 + indexation / 100;
    }
}

void Bob_process ()
{
    for (int month = 1; month <= years * 12; month++) 
    {
       
        Bob_food();
        Bob_rent();

        Bob_bank();
        Bob_poluchka();

        Bob_inflation(month);
    };
}

void Alice_print()
{
    printf("%s %d %s\n", "Спустя", years, "лет баланс Алисы:");
    printf("    %lld %s", Alice.balance / 100, "рублей");
    printf(" %02d %s\n", Alice.balance % 100, "копеек");
    printf("\n");
}

void Alice_bank()
{
    Alice.balance *= 1 + deposite / 12 / 100;
}


void Alice_inflation(int month)
{
    if (month % 12 == 0) 
    {
        Alice.expen.food *= 1 + indexation / 100;
        Alice.earn.poluchka *= 1 + indexation / 100;
        Alice.expen.reparation *= 1 + indexation / 100;
    }
}


void Alice_mortgage ()
{
    double degree = pow (1 + flat_rate / 1200, years * 12);
    double mortgage_month = Alice.expen.flat_cost * flat_rate/ 1200 * degree / (degree - 1);
    Alice.balance -= mortgage_month;
}


void Alice_buying_flat()
{
    Alice.balance -= Alice.expen.flat_cost;
}


void Alice_poluchka(int month)
{
    int poluchka = Alice.earn.poluchka;

    Alice.balance += poluchka;
    
    if (month > years_no_job * 12 && month < years_no_job * 12 + month_tunejad) 
    {
        poluchka = 0;
    }

    if (month == years_no_job * 12 + month_tunejad) 
    {
        poluchka = Alice.earn.poluchka * 1.5;
    }
}


void Alice_food()
{
    Alice.balance -= Alice.expen.food;
}


void Alice_reparation(int month)
{
    if (month <= years_reparation * 12) 
    {
        Alice.balance -= Alice.expen.reparation;
    }
}


void Alice_process()
{
    for (int month = 1; month <= years * 12; month++) 
    {
       
        Alice_mortgage();
        Alice_food();
        
        Alice_reparation(month);
        
        Alice_bank();
        Alice_poluchka(month);

        Alice_inflation(month);
    }
};

//***********************************************//

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
