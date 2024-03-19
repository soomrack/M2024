#include <stdio.h>
#include <math.h>


typedef long long int Money;  // копейки

int YEAR = 2023;
int MONTH = 9;
int MORTAGE_PERIOD = 240;

double FLAT_TAX = 0.05;
double M_KOEF = 0.07 / 12;  // коэфф расчета платежа

Money MORTAGE = 19 * 1000 * 1000 * 100;  // сумма кредита
Money MORTAGE_PAYMENT;
Money FLAT = 20 * 1000 * 1000 * 100;


struct Person {
    Money balance;  // кол-во денег после расходов на жизнь
    Money bank;
    Money capital;  // конечный результат
    Money salary;
    Money rent;
    Money spending;  // траты на жизнь
    Money deposit_bonus;
    double deposit;  // процент счета в банке
};


struct Person Alice;
struct Person Bob;


void initialization_Alice()
{
    Alice.bank = 1 * 1000 * 1000 * 100;
    Alice.capital = 0;
    Alice.salary = 200 * 1000 * 100;
    Alice.rent = 30 * 1000 * 100;
    Alice.deposit = 0.09;
    Alice.spending = 15 * 1000 * 100;
    Alice.deposit_bonus = 5 * 1000 * 1000 * 100;
}


void initialization_Bob()
{
    Bob.bank = 0;
    Bob.capital = 0;
    Bob.salary = 200 * 1000 * 100;
    Bob.rent = 0;
    Bob.deposit = 0.09;
    Bob.spending = 15 * 1000 * 100;
    Bob.deposit_bonus = 5 * 1000 * 1000 * 100;
}


void mortage_pay(Money MORTAGE, double M_KOEF, int MORTAGE_PERIOD)  // расчет платежа по кредиту
{
    MORTAGE_PAYMENT = MORTAGE * ((M_KOEF * pow(1 + M_KOEF, MORTAGE_PERIOD)) / (pow(1 + M_KOEF, MORTAGE_PERIOD) - 1));
}


void time_count()  // счетчик лет
{
    MONTH += 1;
    if (MONTH == 13) {
        MONTH = 1;
    }

    if (MONTH % 12 == 0) {
        YEAR += 1;
    }
}


void Alice_salary(int condition_year, int condition_month)
{
    if ((YEAR == condition_year) && (MONTH == condition_month)) {  // Алиса теряет зарплату
        Alice.salary = 0;
    }
    else {
        Alice.salary = 200 * 1000 * 100;
    }
    Alice.bank += Alice.salary;
}


void Bob_salary(int condition_year, int condition_month)
{
    if ((YEAR == condition_year) && (MONTH == condition_month)) {  // Боб теряет зарплату
        Bob.salary = 0;
    }
    else {
        Bob.salary = 200 * 1000 * 100;
    }
    Bob.bank += Bob.salary;
}


void deposit_bonus()
{
    if (Alice.bank / Alice.deposit_bonus == 1) {  // увеличение ставки
        Alice.deposit_bonus += 5 * 1000 * 1000 * 100;
        Alice.deposit += 0.005;
    }

    if (Bob.bank / Bob.deposit_bonus == 1) {
        Bob.deposit_bonus += 5 * 1000 * 1000 * 100;
        Bob.deposit += 0.005;
    }
}


void flat_tax()  // налог на квартиру
{
    if (MONTH == 9) {
        Bob.bank -= FLAT * FLAT_TAX;
    }
}


void Alice_spend_money()
{
    if (MONTH == 1) {
        Alice.spending += Alice.spending * 0.03;
    }
    Alice.bank -= Alice.spending - Alice.rent;
}


void Bob_spend_money()
{
    if (MONTH == 1) {
        Bob.spending += Bob.spending * 0.03;
    }
    Bob.bank -= Bob.spending - MORTAGE_PAYMENT;
}


void Alice_bank()
{
    Alice.bank += Alice.bank * (Alice.deposit / 12);
    Alice.bank -= Alice.bank * (0.07 / 12);
}


void Bob_bank()
{
    Bob.bank += Bob.bank * (Bob.deposit / 12);
    Bob.bank -= Bob.bank * (0.07 / 12);
    FLAT += FLAT * (0.01 / 12);
}


void total_counting(int condition_year, int condition_month)
{
    for (int month = 0; month <= MORTAGE_PERIOD; ++month) {
        time_count();
        deposit_bonus();

        Alice_salary(condition_year, condition_month);
        Alice_spend_money();
        Alice_bank();

        Bob_salary(condition_year, condition_month);
        flat_tax();
        Bob_spend_money();
        Bob_bank();
    }
}


void final_Alice()
{
    Alice.capital = Alice.bank / 100;
}


void final_Bob()
{
    Bob.capital = (Bob.bank + FLAT) / 100;
}


void result()
{
    printf("%lld   %lld", Alice.capital, Bob.capital);
}


main()
{
    mortage_pay(MORTAGE, M_KOEF, MORTAGE_PERIOD);

    initialization_Alice();
    initialization_Bob();

    total_counting(2040, 3);

    final_Alice();
    final_Bob();

    result();
}