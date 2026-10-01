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

void peter_damage(double amount, const char *source)
{
    peter.health -= amount;

    if (peter.health < 0.0)
    {
        peter.health = 0.0;
    }

    peter.last_damage_source = source;
}


void peter_disease_cold()
{
    if (int_number_generator(1, 36*peter.mental_factor) == 1)
    {
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
    if (int_number_generator(1, 720*peter.mental_factor) == 1)
    {
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
    if (int_number_generator(1, 1440*peter.mental_factor) == 1)
    {
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
    if (int_number_generator(1, 1440*peter.mental_factor) == 1)
    {
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
    if (int_number_generator(1, 7200*peter.mental_factor) == 1)
    {
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
        peter.mental += 1;
    }
    if (peter.married == true) {
        peter.mental += 2;
    }
    peter.mental += peter.childs;

}



