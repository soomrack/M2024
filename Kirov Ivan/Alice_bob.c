

#include<stdio.h>
#include<string.h>
#include<math.h>

typedef long long int Money;  // копейки

struct Person {
    Money bank_account;
    int bank_account_pp;
    Money salary;
    Money rent;
    char name[6];
    double inflation_pp;
    Money house_price;
    Money mortgage_first_fee;
    Money expenses;  // траты на еду, коммунальные платежи, одежду
    double mortgage_pp;
    Money mortgage_monthly_payment;
    Money car;
};

struct Person bob;
struct Person alice;


void bob_init()
{
    bob.bank_account = 2000 * 1000 * 100;
    bob.bank_account_pp = 11;
    bob.salary = 200 * 1000 * 100;
    bob.inflation_pp = 7;
    bob.house_price = 0;
    strcpy(bob.name, "Bob");
    bob.rent = 35 * 1000 * 100;
    bob.expenses = 10 * 1000 * 100;
    
}


void alice_init()
{
    alice.bank_account = 2000 * 1000 * 100;
    alice.bank_account_pp = 11;
    alice.salary = 200 * 1000 * 100;
    alice.inflation_pp = 7;
    alice.house_price = 20 * 1000 * 1000 * 100;
    strcpy(alice.name, "Alice");
    alice.mortgage_first_fee = 1000 * 1000 * 100;
    alice.expenses = 10 * 1000 * 100;
    alice.mortgage_pp = 7;
    alice.mortgage_monthly_payment = 0;
}
void bob_car_buy (const int year, const int month)  // покупка машины
{
    if(month==2 && year == 2026){
        bob.bank_account = bob.bank_account - 2000 * 1000 * 100;
    }
}

void bob_car_tires (const int year, const int month)  // траты на шины
{
    if (month == 4 && year >= 2026){
        bob.expenses = bob.expenses + 40 * 1000 * 100;
    }
    if (month == 5 && year >= 2026){
        bob.expenses = bob.expenses - 40 * 1000 * 100;
    }
    if (month == 10  && year >= 2026){
        bob.expenses = bob.expenses + 50 * 1000 * 100;
    }
    if (month == 11 && year >= 2026){
        bob.expenses = bob.expenses - 50 * 1000 * 100;
    }
}


void bob_car_to (const int year, const int month)  // тех осмотр
{
    if (month ==2 && year >= 2026){
        bob.expenses= bob.expenses+15*1000*100;
    }
    if (month ==3 && year >= 2026){
        bob.expenses= bob.expenses-15*1000*100;
    }
}

void bob_car (const int year, const int month)
{
    bob_car_buy(year,month);
    bob_car_to (year,month);
    bob_car_tires(year,month);
}


void bob_salary_income(const int year, const int month)  // доход Боба с учтом инфляции
{
    bob.bank_account += bob.salary;
    if (month == 12) {
        bob.salary = (Money)(bob.salary * (bob.inflation_pp / 100.));
    }
}


void bob_deposite_income(const int year, const int month)  // рост вклада Боба
{
    bob.bank_account += (Money)(bob.bank_account * bob.bank_account_pp / 100.0 / 12.0);
}


void alice_deposite_income(const int year, const int month) // рост вклада Алисы
{
    alice.bank_account += (Money)(alice.bank_account * alice.bank_account_pp / 100.0 / 12.0);
}


void bob_rent(const int year, const int month)  // аренда жилья Боба
{
    bob.bank_account -= bob.rent;
    if (month == 12) {
        bob.rent = (Money)(bob.rent * (bob.inflation_pp / 100.));
    }
}


void bob_expenses(const int year, const int month) // расходы на жизнь Боба
{
    bob.bank_account -= bob.expenses;
    if (month == 12) {
        bob.expenses = (Money)(bob.expenses * (bob.inflation_pp / 100.));
    }
}


void alice_mortgage_monthly_payment(const Money pay , const double pp , const int months) {
    // формула аннуитетного платежа по ипотеке
    Money mortgage_monthly_installment = ((pay) * (pp * 0.01 / 12) * pow((1.0 + (pp * 0.01 / 12)), (months))) /
        (pow((1.0 + (pp * 0.01 / 12)), (months)) - 1.0);

    alice.mortgage_monthly_payment = mortgage_monthly_installment;
    alice.bank_account -= alice.mortgage_first_fee;
}


void alice_expenses(const int year, const int month)  // Расходы на жизнь Алисы
{
    alice.bank_account -= alice.expenses;
    if (month == 12) {
        alice.expenses = (Money)(alice.expenses * (alice.inflation_pp / 100.));
    }
}


void alice_salary_income(const int year, const int month)  // Доходы Алисы с учетом инфляции
{
    alice.bank_account += alice.salary;
    if (month == 12) {
        alice.salary = (Money)(alice.salary * (alice.inflation_pp / 100.));
    }
}


void alice_house_price(const int year, const int month)  // Цена дома Алисы
{
    if (month == 12) {
        alice.house_price += (Money)(alice.house_price * (alice.inflation_pp / 100.));
    }
}


void simulation()
{
    int month = 2;
    int year = 2024;

    alice_mortgage_monthly_payment(alice.house_price - alice.mortgage_first_fee,
        alice.mortgage_pp, 30 * 12);


    while (!(year == 2054 && month == 2)) {
        bob_car(year,month);
        bob_deposite_income(year, month);
        bob_salary_income(year, month);
        bob_rent(year, month);
        bob_expenses(year, month);  // траты на еду, коммунальные платежи, одежду,машину

        alice_deposite_income(year, month);
        alice_salary_income(year, month);
        alice_expenses(year, month);  // траты на еду, коммунальные платежи, одежду
        alice_house_price(year, month);

        ++month;
        if (month == 13) {
            month = 1;
            ++year;
        }
    }
}


void print_person(const struct Person person)
{
    printf("%s:\n", person.name);
    printf(" bank_account = %lld rub, %lld kop\n", (Money)(person.bank_account / 100.0), person.bank_account % 100);

    Money capital = person.bank_account + person.house_price;
    printf(" capital = %lld rub, %lld kop\n", (Money)(capital / 100.0), capital % 100);
    printf("\n");


}
void who_won()
{
    if (alice.bank_account + alice.house_price > bob.bank_account + bob.house_price){
        printf ("alice_won\n");
        printf("\n");
    }
    else{
        printf ("bob_won\n");
        printf("\n");
    }
}


int main()
{
    bob_init();
    alice_init();

    simulation();

    print_person(bob);
    print_person(alice);
    
    who_won();
    return 0;
}

    
    
    
    
    
    
    

