#include "log.h"
#include "mortgage.h"
#include "peter.h"
#include "time.h"
#include "world.h"
extern Person peter;
extern World world;
extern Time time;

#include <algorithm>
#include <cmath>
#include <vector>

Mortgage mortgage_init(unsigned int room_count) {
    Mortgage mortgage = {}; 

    mortgage.quad_meters = int_number_generator(36 * room_count, 45 * room_count);
    mortgage.debt = world.cost_per_quad_meter * mortgage.quad_meters;
    mortgage.down_payment = 0.2 * mortgage.debt;
    mortgage.principal_amount = mortgage.debt - mortgage.down_payment;
    mortgage.interest_rate = (world.key_rate + 4) / 100.0 / 12.0;
    mortgage.month = 12 * 10;

    RUB K = mortgage.principal_amount;
    double r = mortgage.interest_rate;
    double t = std::pow(1.0 + r, mortgage.month);

    mortgage.payment = K * (r * t) / (t - 1);
    mortgage.room_count = room_count;
    mortgage.active = true;

    return mortgage;
}


Flat flat_init(RUB cost, unsigned int room_count, unsigned int quad_meters) {
    Flat flat;

    flat.quad_meters = quad_meters;
    flat.cost = cost;
    flat.room_count = room_count;

    return flat;
}


void checking_readiness()
{
    unsigned int room_count = 1;
    if (peter.childs == 1 and peter.flat_roomcount < 2) {
        room_count = 2;
    }
    else if (peter.childs >= 2 and peter.flat_roomcount < 3) {
        room_count = 3;
    }

    Mortgage mortgage = mortgage_init(room_count);
    RUB payments = mortgage.payment;
    for (Mortgage &current_mortgage : peter.mortgages) {
        if (current_mortgage.active) {
            payments += current_mortgage.payment;
        }
    }

    RUB living_expenses = peter.month_expenses_on_food
                        + peter.month_expenses_on_healing 
                        + peter.month_expenses_on_entertainment;

    RUB reserve = static_cast<RUB>((payments + living_expenses) * 1.5);

    if (peter.month_parent_help > 0 or peter.cash < mortgage.down_payment 
        or peter.cash - mortgage.down_payment < reserve 
        or payments > 0.7 * peter.month_income
        or payments + living_expenses > 0.9 * peter.month_income) {
        return;
    }

    peter.cash -= mortgage.down_payment;
    peter.month_expenses += mortgage.down_payment;
    peter.month_down_payment += mortgage.down_payment;
    peter.mortgages.push_back(mortgage);
    peter.flats.push_back(flat_init(mortgage.debt, room_count, mortgage.quad_meters));
    log_event("взял в ипотеку %u-комн. квартиру", room_count);
    peter_personal_flat();
}


void peter_personal_flat()
{
    for (const Flat &flat : peter.flats) {
        if (flat.room_count > peter.flat_roomcount) {
            peter.flat = flat.room_count;
            peter.flat_roomcount = flat.room_count;
            peter.flat_cost = flat.cost;
            peter.flat_quad_meters = flat.quad_meters;
            log_event("переехал в %u-комн. квартиру", peter.flat);
        }
    }
}


void peter_mortgage()
{
    for (Mortgage &mortgage : peter.mortgages) {
        if (!mortgage.active) {
            continue;
        }
        if (mortgage.principal_amount == 0) {
            mortgage.active = false;
            continue;
        }

        RUB interest = static_cast<RUB>(std::round(
            mortgage.principal_amount * mortgage.interest_rate));
        RUB payment = std::min(mortgage.payment, mortgage.principal_amount + interest);

        peter.cash += payment;
        peter.month_parent_help += payment;
        peter.month_income += payment;
        peter_remove_mental(2, "стыдно перед родителями за оплату ипотеки");
        log_event("родители оплатили ипотеку: %llu", payment);

        peter.cash -= payment;
        peter.month_expenses += payment;
        peter.month_mortgage_payment += payment;
        if (payment > interest) {
            mortgage.principal_amount -= payment - interest;
        }
        if (mortgage.month > 0) {
            --mortgage.month;
        }
        if (mortgage.principal_amount == 0) {
            mortgage.active = false;
            peter.month_mortgage_paid_off = true;
            log_event("ипотека выплачена; квартира: %u-комн.", mortgage.room_count);
        }
    }
}
