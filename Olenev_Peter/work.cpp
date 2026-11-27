#include "struct_peter.h"
#include "struct_world.h"
#include "struct_time.h"
#include "struct_mortgage.h"
#include "struct_log.h"
#include <algorithm>
extern Person peter;
extern World world;
extern Time time;


void peter_search_first_work()
{
    if (peter.first_work.active == false) {
        if (int_number_generator(1, static_cast<int>(3 / peter.mental_factor)) == 1) {
            peter.first_work.active = true
            log_event("нашёл работу");
        }
    }
}


void peter_search_second_work()
{
    if (peter.second_work.active == false
    and peter.first_work.number_of_promotion == 4
    and int_number_generator(1, static_cast<int>(12 / peter.mental_factor)) == 1) {
        peter.second_work.active = true
        log_event("повысили до тимлмида второй команды (вторая работа)");
    }
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
