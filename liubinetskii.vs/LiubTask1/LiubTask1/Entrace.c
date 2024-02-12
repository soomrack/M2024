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


int main() {

    double mortgageAmount = 20000000.0;
    double startPayment = 2000000.0;
    double creditAmount = mortgageAmount - startPayment;

    int durationYears = 30;
    int durationMonths = durationYears * 12;
    double creditRate = 0.18;

    double mothlyPayment = 0;

    mothlyPayment = getCreditMonthlyPayment(creditAmount, creditRate, durationMonths);
	printf("%.2f\n", mothlyPayment);

	return 0;
}
