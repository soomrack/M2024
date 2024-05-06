#include <stdio.h>
#include <stdbool.h>

// Alice
long FLAT_PRICE = 20 * 1000 * 1000;
long CREDIT = 18 * 1000 * 1000;
double CREDIT_RATE = 0.11;
long FLAT_RENOVATION = 100 * 1000;
// Bob
long FLAT_RENT = 25 * 1000;
double DEPOSIT_RATE = 0.16;
// Common
long WASTES = 30 * 1000;
const double INFLATION = 0.12;
const int YEARS = 30;
int MONTHS = 30 * 12;

int START_YEAR = 2024;
int END_YEAR = 2054;
int END_FLAT_RENOVATION_YEAR = 2026;

typedef struct{
    char* name;
    long salary;
    long expenses;
    int flat_renovation_years;
    bool hasCredit;
    long credit_payment;
} Person;
Person alice = {"Alice", 150 * 1000, 30 * 1000, 2, true, 0};
Person bob = { "Bob", 150 * 1000, 30 * 1000 + 20 * 1000, 0, false, 0};

// Считает степень
double recursive(double base, int exponent){
    if (exponent == 0)
        return 1;
    else
        return base * recursive(base, exponent - 1);
}

// Ежемесячная плата по кредиту при аннуитентном платеже
void annuity_monthly_payment(long *monthly_payment){
    double monthly_credit_rate = CREDIT_RATE / 12;
    double all_credit_time_inflation = \
    recursive((1 + monthly_credit_rate), YEARS * 12);
    // Коэффициент К из экономической формулы 
    double K = (monthly_credit_rate * all_credit_time_inflation)/\
    (all_credit_time_inflation - 1);

    *monthly_payment = (long)(CREDIT * K);
}

// Функция считает повышение на процент
long percent_calculation(long  value, int rate){
    value = value * (1 + rate);
    return value;
}


// Остаток год
long person_year_balance(Person person){
    long person_year_balance = (person.salary - person.expenses - person.credit_payment) * 12;
    return person_year_balance;
}

// Траты в год
long person_year_expenses(Person person){
    long person_year_expenses = \
        (person.expenses + person.credit_payment + FLAT_RENOVATION * person.flat_renovation_years) * 12;

    return person_year_expenses;
}

// Сколько всего денег потратил person за весь период
long person_expenses_entire_period(Person person){
    int CURRENT_YEAR = START_YEAR;
    while (CURRENT_YEAR != END_YEAR)
    {
        CURRENT_YEAR = START_YEAR + 1;
        person_year_expenses(person) * (1 + INFLATION);
    }
}

int who_paid_more(){
    if (person_expenses_entire_period(alice) != person_expenses_entire_period(bob)){
        if (person_expenses_entire_period(alice) < person_expenses_entire_period(bob)){
            return 1;
        }
        else return 2;
    }
    else return 0;
    
}

// Сколько будет стоить квартира
long final_flat_cost_func(){
    int CURRENT_YEAR = START_YEAR;
    while (CURRENT_YEAR != END_YEAR );
    {
        CURRENT_YEAR = START_YEAR + 1;
        long final_flat_cost = 0;
        long month_flat_cost = percent_calculation(FLAT_PRICE, INFLATION);
        final_flat_cost += percent_calculation(FLAT_PRICE, INFLATION);
    }
}

// Сколько у Боба на вкладе накопилось
long bob_deposit_balance(){
    int CURRENT_YEAR = START_YEAR;
    long deposit_balance = 0;
    while (CURRENT_YEAR != END_YEAR );
    {
        deposit_balance += person_year_balance(bob);
        deposit_balance = percent_calculation(deposit_balance, DEPOSIT_RATE);

        return deposit_balance;
    }
}

int can_bob_buy_flat(){
    if (bob_deposit_balance() < final_flat_cost_func())
        return 0;
    else
        return 1;
}

void print_func(){
    if (can_bob_buy_flat() == 0)
        printf("Bob can't buy a flat!\n");
    else printf("Bob bougth a flat.\n");
    switch (who_paid_more()){
    case 1:
        printf("Bob paid more.\n");
        break;
    case 2:
        printf("Alice paid more.\n");
    default:
        printf("No one paid more.\n");
        break;
    }
}

long main() {
//  В alice_year_balance передавать посчитанную месячную плату предиту
    person_expenses_entire_period(alice);
    person_expenses_entire_period(bob);
    who_paid_more();
    can_bob_buy_flat();

    print_func();

    return 0;    
}