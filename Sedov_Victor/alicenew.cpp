#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

using RUB = long long int;  // %lld

constexpr int months_in_simulation = 15 * 12;

struct person
{
    RUB cash;   //налоги на заработок + деньги на банк ложаться
    RUB salary;
    // Bank vtb;
    // Bank tbank;
    //constexpr RUB food_base_cost = 15'000;
//constexpr RUB utilities_base_cost = 6'000;
//constexpr RUB transport_base_cost = 4'000;
//constexpr RUB subscriptions_base_cost = 1'500; 
};

struct person alice;


void alice_salary(int month)
{
    alice.cash += alice.salary;
}


void spend(RUB amount)
{
    alice.cash = (alice.cash -= amount) ;
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
    study,
    entertainment,
};

constexpr double base_monthly_inflation = 0.006; // общая инфляция, ~7.2% в год

struct category_inflation  // CategoryInflation
{
    double individual_rate;       // своя скорость категории, доля в месяц
    double current_multiplier = 1.0;
};

category_inflation inflation_table[] =
{
    { 0.004 },  // food
    { 0.003 },  // utilities
    { 0.002 },  // transport
    { -0.001 }, // subscriptions -- дешевеют из-за конкуренции сервисов
    { 0.005 },  // housing
    { 0.001 },  // study
    { 0.003 },  // entertainment
};

void alice_inflation(const int month) // если в функции не используется (month), то следует ее сделать const,  чтобы удобнее было читать
{
    for (auto& entry : inflation_table)
    {
        double noise = (roll_d100() - 50) / 10000.0; // небольшой случайный шум, +-0.5%
        entry.current_multiplier *= (1.0 + base_monthly_inflation + entry.individual_rate + noise);
    }
}

// Текущая цена с учётом накопленной инфляции категории.
RUB get_price(category c, RUB base_amount)   //c замеинть
{
    double multiplier = inflation_table[static_cast<int>(c)].current_multiplier;
    return static_cast<RUB>(base_amount * multiplier);
}

//  Обязательные расходы 
constexpr RUB food_base_cost = 15'000;
constexpr RUB utilities_base_cost = 6'000;
constexpr RUB transport_base_cost = 4'000;
constexpr RUB subscriptions_base_cost = 1'500; // VPN + прочие подписки

void alice_food(int month)
{
    spend(get_price(category::food, food_base_cost));
}

void alice_utilities(int month)
{
    spend(get_price(category::utilities, utilities_base_cost));
}

void alice_transport(int month)
{
    spend(get_price(category::transport, transport_base_cost));
}

void alice_subscriptions(int month)
{
    spend(get_price(category::subscriptions, subscriptions_base_cost));
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
        alice.cash -= study_cost; // что такое учеба - это повышение квалификации или учеба в универе или ...
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
//казик газ делать, система мотиваций все норм, назвыать функции более явно, чтобы понятно было что к чему и что делает

