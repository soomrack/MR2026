#include <stdio.h>
#include <cmath>
#include <random>
#include <string>
#include <algorithm>
#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"

extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;


void peter_init()
{
    peter.age = 21;
    peter.mental = 100;
    peter.health = 60.0;
    peter.count_cold = 0;
    peter.count_angina = 0;
    peter.count_broken_bone = 0;
    peter.count_heart_attack = 0;
    peter.month_disease = false;
    peter.month_disease_name = "";
    peter.month_disease_damage = 0.0;
    peter.last_damage_source = "старость";

    peter.cash = 0;
    peter.salary = 40000;
    peter.base_salary = 40000;
    peter.month_income = 0;
    peter.number_of_promotions = 0;
    peter.month_promotion = false;
    peter.dismission = false;
    peter.dismissions_count = 0;

    peter.month_mortgage_payment = 0;
    peter.month_expenses = 0;
    peter.month_dismissed = false;
    peter.month_mortgage_paid_off = false;
    peter.expenses_on_healing = 0;

    peter.girlfriend = false;
    peter.girlfriend_possibility = true;
    peter.married = false;
    peter.girlfriend_time = 0;
    peter.married_time = 0;
    peter.childs = 0;
    peter.wife = false;

    peter.car = false;
    peter.flat = 0;
    peter.flat_cost = 0;
}


void peter_reset_month_stats()
{
    peter.month_income = 0;
    peter.month_mortgage_payment = 0;
    peter.month_expenses = 0;
    peter.month_promotion = false;
    peter.month_dismissed = false;
    peter.month_disease = false;
    peter.month_mortgage_paid_off = false;
    peter.month_disease_name = "";
    peter.month_disease_damage = 0.0;
}