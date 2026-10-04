#pragma once

#include <string>

using RUB = unsigned long long int;
using PERCENT = unsigned int;
using FACTOR = double;
using YEARS = unsigned int;
using MONTHES = unsigned int;

struct World {
    // Инфляция
    FACTOR min_inflation;
    FACTOR max_inflation;
    FACTOR inflation;

    // Еда
    FACTOR factor_expenses_food;
    RUB expenses_food_one_person;
    RUB expenses_food_with_partner;
    RUB expenses_food_with_one_child;
    RUB expenses_food_with_two_childs;

    // Лечение
    FACTOR factor_expenses_medicine;
    RUB expenses_healing_cold;
    RUB expenses_healing_angina;
    RUB expenses_healing_broken_bone;
    RUB expenses_healing_caries;

    // Развлечения 
    FACTOR factor_expenses_entertainment;
    RUB month_expenses_on_entertainment;
    RUB expenses_playing_airsoft;
    RUB expenses_dating;
    RUB chids_entertainment;
    RUB min_salary_for_marriage;

    // Дни рождения
    RUB birthday_expenses_girlfriend;
    RUB birthday_expenses_wife;
    RUB birthday_expenses_first_child;
    RUB birthday_expenses_second_child;
    RUB birthday_expenses_mother;
    RUB birthday_expenses_father;

    MONTHES birthday_month_girlfriend;
    MONTHES birthday_month_wife;
    MONTHES birthday_month_first_child;
    MONTHES birthday_month_second_child;
    MONTHES birthday_month_mother;
    MONTHES birthday_month_father;

    MP birthday_mental_bonus;

    // ?????
    unsigned int rental_flat_min_quad_meters;
    unsigned int rental_flat_max_quad_meters;
    FACTOR rental_flat_min_price_factor;
    FACTOR rental_flat_max_price_factor;
    RUB rental_min_rent_per_square_meter;
    RUB rental_max_rent_per_square_meter;
    RUB rental_min_maintenance;
    RUB rental_max_maintenance;
    FACTOR rental_down_payment_factor;
    FACTOR rental_mortgage_annual_rate;
    unsigned int rental_mortgage_months;
    unsigned int rental_max_flats;
    unsigned int rental_min_tenant_search_months;
    unsigned int rental_max_tenant_search_months;
    unsigned int rental_min_tenant_stay_months;
    unsigned int rental_max_tenant_stay_months;
    unsigned int rental_min_damage_period_months;
    unsigned int rental_max_damage_period_months;
    FACTOR rental_min_damage_factor;
    FACTOR rental_max_damage_factor;
    FACTOR rental_purchase_reserve_factor;
    RUB rental_min_reserve;
    FACTOR rental_early_payment_factor;

    // Повышение стоимости квадратного метра
    FACTOR factor_cost_per_quad_meter;
    FACTOR min_cost_per_quad_meter_grow;
    FACTOR max_cost_per_quad_meter_grow;

    // Ключевая ставка ЦБ
    FACTOR key_rate;
    RUB cost_per_quad_meter;

    // Зарплата
    FACTOR factor_salary_indexation;
    RUB first_promotion_salary_min;
    RUB first_promotion_salary_max;
    RUB second_promotion_salary_min;
    RUB second_promotion_salary_max;
    RUB third_promotion_salary_min;
    RUB third_promotion_salary_max;
    RUB fourth_promotion_salary_min;
    RUB fourth_promotion_salary_max;
    RUB fifth_promotion_salary_min;
    RUB fifth_promotion_salary_max;

};

extern World world;

void world_init();
void inflation_in_this_year();
double double_number_generator(double min, double max);
int int_number_generator(int min, int max);
