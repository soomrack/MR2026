#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
#include "log.h"
#include <algorithm>
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;

void peter_vacation_month()
{
    peter.vacation_month = int_number_generator(1, 12);
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


void peter_promotion_at_work()
{
    int x = peter.number_of_promotions;
    bool flag = !peter.month_promotion;

    if (x == 0 and flag) {
        if (int_number_generator(1, static_cast<int>(5 / peter.mental_factor)) == 1) {
            peter.number_of_promotions += 1;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 1 and flag) {
        if (int_number_generator(1, static_cast<int>(24 / peter.mental_factor)) == 1) {
            peter.number_of_promotions += 1;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 2 and flag) {
        if ((int_number_generator(1, static_cast<int>(48 / peter.mental_factor)) == 1)) {
            peter.number_of_promotions += 1;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 3 and flag) {
        if ((int_number_generator(1, static_cast<int>(96 / peter.mental_factor)) == 1)) {
            peter.number_of_promotions += 1;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 4 and flag) {
        if ((int_number_generator(1, static_cast<int>(96 / peter.mental_factor)) == 1)) {
            peter.number_of_promotions += 1;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (peter.month_promotion) {
        log_event("получил повышение %u", peter.number_of_promotions);
    }
}


void peter_dismissial_from_work()
{
    if (peter.dismissioned and !peter.retired) {
        if (int_number_generator(1, static_cast<int>(600 * peter.mental_factor)) == 1) {
            peter.dismissioned = true;
            peter.dismissions_count += 1;
            log_event("уволен с работы");
        }
    }
}


void peter_find_work()
{
    if (peter.dismissioned and !peter.retired) {
        if (int_number_generator(1, static_cast<int>(3 / peter.mental_factor)) == 1) {
            peter.dismissioned = false;
            log_event("нашёл новую работу");
        }
    }
}


void peter_salary()
{
    if (!peter.retired) {
        if (peter.dismissioned) {
            peter.salary_this_month = 0;
        }

        else if (!peter.retired) {
            peter.salary_this_month = peter.salary;
            peter_remove_mental(2, "рабочая нагрузка");
        }
    }
}


void peter_pension()
{
    if (peter.age >= 70) {
        peter.retired = true;
        peter.dismissioned = false;
        log_event("вышел на пенсию; ежемесячная пенсия: %llu", peter.pension);
        peter.month_income += peter.pension;
    }
}
