//Expenses
extern int food;
extern int flat_renovation;
extern int flat_rent;
extern int months;
extern float inflation;

float all_time_expenses(float full_expenses, int how_many_years){
    int i = 0;
    while (i < how_many_years){
        full_expenses = full_expenses * (1 + inflation);
        // printf("full_expenses - %d: %.2f\n", i, full_expenses);
        i++;
    }
    return full_expenses;
}