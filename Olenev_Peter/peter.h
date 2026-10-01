#pragma once

#include <string>

using RUB = unsigned long long int;
using PERCENT = unsigned int;

using YEARS = unsigned int;
using MONTHES = unsigned int;

using HP = double;
using MP = int;

struct Person {
    YEARS age;
    HP health;
    MP mental;
    double mental_factor;


    int count_cold;
    int count_angina;
    int count_broken_bone;
    int count_heart_attack;
    int count_caries;
    RUB month_expenses_on_healing;

    bool month_disease;
    RUB month_disease_cost;
    RUB month_expenses_on_entertainment;

    std::string last_damage_source;
    std::string month_disease_name;

    double month_disease_damage;

    RUB cash;
    RUB salary;
    RUB base_salary;
    RUB month_income;

    unsigned int number_of_promotions;

    bool month_promotion;
    bool dismissioned;

    unsigned int dismissions_count;

    RUB month_mortgage_payment;
    RUB month_expenses;
    RUB expenses_on_healing;
    RUB salary_this_month;
    RUB month_expenses_on_food;
    bool month_dismissed;
    bool month_mortgage_paid_off;

    RUB month_expenses_playing_airsoft;
    RUB month_expenses_dating;
    RUB month_expenses_chids_entertainment;

    bool girlfriend;
    bool girlfriend_possibility;
    bool married;

    unsigned int girlfriend_time;
    unsigned int married_time;

    int childs;
    bool wife;

    bool car;
    unsigned int flat;
    RUB flat_cost;
    // Площадь текущей квартиры нужна для её продажи по рыночной цене.
    unsigned int flat_quad_meters;
};

extern Person peter;

void peter_init();
void peter_reset_month_stats();
void peter_health();

void peter_girlfriend();
void peter_married();
void peter_childrens();
void peter_family();

void peter_food();
void peter_entertainment();
void peter_mentality();
void peter_disease_cold();
void peter_disease_angina();
void peter_disease_broken_bone();
void peter_disease_heart_attack();
void peter_disease_caries();

void peter_salary();
void peter_salary_after_promotion();
void peter_salary_indexation();

void peter_month_expenses();

void peter_promotion_at_work();
void peter_dismissial_from_work();
void peter_find_work();
void peter_month_income();

void peter_health();
