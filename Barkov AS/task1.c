#include <stdio.h>
#include <math.h>

typedef long long int Money; // копейка

struct Person {
    Money capital;
    Money deposit;
    Money salary;
    Money food;
    Money utilities; // комунальные услуги
};

Money APARTMENT = 20 * 1000 * 1000 * 100; // стоимость квартиры
const Money PAY = 14730680; // платеж по ипотеке 
const Money PAY_LAST = 14202190; // последний платеж по ипотеке
const double PERCENT_1 = 0.09; // процент по вкладу
const double PERCENT_2 = 0.11; // увеличенная ставка по вкладу
const double INFLATION = 0.07; // инфляция
const Money RENT = 30 * 1000 * 100;  // плата за квартиру
const Money VACATION = 30 * 1000 * 100;


void profit_deposit(struct Person* human, double percent_1, double percent_2)
{
    double percent;

    if (human->deposit >= 5 * 1000 * 1000 * 100) //увеличение ставки по вкладу за отстаток на счету 5 млн
        percent = percent_2;
    else
        percent = percent_1;

    human->deposit = human->deposit + human->deposit * percent / 12.0;
    human->deposit = human->deposit + human->salary;
}


void price_apartment(Money* apartment, double inflation)
{
    *apartment = *apartment + *apartment * inflation / 12.0;
}


void spend_food(struct Person* human)
{
    human->deposit = human->deposit - human->food;
}


void spend_utilities(struct Person* human)
{
    human->deposit = human->deposit - human->utilities;
}


void spend_rent(struct Person* human, Money rent)
{
    human->deposit = human->deposit - rent;
}

void spend_vacation(struct Person* human, Money vacation, int* month, double inflation)
{
    if (*month == 8)
        human->deposit = human->deposit - vacation;
        vacation = vacation + vacation * inflation; 
}


void spend_mortgage(struct Person* human, Money pay, Money pay_last, int* month, int* year)
{
    if (*month == 8 && *year == 2043)
        human->deposit = human->deposit - pay_last;
    else
        human->deposit = human->deposit - pay;
}


void spend_doctor(struct Person* human, int* year, int* starting_point)
{
    if (*year - *starting_point == 10) {
        human->deposit = human->deposit - 500 * 1000 * 100;
        *starting_point = *year;
    }
}


void init_bob(struct Person* human)
{
    human->capital = 0;
    human->deposit = 0;
    human->salary = 200 * 1000 * 100;
    human->food = 15 * 1000 * 100;
    human->utilities = 5 * 1000 * 100;
}


void init_alice(struct Person* human)
{
    human->capital = 0;
    human->deposit = 1 * 1000 * 1000 * 100;
    human->salary = 200 * 1000 * 100;
    human->food = 15 * 1000 * 100;
    human->utilities = 5 * 1000 * 100;
}


void simulation(struct Person* bob, struct Person* alice)
{
    int month = 2;
    int year = 2024;
    int starting_point_alice = year; // начало отсчета для обращения к зубному 
    int starting_point_bob = year;

    while (!(month == 2 && year == 2044)) {

        if (month == 13) {
            month = 1;
            year++;
        }

        profit_deposit(bob, PERCENT_1, PERCENT_2);
        spend_mortgage(bob, PAY, PAY_LAST, &month, &year);
        price_apartment(&APARTMENT, INFLATION);
        spend_food(bob);
        spend_utilities(bob);
        spend_doctor(bob, &year, &starting_point_bob);
        spend_vacation(bob, VACATION, &month,INFLATION);

        profit_deposit(alice, PERCENT_1, PERCENT_2);
        spend_rent(alice, RENT);
        spend_food(alice);
        spend_utilities(alice);
        spend_doctor(alice, &year, &starting_point_alice);

        month++;
    }

    bob->capital = bob->deposit + APARTMENT;
    alice->capital = alice->deposit;
}


void printf_result(Money alice_capital, Money bob_capital)
{
    if (alice_capital > bob_capital)
        printf("Kredit ne vygoden\n");
    else
        printf("Kredit vygoden\n");

    printf("Alice=%lld\nBob=%lld\n", alice_capital, bob_capital);
}


int main()
{
    struct Person alice, bob;

    init_alice(&alice);
    init_bob(&bob);
    simulation(&bob, &alice);
    printf_result(alice.capital, bob.capital);

    return 0;
}
