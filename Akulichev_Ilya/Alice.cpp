#include <stdio.h>

//tupedef int RUB;
using RUB = unsigned long long int;
using USD = unsigned int;

struct Car { // Налог, ремонт, топливо, обслуживание
    RUB repair;
    RUB fuel;
    RUB service;
};

struct Tax{
    RUB car_tax;
    RUB home_tax;
    RUB land_tax;
};

struct Family {  // Еда, собака, одежда
    RUB food;
    RUB dog;
    RUB clothes;
};

struct medicine { // Лекарства, зубы, болезни
    RUB remedy;
    RUB teeth;
    RUB disease;
};

struct Home{ // Коммунальные услуги, ремонт, ипотека, аренда
    RUB repair;
    RUB mortgage;
    RUB rent;
    RUB rental;
};

struct Bank{ // Доход от банка, проценты по кредиту
    RUB account_RUB;
    USD account_USD;
    RUB check;
    RUB credit;
};

struct Person { // Зарплата, наличные, налоги на машины, расходы на семью, расходы на медицину, расходы на дом
    RUB cash;
    RUB salary;
    RUB rental_income;
};

struct Person alice;

void alice_salary(const int year, const int month)
{
    if (year == 2027 and month == 1) // Promotion
    {
        alice.salary = 100'000;
    }
    
    alice.cash += alice.salary;
}


void simulation()
{
    int year = 2026;
    int month = 9;

    while ( not ( year == 2036 and month == 9 ) )
    {
        alice_salary(year, month);
        // alice_mortgadge();
        // alice_rent();
        // alice_home_bills();
        // alice_food();
        // alice_dog();
        // alice_bank_income();

        ++month;
        if (month == 13)
        {
            ++year;
            month = 1;
        }
    }
};

void alice_init()
{
    alice.cash = 20'000;
    alice.salary = 80'000;
};

void alice_print()
{
    printf("Alice salary = %llu\n", alice.salary);
    printf("Alice cash = %llu\n", alice.cash);
}

int main()
{
    alice_init();

    simulation();

    alice_print();
}