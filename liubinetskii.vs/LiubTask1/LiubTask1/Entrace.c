#include <stdio.h>
#include <math.h>

// Liubinetskii V. (@liubvlad)
// code style for C
// https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide/
// https://github.com/MaJerle/c-code-style
//
// variable naming rules for C

#pragma region Black box for calculating credit monthly payment

double creditCalculationRecursion(double base, int exponent) {
    if (exponent == 0)
        return 1;
    else
        return base * creditCalculationRecursion(base, exponent - 1);
}

double getCreditMonthlyPayment(double creditAmount, double annualCreditRate, int durationMonths) {
    double monthlyCreditRate = annualCreditRate / 12;

    double allCreditTimeInflation = creditCalculationRecursion((1 + monthlyCreditRate), durationMonths);

    double coefficient = (monthlyCreditRate * allCreditTimeInflation) / (allCreditTimeInflation - 1);

    double monthlyPayment = creditAmount * coefficient;
    return monthlyPayment;
}

#pragma endregion

// Temporary shared variables 
double creditAmount = 0;
double creditRate = 0.18;
double aliceCreditPayed = 0;

double bobAccountAmount = 0;
double accountRate = 0.12;


void timeToAnnularPercents() {
    // TODO negative collapse
    creditAmount += (creditAmount - aliceCreditPayed) * creditRate;

    bobAccountAmount *= 1 + accountRate;
}

int main() {
    const double INITIAL_MONEY = 2000000.0;

    // Alice & her mortgage
    double mortgageAmount = 20000000.0;
    double initialPayment = INITIAL_MONEY;
    creditAmount = mortgageAmount - initialPayment;
    
    //double aliceImpact = 300000;

    int durationYears = 30;
    int durationMonths = durationYears * 12;

    double mothlyPayment = 0;

    mothlyPayment = getCreditMonthlyPayment(creditAmount, creditRate, durationMonths);
	printf("Alice should pay every month = %.2f\n", mothlyPayment);
    

    // Bob & his savings account 
    bobAccountAmount = INITIAL_MONEY;
    double bobImpact = 200000;


    printf("Financial impact:\t Alice:%.2lf\t\t Bob:%.2lf\n", mothlyPayment, bobImpact);

    // every month
    int monthsGoing = 20 * 12 + 1; // temp for debug
    for (int month = 1; month <= monthsGoing; month++) {
        
        aliceCreditPayed += mothlyPayment;
        bobAccountAmount += bobImpact;

        printf("Month='%d'\t alicePayed: %.2lf/%.2lf\t bobAccountAmount: %.2lf\n", month, aliceCreditPayed, creditAmount, bobAccountAmount);

        if (month % 12 == 0) {
            timeToAnnularPercents();
            printf("--------timeToPercents--------\n");
        }
    }

	return 0;
}
