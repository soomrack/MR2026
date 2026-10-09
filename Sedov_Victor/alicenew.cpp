#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

using RUB = long long int;  // %lld

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

    static constexpr RUB checkupBaseCost = 15'000;  // ежегодное обследование
    static constexpr RUB dentistBaseCost = 12'000;  // стоматолог раз в год
    static constexpr RUB illnessBaseCost = 5'000;   // лекарства при простуде
};

struct person alice;

constexpr RUB tbank_buffer = 50'000; // сколько всегда оставляем на тбанке про запас, остальное уходит в втб

// НДФЛ: работодатель каждый месяц удерживает плоские 13%, а прогрессивная шкала
// применяется раз в год, в январе, как доплата по итогам прошлого года.
constexpr int ndfl_withhold_percent = 13;

struct ndflState
{
    RUB income_year = 0;
    RUB withheld_year = 0;
};

struct ndflState alice_ndfl_state;

// Берёт валовый доход, удерживает 13% НДФЛ (как это делает любой налоговый агент: работодатель,
// арендатор и так далее), остальное зачисляет на Т-Банк. Доход и налог копятся для годовой сверки
// по прогрессивной шкале в alice_ndfl_year_close. Возвращает сумму, которая реально пришла на руки.
RUB pay_taxable_income(RUB gross_amount)
{
    RUB tax = gross_amount * ndfl_withhold_percent / 100;
    RUB net_amount = gross_amount - tax;

    alice_ndfl_state.income_year += gross_amount;
    alice_ndfl_state.withheld_year += tax;

    alice.tbank.balance += net_amount;

    return net_amount;
}

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

struct eventState
{
    int sick_months_left = 0;          // сколько ещё месяцев зарплата урезана из-за болезни
    int months_since_big_purchase = 0; // для роста вероятности импульсивной покупки
};

struct eventState alice_event_state;

void alice_salary(const int month) // month не используется -- параметр const
{
    RUB effective_salary = alice.salary;

    if (alice_event_state.sick_months_left > 0) // на больничном платят меньше
    {
        effective_salary = alice.salary / 2;
        alice_event_state.sick_months_left -= 1;
    }

    pay_taxable_income(effective_salary);
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

void alice_sweep_to_vtb(const int month) // month не используется -- параметр const
{
    if (alice.tbank.balance > tbank_buffer)
    {
        RUB free_money = alice.tbank.balance - tbank_buffer;
        alice.tbank.balance -= free_money;
        alice.vtb.balance += free_money;
    }
}

// Снимает с ВТБ сколько получится, вплоть до amount. Возвращает, сколько реально сняла.
RUB withdraw_available_from_vtb(RUB amount)
{
    RUB taken = (alice.vtb.balance >= amount) ? amount : alice.vtb.balance;
    alice.vtb.balance -= taken;
    return taken;
}

bool spend(RUB amount)
{
    // Сначала платим с Т-Банка
    if (alice.tbank.balance >= amount)
    {
        alice.tbank.balance -= amount;
        return true;
    }

    // Сколько не хватает
    RUB missing = amount - alice.tbank.balance;

    // Забираем всё, что осталось на Т-Банке
    alice.tbank.balance = 0;

    // Остаток пытаемся взять из ВТБ, сколько там есть
    missing -= withdraw_available_from_vtb(missing);

    if (missing == 0)
    {
        return true;
    }

    // Денег не хватает нигде -- уходим в минус по Т-Банку
    alice.tbank.balance -= missing;

    printf(
        "ВНИМАНИЕ: Алисе не хватило %lld руб., баланс ушёл в минус.\n",
        missing
    );

    return false;
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
    health,
};

constexpr double base_monthly_inflation = 0.006; // общая инфляция: (1.006)^12 - 1 ≈ 7.44% в год (сложный процент)


struct categoryInflation
{
    double individual_rate;       // своя скорость категории, доля в месяц
    double current_multiplier = 1.0;
};

// Итоговая месячная скорость категории = base_monthly_inflation + individual_rate.
// Например food: 0.006 + 0.004 = 0.01 в месяц -> (1.01)^12 - 1 ≈ 12.68% в год.
categoryInflation inflation_table[] =
{
    { 0.004 },  // food
    { 0.003 },  // utilities
    { 0.002 },  // transport
    { -0.001 }, // subscriptions 
    { 0.005 },  // housing
    { 0.001 },  // profDevelopment
    { 0.003 },  // entertainment
    { 0.004 },  // health
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

//  Здоровье 

void alice_checkup(const int month) // month не используется -- параметр const
{
    if (month != 2) // ежегодное обследование
    {
        return;
    }

    spend(get_price(category::health, person::checkupBaseCost));
}

void alice_dentist(const int month) // month не используется -- параметр const
{
    if (month != 8) // стоматолог раз в год
    {
        return;
    }

    spend(get_price(category::health, person::dentistBaseCost));
}

void alice_illness(const int month) // month не используется -- параметр const
{
    if (month != 3 and month != 11) // простуда весной и осенью
    {
        return;
    }

    spend(get_price(category::health, person::illnessBaseCost));
}

//  Ипотека и недвижимость 

constexpr RUB apartment_price = 10'000'000;
constexpr double down_payment_percent = 20.0;
constexpr RUB down_payment_needed = static_cast<RUB>(apartment_price * down_payment_percent / 100.0);

constexpr int mortgage_term_months = 15 * 12;
constexpr double mortgage_annual_rate = 18.0; // %, фиксированная ставка на весь срок

constexpr double property_tax_rate = 0.1; // % от стоимости квартиры в год

struct mortgageState
{
    bool is_taken = false;
    RUB remaining_debt = 0;
    int months_left = 0;
};

struct mortgageState alice_mortgage_state;

// Как только накоплений хватает на первоначальный взнос -- покупаем квартиру в ипотеку.
void alice_try_buy_apartment(const int month) // month не используется -- параметр const
{
    if (alice_mortgage_state.is_taken) // ипотека уже взята -- повторно покупать не нужно
    {
        return;
    }

    RUB total_savings = alice.tbank.balance + alice.vtb.balance;

    if (total_savings < down_payment_needed)
    {
        return;
    }

    RUB from_vtb = (alice.vtb.balance >= down_payment_needed) ? down_payment_needed : alice.vtb.balance;
    RUB from_tbank = down_payment_needed - from_vtb;

    alice.vtb.balance -= from_vtb;
    alice.tbank.balance -= from_tbank;

    alice_mortgage_state.is_taken = true;
    alice_mortgage_state.remaining_debt = apartment_price - down_payment_needed;
    alice_mortgage_state.months_left = mortgage_term_months;
}

// Ежемесячный платёж: доля долга + проценты на то, что ещё осталось.
void alice_mortgage_payment(const int month) // month не используется -- параметр const
{
    if (not alice_mortgage_state.is_taken or alice_mortgage_state.months_left == 0)
    {
        return;
    }

    RUB principal_part = alice_mortgage_state.remaining_debt / alice_mortgage_state.months_left;
    RUB interest_part = static_cast<RUB>(alice_mortgage_state.remaining_debt * (mortgage_annual_rate / 100.0 / 12.0));

    spend(principal_part + interest_part);

    alice_mortgage_state.remaining_debt -= principal_part;
    alice_mortgage_state.months_left -= 1;
}

// Налог на имущество -- квартира это объект налогообложения, платится раз в год.
void alice_property_tax(const int month) // month не используется -- параметр const
{
    if (not alice_mortgage_state.is_taken)
    {
        return;
    }

    if (month != 12)
    {
        return;
    }

    RUB tax = static_cast<RUB>(apartment_price * (property_tax_rate / 100.0));
    spend(tax);
}

constexpr double rent_monthly_yield_percent = 0.4; // % от стоимости квартиры в месяц -- типичная доходность аренды (~4.8% годовых)

// Алиса сдаёт купленную квартиру целиком и живёт отдельно. Доход облагается НДФЛ как любой другой.
void alice_rent_income(const int month) // month не используется -- параметр const
{
    if (not alice_mortgage_state.is_taken)
    {
        return;
    }

    RUB rent_gross = static_cast<RUB>(apartment_price * rent_monthly_yield_percent / 100.0);
    pay_taxable_income(rent_gross);
}

//  Льготы 

constexpr RUB monthly_benefit_amount = 10'000; // ежемесячное пособие фиксированной суммой, НДФЛ не облагается

void alice_benefits(const int month) // month не используется -- параметр const
{
    alice.tbank.balance += monthly_benefit_amount;
}

//  Случайные события 

constexpr double big_purchase_base_chance = 3.0; // %, сразу после прошлой крупной покупки
constexpr double big_purchase_growth = 1.1;      // вероятность растёт на 10% каждый месяц без покупки

// Чем дольше Алиса не тратилась крупно, тем выше шанс, что сорвётся в этом месяце.
double big_purchase_chance()
{
    double chance = big_purchase_base_chance * std::pow(big_purchase_growth, alice_event_state.months_since_big_purchase);
    return (chance > 100.0) ? 100.0 : chance;
}

void alice_random_events(const int month) // month не используется -- параметр const
{
    int roll = roll_d100();

    if (roll <= 3) // 3% -- премия, разовая выплата
    {
        RUB bonus = alice.salary;
        alice.tbank.balance += bonus;
        printf("Премия: +%lld руб.\n", bonus);
    }
    else if (roll <= 6) // ещё 3% -- заболела, зарплата временно урезана
    {
        alice_event_state.sick_months_left = 2 + (roll_d100() % 2); // 2 или 3 месяца
        printf("Алиса заболела, зарплата временно уменьшена.\n");
    }
    else if (roll <= 8) // ещё 2% -- зарплату проиндексировали навсегда
    {
        alice.salary = static_cast<RUB>(alice.salary * 1.1);
        printf("Зарплату проиндексировали: теперь %lld руб.\n", alice.salary);
    }

    alice_event_state.months_since_big_purchase += 1;

    if (roll_d100() <= big_purchase_chance())
    {
        double percent = 20.0 + (roll_d100() % 31); // от 20% до 50% от текущих сбережений
        RUB total_savings = alice.tbank.balance + alice.vtb.balance;
        RUB purchase_amount = static_cast<RUB>(total_savings * percent / 100.0);

        spend(purchase_amount);
        printf("Алиса не удержалась и потратила %lld руб. на крупную покупку.\n", purchase_amount);

        alice_event_state.months_since_big_purchase = 0;
    }
}

//  Казино 

constexpr double casino_addiction_start = 10.0;
constexpr double casino_addiction_max = 100.0;

constexpr double casino_base_play_probability = 5.0;          // %, если зависимости ещё нет
constexpr double casino_play_probability_per_addiction = 1.0; // прибавка к шансу играть за каждый пункт зависимости

constexpr double casino_base_loss_probability = 55.0;          // %, если зависимости ещё нет
constexpr double casino_loss_probability_per_addiction = 0.3;  // с ростом зависимости играть становится хуже

constexpr double casino_base_budget_percent = 10.0;             // % от месячной зарплаты, если зависимости ещё нет
constexpr double casino_budget_percent_per_addiction = 0.3;     // с ростом зависимости ставки растут мягче

constexpr double casino_big_outcome_threshold = 35.0; // %, после чего исход считается крупным
constexpr double casino_big_win_addiction_gain = 8.0;
constexpr double casino_big_loss_addiction_drop = 10.0;

constexpr double casino_addiction_decay_normal = 0.5;      // за месяц без игры
constexpr double casino_addiction_decay_during_quit = 3.0; // за месяц во время завязки

struct casinoState
{
    double addiction_level = casino_addiction_start;
    int quit_months_left = 0;

    RUB total_won = 0;
    RUB total_lost = 0;
    int times_quit = 0;
};

struct casinoState alice_casino_state;

void clamp_addiction()
{
    if (alice_casino_state.addiction_level < 0.0)
    {
        alice_casino_state.addiction_level = 0.0;
    }
    if (alice_casino_state.addiction_level > casino_addiction_max)
    {
        alice_casino_state.addiction_level = casino_addiction_max;
    }
}

void alice_casino(const int month) // month не используется -- параметр const
{
    // Если Алиса в завязке -- зависимость отходит быстрее, и в этом месяце она не играет
    if (alice_casino_state.quit_months_left > 0)
    {
        alice_casino_state.quit_months_left -= 1;
        alice_casino_state.addiction_level -= casino_addiction_decay_during_quit;
        clamp_addiction();
        return;
    }

    double play_probability = casino_base_play_probability + alice_casino_state.addiction_level * casino_play_probability_per_addiction;
    if (play_probability > 80.0)
    {
        play_probability = 80.0;
    }

    // Не играет в этом месяце -- зависимость медленно отходит сама
    if (roll_d100() > play_probability)
    {
        alice_casino_state.addiction_level -= casino_addiction_decay_normal;
        clamp_addiction();
        return;
    }

    double budget_percent = casino_base_budget_percent + alice_casino_state.addiction_level * casino_budget_percent_per_addiction;
    if (budget_percent > 50.0)
    {
        budget_percent = 50.0;
    }

    // Ставка считается от месячной зарплаты, а не от всех накоплений -- иначе казино
    // может смести деньги, отложенные на первый взнос за квартиру или подушку безопасности.
    RUB stake = static_cast<RUB>(alice.salary * budget_percent / 100.0);

    double loss_probability = casino_base_loss_probability + alice_casino_state.addiction_level * casino_loss_probability_per_addiction;
    if (loss_probability > 75.0)
    {
        loss_probability = 75.0;
    }

    double outcome_percent = 5.0 + roll_d100() % 46; // 5..50, насколько от ставки выиграли или проиграли

    if (roll_d100() <= loss_probability)
    {
        RUB loss_amount = static_cast<RUB>(stake * outcome_percent / 100.0);
        spend(loss_amount);
        alice_casino_state.total_lost += loss_amount;

        if (outcome_percent >= casino_big_outcome_threshold) // крупный проигрыш -- уходит в завязку
        {
            alice_casino_state.addiction_level -= casino_big_loss_addiction_drop;
            alice_casino_state.quit_months_left = 2 + (roll_d100() % 7); // 2..8 месяцев
            alice_casino_state.times_quit += 1;
        }
    }
    else
    {
        RUB win_amount = static_cast<RUB>(stake * outcome_percent / 100.0);
        alice.tbank.balance += win_amount;
        alice_casino_state.total_won += win_amount;

        if (outcome_percent >= casino_big_outcome_threshold) // крупный выигрыш -- сильно затягивает
        {
            alice_casino_state.addiction_level += casino_big_win_addiction_gain;
        }
    }

    clamp_addiction();
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

    printf("%d: доход = %lld, ндфл удержано = %lld, тбанк = %lld, втб = %lld, долг по ипотеке = %lld, зависимость = %.1f\n",
        year, alice_ndfl_state.income_year, alice_ndfl_state.withheld_year,
        alice.tbank.balance, alice.vtb.balance, alice_mortgage_state.remaining_debt,
        alice_casino_state.addiction_level);
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

    for (int i = 0; i < months_in_simulation; ++i)
    {
        alice_inflation(month);
        alice_career(month);
        alice_ndfl_year_close(year, month);
        alice_salary(month);
        alice_bank_interest(month);
        alice_food(month);
        alice_utilities(month);
        alice_transport(month);
        alice_subscriptions(month);
        alice_checkup(month);
        alice_dentist(month);
        alice_illness(month);
        alice_mortgage_payment(month);
        alice_property_tax(month);
        alice_rent_income(month);
        alice_benefits(month);
        alice_random_events(month);
        alice_casino(month);
        alice_sweep_to_vtb(month);
        alice_try_buy_apartment(month);

        // alice_entertainment(month);

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
    printf("Всего на счетах: %lld руб.\n", alice.tbank.balance + alice.vtb.balance);

    if (alice_mortgage_state.is_taken)
    {
        printf("Квартира куплена, остаток долга по ипотеке: %lld руб.\n", alice_mortgage_state.remaining_debt);
    }
    else
    {
        printf("Квартира ещё не куплена.\n");
    }

    printf("Казино: выиграно %lld, проиграно %lld, уходила в завязку %d раз\n",
        alice_casino_state.total_won, alice_casino_state.total_lost, alice_casino_state.times_quit);
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
