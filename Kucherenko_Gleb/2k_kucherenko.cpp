#include <iostream>
#include <stdio.h>
#include <cstdio>

using RUB = int;

int year = 2026;
int month = 9;

struct Person
{
    RUB cash;
    RUB salary;
    RUB deposit;
    int age;
    int exp_age;
};

Person alice;
Person ivan;

void alice_income()
{
    if (year == 2027)
    {
        alice.salary = 95'000;
    }
    if (year == 2028)
    {
        alice.salary = 110'000;
    }
    if (year == 2029)
    {
        alice.salary = 120'000;
    }
    if (year == 2030)
    {
        alice.salary = 140'000;
    }
    if (year == 2031)
    {
        alice.salary = 0;
    }
    if (year == 2032)
    {
        alice.salary = 67'000;
    }
    if (year == 2033)
    {
        alice.salary = 69'000;
    }
    if (year == 2034)
    {
        alice.salary = 99'000;
    }
    if (year == 2035)
    {
        alice.salary = 125'000;
    }
    if (year == 2036)
    {
        alice.salary = 140'000;
    }
}

void ivan_income()
{
    if (year == 2027)
    {
        ivan.salary = 80'000;
    }
    if (year == 2028)
    {
        ivan.salary = 0;
    }
    if (year == 2029)
    {
        ivan.salary = 95'000;
    }
    if (year == 2030)
    {
        ivan.salary = 95'000;
    }
    if (year == 2031)
    {
        ivan.salary = 120'000;
    }
    if (year == 2032)
    {
        ivan.salary = 125'000;
    }
    if (year == 2033)
    {
        ivan.salary = 135'000;
    }
    if (year == 2034)
    {
        ivan.salary = 135'000;
    }
    if (year == 2035)
    {
        ivan.salary = 135'000;
    }
    if (year == 2036)
    {
        ivan.salary = 140'000;
    }
}

void alice_deposit()
{
    double monthly_rate = 0.10/12;
    RUB bankmoney = alice.deposit * monthly_rate;
    alice.deposit += bankmoney;
}

void ivan_deposit()
{
    double monthly_rate = 0.22/12;
    RUB bankmoney = ivan.deposit * monthly_rate;
    ivan.deposit += bankmoney;
}

void alice_print()
{
    printf("alice cash = %d\n", alice.cash);
}

void ivan_print()
{
    printf("ivan cash = %d\n", ivan.cash);
}

void simulation()
{
    while (!((year == 2036) && (month == 9)))
    {
        alice.cash += alice.salary;
        ivan.cash += ivan.salary;

        alice_deposit();
        ivan_deposit();
        
        std::cout << "Year: " << year
                  << ", Month: " << month << std::endl;
        std::cout << "Alice Cash " << alice.cash
                  << ", Alice Salary " << alice.salary << std::endl;
        std::cout << "Alice Bank Cash " << alice.deposit << std::endl;
                  std::cout << "Ivan Cash " << ivan.cash
                  << ", Ivan Salary " << ivan.salary << std::endl;
        std::cout << "Ivan Bank Cash " << ivan.deposit << std::endl;
        
        month++;

        if (month > 12)
        {
            month = 1;
            year++;

            alice_income();
            ivan_income();
            
        }
    }
}

int main()
{
    alice.cash = 300'000;
    alice.salary = 80'000;
    alice.deposit = 1'000'000;

    ivan.cash = 100'000;
    ivan.salary = 75'000;
    ivan.deposit = 300000;

    alice_income();
    ivan_income();

    simulation();

    alice_print();
    ivan_print();

    return 0;
}