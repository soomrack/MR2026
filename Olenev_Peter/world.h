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
    FACTOR factor_expenses_food;
    RUB expenses_food_one_person;
    RUB expenses_food_with_partner;
    RUB expenses_food_with_one_child;
    RUB expenses_food_with_two_childs;

    // повышение стоимости лечения
    FACTOR factor_expenses_medicine;
    RUB expenses_healing_cold;
    RUB expenses_healing_angina;
    RUB expenses_healing_broken_bone;
    RUB expenses_healing_caries;

    RUB expenses_dating;
    RUB chids_entertainment;

    // повышение стоимости развлечения
    FACTOR factor_expenses_entertainment;
    RUB month_expenses_on_entertainment;
    RUB expenses_playing_airsoft;

    FACTOR factor_salary_indexation;

    RUB cost_per_quad_meter;


    // повышение стоимости квадратного метра
    FACTOR factor_cost_per_quad_meter;
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
