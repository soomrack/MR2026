#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

using RUB = long long int;  

constexpr int months_in_simulation = 15 * 12;
constexpr int start_year = 2026;

struct bankAccount
{
    RUB balance = 0;
    double monthly_rate;
};

struct person
{
    RUB salary;
    bankAccount tbank{0, 0.003}; // 0.3% в месяц = 3.6% годовых
    bankAccount vtb{0, 0.005};   // 0.5% в месяц = 6% годовых

    static constexpr RUB foodBaseCost = 15'000;
    static constexpr RUB utilitiesBaseCost = 6'000;
    static constexpr RUB transportBaseCost = 4'000;
    static constexpr RUB subscriptionsBaseCost = 1'500; // VPN + прочие подписки
    static constexpr RUB profDevelopmentCost = 8'000;   // цена одного месяца обучения/повышения квалификации
};

struct person alice;

constexpr RUB tbank_buffer = 100'000; // сколько всегда оставляем на тбанке про запас, остальное уходит в втб

// НДФЛ: работодатель каждый месяц удерживает плоские 13%, а прогрессивная шкала

constexpr int ndfl_withhold_percent = 13;

struct ndflState
{
    RUB income_year = 0;
    RUB withheld_year = 0;
};

struct ndflState alice_ndfl_state;

// Считает налог за год по прогрессивным ступеням.
RUB calculate_annual_tax(RUB annual_income)
{
    RUB tax = 0;

    if (annual_income > 50'000'000)
    {
        tax += (annual_income - 50'000'000) * 22 / 100;
        annual_income = 50'000'000;
    }
    if (annual_income > 20'000'000)
    {
        tax += (annual_income - 20'000'000) * 20 / 100;
        annual_income = 20'000'000;
    }
    if (annual_income > 5'000'000)
    {
        tax += (annual_income - 5'000'000) * 18 / 100;
        annual_income = 5'000'000;
    }
    if (annual_income > 2'400'000)
    {
        tax += (annual_income - 2'400'000) * 15 / 100;
        annual_income = 2'400'000;
    }

    tax += annual_income * 13 / 100;

    return tax;
}

void alice_salary(const int month) // month не используется -- параметр const сделать так, чтобы ндфл рассчитывал каждый работодатель сам
{
    RUB tax = alice.salary * ndfl_withhold_percent / 100;
    RUB net_salary = alice.salary - tax;

    alice_ndfl_state.income_year += alice.salary;
    alice_ndfl_state.withheld_year += tax;

    alice.tbank.balance += net_salary;
}

void alice_ndfl_year_close(const int year, const int month) // year не используется -- параметр const
{
    if (month != 1) // закрываем прошлый год в январе, все выплаты за декабрь уже прошли
    {
        return;
    }

    RUB correct_tax = calculate_annual_tax(alice_ndfl_state.income_year);
    RUB extra = correct_tax - alice_ndfl_state.withheld_year;

    if (extra > 0)
    {
        alice.tbank.balance -= extra;
    }

    alice_ndfl_state.income_year = 0;
    alice_ndfl_state.withheld_year = 0;
}

void alice_bank_interest(const int month) // month не используется -- параметр const
{
    RUB tbank_interest = static_cast<RUB>(alice.tbank.balance * alice.tbank.monthly_rate);
    alice.tbank.balance += tbank_interest;

    RUB vtb_interest = static_cast<RUB>(alice.vtb.balance * alice.vtb.monthly_rate);
    alice.vtb.balance += vtb_interest;
}

void alice_sweep_to_vtb() {
    if (alice.tbank.balance > tbank_buffer)
    {
        RUB free_money = alice.tbank.balance - tbank_buffer;
        alice.tbank.balance -= free_money;
        alice.vtb.balance += free_money;
    }
}

void spend(RUB amount)
{
    alice.tbank.balance -= amount;
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
    profDevelopment,   
    entertainment,
};

constexpr double base_monthly_inflation = 0.006; // общая инфляция, ~7.2% в год


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
    { -0.001 }, // subscriptions 
    { 0.005 },  // housing
    { 0.001 },  // profDevelopment
    { 0.003 },  // entertainment
};

void alice_inflation(const int month) 
{
    for (auto& entry : inflation_table) //разобраться с auto&
    {
        double noise = (roll_d100() - 50) / 10000.0; // небольшой случайный шум, +-0.5%
        entry.current_multiplier *= (1.0 + base_monthly_inflation + entry.individual_rate + noise);
    }
}

// Текущая цена с учётом накопленной инфляции категории.
RUB get_price(const category itemCategory, const RUB base_amount) 
{
    double multiplier = inflation_table[static_cast<int>(itemCategory)].current_multiplier;
    return static_cast<RUB>(base_amount * multiplier);
}

//  Обязательные расходы 

void alice_food(const int month) 
{
    spend(get_price(category::food, person::foodBaseCost));
}

void alice_utilities(const int month) 
{
    spend(get_price(category::utilities, person::utilitiesBaseCost));
}

void alice_transport(const int month) 
{
    spend(get_price(category::transport, person::transportBaseCost));
}

void alice_subscriptions(const int month) {
    spend(get_price(category::subscriptions, person::subscriptionsBaseCost));
}

constexpr int exp_per_month_from_work = 2;
constexpr int exp_per_month_from_prof_development = 5; 
constexpr int exp_to_level_up = 100;
constexpr int max_career_level = 5;


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

void alice_career(const int month) 
{
    int gained_exp = exp_per_month_from_work;

    if (alice_career_state.level < max_career_level and alice.tbank.balance >= person::profDevelopmentCost)
    {
        alice.tbank.balance -= person::profDevelopmentCost;
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

void alice_year_report(const int year, const int month)
{
    if (month != 12) // отчёт раз в год, когда все выплаты за этот год уже прошли
    {
        return;
    }

    printf("%d: доход = %lld, ндфл удержано = %lld, тбанк = %lld, втб = %lld\n",
        year, alice_ndfl_state.income_year, alice_ndfl_state.withheld_year,
        alice.tbank.balance, alice.vtb.balance);
}

void alice_init()
{
    alice.tbank.balance = 20'000;
    alice.salary = salary_for_level(alice_career_state.level);

    //alice_entertainment_state.threshold_months = 2 + (roll_d100() % 2);
}

void simulation()
{
    printf("Начало симуляции\n");

    int year = start_year;
    int month = 1;

    for (int i = 0; i < months_in_simulation; ++i)  //написать симуляцию до месяца года 
    {
        alice_inflation(month);

        alice_ndfl_year_close(year, month);

        alice_salary(month);

        alice_bank_interest(month);

        alice_career(month);

        alice_food(month);
        alice_utilities(month);
        alice_transport(month);
        alice_subscriptions(month);

        alice_sweep_to_vtb();
    
        // alice_entertainment(month);

        // alice_casino(month);
        // alice_mortgage(month);
        // alice_rent(month);

        alice_year_report(year, month);

        ++month;
        if (month == 13)
        {
            ++year;
            month = 1;
        }
    }
}


void alice_print()
{
    printf("Итог симуляции\n");
    printf("Т-Банк: %lld руб.\n", alice.tbank.balance);
    printf("ВТБ: %lld руб.\n", alice.vtb.balance);
    printf("Всего: %lld руб.\n", alice.tbank.balance + alice.vtb.balance);
}

int main()
{
    srand((unsigned)time(NULL));

    alice_init();
    simulation();
    alice_print();

    return 0;
}
//казик газ делать, система мотиваций все норм
