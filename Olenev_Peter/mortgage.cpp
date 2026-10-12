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

    if (room_count == 1) {
        mortgage.quad_meters = int_number_generator(36, 40);
    }
    
    if (room_count == 2) {
        mortgage.quad_meters = int_number_generator(50, 55);
    }

    if (room_count == 3) {
        mortgage.quad_meters = int_number_generator(75, 100);
    }

    mortgage.debt = world.cost_per_quad_meter * mortgage.quad_meters;
    mortgage.down_payment = 0.2 * mortgage.debt;
    mortgage.principal_amount = mortgage.debt - mortgage.down_payment;
    mortgage.interest_rate = (world.inflation + 4) / 100.0 / 12.0;
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
    flat.rent = room_count * world.rent_cost_per_room;
    flat.used_by_peter = false;

    return flat;
}



bool can_afford_mortgage(Mortgage &candidate, double living_expenses)
{
    if (peter.cash < candidate.down_payment) {
        return false;
    }
    std::vector<Mortgage> mortgages;
    RUB payments = candidate.payment;
    for (Mortgage &mortgage : peter.mortgages) {
        if (mortgage.active and mortgage.principal_amount > 0) {
            mortgages.push_back(mortgage);
            payments += mortgage.payment;
        }
    }
    mortgages.push_back(candidate);

    RUB reserve = 6.0 * (living_expenses + payments);
    RUB cash = peter.cash - candidate.down_payment;
    if (cash < reserve) {
        return false;
    }

    RUB salary_income = peter.month_salary_income;
    YEARS forecast_age = peter.age;
    MONTHES forecast_month = time.month;
    while (!mortgages.empty()) {
        if (++forecast_month > 12) {
            forecast_month = 1;
            forecast_age+=1;
        }
        cash += (peter.retired or forecast_age >= 70) ? peter.pension : salary_income;
        cash -= living_expenses;

        for (Mortgage &mortgage : mortgages) {
            if (mortgage.principal_amount == 0) {
                continue;
            }
            RUB interest = static_cast<RUB>(std::round(
                mortgage.principal_amount * mortgage.interest_rate));
            RUB payment = std::min(mortgage.payment, mortgage.principal_amount + interest);
            if (payment <= interest) {
                return false;
            }
            cash -= payment;
            mortgage.principal_amount -= payment - interest;
        }
        if (cash < reserve) {
            return false;
        }
        mortgages.erase(std::remove_if(mortgages.begin(), mortgages.end(), [](const Mortgage &mortgage) {
            return mortgage.principal_amount == 0;
        }), mortgages.end());
    }
    return true;
}


void checking_readiness()
{
    RUB current_living_expenses = peter.month_expenses_on_food
                               + peter.month_expenses_on_healing
                               + peter.month_expenses_on_entertainment
                               + peter.month_salary_tax + peter.month_rent_tax
                               + peter.month_transport_tax
                               + peter.flats.size() * world.flat_annual_insurance / 12
                               + (peter.car ? world.car_annual_insurance / 12 : 0);
    peter.living_expenses_history.push_back(current_living_expenses);
    if (peter.living_expenses_history.size() > 12) {
        peter.living_expenses_history.erase(peter.living_expenses_history.begin());
    }
    long double living_expenses = 0;
    for (RUB expenses : peter.living_expenses_history) {
        living_expenses += expenses;
    }
    living_expenses = std::max(static_cast<long double>(current_living_expenses),
        living_expenses / peter.living_expenses_history.size());

    if (peter.age >= 60) {
        return;
    }

    unsigned int room_count = 1;
    if (peter.flat_roomcount < 2) {
        room_count = 2;
    }
    else if (peter.flat_roomcount < 3) {
        room_count = 3;
    }

    Mortgage mortgage = mortgage_init(room_count);
    
    if (can_afford_mortgage(mortgage, living_expenses)) {
        peter.cash -= mortgage.down_payment;
        peter.month_expenses += mortgage.down_payment;
        peter.month_down_payment += mortgage.down_payment;
        peter.mortgages.push_back(mortgage);
        peter.flats.push_back(flat_init(mortgage.debt, room_count, mortgage.quad_meters));
        log_event("взял в ипотеку %u-комн. квартиру", room_count);
        peter_personal_flat();  
    }


}


void peter_personal_flat()
{
    for (Flat &flat : peter.flats) {
        if (flat.room_count > peter.flat_roomcount) {
            for (Flat &previous : peter.flats) {
                previous.used_by_peter = false;
            }
            peter.flat = flat.room_count;
            peter.flat_roomcount = flat.room_count;
            flat.used_by_peter = true;
            flat.tenant_search_months = 0;
            flat.tenant_stay_months = 0;
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

        if (peter.cash < payment) {
            RUB help = payment;
            peter.cash += help;
            peter.month_parent_help += help;
            peter.month_income += help;
            peter_remove_mental(2, "стыдно перед родителями за оплату ипотеки");
        }

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
        }
    }
}
