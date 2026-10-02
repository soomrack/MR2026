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

void peter_salary()
{
    if (peter.retired) {
        peter.salary_this_month = peter.pension;
    }
    if (peter.dismissioned) {
        peter.salary_this_month = 0;
    }
    else if (!peter.retired) {
        peter.salary_this_month = peter.salary;
    }
}


void peter_vacation()
{
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
    peter.salary = static_cast<RUB>(
        peter.salary * (1.0 + world.inflation)
    );
    peter.pension = static_cast<RUB>(
        peter.pension * (1.0 + world.inflation)
    );
    peter.birthday_expenses = static_cast<RUB>(
        peter.birthday_expenses * (1.0 + world.inflation)
    );
}


void peter_promotion_at_work()
{
    int x = peter.number_of_promotions;
    bool flag = !peter.month_promotion;
    // Хорошее ментальное состояние повышает шансы на продвижение, а плохое — снижает.
    const double career_factor = peter_mental_factor();
    const auto promotion_happened = [career_factor](int base_period) {
        const int adjusted_period = std::max(
            1, static_cast<int>(base_period / career_factor)
        );
        return int_number_generator(1, adjusted_period) == 1;
    };

    if (x == 0 and flag) {
        if (promotion_happened(5)) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 1 and flag) {
        if (promotion_happened(24)) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 2 and flag) {
        if (promotion_happened(48)) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 3 and flag) {
        if (promotion_happened(114)) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }

    if (x == 4 and flag) {
        if (promotion_happened(114)) {
            peter.number_of_promotions++;
            peter.month_promotion = true;
            peter_salary_after_promotion();
        }
    }
}


void peter_dismissial_from_work()
{
    if (peter.dismissioned) {
        peter_remove_mental(5, "безработица");
    }
    else if (int_number_generator(
                 1,
                 std::max(1, static_cast<int>(
                     600 * peter_mental_factor()
                 ))
             ) == 1) {
        peter.dismissioned = true;
        peter.dismissions_count += 1;
        log_event("уволен с работы");
    }
}


void peter_find_work()
{
    if (peter.dismissioned) {
        const int search_period = std::max(1, static_cast<int>(
            3 / peter_mental_factor()
        ));
        if (int_number_generator(1, search_period) == 1) {
            peter.dismissioned = false;
            log_event("нашёл новую работу");
        }
    }
}


void peter_month_income()
{
    if (peter.age >= 70) {
        if (!peter.retired) {
            peter.retired = true;
            peter.dismissioned = false;
            log_event("вышел на пенсию; ежемесячная пенсия: %llu", peter.pension);
        }
        peter_salary();
        peter.month_pension = peter.salary_this_month;
        peter.month_income += peter.month_pension;
        peter.cash += peter.month_income;
        return;
    }

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
