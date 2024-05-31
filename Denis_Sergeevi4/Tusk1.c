#include <stdio.h> 
#include <math.h> 
 
 
#define PERIOD_JOB_LOST 1 
#define PERIOD_JOB_LOST_AGAIN 1 
 
typedef long long int Money; // считаем в копейках 
 
struct Person { 
    Money capital; 
    Money income; 
    Money spendings; 
    Money flat_payment; 
    Money overpayment; 
    Money car_expenses; 
}; 
 
 
struct Person Alice, Bob; 
 
const Money FLAT_PRICE = 20 * 1000 * 1000 * 100; 
const Money FLAT_RENT_PRICE = 25 * 1000 * 100; 
const Money SALARY = 150 * 1000 * 100; 
const Money FIRST_FLAT_PAYNMENT = 2000 * 1000 * 100; 
const Money LIFE_SPENDING = 15 * 1000 * 100; 
const Money CAR_EXPENSES = 100 * 1000 * 100;  
const short YEARS = 30; 
const short START_MONTH = 2; 
const short START_YEAR = 2024; 
const double INFLATION = 0.07; 
const double BANK_RATE = 0.05; 
 
Money updated_salary = 180 * 1000 * 100; 
 
 
void bob_init() 
{ 
    Bob.income = SALARY; 
    Bob.capital = FIRST_FLAT_PAYNMENT; 
    Bob.spendings = LIFE_SPENDING; 
    Bob.flat_payment = FLAT_RENT_PRICE; 
    Bob.car_expenses = CAR_EXPENSES; 
} 
 
 
void alice_init() 
{ 
    Alice.income = SALARY; 
    Alice.capital = 2000 * 1000 * 100; 
    Alice.spendings = LIFE_SPENDING; 
    Alice.flat_payment = ((FLAT_PRICE - FIRST_FLAT_PAYNMENT) * (BANK_RATE / 12.0) * pow((1.0 + BANK_RATE / 12.0), (YEARS * 12.0))) 
        / (pow((1.0 + BANK_RATE / 12.0), (YEARS * 12.0)) - 1); 
    Alice.overpayment = Alice.flat_payment * 12 * YEARS - FLAT_PRICE; 
    Alice.car_expenses = CAR_EXPENSES; 
} 
 
 
void bob_salary(int year, int month) 
{ 
    Bob.capital += Bob.income; 
    if (month % 12 == 0) { 
        Bob.capital += Bob.income; 
    } 
} 
 
 
void bob_deposite(int year, int month) 
{ 
    Bob.capital *= (1.0 + BANK_RATE / 12.0); 
} 
 
 
void bob_spendings(int year, int month) 
{ 
    if (month == 4 || month == 50) { 
        Bob.capital -= (Bob.spendings + Bob.flat_payment + Bob.car_expenses); 
    } 
    else { 
        Bob.capital -= (Bob.spendings + Bob.flat_payment); 
    } 
} 
 
 
void alice_salary(int year, int month) 
{ 
    if (month < 40) {  // ПЕРВАЯ РАБОТА 
        Alice.capital += Alice.income; 
        if (month % 12 == 0) { 
            Alice.capital += Alice.income; 
        } 
    } 
    if (month >= 40 && month <= 41) { 
        Alice.income = 0; 
    } 
    if (month > 41 && month < 90) { // ВТОРАЯ РАБОТА 
        static int counter = PERIOD_JOB_LOST; 
        if (counter > 0) { 
            Alice.income = 180 * 1000 * 10; 
            counter--; 
        } 
        Alice.capital += Alice.income; 
    } 
    if (month % 12 == 0) { 
        Alice.capital += Alice.income; 
    } 
    else if (month >= 90 && month <= 91) { 
        Alice.income = 0; 
    } 
    if (month > 91) {  // ТРЕТЬЯ РАБОТА 
        static int schetchik = PERIOD_JOB_LOST_AGAIN; 
        if (schetchik > 0) { 
            Alice.income = 250 * 1000 * 10; 
            schetchik--; 
        } 
        Alice.capital += Alice.income; 
        if (month % 12 == 0) { 
            Alice.capital += Alice.income; 
        } 
    } 
} 
  
void alice_deposite(int year, int month) 
{ 
    Alice.capital *= (1.0 + BANK_RATE / 12.0); 
} 
 
void alice_spendings(int year, int month) 
{ 
    if (month == 8 || month == 180) { 
        Alice.capital -= (Alice.spendings + Alice.flat_payment + Alice.car_expenses); 
    } 
    else { 
        Alice.capital -= (Alice.spendings + Alice.flat_payment); 
    } 
} 


void inflation(int year, int month) 
{ 
    if (month % 12 == 0) { 
        Alice.income *= (1.0 + INFLATION); 
        Alice.spendings *= (1.0 + INFLATION); 
        Alice.car_expenses *= (1.0 + INFLATION); 
        Bob.income *= (1.0 + INFLATION); 
        Bob.flat_payment *= (1.0 + INFLATION); 
        Bob.spendings *= (1.0 + INFLATION); 
        Bob.car_expenses *= (1.0 + INFLATION); 
        //updated_salary *= (1.0 + INFLATION); 
    } 
} 

void buy_cat(size_t month){
    if(month % 12 == 8 && START_YEAR * 12 + month == 2026 * 12){
        Alice.capital -= 30*1000;
    }
    return;
}

void simulation() 
{ 
    int month = START_MONTH; 
    int year = START_YEAR; 
    while (month != YEARS * 12.0 + START_MONTH) { 
        alice_deposite(year, month); 
        // alice_lost_job(year, month);
alice_salary(year, month); 
        alice_spendings(year, month); 
 
        bob_deposite(year, month); 
        bob_salary(year, month); 
        bob_spendings(year, month); 
 
        inflation(year, month); 
 
        month++; 
 
        if (month % 12 == 0) { 
            year++; 
        } 
    } 
} 


void print_bob() 
{ 
    printf("Bob Capital: %lli rub\n", Bob.capital / 100); 
    printf("Bob Income: %lli rub\n", Bob.income / 100); 
    printf("Bob Life spendings: %lli rub\n", Bob.spendings / 100); 
    printf("Bob Monthly payment: %lli rub\n", Bob.flat_payment / 100); 
} 


void print_alice() 
{ 
    printf("Alice Capital: %lli rub\n", Alice.capital / 100); 
    printf("Alice Income: %lli rub\n", Alice.income / 100); 
    printf("Alice Life spendings: %lli rub\n", Alice.spendings / 100); 
    printf("Alice Monthly payment: %lli rub\n", Alice.flat_payment / 100); 
    printf("Alice Overpayment: %lli rub\n", Alice.overpayment / 100); 
    printf("\n"); 
} 


int main() 
{ 
    alice_init(); 
    bob_init();
    simulation(); 
    print_alice(); 
    print_bob(); 
 
    return 0; 
}
