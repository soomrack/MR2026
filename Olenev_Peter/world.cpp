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

#include <random>
#include <algorithm> // Для std::swap

double double_number_generator(double min, double max)
{
    if (min > max) {
        std::swap(min, max);
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(min, max);
    
    return distr(gen);
}


int int_number_generator(int min, int max)
{
    if (min > max) {
        std::swap(min, max);
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(min, max);
    
    return distr(gen);
}


void world_init()
{
    world.min_inflation = 0.04;
    world.max_inflation = 0.10;
    world.inflation = 0.07;

    world.factor_expenses_food = 1.0;
    world.factor_expenses_medicine = 1.0;
    world.factor_expenses_entertainment = 1.0;
    world.factor_cost_per_quad_meter = 1.0;
    world.factor_salary_indexation = 1.0;

    world.expenses_healing_cold = 3000;
    world.expenses_healing_angina = 16000;
    world.expenses_healing_broken_bone = 8000;
    world.expenses_healing_caries = 10000;

    world.expenses_food_one_person = 8000;
    world.expenses_food_with_partner = 18000;
    world.expenses_food_with_one_child = 23000;
    world.expenses_food_with_two_childs = 28000;

    world.cost_per_quad_meter = 286000;

    world.expenses_playing_airsoft = 1500;
    world.chids_entertainment = 5000;
    world.expenses_dating = 4000;


    world.first_promotion_salary_min = 70000;
    world.first_promotion_salary_max = 90000;

    world.second_promotion_salary_min = 110000;
    world.second_promotion_salary_max = 130000;

    world.third_promotion_salary_min = 150000;
    world.third_promotion_salary_max = 160000;

    world.fourth_promotion_salary_min = 190000;
    world.fourth_promotion_salary_max = 210000;

    world.fifth_promotion_salary_min = 230000;
    world.fifth_promotion_salary_max = 300000;

    time.year = 2027;
    time.month = 1;
}


void inflation_in_this_year()
{
    world.inflation = double_number_generator(world.min_inflation, world.max_inflation);
    
    // Коэффицинеты разброса роста цент относительно инфляции
    double e_min = world.inflation * 0.9;       
    double e_max = world.inflation * 1.1;

    double ef_grow = double_number_generator(e_min, e_max);
    double em_grow = double_number_generator(e_min, e_max);
    double ee_grow = double_number_generator(e_min, e_max);

    double qm_grow = double_number_generator(
        world.inflation * 0.9,
        world.inflation * 1.1
    );

    // Лечение
    world.factor_expenses_medicine = 1.0 + em_grow;
    world.expenses_healing_cold *= world.factor_expenses_medicine;
    world.expenses_healing_angina *= world.factor_expenses_medicine;
    world.expenses_healing_broken_bone *= world.factor_expenses_medicine;
    world.expenses_healing_caries *= world.factor_expenses_medicine;

    // Еда
    world.factor_expenses_food = 1.0 + ef_grow;
    world.expenses_food_one_person *= world.factor_expenses_food;
    world.expenses_food_with_partner *= world.factor_expenses_food;
    world.expenses_food_with_one_child *= world.factor_expenses_food;
    world.expenses_food_with_two_childs *= world.factor_expenses_food;

    // Развлечение
    world.factor_expenses_entertainment = 1.0 + ee_grow;
    world.expenses_playing_airsoft *= world.factor_expenses_entertainment;

    // Недвижимость
    world.factor_cost_per_quad_meter = 1.0 + qm_grow;
    world.cost_per_quad_meter *= world.factor_cost_per_quad_meter;
    
    // Работа
    world.factor_salary_indexation = 1.0 + world.inflation;

    world.first_promotion_salary_min *= (world.inflation + 1.0);
    world.first_promotion_salary_max *= (world.inflation + 1.0);

    world.second_promotion_salary_min *= (world.inflation + 1.0);
    world.second_promotion_salary_max *= (world.inflation + 1.0);

    world.third_promotion_salary_min *= (world.inflation + 1.0);
    world.third_promotion_salary_max *= (world.inflation + 1.0);

    world.fourth_promotion_salary_min *= (world.inflation + 1.0);
    world.fourth_promotion_salary_max *= (world.inflation + 1.0);

    world.fifth_promotion_salary_min *= (world.inflation + 1.0);
    world.fifth_promotion_salary_max *= (world.inflation + 1.0);
}
