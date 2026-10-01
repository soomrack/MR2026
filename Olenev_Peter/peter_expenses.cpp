#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;
#include <random>


void peter_food()
{
    if (peter.girlfriend == false and
    peter.married == false and
    peter.childs == 0) {
        peter.month_expenses_on_food = static_cast<RUB>(
        world.expenses_food_one_person * double_number_generator(0.9, 1.1)
        );
    }
    
    else if (peter.childs == 0) {
        peter.month_expenses_on_food = static_cast<RUB>(
        world.expenses_food_with_partner * double_number_generator(0.9, 1.1)
        );
    }

    else if (peter.childs == 1) {
        peter.month_expenses_on_food = static_cast<RUB>(
        world.expenses_food_with_one_child * double_number_generator(0.9, 1.1)
        );
    }
    else {
        peter.month_expenses_on_food = static_cast<RUB>(
        world.expenses_food_with_two_childs * double_number_generator(0.9, 1.1)
        );
    }
}


void peter_entertainment()
{
    if (peter.age < 35) {
        peter.month_expenses_playing_airsoft = static_cast<RUB>(
        world.expenses_playing_airsoft * int_number_generator(2, 4) 
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_playing_airsoft;
    }

    if (peter.girlfriend) {
        peter.month_expenses_dating = static_cast<RUB>(
        world.expenses_dating * int_number_generator(3, 6)
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_dating;
    }

    if (peter.married) {
        peter.month_expenses_dating = static_cast<RUB>(
        world.expenses_dating * int_number_generator(1, 4)
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_dating;
    }

    if (peter.childs) {
        peter.month_expenses_chids_entertainment = static_cast<RUB>(
        world.chids_entertainment * peter.childs * int_number_generator(2, 6)
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_chids_entertainment;
    }
}

void peter_month_expenses()
{
    peter_entertainment();

    peter.month_expenses+=peter.month_expenses_on_food;
    peter.month_expenses+=peter.month_expenses_on_healing;
    peter.month_expenses+=peter.month_expenses_on_entertainment;

    
    peter.cash -= peter.month_expenses;
}
