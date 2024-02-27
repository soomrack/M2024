#include <stdio.h>
#include <stdbool.h>

// Все денежные переменные указываются в сумма/месяц, кроме CREDIT
// Alice
long FLAT_PRICE = 20 * 1000 * 1000;
long CREDIT = 18 * 1000 * 1000;
double CREDIT_RATE = 0.11;
long FLAT_RENOVATION = 100 * 1000;
// Bob
long FLAT_RENT = 25 * 1000;
double DEPOSIT_RATE = 0.16;
// Common Expenses
long WASTES = 30 * 1000;
const double INFLATION = 0.04;
const int YEARS = 30;
int MONTHS = 30 * 12;


typedef struct{
    char* name;
    long salary;
    long expenses;
    int flat_renovation_years;
    bool hasCredit;
} Person;

Person alice = {"Alice", 150 * 1000, 30 * 1000, 2, true};

Person bob = { "Bob", 150 * 1000, 30 * 1000 + 20 * 1000, 0, false};

// Считает степень
double recursive(double base, int exponent){
    if (exponent == 0)
        return 1;
    else
        return base * recursive(base, exponent - 1);
}

// Ежемесячная плата по кредиту при аннуитентном плетеже
void annuity_monthly_payment(long *monthly_payment){
    double monthly_credit_rate = CREDIT_RATE / 12;
    double all_credit_time_inflation = \
    recursive((1 + monthly_credit_rate), YEARS * 12);
    // Коэффициент К из экономической формулы 
    double K = (monthly_credit_rate * all_credit_time_inflation)/\
    (all_credit_time_inflation - 1);

    *monthly_payment = (long)(CREDIT * K);
    printf("%s в месяц будет платить по кредиту: %ld у.е.\n", alice.name, *monthly_payment);
}

// Функция считает сумму всех значений, повышенных на инфляцию
long percentage_calculation(long  value, long time_unit){
    value = value * 12;
    long sum = 0;
    for (long i=0; i<time_unit; i++){
        value = value * (1 + INFLATION);
        sum = sum + value;
    }
    return sum;
}

void how_much_person_will_pay_for_YEARS(Person person, long value, long time_unit, long *monthly_payment){
    // В зависимости от того, есть ли плата по кредиту, меняется вывод общих трат за YEARS
    if (person.hasCredit == true){
        printf("%s потратил(-а) на жизнь за %d лет: %ld у.е.\n", 
            person.name,
            time_unit,
            // Считается расходы + ремонт + оплата по кредиту 
            (percentage_calculation(value, time_unit) + \
            percentage_calculation(FLAT_RENOVATION, person.flat_renovation_years) + \
            (*monthly_payment * 12 * YEARS)));
    }
    else{
        printf("%s потратил(-а) на жизнь за %d лет: %ld у.е.\n", 
            person.name,
            time_unit,
            // Считается расходы + ремонт + оплата по кредиту 
            (percentage_calculation(value, time_unit) + \
            percentage_calculation(FLAT_RENOVATION, person.flat_renovation_years)));
    }
}

void how_much_flat_will_cost(){
    // стоимость * на 30летнию инфляцию
    long flat_will_cost = FLAT_PRICE * (recursive((1 + INFLATION), YEARS));
    printf("Через %d лет квартира будет стоит: %ld\n", YEARS, flat_will_cost);
}

long entire_period_deposit_balance(Person person){
    long margin = (person.salary - person.expenses);
    long balance = margin * 12 * recursive((1 + DEPOSIT_RATE),YEARS);
    printf("On deposit for %d years: %ld\n", YEARS, balance);

    return balance;
}

long main() {
    long monthly_payment;
    annuity_monthly_payment(&monthly_payment);
    how_much_person_will_pay_for_YEARS(alice, alice.expenses, YEARS, &monthly_payment);
    how_much_person_will_pay_for_YEARS(bob, bob.expenses, YEARS, &monthly_payment);
    how_much_flat_will_cost();
    entire_period_deposit_balance(bob);
    // double alice_payment_for_credit_for_YEARS = annuity_monthly_payment()* MONTHS;
    // double alice_wastes_for_YEARS = \
    // how_much_person_will_pay_for_YEARS(WASTES, YEARS) + how_much_person_will_pay_for_YEARS(FLAT_RENOVATION, FLAT_RENOVATION_YEARS);
    // double alice_all_payments_for_YEARS = annuity_monthly_payment()* MONTHS \
    // + how_much_person_will_pay_for_YEARS(WASTES, YEARS) + how_much_person_will_pay_for_YEARS(FLAT_RENOVATION, FLAT_RENOVATION_YEARS);
// 
    // double bob_wastes_for_YEARS = how_much_person_will_pay_for_YEARS((WASTES + FLAT_RENT), YEARS);
    // double flat_price_after_YEARS = FLAT_PRICE * recursive((1 + INFLATION), YEARS);
// 
    // printf("---------ALICE---------\n");
    // printf("Alice payment for credit for %d years: %.0f\n", YEARS, alice_payment_for_credit_for_YEARS);
    // printf("Alice wastes for %d years without credit: %.0f\n", YEARS, alice_wastes_for_YEARS);
    // printf("Alice payment for credit and wastes for 30 %d years: %.0f\n", YEARS, alice_all_payments_for_YEARS);
// 
    // printf("---------BOB---------\n");
    // printf("Bob has spent money for %d years: %.0f\n", YEARS, bob_wastes_for_YEARS);
    // printf("After %d years flat will cost: %.0f\n", YEARS, flat_price_after_YEARS);
    // double bob_deposit_balance = entire_period_deposit_balance();
// 
    // printf("---------FINALLY---------\n");
    // if (alice_all_payments_for_YEARS != bob_wastes_for_YEARS){
        // if (alice_all_payments_for_YEARS < bob_wastes_for_YEARS)
            // printf("Alice spent less money.\n");
        // else
            // printf("Bob spent less money.\n");
    // }
    // else
        // printf("Strangely, they spent the same amount\n");
// 
    // if (bob_deposit_balance < flat_price_after_YEARS)
        // printf("Unfortunately Bob didn't save enough money up for an apartment:(\n");
    // else
        // printf("Bob can buy a flat!\n");

    return 0;    
}