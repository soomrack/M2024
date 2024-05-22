#include <stdio.h>
#include <math.h>
#include <string.h>

typedef long long int Money;

const double inflation_pp = 7;

typedef struct Person
{
Money bank_account;
Money salary;
Money flat;
Money mortgage;
Money first_pay;
Money expenditure;
Money flat_expenditure;
short int duration;
double bank_account_pp;
char name[6];
} Person;

Person alice;
Person bob;

void bob_init()
{
bob.bank_account = 0;
bob.salary = 200 * 1000 * 100;
bob.flat = 20 * 1000 * 1000 * 100;
bob.first_pay = 1000 * 1000 * 100;
bob.duration = 30 * 12;
bob.bank_account_pp = 11.0;
bob.expenditure = 50 * 1000 * 100;
strcpy(bob.name, "Bob");
bob.flat_expenditure = 0; //monthly_mortgage_pay_bob(7.0);
};

void alice_init()
{
alice.bank_account = 1000 * 1000 * 100;
alice.salary = 200 * 1000 * 100;
alice.flat = 0;
alice.first_pay = 0;
alice.bank_account_pp = 11.0;
alice.expenditure = 50 * 1000 * 100;
alice.flat_expenditure = 30 * 1000 * 100;
strcpy(alice.name, "Alice");
};

void alice_deposite_income()
{
alice.bank_account +=(Money)(alice.bank_account * alice.bank_account_pp / 100.0 / 12.0);
};

Money alice_salary(const int year, const int month)
{
if (month == 12) alice.salary += (Money)(alice.salary * inflation_pp / 100);
if (month != 3) alice.bank_account += alice.salary;
};

void alice_flat_bills()
{
alice.bank_account -= alice.flat_expenditure;
alice.flat_expenditure += (Money)(alice.flat_expenditure * inflation_pp / 100 / 12);
};

void alice_expenditure() // life expenditure for Alice
{
alice.bank_account -= alice.expenditure;
alice.expenditure += (Money)(alice.expenditure * inflation_pp / 100 / 12);
};

void bob_expenditure() // life expenditure for Bob
{
bob.bank_account -= bob.expenditure;
bob.expenditure += (Money)(bob.expenditure * inflation_pp / 100 / 12);

};

void bob_flat_tax(double tax_pp, int month)
{
if (month == 8) bob.bank_account -= (Money)(bob.flat * tax_pp / 100);
};

void monthly_mortgage_pay_bob(double mortgage_percent)
{
double monthly_pp = mortgage_percent / 12.0;
Money flat_price = bob.flat - bob.first_pay;

bob.flat_expenditure = (double)flat_price *
((monthly_pp / 100.0) * (powf(1.0 + (monthly_pp / 100.0), (double)bob.duration))
/ (powf(1.0 + (monthly_pp / 100.0), (double)bob.duration) - 1.0));
};

Money bob_salary(const int year, const int month)
{
if (!(month >= 4 && month <= 6 && year == 2030)) bob.bank_account += bob.salary;
if (month == 12) bob.salary += (Money)(bob.salary * inflation_pp / 100);
};

void bob_deposite_income()
{
bob.bank_account += (Money)(bob.bank_account * bob.bank_account_pp / 100.0 / 12.0);
};

void bob_flat_cost(int month)
{
if (month == 12) bob.flat += (Money)(bob.flat * (inflation_pp / 100));
};

void bob_flat_bills()
{
bob.bank_account -= bob.flat_expenditure;

};

void simulation(int month, int year)
{
int temp_year = year;
int temp_month = month;
bob.bank_account = 0;
monthly_mortgage_pay_bob(7.0);

while (!(year == (temp_year + 30) && month == temp_month)) {
alice_salary(year, month);
alice_flat_bills();
alice_expenditure();
alice_deposite_income();

bob_salary(year, month);
bob_flat_bills();
bob_expenditure();
bob_flat_tax(0.1, month);
bob_deposite_income();
bob_flat_cost(month);
++month;

if (month == 13) {
month = 1;
++year;
}
}
bob.bank_account += bob.flat;
};

void print_person(const Person person)
{
printf("%s:\n", person.name);
printf(" bank_account = %lld rub\n", (Money)(person.bank_account / 100));
};

int main()
{
alice_init();
bob_init();

simulation(9, 2023);

print_person(bob);
print_person(alice);

return 0;
}
