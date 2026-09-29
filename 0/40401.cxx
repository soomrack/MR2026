#include <stdio.h>


using RUB = unsigned long long int;


struct Person {
    RUB cash;
    RUB salary;
};

struct Person alice;


void alice_init()
{
    alice.cash = 20'000;
    alice.salary = 80'000;
}


void alice_salary(const int year, const int month)
{
    if(year == 2026 and month == 12) {  // promotion
        alice.salary = 120'000;
    }

    alice.cash += alice.salary;
}

    
void alice_print()
{
    printf("Alice cash = %llu\n", alice.cash);
}


void simulation()
{
    int year = 2026;
    int month = 9;
    while(not (year == 2027 and month == 9)) {

        alice_salary(year, month);
        // alice_car();
        // alice_mortgage();
        // alice_cat();
        // alice_rent();
        // alice_tax();
        // alice_bank_income();

        // bob_salary();

        ++month;
        if(month == 13) {
            ++year;
            month = 1;
        }
    }
}


int main()
{
    alice_init();
    
    simulation();

    alice_print();
}
