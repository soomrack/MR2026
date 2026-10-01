#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
#include "log.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;

void peter_salary()
{
    if (peter.dismissioned) {
        peter.salary_this_month = 0;
    }
    else {
        peter.salary_this_month = peter.salary;
    }
}


void peter_vacation()
{
    // Учитываем восстановление во время отпуска.
    peter_add_mental(5, "отпуск");
}


void peter_salary_after_promotion()
{
    unsigned int x = peter.number_of_promotions;

    RUB fp_min = world.first_promotion_salary_min;
    RUB fp_max = world.first_promotion_salary_max;
    RUB sp_min = world.second_promotion_salary_min;
    RUB sp_max = world.second_promotion_salary_max;
    RUB tp_min = world.third_promotion_salary_min;
    RUB tp_max = world.third_promotion_salary_max;
    RUB frp_min = world.fourth_promotion_salary_min;
    RUB frp_max = world.fourth_promotion_salary_max;
    RUB fip_min = world.fifth_promotion_salary_min;
    RUB fip_max = world.fifth_promotion_salary_max;

    if (x == 0) {
        peter.salary = static_cast<RUB>(int_number_generator(fp_min, fp_max));
    }

    else if (x == 1) {
        peter.salary = static_cast<RUB>(int_number_generator(sp_min, sp_max));
    }

    else if (x == 2) {
        peter.salary = static_cast<RUB>(int_number_generator(tp_min, tp_max));
    }

    else if (x == 3) {
        peter.salary = static_cast<RUB>(int_number_generator(frp_min, frp_max));
    }

    else {
        peter.salary = static_cast<RUB>(int_number_generator(fip_min, fip_max));
    }
}


void peter_salary_indexation()
{
    // Текущая зарплата растёт ежегодно вместе с инфляцией.
    peter.salary = static_cast<RUB>(
        peter.salary * (1.0 + world.inflation)
    );
}


void peter_promotion_at_work()
{
    int x = peter.number_of_promotions;
    bool flag = !peter.month_promotion;

    if (x == 0 and flag) {
        if (int_number_generator(1, 5) == 1) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 1 and flag) {
        if (int_number_generator(1, 24) == 1) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 2 and flag) {
        if (int_number_generator(1, 48) == 1) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 3 and flag) {
        if (int_number_generator(1, 114) == 1) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 4 and flag) {
        if (int_number_generator(1, 114) == 1) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }
}


void peter_dismissial_from_work()
{
    if (peter.dismissioned) {
        // Учитываем потерю настроения во время безработицы.
        peter_remove_mental(5, "безработица");
    }

    else if (int_number_generator(1, peter.mental * 6) == 1) {
        peter.dismissioned = true;
        peter.dismissions_count += 1;
        log_event("уволен с работы");
    }
}


void peter_find_work()
{
    if (peter.dismissioned) {
        if (int_number_generator(1, 3) == 1) {
            peter.dismissioned = false;
            log_event("нашёл новую работу");
        }
    }
}


void peter_month_income()
{
    peter_dismissial_from_work();
    peter_find_work();
    peter_promotion_at_work();
    if (peter.month_promotion) {
        log_event("получил повышение; новая зарплата: %llu", peter.salary);
    }
    peter_salary();

    if (!peter.dismissioned) {
        // Учитываем ежемесячную усталость от работы.
        peter_remove_mental(2, "рабочая нагрузка");
    }

    peter.month_income += peter.salary_this_month;
    peter.cash += peter.month_income;
}
