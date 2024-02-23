#include <stdio.h>
#include <math.h>
int zp =150000,eda=15000, gkx=10000,odezda=10000, kv = 30000,otdyx=50000;
long B=2000000,A=0,kvartira=20000000;
int num_of_years=30;// срок подсчета
float monthly_payment () {
    int annual_interest_rate=5;
    // Преобразуем годовую процентную ставку в месячную
    float monthly_interest_rate = annual_interest_rate / 12 / 100;
    // Вычисляем общее количество платежей
    int total_payments = num_of_years * 12;
    // Вычисляем ежемесячный платеж используя формулу аннуитетного платежа
   int ipoteka = ((kvartira-2000000) * monthly_interest_rate) / (1 - pow(1 + monthly_interest_rate, -total_payments));
    return ipoteka;
    
}
int main() {
  int ipoteka = monthly_payment();
  int rashod_a = eda+gkx+odezda+ipoteka;
int rashod_b = eda+kv+odezda;
for (int year = 2024 ;  year<2054; year++){//цикл-года
       for (int month = 1; month<13; month++){//цикл-месяцы
        B=B-rashod_b+zp;
        A=A-rashod_a+zp;
        B=B*1.01;
    }
    B=B-otdyx;
    A=A-otdyx;
  kvartira=kvartira*1.1;
 rashod_a=rashod_a*1.12;
  rashod_b=rashod_b*1.12;
 zp=zp*1.12;
}
A=A+kvartira;
printf("Alice=%d\n",A);
printf("Bob=%d\n",B);
printf("KVARTIRA=%d\n",kvartira);

return 0;
}
