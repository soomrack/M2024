#include <stdio.h>
#include <math.h>

// Liubinetskii V. (@liubvlad)
// code style for C
// https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide/
// https://github.com/MaJerle/c-code-style
//
// variable naming rules for C

#pragma region Black box for calculating credit monthly payment

double сredit_сalculation_recursion(double base, int exponent) {
    if (exponent == 0)
        return 1;
    else
        return base * сredit_сalculation_recursion(base, exponent - 1);
}

double get_credit_monthly_payment(double credit_amount, double annual_credit_rate, int duration_months) {
    double monthly_credit_rate = annual_credit_rate / 12;

    double all_credit_time_inflation = сredit_сalculation_recursion((1 + monthly_credit_rate), duration_months);

    double coefficient = (monthly_credit_rate * all_credit_time_inflation) / (all_credit_time_inflation - 1);

    double monthlyPayment = credit_amount * coefficient;
    return monthlyPayment;
}

#pragma endregion

// Temporary shared variables 

const int MONTHS_IN_YEAR = 12;
const long long int INITIAL_MONEY = 2000000;


double credit_amount = 0;
double credit_rate = 0.18;
double alice_credit_payed = 0;

double bob_account_amount = 0;
double account_rate = 0.16;

double indexation_rate = 0.12;

double expenses_renovation_year = 100000;
double expenses_food_cost = 15000;
double alice_flat_cost = 20 * 1000 * 1000;
double bob_flat_rent = 25 * 1000;


double alice_salary = 300000;
double bob_salary = 200000;


void time_to_annular_percents() {
    printf("--------timeToPercents--------\n");

    double credit_charges = (credit_amount - alice_credit_payed) * credit_rate;
    if (credit_charges > 0) {
        credit_amount += credit_charges;
    }

    bob_account_amount *= 1 + account_rate;
}


// Годовая индексация
double indexation(double amount) {
    amount += amount * indexation_rate;
    return amount;
}

void amount_indexation() {
    expenses_renovation_year = indexation(expenses_renovation_year);
    expenses_food_cost = indexation(expenses_food_cost);
    alice_flat_cost = indexation(alice_flat_cost);
    bob_flat_rent = indexation(bob_flat_rent);

    alice_salary = indexation(alice_salary);
    bob_salary = indexation(bob_salary);
}

void print_result()
{
    printf("Alice's capital: \n%.2lf rubs.\n", ((credit_amount - alice_credit_payed) + alice_flat_cost));
    printf("Bob's capital: \n%.2lf rubs.\n", bob_account_amount);
}


int main() {

    // Alice & her mortgage
    double mortgage_amount = 20000000.0;
    double initial_payment = INITIAL_MONEY;
    credit_amount = mortgage_amount - initial_payment;
    
    //double aliceImpact = 300000;

    int duration_years = 30;
    int duration_months = duration_years * MONTHS_IN_YEAR;

    double mothly_payment = 0;

    mothly_payment = getCreditMonthlyPayment(credit_amount, credit_rate, duration_months);
	printf("Alice should pay every month = %.2f\n", mothly_payment);
    

    // Bob & his savings account 
    bob_account_amount = INITIAL_MONEY;


    printf("Financial salary:\t Alice:%.2lf\t\t Bob:%.2lf\n", alice_salary, bob_salary);


    
    for (int current_year = 0; current_year < duration_years; current_year++) {
        for (int month_in_year = 1; month_in_year <= MONTHS_IN_YEAR; month_in_year++)
        {
            int month = current_year * MONTHS_IN_YEAR + month_in_year;

            alice_credit_payed += mothly_payment;
            bob_account_amount += (bob_salary - expenses_food_cost - bob_flat_rent);

            printf("Month='%d'\t alice_payed: %.2lf/%.2lf\t bob_account_amount: %.2lf\n", month, alice_credit_payed, credit_amount, bob_account_amount);

        }

        // TEMP
        if (current_year == 29) break;

        time_to_annular_percents();
        amount_indexation();


    }

    print_result();

	return 0;
}



/*
Увеличить начальный капитал
года текущие
отсутпы 
змея_кэйс

в майн только важное

Кредит работает как обязательство на n-лет

*/