#include <stdio.h>

//typedef int RUB;
using RUB = unsigned long long int;


struct Person {
    RUB cash;
    RUB salary;
};

struct Person alice;


void alice_salary(const int year, const int month)
{
    if(year == 2027 and month == 1) {  // Promotion
        alice.salary = 100'000;
    }
        
    alice.cash += alice.salary;
}



void simulation()
{
    int year = 2026;
    int month = 9;
    while (not (year == 2027 and month == 9)) {

        alice_salary(year, month);
        // alice_car();
        // alice_mortgage();
        // alice_rent();
        // alice_home_bills();
        // alice_food();
        // alice_dog();
        // alice_bank_income();

        // bob_salary();        
        
        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }    
}


void alice_init()
{
    alice.cash = 20'000;
    alice.salary = 80'000;
}


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
