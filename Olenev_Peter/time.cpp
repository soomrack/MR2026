#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;


void world_tick()
{
    if (time.month == 12)
    {
        ++time.year;
        time.month = 1;
        peter.age += 1;

        inflation_in_this_year();
        peter_salary_indexation();
    }
    else
    {
        ++time.month;
    }

    if (peter.health <= 0.0)
        return;

    peter.health -= 1.0 / 12.0;
    peter.mental -= 1;

    if (peter.dismissioned == true)
    {
        peter.mental -= 1;
    }

    if (peter.health <= 0.0)
    {
        peter.health = 0.0;
        peter.last_damage_source = "старость";
    }
}
