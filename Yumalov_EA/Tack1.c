#include <stdio.h>
#include <math.h>

typedef long long Money;  // Задаем новый тип данных для сокращения

struct Person{ 
    char* name;
    Money salary;
    Money bank_account;
    Money credit_monthly_payment;
    int saving_account_rate;
   } alice, bob;

  // Alice
Money kvartira = 20000000 * 100;
Money KREDIT = 18000000 * 100;
int IPOTEKA_RATE = 5;
Money REMONT = 50000 * 100;


  // Bob
Money RENT = 25000 * 100;

  // Common
Money EDA = 15000 * 100;
Money ODEZDA = 5000 * 100; 
Money INTERTAIMENTS = 10000 * 100; 
int INFLATION = 12;
int NUM_OF_YEARS = 30;
int CURRENT_YEAR = 2024;
int CURRENT_MONTH = 2;
int END_YEAR = 2054;
int END_MONTH = 2;

Money alice_monthly_payment() {  // Расчет ежемесячного платежа по ипотеке
    float monthly_ipoteka_rate = IPOTEKA_RATE / 12.0 / 100.0;
    int total_payments = NUM_OF_YEARS * 12; // Вычисляем общее количество платежей
    Money ipoteka = (KREDIT * monthly_ipoteka_rate) / (1 - pow(1 + monthly_ipoteka_rate, -total_payments)); // Вычисляем ежемесячный платеж используя формулу аннуитетного платежа
    return ipoteka;
}


void init_bob() {  // Боб
    bob.salary = 150 * 1000 * 100;
    bob.bank_account = 2 * 1000 * 1000 * 100;
    bob.saving_account_rate = 10;
    return bob.salary;
}


void init_alice() {  // Алиса
    alice.salary = 150 * 1000 * 100;
    alice.bank_account = 100 * 1000 * 100;
    alice.credit_monthly_payment = alice_monthly_payment ();
}


void common_inflation() {  // Инфляция
    if (CURRENT_MONTH == 1) {
        alice.salary *= (1.0 + INFLATION / 100.0);
        kvartira *= (1.0 + INFLATION / 100.0);
        bob.salary *= (1.0 + INFLATION / 100.0);
        EDA *= (1.0 + INFLATION / 100.0);
        ODEZDA *= (1.0 + INFLATION / 100.0);
        INTERTAIMENTS *= (1.0 + INFLATION / 100.0);
    }
}


void alice_zp() {
    alice.bank_account += alice.salary;
}


void bob_zp() {
    bob.bank_account += bob.salary;
    static Money salary = 0;
    if ((CURRENT_YEAR == 2026 &&  CURRENT_MONTH == 12) || (CURRENT_YEAR == 2026 && CURRENT_MONTH == 1)){
        salary = 0;
    }
   // if (CURRENT_YEAR == 2027 && CURRENT_MONTH == 2) {
    salary = bob.salary * 1.5;
    } 
}


void bob_vklad() {
    if (CURRENT_MONTH == 1) {
        bob.bank_account *= (1.0 + bob.saving_account_rate / 100.0);
    }
}


void common_eda() {
    alice.bank_account -= EDA;
    bob.bank_account -= EDA;
}


void alice_odezda() {
    alice.bank_account -= ODEZDA;
    }


void bob_odezda() {
     if (CURRENT_YEAR == 2026 && CURRENT_MONTH == 2){
        Money kostuym = 200*1000*100;
        bob.bank_account -= kostuym;
    }
    bob.bank_account -= ODEZDA;
}


void common_intertaiments() {
    alice.bank_account -= INTERTAIMENTS;
    bob.bank_account -= INTERTAIMENTS;
}


void alice_ipoteka() {
  alice.bank_account -= alice.credit_monthly_payment;
}


void alice_remont() {
     if (CURRENT_YEAR < 2027) {
        alice.bank_account -= REMONT;
    }
}


void bob_rent() {
    bob.bank_account -= RENT;
}


void print_common() {
    if (CURRENT_MONTH == 1) {
        printf("[%d]Alice: money %lld\n", CURRENT_YEAR, alice.bank_account / 100 + kvartira / 100);
        printf("[%d]Bob: money %lld\n", CURRENT_YEAR, bob.bank_account / 100);
    }    
}


void processing() {
    while (CURRENT_YEAR* 12 + CURRENT_MONTH != END_YEAR * 12 + END_MONTH) {
        alice_zp();
        bob_zp();
        bob_vklad();
        common_eda();
        bob_odezda();
        alice_odezda();
        common_intertaiments();
        alice_ipoteka();
        alice_remont();
        bob_rent();
        common_inflation();
        print_common();
        CURRENT_MONTH++;
        if (CURRENT_MONTH % 13 == 0) {
            CURRENT_MONTH = 1;
            CURRENT_YEAR++;
        }
    }
}


void result() {
    if (CURRENT_YEAR = END_YEAR) {
        printf("In %d years Alice will have %lld rubles \n ", NUM_OF_YEARS, alice.bank_account / 100 + kvartira / 100);
        printf("In %d years Alice flat will cost %lld rubles \n ", NUM_OF_YEARS, kvartira / 100);
        printf("In %d years Bob will have %lld rubles \n ", NUM_OF_YEARS, bob.bank_account / 100);
    }
}


int main() {
    init_alice();
    init_bob();
    processing();
    result();
       return 0;
}

// боб в 2026 в феврале покупает сбе костюм за 200 тыщ, потерял работу на 2 месяца (дек и янв), в феврале нашел работу с зп в 1.5 р больше
