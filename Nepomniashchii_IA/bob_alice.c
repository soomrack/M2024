#include <stdio.h>
#include <locale.h>

typedef long long int money;

const int simulation_time = 30;

const money start_capital = 2 * 1000 * 1000 * 100;
money flat_price = 20 * 1000 * 1000 * 100;
money salary = 350 * 1000 * 100;

money flat_rent = 80 * 1000 * 100;
const double savings_percents = 0.16;

const double credit_percents = 0.175;

money food_spendings = 20 * 1000 * 100;
money life_spendings = 15 * 1000 * 100;
money random_spendings = 8 * 1000 * 100;
const double inflation = 0.12;

int main(void) {
    money AliceMoney = 0;
    money AliceDebt = flat_price - start_capital;
    money BobMoney = 0;
    money BobBank = start_capital;

    for (int i = 0; i < simulation_time; i++) {
        for (int k = 0; k < 12; k++) {
            AliceMoney += salary;
            AliceMoney -= food_spendings + life_spendings + random_spendings;
            if (AliceDebt > 0) {
                AliceDebt *= 1 + (credit_percents / 12.);
                AliceDebt -= AliceMoney;
                AliceMoney = 0;
            }
            else {
                AliceMoney -= AliceDebt;
                AliceDebt = 0;
                AliceMoney *= 1 + (savings_percents / 12.);
            }

            BobMoney += salary;
            BobMoney -= food_spendings + life_spendings + random_spendings + flat_rent;
            BobBank *= 1 + (savings_percents / 12.);
            BobBank += BobMoney;
            BobMoney = 0;
        }

        food_spendings += food_spendings * inflation;
        life_spendings += life_spendings * inflation;
        random_spendings += random_spendings * inflation;
        flat_rent += flat_rent * inflation;
        salary += salary * inflation;
        flat_price += flat_price * inflation;
    }

    BobBank -= flat_price;

    printf("Bob - %lld\nAlice - %lld\nflat price - %lld\n", BobBank, AliceMoney, flat_price);
    return 0;
}
