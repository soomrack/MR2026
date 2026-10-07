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

    return flat;
}



static bool can_afford_mortgage(Mortgage &candidate, long double living_expenses)
{
    if (peter.cash < candidate.down_payment) {
        return false;
    }
    std::vector<Mortgage> loans;
    long double payments = candidate.payment;
    for (const Mortgage &loan : peter.mortgages) {
        if (loan.active && loan.principal_amount > 0) {
            loans.push_back(loan);
            payments += loan.payment;
        }
    }
    loans.push_back(candidate);

    long double reserve = 6.0L * (living_expenses + payments);
    long double cash = static_cast<long double>(peter.cash) - candidate.down_payment;
    if (cash < reserve) {
        return false;
    }

    RUB salary_income = peter.month_salary_income;
    unsigned int forecast_age = peter.age;
    unsigned int forecast_month = time.month;
    while (!loans.empty()) {
        if (++forecast_month > 12) {
            forecast_month = 1;
            ++forecast_age;
        }
        cash += (peter.retired || forecast_age >= 70) ? peter.pension : salary_income;
        cash -= living_expenses;

        for (Mortgage &loan : loans) {
            if (loan.principal_amount == 0) {
                continue;
            }
            RUB interest = static_cast<RUB>(std::round(
                loan.principal_amount * loan.interest_rate));
            RUB payment = std::min(loan.payment, loan.principal_amount + interest);
            if (payment <= interest) {
                return false;
            }
            cash -= payment;
            loan.principal_amount -= payment - interest;
        }
        if (cash < reserve) {
            return false;
        }
        loans.erase(std::remove_if(loans.begin(), loans.end(), [](const Mortgage &loan) {
            return loan.principal_amount == 0;
        }), loans.end());
    }
    return true;
}


void checking_readiness()
{
    RUB current_living_expenses = peter.month_expenses_on_food
                               + peter.month_expenses_on_healing
                               + peter.month_expenses_on_entertainment;
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

        if (peter.cash < payment) {
            RUB help = payment;
            peter.cash += help;
            peter.month_parent_help += help;
            peter.month_income += help;
            peter_remove_mental(2, "стыдно перед родителями за оплату ипотеки");
            log_event("родители помогли с ипотекой: %llu", help);
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
            log_event("ипотека выплачена; квартира: %u-комн.", mortgage.room_count);
        }
    }
}
