#include <stdio.h>
#include <locale.h>

typedef long long int money;

const int simulation_time_years = 30;

const money start_capital = 2 * 1000 * 1000 * 100;
const money flat_price = 20 * 1000 * 1000 * 100;
const money salary = 350 * 1000 * 100;

const money flat_rent = 80 * 1000 * 100;
const double savings_percents = 0.16;

const double credit_percents = 0.175;

const money food_spendings = 20 * 1000 * 100;
const money life_spendings = 15 * 1000 * 100;
const money random_spendings = 8 * 1000 * 100;
const double inflation = 0.12;

typedef struct {
    money debt_cents;
    int rate;
} CreditAccount;

int is_credit_done(CreditAccount* credit) {
    if (credit->debt_cents <= 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

typedef struct {
    money amount_cents;
    int rate;
} SavingsAccount;


typedef struct
{
    money available_funds_cents;
    money salary;
    money food_spendings;
    money life_spendings;
    money random_spendings;
    money flat_rent;
    money flat_price;
    CreditAccount* credit;
    SavingsAccount* savings;
} Person;

void add_salary(Person* person) {
    person->available_funds_cents += person->salary;
}

void pay_spendings(Person* person, int is_sick) {
    if (is_sick)
    {
        person->available_funds_cents -= person->flat_rent;
    }
    else
    {
        person->available_funds_cents -= (person->food_spendings + person->life_spendings + person->random_spendings + person->flat_rent);
    }
}

void pay_credit(Person* person) {
    CreditAccount* credit = person->credit;

    if (is_credit_done(credit)) {
        person->available_funds_cents += (-credit->debt_cents);
        credit->debt_cents = 0;
        return;
    }

    credit->debt_cents -= person->available_funds_cents;
    person->available_funds_cents = 0;

    credit->debt_cents *= 1 + (credit->rate / 12.0);
}

void handle_savings(Person* person) {
    SavingsAccount* savings = person->savings;

    savings->amount_cents *= 1 + (savings->rate / 12.0);

    savings->amount_cents += person->available_funds_cents;
    person->available_funds_cents = 0;
}

void handle_inflation_for_year(Person* person) {
    person->food_spendings *= 1 + inflation;
    person->life_spendings *= 1 + inflation;
    person->random_spendings *= 1 + inflation;
    person->flat_rent *= 1 + inflation;
    person->flat_price *= 1 + inflation;
    person->salary *= 1 + inflation;
}


int main(void) {
    CreditAccount credit = {
            .debt_cents = flat_price - start_capital,
            .rate = credit_percents
    };
    Person Alice = {
            .available_funds_cents = 0,
            .credit = &credit,
            .flat_price = 0,
            .flat_rent = 0,
            .food_spendings = food_spendings,
            .life_spendings = life_spendings,
            .random_spendings = random_spendings,
            .salary = salary
    };

    SavingsAccount savings = {
            .amount_cents = start_capital,
            .rate = savings_percents
    };
    Person Bob = {
            .available_funds_cents = 0,
            .savings = &savings,
            .flat_price = flat_price,
            .flat_rent = flat_rent,
            .food_spendings = food_spendings,
            .life_spendings = life_spendings,
            .random_spendings = random_spendings,
            .salary = salary
    };

    int current_year = 2024;
    int current_month = 1;
    for (int i = 0; i < simulation_time_years; i++) {
        for (int k = 0; k < 12; k++) {
            if (current_year == 2027 && current_month <= 3)
            {
                pay_spendings(&Bob, 1);
            }
            else if (current_year == 2027 && current_month == 4)
            {
                Bob.salary /= 1.5;
            }
            else
            {
                add_salary(&Bob);
                pay_spendings(&Bob, 0);
            }

            add_salary(&Alice);
            pay_spendings(&Alice, 0);
            pay_credit(&Alice);


                handle_savings(&Bob);
            current_month++;
        }
        handle_inflation_for_year(&Bob);
        handle_inflation_for_year(&Alice);
        current_month = 1;
        current_year++;
    }

    Bob.savings->amount_cents -= Bob.flat_price;

    printf("Bob - %lld\nAlice - %lld\nflat price - %lld\n", Bob.savings->amount_cents, Alice.available_funds_cents, Bob.flat_price);
    return 0;
}
