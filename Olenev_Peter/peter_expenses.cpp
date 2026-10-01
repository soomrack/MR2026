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
        int k = int_number_generator(2, 4);
        peter.month_expenses_playing_airsoft = static_cast<RUB>(
        world.expenses_playing_airsoft * k
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_playing_airsoft;
        // Учитываем положительные эмоции от страйкбола.
        peter_add_mental(k, "страйкбол");
    }

    if (peter.girlfriend) {
        int k = int_number_generator(2, 4);
        // Учитываем положительные эмоции от свиданий.
        // Ограничиваем ежемесячный эффект свиданий для баланса ментального состояния.
        peter_add_mental(3, "свидания с девушкой");
        peter.month_expenses_dating = static_cast<RUB>(
        world.expenses_dating * k
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_dating;
    }

    if (peter.married) {
        int k = int_number_generator(1, 4);
        // Учитываем положительные эмоции от свиданий.
        // Ограничиваем ежемесячный эффект свиданий для баланса ментального состояния.
        peter_add_mental(3, "свидания с женой");
        peter.month_expenses_dating = static_cast<RUB>(
        world.expenses_dating * k
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_dating;
    }

    if (peter.childs) {
        int k = int_number_generator(2, 6);
        // Учитываем положительные эмоции от развлечений с детьми.
        peter_add_mental(k, "развлечения с детьми");
        peter.month_expenses_chids_entertainment = static_cast<RUB>(
        world.chids_entertainment * peter.childs * k
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_chids_entertainment;
    }
}

void peter_month_expenses()
{
    peter.month_expenses+=peter.month_expenses_on_food;
    peter.month_expenses+=peter.month_expenses_on_healing;
    peter.month_expenses+=peter.month_expenses_on_entertainment;

    
    peter.cash -= peter.month_expenses;
}
