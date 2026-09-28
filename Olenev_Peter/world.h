#pragma once

#include <string>

using RUB = unsigned long long int;
using PERCENT = unsigned int;
using YEARS = unsigned int;
using MONTHES = unsigned int;

struct World {
    // индексация зп
    PERCENT min_inflation;
    PERCENT max_inflation;
    PERCENT inflation;

    // повышение стоимости продуктов
    RUB base_month_expenses;
    RUB cost_per_quad_meter;

    // повышение стоимости квадратного метра
    PERCENT cost_per_quad_meter_grow;
    PERCENT min_cost_per_quad_meter_grow;
    PERCENT max_cost_per_quad_meter_grow;

    // ключевая ставка ЦБ
    PERCENT key_rate;
};

extern World world;

void world_init();
void time_init();
void inflation_in_this_year();
int number_generator(int min, int max);

