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
    RUB expenses_playing_airsoft;
    RUB expenses_dating;
    RUB chids_entertainment;
    RUB min_salary_for_marriage;

    RUB agama_purchase_cost;
    RUB agama_month_cost;
    MONTHES agama_lifespan_months;
    FACTOR factor_expenses_car;
    RUB car_purchase_cost;
    RUB car_month_cost;
    MONTHES car_replacement_months;

    unsigned int car_horsepower;
    RUB transport_tax_per_horsepower;
    RUB flat_annual_insurance;
    RUB car_annual_insurance;

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


    unsigned int rental_min_tenant_search_months;
    unsigned int rental_max_tenant_search_months;
    unsigned int rental_min_tenant_stay_months;
    unsigned int rental_max_tenant_stay_months;


    // Повышение стоимости квадратного метра
    FACTOR factor_cost_per_quad_meter;

    // Ключевая ставка ЦБ
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

    RUB rent_cost_per_room;
    double factor_expenses_rent;

};

extern World world;

void world_init();
void inflation_in_this_year();
double double_number_generator(double min, double max);
int int_number_generator(int min, int max);
