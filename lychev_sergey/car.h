#ifndef CAR_H
#define CAR_H

#include "bank.h"

struct Car {
    bool has_car        = true;
    int  age_months     = 0;

    // Текущие цены (инфлируются ежемесячно на infl_year)
    RUB  monthly_cost   = 10000;   // топливо
    RUB  taxi_cost      = 15000;   // такси, пока машина сломана
    RUB  repair_min     = 24000;   // нижняя граница ролла ремонта
    RUB  repair_max     = 90000;   // верхняя граница ролла ремонта
    RUB  new_car_price  = 1500000; // цена новой машины

    RUB  repair_cost    = 0;       // конкретный счёт за ремонт (фиксируется при поломке)
    bool broken         = false;

    int  repairs_total  = 0;
    RUB  total_spent    = 0;

    double infl_year    = 5.0;
    double break_chance = 0.04;
};

void car_init(Car& c);
void car_monthly(Car& c, RUB& cash, RUB& total_expense,
                 int year, int month, const char* owner);

#endif
