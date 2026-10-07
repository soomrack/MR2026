#include <stdio.h>
#include <cmath>
#include <random>
#include <string>
#include <algorithm>
#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortgage.h"
#include "flat.h"

extern Person peter;
extern World world;
// Изменено: ипотеки хранятся в peter.mortgages.
extern Time time;


void peter_init()
{
    peter.age = 21;
    peter.mental = 100;
    peter.mental_factor = peter.mental / 100.0;

    peter.month_mental = 0;
    peter.month_mental_loss = 0;
    peter.month_mental_plus = 0;
    peter.health = 60.0;
    peter.count_cold = 0;
    peter.count_angina = 0;
    peter.count_broken_bone = 0;
    peter.count_heart_attack = 0;
    peter.count_caries = 0;
    peter.month_disease_name = "";
    peter.month_disease_damage = 0.0;
    peter.last_damage_source = "старость";

    peter.cash = 0;
    peter.salary = 40000;
    peter.pension = 30000;
    peter.month_pension = 0;
    peter.birthday_month = 3;
    peter.birthday_expenses = 10000;
    peter.birthday_mental_bonus = 8;
    peter.number_of_promotions = 0;
    peter.month_promotion = false;
    peter.dismissioned = false;
    peter.retired = false;
    peter.dismissions_count = 0;

    peter.girlfriend = false;
    peter.girlfriend_possibility = true;
    peter.married = false;
    peter.girlfriend_time = 0;
    peter.married_time = 0;
    peter.childs = 0;
    peter.first_child_age = 0;
    peter.second_child_age = 0;
    peter.wife = false;

    peter.car = false;
    peter.flat = 0;
    peter.flat_cost = 0;
    peter.flat_quad_meters = 0;
    peter.flat_roomcount = 0;
    peter.mortgages.clear();
    peter.flats.clear();
    // Добавлено: новая симуляция начинается без истории расходов.
    peter.living_expenses_history.clear();
    peter_vacation();
}


void peter_reset_month_stats()
{
    // Доходы
    peter.month_income = 0;
    peter.month_salary_income = 0;
    peter.month_pension = 0;
    peter.salary_this_month = 0;
    peter.month_promotion = false;
    peter.month_dismissed = false;

    // Менталка
    peter.month_mental = 0;
    peter.month_mental_loss = 0;
    peter.month_mental_plus = 0;
    peter.month_mental_losses.clear();
    peter.month_mental_pluses.clear();
    peter.month_damage.clear();

    // Расходы
    peter.month_expenses_on_food = 0;
    peter.month_expenses_on_entertainment = 0;
    peter.month_expenses = 0;

    // Болезни
    peter.month_expenses_on_healing = 0;

    // Ипотека
    // Добавлено: отдельный учёт взноса и помощи родителей.
    peter.month_down_payment = 0;
    peter.month_parent_help = 0;
    peter.month_mortgage_payment = 0;
    peter.month_mortgage_paid_off = false;

    // Развлечения
    peter.month_expenses_playing_airsoft = 0;
    peter.month_expenses_dating = 0;
    peter.month_expenses_chids_entertainment = 0;
    peter.month_expenses_birthdays = 0;
}
