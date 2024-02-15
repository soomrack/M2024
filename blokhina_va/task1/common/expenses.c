//Expenses
extern float wastes;
extern float flat_renovation;
extern int flat_rent;
extern int months;
extern float inflation;

extern float salary;


// Функция считает сумму расходов за 30 лет. Для Боба дополнительно функция считает также ежемесячный остаток на счёте.
float inflation_calculation(float value, int time_unit, short is_it_Bob){
    float sum = 0;
    float balance = 0;
    for (int i=0; i<time_unit; i++){
        value = value * (1 + inflation);
        if (is_it_Bob = 1){
            balance = balance + deposit_balance(value);
            // Тут нужно повышать зп
        }
        sum = sum + value;
    }
    return sum;
}

float deposit_balance(float wastes){
    float monthly_balance = salary - wastes;
    return monthly_balance;
}