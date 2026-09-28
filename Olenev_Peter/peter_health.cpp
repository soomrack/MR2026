#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
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
    if (number_generator(1, 36) == 1)
    {
        peter.count_cold++;
        peter.month_disease = true;
        peter.month_disease_name = "простуда";
        peter.month_disease_damage = 0.1;
        peter_damage(0.1, "простуда");
    }
}


void peter_disease_angina()
{
    if (number_generator(1, 720) == 1)
    {
        peter.count_angina++;
        peter.month_disease = true;
        peter.month_disease_name = "ангина";
        peter.month_disease_damage = 0.5;
        peter_damage(0.5, "ангина");
    }
}


void peter_disease_broken_bone()
{
    if (number_generator(1, 1440) == 1)
    {
        peter.count_broken_bone++;
        peter.month_disease = true;
        peter.month_disease_name = "перелом кости";
        peter.month_disease_damage = 0.3;
        peter_damage(0.3, "перелом кости");
    }
}


void peter_disease_heart_attack()
{
    if (number_generator(1, 7200) == 1)
    {
        peter.count_heart_attack++;
        peter.month_disease = true;
        peter.month_disease_name = "сердечный приступ";
        peter.month_disease_damage = 100.0;
        peter_damage(100.0, "сердечный приступ");
    }
}


void peter_disease()
{
    if (peter.health <= 0.0)
        return;

    peter_disease_cold();

    if (peter.health <= 0.0)
        return;

    peter_disease_angina();

    if (peter.health <= 0.0)
        return;

    peter_disease_broken_bone();

    if (peter.health <= 0.0)
        return;

    peter_disease_heart_attack();
}


void peter_expences_on_healing()
{
    RUB k = world.base_month_expenses;
    double d = number_generator(80, 120) / 100.0;
    peter.expenses_on_healing =
        static_cast<RUB>(peter.month_disease_damage * k * 10 * d);
}


void peter_health()
{
    peter_disease();
    peter_expences_on_healing();
}


// ================= МЕНТАЛЬНОЕ ЗДОРОВЬЕ ====================

void peter_mantality()
{
    if (peter.mental <= 0) {
        peter.health = 0;
        peter.month_disease_name = "ffff";
    }
}
