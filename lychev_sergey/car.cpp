#include "car.h"
#include <cstdlib>
#include <cmath>

static int roll_range(int lo, int hi)
{
    return lo + std::rand() % (hi - lo + 1);
}

static bool roll_chance(double p)
{
    return (static_cast<double>(std::rand()) / RAND_MAX) < p;
}

void car_init(Car& c)
{
    c.has_car        = true;
    c.age_months     = roll_range(12, 60);
    c.monthly_cost   = 10000;
    c.repair_cost    = 0;
    c.broken         = false;
    c.repairs_total  = 0;
    c.total_spent    = 0;
    c.new_car_price  = 1500000;
}

void car_monthly(Car& c, RUB& cash, RUB& total_expense,
                 int year, int month, const char* owner)
{
    (void)year; (void)month; (void)owner;

    if (!c.has_car) return;

    c.age_months++;

    if (c.broken) {
        RUB taxi = 15000;
        cash -= taxi;
        total_expense += taxi;
        c.total_spent += taxi;

        if (cash >= c.repair_cost) {
            cash -= c.repair_cost;
            total_expense += c.repair_cost;
            c.total_spent += c.repair_cost;
            c.broken = false;
            c.repairs_total++;
        } else if (c.age_months >= 15 * 12 && cash >= 2000000) {
            RUB price = static_cast<RUB>(
                static_cast<double>(c.new_car_price) *
                std::pow(1.0 + c.infl_year / 100.0, c.age_months / 12.0));
            if (cash >= price) {
                cash -= price;
                total_expense += price;
                c.total_spent += price;
                c.age_months = 0;
                c.broken = false;
                c.repair_cost = 0;
            }
        }
        return;
    }

    RUB fuel = c.monthly_cost;
    cash -= fuel;
    total_expense += fuel;
    c.total_spent += fuel;

    c.monthly_cost = static_cast<RUB>(
        static_cast<double>(c.monthly_cost) *
        std::pow(1.0 + 5.0 / 100.0, 1.0 / 12.0));

    if (roll_chance(c.break_chance)) {
        c.broken = true;
        c.repair_cost = static_cast<RUB>(roll_range(24000, 36000));
    }
}