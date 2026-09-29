#include <cstdio>
#include <cstdlib>
#include <ctime>

using RUB = unsigned long long int;

constexpr int months_in_simulation = 15 * 12;

struct person
{
    RUB cash;
    RUB salary;
};

struct person alice;

int roll_d100()
{
    int limit = RAND_MAX - (RAND_MAX % 100);
    int r;
    do
    {
        r = rand();
    } while (r >= limit);

    return r % 100 + 1;
}

void alice_salary(int month)
{
    alice.cash += alice.salary;
}

constexpr int exp_per_month_from_work = 2;
constexpr int exp_per_month_from_study = 5;
constexpr RUB study_cost = 8'000;

constexpr int exp_to_level_up = 100;
constexpr int max_career_level = 5;

struct career_state
{
    int exp = 0;
    int level = 1;
};

struct career_state alice_career_state;

RUB salary_for_level(int level)
{
    switch (level)
    {
        case 1: return 60'000;
        case 2: return 80'000;
        case 3: return 100'000;
        case 4: return 130'000;
        case 5: return 170'000;
        default: return 170'000;
    }
}

void alice_career(int month)
{
    int gained_exp = exp_per_month_from_work;

    if (alice_career_state.level < max_career_level and alice.cash >= study_cost)
    {
        alice.cash -= study_cost;
        gained_exp += exp_per_month_from_study;
    }

    alice_career_state.exp += gained_exp;

    if (alice_career_state.exp >= exp_to_level_up and alice_career_state.level < max_career_level)
    {
        alice_career_state.exp -= exp_to_level_up;
        alice_career_state.level += 1;
        alice.salary = salary_for_level(alice_career_state.level);
    }
}

void alice_init()
{
    alice.cash = 20'000;
    alice.salary = salary_for_level(alice_career_state.level);
}

void simulation()
{
    printf("Начало симуляции\n");

    for (int month = 0; month < months_in_simulation; ++month)
    {
        alice_career(month);
        alice_salary(month);

        // alice_casino(month);
        // alice_mortgage(month);
        // alice_rent(month);
        // alice_food(month);
        // alice_utilities(month);
        // alice_inflation(month);
    }
}

void alice_print()
{
    printf("Итог симуляции\n");
    printf("Финальный баланс: %llu руб.\n", alice.cash);
}

int main()
{
    srand((unsigned)time(NULL));

    alice_init();
    simulation();
    alice_print();

    return 0;
}
