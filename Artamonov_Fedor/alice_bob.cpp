#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#endif

using Rub = double;
using Random_engine = std::mt19937;

constexpr int people_count = 2;
constexpr int alice_id = 0;
constexpr int bob_id = 1;

enum class Housing_strategy {
    rent_and_save,
    mortgage_regular,
    mortgage_early_repayment
};

constexpr int strategy_count = 3;

enum class Loan_kind {
    mortgage,
    car_loan
};

enum class Price_category {
    food, utilities, rent, fuel, car, pet, services, goods, transport, real_estate, medicine, gold
};

constexpr int price_category_count = 12;

enum class Money_flow {
    salary, sick_pay, vacation_pay, severance, unemployment_benefit, freelance, deposit_interest, tax_refund, gold_sale,
    food, rent, utilities, communication, mortgage_payment, car_loan_payment, credit_card_interest, bank_fees,
    car, public_transport, dog, health, vacations, household, gifts, taxes, hobby, charity,
    early_repayment, home_purchase, car_purchase, gold_purchase
};

constexpr int money_flow_count = 31;
constexpr int last_income_flow = static_cast<int>(Money_flow::gold_sale);
constexpr int first_asset_flow = static_cast<int>(Money_flow::early_repayment);

using Flow_table = std::array<Rub, money_flow_count>;


namespace simulation_rules {

constexpr int start_year = 2026;
constexpr int start_month = 9;
constexpr int min_months = 121;
constexpr int max_months = 360;
constexpr int default_runs = 300;
constexpr unsigned default_seed = 2026;
constexpr int max_log_lines = 70;

}

namespace family_start {

constexpr Rub alice_salary = 80'000;
constexpr Rub bob_salary = 120'000;
constexpr Rub alice_savings = 20'000;
constexpr Rub bob_savings = 1'450'000;

constexpr int alice_promotion_year = 2027;
constexpr int alice_promotion_month = 1;
constexpr Rub alice_promotion_salary = 100'000;

constexpr int dog_age_months = 48;
constexpr int dog_min_life_months = 132;
constexpr int dog_max_life_months = 180;

constexpr int car_production_year = 2017;
constexpr Rub car_value = 1'050'000;
constexpr int car_power_hp = 123;
constexpr double car_fuel_per_100km = 7.8;
constexpr int car_km_per_month = 1'100;
constexpr int car_insurance_month = 3;

}

namespace spb_prices {

constexpr Rub food_per_adult = 20'000;
constexpr double food_share_during_trip = 0.65;

constexpr Rub household_goods = 4'500;
constexpr Rub clothes_per_adult = 3'500;
constexpr Rub entertainment = 8'000;
constexpr double entertainment_share_when_unemployed = 0.4;

constexpr Rub internet_and_mobile = 2'400;
constexpr Rub utilities_owner = 6'800;
constexpr Rub utilities_tenant = 4'200;

constexpr Rub rent_studio = 38'000;
constexpr int lease_anniversary_month = 3;

constexpr double flat_area_m2 = 28.0;
constexpr Rub flat_price_per_m2 = 185'000;
constexpr double cadastral_share = 0.8;
constexpr Rub deal_costs = 90'000;
constexpr Rub moving_cost = 15'000;

constexpr Rub metro_ride = 90;
constexpr int work_rides_per_month = 44;
constexpr int leisure_rides_per_month = 10;

constexpr Rub fuel_liter = 66;
constexpr Rub parking_and_wash = 2'500;
constexpr Rub tyre_change = 3'500;
constexpr Rub car_service = 35'000;
constexpr double car_service_growth_per_year_of_age = 0.03;
constexpr Rub osago_year = 9'500;
constexpr Rub car_registration = 6'000;
constexpr Rub new_car_price = 2'300'000;
constexpr int new_car_power_hp = 150;
constexpr double new_car_fuel_per_100km = 8.2;
constexpr int car_replace_age = 14;
constexpr int car_replace_month = 4;
constexpr double car_depreciation_year = 0.08;
constexpr double car_minimum_usage = 0.4;

constexpr Rub dog_food = 4'200;
constexpr Rub dog_grooming = 2'500;
constexpr Rub dog_vaccination = 6'500;
constexpr Rub dog_funeral = 9'000;

constexpr Rub dentist_per_adult = 16'000;
constexpr int alice_dentist_month = 2;
constexpr int bob_dentist_month = 11;

constexpr Rub summer_trip = 110'000;
constexpr Rub vacation_leisure_per_person = 10'000;
constexpr Rub new_year_gifts = 25'000;
constexpr Rub birthday_gift = 7'000;
constexpr int alice_birthday_month = 3;
constexpr int bob_birthday_month = 7;

}

namespace tax_rules {

struct Ndfl_bracket {
    Rub upper_limit;
    double rate;
};

constexpr std::array<Ndfl_bracket, 5> ndfl_scale = {{
    {2'400'000, 0.13},
    {5'000'000, 0.15},
    {20'000'000, 0.18},
    {50'000'000, 0.20},
    {1e15, 0.22},
}};

constexpr double npd_rate_individuals = 0.04;
constexpr double deduction_rate = 0.13;
constexpr Rub property_deduction_limit = 2'000'000;
constexpr Rub mortgage_interest_deduction_limit = 3'000'000;
constexpr Rub social_deduction_limit = 150'000;
constexpr int refund_arrival_month = 5;

constexpr Rub deposit_tax_free_base = 1'000'000;
constexpr double deposit_interest_tax = 0.13;
constexpr int annual_tax_payment_month = 12;

constexpr double property_tax_rate = 0.001;
constexpr double tax_free_m2_per_owner = 20.0;

struct Transport_tax_step {
    int max_power_hp;
    double rate_per_hp;
};

constexpr std::array<Transport_tax_step, 6> transport_tax_scale = {{
    {100, 24}, {125, 35}, {150, 50}, {175, 55}, {200, 65}, {1000, 75},
}};

}

namespace social_rules {

constexpr double sick_pay_share = 0.8;
constexpr Rub max_daily_sick_base = 6'827;
constexpr int sick_base_days = 730;
constexpr double average_days_in_month = 29.3;
constexpr int vacation_part_days = 14;
constexpr Rub unemployment_benefit = 16'000;

}

namespace bank_rules {

constexpr double start_key_rate = 0.15;
constexpr double long_term_key_rate = 0.085;
constexpr double min_key_rate = 0.05;
constexpr double max_key_rate = 0.25;
constexpr double key_rate_step = 0.0025;

constexpr double deposit_spread = -0.015;
constexpr double mortgage_spread = 0.04;
constexpr double car_loan_spread = 0.07;

constexpr double credit_card_rate = 0.34;
constexpr Rub credit_card_limit = 300'000;
constexpr int credit_card_grace_months = 3;

constexpr double min_down_payment_share = 0.2;
constexpr int mortgage_months = 120;
constexpr int car_loan_months = 60;
constexpr double mortgage_insurance_rate = 0.004;

constexpr double refinance_min_gain = 0.02;
constexpr int refinance_min_months_left = 24;
constexpr int months_between_refinances = 12;
constexpr Rub refinance_costs = 30'000;

constexpr Rub min_early_repayment = 10'000;
constexpr Rub min_car_loan = 150'000;
constexpr double car_trade_in_share = 0.9;

constexpr Rub cash_cushion = 40'000;
constexpr Rub start_reserve = 250'000;
constexpr int emergency_fund_months = 3;
constexpr Rub start_monthly_spending = 150'000;
constexpr double overdue_penalty_monthly = 0.02;

}

namespace world_rules {

constexpr double key_rate_reversion = 0.025;
constexpr double key_rate_volatility = 0.003;
constexpr double key_rate_shock_chance = 0.015;
constexpr double key_rate_shock_min = 0.01;
constexpr double key_rate_shock_max = 0.03;

constexpr double wage_indexation_mean = 0.065;
constexpr double wage_indexation_deviation = 0.02;
constexpr double promotion_chance = 0.18;
constexpr double promotion_min = 0.08;
constexpr double promotion_max = 0.20;
constexpr int first_random_promotion_year = 2028;

constexpr double layoff_chance_per_month = 0.002;
constexpr int max_unemployed_months = 6;
constexpr double new_job_min_factor = 0.95;
constexpr double new_job_max_factor = 1.15;

constexpr double sickness_chance_winter = 0.11;
constexpr double sickness_chance_summer = 0.04;
constexpr double sickness_chance_other = 0.07;
constexpr double serious_illness_share = 0.06;

constexpr double freelance_chance = 0.30;
constexpr double freelance_chance_unemployed = 0.60;

constexpr double dog_vet_chance = 0.05;
constexpr double old_dog_vet_chance = 0.10;
constexpr int old_dog_age_months = 108;

constexpr double car_breakdown_base_chance = 0.015;
constexpr double car_breakdown_chance_per_year = 0.002;
constexpr double appliance_breakdown_chance = 0.015;

}

namespace gold_rules {

constexpr Rub price_per_gram = 9'500;
constexpr double bar_weight_grams = 10.0;
constexpr double bank_margin = 0.06;
constexpr double sell_discount = 0.08;
constexpr Rub min_free_money_to_buy = 400'000;

}

namespace hobby_rules {

constexpr Rub bicycle_price = 65'000;
constexpr Rub concert_tickets = 12'000;
constexpr Rub charity_donation = 5'000;
constexpr Rub min_free_money_for_bicycle = 200'000;
constexpr Rub min_free_money_for_concert = 80'000;

}

namespace spontaneous_rules {

constexpr int max_actions = 8;
constexpr double gold_bar_chance = 0.012;
constexpr double bicycle_chance = 0.004;
constexpr double concert_chance = 0.030;
constexpr double charity_chance = 0.050;

}

namespace csv_rules {

constexpr char separator = ';';
constexpr const char* byte_order_mark = "\xEF\xBB\xBF";
constexpr const char* default_folder = "alice_bob_csv";

}


struct Sim_date {
    int year = 0;
    int month = 0;
};

struct Month_day {
    int month = 0;
    int day = 0;
};

constexpr std::array<Month_day, 6> fixed_holidays = {{
    {2, 23}, {3, 8}, {5, 1}, {5, 9}, {6, 12}, {11, 4},
}};

struct Inflation_profile {
    Price_category category = Price_category::food;
    double mean_annual = 0.0;
    double deviation = 0.0;
};

constexpr std::array<Inflation_profile, price_category_count> inflation_profiles = {{
    {Price_category::food, 0.080, 0.025},
    {Price_category::utilities, 0.095, 0.020},
    {Price_category::rent, 0.070, 0.030},
    {Price_category::fuel, 0.070, 0.025},
    {Price_category::car, 0.060, 0.030},
    {Price_category::pet, 0.080, 0.020},
    {Price_category::services, 0.090, 0.020},
    {Price_category::goods, 0.055, 0.020},
    {Price_category::transport, 0.100, 0.030},
    {Price_category::real_estate, 0.060, 0.040},
    {Price_category::medicine, 0.085, 0.020},
    {Price_category::gold, 0.090, 0.060},
}};

struct Person_events {
    bool employed = true;
    bool laid_off = false;
    bool found_new_job = false;
    double new_job_salary_factor = 1.0;
    Rub fixed_new_salary = 0.0;
    double promotion_raise = 0.0;
    int sick_days = 0;
    bool serious_illness = false;
    Rub treatment_cost = 0.0;
    int vacation_days = 0;
    Rub freelance_gross = 0.0;
};

struct World_month {
    Sim_date date;
    int calendar_days = 30;
    int working_days = 21;

    double key_rate = 0.0;
    double wage_level = 1.0;
    double wage_indexation = 0.0;
    std::array<double, price_category_count> price_level{};

    std::array<Person_events, people_count> people{};
    bool summer_vacation = false;

    bool dog_alive = true;
    bool dog_died = false;
    int dog_age_months = 0;
    Rub dog_vet_cost = 0.0;

    double car_breakdown_roll = 1.0;
    Rub car_repair_base = 0.0;
    Rub appliance_cost = 0.0;

    std::array<double, spontaneous_rules::max_actions> action_rolls{};
};

struct World_timeline {
    unsigned seed = 0;
    std::vector<World_month> months;
};

struct World_generator {
    Random_engine rng;

    double key_rate_raw = bank_rules::start_key_rate;
    double wage_level = 1.0;
    std::array<double, price_category_count> price_level{};
    std::array<double, price_category_count> annual_inflation{};

    int summer_month = 0;
    std::array<int, people_count> own_vacation_month{};
    std::array<int, people_count> unemployed_months_left{};
    std::array<bool, people_count> returning_to_work{};

    int dog_age_months = 0;
    int dog_death_age_months = 0;
    bool dog_alive = true;
};


int to_index(Price_category category) {
    return static_cast<int>(category);
}

int to_index(Money_flow flow) {
    return static_cast<int>(flow);
}

bool is_income_flow(Money_flow flow) {
    return to_index(flow) <= last_income_flow;
}

bool is_asset_flow(Money_flow flow) {
    return to_index(flow) >= first_asset_flow;
}

Sim_date make_date(int year, int month) {
    Sim_date date;
    date.year = year;
    date.month = month;
    return date;
}

Sim_date add_months(Sim_date date, int months) {
    const int total_months = date.year * 12 + (date.month - 1) + months;
    return make_date(total_months / 12, total_months % 12 + 1);
}

int months_between(Sim_date from, Sim_date to) {
    return (to.year * 12 + to.month) - (from.year * 12 + from.month);
}

int days_in_month(int year, int month) {
    const std::chrono::year_month_day_last last_day{
        std::chrono::year{year} / std::chrono::month{static_cast<unsigned>(month)} / std::chrono::last};

    return static_cast<int>(static_cast<unsigned>(last_day.day()));
}

bool is_weekend(int year, int month, int day) {
    const std::chrono::year_month_day date{
        std::chrono::year{year},
        std::chrono::month{static_cast<unsigned>(month)},
        std::chrono::day{static_cast<unsigned>(day)}};

    const std::chrono::weekday week_day{std::chrono::sys_days{date}};

    return week_day == std::chrono::Saturday || week_day == std::chrono::Sunday;
}

bool is_public_holiday(int month, int day) {
    const bool new_year_holidays = month == 1 && day <= 8;
    if (new_year_holidays) {
        return true;
    }

    for (const Month_day& holiday : fixed_holidays) {
        if (holiday.month == month && holiday.day == day) {
            return true;
        }
    }

    return false;
}

int count_working_days(int year, int month) {
    const int total_days = days_in_month(year, month);
    int working_days = 0;

    for (int day = 1; day <= total_days; ++day) {
        const bool day_off = is_weekend(year, month, day) || is_public_holiday(month, day);
        if (!day_off) {
            ++working_days;
        }
    }

    return working_days;
}

bool roll_chance(Random_engine& rng, double probability) {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(rng) < probability;
}

int random_int(Random_engine& rng, int low, int high) {
    std::uniform_int_distribution<int> distribution(low, high);
    return distribution(rng);
}

double random_real(Random_engine& rng, double low, double high) {
    std::uniform_real_distribution<double> distribution(low, high);
    return distribution(rng);
}

double random_normal(Random_engine& rng, double mean, double deviation) {
    std::normal_distribution<double> distribution(mean, deviation);
    return distribution(rng);
}

double price_level(const World_month& month, Price_category category) {
    return month.price_level[to_index(category)];
}

double general_price_level(const World_month& month) {
    const double food_part = 0.35 * price_level(month, Price_category::food);
    const double services_part = 0.25 * price_level(month, Price_category::services);
    const double goods_part = 0.25 * price_level(month, Price_category::goods);
    const double utilities_part = 0.15 * price_level(month, Price_category::utilities);

    return food_part + services_part + goods_part + utilities_part;
}

Rub gold_price_per_gram(const World_month& month) {
    return gold_rules::price_per_gram * price_level(month, Price_category::gold);
}

void roll_annual_inflation(World_generator& generator) {
    for (const Inflation_profile& profile : inflation_profiles) {
        const double rate = random_normal(generator.rng, profile.mean_annual, profile.deviation);
        generator.annual_inflation[to_index(profile.category)] = std::clamp(rate, -0.02, 0.25);
    }
}

bool is_indexed_once_a_year(Price_category category) {
    return category == Price_category::utilities || category == Price_category::transport;
}

int indexation_month(Price_category category) {
    const int utilities_indexation_month = 10;
    const int other_indexation_month = 1;

    return category == Price_category::utilities ? utilities_indexation_month : other_indexation_month;
}

void update_price_levels(World_generator& generator, int month) {
    for (int i = 0; i < price_category_count; ++i) {
        const Price_category category = static_cast<Price_category>(i);
        const double annual_rate = generator.annual_inflation[i];

        if (is_indexed_once_a_year(category)) {
            if (month == indexation_month(category)) {
                generator.price_level[i] *= 1.0 + annual_rate;
            }
        } else {
            generator.price_level[i] *= std::pow(1.0 + annual_rate, 1.0 / 12.0);
        }
    }
}

double round_key_rate(double rate) {
    const double rounded_rate = std::round(rate / bank_rules::key_rate_step) * bank_rules::key_rate_step;
    return std::clamp(rounded_rate, bank_rules::min_key_rate, bank_rules::max_key_rate);
}

void update_key_rate(World_generator& generator) {
    const double distance_to_target = bank_rules::long_term_key_rate - generator.key_rate_raw;

    double change = distance_to_target * world_rules::key_rate_reversion;
    change += random_normal(generator.rng, 0.0, world_rules::key_rate_volatility);

    if (roll_chance(generator.rng, world_rules::key_rate_shock_chance)) {
        change += random_real(generator.rng, world_rules::key_rate_shock_min, world_rules::key_rate_shock_max);
    }

    generator.key_rate_raw = std::clamp(generator.key_rate_raw + change,
                                        bank_rules::min_key_rate,
                                        bank_rules::max_key_rate);
}

double roll_wage_indexation(World_generator& generator) {
    const double raw_raise = random_normal(generator.rng,
                                           world_rules::wage_indexation_mean,
                                           world_rules::wage_indexation_deviation);
    const double raise = std::clamp(raw_raise, 0.0, 0.2);

    generator.wage_level *= 1.0 + raise;

    return raise;
}

void plan_first_vacations(World_generator& generator) {
    const int first_autumn_month = 10;
    const int last_autumn_month = 12;

    generator.summer_month = 0;

    for (int person = 0; person < people_count; ++person) {
        generator.own_vacation_month[person] = random_int(generator.rng, first_autumn_month, last_autumn_month);
    }
}

void plan_year_vacations(World_generator& generator) {
    const int first_summer_month = 6;
    const int last_summer_month = 8;

    generator.summer_month = random_int(generator.rng, first_summer_month, last_summer_month);

    for (int person = 0; person < people_count; ++person) {
        int own_month = random_int(generator.rng, 1, 12);

        while (own_month == generator.summer_month) {
            own_month = random_int(generator.rng, 1, 12);
        }

        generator.own_vacation_month[person] = own_month;
    }
}

void roll_employment(World_generator& generator, Person_events& events, int person) {
    if (generator.returning_to_work[person]) {
        generator.returning_to_work[person] = false;
        events.found_new_job = true;
        events.new_job_salary_factor = random_real(generator.rng,
                                                   world_rules::new_job_min_factor,
                                                   world_rules::new_job_max_factor);
        return;
    }

    if (generator.unemployed_months_left[person] > 0) {
        events.employed = false;
        --generator.unemployed_months_left[person];

        if (generator.unemployed_months_left[person] == 0) {
            generator.returning_to_work[person] = true;
        }
        return;
    }

    if (roll_chance(generator.rng, world_rules::layoff_chance_per_month)) {
        events.employed = false;
        events.laid_off = true;
        generator.unemployed_months_left[person] = random_int(generator.rng, 1, world_rules::max_unemployed_months);
    }
}

void roll_career(World_generator& generator, Person_events& events, int person, Sim_date date) {
    const bool alice_promotion_date = date.year == family_start::alice_promotion_year
                                   && date.month == family_start::alice_promotion_month;

    if (person == alice_id && alice_promotion_date) {
        events.fixed_new_salary = family_start::alice_promotion_salary;
        return;
    }

    const bool promotion_season = date.month == 1 && date.year >= world_rules::first_random_promotion_year;
    if (!promotion_season) {
        return;
    }

    if (roll_chance(generator.rng, world_rules::promotion_chance)) {
        events.promotion_raise = random_real(generator.rng, world_rules::promotion_min, world_rules::promotion_max);
    }
}

void roll_vacation_days(const World_generator& generator, Person_events& events, int person, int month) {
    if (month == generator.summer_month) {
        events.vacation_days += social_rules::vacation_part_days;
    }

    if (month == generator.own_vacation_month[person]) {
        events.vacation_days += social_rules::vacation_part_days;
    }
}

double sickness_chance(int month) {
    const bool cold_season = month >= 11 || month <= 3;
    const bool warm_season = month >= 6 && month <= 8;

    if (cold_season) {
        return world_rules::sickness_chance_winter;
    }

    if (warm_season) {
        return world_rules::sickness_chance_summer;
    }

    return world_rules::sickness_chance_other;
}

void roll_sickness(World_generator& generator, Person_events& events, int month) {
    if (!roll_chance(generator.rng, sickness_chance(month))) {
        return;
    }

    const double medicine_level = generator.price_level[to_index(Price_category::medicine)];

    if (roll_chance(generator.rng, world_rules::serious_illness_share)) {
        events.serious_illness = true;
        events.sick_days = random_int(generator.rng, 14, 30);
        events.treatment_cost = random_real(generator.rng, 20'000, 80'000) * medicine_level;
    } else {
        events.sick_days = random_int(generator.rng, 4, 12);
        events.treatment_cost = random_real(generator.rng, 1'500, 7'000) * medicine_level;
    }
}

void roll_freelance(World_generator& generator, Person_events& events) {
    const double chance = events.employed ? world_rules::freelance_chance
                                          : world_rules::freelance_chance_unemployed;

    if (!roll_chance(generator.rng, chance)) {
        return;
    }

    const double services_level = generator.price_level[to_index(Price_category::services)];

    if (events.employed) {
        events.freelance_gross = random_real(generator.rng, 8'000, 25'000) * services_level;
    } else {
        events.freelance_gross = random_real(generator.rng, 15'000, 45'000) * services_level;
    }
}

void generate_person_month(World_generator& generator, World_month& month, int person) {
    Person_events& events = month.people[person];

    roll_employment(generator, events, person);

    if (events.employed) {
        roll_career(generator, events, person, month.date);
        roll_vacation_days(generator, events, person, month.date.month);
    }

    roll_sickness(generator, events, month.date.month);

    if (!events.employed) {
        events.sick_days = 0;
    }

    const bool absent_too_long = events.sick_days + events.vacation_days > month.calendar_days;
    if (absent_too_long) {
        events.vacation_days = std::max(0, month.calendar_days - events.sick_days);
    }

    if (person == bob_id) {
        roll_freelance(generator, events);
    }
}

void roll_dog_month(World_generator& generator, World_month& month) {
    if (!generator.dog_alive) {
        month.dog_alive = false;
        return;
    }

    ++generator.dog_age_months;
    month.dog_age_months = generator.dog_age_months;

    const bool dog_died_of_old_age = generator.dog_age_months >= generator.dog_death_age_months;
    if (dog_died_of_old_age) {
        generator.dog_alive = false;
        month.dog_alive = false;
        month.dog_died = true;
        return;
    }

    const bool old_dog = generator.dog_age_months > world_rules::old_dog_age_months;
    const double vet_chance = old_dog ? world_rules::old_dog_vet_chance : world_rules::dog_vet_chance;

    if (roll_chance(generator.rng, vet_chance)) {
        const double min_bill = old_dog ? 5'000 : 3'000;
        const double max_bill = old_dog ? 35'000 : 18'000;
        const double pet_level = generator.price_level[to_index(Price_category::pet)];

        month.dog_vet_cost = random_real(generator.rng, min_bill, max_bill) * pet_level;
    }
}

void roll_household_accidents(World_generator& generator, World_month& month) {
    const double car_level = generator.price_level[to_index(Price_category::car)];
    const double goods_level = generator.price_level[to_index(Price_category::goods)];

    month.car_breakdown_roll = random_real(generator.rng, 0.0, 1.0);
    month.car_repair_base = random_real(generator.rng, 8'000, 45'000) * car_level;

    if (roll_chance(generator.rng, world_rules::appliance_breakdown_chance)) {
        month.appliance_cost = random_real(generator.rng, 12'000, 65'000) * goods_level;
    }
}

void roll_spontaneous_actions(World_generator& generator, World_month& month) {
    for (double& roll : month.action_rolls) {
        roll = random_real(generator.rng, 0.0, 1.0);
    }
}

World_generator create_world_generator(unsigned seed) {
    World_generator generator;

    generator.rng.seed(seed);
    generator.price_level.fill(1.0);
    generator.dog_age_months = family_start::dog_age_months;
    generator.dog_death_age_months = random_int(generator.rng,
                                                family_start::dog_min_life_months,
                                                family_start::dog_max_life_months);

    roll_annual_inflation(generator);
    plan_first_vacations(generator);

    return generator;
}

World_month generate_month(World_generator& generator, Sim_date date, bool first_month) {
    World_month month;

    month.date = date;
    month.calendar_days = days_in_month(date.year, date.month);
    month.working_days = count_working_days(date.year, date.month);

    const bool new_year_started = !first_month && date.month == 1;
    if (new_year_started) {
        roll_annual_inflation(generator);
        month.wage_indexation = roll_wage_indexation(generator);
        plan_year_vacations(generator);
    }

    if (!first_month) {
        update_price_levels(generator, date.month);
        update_key_rate(generator);
    }

    month.key_rate = round_key_rate(generator.key_rate_raw);
    month.wage_level = generator.wage_level;
    month.price_level = generator.price_level;
    month.summer_vacation = date.month == generator.summer_month;

    for (int person = 0; person < people_count; ++person) {
        generate_person_month(generator, month, person);
    }

    roll_dog_month(generator, month);
    roll_household_accidents(generator, month);
    roll_spontaneous_actions(generator, month);

    return month;
}

World_timeline generate_world(unsigned seed, int months_count) {
    World_timeline world;
    world.seed = seed;
    world.months.reserve(months_count);

    World_generator generator = create_world_generator(seed);
    Sim_date date = make_date(simulation_rules::start_year, simulation_rules::start_month);

    for (int index = 0; index < months_count; ++index) {
        const bool first_month = index == 0;

        world.months.push_back(generate_month(generator, date, first_month));
        date = add_months(date, 1);
    }

    return world;
}


struct Worker {
    std::string name;
    Rub gross_salary = 0.0;
    std::array<Rub, 24> gross_history{};
    int history_position = 0;

    Rub year_taxable_income = 0.0;
    Rub year_ndfl_paid = 0.0;
    Rub year_medical_expenses = 0.0;
    Rub year_mortgage_interest = 0.0;

    Rub property_base_left = 0.0;
    Rub property_deduction_left = tax_rules::property_deduction_limit;
    Rub interest_deduction_left = tax_rules::mortgage_interest_deduction_limit;
    Rub interest_carry_over = 0.0;
};

struct Loan {
    Loan_kind kind = Loan_kind::mortgage;
    std::string title;
    Rub start_amount = 0.0;
    Rub balance = 0.0;
    double annual_rate = 0.0;
    int months_left = 0;
    Rub monthly_payment = 0.0;
    Rub interest_paid = 0.0;
    Rub early_repaid = 0.0;
    Sim_date opened;
    Sim_date last_rate_change;
    bool active = false;
};

struct Credit_card {
    Rub limit = 0.0;
    Rub debt = 0.0;
    double annual_rate = 0.0;
    int grace_months_left = 0;
    Rub interest_paid = 0.0;
    int months_in_debt = 0;
};

struct Savings_account {
    Rub balance = 0.0;
    Rub interest_year = 0.0;
    Rub interest_total = 0.0;
    double max_key_rate_year = 0.0;
    Rub tax_due = 0.0;
};

struct Family_car {
    bool owned = false;
    std::string model;
    int production_year = 0;
    Rub market_value = 0.0;
    int power_hp = 0;
    double fuel_per_100km = 0.0;
    int km_per_month = 0;
    int insurance_month = 1;
};

struct Home {
    bool owned = false;
    double area_m2 = 0.0;
    Rub purchase_price = 0.0;
    double price_level_at_purchase = 1.0;
    Sim_date bought;
};

struct Rental {
    bool active = false;
    Rub monthly_rent = 0.0;
    Rub security_deposit = 0.0;
    int anniversary_month = 1;
};

struct Pet {
    bool alive = false;
    std::string name;
};

struct Gold_stock {
    double grams = 0.0;
    Rub money_spent = 0.0;
    int bars_bought = 0;
};

struct Pending_refund {
    std::string owner;
    Rub amount = 0.0;
    Sim_date arrival;
};

struct Month_row {
    Sim_date date;
    Rub income = 0.0;
    Rub spending = 0.0;
    Rub liquid = 0.0;
    Rub card_debt = 0.0;
    Rub loans = 0.0;
    Rub net_worth = 0.0;
};

struct Year_snapshot {
    int year = 0;
    Rub income = 0.0;
    Rub spending = 0.0;
    Rub liquid = 0.0;
    Rub debts = 0.0;
    Rub net_worth = 0.0;
    double key_rate = 0.0;
};

struct Family {
    Housing_strategy strategy = Housing_strategy::rent_and_save;
    bool keep_history = false;

    std::array<Worker, people_count> workers;
    Rub cash = 0.0;
    Savings_account savings;
    Credit_card card;
    std::vector<Loan> loans;

    Family_car car;
    Home flat;
    Rental lease;
    Pet dog;
    Gold_stock gold;
    std::vector<Pending_refund> refunds;

    Flow_table month_flows{};
    Flow_table year_flows{};
    Flow_table total_flows{};

    Rub typical_monthly_spending = bank_rules::start_monthly_spending;
    Rub tax_refunds_received = 0.0;
    bool on_trip = false;
    int overdue_months = 0;
    Rub overdue_penalties = 0.0;
    int months_simulated = 0;
    int month_flat_bought = -1;
    int month_mortgage_closed = -1;

    std::vector<std::string> log;
    std::vector<Month_row> monthly_rows;
    std::vector<Year_snapshot> yearly;
};

struct Run_result {
    Housing_strategy strategy = Housing_strategy::rent_and_save;
    Rub net_worth = 0.0;
    Rub real_net_worth = 0.0;
    Rub liquid = 0.0;
    Rub debts = 0.0;
    Rub interest_paid = 0.0;
    Rub tax_refunds = 0.0;
    Rub gold_value = 0.0;
    int card_debt_months = 0;
    int overdue_months = 0;
    int month_flat_bought = -1;
    int month_mortgage_closed = -1;
    int months = 0;
};

struct Strategy_statistics {
    Housing_strategy strategy = Housing_strategy::rent_and_save;
    std::vector<Rub> real_net_worths;
    Rub interest_sum = 0.0;
    Rub refund_sum = 0.0;
    long long card_debt_months_sum = 0;
    int runs_with_overdue = 0;
    int wins = 0;
    int runs_with_flat = 0;
    long long months_to_flat_sum = 0;
};

struct Program_options {
    unsigned seed = simulation_rules::default_seed;
    int runs = simulation_rules::default_runs;
    bool monthly_table = false;
    bool open_folder = true;
    std::string csv_folder = csv_rules::default_folder;
};


constexpr std::array<const char*, strategy_count> strategy_names = {
    "аренда + накопления",
    "ипотека по графику",
    "ипотека + досрочка",
};

constexpr std::array<const char*, money_flow_count> flow_names = {
    "зарплата", "больничные", "отпускные", "выходное пособие", "пособие по безработице",
    "подработка (репетиторство)", "проценты по вкладу", "возврат налогов", "продажа золота",
    "еда", "аренда", "коммунальные услуги", "интернет и связь", "платежи по ипотеке",
    "платежи по автокредиту", "проценты по кредитке", "страховки, комиссии, штрафы",
    "содержание машины", "общественный транспорт", "собака", "здоровье", "отпуск",
    "быт, одежда, досуг", "подарки", "налоги (транспорт, имущество, вклад)", "хобби", "благотворительность",
    "досрочное погашение", "покупка квартиры (свои деньги)", "покупка машины (свои деньги)", "покупка золота",
};

const char* strategy_name(Housing_strategy strategy) {
    return strategy_names[static_cast<int>(strategy)];
}

const char* flow_name(Money_flow flow) {
    return flow_names[to_index(flow)];
}

std::string format_date(Sim_date date) {
    return std::format("{:04}-{:02}", date.year, date.month);
}

std::string format_money(Rub value) {
    long long amount = std::llround(value);
    const bool negative = amount < 0;

    if (negative) {
        amount = -amount;
    }

    const std::string digits = std::to_string(amount);
    std::string result;
    int digits_in_group = 0;

    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
        result.insert(result.begin(), digits[i]);
        ++digits_in_group;

        const bool need_space = digits_in_group % 3 == 0 && i > 0;
        if (need_space) {
            result.insert(result.begin(), ' ');
        }
    }

    if (negative) {
        result.insert(result.begin(), '-');
    }

    return result;
}

std::string format_percent(double value) {
    std::string result = std::format("{:.2f}%", value * 100.0);
    std::replace(result.begin(), result.end(), '.', ',');
    return result;
}

std::string format_number(double value) {
    std::string result = std::format("{:.1f}", value);
    std::replace(result.begin(), result.end(), '.', ',');
    return result;
}

void add_log(Family& family, Sim_date date, const std::string& text) {
    if (!family.keep_history) {
        return;
    }

    family.log.push_back(format_date(date) + "  " + text);
}

void record_flow(Family& family, Money_flow flow, Rub amount) {
    family.month_flows[to_index(flow)] += amount;
}

void receive(Family& family, Money_flow flow, Rub amount) {
    if (amount <= 0.0) {
        return;
    }

    family.cash += amount;
    record_flow(family, flow, amount);
}

void spend(Family& family, Money_flow flow, Rub amount) {
    if (amount <= 0.0) {
        return;
    }

    family.cash -= amount;
    record_flow(family, flow, amount);
}

Rub sum_income(const Flow_table& flows) {
    Rub total = 0.0;

    for (int i = 0; i <= last_income_flow; ++i) {
        total += flows[i];
    }

    return total;
}

Rub sum_spending(const Flow_table& flows) {
    Rub total = 0.0;

    for (int i = last_income_flow + 1; i < first_asset_flow; ++i) {
        total += flows[i];
    }

    return total;
}

double deposit_rate(double key_rate) {
    return std::max(0.01, key_rate + bank_rules::deposit_spread);
}

double mortgage_rate(double key_rate) {
    return key_rate + bank_rules::mortgage_spread;
}

double car_loan_rate(double key_rate) {
    return key_rate + bank_rules::car_loan_spread;
}

Rub annuity_payment(Rub principal, double annual_rate, int months) {
    if (months <= 0) {
        return principal;
    }

    const double monthly_rate = annual_rate / 12.0;
    if (monthly_rate <= 0.0) {
        return principal / months;
    }

    const double growth = std::pow(1.0 + monthly_rate, months);

    return principal * monthly_rate * growth / (growth - 1.0);
}

int months_to_repay(Rub principal, double annual_rate, Rub payment) {
    if (principal <= 0.0) {
        return 0;
    }

    const double monthly_rate = annual_rate / 12.0;
    if (monthly_rate <= 0.0) {
        return static_cast<int>(std::ceil(principal / payment));
    }

    const double monthly_interest = principal * monthly_rate;
    if (payment <= monthly_interest) {
        return simulation_rules::max_months;
    }

    const double months = -std::log(1.0 - monthly_interest / payment) / std::log(1.0 + monthly_rate);

    return std::max(1, static_cast<int>(std::ceil(months - 1e-9)));
}

Rub flat_market_price(const World_month& month) {
    return spb_prices::flat_area_m2
         * spb_prices::flat_price_per_m2
         * price_level(month, Price_category::real_estate);
}

Loan* find_active_loan(Family& family, Loan_kind kind) {
    for (Loan& loan : family.loans) {
        if (loan.active && loan.kind == kind) {
            return &loan;
        }
    }

    return nullptr;
}

bool has_active_loan(const Family& family, Loan_kind kind) {
    for (const Loan& loan : family.loans) {
        if (loan.active && loan.kind == kind) {
            return true;
        }
    }

    return false;
}

Rub total_loan_balance(const Family& family) {
    Rub total = 0.0;

    for (const Loan& loan : family.loans) {
        if (loan.active) {
            total += loan.balance;
        }
    }

    return total;
}

Rub total_interest_paid(const Family& family) {
    Rub total = family.card.interest_paid;

    for (const Loan& loan : family.loans) {
        total += loan.interest_paid;
    }

    return total;
}

Rub liquid_money(const Family& family) {
    return family.cash + family.savings.balance - family.card.debt;
}

Rub total_debts(const Family& family) {
    Rub debts = total_loan_balance(family) + family.card.debt + family.savings.tax_due;

    if (family.cash < 0.0) {
        debts -= family.cash;
    }

    return debts;
}

Rub pending_refunds_sum(const Family& family) {
    Rub total = 0.0;

    for (const Pending_refund& refund : family.refunds) {
        total += refund.amount;
    }

    return total;
}

Rub gold_market_value(const Family& family, const World_month& month) {
    return family.gold.grams * gold_price_per_gram(month) * (1.0 - gold_rules::sell_discount);
}

Rub net_worth(const Family& family, const World_month& month) {
    Rub assets = std::max(0.0, family.cash) + family.savings.balance + pending_refunds_sum(family);

    assets += gold_market_value(family, month);

    if (family.flat.owned) {
        assets += flat_market_price(month);
    }

    if (family.car.owned) {
        assets += family.car.market_value;
    }

    if (family.lease.active) {
        assets += family.lease.security_deposit;
    }

    return assets - total_debts(family);
}

Rub emergency_reserve(const Family& family) {
    return family.typical_monthly_spending * bank_rules::emergency_fund_months;
}

Rub free_money(const Family& family) {
    return liquid_money(family) - emergency_reserve(family);
}

void withdraw_to_cash(Family& family, Rub amount) {
    if (family.cash >= amount) {
        return;
    }

    const Rub needed = amount - family.cash;
    const Rub taken_from_savings = std::min(needed, family.savings.balance);

    family.savings.balance -= taken_from_savings;
    family.cash += taken_from_savings;
}

void put_on_deposit(Family& family, Rub amount) {
    if (amount <= 0.0) {
        return;
    }

    family.cash -= amount;
    family.savings.balance += amount;
}

Worker create_worker(const std::string& name, Rub salary) {
    Worker worker;

    worker.name = name;
    worker.gross_salary = salary;
    worker.gross_history.fill(salary);

    return worker;
}

Family create_family(Housing_strategy strategy, bool keep_history) {
    Family family;

    family.strategy = strategy;
    family.keep_history = keep_history;

    family.workers[alice_id] = create_worker("Алиса", family_start::alice_salary);
    family.workers[bob_id] = create_worker("Боб", family_start::bob_salary);

    const Rub start_money = family_start::alice_savings + family_start::bob_savings;
    family.cash = bank_rules::cash_cushion;
    family.savings.balance = start_money - bank_rules::cash_cushion;

    family.card.limit = bank_rules::credit_card_limit;
    family.card.annual_rate = bank_rules::credit_card_rate;
    family.card.grace_months_left = bank_rules::credit_card_grace_months;

    family.car.owned = true;
    family.car.model = "Kia Rio 2017";
    family.car.production_year = family_start::car_production_year;
    family.car.market_value = family_start::car_value;
    family.car.power_hp = family_start::car_power_hp;
    family.car.fuel_per_100km = family_start::car_fuel_per_100km;
    family.car.km_per_month = family_start::car_km_per_month;
    family.car.insurance_month = family_start::car_insurance_month;

    family.flat.area_m2 = spb_prices::flat_area_m2;

    family.lease.active = true;
    family.lease.monthly_rent = spb_prices::rent_studio;
    family.lease.security_deposit = spb_prices::rent_studio;
    family.lease.anniversary_month = spb_prices::lease_anniversary_month;

    family.dog.alive = true;
    family.dog.name = "Рекс";

    return family;
}


Rub calculate_ndfl(Rub earned_before, Rub new_income) {
    const Rub income_from = earned_before;
    const Rub income_to = earned_before + new_income;

    Rub tax = 0.0;
    Rub bracket_start = 0.0;

    for (const tax_rules::Ndfl_bracket& bracket : tax_rules::ndfl_scale) {
        const Rub part_from = std::max(income_from, bracket_start);
        const Rub part_to = std::min(income_to, bracket.upper_limit);

        if (part_to > part_from) {
            tax += (part_to - part_from) * bracket.rate;
        }

        bracket_start = bracket.upper_limit;
    }

    return tax;
}

void receive_taxed(Family& family, int person, Money_flow flow, Rub gross) {
    if (gross <= 0.0) {
        return;
    }

    Worker& worker = family.workers[person];
    const Rub ndfl = calculate_ndfl(worker.year_taxable_income, gross);

    worker.year_taxable_income += gross;
    worker.year_ndfl_paid += ndfl;

    receive(family, flow, gross - ndfl);
}

Rub last_months_gross(const Worker& worker, int months) {
    const int history_size = static_cast<int>(worker.gross_history.size());
    Rub sum = 0.0;

    for (int step_back = 1; step_back <= months; ++step_back) {
        const int index = (worker.history_position - step_back + history_size) % history_size;
        sum += worker.gross_history[index];
    }

    return sum;
}

void remember_gross(Worker& worker, Rub gross) {
    const int history_size = static_cast<int>(worker.gross_history.size());

    worker.gross_history[worker.history_position] = gross;
    worker.history_position = (worker.history_position + 1) % history_size;
}

Rub average_daily_for_vacation(const Worker& worker) {
    return last_months_gross(worker, 12) / 12.0 / social_rules::average_days_in_month;
}

Rub average_daily_for_sick_leave(const Worker& worker, const World_month& month) {
    const Rub own_daily = last_months_gross(worker, 24) / social_rules::sick_base_days;
    const Rub state_limit = social_rules::max_daily_sick_base * month.wage_level;

    return std::min(own_daily, state_limit);
}

void apply_career_changes(Family& family, int person, const World_month& month) {
    Worker& worker = family.workers[person];
    const Person_events& events = month.people[person];

    if (events.found_new_job) {
        worker.gross_salary *= events.new_job_salary_factor;
        add_log(family, month.date, std::format("{}: новая работа, зарплата {}",
                                                worker.name, format_money(worker.gross_salary)));
    }

    if (events.fixed_new_salary > 0.0) {
        worker.gross_salary = events.fixed_new_salary;
        add_log(family, month.date, std::format("{}: повышение в должности, зарплата {}",
                                                worker.name, format_money(worker.gross_salary)));
    } else if (events.employed && month.wage_indexation > 0.0) {
        worker.gross_salary *= 1.0 + month.wage_indexation;
    }

    if (events.promotion_raise > 0.0) {
        worker.gross_salary *= 1.0 + events.promotion_raise;
        add_log(family, month.date, std::format("{}: прибавка +{}, зарплата {}",
                                                worker.name,
                                                format_percent(events.promotion_raise),
                                                format_money(worker.gross_salary)));
    }
}

void receive_unemployed_income(Family& family, int person, const World_month& month) {
    Worker& worker = family.workers[person];
    const Person_events& events = month.people[person];

    if (events.laid_off) {
        receive(family, Money_flow::severance, worker.gross_salary);
        add_log(family, month.date, std::format("{}: сокращение, выходное пособие {}",
                                                worker.name, format_money(worker.gross_salary)));
    } else {
        receive(family, Money_flow::unemployment_benefit, social_rules::unemployment_benefit);
    }

    remember_gross(worker, 0.0);
}

void receive_work_income(Family& family, int person, const World_month& month) {
    Worker& worker = family.workers[person];
    const Person_events& events = month.people[person];

    if (!events.employed) {
        receive_unemployed_income(family, person, month);
        return;
    }

    const int absent_days = std::min(month.calendar_days, events.sick_days + events.vacation_days);
    const double absent_share = static_cast<double>(absent_days) / month.calendar_days;
    const int absent_working_days = static_cast<int>(std::lround(month.working_days * absent_share));
    const int worked_days = std::max(0, month.working_days - absent_working_days);

    const Rub salary_gross = worker.gross_salary * worked_days / month.working_days;

    Rub sick_gross = 0.0;
    if (events.sick_days > 0) {
        sick_gross = average_daily_for_sick_leave(worker, month)
                   * social_rules::sick_pay_share
                   * events.sick_days;
    }

    Rub vacation_gross = 0.0;
    if (events.vacation_days > 0) {
        vacation_gross = average_daily_for_vacation(worker) * events.vacation_days;
    }

    receive_taxed(family, person, Money_flow::salary, salary_gross);
    receive_taxed(family, person, Money_flow::sick_pay, sick_gross);
    receive_taxed(family, person, Money_flow::vacation_pay, vacation_gross);

    remember_gross(worker, worker.gross_salary);
}

void receive_freelance_income(Family& family, const World_month& month) {
    for (int person = 0; person < people_count; ++person) {
        const Rub gross = month.people[person].freelance_gross;

        if (gross <= 0.0) {
            continue;
        }

        const Rub npd_tax = gross * tax_rules::npd_rate_individuals;
        receive(family, Money_flow::freelance, gross - npd_tax);
    }
}

void receive_tax_refunds(Family& family, const World_month& month) {
    for (const Pending_refund& refund : family.refunds) {
        const bool arrived_now = refund.arrival.year == month.date.year
                              && refund.arrival.month == month.date.month;

        if (!arrived_now) {
            continue;
        }

        receive(family, Money_flow::tax_refund, refund.amount);
        family.tax_refunds_received += refund.amount;
        add_log(family, month.date, std::format("{}: пришёл возврат налога {}",
                                                refund.owner, format_money(refund.amount)));
    }

    const auto arrived_now = [&month](const Pending_refund& refund) {
        return refund.arrival.year == month.date.year && refund.arrival.month == month.date.month;
    };

    family.refunds.erase(std::remove_if(family.refunds.begin(), family.refunds.end(), arrived_now),
                         family.refunds.end());
}

void accrue_deposit_interest(Family& family, const World_month& month) {
    Savings_account& savings = family.savings;
    savings.max_key_rate_year = std::max(savings.max_key_rate_year, month.key_rate);

    if (savings.balance <= 0.0) {
        return;
    }

    const Rub interest = savings.balance * deposit_rate(month.key_rate) / 12.0;

    savings.balance += interest;
    savings.interest_year += interest;
    savings.interest_total += interest;
    record_flow(family, Money_flow::deposit_interest, interest);
}

void receive_incomes(Family& family, const World_month& month) {
    for (int person = 0; person < people_count; ++person) {
        apply_career_changes(family, person, month);
        receive_work_income(family, person, month);
    }

    receive_freelance_income(family, month);
    receive_tax_refunds(family, month);
    accrue_deposit_interest(family, month);
}


double presence_share(const World_month& month, int person) {
    const Person_events& events = month.people[person];

    if (!events.employed) {
        return 0.0;
    }

    const int absent_days = std::min(month.calendar_days, events.sick_days + events.vacation_days);

    return 1.0 - static_cast<double>(absent_days) / month.calendar_days;
}

bool everybody_employed(const World_month& month) {
    for (const Person_events& events : month.people) {
        if (!events.employed) {
            return false;
        }
    }

    return true;
}

int car_age(const Family& family, const World_month& month) {
    return month.date.year - family.car.production_year;
}

Rub summer_trip_cost(const World_month& month) {
    return spb_prices::summer_trip * price_level(month, Price_category::services);
}

bool can_afford_summer_trip(const Family& family, const World_month& month) {
    const bool trip_season = month.summer_vacation && everybody_employed(month);

    if (!trip_season) {
        return false;
    }

    const Rub money_after_trip = liquid_money(family) - summer_trip_cost(month);

    return money_after_trip >= bank_rules::cash_cushion * 2;
}

void pay_rent(Family& family, const World_month& month) {
    Rental& lease = family.lease;

    const bool lease_anniversary = month.date.month == lease.anniversary_month
                                && month.date.year > simulation_rules::start_year;

    if (lease_anniversary) {
        const Rub market_rent = spb_prices::rent_studio * price_level(month, Price_category::rent);

        if (market_rent > lease.monthly_rent) {
            add_log(family, month.date, std::format("Арендодатель поднял плату: {} -> {}",
                                                    format_money(lease.monthly_rent),
                                                    format_money(market_rent)));
            lease.monthly_rent = market_rent;
        }
    }

    spend(family, Money_flow::rent, lease.monthly_rent);
    spend(family, Money_flow::utilities,
          spb_prices::utilities_tenant * price_level(month, Price_category::utilities));
}

void pay_mortgage_insurance(Family& family, const World_month& month) {
    const Loan* mortgage = find_active_loan(family, Loan_kind::mortgage);

    if (mortgage == nullptr) {
        return;
    }

    const Rub insurance = mortgage->balance * bank_rules::mortgage_insurance_rate;

    spend(family, Money_flow::bank_fees, insurance);
    add_log(family, month.date, std::format("Оплачена страховка по ипотеке: {}", format_money(insurance)));
}

void pay_owner_costs(Family& family, const World_month& month) {
    spend(family, Money_flow::utilities,
          spb_prices::utilities_owner * price_level(month, Price_category::utilities));

    const bool purchase_anniversary = month.date.month == family.flat.bought.month
                                   && month.date.year > family.flat.bought.year;

    if (purchase_anniversary) {
        pay_mortgage_insurance(family, month);
    }
}

void pay_housing(Family& family, const World_month& month) {
    if (family.lease.active) {
        pay_rent(family, month);
    }

    if (family.flat.owned) {
        pay_owner_costs(family, month);
    }
}

void pay_food(Family& family, const World_month& month) {
    Rub food_cost = spb_prices::food_per_adult * people_count * price_level(month, Price_category::food);

    if (family.on_trip) {
        food_cost *= spb_prices::food_share_during_trip;
    }

    spend(family, Money_flow::food, food_cost);
}

void pay_communication(Family& family, const World_month& month) {
    spend(family, Money_flow::communication,
          spb_prices::internet_and_mobile * price_level(month, Price_category::services));
}

void pay_public_transport(Family& family, const World_month& month) {
    const Rub ride_price = spb_prices::metro_ride * price_level(month, Price_category::transport);
    int total_rides = 0;

    for (int person = 0; person < people_count; ++person) {
        const bool drives_own_car_to_work = person == bob_id && family.car.owned;

        if (!drives_own_car_to_work) {
            const double days_at_work = presence_share(month, person);
            total_rides += static_cast<int>(std::lround(spb_prices::work_rides_per_month * days_at_work));
        }

        total_rides += spb_prices::leisure_rides_per_month;
    }

    spend(family, Money_flow::public_transport, total_rides * ride_price);
}

void pay_car_breakdown(Family& family, const World_month& month) {
    const int age_years = car_age(family, month);
    const double breakdown_chance = world_rules::car_breakdown_base_chance
                                  + world_rules::car_breakdown_chance_per_year * age_years;

    if (month.car_breakdown_roll >= breakdown_chance) {
        return;
    }

    const Rub repair_cost = month.car_repair_base * (1.0 + age_years / 10.0);

    spend(family, Money_flow::car, repair_cost);
    add_log(family, month.date, std::format("Поломка машины, ремонт {}", format_money(repair_cost)));
}

void pay_car_expenses(Family& family, const World_month& month) {
    if (!family.car.owned) {
        return;
    }

    const Family_car& car = family.car;
    const double services_level = price_level(month, Price_category::services);
    const double usage = std::max(spb_prices::car_minimum_usage, presence_share(month, bob_id));

    const Rub fuel_cost = car.km_per_month * usage * car.fuel_per_100km / 100.0
                        * spb_prices::fuel_liter * price_level(month, Price_category::fuel);

    spend(family, Money_flow::car, fuel_cost);
    spend(family, Money_flow::car, spb_prices::parking_and_wash * services_level);

    const bool tyre_season = month.date.month == 4 || month.date.month == 10;
    if (tyre_season) {
        spend(family, Money_flow::car, spb_prices::tyre_change * services_level);
    }

    const bool service_month = month.date.month == 5;
    if (service_month) {
        const double age_factor = 1.0 + spb_prices::car_service_growth_per_year_of_age * car_age(family, month);
        spend(family, Money_flow::car, spb_prices::car_service * services_level * age_factor);
    }

    const bool insurance_month = month.date.month == car.insurance_month;
    if (insurance_month) {
        spend(family, Money_flow::car, spb_prices::osago_year * services_level);
    }

    pay_car_breakdown(family, month);
}

void update_car_value(Family& family, const World_month& month) {
    (void)month;

    if (!family.car.owned) {
        return;
    }

    family.car.market_value *= 1.0 - spb_prices::car_depreciation_year / 12.0;
}

void pay_dog_expenses(Family& family, const World_month& month) {
    if (!family.dog.alive) {
        return;
    }

    const double pet_level = price_level(month, Price_category::pet);

    if (month.dog_died) {
        family.dog.alive = false;
        spend(family, Money_flow::dog, spb_prices::dog_funeral * pet_level);
        add_log(family, month.date, std::format("Собака {} умерла в возрасте {} лет",
                                                family.dog.name, month.dog_age_months / 12));
        return;
    }

    spend(family, Money_flow::dog, spb_prices::dog_food * pet_level);

    const bool grooming_month = month.date.month % 3 == 0;
    if (grooming_month) {
        spend(family, Money_flow::dog, spb_prices::dog_grooming * pet_level);
    }

    const bool vaccination_month = month.date.month == 4;
    if (vaccination_month) {
        spend(family, Money_flow::dog, spb_prices::dog_vaccination * pet_level);
    }

    if (month.dog_vet_cost > 0.0) {
        spend(family, Money_flow::dog, month.dog_vet_cost);

        const bool expensive_treatment = month.dog_vet_cost > 15'000;
        if (expensive_treatment) {
            add_log(family, month.date, std::format("Собака {} у ветеринара, счёт {}",
                                                    family.dog.name, format_money(month.dog_vet_cost)));
        }
    }
}

void pay_medical_bill(Family& family, int person, Rub amount) {
    spend(family, Money_flow::health, amount);
    family.workers[person].year_medical_expenses += amount;
}

void pay_health_expenses(Family& family, const World_month& month) {
    for (int person = 0; person < people_count; ++person) {
        const Person_events& events = month.people[person];

        if (events.treatment_cost > 0.0) {
            pay_medical_bill(family, person, events.treatment_cost);
        }

        if (events.serious_illness) {
            add_log(family, month.date, std::format("{}: серьёзная болезнь, {} дней, лечение {}",
                                                    family.workers[person].name,
                                                    events.sick_days,
                                                    format_money(events.treatment_cost)));
        }
    }

    const Rub dentist_bill = spb_prices::dentist_per_adult * price_level(month, Price_category::medicine);

    if (month.date.month == spb_prices::alice_dentist_month) {
        pay_medical_bill(family, alice_id, dentist_bill);
    }

    if (month.date.month == spb_prices::bob_dentist_month) {
        pay_medical_bill(family, bob_id, dentist_bill);
    }
}

void pay_household_expenses(Family& family, const World_month& month) {
    const double goods_level = price_level(month, Price_category::goods);
    const double services_level = price_level(month, Price_category::services);

    spend(family, Money_flow::household, spb_prices::household_goods * goods_level);
    spend(family, Money_flow::household, spb_prices::clothes_per_adult * people_count * goods_level);

    Rub entertainment_cost = spb_prices::entertainment * services_level;
    if (!everybody_employed(month)) {
        entertainment_cost *= spb_prices::entertainment_share_when_unemployed;
    }

    spend(family, Money_flow::household, entertainment_cost);

    if (month.appliance_cost > 0.0) {
        spend(family, Money_flow::household, month.appliance_cost);
        add_log(family, month.date, std::format("Сломалась бытовая техника, замена {}",
                                                format_money(month.appliance_cost)));
    }
}

void pay_gifts(Family& family, const World_month& month) {
    const double goods_level = price_level(month, Price_category::goods);

    const bool new_year_month = month.date.month == 12;
    if (new_year_month) {
        spend(family, Money_flow::gifts, spb_prices::new_year_gifts * goods_level);
    }

    const bool birthday_month = month.date.month == spb_prices::alice_birthday_month
                             || month.date.month == spb_prices::bob_birthday_month;
    if (birthday_month) {
        spend(family, Money_flow::gifts, spb_prices::birthday_gift * goods_level);
    }
}

void pay_vacations(Family& family, const World_month& month) {
    if (family.on_trip) {
        const Rub trip_cost = summer_trip_cost(month);

        spend(family, Money_flow::vacations, trip_cost);
        add_log(family, month.date, std::format("Совместная летняя поездка, стоимость {}",
                                                format_money(trip_cost)));
    } else if (month.summer_vacation) {
        add_log(family, month.date, "Летняя поездка отменена: не хватает денег или кто-то без работы");
    }

    for (int person = 0; person < people_count; ++person) {
        const bool own_vacation = month.people[person].vacation_days > 0 && !month.summer_vacation;

        if (own_vacation) {
            spend(family, Money_flow::vacations,
                  spb_prices::vacation_leisure_per_person * price_level(month, Price_category::services));
        }
    }
}

double transport_tax_rate(int power_hp) {
    for (const tax_rules::Transport_tax_step& step : tax_rules::transport_tax_scale) {
        if (power_hp <= step.max_power_hp) {
            return step.rate_per_hp;
        }
    }

    return tax_rules::transport_tax_scale.back().rate_per_hp;
}

Rub property_tax(const Family& family, const World_month& month) {
    const double price_growth = price_level(month, Price_category::real_estate)
                              / family.flat.price_level_at_purchase;
    const Rub cadastral_value = family.flat.purchase_price * spb_prices::cadastral_share * price_growth;

    const double free_area = tax_rules::tax_free_m2_per_owner * people_count;
    const double taxable_area = std::max(0.0, family.flat.area_m2 - free_area);
    const double taxable_share = taxable_area / family.flat.area_m2;

    return cadastral_value * taxable_share * tax_rules::property_tax_rate;
}

void pay_annual_taxes(Family& family, const World_month& month) {
    const bool tax_payment_month = month.date.month == tax_rules::annual_tax_payment_month;

    if (!tax_payment_month) {
        return;
    }

    if (family.savings.tax_due > 0.0) {
        spend(family, Money_flow::taxes, family.savings.tax_due);
        add_log(family, month.date, std::format("Уплачен налог на проценты по вкладу: {}",
                                                format_money(family.savings.tax_due)));
        family.savings.tax_due = 0.0;
    }

    if (family.car.owned) {
        const Rub transport_tax = transport_tax_rate(family.car.power_hp) * family.car.power_hp;
        spend(family, Money_flow::taxes, transport_tax);
    }

    const bool flat_owned_full_year = family.flat.owned && family.flat.bought.year < month.date.year;
    if (flat_owned_full_year) {
        spend(family, Money_flow::taxes, property_tax(family, month));
    }
}


struct Monthly_step {
    const char* title;
    void (*apply)(Family& family, const World_month& month);
};

constexpr std::array<Monthly_step, 10> living_expense_steps = {{
    {"жильё", pay_housing},
    {"еда", pay_food},
    {"связь", pay_communication},
    {"общественный транспорт", pay_public_transport},
    {"машина", pay_car_expenses},
    {"собака", pay_dog_expenses},
    {"здоровье", pay_health_expenses},
    {"быт и досуг", pay_household_expenses},
    {"подарки", pay_gifts},
    {"отпуск", pay_vacations},
}};

void pay_living_expenses(Family& family, const World_month& month) {
    for (const Monthly_step& step : living_expense_steps) {
        step.apply(family, month);
    }

    pay_annual_taxes(family, month);
}


void buy_gold_bar(Family& family, const World_month& month) {
    const Rub bar_price = gold_price_per_gram(month)
                        * gold_rules::bar_weight_grams
                        * (1.0 + gold_rules::bank_margin);

    const bool rich_enough = free_money(family) > gold_rules::min_free_money_to_buy + bar_price;
    if (!rich_enough) {
        return;
    }

    withdraw_to_cash(family, bar_price);
    spend(family, Money_flow::gold_purchase, bar_price);

    family.gold.grams += gold_rules::bar_weight_grams;
    family.gold.money_spent += bar_price;
    ++family.gold.bars_bought;

    add_log(family, month.date, std::format("Спонтанная покупка: слиток золота {} г за {}",
                                            gold_rules::bar_weight_grams, format_money(bar_price)));
}

void buy_bicycle(Family& family, const World_month& month) {
    const bool cycling_season = month.date.month >= 4 && month.date.month <= 8;
    if (!cycling_season) {
        return;
    }

    const Rub bicycle_price = hobby_rules::bicycle_price * price_level(month, Price_category::goods);

    const bool rich_enough = free_money(family) > hobby_rules::min_free_money_for_bicycle + bicycle_price;
    if (!rich_enough) {
        return;
    }

    withdraw_to_cash(family, bicycle_price);
    spend(family, Money_flow::hobby, bicycle_price);

    add_log(family, month.date, std::format("Спонтанная покупка: велосипед за {}", format_money(bicycle_price)));
}

void go_to_concert(Family& family, const World_month& month) {
    const Rub tickets_price = hobby_rules::concert_tickets * price_level(month, Price_category::services);

    const bool rich_enough = free_money(family) > hobby_rules::min_free_money_for_concert + tickets_price;
    if (!rich_enough) {
        return;
    }

    spend(family, Money_flow::hobby, tickets_price);
}

void donate_to_charity(Family& family, const World_month& month) {
    const Rub donation = hobby_rules::charity_donation * general_price_level(month);

    const bool rich_enough = free_money(family) > donation;
    if (!rich_enough) {
        return;
    }

    spend(family, Money_flow::charity, donation);
}

struct Spontaneous_action {
    const char* title;
    double chance_per_month;
    void (*apply)(Family& family, const World_month& month);
};

constexpr std::array<Spontaneous_action, 4> spontaneous_actions = {{
    {"покупка слитка золота", spontaneous_rules::gold_bar_chance, buy_gold_bar},
    {"покупка велосипеда", spontaneous_rules::bicycle_chance, buy_bicycle},
    {"поход на концерт", spontaneous_rules::concert_chance, go_to_concert},
    {"благотворительность", spontaneous_rules::charity_chance, donate_to_charity},
}};

static_assert(spontaneous_actions.size() <= spontaneous_rules::max_actions);

void run_spontaneous_actions(Family& family, const World_month& month) {
    for (size_t i = 0; i < spontaneous_actions.size(); ++i) {
        const Spontaneous_action& action = spontaneous_actions[i];
        const bool action_happened = month.action_rolls[i] < action.chance_per_month;

        if (action_happened) {
            action.apply(family, month);
        }
    }
}

Rub sell_gold(Family& family, const World_month& month, Rub money_needed) {
    if (family.gold.grams <= 0.0) {
        return 0.0;
    }

    const Rub price_per_gram = gold_price_per_gram(month) * (1.0 - gold_rules::sell_discount);
    const double grams_needed = money_needed / price_per_gram;
    const double grams_sold = std::min(family.gold.grams, grams_needed);
    const Rub money_received = grams_sold * price_per_gram;

    family.gold.grams -= grams_sold;
    receive(family, Money_flow::gold_sale, money_received);

    add_log(family, month.date, std::format("Продано {} г золота за {}",
                                            format_number(grams_sold), format_money(money_received)));

    return money_received;
}


void open_loan(Family& family,
               Loan_kind kind,
               const std::string& title,
               Rub amount,
               double annual_rate,
               int months,
               const World_month& month) {
    Loan loan;

    loan.kind = kind;
    loan.title = title;
    loan.start_amount = amount;
    loan.balance = amount;
    loan.annual_rate = annual_rate;
    loan.months_left = months;
    loan.monthly_payment = annuity_payment(amount, annual_rate, months);
    loan.opened = month.date;
    loan.last_rate_change = month.date;
    loan.active = true;

    family.loans.push_back(loan);

    add_log(family, month.date, std::format("{} оформлен: {} под {} на {} мес., платёж {}",
                                            title,
                                            format_money(amount),
                                            format_percent(annual_rate),
                                            months,
                                            format_money(loan.monthly_payment)));
}

void close_loan(Family& family, Loan& loan, const World_month& month) {
    loan.active = false;
    loan.balance = 0.0;
    loan.months_left = 0;

    if (loan.kind == Loan_kind::mortgage) {
        family.month_mortgage_closed = family.months_simulated;
    }

    add_log(family, month.date, std::format("{} закрыт. Уплачено процентов: {}, погашено досрочно: {}",
                                            loan.title,
                                            format_money(loan.interest_paid),
                                            format_money(loan.early_repaid)));
}

void add_mortgage_interest_to_workers(Family& family, Rub interest) {
    for (Worker& worker : family.workers) {
        worker.year_mortgage_interest += interest / people_count;
    }
}

void pay_one_loan(Family& family, Loan& loan, const World_month& month) {
    const bool first_month_of_loan = months_between(loan.opened, month.date) < 1;

    if (first_month_of_loan) {
        return;
    }

    const Rub interest = loan.balance * loan.annual_rate / 12.0;
    const bool last_payment = loan.months_left <= 1;
    const Rub payment = last_payment ? loan.balance + interest
                                     : std::min(loan.monthly_payment, loan.balance + interest);
    const Rub principal = payment - interest;

    loan.balance -= principal;
    loan.months_left -= 1;
    loan.interest_paid += interest;

    if (loan.kind == Loan_kind::mortgage) {
        spend(family, Money_flow::mortgage_payment, payment);
        add_mortgage_interest_to_workers(family, interest);
    } else {
        spend(family, Money_flow::car_loan_payment, payment);
    }

    const bool loan_repaid = loan.balance < 1.0;
    if (loan_repaid) {
        close_loan(family, loan, month);
    }
}

void pay_loans(Family& family, const World_month& month) {
    for (Loan& loan : family.loans) {
        if (loan.active) {
            pay_one_loan(family, loan, month);
        }
    }
}

Rub early_repay_loan(Family& family, Loan_kind kind, Rub amount, const World_month& month) {
    Loan* loan = find_active_loan(family, kind);

    const bool repayment_possible = loan != nullptr && amount >= bank_rules::min_early_repayment;
    if (!repayment_possible) {
        return 0.0;
    }

    const Rub repaid = std::min(amount, loan->balance);

    loan->balance -= repaid;
    loan->early_repaid += repaid;
    spend(family, Money_flow::early_repayment, repaid);

    if (loan->balance < 1.0) {
        close_loan(family, *loan, month);
    } else {
        loan->months_left = months_to_repay(loan->balance, loan->annual_rate, loan->monthly_payment);
    }

    return repaid;
}

void try_refinance_mortgage(Family& family, const World_month& month) {
    Loan* mortgage = find_active_loan(family, Loan_kind::mortgage);

    if (mortgage == nullptr) {
        return;
    }

    const bool too_little_time_left = mortgage->months_left < bank_rules::refinance_min_months_left;
    const bool refinanced_recently = months_between(mortgage->last_rate_change, month.date)
                                   < bank_rules::months_between_refinances;

    if (too_little_time_left || refinanced_recently) {
        return;
    }

    const double new_rate = mortgage_rate(month.key_rate);
    const bool gain_too_small = new_rate > mortgage->annual_rate - bank_rules::refinance_min_gain;

    if (gain_too_small) {
        return;
    }

    const Rub paperwork_costs = bank_rules::refinance_costs * price_level(month, Price_category::services);
    const bool can_pay_paperwork = liquid_money(family) >= paperwork_costs + bank_rules::cash_cushion;

    if (!can_pay_paperwork) {
        return;
    }

    const double old_rate = mortgage->annual_rate;
    const Rub old_payment = mortgage->monthly_payment;

    spend(family, Money_flow::bank_fees, paperwork_costs);

    mortgage->annual_rate = new_rate;
    mortgage->monthly_payment = annuity_payment(mortgage->balance, new_rate, mortgage->months_left);
    mortgage->last_rate_change = month.date;

    add_log(family, month.date, std::format("Рефинансирование ипотеки: {} -> {}, платёж {} -> {}",
                                            format_percent(old_rate),
                                            format_percent(new_rate),
                                            format_money(old_payment),
                                            format_money(mortgage->monthly_payment)));
}

void try_replace_car(Family& family, const World_month& month) {
    const bool replacement_month = family.car.owned && month.date.month == spb_prices::car_replace_month;

    if (!replacement_month) {
        return;
    }

    const bool car_too_old = car_age(family, month) >= spb_prices::car_replace_age;
    if (!car_too_old) {
        return;
    }

    const Rub new_car_price = spb_prices::new_car_price * price_level(month, Price_category::car);
    const Rub trade_in_value = family.car.market_value * bank_rules::car_trade_in_share;
    const Rub money_needed = new_car_price - trade_in_value + spb_prices::car_registration;

    Rub own_money = std::min(money_needed, std::max(0.0, free_money(family)));
    Rub credit_part = money_needed - own_money;

    const bool credit_too_small = credit_part > 0.0 && credit_part < bank_rules::min_car_loan;
    if (credit_too_small) {
        own_money = money_needed;
        credit_part = 0.0;
    }

    withdraw_to_cash(family, own_money);
    spend(family, Money_flow::car_purchase, own_money);

    const std::string old_model = family.car.model;

    family.car.model = std::format("новый кроссовер {}", month.date.year);
    family.car.production_year = month.date.year;
    family.car.market_value = new_car_price;
    family.car.power_hp = spb_prices::new_car_power_hp;
    family.car.fuel_per_100km = spb_prices::new_car_fuel_per_100km;
    family.car.insurance_month = month.date.month;

    add_log(family, month.date, std::format("{} сдана в трейд-ин, куплен {} (цена {}, свои деньги {})",
                                            old_model,
                                            family.car.model,
                                            format_money(new_car_price),
                                            format_money(own_money)));

    if (credit_part > 0.0) {
        open_loan(family, Loan_kind::car_loan, "Автокредит", credit_part,
                  car_loan_rate(month.key_rate), bank_rules::car_loan_months, month);
    }

    spend(family, Money_flow::car, spb_prices::osago_year * price_level(month, Price_category::services));
}

void register_flat_purchase(Family& family, const World_month& month, Rub price) {
    family.flat.owned = true;
    family.flat.purchase_price = price;
    family.flat.bought = month.date;
    family.flat.price_level_at_purchase = price_level(month, Price_category::real_estate);
    family.month_flat_bought = family.months_simulated;

    for (Worker& owner : family.workers) {
        const Rub own_share = price / people_count;
        const Rub deduction_base = std::min(own_share, owner.property_deduction_left);

        owner.property_deduction_left -= deduction_base;
        owner.property_base_left += deduction_base;
    }
}

void end_lease(Family& family, const World_month& month) {
    if (!family.lease.active) {
        return;
    }

    family.cash += family.lease.security_deposit;
    family.lease.active = false;

    add_log(family, month.date, std::format("Аренда закончена, возвращён залог {}",
                                            format_money(family.lease.security_deposit)));
}

Rub purchase_side_costs(const World_month& month) {
    return (spb_prices::deal_costs + spb_prices::moving_cost)
         * price_level(month, Price_category::services);
}

void buy_flat_with_mortgage(Family& family, const World_month& month) {
    end_lease(family, month);

    const Rub price = flat_market_price(month);
    const Rub side_costs = purchase_side_costs(month);
    const Rub min_down_payment = price * bank_rules::min_down_payment_share;

    Rub down_payment = liquid_money(family) - side_costs - bank_rules::start_reserve;

    if (down_payment < min_down_payment) {
        down_payment = min_down_payment;
        add_log(family, month.date, "Внимание: первоначальный взнос съедает подушку безопасности");
    }

    withdraw_to_cash(family, down_payment + side_costs);
    spend(family, Money_flow::home_purchase, down_payment);
    spend(family, Money_flow::bank_fees, side_costs);

    register_flat_purchase(family, month, price);

    add_log(family, month.date, std::format("Куплена квартира {} м2 за {}, первоначальный взнос {}",
                                            spb_prices::flat_area_m2,
                                            format_money(price),
                                            format_money(down_payment)));

    open_loan(family, Loan_kind::mortgage, "Ипотечный кредит", price - down_payment,
              mortgage_rate(month.key_rate), bank_rules::mortgage_months, month);

    pay_mortgage_insurance(family, month);
}

void try_buy_flat_with_savings(Family& family, const World_month& month) {
    const bool saving_for_flat = family.strategy == Housing_strategy::rent_and_save && !family.flat.owned;

    if (!saving_for_flat) {
        return;
    }

    const Rub price = flat_market_price(month);
    const Rub side_costs = purchase_side_costs(month);
    const bool enough_money = free_money(family) >= price + side_costs;

    if (!enough_money) {
        return;
    }

    withdraw_to_cash(family, price + side_costs);
    spend(family, Money_flow::home_purchase, price);
    spend(family, Money_flow::bank_fees, side_costs);

    register_flat_purchase(family, month, price);

    add_log(family, month.date, std::format("Куплена квартира {} м2 на накопления за {}",
                                            spb_prices::flat_area_m2, format_money(price)));

    end_lease(family, month);
}

void apply_start_decision(Family& family, const World_month& month) {
    if (family.strategy == Housing_strategy::rent_and_save) {
        add_log(family, month.date, "Решение: снимать жильё и копить на вкладе");
        return;
    }

    add_log(family, month.date, "Решение: сразу купить квартиру в ипотеку");
    buy_flat_with_mortgage(family, month);
}

void make_credit_decisions(Family& family, const World_month& month) {
    try_refinance_mortgage(family, month);
    try_replace_car(family, month);
    try_buy_flat_with_savings(family, month);
}


void charge_credit_card_interest(Family& family, const World_month& month) {
    Credit_card& card = family.card;

    if (card.debt <= 0.0) {
        card.grace_months_left = bank_rules::credit_card_grace_months;
        return;
    }

    if (card.grace_months_left > 0) {
        --card.grace_months_left;
        return;
    }

    const Rub interest = card.debt * card.annual_rate / 12.0;

    card.debt += interest;
    card.interest_paid += interest;
    record_flow(family, Money_flow::credit_card_interest, interest);

    const bool debt_is_dangerous = card.debt > card.limit * 0.8;
    if (debt_is_dangerous) {
        add_log(family, month.date, std::format("Опасный долг по кредитке: {}", format_money(card.debt)));
    }
}

void cover_cash_gap(Family& family, const World_month& month) {
    Rub money_missing = -family.cash;

    const Rub taken_from_savings = std::min(money_missing, family.savings.balance);
    family.savings.balance -= taken_from_savings;
    money_missing -= taken_from_savings;

    const bool gold_can_help = money_missing > 0.0 && family.gold.grams > 0.0;
    if (gold_can_help) {
        money_missing -= sell_gold(family, month, money_missing);
    }

    if (money_missing > 0.0) {
        Credit_card& card = family.card;
        const Rub free_limit = std::max(0.0, card.limit - card.debt);
        const Rub taken_from_card = std::min(money_missing, free_limit);

        const bool first_time_in_debt = card.debt <= 0.0 && taken_from_card > 0.0;
        if (first_time_in_debt) {
            add_log(family, month.date, std::format("Накопления закончились, использована кредитка: {}",
                                                    format_money(taken_from_card)));
        }

        card.debt += taken_from_card;
        money_missing -= taken_from_card;
    }

    if (money_missing > 0.0) {
        const Rub penalty = money_missing * bank_rules::overdue_penalty_monthly;

        ++family.overdue_months;
        family.overdue_penalties += penalty;
        record_flow(family, Money_flow::bank_fees, penalty);
        family.cash = -(money_missing + penalty);

        add_log(family, month.date, std::format("Не хватает денег! Просрочка {}, штраф {}",
                                                format_money(money_missing), format_money(penalty)));
        return;
    }

    family.cash = 0.0;
}

void repay_credit_card(Family& family, const World_month& month) {
    Credit_card& card = family.card;

    if (card.debt <= 0.0) {
        return;
    }

    const Rub spare_cash = std::max(0.0, family.cash - bank_rules::cash_cushion / 2.0);
    const Rub paid_from_cash = std::min(card.debt, spare_cash);
    family.cash -= paid_from_cash;
    card.debt -= paid_from_cash;

    const Rub paid_from_savings = std::min(card.debt, family.savings.balance);
    family.savings.balance -= paid_from_savings;
    card.debt -= paid_from_savings;

    const bool debt_closed = card.debt < 1.0;
    if (debt_closed) {
        card.debt = 0.0;
        add_log(family, month.date, "Долг по кредитке полностью погашен");
    }
}

void use_free_cash(Family& family, const World_month& month) {
    repay_credit_card(family, month);

    Rub spare_cash = family.cash - bank_rules::cash_cushion;
    if (spare_cash <= 0.0) {
        return;
    }

    const Rub reserve = emergency_reserve(family);
    if (family.savings.balance < reserve) {
        const Rub money_to_reserve = std::min(spare_cash, reserve - family.savings.balance);

        put_on_deposit(family, money_to_reserve);
        spare_cash -= money_to_reserve;
    }

    if (spare_cash <= 0.0) {
        return;
    }

    spare_cash -= early_repay_loan(family, Loan_kind::car_loan, spare_cash, month);

    if (family.strategy == Housing_strategy::mortgage_early_repayment) {
        spare_cash -= early_repay_loan(family, Loan_kind::mortgage, spare_cash, month);
    }

    put_on_deposit(family, spare_cash);
}

void settle_cash(Family& family, const World_month& month) {
    if (family.cash < 0.0) {
        cover_cash_gap(family, month);
    } else {
        use_free_cash(family, month);
    }
}


Rub calculate_refund(Worker& worker) {
    const Rub max_base = worker.year_ndfl_paid / tax_rules::deduction_rate;
    Rub used_base = 0.0;

    const Rub social_base = std::min({worker.year_medical_expenses,
                                      tax_rules::social_deduction_limit,
                                      max_base});
    used_base += social_base;

    const Rub property_base = std::min(worker.property_base_left, max_base - used_base);
    worker.property_base_left -= property_base;
    used_base += property_base;

    const Rub interest_claim = worker.year_mortgage_interest + worker.interest_carry_over;
    const Rub interest_base = std::min({interest_claim,
                                        worker.interest_deduction_left,
                                        max_base - used_base});
    worker.interest_deduction_left -= interest_base;
    worker.interest_carry_over = std::min(interest_claim - interest_base, worker.interest_deduction_left);
    used_base += interest_base;

    return used_base * tax_rules::deduction_rate;
}

void file_tax_returns(Family& family, const World_month& month, int finished_year) {
    for (Worker& worker : family.workers) {
        const Rub refund = calculate_refund(worker);

        if (refund < 1.0) {
            continue;
        }

        Pending_refund pending;
        pending.owner = worker.name;
        pending.amount = refund;
        pending.arrival = make_date(month.date.year, tax_rules::refund_arrival_month);

        family.refunds.push_back(pending);

        add_log(family, month.date, std::format("{}: подана 3-НДФЛ за {} год, ожидаемый возврат {}",
                                                worker.name, finished_year, format_money(refund)));
    }
}

void calculate_deposit_tax(Family& family, const World_month& month, int finished_year) {
    Savings_account& savings = family.savings;

    const Rub tax_free_interest = tax_rules::deposit_tax_free_base
                                * savings.max_key_rate_year
                                * people_count;
    const Rub taxable_interest = std::max(0.0, savings.interest_year - tax_free_interest);

    if (taxable_interest <= 0.0) {
        return;
    }

    const Rub tax = taxable_interest * tax_rules::deposit_interest_tax;
    savings.tax_due += tax;

    add_log(family, month.date, std::format("Проценты по вкладу за {} год: {}, налог составит {}",
                                            finished_year,
                                            format_money(savings.interest_year),
                                            format_money(tax)));
}

void save_year_snapshot(Family& family, int year, const World_month& month) {
    Year_snapshot snapshot;

    snapshot.year = year;
    snapshot.income = sum_income(family.year_flows);
    snapshot.spending = sum_spending(family.year_flows);
    snapshot.liquid = liquid_money(family);
    snapshot.debts = total_debts(family);
    snapshot.net_worth = net_worth(family, month);
    snapshot.key_rate = month.key_rate;

    family.yearly.push_back(snapshot);
}

void reset_year_counters(Family& family) {
    for (Worker& worker : family.workers) {
        worker.year_taxable_income = 0.0;
        worker.year_ndfl_paid = 0.0;
        worker.year_medical_expenses = 0.0;
        worker.year_mortgage_interest = 0.0;
    }

    family.savings.interest_year = 0.0;
    family.savings.max_key_rate_year = 0.0;
    family.year_flows.fill(0.0);
}

void start_new_year(Family& family, const World_month& month) {
    const int finished_year = month.date.year - 1;

    save_year_snapshot(family, finished_year, month);
    file_tax_returns(family, month, finished_year);
    calculate_deposit_tax(family, month, finished_year);
    reset_year_counters(family);
}

void finish_month(Family& family, const World_month& month) {
    if (family.card.debt > 0.0) {
        ++family.card.months_in_debt;
    }

    const Rub month_spending = sum_spending(family.month_flows);
    family.typical_monthly_spending = 0.9 * family.typical_monthly_spending + 0.1 * month_spending;

    for (int i = 0; i < money_flow_count; ++i) {
        family.year_flows[i] += family.month_flows[i];
        family.total_flows[i] += family.month_flows[i];
    }

    if (family.keep_history) {
        Month_row row;

        row.date = month.date;
        row.income = sum_income(family.month_flows);
        row.spending = month_spending;
        row.liquid = liquid_money(family);
        row.card_debt = family.card.debt;
        row.loans = total_loan_balance(family);
        row.net_worth = net_worth(family, month);

        family.monthly_rows.push_back(row);
    }

    ++family.months_simulated;
}

void simulate_month(Family& family, const World_month& month) {
    family.month_flows.fill(0.0);

    const bool very_first_month = family.months_simulated == 0;
    const bool new_year_started = !very_first_month && month.date.month == 1;

    if (new_year_started) {
        start_new_year(family, month);
    }

    if (very_first_month) {
        apply_start_decision(family, month);
    }

    receive_incomes(family, month);

    family.on_trip = can_afford_summer_trip(family, month);
    pay_living_expenses(family, month);

    pay_loans(family, month);
    make_credit_decisions(family, month);
    run_spontaneous_actions(family, month);

    charge_credit_card_interest(family, month);
    settle_cash(family, month);

    update_car_value(family, month);
    finish_month(family, month);
}


bool any_mortgage_active(const std::vector<Family>& families) {
    for (const Family& family : families) {
        if (has_active_loan(family, Loan_kind::mortgage)) {
            return true;
        }
    }

    return false;
}

std::vector<Family> create_all_families(bool keep_history) {
    std::vector<Family> families;

    for (int i = 0; i < strategy_count; ++i) {
        families.push_back(create_family(static_cast<Housing_strategy>(i), keep_history));
    }

    return families;
}

int run_world(const World_timeline& world, std::vector<Family>& families) {
    const int months_count = static_cast<int>(world.months.size());
    int last_index = months_count - 1;

    for (int index = 0; index < months_count; ++index) {
        for (Family& family : families) {
            simulate_month(family, world.months[index]);
        }

        const bool horizon_reached = index + 1 >= simulation_rules::min_months;
        const bool all_mortgages_closed = !any_mortgage_active(families);

        if (horizon_reached && all_mortgages_closed) {
            last_index = index;
            break;
        }
    }

    for (Family& family : families) {
        save_year_snapshot(family, world.months[last_index].date.year, world.months[last_index]);
    }

    return last_index;
}

Run_result make_result(const Family& family, const World_month& last_month) {
    Run_result result;

    result.strategy = family.strategy;
    result.net_worth = net_worth(family, last_month);
    result.real_net_worth = result.net_worth / general_price_level(last_month);
    result.liquid = liquid_money(family);
    result.debts = total_debts(family);
    result.interest_paid = total_interest_paid(family);
    result.tax_refunds = family.tax_refunds_received;
    result.gold_value = gold_market_value(family, last_month);
    result.card_debt_months = family.card.months_in_debt;
    result.overdue_months = family.overdue_months;
    result.month_flat_bought = family.month_flat_bought;
    result.month_mortgage_closed = family.month_mortgage_closed;
    result.months = family.months_simulated;

    return result;
}

Rub start_net_worth() {
    const Rub start_money = family_start::alice_savings + family_start::bob_savings;

    return start_money + family_start::car_value + spb_prices::rent_studio;
}

const char* life_verdict(const Run_result& result) {
    if (result.overdue_months > 0) {
        return "ПЛОХО: в некоторые месяцы семья не могла оплатить счета";
    }

    if (result.real_net_worth < start_net_worth()) {
        return "ПЛОХО: семья обеднела с учётом инфляции";
    }

    if (result.card_debt_months > 12) {
        return "РИСКОВАННО: семья жила в долг по кредитке больше года";
    }

    if (result.real_net_worth < start_net_worth() * 2.0) {
        return "НОРМАЛЬНО: капитал растёт, но медленно";
    }

    return "ХОРОШО: стабильная жизнь и реальный рост капитала";
}

Rub mean_value(const std::vector<Rub>& values) {
    if (values.empty()) {
        return 0.0;
    }

    Rub sum = 0.0;
    for (Rub value : values) {
        sum += value;
    }

    return sum / static_cast<double>(values.size());
}

Rub percentile_value(std::vector<Rub> values, double share) {
    if (values.empty()) {
        return 0.0;
    }

    std::sort(values.begin(), values.end());
    const int index = static_cast<int>(std::lround(share * (values.size() - 1)));

    return values[index];
}

int best_strategy_index(const std::vector<Run_result>& results) {
    int best = 0;

    for (int i = 1; i < static_cast<int>(results.size()); ++i) {
        if (results[i].real_net_worth > results[best].real_net_worth) {
            best = i;
        }
    }

    return best;
}

void add_to_statistics(Strategy_statistics& statistics, const Run_result& result) {
    statistics.real_net_worths.push_back(result.real_net_worth);
    statistics.interest_sum += result.interest_paid;
    statistics.refund_sum += result.tax_refunds;
    statistics.card_debt_months_sum += result.card_debt_months;

    if (result.overdue_months > 0) {
        ++statistics.runs_with_overdue;
    }

    if (result.month_flat_bought >= 0) {
        ++statistics.runs_with_flat;
        statistics.months_to_flat_sum += result.month_flat_bought;
    }
}

std::array<Strategy_statistics, strategy_count> run_monte_carlo(unsigned base_seed, int runs) {
    std::array<Strategy_statistics, strategy_count> statistics;

    for (int i = 0; i < strategy_count; ++i) {
        statistics[i].strategy = static_cast<Housing_strategy>(i);
    }

    for (int run = 0; run < runs; ++run) {
        const unsigned run_seed = base_seed * 100'003u + static_cast<unsigned>(run) + 1u;

        const World_timeline world = generate_world(run_seed, simulation_rules::max_months);
        std::vector<Family> families = create_all_families(false);
        const int last_index = run_world(world, families);

        std::vector<Run_result> results;
        for (const Family& family : families) {
            results.push_back(make_result(family, world.months[last_index]));
        }

        for (int i = 0; i < strategy_count; ++i) {
            add_to_statistics(statistics[i], results[i]);
        }

        ++statistics[best_strategy_index(results)].wins;
    }

    return statistics;
}


int display_width(const std::string& text) {
    int width = 0;

    for (unsigned char symbol : text) {
        const bool start_of_character = (symbol & 0xC0) != 0x80;

        if (start_of_character) {
            ++width;
        }
    }

    return width;
}

std::string pad_left(const std::string& text, int width) {
    const int spaces = std::max(0, width - display_width(text));

    return std::string(spaces, ' ') + text;
}

std::string pad_right(const std::string& text, int width) {
    const int spaces = std::max(0, width - display_width(text));

    return text + std::string(spaces, ' ');
}

void print_table_row(const std::vector<std::string>& cells, const std::vector<int>& widths) {
    std::string line;

    for (size_t i = 0; i < cells.size(); ++i) {
        if (i > 0) {
            line += ' ';
        }

        const bool align_left = widths[i] < 0;
        line += align_left ? pad_right(cells[i], -widths[i]) : pad_left(cells[i], widths[i]);
    }

    while (!line.empty() && line.back() == ' ') {
        line.pop_back();
    }

    std::cout << line << '\n';
}

void print_line(char symbol, int length) {
    std::cout << std::string(length, symbol) << '\n';
}

void print_title(const std::string& title) {
    std::cout << '\n';
    print_line('=', 100);
    std::cout << title << '\n';
    print_line('=', 100);
}

std::string describe_month_index(int month_index) {
    if (month_index < 0) {
        return "нет";
    }

    const Sim_date start = make_date(simulation_rules::start_year, simulation_rules::start_month);

    return format_date(add_months(start, month_index));
}

void print_assumptions(const Program_options& options) {
    print_title("АЛИСА И БОБ: МОДЕЛИРОВАНИЕ СЕМЕЙНЫХ ФИНАНСОВ (Санкт-Петербург)");

    std::cout << std::format("Старт: {:04}-{:02}, не меньше {} мес. и до полной выплаты ипотеки\n",
                             simulation_rules::start_year,
                             simulation_rules::start_month,
                             simulation_rules::min_months);
    std::cout << std::format("Seed демонстрации: {}, прогонов Монте-Карло: {}\n",
                             options.seed, options.runs);

    std::cout << std::format("Зарплата Алисы до НДФЛ: {}, повышение до {} в {:04}-{:02}\n",
                             format_money(family_start::alice_salary),
                             format_money(family_start::alice_promotion_salary),
                             family_start::alice_promotion_year,
                             family_start::alice_promotion_month);
    std::cout << std::format("Зарплата Боба до НДФЛ:  {}, плюс репетиторство как самозанятый (НПД {})\n",
                             format_money(family_start::bob_salary),
                             format_percent(tax_rules::npd_rate_individuals));

    std::cout << std::format("Накопления: {}, машина Kia Rio 2017 ({}), собаке {} лет\n",
                             format_money(family_start::alice_savings + family_start::bob_savings),
                             format_money(family_start::car_value),
                             family_start::dog_age_months / 12);
    std::cout << std::format("Аренда студии: {}, квартира {} м2 по {} за м2 = {}\n",
                             format_money(spb_prices::rent_studio),
                             spb_prices::flat_area_m2,
                             format_money(spb_prices::flat_price_per_m2),
                             format_money(spb_prices::flat_area_m2 * spb_prices::flat_price_per_m2));

    std::cout << std::format("Ключевая ставка на старте: {}, ипотека = ключевая + {}, вклад = ключевая {}\n",
                             format_percent(bank_rules::start_key_rate),
                             format_percent(bank_rules::mortgage_spread),
                             format_percent(bank_rules::deposit_spread));
    std::cout << std::format("Больничный: {} среднего заработка, отпуск: 2 x {} дней в год\n",
                             format_percent(social_rules::sick_pay_share),
                             social_rules::vacation_part_days);

    std::cout << "\nСлучайные траты по настроению:\n";
    for (const Spontaneous_action& action : spontaneous_actions) {
        print_table_row({"  " + std::string(action.title),
                         "вероятность " + format_percent(action.chance_per_month) + " в месяц"},
                        {-28, -30});
    }
}

const std::vector<int> year_table_widths = {6, 14, 14, 14, 14, 14, 9};

void print_year_table(const Family& family) {
    print_table_row({"год", "доходы", "расходы", "свои деньги", "долги", "капитал", "ставка"},
                    year_table_widths);
    print_line('-', 91);

    for (const Year_snapshot& snapshot : family.yearly) {
        print_table_row({std::to_string(snapshot.year),
                         format_money(snapshot.income),
                         format_money(snapshot.spending),
                         format_money(snapshot.liquid),
                         format_money(snapshot.debts),
                         format_money(snapshot.net_worth),
                         format_percent(snapshot.key_rate)},
                        year_table_widths);
    }
}

const std::vector<int> month_table_widths = {8, 12, 12, 13, 11, 13, 13};

void print_monthly_table(const Family& family) {
    std::cout << '\n';
    print_table_row({"месяц", "доходы", "расходы", "свои деньги", "карта", "кредиты", "капитал"},
                    month_table_widths);
    print_line('-', 88);

    for (const Month_row& row : family.monthly_rows) {
        print_table_row({format_date(row.date),
                         format_money(row.income),
                         format_money(row.spending),
                         format_money(row.liquid),
                         format_money(row.card_debt),
                         format_money(row.loans),
                         format_money(row.net_worth)},
                        month_table_widths);
    }
}

void print_event_log(const Family& family) {
    std::cout << "\nВажные события:\n";
    int printed_lines = 0;

    for (const std::string& line : family.log) {
        if (printed_lines >= simulation_rules::max_log_lines) {
            std::cout << std::format("  ... и ещё {} событий\n", family.log.size() - printed_lines);
            break;
        }

        std::cout << "  " << line << '\n';
        ++printed_lines;
    }
}

void print_flow_totals(const Family& family) {
    std::cout << "\nДвижение денег за весь период:\n";

    for (int i = 0; i < money_flow_count; ++i) {
        const Money_flow flow = static_cast<Money_flow>(i);

        if (family.total_flows[i] < 1.0) {
            continue;
        }

        const std::string direction = is_income_flow(flow) ? "+" : "-";

        print_table_row({"  " + direction + " " + flow_name(flow), format_money(family.total_flows[i])},
                        {-40, 15});
    }
}

void print_family_report(const Family& family, bool monthly_table) {
    print_title(std::string("Стратегия: ") + strategy_name(family.strategy));

    print_year_table(family);

    if (monthly_table) {
        print_monthly_table(family);
    }

    print_event_log(family);
    print_flow_totals(family);
}

const std::vector<int> comparison_widths = {-24, 14, 14, 13, 12, 8, 9};

void print_demo_comparison(const std::vector<Run_result>& results) {
    print_title("A/B/C-ТЕСТ НА ОДИНАКОВЫХ СОБЫТИЯХ (демонстрационный seed)");

    print_table_row({"стратегия", "капитал", "в ценах 2026", "проценты", "возвраты", "карта", "квартира"},
                    comparison_widths);
    print_line('-', 101);

    for (const Run_result& result : results) {
        print_table_row({strategy_name(result.strategy),
                         format_money(result.net_worth),
                         format_money(result.real_net_worth),
                         format_money(result.interest_paid),
                         format_money(result.tax_refunds),
                         std::to_string(result.card_debt_months),
                         describe_month_index(result.month_flat_bought)},
                        comparison_widths);
    }

    std::cout << "\nОценка каждой модели жизни:\n";
    for (const Run_result& result : results) {
        print_table_row({"  " + std::string(strategy_name(result.strategy)), life_verdict(result)},
                        {-26, -60});
    }
}

const std::vector<int> monte_carlo_widths = {-24, 13, 13, 13, 8, 10, 9, 10};

void print_monte_carlo(const std::array<Strategy_statistics, strategy_count>& statistics, int runs) {
    print_title(std::format("МОНТЕ-КАРЛО: {} СЛУЧАЙНЫХ ЖИЗНЕЙ, КАПИТАЛ В ЦЕНАХ 2026 ГОДА", runs));

    print_table_row({"стратегия", "медиана", "среднее", "худшие 10%", "побед", "просрочки", "карта", "квартира"},
                    monte_carlo_widths);
    print_line('-', 106);

    for (const Strategy_statistics& stats : statistics) {
        std::string flat_time = "нет";
        if (stats.runs_with_flat > 0) {
            const int average_month = static_cast<int>(stats.months_to_flat_sum / stats.runs_with_flat);
            flat_time = describe_month_index(average_month);
        }

        const double wins_share = static_cast<double>(stats.wins) / runs;
        const double overdue_share = static_cast<double>(stats.runs_with_overdue) / runs;
        const double card_months = static_cast<double>(stats.card_debt_months_sum) / runs;

        print_table_row({strategy_name(stats.strategy),
                         format_money(percentile_value(stats.real_net_worths, 0.5)),
                         format_money(mean_value(stats.real_net_worths)),
                         format_money(percentile_value(stats.real_net_worths, 0.1)),
                         format_percent(wins_share),
                         format_percent(overdue_share),
                         format_number(card_months),
                         flat_time},
                        monte_carlo_widths);
    }

    std::cout << "\nВ среднем уплачено процентов и получено налоговых возвратов:\n";
    for (const Strategy_statistics& stats : statistics) {
        print_table_row({"  " + std::string(strategy_name(stats.strategy)),
                         "проценты " + format_money(stats.interest_sum / runs),
                         "возвраты " + format_money(stats.refund_sum / runs)},
                        {-26, -24, -24});
    }
}

void print_conclusion(const std::array<Strategy_statistics, strategy_count>& statistics, int runs) {
    print_title("ВЫВОД");

    int best_median = 0;
    int safest = 0;

    for (int i = 1; i < strategy_count; ++i) {
        const Rub median_now = percentile_value(statistics[i].real_net_worths, 0.5);
        const Rub median_best = percentile_value(statistics[best_median].real_net_worths, 0.5);

        if (median_now > median_best) {
            best_median = i;
        }

        if (statistics[i].runs_with_overdue < statistics[safest].runs_with_overdue) {
            safest = i;
        }
    }

    const Strategy_statistics& renting = statistics[static_cast<int>(Housing_strategy::rent_and_save)];
    const Strategy_statistics& mortgage = statistics[static_cast<int>(Housing_strategy::mortgage_regular)];
    const Strategy_statistics& early = statistics[static_cast<int>(Housing_strategy::mortgage_early_repayment)];

    const Rub renting_median = percentile_value(renting.real_net_worths, 0.5);
    const Rub mortgage_median = percentile_value(mortgage.real_net_worths, 0.5);

    std::cout << std::format("Лучший типичный результат: {}\n", strategy_name(statistics[best_median].strategy));
    std::cout << std::format("Меньше всего жизней с просрочками: {} ({} из {})\n",
                             strategy_name(statistics[safest].strategy),
                             statistics[safest].runs_with_overdue,
                             runs);

    if (mortgage_median > renting_median) {
        std::cout << std::format("Ипотека выгоднее накоплений на {} (медиана, в рублях 2026 года).\n",
                                 format_money(mortgage_median - renting_median));
        std::cout << "Алисе и Бобу выгоднее сразу взять ипотеку, чем снимать жильё и копить.\n";
    } else {
        std::cout << std::format("Накопления выгоднее ипотеки на {} (медиана, в рублях 2026 года).\n",
                                 format_money(renting_median - mortgage_median));
        std::cout << "Алисе и Бобу выгоднее снимать жильё и копить, чем платить высокие проценты по ипотеке.\n";
    }

    const int mortgage_overdue_runs = std::max(mortgage.runs_with_overdue, early.runs_with_overdue);
    if (mortgage_overdue_runs > renting.runs_with_overdue) {
        std::cout << "Но ипотека делает бюджет хрупким: болезнь или потеря работы приводят к долгам.\n";
    }
}


std::string csv_text(const std::string& value) {
    std::string result = "\"";

    for (char symbol : value) {
        if (symbol == '"') {
            result += '"';
        }

        result += symbol;
    }

    result += "\"";

    return result;
}

std::string csv_money(Rub value) {
    return std::to_string(std::llround(value));
}

std::string csv_rate(double value) {
    std::string result = std::format("{:.4f}", value);
    std::replace(result.begin(), result.end(), '.', ',');

    return result;
}

std::filesystem::path utf8_path(const std::string& text) {
    const std::u8string wide_text(reinterpret_cast<const char8_t*>(text.c_str()));

    return std::filesystem::path(wide_text);
}

void write_row(std::ofstream& file, const std::vector<std::string>& cells) {
    for (size_t i = 0; i < cells.size(); ++i) {
        if (i > 0) {
            file << csv_rules::separator;
        }

        file << cells[i];
    }

    file << '\n';
}

std::ofstream open_csv(const std::string& folder, const std::string& file_name) {
    std::ofstream file(utf8_path(folder) / utf8_path(file_name), std::ios::binary);
    file << csv_rules::byte_order_mark;

    return file;
}

void export_start_conditions(const std::string& folder, const Program_options& options) {
    std::ofstream file = open_csv(folder, "01_условия.csv");

    write_row(file, {csv_text("параметр"), csv_text("значение")});
    write_row(file, {csv_text("город"), csv_text("Санкт-Петербург")});
    write_row(file, {csv_text("начало симуляции"),
                     csv_text(std::format("{:04}-{:02}",
                                          simulation_rules::start_year,
                                          simulation_rules::start_month))});
    write_row(file, {csv_text("seed"), std::to_string(options.seed)});
    write_row(file, {csv_text("прогонов Монте-Карло"), std::to_string(options.runs)});

    write_row(file, {csv_text("зарплата Алисы до НДФЛ"), csv_money(family_start::alice_salary)});
    write_row(file, {csv_text("зарплата Алисы после повышения"), csv_money(family_start::alice_promotion_salary)});
    write_row(file, {csv_text("зарплата Боба до НДФЛ"), csv_money(family_start::bob_salary)});
    write_row(file, {csv_text("начальные накопления"),
                     csv_money(family_start::alice_savings + family_start::bob_savings)});
    write_row(file, {csv_text("стоимость машины"), csv_money(family_start::car_value)});

    write_row(file, {csv_text("аренда студии"), csv_money(spb_prices::rent_studio)});
    write_row(file, {csv_text("площадь квартиры, м2"), csv_rate(spb_prices::flat_area_m2)});
    write_row(file, {csv_text("цена за м2"), csv_money(spb_prices::flat_price_per_m2)});
    write_row(file, {csv_text("цена квартиры"),
                     csv_money(spb_prices::flat_area_m2 * spb_prices::flat_price_per_m2)});
    write_row(file, {csv_text("еда на человека в месяц"), csv_money(spb_prices::food_per_adult)});

    write_row(file, {csv_text("ключевая ставка на старте"), csv_rate(bank_rules::start_key_rate)});
    write_row(file, {csv_text("надбавка к ставке по ипотеке"), csv_rate(bank_rules::mortgage_spread)});
    write_row(file, {csv_text("ставка по кредитной карте"), csv_rate(bank_rules::credit_card_rate)});
    write_row(file, {csv_text("доля больничного от заработка"), csv_rate(social_rules::sick_pay_share)});
    write_row(file, {csv_text("цена грамма золота"), csv_money(gold_rules::price_per_gram)});
}

void export_yearly_tables(const std::string& folder, const std::vector<Family>& families) {
    std::ofstream file = open_csv(folder, "02_по_годам.csv");

    write_row(file, {csv_text("стратегия"), csv_text("год"), csv_text("доходы"), csv_text("расходы"),
                     csv_text("свободные деньги"), csv_text("долги"), csv_text("капитал"),
                     csv_text("ключевая ставка")});

    for (const Family& family : families) {
        for (const Year_snapshot& snapshot : family.yearly) {
            write_row(file, {csv_text(strategy_name(family.strategy)),
                             std::to_string(snapshot.year),
                             csv_money(snapshot.income),
                             csv_money(snapshot.spending),
                             csv_money(snapshot.liquid),
                             csv_money(snapshot.debts),
                             csv_money(snapshot.net_worth),
                             csv_rate(snapshot.key_rate)});
        }
    }
}

void export_monthly_tables(const std::string& folder, const std::vector<Family>& families) {
    std::ofstream file = open_csv(folder, "03_по_месяцам.csv");

    write_row(file, {csv_text("стратегия"), csv_text("месяц"), csv_text("доходы"), csv_text("расходы"),
                     csv_text("свободные деньги"), csv_text("долг по карте"),
                     csv_text("остаток кредитов"), csv_text("капитал")});

    for (const Family& family : families) {
        for (const Month_row& row : family.monthly_rows) {
            write_row(file, {csv_text(strategy_name(family.strategy)),
                             csv_text(format_date(row.date)),
                             csv_money(row.income),
                             csv_money(row.spending),
                             csv_money(row.liquid),
                             csv_money(row.card_debt),
                             csv_money(row.loans),
                             csv_money(row.net_worth)});
        }
    }
}

void export_money_flows(const std::string& folder, const std::vector<Family>& families) {
    std::ofstream file = open_csv(folder, "04_движение_денег.csv");

    write_row(file, {csv_text("стратегия"), csv_text("категория"), csv_text("тип"),
                     csv_text("сумма за весь период")});

    for (const Family& family : families) {
        for (int i = 0; i < money_flow_count; ++i) {
            if (family.total_flows[i] < 1.0) {
                continue;
            }

            const Money_flow flow = static_cast<Money_flow>(i);
            const char* kind = is_income_flow(flow) ? "доход" : "расход";

            write_row(file, {csv_text(strategy_name(family.strategy)),
                             csv_text(flow_name(flow)),
                             csv_text(kind),
                             csv_money(family.total_flows[i])});
        }
    }
}

void export_events(const std::string& folder, const std::vector<Family>& families) {
    std::ofstream file = open_csv(folder, "05_события.csv");

    write_row(file, {csv_text("стратегия"), csv_text("дата"), csv_text("событие")});

    for (const Family& family : families) {
        for (const std::string& line : family.log) {
            const std::string date = line.substr(0, 7);
            const std::string text = line.substr(std::min<size_t>(9, line.size()));

            write_row(file, {csv_text(strategy_name(family.strategy)), csv_text(date), csv_text(text)});
        }
    }
}

void export_comparison(const std::string& folder, const std::vector<Run_result>& results) {
    std::ofstream file = open_csv(folder, "06_сравнение_стратегий.csv");

    write_row(file, {csv_text("стратегия"), csv_text("капитал"), csv_text("капитал в рублях 2026"),
                     csv_text("уплачено процентов"), csv_text("возвраты налогов"), csv_text("золото"),
                     csv_text("месяцев с долгом по карте"), csv_text("месяцев с просрочкой"),
                     csv_text("покупка квартиры"), csv_text("оценка")});

    for (const Run_result& result : results) {
        write_row(file, {csv_text(strategy_name(result.strategy)),
                         csv_money(result.net_worth),
                         csv_money(result.real_net_worth),
                         csv_money(result.interest_paid),
                         csv_money(result.tax_refunds),
                         csv_money(result.gold_value),
                         std::to_string(result.card_debt_months),
                         std::to_string(result.overdue_months),
                         csv_text(describe_month_index(result.month_flat_bought)),
                         csv_text(life_verdict(result))});
    }
}

void export_monte_carlo(const std::string& folder,
                        const std::array<Strategy_statistics, strategy_count>& statistics,
                        int runs) {
    std::ofstream file = open_csv(folder, "07_монте_карло.csv");

    write_row(file, {csv_text("стратегия"), csv_text("медиана капитала"), csv_text("средний капитал"),
                     csv_text("худшие 10%"), csv_text("доля побед"), csv_text("доля жизней с просрочкой"),
                     csv_text("месяцев с долгом по карте"), csv_text("средние проценты"),
                     csv_text("средние возвраты"), csv_text("средняя дата покупки квартиры")});

    for (const Strategy_statistics& stats : statistics) {
        std::string flat_time = "нет";
        if (stats.runs_with_flat > 0) {
            const int average_month = static_cast<int>(stats.months_to_flat_sum / stats.runs_with_flat);
            flat_time = describe_month_index(average_month);
        }

        write_row(file, {csv_text(strategy_name(stats.strategy)),
                         csv_money(percentile_value(stats.real_net_worths, 0.5)),
                         csv_money(mean_value(stats.real_net_worths)),
                         csv_money(percentile_value(stats.real_net_worths, 0.1)),
                         csv_rate(static_cast<double>(stats.wins) / runs),
                         csv_rate(static_cast<double>(stats.runs_with_overdue) / runs),
                         csv_rate(static_cast<double>(stats.card_debt_months_sum) / runs),
                         csv_money(stats.interest_sum / runs),
                         csv_money(stats.refund_sum / runs),
                         csv_text(flat_time)});
    }
}

void open_in_file_manager(const std::string& path) {
#if defined(_WIN32)
    const std::string command = "start \"\" \"" + path + "\"";
#elif defined(__APPLE__)
    const std::string command = "open \"" + path + "\"";
#else
    const std::string command = "xdg-open \"" + path + "\" > /dev/null 2>&1";
#endif

    const int result = std::system(command.c_str());

    if (result != 0) {
        std::cout << "Не удалось открыть папку автоматически, откройте её вручную.\n";
    }
}

void export_all_tables(const Program_options& options,
                       const std::vector<Family>& families,
                       const std::vector<Run_result>& results,
                       const std::array<Strategy_statistics, strategy_count>& statistics) {
    std::error_code error;
    std::filesystem::create_directories(utf8_path(options.csv_folder), error);

    if (error) {
        std::cout << std::format("\nНе удалось создать папку {}: {}\n", options.csv_folder, error.message());
        return;
    }

    export_start_conditions(options.csv_folder, options);
    export_yearly_tables(options.csv_folder, families);
    export_monthly_tables(options.csv_folder, families);
    export_money_flows(options.csv_folder, families);
    export_events(options.csv_folder, families);
    export_comparison(options.csv_folder, results);
    export_monte_carlo(options.csv_folder, statistics, options.runs);

    const std::filesystem::path full_path = std::filesystem::absolute(utf8_path(options.csv_folder));
    const std::string shown_path = reinterpret_cast<const char*>(full_path.u8string().c_str());

    print_title("ТАБЛИЦЫ ДЛЯ EXCEL");
    std::cout << std::format("Файлы CSV сохранены в папку: {}\n", shown_path);
    std::cout << "Разделитель - точка с запятой, кодировка UTF-8 с BOM, Excel открывает их двойным кликом.\n";

    if (options.open_folder) {
        open_in_file_manager(shown_path);
    }
}


void prepare_console() {
#if defined(_WIN32)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void print_usage() {
    std::cout << "Использование: alice_bob [--seed N] [--runs N] [--monthly] [--csv-dir ПАПКА] [--no-open]\n";
}

Program_options parse_options(int argc, char** argv) {
    Program_options options;

    for (int i = 1; i < argc; ++i) {
        const std::string argument = argv[i];

        if (argument == "--seed" && i + 1 < argc) {
            options.seed = static_cast<unsigned>(std::strtoul(argv[++i], nullptr, 10));
        } else if (argument == "--runs" && i + 1 < argc) {
            options.runs = std::max(1, std::atoi(argv[++i]));
        } else if (argument == "--monthly") {
            options.monthly_table = true;
        } else if (argument == "--csv-dir" && i + 1 < argc) {
            options.csv_folder = argv[++i];
        } else if (argument == "--no-open") {
            options.open_folder = false;
        } else {
            print_usage();
        }
    }

    return options;
}

int main(int argc, char** argv) {
    prepare_console();

    const Program_options options = parse_options(argc, argv);
    print_assumptions(options);

    const World_timeline demo_world = generate_world(options.seed, simulation_rules::max_months);
    std::vector<Family> families = create_all_families(true);
    const int last_index = run_world(demo_world, families);

    std::vector<Run_result> demo_results;
    for (const Family& family : families) {
        print_family_report(family, options.monthly_table);
        demo_results.push_back(make_result(family, demo_world.months[last_index]));
    }

    print_demo_comparison(demo_results);

    const std::array<Strategy_statistics, strategy_count> statistics =
        run_monte_carlo(options.seed, options.runs);

    print_monte_carlo(statistics, options.runs);
    print_conclusion(statistics, options.runs);

    export_all_tables(options, families, demo_results, statistics);

    return 0;
}
