#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
#include "log.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;

#include <random>
#include <algorithm>

void peter_add_mental(MP amount, const char *source)
{
    MP actual_amount = std::min(amount, 120 - peter.mental);
    if (actual_amount <= 0) {
        return;
    }

    peter.mental += actual_amount;
    peter.mental_factor = peter.mental / 100.0;
    peter.month_mental_plus += actual_amount;
    peter.month_mental += actual_amount;
    peter.month_mental_pluses.push_back(
        std::string(source) + ": +" + std::to_string(actual_amount)
    );
}


void peter_remove_mental(MP amount, const char *source)
{
    MP actual_amount = std::min(amount, peter.mental);
    if (actual_amount <= 0) {
        return;
    }

    peter.mental -= actual_amount;
    peter.mental_factor = peter.mental / 100.0;
    peter.month_mental_loss += actual_amount;
    peter.month_mental -= actual_amount;
    peter.month_mental_losses.push_back(
        std::string(source) + ": -" + std::to_string(actual_amount)
    );
}


void peter_damage(double amount, const char *source)
{
    peter.health -= amount;

    if (peter.health < 0.0) {
        peter.health = 0.0;
    }

    peter.last_damage_source = source;
}


void peter_disease_cold()
{
    if (int_number_generator(1, 36 * peter.mental_factor) == 1) {
        peter.count_cold++;
        peter.month_disease = true;
        peter.month_disease_name = "простуда";
        peter.month_disease_damage = 0.1;
        peter.month_expenses_on_healing += world.expenses_healing_cold;
        peter_damage(0.1, "простуда");
        log_event("заболел: простуда");
    }
}


void peter_disease_angina()
{
    if (int_number_generator(1, 720 * peter.mental_factor) == 1) {
        peter.count_angina++;
        peter.month_disease = true;
        peter.month_disease_name = "ангина";
        peter.month_disease_damage = 0.5;
        peter.month_expenses_on_healing += world.expenses_healing_angina;
        peter_damage(0.5, "ангина");
        log_event("заболел: ангина");
    }
}


void peter_disease_broken_bone()
{
    if (int_number_generator(1, 1440 * peter.mental_factor) == 1) {
        peter.count_broken_bone++;
        peter.month_disease = true;
        peter.month_disease_name += "перелом кости ";
        peter.month_disease_damage += 0.3;
        peter.month_expenses_on_healing += world.expenses_healing_broken_bone;
        peter_damage(0.3, "перелом кости");
        log_event("получил травму: перелом кости");
    }
}


void peter_disease_caries()
{
    if (int_number_generator(1, 1440 * peter.mental_factor) == 1) {
        peter.count_caries++;
        peter.month_disease = true;
        peter.month_disease_name += "кариес ";
        peter.month_disease_damage += 0.3;
        peter.month_expenses_on_healing += world.expenses_healing_caries;
        peter_damage(0.3, "перелом кости");
        log_event("заболел: кариес");
    }
}


void peter_disease_heart_attack()
{
    if (int_number_generator(1, 7200 * peter.mental_factor) == 1) {
        peter.count_heart_attack++;
        peter.month_disease = true;
        peter.month_disease_name = "сердечный приступ ";
        peter.month_disease_damage = 100.0;
        peter_damage(100.0, "сердечный приступ");
        log_event("сердечный приступ");
    }
}


void peter_mentality()
{
    if (peter.mental <= 0) {
        peter_damage(100.0, "депрессия");
        peter.month_disease_name = "депрессия ";
        log_event("депрессия");
    }
    if (peter.girlfriend == true) {
        peter_add_mental(1, "отношения");
        peter_remove_mental(4, "повседневные заботы в отношениях");
    }
    if (peter.married == true) {
        peter_add_mental(2, "брак");
        peter_remove_mental(5, "домашние обязанности");
    }
    const unsigned int dependent_children = peter_dependent_children_count();
    if (dependent_children > 0) {
        peter_add_mental(dependent_children, "дети");
        peter_remove_mental(3 * dependent_children, "забота о детях");
    }
}


void peter_month_mental_end()
{
    peter_remove_mental(1, "течение времени");

    if (peter.dismissioned) {
        peter_remove_mental(1, "безработица");
    }

    peter_add_mental(3, "личный отдых");

    int mental_event = int_number_generator(1, 100);
    if (mental_event <= 8) {
        peter_add_mental(6, "хорошие новости");
        log_event("хорошие новости");
    }
    else if (mental_event <= 16) {
        peter_remove_mental(6, "стрессовое событие");
        log_event("стрессовое событие");
    }
    else if (mental_event == 17) {
        peter_add_mental(12, "большой успех");
        log_event("большой успех");
    }
    else if (mental_event == 18) {
        peter_remove_mental(12, "серьёзный кризис");
        log_event("серьёзный кризис");
    }
}
