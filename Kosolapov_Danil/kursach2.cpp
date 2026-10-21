////////////// 21.10.26 с пк
// добавить проверку покупки уже квартиры (если mortgage_debt = 0)
// & - ссылка на 

#include <stdio.h>
#include <iostream>
using namespace std;


using RUB = unsigned long long int;


struct Person {
    RUB cash;
    RUB salary;
};

struct Person alice;


void alice_init()
{
    alice.cash = 400'000;
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

void alice_rent(const bool& housing) {
    if (housing == 0) {
        int rent_cost = 35'000;
        alice.cash -= rent_cost;
    }

}


void alice_mortgage(bool& housing) {
    int down_payment = 1'000'000;
    int apartment_price = 7'000'000;
    int mortgage_debt = apartment_price - down_payment;
    int mortgage_term = 10;  // years of mortgage.  MB сделать ввод??
    int mortgage_payment = mortgage_debt / (mortgage_term * 12);
    if (housing == 0) {
        if (alice.cash >= down_payment) {
            alice.cash -= down_payment;
            printf(" Alice took out a mortgage!!!\n");
            housing = 1;
        }
    } 
    else {
            alice.cash -= mortgage_payment;
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



void alice_print(bool& housing)
{
    printf("Alice cash = %llu", alice.cash);
    if (housing == 1) {
        printf("  Mortgage\n");
    }
    else {
        printf("  Rent\n");
    }
}



void simulation()
{
    int year = 2026;
    int month = 9;
    bool housing = 0; // 0 - rent,  1 - mortgage
    while (not (year == 2027 and month == 9)) {

        alice_salary(year, month);
        printf("Month/Year - %u", month);
        printf("/%u", year);
        printf("  ");
        alice_print(housing);
        alice_rent(housing);
        alice_mortgage(housing);
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

