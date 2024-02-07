#include <iostream>
#include <stdlib.h>

using namespace std;


int inflation = 12;
int duration = 30;

float indexation(float x)
{
    x = x * inflation / 100 + x;
    return x;
}

class credit
{
private:
    int loan_interest = 18;
    int flat_cost = 20000000;
    int capital = 2000000;
    float salary = 150000;
    float food_cost = 15000;
    float renovation_per_year = 100000;
    int renovation_duration = 2;
public:
    double cred = double(flat_cost);
    float unexpected_waste = 0;
    void index_all()
    {
        salary = indexation(salary);
        food_cost = indexation(food_cost);

    }
}Alise;

class deposit
{
private:
    int deposit_interest = 16;
    float rent = 25000;
    float capital = 2000000;
    float salary = 150000;
    float food_cost = 15000;
public:
    double depos = double(capital);
    float unexpected_waste = 0;
    void index_all()
    {
        salary = indexation(salary);
        food_cost = indexation(food_cost);
        rent = indexation(rent);
    }
    void deposit_add_per_month()
    {
        double balance = salary - food_cost - rent - unexpected_waste;
        depos += depos * deposit_interest / 12 / 100;
        depos += balance;
    }
    void result()
    {
        cout.precision(2);
        cout << "Капитал Боба спустя 30 лет составил:  " << '\n';
        cout << unsigned long(Bob.depos) << " рублей  " << (Bob.depos - unsigned long(Bob.depos)) * 100 << " копеек" << '\n';
    }
}Bob;

int main()
{
    setlocale(LC_ALL, "rus");

    for (short int year = 0; year < duration; year += 1)
    {
        for (short int month = 0; month < 12; month += 1)
        {
            Bob.deposit_add_per_month();
        }
        Bob.index_all();
    }

    Bob.result();
    return 0;
}
