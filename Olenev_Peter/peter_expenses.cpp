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
        peter.month_expenses_on_food = world.base_expenses_food_one_person;
    }
    
    else if (peter.childs == 0) {
        peter.month_expenses_on_food = world.base_expenses_food_with_partner;
    }

    else if (peter.childs == 1) {
        peter.month_expenses_on_food = world.base_expenses_food_with_one_child;
    }
    else {
        peter.month_expenses_on_food = world.base_expenses_food_with_two_childs;
    }
}


void peter_entertainment()
{
    peter.month_expenses_on_entertainment += world.expenses_airsoft * int_number_generator(2, 4);
    peter.month_expenses_on_entertainment += peter.restoraunts;

}

void peter_month_expenses()
{
    peter.month_expenses+=peter.month_expenses_on_food;
    peter.month_expenses+=peter.month_expenses_on_healing;
    
    peter.cash -= peter.month_expenses;
}