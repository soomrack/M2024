#include <iostream>
#include <math.h>
#include <conio.h>
#include <cmath>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    float S = 2000000, H = 20000000, P, Summ = 0, Itog, pay, Prem;
    int k = 30;
    float percentIP = 18, percentVK = 16, percentINF = 12;
    float salary = 150000;
    float food = 15000;
    float arenda = 25000;
    float rem = 100000, n = 2;
    int i; 

    //Боб
    //Каким станет стартовый капитал через 30 лет с учётом процентов на вкладе
    for (i = 1; i <= k; i++) {
        S = S * (1 + percentVK / 100);
    }
    //Сумма, которую боб кладёт на вклад каждый месяц без учёта инфляции
    P = salary - food - arenda;
    //Сумма, которую боб кладёт на вклад каждый месяц с учётом инфляции
    for (i = 1; i <= k * 12; i++){
        Summ += P;
        P = P * (1 + percentINF / (100*12));
        //Сумма, накопленная бобом за год с учётом процентов на вкладе
        if (i % 12 == 0) {
            Summ *= (1 + percentVK/100);
        }
    }
    //Сумма денег у Боба через 30 лет
    Itog = S + Summ;
    cout << "Боб накопил: " << Itog << endl;
    //Цена квартиры через 30 лет
    for (i = 1; i <= k; i++) {
        H = H * (1 + percentINF / 100);
    }
    cout << "Цена квартиры: " << H << endl;

    //Алиса
    H = 20000000;
    S = 2000000;
    H = H - S;
    P = salary - food;  
    Prem = salary - food - rem;
    //Размер ежемесячного платежа по ипотеке;
    pay = H * (percentIP / (100 * 12)) / (1 - powf((1 + (percentIP / (100 * 12))), -k*12-1));
    cout << "Ежемесячный платёж по ипотеке: " << pay << endl;
    //Сумма, которую Алиса может платить за ипотеку
    for (i = 1; i <= k * 12; i++) {
        if (i <= n * 12) {
            //1. Во время ремонта
            Prem *= (1 + percentINF / (100 * 12));
            //Сумма долга после ремонта
            H += (pay - Prem);
        }
        else {
            //2. Без ремонта
            P *= (1 + percentINF / (100 * 12));
            //Сумма долга без ремонта
            H += (pay - P);
        if (H <= 0) {
                cout << "Через " << i << " месяцев ипотека будет выплачена";
                return 0;
            }
        }  
    }
    return 0;
}


