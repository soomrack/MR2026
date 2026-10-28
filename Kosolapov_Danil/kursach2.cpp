////////////// 28.10.26 с пк
// исправлена ипотека, добавлена ее выплата


#include <stdio.h>
#include <iostream>
#include <cstdlib>
using namespace std;


using RUB = unsigned long long int;


struct Person {
    RUB cash;
    RUB salary;
};

struct Person alice;


void alice_init()
{
    alice.cash = 900'000;
    alice.salary = 80'000;
}


void alice_salary(const int year, const int month)
{
    if (year == 2026 and month == 12) {  // promotion
        alice.salary = 120'000;
    }

    alice.cash += alice.salary;
}

void alice_inflation() {

}

void alice_rent(const short& housing) {
    if (housing == 0) {
        int rent_cost = 35'000;
        alice.cash -= rent_cost;
    }

}


void alice_mortgage(short& housing, int& down_payment, int& apartment_price, int& mortgage_debt, int& mortgage_term, int& mortgage_number_of_payments, int& mortgage_payment) {

    if (housing == 0) {
        if (alice.cash >= down_payment) {
            alice.cash -= down_payment;
            printf(" Alice took out a mortgage!!!\n");
            housing = 1;
        }
    }
    if (housing == 1) {
        alice.cash -= mortgage_payment;
        mortgage_debt -= mortgage_payment;
        mortgage_number_of_payments -= 1;
        if (mortgage_debt <= 0) {
            alice.cash += abs(mortgage_debt);
            mortgage_debt = 0;
            mortgage_payment = 0;
            printf(" Alice paid off the mortgage!!!\n");
            housing = 2;
        }
    }
}

void alice_early_mortgage_repayment(short& housing, int& mortgage_debt, int& mortgage_payment) {
    if (housing == 1) {
        if (alice.cash >= mortgage_payment * 2) {

        }
    }
}
   


void car() {

}

void alice_cat() {

}

void alice_tax() {

}

void alice_bank_income() {

}



void alice_print(short& housing)
{
    printf("Alice cash = %llu", alice.cash);
    if (housing == 1) {
        printf("  Mortgage\n");
    }
    if (housing == 2) {
        printf("  Got own apartment\n");
    }
    if (housing == 0) {
        printf("  Rent\n");
    }
}



void simulation()
{
    int year = 2026;
    int month = 9;
    short housing = 0;// 0 - rent,  1 - mortgage, 2 - already got
    int down_payment = 1'000'000;
    int apartment_price = 1'200'000;
    int mortgage_debt = apartment_price - down_payment;
    int mortgage_term = 1;  // years of mortgage.  MB сделать ввод??
    int mortgage_number_of_payments = mortgage_term * 12;
    int mortgage_payment = mortgage_debt / (mortgage_number_of_payments);
    while (not (year == 2028 and month == 9)) {

        alice_salary(year, month);
        printf("Month/Year - %u", month);
        printf("/%u", year);
        printf("  ");
        alice_print(housing);
        alice_rent(housing);
        alice_mortgage(housing, down_payment, apartment_price, mortgage_debt, mortgage_term, mortgage_number_of_payments, mortgage_payment);
        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

int main() {
    alice_init();
    simulation();

}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"  Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

 

// Задание 01. "Alice and Bob"
//Нужно написать программу моделирующую жизнь человека(одного или двух).
//Ключевой аспект моделирования : финансовое состояние, т.е.деньги.
//
//Моделирование должно учитывать :
//+доходы
//1. зарплата(может быть две работы)
//2. проценты по накоплениям
//3. возврат налогов
//+ расходы
//1. еда
//2. коммуналка
//3. ипотека
//4. аренда
//5. домашнее животное(на время его жизни)
//6. машина
//+ инфляцию(индивидуальная для еды, коммуналки и т.д.)
//
//Время симуляции : полная выплата ипотеки(т.е.от 10 лет)
//Шаг симуляции : не больше месяца
//
//Технические требования :
//+объем программы(пустые строки учитываются)
//1. 2000 строк если есть принят за месяц(возможно с последующим исправлением недочетов)
//2. 3000 строк если сдаете в последний месяц семестра
//+ содержание
//1. не более 30 % кода занимают структуры данных
//2. одну тему промоделировать детально, варинты:
//-открытие ПВЗ
//- кредитование(несколько кредитов)
//- здоровое питание
//- и т.д.
//
//Качество кода :
//1. глядя на маленький кусок программы должно быть понятно, что учтено в моделировании
//2. код читаемый и понятный(одногруппник может легко добавить что - то в моделирование)
//3. придерживаться стиля оформления(snake - style)
//4. добавление новых деталей в моделировани не должно противоречить архитектуре программы
//
//
//Моделирование должно соответствовать реальному миру(разумеется, не абсолютно точно).
//
//
//Программа должна позволять делать выводы(хотя бы один из) :
//    1. хорошая модель жизни у Алисы или плохая
//    2. что лучше : взять ипотеку или копить на покупку
//    3. A / B тестирование двух моделей жизни(со случайными событиями)
//
//
//    PS :
//    1. использовать код сгенерированный AI настоятельно не рекоммендуется
//    2. можно использовать любой функционал С или C++ (из стандартной библиотеки)
//    3. можно написать на чистом С

