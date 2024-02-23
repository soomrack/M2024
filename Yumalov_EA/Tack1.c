#include <stdio.h>
#include <math.h>

typedef long long Money; //Задаем новый тип данных для сокращения

struct Person{ 
    char* name;
    Money salary;
    Money bank_account;
    Money credit_monthly_payment;
    int saving_account_rate;
    //int credit_rate;
} alice, bob;

//Alice
Money kvartira = 20000000 * 100;
Money kredit = 18000000 * 100;
int ipoteka_rate = 5;
Money remont = 50000 * 100;
//Money wastes_alice = eda + odezda + intertaiments;

//Bob
Money rent = 25000 * 100;

//Common
Money eda = 15000 * 100;
Money odezda = 5000 * 100; 
Money intertaiments = 10000 * 100; 
int inflation = 12;
int num_of_years = 30;
int current_year = 2024;
int current_month = 2;
int end_year = 2054;
int end_month = 2;

Money alice_monthly_payment() { //Расчет ежемесячного платежа по ипотеке
    float monthly_ipoteka_rate = ipoteka_rate / 12.0 / 100.0;
    int total_payments = num_of_years * 12; // Вычисляем общее количество платежей
    Money ipoteka = (kredit * monthly_ipoteka_rate) / (1 - pow(1 + monthly_ipoteka_rate, -total_payments)); // Вычисляем ежемесячный платеж используя формулу аннуитетного платежа
    return ipoteka;
}

void init_bob() { //Боб
    bob.salary = 150 * 1000 * 100;
    bob.bank_account = 2 * 1000 * 1000 * 100;
    bob.saving_account_rate = 16;
}


void init_alice() { //Алиса
    alice.salary = 150 * 1000 * 100;
    alice.bank_account = 100 * 1000 * 100;
    alice.credit_monthly_payment = alice_monthly_payment ();
}


void inflation_common() { //Инфляция
    if (current_month == 1) {
        alice.salary *= (1.0 + inflation / 100.0);
        kvartira *= (1.0 + inflation / 100.0);
        bob.salary *= (1.0 + inflation / 100.0);
        eda *= (1.0 + inflation / 100.0);
        odezda *= (1.0 + inflation / 100.0);
        intertaiments *= (1.0 + inflation / 100.0);

    }
}


void enrollment_alice() {
    alice.bank_account += alice.salary;
}


void enrollment_bob() {
    if (current_month == 1) {
        bob.bank_account *= (1.0 + bob.saving_account_rate / 100.0);
    }
    bob.bank_account += bob.salary;
}

void spendings_alice() {
    alice.bank_account -= eda + odezda + intertaiments;
    if (current_year < 2027) {
        alice.bank_account -= remont;
    }
    alice.bank_account -= alice.credit_monthly_payment;
}


void spendings_bob() {
    bob.bank_account -= eda + odezda + intertaiments + rent;
}



void print_alice() {
    if (current_month == 1) {
       
    
        printf("[%d]Alice: money %lld\n", current_year, alice.bank_account + kvartira);
    }    
}


void print_bob() {
    if (current_month == 1) {
       
    
        printf("[%d]Bob: money %lld\n", current_year, bob.bank_account);
    }    
}


void processing() {
    while (current_year * 12 + current_month != end_year * 12 + end_month) {
        inflation_common();
        enrollment_alice();
        enrollment_bob();
        spendings_alice();
        spendings_bob();
        print_alice();
        print_bob();

        current_month++;
        if (current_month % 13 == 0) {
            current_month = 1;
            current_year++;
        }
    }
}


int main() {
    init_alice ();
    init_bob ();
    processing ();
       return 0;
}
