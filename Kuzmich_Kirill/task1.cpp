#include <iostream>
#include <cstdio>
using namespace std;

using RUB = int;

struct Person
{
    RUB cash;
    RUB salary;

};

Person kirill;

void simulation()
{
    int year = 2026;
    int month = 9;

    while (!(year == 2036 and month == 9))
    {
        kirill.cash += kirill.salary;

        cout << "Year: " << year << ", Month: " << month << endl;

        month++;

        if (month > 12)
        {
            month = 1;
            year++;
        }
    }
}

void kirill_income()
{
    kirill.cash = 300'000;
    kirill.salary = 80'000;
}

void kirill_print()
{
    printf ("Kirill cash = %d\n", kirill.cash);
}

int main()
{
    kirill_income();
    simulation();
    kirill_print();
}