#include <stdio.h>
#include <math.h>

typedef long long int Money;
struct Person
{
    Money salary;
    Money deposit;
    Money expenses;
    Money flat_payment;
    int has_flat;
};

struct Person A;
struct Person B;

const Money SALARY = 150 * 1000 * 100;
const Money DEPOSIT = 2 * 1000 * 1000 * 100;
const Money EXPENSES = 15 * 1000 * 100;
const Money FLAT_PRICE = 20 * 1000 * 1000 * 100;
const Money FLAT_RANT = 25 * 1000 * 100;

const int NOW_MONTH = 2;
const int NOW_YEAR = 2024;
const int YEARS = 30;

double INFLATION = 0.08;
double BANK_RATE = 0.15;
double MORTGAGE_RATE = 0.08;

int TIME_OF_BUYING_FLAT = 0;


void alice_init() {
    A.salary = SALARY;
    A.deposit = 0;
    A.expenses = EXPENSES;
    A.flat_payment = (FLAT_PRICE - DEPOSIT) * (MORTGAGE_RATE / 12.0) * (pow((1.0 + MORTGAGE_RATE / 12.0), (YEARS * 12.0)))
        / (pow((1.0 + MORTGAGE_RATE / 12.0), (YEARS * 12.0)) - 1.0);
}


void bob_init() {
    B.salary = SALARY;
    B.deposit = DEPOSIT;
    B.expenses = EXPENSES;
    B.flat_payment = FLAT_RANT;
    B.has_flat = 0;
}


void alice_print() {    
    printf("Алиса тратит %lld \n", A.expenses);
    printf("Алиса накопит %lld \n", A.deposit);
    printf("Алиса платит за ипотеку %lld \n", A.flat_payment);
    printf("\n");
}


void bob_print() {
        printf("Боб тратит %lld \n", B.expenses);
    printf("Боб накопит %lld \n", B.deposit);
    if (B.has_flat == 0){
        printf("Боб не купил квартиру \n");
        printf("Боб платит за съёмную квартиру %lld \n", B.flat_payment);
    }
    else {
        printf("Боб купил квартиру в %d году\n", TIME_OF_BUYING_FLAT);
    }
}


void alice_bank(int year, int month) {
    A.deposit = A.deposit * (1.0 + BANK_RATE / 12.0) + A.salary;
}


void alice_expenses(int year, int month) {
    A.deposit -= A.expenses;
}

void alice_flat_payment(int year, int month) {
    A.deposit -=  A.flat_payment;
}

void bob_bank(int year, int month) {
    B.deposit = B.deposit * (1.0 + BANK_RATE / 12.0) + B.salary;
}


void bob_expenses(int year, int month) {
    B.deposit = B.deposit - B.expenses;
}


void bob_flat_payment(int year, int month) {
    if (B.has_flat == 0) {
        B.deposit = B.deposit - B.flat_payment;
    }
}


void bob_buy_flat(int year, int month) {
    if ((B.has_flat == 0) && (B.deposit > FLAT_PRICE)){
        B.deposit = B.deposit - FLAT_PRICE;
        B.has_flat = 1;
        TIME_OF_BUYING_FLAT = year;
    }
}


void inflation(int year, int month) {
    if (month % 12 == 0) {
        A.salary *= (1.0 + INFLATION);
        A.expenses *= (1.0 + INFLATION);
        B.salary *= (1.0 + INFLATION);
        B.flat_payment *= (1.0 + INFLATION);
        B.expenses *= (1.0 + INFLATION);
    }
}


void simulation() {
    int month = NOW_MONTH;
    int year = NOW_YEAR;
    while (!(year == NOW_YEAR + YEARS && month == NOW_MONTH)) {
        alice_bank(year, month);
        bob_bank(year, month);

        alice_expenses(year, month);
        bob_expenses(year, month);
        
        bob_buy_flat(year, month);
               
        alice_flat_payment(year, month);
        bob_flat_payment(year, month);
    
        inflation(year, month);
        
        month += 1;
        
        if (month == 13) {
            month = 1;
            year += 1;
        }
       
    }
}


int main() {      

    alice_init();
    bob_init(); 

    simulation();

    alice_print();
    bob_print();

    return 0;
}
