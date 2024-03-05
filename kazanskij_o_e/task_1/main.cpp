#include <iostream>
#include <cstdlib>
#include <math.h>

const uint8_t COUNT_YEARS = 30;     // Кол-во симулируемых лет
const uint8_t CREDIT_PERIOD = 30;   // период кредитования
const float INFLATION = 12; // Инфляция 
const float BANK_PROCENT = 16; // Процентная ставка банка по вкладу
const float CREDIT_PROCENT = 18; // Процентная ставка банка по кредиту
//Все денежные средства будут указаны в копейках
const uint64_t START_BANK = 2*1000*1000*100; // Стартовый капитал
const uint64_t MONTH_SALARY = 500*1000*10; // Ежемесячная зарплата
int current_year = 2024;    // текущий год в симуляции
uint8_t current_month = 2;  // текущий месяц в симуляции
uint64_t flat_cost = 20*1000*1000*100; // Стоимость квартиры
uint64_t home_pay = 15*1000*10; // Бытовые расходы
uint64_t home_repain = 150*1000*10; // стоймость ремонта 

struct Person{
    int64_t count_money = 0; // капитал человека
    uint64_t salary = 0; // зарплата человека
    uint64_t credit_payment = 0; // платёж кредита (если есть)
    int have_home = 0; // наличие дома
    uint64_t rent = 0; // аренда квартиры
    uint64_t vklad = 0; // счёт по вкладу
};

Person alice, bob; // создание испытуемых

/*
Вычисление ежемесячной оплаты кредита
@param credit сумма кредита
@param procent процентная ставка
@param period срок оплаты кредита
@param vznos=0 первоначальный взнос
*/
uint64_t credit_add(uint64_t credit, float procent, uint8_t period, uint64_t vznos = 0){
    float month_procent = procent / 12 / 100; // вычисление месячной процентной ставки
    float general_rate = pow(1 + month_procent, period * 12); // вычисление общей ставки
    return (credit - vznos) * month_procent * general_rate / (general_rate - 1);
}

//<info> инициализация испытуемых

void alice_init(){
    alice.salary = MONTH_SALARY;
    alice.credit_payment = credit_add(flat_cost, CREDIT_PROCENT, CREDIT_PERIOD, START_BANK);
    alice.have_home = 1;
}

void bob_init(){
    bob.salary = MONTH_SALARY;
    bob.rent = 25 * 1000 * 100;
    bob.vklad = START_BANK;
}
//</info>

//зачисление зарплаты за месяц (bob)
void bob_salary_payment(){
    bob.count_money += bob.salary;
}

//зачисление зарплаты за месяц (alice)
void alice_salary_payment(){
    alice.count_money += alice.salary;
}

//ежегодное инфляция
void start_inflation(){
    float koef = 1 + (INFLATION / 100); // Коэффицент изменения в десятичном виде
    bob.salary *= koef;
    alice.salary *= koef;
    home_pay *= koef;
    flat_cost *= koef;
    home_repain *= koef;
    if(alice.have_home == 0){alice.rent *= koef;}
    if(bob.have_home == 0){bob.rent *= koef;}
}

// оплата бытовых расходов (bob)
void bob_home_payment(){
    bob.count_money -= home_pay;
}

// оплата бытовых расходов (alice)
void alice_home_payment(){
    alice.count_money -= home_pay;
}

// ежемесячное выплата кредита
void alice_credit_payment(int end_year){
    // Вычисление пройденных месяцев выплат без учёта текущего месяца
    int current_count_month = ((end_year - current_year) * 12) + current_month - 1;
    // Проверка выплаченных месяцев
    if(current_count_month >= CREDIT_PERIOD * 12){return;} 
    alice.count_money -= alice.credit_payment;
}

// Оплата ремонта дома
void home_repain_payment(){
    static bool is_repain = false; 
    // проверка на то, что прошло два года с момента последнего ремонта
    if(!is_repain){is_repain = !is_repain; return;} 
    //проверка испытуемых на наличие дома
    if(alice.have_home == 1){alice.count_money -= home_repain;} 
    if(bob.have_home == 1){bob.count_money -= home_repain;}
    is_repain = !is_repain; // говорим, что прошло один год с момента последнего ремонта
}

//оплата аренды квартиры (bob)
void bob_rent_payment(){
    if(bob.have_home == 0){bob.count_money -= bob.rent;}
}

/*
Расчёт вклада по истечению месяца
@param sum сумма депозита
@param procent годовой процентная ставка по вкладу
@return сумму вклада после изменения
*/
uint64_t up_vklad(uint64_t sum, float procent){
    procent /= 100; // перевод процентов в десятичный вид 
    return sum * (1 + procent / 12); 
}

void bob_up_vklad(){
    bob.vklad = up_vklad(bob.vklad, BANK_PROCENT);
}

void bob_pay_home(){
    if(bob.vklad >= flat_cost){
        bob.vklad -= flat_cost;
        bob.have_home = 1;
        bob.rent = 0;
    };
}

// вывод общего капитала
void rezult(){
    printf("капитал:\n");
    printf("\tAlice: %d\n", alice.count_money);
    printf("\tBob: %d\n", bob.count_money);
}

// Симуляция жизни испытуемых
void simulation(){
    int end_year = current_year + COUNT_YEARS;  //Последний год симуляции
    uint8_t end_month = current_month; // последний месяц в симуляции
    while(current_year != end_year || current_month != end_month){
        alice_salary_payment();
        alice_home_payment();
        alice_credit_payment(end_year);

        bob_salary_payment();
        bob_home_payment();
        bob_rent_payment();
        bob_up_vklad();
        bob_pay_home();

        current_month++;
        if(current_month == 12){
            ++current_year;
            home_repain_payment();
            start_inflation();
        }
    }
    bob.count_money += bob.vklad;
    bob.vklad = 0;
}

int main(){
    system("chcp 65001");
    alice_init();
    bob_init();
    simulation();
    rezult();
    return 0;
}
