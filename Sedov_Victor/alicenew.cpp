#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

using RUB = long long int;  // %lld

constexpr int months_in_simulation = 15 * 12;


struct person
{
    RUB cash;
    RUB salary;
// Bank vtb;    // Bank tbank; //налоги на заработок + деньги на банк ложаться
    static constexpr RUB foodBaseCost = 15'000;
    static constexpr RUB utilitiesBaseCost = 6'000;
    static constexpr RUB transportBaseCost = 4'000;
    static constexpr RUB subscriptionsBaseCost = 1'500; // VPN + прочие подписки
    static constexpr RUB profDevelopmentCost = 8'000;   // цена одного месяца обучения/повышения квалификации
};

struct person alice;


void alice_salary(const int month) // 
{
    alice.cash += alice.salary;
}

void spend(RUB amount)
{
    alice.cash = (alice.cash >= amount) ? (alice.cash - amount) : 0;
}

int roll_d100()
{
    int limit = RAND_MAX - (RAND_MAX % 100);
    int r;
    do {
        r = rand();
    } while (r >= limit);

    return r % 100 + 1;
}


enum class category
{
    food,
    utilities,
    transport,
    subscriptions,
    housing,
    profDevelopment,   // было "study" -- переименовано понятнее: развитие в профессии
    entertainment,
};

constexpr double base_monthly_inflation = 0.006; // общая инфляция, ~7.2% в год

// categoryInflation -- в стиле camelCase
struct categoryInflation
{
    double individual_rate;       // своя скорость категории, доля в месяц
    double current_multiplier = 1.0;
};

categoryInflation inflation_table[] =
{
    { 0.004 },  // food
    { 0.003 },  // utilities
    { 0.002 },  // transport
    { -0.001 }, // subscriptions -- дешевеют из-за конкуренции сервисов
    { 0.005 },  // housing
    { 0.001 },  // profDevelopment
    { 0.003 },  // entertainment
};

void alice_inflation(const int month) // month не используется -- параметр const
{
    for (auto& entry : inflation_table)
    {
        double noise = (roll_d100() - 50) / 10000.0; // небольшой случайный шум, +-0.5%
        entry.current_multiplier *= (1.0 + base_monthly_inflation + entry.individual_rate + noise);
    }
}

// Текущая цена с учётом накопленной инфляции категории.
RUB get_price(const category itemCategory, const RUB base_amount) // c заменено на itemCategory -- понятнее, что за значение
{
    double multiplier = inflation_table[static_cast<int>(itemCategory)].current_multiplier;
    return static_cast<RUB>(base_amount * multiplier);
}

//  Обязательные расходы 

void alice_food(const int month) // month не используется -- параметр const
{
    spend(get_price(category::food, person::foodBaseCost));
}

void alice_utilities(const int month) // month не используется -- параметр const
{
    spend(get_price(category::utilities, person::utilitiesBaseCost));
}

void alice_transport(const int month) // month не используется -- параметр const
{
    spend(get_price(category::transport, person::transportBaseCost));
}

void alice_subscriptions(const int month) // month не используется -- параметр const
{
    spend(get_price(category::subscriptions, person::subscriptionsBaseCost));
}

constexpr int exp_per_month_from_work = 2;
constexpr int exp_per_month_from_prof_development = 5; // было exp_per_month_from_study
constexpr int exp_to_level_up = 100;
constexpr int max_career_level = 5;

// careerState -- в стиле camelCase, как и просили для структур
struct careerState
{
    int exp = 0;
    int level = 1;
};

struct careerState alice_career_state;

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

void alice_career(const int month) // month не используется -- параметр const
{
    int gained_exp = exp_per_month_from_work;

    if (alice_career_state.level < max_career_level and alice.cash >= person::profDevelopmentCost)
    {
        alice.cash -= person::profDevelopmentCost;
        gained_exp += exp_per_month_from_prof_development;
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

    //alice_entertainment_state.threshold_months = 2 + (roll_d100() % 2);
}

void simulation()
{
    printf("Начало симуляции\n");

    for (int month = 0; month < months_in_simulation; ++month)
    {
        alice_inflation(month);
        alice_career(month);
        alice_salary(month);
        alice_food(month);
        alice_utilities(month);
        alice_transport(month);
        alice_subscriptions(month);

        // alice_entertainment(month);

        // alice_casino(month);
        // alice_mortgage(month);
        // alice_rent(month);
    }
}

void alice_print()
{
    printf("Итог симуляции\n");
    printf("Финальный баланс: %lld руб.\n", alice.cash);
}

int main()
{
    srand((unsigned)time(NULL));

    alice_init();
    simulation();
    alice_print();

    return 0;
}
//казик газ делать, система мотиваций все норм, назвыать функции более явно, чтобы понятно было что к чему и что делает