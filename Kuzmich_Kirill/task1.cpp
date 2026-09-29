#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <string>

using namespace std;

using RUB = int;

struct Person // создание персоны
{
    RUB cash;
    RUB salary;
};

Person kirill; // создание персоны Кирилла
Person bob; // создание персоны Боба

namespace config // константы симуляции
{
    constexpr int start_year = 2026;
    constexpr int start_month = 9;

    constexpr int simulation_years = 10;

    constexpr double bank_annual_percent = 0.10;

    constexpr double salary_annual_growth = 0.05;

    constexpr double food_inflation = 0.07;
    constexpr double utility_inflation = 0.06;
    constexpr double rent_inflation = 0.06;
    constexpr double car_inflation = 0.05;
    constexpr double pet_inflation = 0.06;

    constexpr int car_repair_chance = 5;
    constexpr int car_repair_cost = 30'000;

    constexpr int unexpected_chance = 3;
    constexpr int unexpected_cost = 20'000;

    constexpr int kirill_cash_reserve = 300'000;
    constexpr int kirill_salary = 90'000;
    constexpr int bob_cash_reserve = 150'000;
    constexpr int bob_salary = 75'000;

    constexpr int kirill_mortgage = 15'000'000;
    constexpr double kirill_mortgage_percent = 0.085;
    constexpr int kirill_mortgage_years = 20;
}

void simulation() // функция симуляции
{
    int year = config::start_year;
    int month = config::start_month;

    while (!(year == config::start_year + config::simulation_years && month == config::start_month))
    {
        kirill.cash += kirill.salary;
        bob.cash += bob.salary;

        cout << "Year: " << year << ", Month: " << month << endl;

        month++;

        if (month > 12)
        {
            month = 1;
            year++;
        }
    }
}

int main() // главная функция
{
    kirill.cash = config::kirill_cash_reserve;
    kirill.salary = config::kirill_salary;

    bob.cash = config::bob_cash_reserve;
    bob.salary = config::bob_salary;

    cout << "Kirill cash: " << kirill.cash << endl;
    cout << "Kirill salary: " << kirill.salary << endl;

    cout << "Bob cash: " << bob.cash << endl;
    cout << "Bob salary: " << bob.salary << endl;
}