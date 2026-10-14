#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

using RUB = long long int;  // %lld

constexpr int months_in_simulation = 15 * 12;
constexpr int start_year = 2026;

//  1. ГЕНЕРАТОР СЛУЧАЙНЫХ ЧИСЕЛ 

struct Rng
{
    unsigned int seed = 0;

    void init(unsigned int s)
    {
        seed = s;
        srand(s);
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
};

Rng g_rng;
 
//  2. ЧЕЛОВЕК И СЧЕТА 

struct bankAccount
{
    RUB balance = 0;
    double monthly_rate;
};

struct person
{
    RUB salary;
    bankAccount tbank{0, 0.003};
    bankAccount vtb{0, 0.005};

    static constexpr RUB foodBaseCost = 15'000;
    static constexpr RUB utilitiesBaseCost = 6'000;
    static constexpr RUB transportBaseCost = 4'000;
    static constexpr RUB subscriptionsBaseCost = 1'500;
    static constexpr RUB profDevelopmentCost = 8'000;

    static constexpr RUB checkupBaseCost = 15'000;
    static constexpr RUB dentistBaseCost = 12'000;
    static constexpr RUB illnessBaseCost = 5'000;

    static constexpr RUB fuelBaseCost = 8'000;
    static constexpr RUB carMaintenanceBaseCost = 25'000;
    static constexpr RUB carRepairBaseCost = 40'000;

    static constexpr RUB mobileBaseCost = 1'200;
    static constexpr RUB entertainmentBaseCost = 5'000;
    static constexpr RUB giftsBaseCost = 5'000;

    static constexpr RUB housingRentBaseCost = 40'000;
};

struct person alice;

RUB get_tbank_buffer()
{
    RUB buffer = 50'000;
    buffer += alice.salary / 2;
    return buffer;
}

//  
//  3. СОСТОЯНИЯ
//  

struct depositState
{
    RUB amount = 0;
    int months_left = 0;

    RUB total_interest_earned = 0;
    int total_deposits_opened = 0;

    bool acquired_in_marriage = false;
};

struct carState
{
    bool is_bought = false;
    RUB remaining_debt = 0;
    int months_left = 0;

    bool acquired_in_marriage = false;
};

struct mortgageState
{
    bool is_taken = false;
    RUB remaining_debt = 0;
    int months_left = 0;

    RUB total_early_repaid = 0;
    int early_repay_count = 0;
    int term_shortened_months = 0;

    int months_since_purchase = -1;
    int next_big_update_year = 0;

    RUB total_spent_renovation = 0;

    bool acquired_in_marriage = false;
};

struct pvzState
{
    bool is_open = false;
    int months_open = 0;

    RUB total_revenue = 0;
    RUB total_usn_tax = 0;
    RUB total_ip_contributions = 0;
    RUB total_other_costs = 0;

    RUB year_revenue = 0;

    bool acquired_in_marriage = false;
};

struct relationshipState
{
    bool has_partner = false;
    bool is_married = false;
    bool is_divorced = false;

    int months_together = 0;
    int meeting_year = 0;

    int marriage_year = 0;
    int marriage_month = 0;

    RUB total_dates_spent = 0;
    RUB wedding_cost = 0;
    RUB divorce_cost = 0;
};

depositState alice_deposit_state;
carState alice_car_state;
mortgageState alice_mortgage_state;
pvzState alice_pvz_state;
relationshipState alice_relationship_state;

constexpr int pet_lifespan_years = 12;
constexpr RUB pet_foodBaseCost = 4'000;

struct petState
{
    bool is_alive = false;
    int age_months = 0;
    int lifespan_months = pet_lifespan_years * 12;
    int sick_months_left = 0;

    RUB total_spent_food = 0;
    RUB total_spent_vet = 0;
    RUB total_spent_illness = 0;

    int times_tried = 0;
};

petState alice_pet_state;

//  4. КРИЗИС (мировой)

constexpr int crisis_min_gap_years = 8;
constexpr int crisis_max_gap_years = 10;

constexpr int crisis_min_duration_months = 12;
constexpr int crisis_max_duration_months = 18;

constexpr double crisis_salary_drop_percent = 25.0;
constexpr double crisis_inflation_bonus = 0.015;
constexpr double crisis_pvz_revenue_drop_percent = 30.0;
constexpr double crisis_deposit_rate_bonus = 4.0;
constexpr double crisis_casino_addiction_bonus = 3.0;

struct crisisState
{
    int years_until_next = 0;
    int months_left = 0;
    int total_crises = 0;
    int months_in_crisis = 0;
};

crisisState alice_crisis_state;

//  5. БАНКРОТСТВО (глобально)

constexpr RUB bankruptcy_trigger_debt = 500'000;
constexpr int bankruptcy_trigger_months = 6;

constexpr int bankruptcy_block_months = 24;
constexpr int bankruptcy_mark_years = 5;
constexpr double bankruptcy_mark_rate_bonus = 5.0;

struct bankruptcyState
{
    int times_went_negative = 0;
    int months_in_negative = 0;
    int max_months_in_negative = 0;

    RUB max_negative_balance = 0;
    RUB total_negative_sum = 0;

    int times_bankrupt = 0;
    bool is_bankrupt_now = false;

    int blocked_months_left = 0;
    int mark_years_left = 0;
};

bankruptcyState alice_bankruptcy_state;
 
//  6. НДФЛ

constexpr int ndfl_withhold_percent = 13;

struct ndflState
{
    RUB income_year = 0;
    RUB withheld_year = 0;
};

ndflState alice_ndfl_state;

RUB pay_taxable_income(RUB gross_amount)
{
    RUB tax = gross_amount * ndfl_withhold_percent / 100;
    RUB net_amount = gross_amount - tax;

    alice_ndfl_state.income_year += gross_amount;
    alice_ndfl_state.withheld_year += tax;

    alice.tbank.balance += net_amount;

    return net_amount;
}

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

//  7. ИНФЛЯЦИЯ

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
    mobile,
    gifts,
    vacation,
    pet,
    pvz,
    count_
};

constexpr int category_count = static_cast<int>(category::count_);

constexpr double base_monthly_inflation = 0.003;

struct categoryInflation
{
    double individual_rate;
    double current_multiplier = 1.0;
};

categoryInflation inflation_table[category_count] =
{
    { 0.0010 },
    { 0.0015 },
    { 0.0003 },
    { -0.0013 },
    { 0.0015 },
    { 0.0005 },
    { 0.0005 },
    { 0.0015 },
    { 0.0015 },
    { 0.0010 },
    { 0.0019 },
    { 0.0019 },
    { 0.0010 },
};

static_assert(sizeof(inflation_table) / sizeof(inflation_table[0]) == category_count,
              "inflation_table размер не совпадает с category::count_");

void alice_inflation()
{
    double crisis_bonus = (alice_crisis_state.months_left > 0) ? crisis_inflation_bonus : 0.0;

    for (auto& entry : inflation_table)
    {
        double noise = (g_rng.roll_d100() - 50) / 10000.0;
        entry.current_multiplier *= (1.0 + base_monthly_inflation + entry.individual_rate + noise + crisis_bonus);
    }
}

RUB get_price(const category itemCategory, const RUB base_amount)
{
    int idx = static_cast<int>(itemCategory);
    if (idx < 0 or idx >= category_count) return base_amount;

    double multiplier = inflation_table[idx].current_multiplier;
    return static_cast<RUB>(base_amount * multiplier);
}

//  8. КАРЬЕРА И СМЕНА РАБОТЫ 

constexpr int exp_per_month_from_work = 4;
constexpr int exp_per_month_from_prof_development = 10;
constexpr int exp_to_level_up = 150;
constexpr int max_career_level = 10;

struct careerState
{
    int exp = 0;
    int level = 1;

    int months_until_next_try = 0;
    int probation_months_left = 0;

    int total_job_changes = 0;
    RUB total_salary_growth = 0;
};

careerState alice_career_state;

RUB salary_for_level(int level)
{
        
    switch (level)
    {
        case 1:  return 80'000;
        case 2:  return 110'000;
        case 3:  return 140'000;
        case 4:  return 180'000;
        case 5:  return 230'000;
        case 6:  return 290'000;
        case 7:  return 360'000;
        case 8:  return 440'000;
        case 9:  return 530'000;
        case 10: return 650'000;
        default: return 650'000;
    }
    
}
 

void alice_career()
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

constexpr int job_change_min_gap_months = 24;
constexpr int job_change_max_gap_months = 36;
constexpr int job_change_probation_months = 3;
constexpr double job_change_salary_min_bonus = 30.0;
constexpr double job_change_salary_max_bonus = 50.0;
constexpr double job_change_probation_coef = 0.7;

int roll_job_change_gap()
{
    int range = job_change_max_gap_months - job_change_min_gap_months + 1;
    return job_change_min_gap_months + (g_rng.roll_d100() % range);
}

void alice_job_change_init()
{
    alice_career_state.months_until_next_try = roll_job_change_gap();
}

void alice_try_change_job()
{
    if (alice_career_state.probation_months_left > 0) return;
    if (alice_bankruptcy_state.blocked_months_left > 0) return;

    if (alice_career_state.months_until_next_try > 0)
    {
        alice_career_state.months_until_next_try -= 1;
        return;
    }

    if (alice_career_state.level < 3)
    {
        alice_career_state.months_until_next_try = 6;
        return;
    }

    double chance = 30.0 + alice_career_state.level * 5.0;
    if (chance > 80.0) chance = 80.0;

    if (g_rng.roll_d100() > chance)
    {
        alice_career_state.months_until_next_try = 6;
        return;
    }

    int bonus_range = static_cast<int>(job_change_salary_max_bonus - job_change_salary_min_bonus + 1);
    double bonus_percent = job_change_salary_min_bonus + (g_rng.roll_d100() % bonus_range);

    RUB old_salary = alice.salary;
    RUB new_salary = static_cast<RUB>(alice.salary * (1.0 + bonus_percent / 100.0));

    RUB cap = static_cast<RUB>(salary_for_level(max_career_level) * 1.5);
    if (new_salary > cap) new_salary = cap;

    alice.salary = new_salary;
    alice_career_state.probation_months_left = job_change_probation_months;
    alice_career_state.months_until_next_try = roll_job_change_gap();
    alice_career_state.total_job_changes += 1;
    alice_career_state.total_salary_growth += (new_salary - old_salary);

    printf("Алиса перешла на новую работу: зарплата %lld -> %lld, "
           "испытательный срок %d мес.\n",
        old_salary, new_salary, bonus_percent);
}
 
//  9. ДВИЖЕНИЕ ДЕНЕГ 

RUB withdraw_available_from_vtb(RUB amount)
{
    RUB taken = (alice.vtb.balance >= amount) ? amount : alice.vtb.balance;
    alice.vtb.balance -= taken;
    return taken;
}

bool spend(RUB amount)
{
    if (alice.tbank.balance >= amount)
    {
        alice.tbank.balance -= amount;
        return true;
    }

    RUB missing = amount - alice.tbank.balance;
    alice.tbank.balance = 0;
    missing -= withdraw_available_from_vtb(missing);

    if (missing == 0) return true;

    alice.tbank.balance -= missing;

    // printf("ВНИМАНИЕ: Алисе не хватило %lld руб., баланс ушёл в минус.\n", missing);
    return false;
}

//  10. ФИНАНСОВЫЙ СОВЕТНИК

constexpr int emergency_fund_months = 6;

RUB estimate_monthly_living_cost()
{
    RUB total = 0;
    total += get_price(category::food, person::foodBaseCost);
    total += get_price(category::utilities, person::utilitiesBaseCost);
    total += get_price(category::transport, person::transportBaseCost);
    total += get_price(category::subscriptions, person::subscriptionsBaseCost);
    total += get_price(category::mobile, person::mobileBaseCost);
    total += get_price(category::entertainment, person::entertainmentBaseCost);
    total += get_price(category::health, person::illnessBaseCost);

    if (not alice_mortgage_state.is_taken)
    {
        total += get_price(category::housing, person::housingRentBaseCost);
    }

    if (alice_car_state.is_bought) // бензин -- тоже обязательный ежемесячный расход
    {
        total += get_price(category::transport, person::fuelBaseCost);
    }

    if (alice_pet_state.is_alive) // питомец ест каждый месяц
    {
        total += get_price(category::pet, pet_foodBaseCost);
    }

    return total;
}

bool has_emergency_fund_after(RUB amount)
{
    RUB savings_after = (alice.tbank.balance + alice.vtb.balance + alice_deposit_state.amount) - amount;
    RUB needed = estimate_monthly_living_cost() * emergency_fund_months;
    return savings_after >= needed;
}

bool is_payment_affordable(RUB monthly_payment, double max_percent_of_salary)
{
    RUB max_payment = static_cast<RUB>(alice.salary * max_percent_of_salary / 100.0);
    return monthly_payment <= max_payment;
}

int months_of_runway(RUB amount)
{
    RUB savings = (alice.tbank.balance + alice.vtb.balance) - amount;
    if (savings <= 0) return 0;

    RUB monthly = estimate_monthly_living_cost();
    if (monthly <= 0) return 999;

    return static_cast<int>(savings / monthly);
}

//  11. ВКЛАД

constexpr double deposit_annual_rate_percent = 16.0;
constexpr int deposit_term_months = 12;
constexpr RUB deposit_min_amount = 50'000;

void alice_deposit_interest()
{
    if (alice_deposit_state.amount <= 0) return;

    double rate = deposit_annual_rate_percent;
    if (alice_crisis_state.months_left > 0)
    {
        rate += crisis_deposit_rate_bonus;
    }

    RUB interest = static_cast<RUB>(
        alice_deposit_state.amount * (rate / 100.0 / 12.0));

    alice_deposit_state.amount += interest;
    alice_deposit_state.total_interest_earned += interest;

    if (alice_deposit_state.months_left > 0) alice_deposit_state.months_left -= 1;
}

void alice_deposit_maturity()
{
    if (alice_deposit_state.amount <= 0) return;
    if (alice_deposit_state.months_left > 0) return;

    printf("Вклад закрылся: %lld руб. вернулись на Т-Банк.\n", alice_deposit_state.amount);

    alice.tbank.balance += alice_deposit_state.amount;
    alice_deposit_state.amount = 0;
    alice_deposit_state.acquired_in_marriage = false;
}

void alice_try_open_deposit()
{
    if (alice_deposit_state.amount > 0) return;
    if (alice_bankruptcy_state.blocked_months_left > 0) return;

    RUB free_money = (alice.tbank.balance + alice.vtb.balance) - get_tbank_buffer();
    if (free_money < deposit_min_amount) return;

    RUB deposit_amount = free_money / 2;
    if (deposit_amount < deposit_min_amount) return;

    RUB from_tbank = (alice.tbank.balance >= deposit_amount) ? deposit_amount : alice.tbank.balance;
    RUB from_vtb = deposit_amount - from_tbank;

    alice.tbank.balance -= from_tbank;
    alice.vtb.balance -= from_vtb;
    alice_deposit_state.amount += deposit_amount;
    alice_deposit_state.months_left = deposit_term_months;
    alice_deposit_state.total_deposits_opened += 1;

    if (alice_relationship_state.is_married)
        alice_deposit_state.acquired_in_marriage = true;

    printf("Алиса открыла вклад на %lld руб. под %.0f%% годовых на %d мес.\n",
        deposit_amount, deposit_annual_rate_percent, deposit_term_months);
}

//  12. ДОХОДЫ

struct eventState
{
    int sick_months_left = 0;
    int months_since_big_purchase = 0;
};

eventState alice_event_state;

void alice_salary()
{
    RUB effective_salary = alice.salary;

    if (alice_event_state.sick_months_left > 0)
    {
        effective_salary = alice.salary / 2;
        alice_event_state.sick_months_left -= 1;
    }
    else if (alice_career_state.probation_months_left > 0)
    {
        effective_salary = static_cast<RUB>(alice.salary * job_change_probation_coef);
        alice_career_state.probation_months_left -= 1;
    }

    if (alice_crisis_state.months_left > 0)
    {
        double coef = 1.0 - crisis_salary_drop_percent / 100.0;
        effective_salary = static_cast<RUB>(effective_salary * coef);
    }

    pay_taxable_income(effective_salary);
}

struct sideJobState
{
    bool is_working = false;
    RUB salary = 25'000;
};

sideJobState alice_side_job_state;

void alice_side_job(const int year, const int month)
{
    if (not alice_side_job_state.is_working)
    {
        if (year >= start_year + 2 and month == 6)
        {
            alice_side_job_state.is_working = true;
        }
        else
        {
            return;
        }
    }

    pay_taxable_income(alice_side_job_state.salary);
}

constexpr double annual_bonus_percent_of_salary = 100.0;

void alice_bonus(const int month)
{
    if (month != 12) return;

    RUB bonus_gross = static_cast<RUB>(alice.salary * annual_bonus_percent_of_salary / 100.0);
    pay_taxable_income(bonus_gross);
}

void alice_ndfl_year_close(const int month)
{
    if (month != 1) return;

    RUB correct_tax = calculate_annual_tax(alice_ndfl_state.income_year);
    RUB extra = correct_tax - alice_ndfl_state.withheld_year;

    if (extra > 0) alice.tbank.balance -= extra;

    alice_ndfl_state.income_year = 0;
    alice_ndfl_state.withheld_year = 0;
}

void alice_bank_interest()
{
    RUB tbank_interest = static_cast<RUB>(alice.tbank.balance * alice.tbank.monthly_rate);
    alice.tbank.balance += tbank_interest;

    RUB vtb_interest = static_cast<RUB>(alice.vtb.balance * alice.vtb.monthly_rate);
    alice.vtb.balance += vtb_interest;
}

void alice_sweep_to_vtb()
{
    RUB buffer = get_tbank_buffer();

    if (alice.tbank.balance > buffer)
    {
        RUB free_money = alice.tbank.balance - buffer;
        alice.tbank.balance -= free_money;
        alice.vtb.balance += free_money;
    }
}

//  13. ОБЯЗАТЕЛЬНЫЕ РАСХОДЫ

void alice_food() { spend(get_price(category::food, person::foodBaseCost)); }
void alice_utilities() { spend(get_price(category::utilities, person::utilitiesBaseCost)); }

void alice_transport()
{
    if (alice_car_state.is_bought) return;
    spend(get_price(category::transport, person::transportBaseCost));
}

void alice_subscriptions() { spend(get_price(category::subscriptions, person::subscriptionsBaseCost)); }
void alice_mobile()        { spend(get_price(category::mobile, person::mobileBaseCost)); }
void alice_entertainment() { spend(get_price(category::entertainment, person::entertainmentBaseCost)); }

void alice_gifts(const int month)
{
    if (month == 3 or month == 6 or month == 9)
        spend(get_price(category::gifts, person::giftsBaseCost));

    if (month == 12)
        spend(get_price(category::gifts, person::giftsBaseCost) * 2);
}

void alice_housing_rent()
{
    if (alice_mortgage_state.is_taken) return;
    spend(get_price(category::housing, person::housingRentBaseCost));
}

//  14. ОТПУСКА

constexpr RUB vacationEconomyBaseCost = 30'000;
constexpr RUB vacationNormalBaseCost  = 60'000;
constexpr RUB vacationLuxuryBaseCost  = 180'000;

constexpr int vacation_luxury_min_level = 5;
constexpr double vacation_skip_chance = 10.0;

struct vacationState
{
    int last_vacation_year = 0;
    int total_vacations = 0;
    int total_skipped = 0;

    RUB total_spent = 0;

    int economy_count = 0;
    int normal_count = 0;
    int luxury_count = 0;
};

vacationState alice_vacation_state;

void alice_vacation(const int year, const int month)
{
    if (month != 7) return;
    if (alice_vacation_state.last_vacation_year == year) return;

    alice_vacation_state.last_vacation_year = year;

    if (g_rng.roll_d100() <= vacation_skip_chance)
    {
        alice_vacation_state.total_skipped += 1;
        printf("Алиса решила в этом году не ехать в отпуск.\n");
        return;
    }

    double roll = g_rng.roll_d100();
    RUB total_savings = alice.tbank.balance + alice.vtb.balance;

    RUB economy_price = get_price(category::vacation, vacationEconomyBaseCost);
    RUB normal_price  = get_price(category::vacation, vacationNormalBaseCost);
    RUB luxury_price  = get_price(category::vacation, vacationLuxuryBaseCost);

    RUB chosen_price = economy_price;
    const char* chosen_name = "эконом";

    if (roll <= 60.0)
    {
        chosen_price = economy_price;
        chosen_name = "эконом";
        alice_vacation_state.economy_count += 1;
    }
    else if (roll <= 95.0)
    {
        if (total_savings >= normal_price)
        {
            chosen_price = normal_price;
            chosen_name = "обычный";
            alice_vacation_state.normal_count += 1;
        }
        else
        {
            chosen_price = economy_price;
            chosen_name = "эконом (не хватило на обычный)";
            alice_vacation_state.economy_count += 1;
        }
    }
    else
    {
        bool career_allows = alice_career_state.level >= vacation_luxury_min_level;
        bool money_allows  = total_savings >= luxury_price * 2;

        if (career_allows and money_allows)
        {
            chosen_price = luxury_price;
            chosen_name = "дорогой";
            alice_vacation_state.luxury_count += 1;
        }
        else if (total_savings >= normal_price)
        {
            chosen_price = normal_price;
            chosen_name = "обычный (не дотянула до дорогого)";
            alice_vacation_state.normal_count += 1;
        }
        else
        {
            chosen_price = economy_price;
            chosen_name = "эконом (не хватило на дорогой)";
            alice_vacation_state.economy_count += 1;
        }
    }

    spend(chosen_price);
    alice_vacation_state.total_spent += chosen_price;
    alice_vacation_state.total_vacations += 1;

    printf("Алиса в отпуске (%s), потратила %lld руб.\n", chosen_name, chosen_price);
}

//  15. ЗДОРОВЬЕ

void alice_checkup(const int month)
{
    if (month != 2) return;
    spend(get_price(category::health, person::checkupBaseCost));
}

void alice_dentist(const int month)
{
    if (month != 8) return;
    spend(get_price(category::health, person::dentistBaseCost));
}

void alice_illness(const int month)
{
    if (month != 3 and month != 11) return;
    spend(get_price(category::health, person::illnessBaseCost));
}

//  16. МАШИНА

constexpr RUB car_price = 1'500'000;
constexpr double car_down_payment_percent = 20.0;
constexpr RUB car_down_payment_needed = static_cast<RUB>(car_price * car_down_payment_percent / 100.0);

constexpr int car_loan_term_months = 3 * 12;
constexpr double car_loan_annual_rate = 20.0;

constexpr double max_car_payment_percent_of_salary = 20.0;

constexpr int car_try_start_year = start_year + 1;
constexpr int car_try_interval_years = 2;
constexpr int car_try_month = 5;

double get_car_loan_rate()
{
    if (alice_bankruptcy_state.mark_years_left > 0) return car_loan_annual_rate + bankruptcy_mark_rate_bonus;
    return car_loan_annual_rate;
}

void alice_try_buy_car(const int year, const int month)
{
    if (alice_bankruptcy_state.blocked_months_left > 0) return;
    if (alice_car_state.is_bought) return;
    if (month != car_try_month) return;
    if (year < car_try_start_year) return;
    if ((year - car_try_start_year) % car_try_interval_years != 0) return;

    RUB total_savings = alice.tbank.balance + alice.vtb.balance;
    if (total_savings < car_down_payment_needed) return;

    double rate = get_car_loan_rate();
    RUB debt = car_price - car_down_payment_needed;
    RUB principal_part = debt / car_loan_term_months;
    RUB interest_part = static_cast<RUB>(debt * (rate / 100.0 / 12.0));
    RUB first_payment = principal_part + interest_part;

    if (not is_payment_affordable(first_payment, max_car_payment_percent_of_salary)) return;
    if (not has_emergency_fund_after(car_down_payment_needed)) return;

    if (not alice_mortgage_state.is_taken)
    {
        int runway = months_of_runway(car_down_payment_needed);
        if (runway < 6) return;
    }

    RUB from_vtb = (alice.vtb.balance >= car_down_payment_needed) ? car_down_payment_needed : alice.vtb.balance;
    RUB from_tbank = car_down_payment_needed - from_vtb;

    alice.vtb.balance -= from_vtb;
    alice.tbank.balance -= from_tbank;

    alice_car_state.is_bought = true;
    alice_car_state.remaining_debt = car_price - car_down_payment_needed;
    alice_car_state.months_left = car_loan_term_months;

    if (alice_relationship_state.is_married)
        alice_car_state.acquired_in_marriage = true;

    printf("Алиса купила машину: взнос %lld, долг %lld, платёж ~%lld/мес.\n",
        car_down_payment_needed, debt, first_payment);
}

void alice_car_loan_payment()
{
    if (not alice_car_state.is_bought or alice_car_state.months_left == 0) return;

    double rate = get_car_loan_rate();
    RUB principal_part = alice_car_state.remaining_debt / alice_car_state.months_left;
    RUB interest_part = static_cast<RUB>(alice_car_state.remaining_debt * (rate / 100.0 / 12.0));

    spend(principal_part + interest_part);

    alice_car_state.remaining_debt -= principal_part;
    alice_car_state.months_left -= 1;
}

void alice_fuel()
{
    if (not alice_car_state.is_bought) return;
    spend(get_price(category::transport, person::fuelBaseCost));
}

void alice_car_maintenance(const int month)
{
    if (not alice_car_state.is_bought) return;
    if (month != 4) return;
    spend(get_price(category::transport, person::carMaintenanceBaseCost));
}

void alice_car_repair(const int year, const int month)
{
    if (not alice_car_state.is_bought) return;
    if (month != 10 or year % 3 != 0) return;
    spend(get_price(category::transport, person::carRepairBaseCost));
}

//  17. ДОМАШНЕЕ ЖИВОТНОЕ 

constexpr int pet_appear_year = start_year + 1;
constexpr int pet_try_interval_years = 2;
constexpr int pet_try_month = 4;

constexpr RUB pet_vetBaseCost = 6'000;
constexpr RUB pet_illnessBaseCost = 8'000;
constexpr RUB pet_toysBaseCost = 2'000;

constexpr double pet_yearly_illness_chance = 15.0;
constexpr double pet_death_chance_growth = 8.0;

void alice_pet_birth(const int year, const int month)
{
    if (alice_bankruptcy_state.blocked_months_left > 0) return;
    if (alice_pet_state.is_alive) return;
    if (month != pet_try_month) return;
    if (year < pet_appear_year) return;
    if ((year - pet_appear_year) % pet_try_interval_years != 0) return;

    alice_pet_state.times_tried += 1;

    if (not has_emergency_fund_after(0))
    {
        printf("Алиса хотела завести питомца (%d-я попытка), но подушки безопасности не хватает.\n",
            alice_pet_state.times_tried);
        return;
    }

    RUB pet_monthly_cost = get_price(category::pet, pet_foodBaseCost) + get_price(category::pet, pet_toysBaseCost);

    RUB monthly_income = alice.salary;
    if (alice_side_job_state.is_working) monthly_income += alice_side_job_state.salary;

    RUB monthly_expenses = estimate_monthly_living_cost() + pet_monthly_cost;
    if (monthly_expenses > monthly_income)
    {
        printf("Алиса хотела завести питомца (%d-я попытка), но расходы превышают доход.\n",
            alice_pet_state.times_tried);
        return;
    }

    alice_pet_state.is_alive = true;
    alice_pet_state.age_months = 0;
    alice_pet_state.lifespan_months = pet_lifespan_years * 12;
    printf("У Алисы появился питомец (%d-я попытка)!\n", alice_pet_state.times_tried);
}

void alice_pet_death_check()
{
    if (not alice_pet_state.is_alive) return;

    if (alice_pet_state.age_months >= alice_pet_state.lifespan_months)
    {
        int months_over = alice_pet_state.age_months - alice_pet_state.lifespan_months;
        double years_over = months_over / 12.0;
        double death_chance = (pet_death_chance_growth / 12.0) * (1.0 + years_over); // растёт с каждым годом сверх срока
        if (death_chance > 100.0) death_chance = 100.0;

        if (g_rng.roll_d100() <= death_chance)
        {
            alice_pet_state.is_alive = false;
            printf("Питомец Алисы умер в возрасте %d месяцев.\n", alice_pet_state.age_months);
        }
    }
}

void alice_pet_aging()
{
    if (not alice_pet_state.is_alive) return;

    alice_pet_state.age_months += 1;
    if (alice_pet_state.sick_months_left > 0) alice_pet_state.sick_months_left -= 1;
}

void alice_pet_food()
{
    if (not alice_pet_state.is_alive) return;

    RUB cost = get_price(category::pet, pet_foodBaseCost);
    spend(cost);
    alice_pet_state.total_spent_food += cost;
}

void alice_pet_toys()
{
    if (not alice_pet_state.is_alive) return;
    spend(get_price(category::pet, pet_toysBaseCost));
}

void alice_pet_vet(const int month)
{
    if (not alice_pet_state.is_alive) return;
    if (month != 5) return;

    RUB cost = get_price(category::pet, pet_vetBaseCost);
    spend(cost);
    alice_pet_state.total_spent_vet += cost;
}

void alice_pet_illness()
{
    if (not alice_pet_state.is_alive) return;
    if (alice_pet_state.age_months % 12 != 0) return;

    if (g_rng.roll_d100() <= pet_yearly_illness_chance)
    {
        RUB cost = get_price(category::pet, pet_illnessBaseCost);
        spend(cost);
        alice_pet_state.total_spent_illness += cost;
        printf("Питомец Алисы заболел, лечение обошлось в %lld руб.\n", cost);
    }
}

//  18. ИПОТЕКА И НЕДВИЖИМОСТЬ 

constexpr RUB apartment_price = 6'000'000;
constexpr double down_payment_percent = 20.0;
constexpr RUB down_payment_needed = static_cast<RUB>(apartment_price * down_payment_percent / 100.0);

constexpr int mortgage_term_months = 15 * 12;
constexpr double mortgage_annual_rate = 18.0;

constexpr double property_tax_rate = 0.1;

constexpr double max_mortgage_payment_percent_of_salary = 35.0;
constexpr RUB post_purchase_reserve = 200'000;

constexpr int mortgage_try_start_year = start_year + 1;
constexpr int mortgage_try_interval_years = 2;
constexpr int mortgage_try_month = 6;

double get_mortgage_rate()
{
    if (alice_bankruptcy_state.mark_years_left > 0) return mortgage_annual_rate + bankruptcy_mark_rate_bonus;
    return mortgage_annual_rate;
}

RUB mortgage_first_payment(RUB debt)
{
    double rate = get_mortgage_rate();
    RUB principal_part = debt / mortgage_term_months;
    RUB interest_part = static_cast<RUB>(debt * (rate / 100.0 / 12.0));
    return principal_part + interest_part;
}

void alice_try_buy_apartment(const int year, const int month)
{
    if (alice_bankruptcy_state.blocked_months_left > 0) return;
    if (alice_mortgage_state.is_taken) return;
    if (month != mortgage_try_month) return;
    if (year < mortgage_try_start_year) return;
    if ((year - mortgage_try_start_year) % mortgage_try_interval_years != 0) return;

    RUB total_savings = alice.tbank.balance + alice.vtb.balance;
    RUB available_for_down_payment = total_savings - post_purchase_reserve;

    if (available_for_down_payment < down_payment_needed) return;

    RUB down_payment = available_for_down_payment;
    if (down_payment > apartment_price) down_payment = apartment_price;

    RUB debt = apartment_price - down_payment;
    RUB first_payment = mortgage_first_payment(debt);

    if (not is_payment_affordable(first_payment, max_mortgage_payment_percent_of_salary)) return;
    if (not has_emergency_fund_after(down_payment)) return;

    int runway = months_of_runway(down_payment);
    if (runway < 3) return;

    RUB from_vtb = (alice.vtb.balance >= down_payment) ? down_payment : alice.vtb.balance;
    RUB from_tbank = down_payment - from_vtb;

    alice.vtb.balance -= from_vtb;
    alice.tbank.balance -= from_tbank;

    alice_mortgage_state.is_taken = true;
    alice_mortgage_state.remaining_debt = debt;
    alice_mortgage_state.months_left = mortgage_term_months;
    alice_mortgage_state.months_since_purchase = 0;

    if (alice_relationship_state.is_married)
        alice_mortgage_state.acquired_in_marriage = true;

    printf("Алиса взяла ипотеку: взнос %lld, долг %lld, платёж ~%lld/мес, запаса ещё на %d мес.\n",
        down_payment, debt, first_payment, months_of_runway(0));
}

void alice_mortgage_payment()
{
    if (not alice_mortgage_state.is_taken or alice_mortgage_state.months_left == 0) return;

    double rate = get_mortgage_rate();
    RUB principal_part = alice_mortgage_state.remaining_debt / alice_mortgage_state.months_left;
    RUB interest_part = static_cast<RUB>(alice_mortgage_state.remaining_debt * (rate / 100.0 / 12.0));

    spend(principal_part + interest_part);

    alice_mortgage_state.remaining_debt -= principal_part;
    alice_mortgage_state.months_left -= 1;
}

constexpr RUB mortgage_early_repay_min = 10'000;
constexpr int mortgage_early_repay_month = 12;
constexpr double mortgage_early_repay_free_part = 0.5;

void alice_mortgage_early_repayment(const int month)
{
    if (not alice_mortgage_state.is_taken or alice_mortgage_state.months_left == 0) return;
    if (month != mortgage_early_repay_month) return;

    RUB buffer = get_tbank_buffer();
    RUB emergency = estimate_monthly_living_cost() * emergency_fund_months;

    RUB free_money = (alice.tbank.balance + alice.vtb.balance) - buffer - emergency;
    if (free_money < mortgage_early_repay_min) return;

    RUB repay_amount = static_cast<RUB>(free_money * mortgage_early_repay_free_part);
    if (repay_amount > alice_mortgage_state.remaining_debt)
        repay_amount = alice_mortgage_state.remaining_debt;

    if (repay_amount < mortgage_early_repay_min) return;

    RUB from_tbank = (alice.tbank.balance >= repay_amount) ? repay_amount : alice.tbank.balance;
    RUB from_vtb = repay_amount - from_tbank;

    alice.tbank.balance -= from_tbank;
    alice.vtb.balance -= from_vtb;

    RUB old_debt = alice_mortgage_state.remaining_debt;
    alice_mortgage_state.remaining_debt -= repay_amount;

    double rate = get_mortgage_rate();
    RUB old_payment = old_debt / alice_mortgage_state.months_left
                    + static_cast<RUB>(old_debt * (rate / 100.0 / 12.0));

    if (old_payment > 0 and alice_mortgage_state.remaining_debt > 0)
    {
        RUB interest_part = static_cast<RUB>(alice_mortgage_state.remaining_debt * (rate / 100.0 / 12.0));
        RUB principal_per_month = old_payment - interest_part;

        if (principal_per_month > 0)
        {
            int new_months = static_cast<int>(alice_mortgage_state.remaining_debt / principal_per_month);
            if (new_months < 1) new_months = 1;

            int shortened = alice_mortgage_state.months_left - new_months;
            if (shortened > 0)
            {
                alice_mortgage_state.term_shortened_months += shortened;
                alice_mortgage_state.months_left = new_months;
            }
        }
    }

    alice_mortgage_state.total_early_repaid += repay_amount;
    alice_mortgage_state.early_repay_count += 1;

    printf("Алиса досрочно погасила ипотеку на %lld руб. "
           "(долг теперь %lld, срок сокращён всего на %d мес.)\n",
        repay_amount,
        alice_mortgage_state.remaining_debt,
        alice_mortgage_state.term_shortened_months);

    if (alice_mortgage_state.remaining_debt == 0)
    {
        alice_mortgage_state.months_left = 0;
        printf("Ипотека полностью погашена!\n");
    }
}

constexpr RUB apartment_initial_renovation_base = 400'000;
constexpr int apartment_big_update_min_years = 5;
constexpr int apartment_big_update_max_years = 7;
constexpr RUB apartment_big_update_base = 250'000;

int roll_apartment_update_gap_years()
{
    int range = apartment_big_update_max_years - apartment_big_update_min_years + 1;
    return apartment_big_update_min_years + (g_rng.roll_d100() % range);
}

void alice_apartment_renovation(const int year, const int month)
{
    if (not alice_mortgage_state.is_taken) return;

    if (alice_mortgage_state.months_since_purchase == 0)
    {
        RUB cost = get_price(category::housing, apartment_initial_renovation_base);
        spend(cost);
        alice_mortgage_state.total_spent_renovation += cost;

        printf("Алиса сделала ремонт при заселении: %lld руб.\n", cost);

        alice_mortgage_state.next_big_update_year = year + roll_apartment_update_gap_years();
        alice_mortgage_state.months_since_purchase = 1;
        return;
    }

    if (year >= alice_mortgage_state.next_big_update_year and month == 11)
    {
        RUB cost = get_price(category::housing, apartment_big_update_base);
        spend(cost);
        alice_mortgage_state.total_spent_renovation += cost;

        printf("Алиса обновила квартиру (техника/мебель/ремонт): %lld руб.\n", cost);

        alice_mortgage_state.next_big_update_year = year + roll_apartment_update_gap_years();
    }

    alice_mortgage_state.months_since_purchase += 1;
}

void alice_property_tax(const int month)
{
    if (not alice_mortgage_state.is_taken) return;
    if (month != 12) return;

    RUB current_value = get_price(category::housing, apartment_price);
    RUB tax = static_cast<RUB>(current_value * (property_tax_rate / 100.0));
    spend(tax);
}

constexpr double rent_monthly_yield_percent = 0.4;

void alice_rent_income()
{
    if (not alice_mortgage_state.is_taken) return;

    RUB rent_gross = static_cast<RUB>(apartment_price * rent_monthly_yield_percent / 100.0);

    if (alice_crisis_state.months_left > 0)
    {
        rent_gross = static_cast<RUB>(rent_gross * 0.7);
    }

    pay_taxable_income(rent_gross);
}

//  19. ПВЗ OZON (ИП) 

constexpr RUB pvz_open_cost = 300'000;
constexpr RUB pvz_monthly_revenue = 250'000;
constexpr RUB pvz_rent_cost = 60'000;
constexpr RUB pvz_staff_salary = 45'000;
constexpr RUB pvz_other_costs = 15'000;

constexpr double pvz_usn_rate_percent = 6.0;

constexpr RUB ip_fixed_contributions_per_year = 50'000;
constexpr double ip_extra_contribution_percent = 1.0;
constexpr RUB ip_extra_contribution_threshold = 300'000;

constexpr int pvz_try_start_year = start_year + 2;
constexpr int pvz_try_interval_years = 2;
constexpr int pvz_try_month = 9;




void alice_try_open_pvz(const int year, const int month)
{
    if (alice_bankruptcy_state.blocked_months_left > 0) return;
    if (alice_pvz_state.is_open) return;
    if (month != pvz_try_month) return;
    if (year < pvz_try_start_year) return;
    if ((year - pvz_try_start_year) % pvz_try_interval_years != 0) return;

    RUB total_savings = alice.tbank.balance + alice.vtb.balance;

    if (total_savings < pvz_open_cost + estimate_monthly_living_cost() * 6)
    {
        printf("Алиса хотела открыть ПВЗ OZON, но не хватает накоплений.\n");
        return;
    }

    if (not has_emergency_fund_after(pvz_open_cost))
    {
        printf("Алиса хотела открыть ПВЗ OZON, но подушки безопасности не хватит.\n");
        return;
    }

    RUB from_tbank = (alice.tbank.balance >= pvz_open_cost) ? pvz_open_cost : alice.tbank.balance;
    RUB from_vtb = pvz_open_cost - from_tbank;

    if (alice.vtb.balance < from_vtb)
    {
        printf("Алиса хотела открыть ПВЗ OZON, но не хватило денег.\n");
        return;
    }

    alice.tbank.balance -= from_tbank;
    alice.vtb.balance -= from_vtb;

    alice_pvz_state.is_open = true;
    alice_pvz_state.months_open = 0;

    if (alice_relationship_state.is_married)
        alice_pvz_state.acquired_in_marriage = true;

    printf("Алиса открыла ПВЗ OZON как ИП (вложила %lld руб.).\n", pvz_open_cost);
}

void alice_pvz_monthly()
{
    if (not alice_pvz_state.is_open) return;

    alice_pvz_state.months_open += 1;

    RUB revenue = get_price(category::pvz, pvz_monthly_revenue);

    if (alice_crisis_state.months_left > 0)
    {
        double coef = 1.0 - crisis_pvz_revenue_drop_percent / 100.0;
        revenue = static_cast<RUB>(revenue * coef);
    }

    alice_pvz_state.total_revenue += revenue;
    alice_pvz_state.year_revenue += revenue;

    RUB usn_tax = static_cast<RUB>(revenue * pvz_usn_rate_percent / 100.0);
    alice_pvz_state.total_usn_tax += usn_tax;

    RUB rent = get_price(category::housing, pvz_rent_cost);
    RUB staff = get_price(category::pvz, pvz_staff_salary);
    RUB other = get_price(category::pvz, pvz_other_costs);

    RUB costs = rent + staff + other + usn_tax;
    alice_pvz_state.total_other_costs += rent + staff + other;

    RUB net = revenue - costs;

    if (net >= 0) alice.tbank.balance += net;
    else spend(-net);
}

void alice_pvz_ip_contributions(const int month)
{
    if (not alice_pvz_state.is_open) return;
    if (month != 12) return;

    RUB fixed = get_price(category::pvz, ip_fixed_contributions_per_year);
    RUB extra = 0;

    if (alice_pvz_state.year_revenue > ip_extra_contribution_threshold)
    {
        RUB above = alice_pvz_state.year_revenue - ip_extra_contribution_threshold;
        extra = static_cast<RUB>(above * ip_extra_contribution_percent / 100.0);
    }

    RUB total = fixed + extra;
    alice_pvz_state.total_ip_contributions += total;

    printf("ПВЗ OZON: страховые взносы ИП за год -- %lld руб.\n", total);

    spend(total);
    alice_pvz_state.year_revenue = 0;
}


void alice_pvz_fire() {
    if (not alice_pvz_state.is_open) return;
    if (alice_pvz_state.months_open != 3) return;

    RUB loss = 100000;
    printf(" ПВЗ OZON СГОРЕЛ (через 3 мес.) ===\nУбыток: %lld руб.\n", loss);
    spend(loss);

    alice_pvz_state.is_open = false;
}


//  20. ОТНОШЕНИЯ: ЗНАКОМСТВО, СВИДАНИЯ, СВАДЬБА, РАЗВОД  

constexpr RUB dateBaseCost = 8'000;
constexpr RUB weddingBaseCost = 800'000;
constexpr RUB divorceBaseCost = 150'000;

constexpr int relationship_try_start_year = start_year + 1;
constexpr int relationship_meeting_max_year = start_year + 5;
constexpr double relationship_meeting_chance_per_year = 25.0;

constexpr int marriage_min_months_together = 24;
constexpr int marriage_max_months_together = 48;
constexpr double marriage_chance = 40.0;

constexpr double divorce_chance_per_year = 4.0;

void alice_relationship_step(const int year, const int month)
{
    if (not alice_relationship_state.has_partner)
    {
        if (year < relationship_try_start_year) return;
        if (year > relationship_meeting_max_year) return;
        if (month != 2) return;

        if (g_rng.roll_d100() <= relationship_meeting_chance_per_year)
        {
            alice_relationship_state.has_partner = true;
            alice_relationship_state.meeting_year = year;
            alice_relationship_state.months_together = 0;
            printf("Алиса познакомилась с партнёром (%d год).\n", year);
        }
        return;
    }

    alice_relationship_state.months_together += 1;

    if (not alice_relationship_state.is_married)
    {
        RUB date_cost = get_price(category::entertainment, dateBaseCost);
        spend(date_cost);
        alice_relationship_state.total_dates_spent += date_cost;
    }

    if (not alice_relationship_state.is_married and
        alice_relationship_state.months_together >= marriage_min_months_together and
        alice_relationship_state.months_together <= marriage_max_months_together and
        month == 8)
    {
        if (g_rng.roll_d100() <= marriage_chance)
        {
            RUB wedding_cost = get_price(category::entertainment, weddingBaseCost);

            if (alice.tbank.balance + alice.vtb.balance >= wedding_cost)
            {
                spend(wedding_cost);
                alice_relationship_state.wedding_cost = wedding_cost;
                alice_relationship_state.is_married = true;
                alice_relationship_state.marriage_year = year;
                alice_relationship_state.marriage_month = month;
                printf("Алиса вышла замуж! Свадьба обошлась в %lld руб.\n", wedding_cost);
            }
        }
    }

    if (alice_relationship_state.is_married and
        not alice_relationship_state.is_divorced and
        alice_relationship_state.months_together >= 60 and
        month == 1)
    {
        if (g_rng.roll_d100() <= divorce_chance_per_year)
        {
            RUB divorce_cost = get_price(category::entertainment, divorceBaseCost);
            spend(divorce_cost);

            printf("=== РАЗВОД ===\n");
            printf("Развод обошёлся в %lld руб.\n", divorce_cost);

            // 1. Деньги на счетах и вклад -- делятся пополам сразу, напрямую.
            //    Это окончательное списание, в payout ниже эти суммы повторно не включаются.
            RUB half_tbank = alice.tbank.balance / 2;
            RUB half_vtb = alice.vtb.balance / 2;
            alice.tbank.balance -= half_tbank;
            alice.vtb.balance -= half_vtb;

            RUB half_deposit = 0;
            if (alice_deposit_state.amount > 0 and alice_deposit_state.acquired_in_marriage)
            {
                half_deposit = alice_deposit_state.amount / 2;
                alice_deposit_state.amount -= half_deposit;
                printf("Вклад поделён: -%lld руб.\n", half_deposit);
            }

            // 2. Остальное имущество не лежит на счетах, поэтому компенсация бывшему
            //    собирается в payout и списывается деньгами одним платежом ниже.
            RUB payout = 0;

            // Машина -- если куплена в браке
            if (alice_car_state.is_bought and alice_car_state.acquired_in_marriage)
            {
                RUB car_value = get_price(category::transport, static_cast<RUB>(car_price * 0.7))
                               - alice_car_state.remaining_debt;
                if (car_value < 0) car_value = 0;

                RUB half_car = car_value / 2;
                payout += half_car;

                alice_car_state.is_bought = false;
                alice_car_state.remaining_debt = 0;
                alice_car_state.months_left = 0;
                alice_car_state.acquired_in_marriage = false;

                printf("Машина поделена: -%lld руб. (остаток стоимости), машина уходит бывшему.\n", half_car);
            }

            
            if (alice_mortgage_state.is_taken and alice_mortgage_state.acquired_in_marriage)
            {
                RUB apartment_value = get_price(category::housing, apartment_price);
                RUB equity = apartment_value - alice_mortgage_state.remaining_debt;

                if (equity > 0)
                {
                    RUB half_equity = equity / 2;
                    payout += half_equity;

                    printf("Квартира поделена: Алиса выплачивает %lld руб. за долю бывшего, "
                           "квартира и весь остаток ипотеки (%lld) остаются у неё.\n",
                        half_equity, alice_mortgage_state.remaining_debt);
                }
                else
                {
                    printf("Квартира в минусе по ипотеке (остаток долга %lld руб. выше стоимости) -- "
                           "делить нечего, весь долг остаётся на Алисе.\n",
                        alice_mortgage_state.remaining_debt);
                }
            }

            
            if (alice_pvz_state.is_open and alice_pvz_state.acquired_in_marriage)
            {
                RUB pvz_value = pvz_open_cost + alice_pvz_state.total_revenue / 4;
                RUB half_pvz = pvz_value / 2;
                payout += half_pvz;

                printf("ПВЗ поделён: Алиса выплачивает бывшему %lld руб. за его долю.\n", half_pvz);
            }

            spend(payout);

            RUB total_given_to_ex = half_tbank + half_vtb + half_deposit + payout;

            alice_relationship_state.is_married = false;
            alice_relationship_state.is_divorced = true;
            alice_relationship_state.has_partner = false;
            alice_relationship_state.divorce_cost = divorce_cost + total_given_to_ex;

            printf("=== РАЗВОД ЗАВЕРШЁН ===\n");
            printf("Итого отдано бывшему: %lld руб. (плюс судебные расходы %lld руб.)\n",
                total_given_to_ex, divorce_cost);
        }
    }
}

//  21. ЛЬГОТЫ

constexpr RUB monthly_benefit_amount = 10'000;
constexpr RUB benefit_income_threshold = 40'000;

void alice_benefits()
{
    RUB monthly_income = alice.salary;
    if (alice_side_job_state.is_working) monthly_income += alice_side_job_state.salary;

    if (monthly_income >= benefit_income_threshold) return;

    alice.tbank.balance += monthly_benefit_amount;
}

//  22. СЛУЧАЙНЫЕ СОБЫТИЯ

constexpr double big_purchase_base_chance = 3.0;
constexpr double big_purchase_growth = 1.1;

double big_purchase_chance()
{
    double chance = big_purchase_base_chance * std::pow(big_purchase_growth, alice_event_state.months_since_big_purchase);
    return (chance > 100.0) ? 100.0 : chance;
}

void alice_random_income_event()
{
    int roll = g_rng.roll_d100();
    if (roll <= 3)
    {
        pay_taxable_income(alice.salary);
        printf("Премия: +%lld руб. (до вычета НДФЛ)\n", alice.salary);
    }
}

void alice_random_health_event()
{
    int roll = g_rng.roll_d100();
    if (roll <= 3)
    {
        alice_event_state.sick_months_left = 2 + (g_rng.roll_d100() % 2);
        printf("Алиса заболела, зарплата временно уменьшена.\n");
    }
}

void alice_random_career_event()
{
    int roll = g_rng.roll_d100();
    if (roll <= 2)
    {
        alice.salary = static_cast<RUB>(alice.salary * 1.1);
        printf("Зарплату проиндексировали: теперь %lld руб.\n", alice.salary);
    }
}

void alice_impulse_purchase()
{
    alice_event_state.months_since_big_purchase += 1;

    if (g_rng.roll_d100() > big_purchase_chance()) return;

    RUB total_savings = alice.tbank.balance + alice.vtb.balance;

    
    if (total_savings <= 0) return;

    double percent = 20.0 + (g_rng.roll_d100() % 31);
    RUB purchase_amount = static_cast<RUB>(total_savings * percent / 100.0);

    if (purchase_amount <= 0) return;

    spend(purchase_amount);
    printf("Алиса не удержалась и потратила %lld руб. на крупную покупку.\n", purchase_amount);

    alice_event_state.months_since_big_purchase = 0;
}


//  23. КАЗИНО


constexpr double casino_addiction_start = 30.0;
constexpr double casino_addiction_max = 100.0;

constexpr double casino_base_play_probability = 1.2;
constexpr double casino_play_probability_per_addiction = 1.4;

constexpr double casino_base_budget_percent = 10.0;
constexpr double casino_budget_percent_per_addiction = 0.3;

constexpr double casino_risky_bet_base_chance = 10.0;
constexpr double casino_risky_bet_chance_per_addiction = 0.6;

constexpr double casino_big_win_addiction_gain = 50.0;
constexpr double casino_big_loss_addiction_drop = 8.0;

constexpr double casino_addiction_decay_normal = 0.5;
constexpr double casino_addiction_decay_during_quit = 3.0;

struct casinoState
{
    double addiction_level = casino_addiction_start;
    int quit_months_left = 0;

    RUB total_won = 0;
    RUB total_lost = 0;
    int times_quit = 0;
};

casinoState alice_casino_state;

int spin_roulette() { return g_rng.roll_d100() % 37; }

void clamp_addiction()
{
    if (alice_casino_state.addiction_level < 0.0) alice_casino_state.addiction_level = 10.0;
    if (alice_casino_state.addiction_level > casino_addiction_max) alice_casino_state.addiction_level = casino_addiction_max;
}

void alice_casino()
{
    if (alice_casino_state.quit_months_left > 0)
    {
        alice_casino_state.quit_months_left -= 1;
        alice_casino_state.addiction_level -= casino_addiction_decay_during_quit;
        clamp_addiction();
        return;
    }

    double play_probability = casino_base_play_probability + alice_casino_state.addiction_level * casino_play_probability_per_addiction;
    if (play_probability > 80.0) play_probability = 80.0;

    if (g_rng.roll_d100() > play_probability)
    {
        alice_casino_state.addiction_level -= casino_addiction_decay_normal;
        clamp_addiction();
        return;
    }

    double budget_percent = casino_base_budget_percent + alice_casino_state.addiction_level * casino_budget_percent_per_addiction;
    if (budget_percent > 50.0) budget_percent = 50.0;

    RUB stake = static_cast<RUB>(alice.salary * budget_percent / 100.0);

    double risky_bet_chance = casino_risky_bet_base_chance + alice_casino_state.addiction_level * casino_risky_bet_chance_per_addiction;
    if (risky_bet_chance > 70.0) risky_bet_chance = 70.0;

    bool bet_on_number = g_rng.roll_d100() <= risky_bet_chance;
    int spin = spin_roulette();

    if (bet_on_number)
    {
        int lucky_number = spin_roulette();
        bool won = (spin == lucky_number);

        if (won)
        {
            RUB win_amount = stake * 36;
            alice.tbank.balance += win_amount;
            alice_casino_state.total_won += win_amount;
            alice_casino_state.addiction_level += casino_big_win_addiction_gain;
        }
        else
        {
            spend(stake);
            alice_casino_state.total_lost += stake;
            alice_casino_state.addiction_level -= casino_big_loss_addiction_drop;
            alice_casino_state.quit_months_left = 2 + (g_rng.roll_d100() % 7);
            alice_casino_state.times_quit += 1;
        }
    }
    else
    {
        bool bet_red = (g_rng.roll_d100() % 2 == 0);
        bool spin_is_red = (spin != 0 and spin % 2 == 1);
        bool won = (spin != 0) and (bet_red == spin_is_red);

        if (won)
        {
            RUB win_amount = stake * 2;
            alice.tbank.balance += win_amount;
            alice_casino_state.total_won += win_amount;
        }
        else
        {
            spend(stake);
            alice_casino_state.total_lost += stake;
        }
    }

    clamp_addiction();
}

//  24. БАНКРОТСТВО (в жизни Элис)

void alice_bankruptcy_check()
{
    if (alice_bankruptcy_state.blocked_months_left > 0)
        alice_bankruptcy_state.blocked_months_left -= 1;
    if (alice_bankruptcy_state.mark_years_left > 0 and
        alice_bankruptcy_state.blocked_months_left == 0)
        alice_bankruptcy_state.mark_years_left -= 1;

    if (alice.tbank.balance < 0)
    {
        if (alice_bankruptcy_state.months_in_negative == 0)
        {
            alice_bankruptcy_state.times_went_negative += 1;
            //printf("Алиса ушла в минус: %lld руб.\n", alice.tbank.balance);
        }

        alice_bankruptcy_state.months_in_negative += 1;
        alice_bankruptcy_state.total_negative_sum += -alice.tbank.balance;

        if (alice_bankruptcy_state.months_in_negative > alice_bankruptcy_state.max_months_in_negative)
            alice_bankruptcy_state.max_months_in_negative = alice_bankruptcy_state.months_in_negative;

        if (-alice.tbank.balance > alice_bankruptcy_state.max_negative_balance)
            alice_bankruptcy_state.max_negative_balance = -alice.tbank.balance;
    }
    else
    {
        alice_bankruptcy_state.months_in_negative = 0;
        alice_bankruptcy_state.is_bankrupt_now = false;
    }

    bool debt_too_big = (-alice.tbank.balance) >= bankruptcy_trigger_debt;
    bool negative_too_long = alice_bankruptcy_state.months_in_negative >= bankruptcy_trigger_months;

    if (debt_too_big and negative_too_long and not alice_bankruptcy_state.is_bankrupt_now)
    {
        printf("=== АЛИСА ОБАНКРОТИЛАСЬ ===\n");
        printf("Долг по Т-Банку: %lld руб., месяцев в минусе: %d\n",
            alice.tbank.balance, alice_bankruptcy_state.months_in_negative);

        alice.tbank.balance = 0;
        alice.vtb.balance = 0;

        if (alice_car_state.is_bought)
        {
            alice_car_state.is_bought = false;
            alice_car_state.remaining_debt = 0;
            alice_car_state.months_left = 0;
            printf("Машина ушла с молотка.\n");
        }

        if (alice_pvz_state.is_open)
        {
            alice_pvz_state.is_open = false;
            alice_pvz_state.months_open = 0;
            printf("ПВЗ OZON закрыт из-за банкротства ИП.\n");
        }

        alice_bankruptcy_state.blocked_months_left = bankruptcy_block_months;
        alice_bankruptcy_state.mark_years_left = bankruptcy_mark_years;

        alice_casino_state.addiction_level -= 15.0;
        clamp_addiction();

        alice_bankruptcy_state.times_bankrupt += 1;
        alice_bankruptcy_state.is_bankrupt_now = true;
        alice_bankruptcy_state.months_in_negative = 0;
    }
}

//  25. КРИЗИС (влияние на жизнь Элис)

void alice_crisis_schedule_next(int current_year)
{
    int gap = crisis_min_gap_years + (g_rng.roll_d100() % (crisis_max_gap_years - crisis_min_gap_years + 1));
    alice_crisis_state.years_until_next = gap;
    printf("Следующий кризис ожидается примерно через %d лет (%d год).\n",
        gap, current_year + gap);
}

void alice_crisis_init(int current_year)
{
    alice_crisis_schedule_next(current_year);
}

void alice_crisis_step(const int year, const int month)
{
    if (alice_crisis_state.months_left > 0)
    {
        alice_crisis_state.months_left -= 1;
        alice_crisis_state.months_in_crisis += 1;

        if (alice_crisis_state.months_left == 0)
        {
            printf("[%d] Кризис закончился, экономика восстанавливается.\n", year);
            alice_crisis_schedule_next(year);
        }
        return;
    }

    if (month != 1) return;
    if (alice_crisis_state.years_until_next > 0)
    {
        alice_crisis_state.years_until_next -= 1;
        return;
    }

    int duration = crisis_min_duration_months +
                   (g_rng.roll_d100() % (crisis_max_duration_months - crisis_min_duration_months + 1));

    alice_crisis_state.months_left = duration;
    alice_crisis_state.total_crises += 1;

    alice_casino_state.addiction_level += crisis_casino_addiction_bonus;
    clamp_addiction();

    printf("=== КРИЗИС (%d год) ===\n", year);
    printf("Зарплата падает на %.0f%%, инфляция растёт, ПВЗ теряет выручку, "
           "казино затягивает сильнее. Длительность: %d мес.\n",
        crisis_salary_drop_percent, duration);
}

//  26. ОТЧЁТНОСТЬ

void alice_year_report(const int year, const int month)
{
    if (month != 12) return;

    printf("%d: доход = %lld, ндфл удержано = %lld, тбанк = %lld, втб = %lld, вклад = %lld, долг по ипотеке = %lld, зависимость = %.1f\n",
        year, alice_ndfl_state.income_year, alice_ndfl_state.withheld_year,
        alice.tbank.balance, alice.vtb.balance, alice_deposit_state.amount,
        alice_mortgage_state.remaining_debt,
        alice_casino_state.addiction_level);
}

//  27. ИНИЦИАЛИЗАЦИЯ И СИМУЛЯЦИЯ

void alice_init()
{
    alice.tbank.balance = 20'000;
    alice.salary = salary_for_level(alice_career_state.level);
    alice_crisis_init(start_year);
    alice_job_change_init();
}

void simulation()
{
    printf("Начало симуляции\n");

    int year = start_year;
    int month = 1;

    for (int i = 0; i < months_in_simulation; ++i)
    {
        alice_crisis_step(year, month);
        alice_inflation();
        alice_career();
        alice_try_change_job();
        alice_ndfl_year_close(month);
        alice_salary();
        alice_side_job(year, month);
        alice_bonus(month);
        alice_bank_interest();
        alice_deposit_interest();
        alice_deposit_maturity();
        alice_food();
        alice_utilities();
        alice_transport();
        alice_subscriptions();
        alice_mobile();
        alice_entertainment();
        alice_gifts(month);
        alice_vacation(year, month);
        alice_housing_rent();
        alice_checkup(month);
        alice_dentist(month);
        alice_illness(month);
        alice_fuel();
        alice_car_maintenance(month);
        alice_car_repair(year, month);
        alice_car_loan_payment();
        alice_pet_aging();
        alice_pet_death_check();
        alice_pet_food();
        alice_pet_toys();
        alice_pet_vet(month);
        alice_pet_illness();
        alice_relationship_step(year, month);
        alice_mortgage_payment();
        alice_mortgage_early_repayment(month);
        alice_apartment_renovation(year, month);
        alice_property_tax(month);
        alice_rent_income();
        alice_benefits();
        alice_random_income_event();
        alice_random_health_event();
        alice_random_career_event();
        alice_mortgage_early_repayment(month);

        alice_impulse_purchase();
        alice_casino();
        alice_pvz_monthly();
        alice_pvz_ip_contributions(month);

// cгруппировать машину и пет
        alice_try_open_deposit();
        alice_sweep_to_vtb();

        alice_pet_birth(year, month);
        alice_try_buy_car(year, month);
        alice_try_buy_apartment(year, month);
        alice_try_open_pvz(year, month);
        alice_pvz_monthly();
        alice_pvz_fire();
        alice_pvz_ip_contributions(month);

        alice_bankruptcy_check();

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
    printf("Вклад: %lld руб. (заморожено %d мес.)\n",
        alice_deposit_state.amount, alice_deposit_state.months_left);
    printf("Всего на счетах (без вклада): %lld руб.\n",
        alice.tbank.balance + alice.vtb.balance);
    printf("Всего с учётом вклада: %lld руб.\n",
        alice.tbank.balance + alice.vtb.balance + alice_deposit_state.amount);

    if (alice_mortgage_state.is_taken)
    {
        printf("Квартира куплена, остаток долга по ипотеке: %lld руб.\n", alice_mortgage_state.remaining_debt);
        printf("  досрочно погашено: %lld руб. за %d раз, срок сокращён на %d мес.\n",
            alice_mortgage_state.total_early_repaid,
            alice_mortgage_state.early_repay_count,
            alice_mortgage_state.term_shortened_months);
        printf("  потрачено на ремонт и обновления: %lld руб.\n",
            alice_mortgage_state.total_spent_renovation);
    }
    else
    {
        printf("Квартира ещё не куплена.\n");
    }

    if (alice_car_state.is_bought)
        printf("Машина куплена, остаток долга по автокредиту: %lld руб.\n", alice_car_state.remaining_debt);
    else
        printf("Машина ещё не куплена.\n");

    if (alice_pet_state.is_alive)
        printf("Питомец жив, возраст %d мес. (попыток завести: %d)\n",
            alice_pet_state.age_months, alice_pet_state.times_tried);
    else
        printf("Питомца нет (попыток завести: %d).\n", alice_pet_state.times_tried);

    printf("Питомец: корм %lld, ветеринар %lld, лечение %lld\n",
        alice_pet_state.total_spent_food,
        alice_pet_state.total_spent_vet,
        alice_pet_state.total_spent_illness);

    printf("Отпуска: всего %d, эконом %d, обычный %d, дорогой %d, пропущено %d, потрачено %lld руб.\n",
        alice_vacation_state.total_vacations,
        alice_vacation_state.economy_count,
        alice_vacation_state.normal_count,
        alice_vacation_state.luxury_count,
        alice_vacation_state.total_skipped,
        alice_vacation_state.total_spent);

    printf("Вклады: открыто %d, всего процентов получено %lld руб.\n",
        alice_deposit_state.total_deposits_opened,
        alice_deposit_state.total_interest_earned);

    if (alice_pvz_state.is_open)
    {
        printf("ПВЗ OZON (ИП): открыт, работал %d мес.\n", alice_pvz_state.months_open);
        printf("  выручка: %lld, УСН 6%%: %lld, страховзносы ИП: %lld, прочие расходы: %lld\n",
            alice_pvz_state.total_revenue,
            alice_pvz_state.total_usn_tax,
            alice_pvz_state.total_ip_contributions,
            alice_pvz_state.total_other_costs);
    }
    else
    {
        printf("ПВЗ OZON не открыт.\n");
    }

    if (alice_relationship_state.has_partner)
    {
        printf("Отношения: вместе %d мес., %s\n",
            alice_relationship_state.months_together,
            alice_relationship_state.is_married ? "в браке" : "не женаты");
    }
    else if (alice_relationship_state.is_divorced)
    {
        printf("Отношения: была замужем, развелась.\n");
        printf("  свидания: %lld руб., свадьба: %lld руб., развод и раздел: %lld руб.\n",
            alice_relationship_state.total_dates_spent,
            alice_relationship_state.wedding_cost,
            alice_relationship_state.divorce_cost);
    }
    else
    {
        printf("Отношения: не было.\n");
    }

    printf("Казино: выиграно %lld, проиграно %lld, уходила в завязку %d раз\n",
        alice_casino_state.total_won, alice_casino_state.total_lost, alice_casino_state.times_quit);

    printf("Банкротство: уходила в минус %d раз, самая долгая серия %d мес., "
           "макс. долг %lld руб., формально банкротилась %d раз\n",
        alice_bankruptcy_state.times_went_negative,
        alice_bankruptcy_state.max_months_in_negative,
        alice_bankruptcy_state.max_negative_balance,
        alice_bankruptcy_state.times_bankrupt);

    if (alice_bankruptcy_state.blocked_months_left > 0)
        printf("  (сейчас действует запрет на крупные решения, ещё %d мес.)\n",
            alice_bankruptcy_state.blocked_months_left);

    if (alice_bankruptcy_state.mark_years_left > 0)
        printf("  (метка банкрота: ещё %d лет кредиты дороже на 5 п.п.)\n",
            alice_bankruptcy_state.mark_years_left);

    printf("Мировой кризис: пережито %d, всего месяцев в кризисе %d\n",
        alice_crisis_state.total_crises,
        alice_crisis_state.months_in_crisis);

    printf("Карьера: уровень %d, зарплата %lld руб.\n",
        alice_career_state.level, alice.salary);
    printf("  смен работы: %d, всего прибавка от переходов: +%lld руб.\n",
        alice_career_state.total_job_changes,
        alice_career_state.total_salary_growth);
}

int main()
{
    g_rng.init((unsigned)time(NULL));

    alice_init();
    simulation();
    alice_print();

    return 0;
}