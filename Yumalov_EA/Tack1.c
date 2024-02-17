#include <stdio.h>
#include <math.h>
int B=2000000,A=0,zp =150000,eda=15000, gkx=10000,odezda=10000, kv = 30000;
int num_of_years=30;// срок подсчета
int monthly_payment () {
    int kvartira=18000000 , annual_interest_rate=5;
    // Преобразуем годовую процентную ставку в месячную
    int monthly_interest_rate = annual_interest_rate / 12 / 100;
    // Вычисляем общее количество платежей
    int total_payments = num_of_years * 12;
    // Вычисляем ежемесячный платеж используя формулу аннуитетного платежа
    int ipoteka = (kvartira * monthly_interest_rate) / (1 - pow(1 + monthly_interest_rate, -total_payments));
    return ipoteka;
    return kvartira;
}
int main() {
  int ipoteka = monthly_payment();
  int rashod_a = eda+gkx+odezda+ipoteka;
int rashod_b = eda+kv+odezda;
int kvartira = monthly_payment();
for (int year = 2024 ;  year<2055; year++){
       for (int month = 1; month<13; month++){
        B=B-rashod_b+zp;
        A=A-rashod_a+zp;
        B=B*1.01;
    }
  kvartira=kvartira*1.1;
 rashod_a=rashod_a*1.12;
  rashod_b=rashod_b*1.12;
 zp=zp*1.12;
}
A=A+kv;
printf("Alice=%d\n",A);
printf("Bob=%d\n",B);
return 0;
}