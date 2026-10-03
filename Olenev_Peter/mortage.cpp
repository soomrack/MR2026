#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
#include "log.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;

#include <cmath>
#include <algorithm>

void peter_personal_mortage()
{
    if (mortage.principal_amount <= 0) {
        if (mortage.active && mortage.room_count > 0) {
            peter.flat = mortage.room_count;
            // Цена текущей квартиры понадобится при её продаже для расширения.
            peter.flat_cost = mortage.debt;
            // Площадь сохраняется отдельно, так как заявка на новую ипотеку её меняет.
            peter.flat_quad_meters = mortage.quad_meters;
            mortage.active = false;
            log_event("получил %u-комн. квартиру", peter.flat);
        }

        return;
    }

    if (peter.cash >= mortage.payment) {
        peter.month_expenses += mortage.payment;
        peter.month_mortgage_payment += mortage.payment;
        // Учитываем напряжение от выплаты ипотеки.
        peter_remove_mental(1, "выплата ипотеки");
    }
    else {
        peter.month_mortgage_payment += mortage.payment;
        // Учитываем напряжение из-за нехватки денег на ипотеку.
        peter_remove_mental(2, "нехватка денег на ипотеку");
    }

    RUB interest =
        static_cast<RUB>(
            mortage.principal_amount *
            mortage.interest_rate
        );

    if (mortage.payment > interest) {
        RUB principal_part =
            mortage.payment - interest;

        if (principal_part > mortage.principal_amount) {
            principal_part = mortage.principal_amount;
        }

        mortage.principal_amount -= principal_part;
    }

    if (mortage.principal_amount == 0) {
        peter.month_mortgage_paid_off = true;
        log_event("ипотека выплачена; квартира: %u-комн.", mortage.room_count);
    }
}


// Индексирует платёж действующей личной ипотеки раз в год.
void peter_personal_flat_indexation()
{
    if (!mortage.active) {
        return;
    }

    mortage.payment = static_cast<RUB>(
        mortage.payment * (1.0 + world.inflation)
    );
}


void personal_mortage_init(unsigned int room_count, RUB down_payment_funds)
{
    mortage.quad_meters = int_number_generator(36 * room_count, 45 * room_count);
    mortage.debt = world.cost_per_quad_meter * mortage.quad_meters;
    RUB minimum_down_payment = static_cast<RUB>(0.2 * mortage.debt);
    // Стоимость проданной квартиры уменьшает тело следующего кредита.
    mortage.down_payment = std::min(
        mortage.debt,
        std::max(minimum_down_payment, down_payment_funds)
    );
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


// Вся логика покупки и расширения личного жилья собрана в одной функции.
void peter_personal_flat()
{
    if (mortage.active) {
        peter_personal_mortage();
        return;
    }

    if (peter.flat == 0) {
        // Для первой квартиры сохраняем стандартный взнос в 20%.
        personal_mortage_init(1, 0);

        if (peter.cash >= mortage.down_payment and
            0.7 * peter.month_income >= mortage.payment) {
            peter.cash -= mortage.down_payment;
            if (mortage.principal_amount > 0 && mortage.payment > 0) {
                log_event(
                    "взял ипотеку на %u-комн. квартиру: взнос %llu, платёж %llu/мес.",
                    mortage.room_count,
                    mortage.down_payment,
                    mortage.payment
                );
            }
            else {
                log_event("оформил %u-комн. квартиру без долга", mortage.room_count);
            }
        }
        else {
            mortage.principal_amount = 0;
            mortage.payment = 0;
            mortage.room_count = 0;
            mortage.active = false;
            return;
        }
    }
    // После рождения первого ребёнка расширяемся до двухкомнатной квартиры.
    else if (peter.flat == 1 and
             peter_dependent_children_count() >= 1) {
        // Перед покупкой учитываем рыночную цену продаваемой квартиры.
        peter.flat_cost = world.cost_per_quad_meter * peter.flat_quad_meters;
        // Деньги от продажи текущей квартиры идут на первый взнос.
        RUB available_cash = peter.cash + peter.flat_cost;
        // Накопления остаются резервом, в взнос идёт цена проданной квартиры.
        personal_mortage_init(2, peter.flat_cost);

        if (available_cash >= mortage.down_payment and
            0.7 * peter.month_income >= mortage.payment) {
            peter.cash = available_cash - mortage.down_payment;
            if (mortage.principal_amount > 0 && mortage.payment > 0) {
                log_event(
                    "взял ипотеку на %u-комн. квартиру: взнос %llu, платёж %llu/мес.",
                    mortage.room_count,
                    mortage.down_payment,
                    mortage.payment
                );
            }
            else {
                log_event("оформил %u-комн. квартиру без долга", mortage.room_count);
            }
        }
        else {
            mortage.principal_amount = 0;
            mortage.payment = 0;
            mortage.room_count = 0;
            mortage.active = false;
            return;
        }
    }
    // После первого ребёнка расширяемся до трёхкомнатной квартиры для второго.
    else if (peter.flat == 2 and
             peter_dependent_children_count() >= 1) {
        // Перед покупкой учитываем рыночную цену продаваемой квартиры.
        peter.flat_cost = world.cost_per_quad_meter * peter.flat_quad_meters;
        // Деньги от продажи текущей квартиры идут на первый взнос.
        RUB available_cash = peter.cash + peter.flat_cost;
        // Накопления остаются резервом, в взнос идёт цена проданной квартиры.
        personal_mortage_init(3, peter.flat_cost);

        if (available_cash >= mortage.down_payment and
            0.7 * peter.month_income >= mortage.payment) {
            peter.cash = available_cash - mortage.down_payment;
            if (mortage.principal_amount > 0 && mortage.payment > 0) {
                log_event(
                    "взял ипотеку на %u-комн. квартиру: взнос %llu, платёж %llu/мес.",
                    mortage.room_count,
                    mortage.down_payment,
                    mortage.payment
                );
            }
            else {
                log_event("оформил %u-комн. квартиру без долга", mortage.room_count);
            }
        }
        else {
            mortage.principal_amount = 0;
            mortage.payment = 0;
            mortage.room_count = 0;
            mortage.active = false;
            return;
        }
    }

    peter_personal_mortage();
}
