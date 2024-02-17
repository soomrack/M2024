#include "../alice/alice_monthly_payment.h"
#include "../../Unity/src/unity.h"
#include "../variables.h"

void test_recursive(void) {
    TEST_ASSERT_EQUAL_FLOAT(8.0f, recursive(2.0f, 3));
    TEST_ASSERT_EQUAL_FLOAT(64.0f, recursive(8.0f, 2));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, recursive(10.0f, 0));
}

void test_K_ratio(void){
    TEST_ASSERT_EQUAL_FLOAT(0, K_ratio());
}

void test_alice_monthly_payment(void){
    TEST_ASSERT_EQUAL_FLOAT(2*18*1000*1000, alice_monthly_payment());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_recursive);
    RUN_TEST(test_K_ratio);
    RUN_TEST(test_alice_monthly_payment);
    return UNITY_END();
}

// Как запускать:
// скомпилировать файлы, участвующие в тестировании
// запустить скрипт вывода инфомации о тесте 
// gcc test.c mycode.c ../../Unity/src/unity.c -o test && ./test
