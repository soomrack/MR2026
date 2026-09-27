#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;

#include <cmath>





void peter_mortage()
{
    if (mortage.principal_amount <= 0)
    {
        if (mortage.room_count > 0)
        {
            peter.flat = mortage.room_count;
            mortage.active = false;
        }

        return;
    }

    if (peter.cash >= mortage.payment)
    {
        peter.cash -= mortage.payment;
        peter.month_mortgage_payment += mortage.payment;
        peter.mental -= 1;
    }
    else
    {
        peter.month_mortgage_payment += mortage.payment;
        peter.mental -= 2;
    }

    RUB interest =
        static_cast<RUB>(
            mortage.principal_amount *
            mortage.interest_rate
        );

    if (mortage.payment > interest)
    {
        RUB principal_part =
            mortage.payment - interest;

        if (principal_part > mortage.principal_amount)
        {
            principal_part = mortage.principal_amount;
        }

        mortage.principal_amount -= principal_part;
    }

    if (mortage.principal_amount == 0)
    {
        peter.month_mortgage_paid_off = true;
    }
}


void mortage_init(unsigned int room_count)
{
    mortage.quad_meters = number_generator(36 * room_count, 45 * room_count);
    mortage.debt = world.cost_per_quad_meter * mortage.quad_meters;
    mortage.down_payment = static_cast<RUB>(0.2 * mortage.debt);
    mortage.principal_amount = mortage.debt - mortage.down_payment;
    mortage.interest_rate = (world.key_rate + 4) / 100.0 / 12.0;
    mortage.month = 12 * 10;

    RUB K = mortage.principal_amount;
    double r = mortage.interest_rate;
    double t = std::pow(1.0 + r, mortage.month);

    mortage.payment = static_cast<RUB>(K * (r * t) / (t - 1));
    mortage.room_count = room_count;
    mortage.active = true;
}


void peter_mortage_readiness()
{
    if (mortage.active)
    {
        peter_mortage();
        return;
    }

    if (peter.flat == 0)
    {
        mortage_init(1);

        if (peter.cash >= mortage.down_payment and
            0.7 * peter.month_income >= mortage.payment)
        {
            peter.cash -= mortage.down_payment;
        }
        else
        {
            mortage.principal_amount = 0;
            mortage.payment = 0;
            mortage.room_count = 0;
            mortage.active = false;
            return;
        }
    }
    else if (peter.flat == 1 and
             peter.childs == 1)
    {
        mortage_init(2);

        if (peter.cash + peter.flat_cost >= mortage.down_payment and
            0.7 * peter.month_income >= mortage.payment)
        {
            peter.cash -= mortage.down_payment;
        }
        else
        {
            mortage.principal_amount = 0;
            mortage.payment = 0;
            mortage.room_count = 0;
            mortage.active = false;
            return;
        }
    }
    else if (peter.flat == 2 and
             peter.childs == 2)
    {
        mortage_init(3);

        if (peter.cash + peter.flat_cost >= mortage.down_payment and 
            0.7 * peter.month_income >= mortage.payment)
        {
            peter.cash -= mortage.down_payment;
        }
        else
        {
            mortage.principal_amount = 0;
            mortage.payment = 0;
            mortage.room_count = 0;
            mortage.active = false;
            return;
        }
    }

    peter_mortage();
}
