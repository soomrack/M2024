#include <stdio.h>
#include <math.h>

typedef long long Cash;  // Один тип данных для копеек

struct Person {
    char* name;
    Cash salary;
    Cash bank_capital;
    Cash monthly_ipoteka_payment;
    int saving_account_rate;
} alice, bob;

// Alice
Cash FLAT = 20000000 * 100;
Cash CREDIT = 18000000 * 100;
int IPOTEKA_RATE = 5;
Cash REPAIR = 50000 * 100;


// Bob
Cash RENT = 25000 * 100;

// Common
Cash FOOD = 15000 * 100;
Cash HOUSE = 2000 * 100; 
Cash OTHER = 5000 * 100; 
int INFLATION = 12;
int NUM_OF_YEARS = 30;
int CURRENT_YEAR = 2024;
int CURRENT_MONTH = 3;
int END_YEAR = 2054;
int END_MONTH = 3;
int JOB_PROMOTION_PERCENT = 8;

Cash alice_monthly_payment() {  // Расчет ежемесячного платежа по ипотеке
    float monthly_ipoteka_rate = IPOTEKA_RATE / 12.0 / 100.0;  // Месячная ставка 
    int total_payments = NUM_OF_YEARS * 12; 
    Cash ipoteka = (CREDIT * monthly_ipoteka_rate) / (1.0 - pow(1.0 + monthly_ipoteka_rate, -total_payments));  // Ежемесячный платеж (аннуитетный)
    return ipoteka;
}

void bob_person() {  // Боб
    bob.salary = 150 * 1000 * 100;
    bob.bank_capital = 2 * 1000 * 1000 * 100;
    bob.saving_account_rate = 16;
}


void alice_person() {  // Алиса
    alice.salary = 150 * 1000 * 100;
    alice.bank_capital = 100 * 1000 * 100;
    alice.monthly_ipoteka_payment = alice_monthly_payment();
}

void common_inflation() {  // Инфляция
    if (CURRENT_MONTH == 1) {
        alice.salary = alice.salary * (Cash)(1.0 + INFLATION / 100.0);
        FLAT = FLAT * (Cash)(1.0 + INFLATION / 100.0);
        bob.salary = bob.salary * (Cash)(1.0 + INFLATION / 100.0);
        FOOD = FOOD * (Cash)(1.0 + INFLATION / 100.0);
        HOUSE = HOUSE * (Cash)(1.0 + INFLATION / 100.0);
        OTHER = OTHER * (Cash)(1.0 + INFLATION / 100.0);
    }
}

void alice_job_promotion(){ // Повышение Алисы на работе каждые 5 лет (из-за ипотеки Алиса не меняет место работы)
    if (CURRENT_MONTH == 1){

    if ( (CURRENT_YEAR % 10 == 4) || (CURRENT_YEAR % 10 == 9) ) {
        alice.salary = alice.salary * (Cash)(1.0 + JOB_PROMOTION_PERCENT / 100.0);
    }
    
    }
    
}



void alice_dohod() {  // Доход Алисы
    alice.bank_capital += alice.salary;
}


void bob_dohod() {  // Доход БОба
    if (CURRENT_MONTH == 1) {
        bob.bank_capital = bob.bank_capital * (Cash)(1.0 + bob.saving_account_rate / 100.0);
    }
    bob.bank_capital = bob.bank_capital + bob.salary;
}

void alice_month_expences() {  // Расходы Алисы
    alice.bank_capital = alice.bank_capital - FOOD - HOUSE - OTHER;
    if (CURRENT_YEAR < 2027) {
        alice.bank_capital = alice.bank_capital - REPAIR;
    }
    alice.bank_capital = alice.bank_capital - alice.monthly_ipoteka_payment;
}


void bob_month_expences() {  // Расходы Боба
    bob.bank_capital = bob.bank_capital - FOOD - HOUSE - OTHER - RENT;
}

void print_information() {
    if (CURRENT_MONTH == 1) {
        printf("[Year: %d ]\n", CURRENT_YEAR);
        printf("Alice's capital: %lld\n", (Cash)((alice.bank_capital + FLAT)/100.));
        printf("Bob's capital: %lld\n\n", (Cash)(bob.bank_capital / 100.));
    }    

}

Cash persons_difference() {  // Расчёт разницы в доходах через 30 лет
    if (CURRENT_YEAR * 12 + CURRENT_MONTH == END_YEAR * 12 + END_MONTH - 1) {
       Cash difference = (Cash)((alice.bank_capital + FLAT - bob.bank_capital) / 100.);
       return difference;
    }
}

void print_final() {
        if (CURRENT_YEAR * 12 + CURRENT_MONTH == END_YEAR * 12 + END_MONTH - 1) {

        printf("Final!\n");

        Cash money_difference = persons_difference();

        if ( money_difference > 0) {
            printf("Alice's strategy is more profitable! She became richer than Bob by %lld money!\n\n", money_difference);
        } 
        else {
            printf("Bob's strategy is more profitable! He became richer than Alice by %lld money!\n\n", llabs( money_difference ));
        }

    }

}

void processing() {
    while (CURRENT_YEAR * 12 + CURRENT_MONTH != END_YEAR * 12 + END_MONTH) {
        alice_dohod();
        alice_month_expences();
        alice_job_promotion();

        bob_dohod();
        bob_month_expences();
        
        common_inflation();
        
        print_information();
        persons_difference();
        print_final();

        CURRENT_MONTH++;
        if (CURRENT_MONTH % 13 == 0) {
            CURRENT_MONTH = 1;
            CURRENT_YEAR++;
        }
    }
        
    
}

int main() {
    alice_person();
    bob_person();
    processing();
       return 0;
}
