#include <stdio.h>
#include <math.h>

struct apartment_data {
    int price; // Стоимость квартиры
    int renovation_years; // Годы на ремонт
    int renovation_cost_per_year; // Расходы на ремонт в год
    int mortgage_years; // Срок ипотеки в годах
};

struct finance_data {
    int initial_capital; // Начальный капитал 
    int mortgage_rate; // Процентная ставка по ипотеке
    int savings_interest_rate; // Процентная ставка по сбережениям
    int inflation_rate; // Инфляция 
    int monthly_salary; // Ежемесячная зарплата 
    int monthly_expenses; // Расходы на еду в месяц 
    int vacation_expenses; // Расходы на отпуск в год 
};

struct apartment_data initialize_apartment_data() {
    struct apartment_data data;
    data.price = 20000000; 
    data.renovation_years = 2;
    data.renovation_cost_per_year = 1000000; 
    data.mortgage_years = 30;
    return data;
}

struct finance_data initialize_finance_data() {
    struct finance_data data;
    data.initial_capital = 2000000; 
    data.mortgage_rate = 16;
    data.savings_interest_rate = 12; 
    data.inflation_rate = 11; 
    data.monthly_salary = 360000;
    data.monthly_expenses = 40000; 
    data.vacation_expenses = 200000;
    return data;
}

// Функция для расчета ежемесячного платежа по ипотеке
int calculate_monthly_mortgage_payment(struct apartment_data data, int mortgage_rate) {
    double mortgage_rate_monthly = (double)mortgage_rate / (12 * 100);
    int total_months = data.mortgage_years * 12;
    int monthly_mortgage_payment = (int)(data.price * mortgage_rate_monthly /
        (1 - pow(1 + mortgage_rate_monthly, -total_months)));
    return monthly_mortgage_payment;
}

// Функция для моделирования финансовых операций на протяжении срока ипотеки
void simulate_financial_operations(struct apartment_data apartment_data, struct finance_data finance_data, int monthly_mortgage_payment, int* alice_final_capital, int* bob_final_capital) {
    int alice_mortgage = apartment_data.price - finance_data.initial_capital;
    int total_alice_payments = monthly_mortgage_payment * apartment_data.mortgage_years * 12;
    int total_alice_earnings = 0;
    int bob_savings = finance_data.initial_capital;
    int total_bob_savings_with_interest = bob_savings;
    int total_bob_payments = 0;
    int total_bob_earnings = 0;

    // Вычисление расходов и доходов за весь срок ипотеки
    for (int year = 1; year <= apartment_data.mortgage_years; ++year) {
        apartment_data.price *= (10000 + finance_data.inflation_rate) / 10000;
        finance_data.mortgage_rate *= (10000 + finance_data.inflation_rate) / 10000.0;
        int renovation_cost = (year <= apartment_data.renovation_years) ? apartment_data.renovation_cost_per_year : 0;
        int monthly_expenses_adjusted = finance_data.monthly_expenses * 12 * year *
            (10000 + finance_data.inflation_rate) / 10000;
        finance_data.monthly_salary *= (10000 + finance_data.inflation_rate) / 10000.0;
        int bob_income = finance_data.monthly_salary * 12;
        total_bob_earnings += bob_income;
        bob_savings += bob_income - monthly_expenses_adjusted - renovation_cost - finance_data.vacation_expenses;
        total_bob_savings_with_interest *= (10000 + finance_data.savings_interest_rate) / 10000.0;
        total_bob_savings_with_interest += bob_savings;
        total_bob_payments += monthly_mortgage_payment;
        total_alice_earnings += finance_data.monthly_salary * 12;
    }
    // Вычисление суммы денег, которую Alice внесла по ипотеке
    int alice_mortgage_contribution = total_alice_payments - alice_mortgage;

    // Вычисление капитала Alice с учетом стоимости квартиры в итоге
    *alice_final_capital = total_alice_earnings - total_alice_payments + apartment_data.price;

    // Вычисление капитала Bob с учетом стоимости квартиры в итоге
    *bob_final_capital = total_bob_savings_with_interest - total_bob_payments + apartment_data.price;
}

// Функция для вывода результатов
void print_results(int alice_final_capital, int bob_final_capital) {
    printf("Alice's capital including the value of the apartment in the end: %d rubles\n", alice_final_capital);
    printf("Bob's capital including the value of the apartment in the end: %d rubles\n", bob_final_capital);

    // Определение победителя
    if (alice_final_capital > bob_final_capital) {
        printf("Alice remains in greater advantage\n");
    }
    else if (alice_final_capital < bob_final_capital) {
        printf("Bob remains in greater advantage\n");
    }
    else {
        printf("Both Alice and Bob remain in equal advantage\n");
    }
}

int main() {
    struct apartment_data apartment_data = initialize_apartment_data();
    struct finance_data finance_data = initialize_finance_data();
    int monthly_mortgage_payment = calculate_monthly_mortgage_payment(apartment_data, finance_data.mortgage_rate);
    int alice_final_capital, bob_final_capital;
    simulate_financial_operations(apartment_data, finance_data, monthly_mortgage_payment, &alice_final_capital, &bob_final_capital);
    print_results(alice_final_capital, bob_final_capital);
    return 0;
}
