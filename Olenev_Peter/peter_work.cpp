#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;

void peter_salary()
{
    if (peter.dismission)
    {
        peter.salary = 0;
        return;
    }
}


void peter_vacation()
{
    peter.mental += 5;
}


void peter_salary_after_promotion()
{
    unsigned int x = peter.number_of_promotions;
    RUB new_base = 0;

    if (x == 0)
    {
        new_base =
            static_cast<RUB>(
                number_generator(30, 50) * 1000ULL
            );
    }
    else if (x == 1)
    {
        new_base =
            static_cast<RUB>(
                number_generator(70, 90) * 1000ULL
            );
    }
    else if (x == 2)
    {
        new_base =
            static_cast<RUB>(
                number_generator(110, 130) * 1000ULL
            );
    }
    else if (x == 3)
    {
        new_base =
            static_cast<RUB>(
                number_generator(150, 170) * 1000ULL
            );
    }
    else if (x == 4)
    {
        new_base =
            static_cast<RUB>(
                number_generator(190, 210) * 1000ULL
            );
    }
    else
    {
        new_base =
            static_cast<RUB>(
                number_generator(230, 300) * 1000ULL
            );
    }

    peter.base_salary = new_base;
    peter.salary =
        peter.dismission ? 0 : peter.base_salary;
}


void peter_salary_indexation()
{
    if (peter.base_salary == 0)
        return;

    peter.base_salary =
        static_cast<RUB>(
            peter.base_salary *
            (1.0 + world.inflation / 100.0)
        );

    if (!peter.dismission)
    {
        peter.salary = peter.base_salary;
    }
}


void peter_promotion_at_work()
{
    if (number_generator(1, 12 * 60 - peter.mental) == 1)
    {
        peter.number_of_promotions++;
        peter.month_promotion = true;
        peter_salary_after_promotion();
    }
}


void peter_dismissial_from_work()
{
    if (peter.dismission)
    {
        peter.mental -= 5;
        return;
    }

    if (number_generator(
            1,
            peter.mental * 6
        ) == 1)
    {
        peter.dismission = true;
        peter.dismissions_count += 1;
        peter.month_dismissed = true;
        peter.salary = 0;
    }
}


void peter_find_work()
{
    if (!peter.dismission)
    {
        return;
    }

    if (number_generator(1, 12 * 60) == 1)
    {
        peter.dismission = false;
        peter.salary = peter.base_salary;
    }
}


void peter_month_income()
{
    peter_dismissial_from_work();
    peter_find_work();
    peter_promotion_at_work();
    peter_salary();

    peter.month_income = peter.salary;
    peter.cash += peter.month_income;
}

