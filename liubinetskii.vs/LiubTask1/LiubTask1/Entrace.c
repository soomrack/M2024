#include <stdio.h>
#include <stdlib.h>

#include <math.h>

int main() {

    double mothlyPayment = 0;
    double creditSum = 20000000.0;
    double startPayment = 2000000.0;

    int numYears = 30;
    int durationMonths = numYears * 12;

    double percent = 0.18;
    //double monthPercent = percent / 12;
        
    mothlyPayment = (creditSum * percent) / (1 - pow(1 + percent, -durationMonths));

    /*
    Х — ежемесячный платеж по кредиту, который и нужно рассчитать
    S — общий размер ипотеки
    P – месячная процентная ставка(т.е.годовая ставка, которая разделена на 12 мес.)
    M — срок ипотечного кредитования(считается в месяцах)
    */
	printf("%.2f", mothlyPayment);

	return 0;
}
