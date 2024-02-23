#include <stdio.h>
#include <math.h>

// Структура для хранения параметров квартиры и финансовых данных
struct apartment_and_finance {
    int apartment_price; // Стоимость квартиры
    int initial_capital; // Начальный капитал
    int mortgage_rate; // Процентная ставка по ипотеке
    int mortgage_years; // Срок ипотеки в годах
    int renovation_years; // Годы на ремонт
    int renovation_cost_per_year; // Расходы на ремонт в год
    int savings_interest_rate; // Процентная ставка по сбережениям
    int inflation_rate; // Инфляция
    int monthly_salary; // Ежемесячная зарплата
    int monthly_expenses; // Расходы на еду в месяц
    int vacation_expenses; // Расходы на отпуск в год
};

// Функция для инициализации структуры apartment_and_finance
struct apartment_and_finance initialize_apartment_and_finance() {
    struct apartment_and_finance data;
    data.apartment_price = 20000000;
    data.initial_capital = 2000000;
    data.mortgage_rate = 16;
    data.mortgage_years = 30;
    data.renovation_years = 2;
    data.renovation_cost_per_year = 1000000;
    data.savings_interest_rate = 12;
    data.inflation_rate = 11;
    data.monthly_salary = 360000;
    data.monthly_expenses = 40000;
    data.vacation_expenses = 200000;
    return data;
}

// Функция для расчета ежемесячного платежа по ипотеке
double calculate_monthly_mortgage_payment(struct apartment_and_finance data) {
    double mortgage_rate_monthly = (double)data.mortgage_rate / (12 * 100);
    int total_months = data.mortgage_years * 12;
    double monthly_mortgage_payment = data.apartment_price * mortgage_rate_monthly / (1 - pow(1 + mortgage_rate_monthly, -total_months));
    return monthly_mortgage_payment;
}

// Функция для моделирования финансовых операций на протяжении срока ипотеки
void simulate_financial_operations(struct apartment_and_finance data, double monthly_mortgage_payment, double* alice_final_capital, double* bob_final_capital) {
    int alice_mortgage = data.apartment_price - data.initial_capital;
    double total_alice_payments = monthly_mortgage_payment * data.mortgage_years * 12;
    double total_alice_earnings = 0;
    int bob_savings = data.initial_capital;
    double total_bob_savings_with_interest = bob_savings;
    double total_bob_payments = 0;
    double total_bob_earnings = 0;

    for (int year = 1; year <= data.mortgage_years; ++year) {
        // Обновление стоимости квартиры с учетом инфляции
        data.apartment_price *= (1 + data.inflation_rate / 100.0);

        // Обновление ставки по ипотеке с учетом инфляции
        data.mortgage_rate *= (1 + data.inflation_rate / 100.0);

        // Расходы на ремонт для первых двух лет
        int renovation_cost = (year <= data.renovation_years) ? data.renovation_cost_per_year : 0;

        // Расходы на еду для каждого месяца с учетом инфляции
        int monthly_expenses_adjusted = data.monthly_expenses * 12 * year * (1 + data.inflation_rate / 100.0);

        // Увеличение зарплаты каждый год с учетом инфляции
        data.monthly_salary *= (1 + data.inflation_rate / 100.0);

        // Обновление финансовых данных Боба
        int bob_income = data.monthly_salary * 12;
        total_bob_earnings += bob_income;
        bob_savings += bob_income - monthly_expenses_adjusted - renovation_cost - data.vacation_expenses;
        total_bob_savings_with_interest *= (1 + data.savings_interest_rate / 100.0);
        total_bob_savings_with_interest += bob_savings;
        total_bob_payments += monthly_mortgage_payment;

        // Обновление финансовых данных Элис
        total_alice_earnings += data.monthly_salary * 12;
    }

    // Вычисление суммы денег, которую Alice внесла по ипотеке
    double alice_mortgage_contribution = total_alice_payments - alice_mortgage;

    // Вычисление капитала Alice с учетом стоимости квартиры в итоге
    *alice_final_capital = total_alice_earnings - total_alice_payments + data.apartment_price;
    // Вычисление капитала Bob с учетом стоимости квартиры в итоге
    *bob_final_capital = total_bob_savings_with_interest - total_bob_payments + data.apartment_price;
}

// Функция для вывода результатов
void print_results(double alice_final_capital, double bob_final_capital) {
    printf("Alice's capital including the value of the apartment in the end: %.2f rubles\n", alice_final_capital);
    printf("Bob's capital including the value of the apartment in the end: %.2f rubles\n", bob_final_capital);

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
    struct apartment_and_finance data = initialize_apartment_and_finance();
    double monthly_mortgage_payment = calculate_monthly_mortgage_payment(data);
    double alice_final_capital, bob_final_capital;
    simulate_financial_operations(data, monthly_mortgage_payment, &alice_final_capital, &bob_final_capital);
    print_results(alice_final_capital, bob_final_capital);
    return 0;
}
