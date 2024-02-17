// Liubinetskii V. (@liubvlad)
// 
// code style for C
// https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide/
// https://github.com/MaJerle/c-code-style
//
// variable naming rules for C -- snake_case
//
// (!) All interest rates are expressed on an annual basis
//
// ☑ Вернуть отработку помесячной индексации
//
// ☑ Увеличить начальный капитал
//
// ☑ года текущие
// ☑ отсутпы
// ☑ змея_кэйс
//
// ☑ в main только важное
//
// ☑ Кредит работает как обязательство на n-лет ☑
// ☑ Добавить собственность человека
// □ Освобождать память

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logger.h"


#define MAX_ELEMENTS_COUNT 8
#define MSG_BUFF 512

#define CU 100
#define THOUSAND_CU 1000 * CU
#define MILLION_CU 1000 * THOUSAND_CU

#define CREDIT_RATE 0.18
#define DEPOSIT_RATE 0.16
#define INFLATION_RATE 0.12
#define PROCESSING_YEARS 30
#define MONTHS_IN_YEAR 12


typedef long long Money;

double сredit_сalculation_recursion(double base, int exponent);
Money credit_get_monthly_payment(Money credit_amount, double annual_credit_rate, int duration_months);

char* generate_line(char sym, int length);
char* generate_line_with_text(char sym, int length, const char* text);


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
    // TODO ADD replenishment percentage 
};

struct Property {
    char* name;
    Money cost;
};

typedef struct Person {
    char* name;
    Money balance;
    Money salary;

    struct Expenses* expenses[MAX_ELEMENTS_COUNT];
    int expenses_count;
    struct Credit* credits[MAX_ELEMENTS_COUNT];
    int credits_count;
    struct Savings* savings[MAX_ELEMENTS_COUNT];
    int savings_count;
    struct Property* properties[MAX_ELEMENTS_COUNT];
    int properties_count;
};

#pragma region Creating structs for Person

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

    struct Expenses* expense = malloc(sizeof(struct Expenses));
    expense->name = name;
    expense->amount = cost;

    if (duration_in_months > 0) {
        expense->has_duration = true;
        expense->duration_months = duration_in_months;
    }
    else {
        expense->has_duration = false;
        expense->duration_months = 0;
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

    struct Credit* credit = malloc(sizeof(struct Credit));
    credit->name = name;
    credit->amount = credit_amount;
    credit->duration_months = duration_months;
    credit->interest_rate = rate;

    credit->monthly_payment = credit_get_monthly_payment(credit_amount, rate, duration_months);

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

    struct Savings* saving = malloc(sizeof(struct Savings));
    saving->name = name;
    saving->balance = balance;
    saving->interest_rate = rate;

    person->savings[person->savings_count] = saving;
    person->savings_count += 1;

    return 1;
}


int create_property(struct Person* person, char* name, Money cost) {

    char* message = (char*)malloc(MSG_BUFF * sizeof(char));
    snprintf(message, MSG_BUFF, "Creating property '%s' for person '%s'",
        name,
        person->name);
    log((struct Log_message) { Debug, message });
    free(message);

    if (person->expenses_count == MAX_ELEMENTS_COUNT)
    {
        log((struct Log_message) { Warning, "Failed create property - out of range!" });
        return 0;
    }

    struct Property* property = malloc(sizeof(struct Property));
    property->name = name;
    property->cost = cost;

    person->properties[person->properties_count] = property;
    person->properties_count += 1;

    return 1;
}

#pragma endregion


int current_year = 2024;
int current_month = 2;

struct Person* persons[MAX_ELEMENTS_COUNT];
int persons_count = 0;


void money_under_rate(Money* money, double rate) {
    if (money == NULL) {
        log((struct Log_message) { Error, "Null pointer passed" });
        return;
    }

    if (rate < -1.0 || rate > 1.0) {
        log((struct Log_message) { Error, "Wrong interest rate" });
        return;
    }

    *money *= (1 + rate);
}


void print_person_data(struct Person* person) {
    printf(generate_line_with_text('-', 60, "Person statistic"));

    printf("%s: balance = %lld.%.2lld, salary = %lld.%.2lld\n\n",
        person->name,
        person->balance / 100,
        person->balance % 100,
        person->salary / 100,
        person->salary % 100);

    printf("Expenses count = %d\n", person->expenses_count);
    for (int i = 0; i < person->expenses_count; i++) {
        printf("\t[%d] Expense info: Name = %s\n", i, person->expenses[i]->name);
        printf("\t[%d] Amount = %lld.%.2lld\n", i,
            person->expenses[i]->amount / 100,
            person->expenses[i]->amount % 100);
        if (person->expenses[i]->has_duration) {
            printf("\t[%d] Duration months = %d\n", i, person->expenses[i]->duration_months);
        }

        printf("\n");
    }

    printf("Credits count = %d\n", person->credits_count);
    for (int i = 0; i < person->credits_count; i++) {
        printf("\t[%d] Credit info: Name = %s\n", i, person->credits[i]->name);
        printf("\t[%d] Amount = %lld.%.2lld\n", i,
            person->credits[i]->amount / 100,
            person->credits[i]->amount % 100);
        printf("\t[%d] Monthly payment = %lld.%.2lld\n", i,
            person->credits[i]->monthly_payment / 100,
            person->credits[i]->monthly_payment % 100);
        printf("\t[%d] Interest rate = %.2f %%\n", i, person->credits[i]->interest_rate * 100);
        printf("\t[%d] Duration months = %d\n", i, person->credits[i]->duration_months);
        printf("\n");
    }

    printf("Savings count = %d\n", person->savings_count);
    for (int i = 0; i < person->savings_count; i++) {
        printf("\t[%d] Expense info: Name = %s\n", i, person->savings[i]->name);
        printf("\t[%d] Balance = %lld.%.2lld\n", i,
            person->savings[i]->balance / 100,
            person->savings[i]->balance % 100);
        printf("\t[%d] Interest rate = %.2f %%\n", i, person->savings[i]->interest_rate * 100);
        printf("\n");
    }

    printf("Properties count = %d\n", person->properties_count);
    for (int i = 0; i < person->properties_count; i++) {
        printf("\t[%d] Property info: Name = %s\n", i, person->properties[i]->name);
        printf("\t[%d] Cost = %lld.%.2lld\n", i,
            person->properties[i]->cost / 100,
            person->properties[i]->cost % 100);
        printf("\n");
    }

    printf(generate_line('=', 60));
    printf("\n");
}


void print_persons_data() {
    for (int i = 0; i < persons_count; i++) {
        struct Person* person = persons[i];
        print_person_data(person);
    }
}


void print_persons_total_balance() {
    printf("Balance status on %d.%02d\n", current_year, current_month);

    for (int i = 0; i < persons_count; i++) {
        struct Person* person = persons[i];
        Money total_balance = person->balance;

        for (int i = 0; i < person->credits_count; i++)
        {
            if (person->credits[i]->duration_months > 0) {
                total_balance -= person->credits[i]->monthly_payment * person->credits[i]->duration_months;
            }
        }

        for (int i = 0; i < person->savings_count; i++)
        {
            total_balance += person->savings[i]->balance;
        }

        for (int i = 0; i < person->properties_count; i++)
        {
            total_balance += person->properties[i]->cost;
        }

        printf("[%s] Total balance = %lld.%.2d\n", 
            person->name,
            total_balance / 100,
            abs(total_balance % 100));
    }

    printf("\n");
}


void start_processing(int months_duration) {
    print_persons_total_balance();

    for (int month = 0; month < months_duration; month++)
    {
        current_month++;
        if (current_month > MONTHS_IN_YEAR) {
            current_month -= MONTHS_IN_YEAR;
            current_year++;

            // (!) every year - implement inflation(indexation)
            for (int i = 0; i < persons_count; i++) {
                struct Person* person = persons[i];

                money_under_rate(&(person->balance), INFLATION_RATE);
                money_under_rate(&(person->salary), INFLATION_RATE);

                for (int i = 0; i < person->expenses_count; i++)
                {
                    money_under_rate(&(person->expenses[i]->amount), INFLATION_RATE);
                }

                // TODO let's go to monthly rates
                for (int i = 0; i < person->savings_count; i++)
                {
                    money_under_rate(&(person->savings[i]->balance), person->savings[i]->interest_rate);
                }

                for (int i = 0; i < person->properties_count; i++)
                {
                    money_under_rate(&(person->properties[i]->cost), INFLATION_RATE);
                }
                
            }

            printf("Information after indexation/inflation of %d.%02d\n", current_year, current_month);
            print_persons_data();
        }
        
        for (int i = 0; i < persons_count; i++) {
            struct Person* person = persons[i];

            // salary comes
            person->balance += person->salary;

            // TO BE DONE savings accounts (monthly rating) 

            // payment on loans/credits
            for (int i = 0; i < person->credits_count; i++)
            {
                // TODO check money balance should be here
                person->balance -= person->credits[i]->monthly_payment;
                person->credits[i]->duration_months--;
            }

            if (person->balance < 0) {
                printf("Oups.. %s is now bankrupt before loads payment.. Balance = %lld.%.2lld\n",
                    person->name,
                    person->balance / 100,
                    person->balance % 100);
            }

            // pay expenses
            for (int i = 0; i < person->expenses_count; i++)
            {
                // TODO check money balance should be here
                if (person->expenses[i]->has_duration) {
                    if (person->expenses[i]->duration_months > 0) {
                        person->balance -= person->expenses[i]->amount;
                        person->expenses[i]->duration_months--;
                    }
                }
                else {
                    person->balance -= person->expenses[i]->amount;
                }
            }

            if (person->balance < 0) {
                printf("Oups.. %s is now bankrupt before expenses payment.. Balance = %lld.%.2lld\n",
                    person->name,
                    person->balance / 100,
                    person->balance % 100);
            }

            // replenish your savings account
            // TEMP top up only the first saving account
            for (int i = 0; i < person->savings_count; i++)
            {
                if (person->balance > 0) {
                    person->savings[0]->balance += person->balance;
                    person->balance = 0;
                }
            }
        }

        print_persons_total_balance();
    }
}


void person_create_Alice() {
    struct Person* alice = malloc(sizeof(struct Person));

    alice->name = "Alice";
    alice->balance = 3 * MILLION_CU;
    alice->salary = 300 * THOUSAND_CU;

    alice->credits_count = 0;
    alice->expenses_count = 0;
    alice->savings_count = 0;
    alice->properties_count = 0;

    create_expense(alice, "Eat", 15 * THOUSAND_CU, 0);
    create_expense(alice, "Renovation", 100 * THOUSAND_CU, 2 * MONTHS_IN_YEAR);

    Money flat_cost = 20 * MILLION_CU;
    Money initial_fee = 2 * MILLION_CU;
    Money mortgage_amount = flat_cost - initial_fee;
    alice->balance -= initial_fee;
    create_credit(alice, "Mortgage", mortgage_amount, CREDIT_RATE, PROCESSING_YEARS * MONTHS_IN_YEAR);

    create_property(alice, "My little home", flat_cost);

    persons[persons_count] = alice;
    persons_count += 1;
}


void person_create_Bob() {
    struct Person* bob = malloc(sizeof(struct Person));

    bob->name = "Bob  ";
    bob->balance = 3 * MILLION_CU;
    bob->salary = 300 * THOUSAND_CU;

    bob->credits_count = 0;
    bob->expenses_count = 0;
    bob->savings_count = 0;
    bob->properties_count = 0;

    create_expense(bob, "Eat", 15 * THOUSAND_CU, 0);
    create_expense(bob, "Flat", 30 * THOUSAND_CU, 0);

    create_saving(bob, "Deposit", 0, DEPOSIT_RATE);

    persons[persons_count] = bob;
    persons_count += 1;
}


int main() {

    person_create_Alice();
    person_create_Bob();

    print_persons_data();

    start_processing(PROCESSING_YEARS * MONTHS_IN_YEAR);

    // TODO garbage collector :)

    return 1;
}

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

#pragma region Vizual methods

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

#pragma endregion
