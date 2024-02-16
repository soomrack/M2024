// Liubinetskii V. (@liubvlad)
// 
// code style for C
// https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide/
// https://github.com/MaJerle/c-code-style
//
// variable naming rules for C -- snake_case
//
// (!) All interest rates are expressed on an annual basis

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logger.h"


#define MAX_ELEMENTS_COUNT 8
#define MSG_BUFF 512

#define CENTS 100
#define THOUSAND_C 1000 * CENTS
#define MILLION_C 1000 * THOUSAND_C
#define CREDIT_RATE 0.18
#define DEPOSIT_RATE 0.16
#define INFLATION_RATE 0.12
#define YEARS 30
#define MONTHS_IN_YEAR 12


typedef long long Money;

double сredit_сalculation_recursion(double base, int exponent);
Money credit_get_monthly_payment(Money credit_amount, double annual_credit_rate, int duration_months);


struct Expenses {
    char* name;
    Money amount;
    bool has_duration;
    int duration_months;
};


struct Credit {
    char* name;
    Money amount;
    Money monthly_payment;
    double interest_rate;
    int duration_months;
    // TODO take init&save init duration
};


struct Savings {
    char* name;
    Money balance;
    double interest_rate;
};


typedef struct Person {
    char* name;
    Money balance;
    Money salary;

    struct Expenses expenses[MAX_ELEMENTS_COUNT];
    int expenses_count;
    struct Credit credits[MAX_ELEMENTS_COUNT];
    int credits_count;
    struct Savings savings[MAX_ELEMENTS_COUNT];
    int savings_count;

    // TODO can add some Property
};


int create_expense(struct Person* person, char* name, Money cost, int duration_in_months) {
    
    char* message = (char*)malloc(MSG_BUFF * sizeof(char));
    snprintf(message, MSG_BUFF, "Creating expense '%s' for person '%s'",
        name,
        person->name);
    log((struct Log_message) { Debug, message });
    free(message);
    
    if (person->expenses_count == MAX_ELEMENTS_COUNT)
    {
        log((struct Log_message) { Warning, "Failed create expense - out of range!" });
        return 0;
    }

    struct Expenses expense;
    expense.name = name;
    expense.amount = cost;

    if (duration_in_months > 0) {
        expense.has_duration = true;
        expense.duration_months = duration_in_months;
    }
    else {
        expense.has_duration = false;
        expense.duration_months = 0;
    }

    person->expenses[person->expenses_count] = expense;
    person->expenses_count += 1;

    return 1;
}


int create_credit(struct Person* person, char* name, Money credit_amount, double rate, int duration_months) {
    
    char* message = (char*)malloc(MSG_BUFF * sizeof(char));
    snprintf(message, MSG_BUFF, "Creating credit '%s' for person '%s'",
        name,
        person->name);
    log((struct Log_message) { Debug, message });
    free(message);

    if (person->credits_count == MAX_ELEMENTS_COUNT)
    {
        log((struct Log_message) { Warning, "Failed create credit - out of range!" });
        return 0;
    }

    struct Credit credit;
    credit.name = name;
    credit.amount = credit_amount;
    credit.duration_months = duration_months;
    credit.interest_rate = rate;

    credit.monthly_payment = credit_get_monthly_payment(credit_amount, rate, duration_months);

    person->credits[person->credits_count] = credit;
    person->credits_count += 1;

    return 1;
}


int create_saving(struct Person* person, char* name, Money balance, double rate) {
    
    char* message = (char*)malloc(MSG_BUFF * sizeof(char));
    snprintf(message, MSG_BUFF, "Creating saving account '%s' with initial balance '%lld.%.2lld' for person '%s'",
        name,
        balance / 100,
        balance % 100,
        person->name);
    log((struct Log_message) { Debug, message });
    free(message);
    
    if (person->credits_count == MAX_ELEMENTS_COUNT)
    {
        log((struct Log_message) { Warning, "Failed create saving account - out of range!" });
        return 0;
    }

    struct Savings saving;
    saving.name = name;
    saving.balance = balance;
    saving.interest_rate = rate;

    person->savings[person->savings_count] = saving;
    person->savings_count += 1;

    return 1;
}


char* generate_line(char sym, int length) {
    char* line = (char*)malloc(length + 2 * sizeof(char));
    
    for (int i = 0; i < length; ++i) {
        line[i] = sym;
    }

    line[length - 1] = '\n';
    line[length] = '\0';
    return line;
}


char* generate_line_with_text(char sym, int length, const char* text) {
    size_t text_length = strlen(text);
    if (text_length > length) {
        return text;
    }

    int left_padding = (length - text_length) / 2;
    char* line_with_text = generate_line(sym, length);

    for (int i = 0; i < text_length; i++) {
        line_with_text[i + left_padding] = text[i];
    }

    return line_with_text;
}


int current_year = 2024;
int current_month = 2;

struct Person persons[MAX_ELEMENTS_COUNT];
int persons_count = 0;


// TEMP & OBSOLETE
void print_person_data(struct Person* person) {
    printf(generate_line_with_text('-', 60, "Person statistic"));

    printf("%s: balance = %lld.%.2lld, salary = %lld.%.2lld\n\n",
        person->name,
        person->balance / 100,
        person->balance % 100,
        person->salary / 100,
        person->salary % 100);

    printf("Expenses count = %d\n", person->expenses_count);
    for (int i = 0; i < person->expenses_count; i++)
    {
        printf("\t[%d] Expense info: Name = %s\n", i, person->expenses[i].name);
        printf("\t[%d] Amount = %lld.%.2lld\n", i,
            person->expenses[i].amount / 100,
            person->expenses[i].amount % 100);
        if (person->expenses[i].has_duration) {
            printf("\t[%d] Duration months = %d\n", i, person->expenses[i].duration_months);
        }

        printf("\n");
    }

    printf("Credits count = %d\n", person->credits_count);
    for (int i = 0; i < person->credits_count; i++)
    {
        printf("\t[%d] Credit info: Name = %s\n", i, person->credits[i].name);
        printf("\t[%d] Amount = %lld.%.2lld\n", i,
            person->credits[i].amount / 100,
            person->credits[i].amount % 100);
        printf("\t[%d] Monthly payment = %lld.%.2lld\n", i,
            person->credits[i].monthly_payment / 100,
            person->credits[i].monthly_payment % 100);
        printf("\t[%d] Interest rate = %.2f %%\n", i, person->credits[i].interest_rate * 100);
        printf("\t[%d] Duration months = %d\n", i, person->credits[i].duration_months);
        printf("\n");
    }

    printf("Savings count = %d\n", person->savings_count);
    for (int i = 0; i < person->savings_count; i++)
    {
        printf("\t[%d] Expense info: Name = %s\n", i, person->savings[i].name);
        printf("\t[%d] Balance = %lld.%.2lld\n", i,
            person->savings[i].balance / 100,
            person->savings[i].balance % 100);
        printf("\t[%d] Interest rate = %.2f %%\n", i, person->savings[i].interest_rate * 100);
        printf("\n");
    }

    printf(generate_line('=', 60));
    printf("\n");
}


void print_persons_data() {
    for (int i = 0; i < persons_count; i++) {
        struct Person* person = &persons[i];
        print_person_data(person);
    }
}


void person_create_Alice() {
    struct Person alice = {
        .name = "Alice",
        .balance = 2 * MILLION_C,
        .salary = 300 * THOUSAND_C,
    };

    create_expense(&alice, "Parking", 9000 * CENTS, 12);
    create_expense(&alice, "Life_trats", 20000 * CENTS, 0);

    create_credit(&alice, "Micro-ZAIM", 2 * MILLION_C, CREDIT_RATE, 36);
    create_credit(&alice, "Ipoteka++", 18 * MILLION_C, CREDIT_RATE, 30 * MONTHS_IN_YEAR);

    persons[persons_count] = alice;
    persons_count += 1;
}


void person_create_Bob() {
    struct Person bob = {
        .name = "Bob",
        .balance = 2 * MILLION_C,
        .salary = 300 * THOUSAND_C,
    };

    create_expense(&bob, "Eat", 15 * THOUSAND_C, 0);
    create_expense(&bob, "Flat", 25 * THOUSAND_C, 0);

    create_saving(&bob, "Deposit", 2 * MILLION_C, DEPOSIT_RATE);

    persons[persons_count] = bob;
    persons_count += 1;
}


int main() {

    person_create_Alice();
    person_create_Bob();

    print_persons_data();

    return 0;
}

    /*

    // Использовать реальные года и месяцы...

    for (int month = 1; month <= YEARS * MONTHS_IN_YEAR; month++) {
            update_monthly_situation(&alice, &bob);
            print_monthly_situation(month, alice, bob);
        }

    void update_monthly(Person* person) {
        alice->loan.balance -= alice->loan.monthly_payment;
        Money interest = bob->savings.balance * bob->savings.interest_rate / MONTHS_IN_YEAR;
        bob->savings.balance += interest;
        //bob->savings.balance *= (1 + GROWTH_RATE / MONTHS_IN_YEAR);
    }

    */

#pragma region Black box for calculating credit monthly payment

double сredit_сalculation_recursion(double base, int exponent) {
    if (exponent == 0)
        return 1;
    else
        return base * сredit_сalculation_recursion(base, exponent - 1);
}

Money credit_get_monthly_payment(Money credit_amount, double annual_credit_rate, int duration_months) {
    double monthly_credit_rate = annual_credit_rate / 12;

    double all_credit_time_inflation = сredit_сalculation_recursion((1 + monthly_credit_rate), duration_months);

    double coefficient = (monthly_credit_rate * all_credit_time_inflation) / (all_credit_time_inflation - 1);

    Money monthlyPayment = (Money)(credit_amount * coefficient);
    return monthlyPayment;
}

#pragma endregion


/*
  □ Вернуть отработку помесячной индексации
 

  □ Увеличить начальный капитал
  □ года текущие
  ☑отсутпы
  ☑змея_кэйс

  □ в майн только важное

  ☑Кредит работает как обязательство на n-лет ☑
*/
