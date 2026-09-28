#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;
#include <random>
#include <algorithm>

int number_generator(int min, int max)
{
    if (min > max)
        std::swap(min, max);

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distr(min, max);

    return distr(gen);
}

void world_init()
{
    world.min_inflation = 4;
    world.max_inflation = 10;
    world.inflation = 7;

    world.base_month_expenses = 1000;

    world.cost_per_quad_meter = 286000;

    world.cost_per_quad_meter_grow = 11;
    world.min_cost_per_quad_meter_grow = 10;
    world.max_cost_per_quad_meter_grow = 35;

    world.key_rate = 20;

    time.year = 2027;
    time.month = 1;
}

void time_init()
{
    time.year = 2027;
    time.month = 1;
}


void inflation_in_this_year()
{
    world.inflation =
        number_generator(
            world.min_inflation,
            world.max_inflation
        );

    world.base_month_expenses =
        static_cast<RUB>(
            world.base_month_expenses *
            (1.0 + world.inflation / 100.0)
        );

    world.cost_per_quad_meter_grow =
        number_generator(
            world.min_cost_per_quad_meter_grow,
            world.max_cost_per_quad_meter_grow
        );

    world.cost_per_quad_meter =
        static_cast<RUB>(
            world.cost_per_quad_meter *
            (1.0 + world.cost_per_quad_meter_grow / 100.0)
        );
}