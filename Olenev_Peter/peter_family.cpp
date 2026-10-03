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

unsigned int peter_dependent_children_count()
{
    unsigned int dependent_children = 0;
    if (peter.childs >= 1 && peter.first_child_age < 20) {
        ++dependent_children;
    }
    if (peter.childs >= 2 && peter.second_child_age < 20) {
        ++dependent_children;
    }
    return dependent_children;
}



void peter_girlfriend()
{
    if (!peter.married && peter.girlfriend == false and
        int_number_generator(1, 50) == 1 and
        peter.girlfriend_possibility == true) {
        peter.girlfriend = true;
        peter_add_mental(10, "начало отношений");
        peter.girlfriend_possibility = false;
        log_event("начал встречаться");
    }

    if (peter.mental < 30) {
        if (peter.girlfriend) {
            log_event("расстался из-за ухудшения ментального состояния");
        }
        peter.girlfriend = false;
        peter.girlfriend_possibility = false;
    }
    else if (!peter.married && peter.mental >= 30 and
             peter.girlfriend == false) {
        peter.girlfriend_possibility = true;
    }

    if (peter.girlfriend == true and
        int_number_generator(1, 500) == 1.0) {
        peter.girlfriend = false;
        peter_remove_mental(10, "расставание");
        peter.girlfriend_possibility = true;
        peter.girlfriend_time = 0;
        log_event("расстался");
    }

    if (peter.girlfriend) {
        peter.girlfriend_time += 1;

        if (int_number_generator(1, 36) == 1) {
            peter_remove_mental(8, "ссора с девушкой");
            log_event("ссора с девушкой");
        }
    }
}


void peter_married()
{
    if (!peter.married and
        peter.girlfriend_time > int_number_generator(24, 36) and
        peter.salary >= world.min_salary_for_marriage) {
        peter.married = true;
        peter.girlfriend = false;
        peter.girlfriend_possibility = false;
        log_event("вступил в брак");
    }

    if (peter.mental < 30) {
        if (peter.married) {
            log_event("развелся из-за ухудшения ментального состояния");
        }
        peter.married = false;
    }

    if (peter.married == true) {
        peter.married_time += 1;

        if (int_number_generator(1, 36) == 1) {
            peter_remove_mental(10, "ссора с женой");
            log_event("ссора с женой");
        }
    }
}


void peter_childrens()
{
    unsigned int ch = peter.childs;

    const double family_factor = peter.mental_factor;
    const auto readiness_period = [family_factor](int min_months, int max_months) {
        return static_cast<unsigned int>(int_number_generator(
            std::max(1, static_cast<int>(min_months / family_factor)),
            std::max(1, static_cast<int>(max_months / family_factor))
        ));
    };

    if (peter.married and peter.mental >= 50 and peter.age < 40 and
        peter.childs < 2) {
        if (peter.married_time > readiness_period(12, 24) and ch == 0) {
            peter.childs += 1;
            peter.first_child_age = 0;
            log_event("родился ребёнок (всего: %d)", peter.childs);
        }

        if (peter.married_time > readiness_period(72, 144) and
            ch == 1 and peter.flat >= 3) {
            peter.childs += 1;
            peter.second_child_age = 0;
            log_event("родился ребёнок (всего: %d)", peter.childs);
        }
    }
}
