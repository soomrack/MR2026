#include "log.h"
#include "mortgage.h"
#include "peter.h"
#include "time.h"
#include "world.h"
extern Person peter;
extern World world;
extern Mortgage mortgage;
extern Time time;

#include <algorithm>
#include <cmath>
#include <vector>

Mortgage mortgage_init(unsigned int room_count) {
    Mortgage mortgage;

    mortgage.quad_meters = int_number_generator(36 * room_count, 45 * room_count);
    mortgage.debt = world.cost_per_quad_meter * mortgage.quad_meters;
    mortgage.down_payment = 0.2 * mortgage.debt;
    mortgage.principal_amount = mortgage.debt - mortgage.down_payment;
    mortgage.interest_rate = (world.key_rate + 4) / 100.0 / 12.0;
    mortgage.month = 12 * 10;

    RUB K = mortgage.principal_amount;
    double r = mortgage.interest_rate;
    double t = std::pow(1.0 + r, mortgage.month);

    mortgage.payment = static_cast<RUB>(K * (r * t) / (t - 1));
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


void checking_readiness() {
  if (peter.flat_roomcount == 0 or peter.flat_roomcount == 3) {
    mortgage_init(1);
    if (peter.cash > mortgage.down_payment and
        0.7 * peter.month_income > mortgage.payment+peter.month_mortgage_payment) {
        peter.mortgages.push_back(mortgage_init(1));
        peter.flats.push_back(flat_init(mortgage.debt, mortgage.room_count, mortgage.quad_meters));
        log_event("взял в ипотеку однокомнатную квартиру");
    }
  }

  if (peter.childs == 1) {
    mortgage_init(2);
    if (peter.cash > mortgage.down_payment and
        0.7 * peter.month_income > mortgage.payment+peter.month_mortgage_payment) {
        peter.mortgages.push_back(mortgage_init(2));
        peter.flats.push_back(mortgage_init(1));
        peter.flats.push_back(flat_init(mortgage.debt, mortgage.room_count, mortgage.quad_meters));
        log_event("взял в ипотеку двухкомнатную квартиру");
    }
  }

  if (peter.childs == 2) {
    mortgage_init(3);
    if (peter.cash > mortgage.down_payment and
        0.7 * peter.month_income > mortgage.payment+peter.month_mortgage_payment) {
        peter.mortgages.push_back(mortgage_init(3));
        peter.flats.push_back(flat_init(mortgage.debt, mortgage.room_count, mortgage.quad_meters));
        log_event("взял в ипотеку трёхкомнатную квартиру");
    }
  }
}


void peter_personal_mortgage()
{
    int size = sizeof(peter.mortgages) / sizeof(peter.mortgages[0]); 

    for (int i = 0; i < size; i++) {
        auto& mortgage = peter.mortgages[i]; 

        if (!mortgage.active) {
            continue; 
        }

        if (mortgage.principal_amount <= 0) {
            if (mortgage.room_count > 0) {
                peter.flat = mortgage.room_count;
                peter.flat_cost = mortgage.debt;
                peter.flat_quad_meters = mortgage.quad_meters;
                mortgage.active = false;
                log_event("получил %u-комн. квартиру", peter.flat);
            }
            continue;
        }

        if (peter.cash >= mortgage.payment) {
            peter.cash -= mortgage.payment;
            peter.month_expenses += mortgage.payment;
            peter.month_mortgage_payment += mortgage.payment;
            peter_remove_mental(1, "выплата ипотеки");
        }
        else {
            // Спасают родители 
            peter.month_mortgage_payment += mortgage.payment;
            peter_remove_mental(2, "стыдно перед родителями за нехватку денег");
        }

        RUB interest = mortgage.principal_amount * mortgage.interest_rate;

        if (mortgage.payment > interest) {
            RUB principal_part = mortgage.payment - interest;

            if (principal_part > mortgage.principal_amount) {
                principal_part = mortgage.principal_amount;
            }

            mortgage.principal_amount -= principal_part;
        }

        if (mortgage.principal_amount <= 0) {
            peter.month_mortgage_paid_off = true;
            log_event("ипотека выплачена; квартира: %u-комн.", mortgage.room_count);
        }
    }
}
