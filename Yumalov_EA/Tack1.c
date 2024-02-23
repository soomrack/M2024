#include <stdio.h>
#include <math.h>

typedef long long Money;

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
//Money vklad = 2000000 * 100;
//int vklad_rate = 10;
//Money wastes_bob = eda + odezda + intertaiments + rent;

//Dla vsex
Money eda = 15000 * 100;
Money odezda = 5000 * 100; 
Money intertaiments = 10000 * 100; 
int inflation = 12;
int num_of_years = 30;
//Money salary = 150000 * 100;

Money alice_monthly_payment () {
    float monthly_ipoteka_rate = ipoteka_rate / 12 / 100;
    int total_payments = num_of_years * 12; // Вычисляем общее количество платежей
    Money ipoteka = (kredit * monthly_ipoteka_rate) / (1 - pow(1 + monthly_ipoteka_rate, -total_payments)); // Вычисляем ежемесячный платеж используя формулу аннуитетного платежа
    return ipoteka;
}

long inflatia(long value, int time_unit){ // Функция считает сумму всех значений, повышенных на инфляцию
    long sum = 0;
    for ( int i = 0; i < time_unit; i++ ) {
        value = value * (1 + inflation / 100);
        sum = sum + value;
    }
    return sum;
}
  
void init_bob () {
    bob.salary = 150 * 1000 * 100;
    bob.bank_account = 2 * 1000 * 1000 * 100;
    bob.saving_account_rate = 16;
}

void init_alice () {
    alice.salary = 150 * 1000 * 100;
    alice.bank_account = 100 * 1000 * 100;
    alice.credit_monthly_payment = alice_monthly_payment ();
}

int current_year = 2024;
int current_month = 2;
int end_year = 2024;
int end_month = 2;

void inflation_alice () {
    if (current_month == 1) {
        alice.salary *= (1.0 + inflation / 100.0);
        kvartira *= (1.0 + inflation / 100.0);
    }
}
void enrollment_alice () {
    alice.bank_account += alice.salary;
}

void spendings_alice () {
    alice.bank_account -= eda + odezda + intertaiments;
    if (current_year < 2027) {
        alice.bank_account -= remont;
    }
    alice.bank_account -= alice.credit_monthly_payment;
}

void print_alice () {
    printf("Alice: money %lld\n", alice.bank_account + kvartira);
}

void processing () {
    while (current_year * 12 + current_month != end_year * 12 + end_month) {
        inflation_alice ();
        enrollment_alice ();
        spendings_alice ();
        print_alice ();

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
   // inflatia (sum);
   // print_results(alice_capital, bob_capital);
    return 0;
}
// Функция для вывода результатов
//void print_results(long alice_capital, long bob_capital) {
 //   printf("Alice's capital including the value of the apartment in the end: %.2f rubles\n", alice_capital);
 //   printf("Bob's capital in the end: %.2f rubles\n", bob_capital);
 
