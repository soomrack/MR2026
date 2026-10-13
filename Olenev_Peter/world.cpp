#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortgage.h"
extern Person peter;
extern World world;
extern Time time;
#include <random>
#include <algorithm>


double double_number_generator(double min, double max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distr(min, std::max(min, max));
    
    return distr(gen);
}


int int_number_generator(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(min, std::max(min, max));
    
    return distr(gen);
}


void world_init()
{
    // Инфляция
    world.min_inflation = 0.04;
    world.max_inflation = 0.10;
    world.inflation = 0.07;

    // Лечение
    world.factor_expenses_medicine = 1.0;
    world.expenses_healing_cold = 3000;
    world.expenses_healing_angina = 16000;
    world.expenses_healing_broken_bone = 8000;
    world.expenses_healing_caries = 10000;

    // Еда
    world.factor_expenses_food = 1.0;
    world.expenses_food_one_person = 8000;
    world.expenses_food_with_partner = 18000;
    world.expenses_food_with_one_child = 23000;
    world.expenses_food_with_two_childs = 28000;


    // Развлечения
    world.factor_expenses_entertainment = 1.0;
    world.expenses_playing_airsoft = 1500;
    world.chids_entertainment = 5000;
    world.expenses_dating = 4000;
    world.min_salary_for_marriage = 80000;

    world.agama_purchase_cost = 30000;
    world.agama_month_cost = 3000;
    world.agama_lifespan_months = 120;

    // Машина
    world.factor_expenses_car = 1.0;
    world.car_purchase_cost = 1500000;
    world.car_month_cost = 15000;
    world.car_replacement_months = 120;
    world.car_horsepower = 100;
    world.transport_tax_per_horsepower = 24;
    world.flat_annual_insurance = 10000;
    world.car_annual_insurance = 12000;

    // Рента
    world.rent_cost_per_room = 20000;
    world.rental_min_tenant_search_months = 1;
    world.rental_max_tenant_search_months = 3;
    world.rental_min_tenant_stay_months = 6;
    world.rental_max_tenant_stay_months = 24;
    world.factor_cost_per_quad_meter = 1.0;
    world.cost_per_quad_meter = 286000;


    // Дни рождения
    world.birthday_month_girlfriend = int_number_generator(1, 12);
    world.birthday_month_wife = world.birthday_month_girlfriend;
    world.birthday_month_first_child = int_number_generator(1, 12);
    world.birthday_month_second_child = int_number_generator(1, 12);
    world.birthday_month_mother = int_number_generator(1, 12);
    world.birthday_month_father = int_number_generator(1, 12);

    world.birthday_expenses_girlfriend = 6000;
    world.birthday_expenses_wife = 10000;
    world.birthday_expenses_first_child = 7000;
    world.birthday_expenses_second_child = 7000;
    world.birthday_expenses_mother = 5000;
    world.birthday_expenses_father = 5000;

    world.birthday_mental_bonus = 5;

    // Работа
    world.factor_salary_indexation = 1.0;
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

    // Время
    time.year = 2027;
    time.month = 1;
}


void inflation_in_this_year()
{
    world.inflation = double_number_generator(world.min_inflation, world.max_inflation);
    // world.inflation = 0.0;
    
    // Коэффицинеты разброса роста цент относительно инфляции
    double e_min = world.inflation * 0.9;
    double e_max = world.inflation * 1.1;
    

    double ef_grow = double_number_generator(e_min, e_max);
    double em_grow = double_number_generator(e_min, e_max);
    double ee_grow = double_number_generator(e_min, e_max);
    double ec_grow = double_number_generator(e_min, e_max);

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

    // Машина
    world.factor_expenses_car = 1.0 + ec_grow;
    world.car_purchase_cost *= world.factor_expenses_car;
    world.car_month_cost *= world.factor_expenses_car;
    world.car_annual_insurance *= world.factor_expenses_car;

    // Еда
    world.factor_expenses_food = 1.0 + ef_grow;
    world.expenses_food_one_person *= world.factor_expenses_food;
    world.expenses_food_with_partner *= world.factor_expenses_food;
    world.expenses_food_with_one_child *= world.factor_expenses_food;
    world.expenses_food_with_two_childs *= world.factor_expenses_food;

    // Развлечение
    world.factor_expenses_entertainment = 1.0 + ee_grow;
    world.expenses_playing_airsoft *= world.factor_expenses_entertainment;
    world.expenses_dating *= world.factor_expenses_entertainment;
    world.chids_entertainment *= world.factor_expenses_entertainment;
    world.agama_purchase_cost *= world.factor_expenses_entertainment;
    world.agama_month_cost *= world.factor_expenses_entertainment;

    // Дни рождения
    world.birthday_expenses_girlfriend *= world.factor_expenses_entertainment;
    world.birthday_expenses_wife *= world.factor_expenses_entertainment;
    world.birthday_expenses_first_child *= world.factor_expenses_entertainment;
    world.birthday_expenses_second_child *= world.factor_expenses_entertainment;
    world.birthday_expenses_mother *= world.factor_expenses_entertainment;
    world.birthday_expenses_father *= world.factor_expenses_entertainment;
    peter.birthday_expenses *= world.factor_expenses_entertainment;

    // Квартиры
    world.factor_cost_per_quad_meter = 1.0 + qm_grow;
    world.cost_per_quad_meter *= world.factor_cost_per_quad_meter;
    world.factor_expenses_rent = 1.0 + qm_grow;
    world.rent_cost_per_room *=world.factor_expenses_rent;
    world.flat_annual_insurance *= world.factor_expenses_rent;
    for (Flat &flat : peter.flats) {
        // Изменено: инфляция обновляет базу налога на квартиру и арендный доход.
        flat.cost = world.cost_per_quad_meter * flat.quad_meters;
        flat.rent = flat.room_count * world.rent_cost_per_room;
    }


    // Работа
    world.factor_salary_indexation = 1.0 + world.inflation;
    world.min_salary_for_marriage *= world.factor_salary_indexation;

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
    
    peter.salary = peter.salary * (1.0 + world.inflation);
    peter.pension = peter.pension * (1.0 + world.inflation);

}
