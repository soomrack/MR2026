#include "peter.h"
#include "world.h"
#include "time.h"
// Исправлено: единое название заголовка ипотеки.
#include "mortgage.h"
extern Person peter;
extern World world;
// Изменено: ипотеки хранятся в peter.mortgages.
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

    world.cost_per_quad_meter = 286000;

    // Развлечения
    world.factor_expenses_entertainment = 1.0;
    world.expenses_playing_airsoft = 1500;
    world.chids_entertainment = 5000;
    world.expenses_dating = 4000;
    world.min_salary_for_marriage = 80000;

    // Рента
    world.rental_flat_min_quad_meters = 25;
    world.rental_flat_max_quad_meters = 32;
    world.rental_flat_min_price_factor = 0.82;
    world.rental_flat_max_price_factor = 0.96;
    world.rental_min_rent_per_square_meter = 1200;
    world.rental_max_rent_per_square_meter = 1500;
    world.rental_min_maintenance = 2500;
    world.rental_max_maintenance = 4500;
    world.rental_down_payment_factor = 0.35;
    world.factor_cost_per_quad_meter = 1.0;
    world.key_rate = 2.0;

    // ????????
    world.rental_mortgage_annual_rate = 0.06;
    world.rental_mortgage_months = 180;
    world.rental_max_flats = 4;
    world.rental_min_tenant_search_months = 1;
    world.rental_max_tenant_search_months = 3;
    world.rental_min_tenant_stay_months = 18;
    world.rental_max_tenant_stay_months = 48;
    world.rental_min_damage_period_months = 24;
    world.rental_max_damage_period_months = 72;
    world.rental_min_damage_factor = 0.003;
    world.rental_max_damage_factor = 0.020;
    world.rental_purchase_reserve_factor = 6.0;
    world.rental_min_reserve = 150000;
    world.rental_early_payment_factor = 0.50;

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
    world.expenses_dating *= world.factor_expenses_entertainment;
    world.chids_entertainment *= world.factor_expenses_entertainment;

    // Дни рождения
    world.birthday_expenses_girlfriend *= world.factor_expenses_entertainment;
    world.birthday_expenses_wife *= world.factor_expenses_entertainment;
    world.birthday_expenses_first_child *= world.factor_expenses_entertainment;
    world.birthday_expenses_second_child *= world.factor_expenses_entertainment;
    world.birthday_expenses_mother *= world.factor_expenses_entertainment;
    world.birthday_expenses_father *= world.factor_expenses_entertainment;
    peter.birthday_expenses *= world.factor_expenses_entertainment;


    // Недвижимость (пофиксить)
    world.factor_cost_per_quad_meter = 1.0 + qm_grow;
    world.cost_per_quad_meter *= world.factor_cost_per_quad_meter;
    world.rental_min_rent_per_square_meter *= world.factor_expenses_entertainment;
    world.rental_max_rent_per_square_meter *= world.factor_expenses_entertainment;
    world.rental_min_maintenance *= world.factor_expenses_entertainment;
    world.rental_max_maintenance *= world.factor_expenses_entertainment;
    world.rental_min_reserve *= world.factor_expenses_entertainment;
    // Изменено: индексируем квартиры, фиксированные платежи не увеличиваем.
    peter_personal_flat_indexation();
       
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
