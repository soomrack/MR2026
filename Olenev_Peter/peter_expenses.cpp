#include "peter.h"
#include "world.h"
#include "time.h"
// Исправлено: единое название заголовка ипотеки.
#include "mortgage.h"
#include "log.h"
extern Person peter;
extern World world;
// Изменено: ипотеки хранятся в peter.mortgages.
extern Time time;
#include <random>

void peter_birthday(MONTHES birthday_month, RUB expenses, MP mental_bonus,
                    const char *person)
{
    if (time.month != birthday_month) {
        return;
    }

    peter.month_expenses_birthdays += expenses;
    peter.month_expenses_on_entertainment += expenses;
    peter_add_mental(mental_bonus, person);
    log_event("день рождения: %s; расходы: %llu", person, expenses);
}


void peter_birthdays()
{
    peter_birthday(
        peter.birthday_month,
        peter.birthday_expenses,
        peter.birthday_mental_bonus,
        "день рождения Петра"
    );
    peter_birthday(
        world.birthday_month_mother,
        world.birthday_expenses_mother,
        world.birthday_mental_bonus,
        "день рождения мамы"
    );
    peter_birthday(
        world.birthday_month_father,
        world.birthday_expenses_father,
        world.birthday_mental_bonus,
        "день рождения папы"
    );

    if (peter.girlfriend) {
        peter_birthday(
            world.birthday_month_girlfriend,
            world.birthday_expenses_girlfriend,
            world.birthday_mental_bonus,
            "день рождения девушки"
        );
    }
    if (peter.married) {
        peter_birthday(
            world.birthday_month_wife,
            world.birthday_expenses_wife,
            world.birthday_mental_bonus,
            "день рождения жены"
        );
    }
    if (peter.childs >= 1) {
        peter_birthday(
            world.birthday_month_first_child,
            world.birthday_expenses_first_child,
            world.birthday_mental_bonus,
            "день рождения первого ребёнка"
        );
    }
    if (peter.childs >= 2) {
        peter_birthday(
            world.birthday_month_second_child,
            world.birthday_expenses_second_child,
            world.birthday_mental_bonus,
            "день рождения второго ребёнка"
        );
    }
}


void peter_food()
{
    const unsigned int dependent_children = peter_dependent_children_count();
    if (peter.girlfriend == false and
    peter.married == false and
    dependent_children == 0) {
        peter.month_expenses_on_food = static_cast<RUB>(
        world.expenses_food_one_person * double_number_generator(0.9, 1.1)
        );
    }
    else if (dependent_children == 0) {
        peter.month_expenses_on_food = static_cast<RUB>(
        world.expenses_food_with_partner * double_number_generator(0.9, 1.1)
        );
    }
    else if (dependent_children == 1) {
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
    const unsigned int dependent_children = peter_dependent_children_count();
    peter_birthdays();
    if (peter.age < 35) {
        int k = int_number_generator(2, 4);
        peter.month_expenses_playing_airsoft = static_cast<RUB>(
        world.expenses_playing_airsoft * k
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_playing_airsoft;
        peter_add_mental(k, "страйкбол");
    }

    if (peter.girlfriend) {
        int k = int_number_generator(2, 4);
        peter_add_mental(k, "свидания с девушкой");
        peter.month_expenses_dating = static_cast<RUB>(
        world.expenses_dating * k
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_dating;
    }

    if (peter.married) {
        int k = int_number_generator(1, 4);
        peter_add_mental(k, "свидания с женой");
        peter.month_expenses_dating = static_cast<RUB>(
        world.expenses_dating * k
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_dating;
    }

    if (dependent_children > 0) {
        int k = int_number_generator(2, 6);
        peter_add_mental(k, "развлечения с детьми");
        peter.month_expenses_chids_entertainment = static_cast<RUB>(
        world.chids_entertainment * dependent_children * k
        * double_number_generator(0.9, 1.1)
        );
        peter.month_expenses_on_entertainment += peter.month_expenses_chids_entertainment;
    }
}


// Изменено: списываем только расходы на жизнь, ипотека уже оплачена отдельно.
void peter_month_expenses()
{
    RUB expenses = peter.month_expenses_on_food + peter.month_expenses_on_healing
        + peter.month_expenses_on_entertainment;
    if (peter.cash < expenses) {
        // Добавлено: помощь родителей предотвращает переполнение беззнакового cash.
        RUB help = expenses - peter.cash;
        peter.cash += help;
        peter.month_income += help;
        peter.month_parent_help += help;
        log_event("родители помогли с расходами: %llu", help);
    }
    peter.cash -= expenses;
    peter.month_expenses += expenses;
}
