#include <stdio.h>
/*#include <math.h>
#include <stdlib.h>
#include <time.h>*/

//typedef int RUB;
using RUB = unsigned long long int;

struct Person {
    RUB cash;
    RUB salary;
};

struct Person bob;

void bob_salary(const int year, const int month) 
{
    if (year == 2027 && month == 1) {  // and --> &&
        bob.salary = 100'000; // добавить комментарии почему и симулировать ещё несколько случаев
    }
    bob.cash += bob.salary;
}


void simulation() 
{
    int year = 2026;
    int month = 9;

    while ( !  (year == 2027 && month == 9) ) {  // and --> &&   not --> !

        bob_salary(year, month);
        
        /*bob_car();
        bob_mortgadge();
        bob_rent();
        bob_home_bills();
        bob_food();
        bob_dog();
        food_bank_income();
        */


        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }

    }

}


void bob_init() 
{
    bob.cash = 20'000;
    bob.salary = 80'000;
}

void bob_print() 
{
    printf("Bob cash = %d\n", bob.cash);
}


int main () 
{
    bob_init();

    simulation();

    bob_print();

    return 0;
}