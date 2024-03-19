#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

bool isRandomInitialized = false;

double random_double(double min, double max) {

	if (!isRandomInitialized) { // переменные через _ и капсом
		srand(time(NULL)); // Инициализация генератора случайных чисел
		isRandomInitialized = true; // Установить флаг в true, чтобы не инициализировать srand повторно
	}

	double range = max - min; // Разница между максимумом и минимумом
	double div = RAND_MAX / range; // Для масштабирования rand() к нашему диапазону
	return min + (rand() / div); // Возвращаем значение в нужном диапазоне
}

int main(void)
{
	double flat_cost = 10000000, salary = 150000, started_money = 2000000, renta = 50000, inflation = 0.1,
		loan_rate = 0.12, cost_meal = 15000, cost_car = 12000, cost_TO = 10000, cost_random_min = 5000,
		cost_random_max = 20000, deposit_rate = 0.08; // заменить double на int

	int months = 30 * 12; 

	double P = loan_rate / 12, DP = deposit_rate / 12, IP = inflation / 12; // переменные подробнее расписать

	double capital_Alice = 0, capital_Bob = started_money, spend_morgage = (flat_cost - started_money) * (P + P / (pow((1 + P), months) - 1)); // длина строки максимум 80 (без коммента)

	for (int i = 0; i < months; i++) { // привести к реальному времени - в февраля 2024 взяли кредить и т.п. чтобы можно было добавить: в 2028 году Боб умер

		capital_Alice += salary; // не засорять main, сделать структуры и в мэйне вызывать функции 
		capital_Alice -= spend_morgage;
		capital_Alice -= cost_meal;
		capital_Alice -= cost_car;
		capital_Alice *= (1 + DP);

		capital_Bob += salary; 
		capital_Bob -= renta;
		capital_Bob -= cost_meal;
		capital_Bob -= cost_car;
		capital_Bob *= (1 + DP);

		double cost_random = random_double(cost_random_min, cost_random_max);

		capital_Alice -= cost_random; 
		capital_Bob -= cost_random;

		if (i % 3 == 0) {

			capital_Alice -= cost_TO;
			capital_Bob -= cost_TO;
		}

		flat_cost *= (1 + IP); 

	}

	printf("Capital Alice: \t%.2lf\n", capital_Alice + flat_cost); // прописать функцией отдельной
	printf("Capital Bob:\t%.2lf\n", capital_Bob);

}