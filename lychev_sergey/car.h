#ifndef CAR_H
#define CAR_H

#include "bank.h"

struct Car {
    bool has_car        = true;
    int  age_months     = 0;
    RUB  monthly_cost   = 10000;
    RUB  repair_cost    = 0;
    bool broken         = false;

    int  repairs_total  = 0;
    RUB  total_spent    = 0;
    RUB  new_car_price  = 1500000;

    double infl_year    = 5.0;
    double break_chance = 0.04;
};

void car_init(Car& c);
void car_monthly(Car& c, RUB& cash, RUB& total_expense,
                 int year, int month, const char* owner);

#endif