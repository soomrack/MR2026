#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
#include "flat.h"
extern Person peter;
extern World world;
extern Mortgage mortage;
extern Time time;


void world_tick()
{
    if (time.month == 12) {
        ++time.year;
        time.month = 1;
        peter.age += 1;
        if (peter.childs >= 1) {
            ++peter.first_child_age;
        }
        if (peter.childs >= 2) {
            ++peter.second_child_age;
        }

        inflation_in_this_year();
        peter_vacation();
    }
    else {
        ++time.month;
    }

    if (peter.health <= 0.0)
        return;

    peter.health -= 1.0 / 12.0;
    if (peter.health <= 0.0) {
        peter.health = 0.0;
        peter.last_damage_source = "старость";
    }
}
