#pragma once

#include <string>
#include <vector>
// Добавлено: типы отдельных квартир и ипотек.
#include "mortgage.h"
#include "flat.h"

using RUB = unsigned long long int;
using PERCENT = unsigned int;

using YEARS = unsigned int;
using MONTHES = unsigned int;

using HP = double;
using MP = int;

struct Person {
    //
    YEARS age;
    HP health;
    MP mental;
    double mental_factor;

    // Менталка
    MP month_mental;
    MP month_mental_loss;
    MP month_mental_plus;
    std::vector<std::string> month_mental_losses;
    std::vector<std::string> month_mental_pluses;
    std::vector<std::string> month_damage;
    MP birthday_mental_bonus;


    // Болезни
    int count_cold;
    int count_angina;
    int count_broken_bone;
    int count_heart_attack;
    int count_caries;
    RUB month_expenses_on_healing;

    RUB month_disease_cost;
    RUB month_expenses_on_entertainment;

    std::string last_damage_source;
    std::string month_disease_name;

    double month_disease_damage;
    // Доходы
    RUB cash;
    RUB salary;
    RUB base_salary;
    RUB pension;
    RUB month_income;
    RUB month_salary_income;
    RUB month_pension;
    MONTHES birthday_month;
    RUB birthday_expenses;
    MONTHES vacation_month;

    unsigned int number_of_promotions;
    bool month_promotion;
    bool dismissioned;
    bool retired;

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
    RUB month_expenses_birthdays;

    // Изменено: квартиры и ипотеки накапливаются отдельно.
    std::vector<Mortgage> mortgages;
    std::vector<Flat> flats;
    RUB month_down_payment;
    RUB month_parent_help;
    // Добавлено: последние 12 месяцев расходов на жизнь для прогноза новых ипотек.
    std::vector<RUB> living_expenses_history;


    bool girlfriend;
    bool girlfriend_possibility;
    bool married;

    unsigned int girlfriend_time;
    unsigned int married_time;

    int childs;
    unsigned int first_child_age;
    unsigned int second_child_age;
    bool wife;

    bool car;
    unsigned int flat;
    RUB flat_cost;
    unsigned int flat_quad_meters;
    unsigned int flat_roomcount;
};

extern Person peter;

void peter_init();
void peter_reset_month_stats();

void peter_girlfriend();
void peter_married();
void peter_childrens();
unsigned int peter_dependent_children_count();

void peter_food();
void peter_entertainment();
void peter_birthdays();
void peter_mentality();

void peter_add_mental(MP amount, const char *source);
void peter_remove_mental(MP amount, const char *source);
void peter_month_mental_end();

void peter_disease_cold();
void peter_disease_angina();
void peter_disease_broken_bone();
void peter_disease_heart_attack();
void peter_disease_caries();

void peter_salary();
void peter_salary_after_promotion();
void peter_salary_indexation();
void peter_pension();

void peter_month_expenses();

void peter_promotion_at_work();
void peter_dismissial_from_work();
void peter_find_work();
void peter_month_income();
void peter_vacation();
void peter_month_vacation();

void peter_health();
