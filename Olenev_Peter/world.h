#pragma once

#include <string>

using RUB = unsigned long long int;
using PERCENT = unsigned int;
using FACTOR = double;
using YEARS = unsigned int;
using MONTHES = unsigned int;

struct World {
    // индексация зп
    FACTOR min_inflation;
    FACTOR max_inflation;
    FACTOR inflation;

    // повышение стоимости продуктов
    FACTOR base_factor_expenses_food;
    FACTOR base_factor_expenses_medicine;
    FACTOR base_factor_expenses_entertainment;
    FACTOR base_factor_cost_per_quad_meter;
    FACTOR base_factor_salary_indexation;

    RUB cost_per_quad_meter;

    RUB cost_healing_cold;
    RUB cost_healing_angina;
    RUB cost_healing_broken_bone;
    RUB cost_healing_caries;
    
    RUB base_expenses_food;
    
    RUB base_expenses_entertainment;


    // повышение стоимости квадратного метра
    FACTOR cost_per_quad_meter_grow;
    FACTOR min_cost_per_quad_meter_grow;
    FACTOR max_cost_per_quad_meter_grow;

    // ключевая ставка ЦБ
    FACTOR key_rate;
    // зарплата
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

