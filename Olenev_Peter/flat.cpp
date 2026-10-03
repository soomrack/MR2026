#include "flat.h"
#include "log.h"
#include "mortage.h"
#include "peter.h"
#include "time.h"
#include "world.h"

#include <algorithm>
#include <cmath>

extern Person peter;
extern Mortage mortage;
extern Time time;
extern World world;

RentalPortfolio rental_portfolio;


// Рассчитывает аннуитетный платёж для инвестиционной ипотеки.
RUB rental_mortgage_payment(RUB principal_amount, double annual_rate,
                            unsigned int months)
{
    double monthly_rate = annual_rate / 12.0;
    double factor = std::pow(1.0 + monthly_rate, months);

    return static_cast<RUB>(
        principal_amount * (monthly_rate * factor) / (factor - 1.0)
    );
}


// Суммирует остаток долга по всем инвестиционным ипотекам.
RUB rental_portfolio_debt()
{
    RUB debt = 0;

    for (const RentalFlat &flat : rental_portfolio.flats) {
        debt += flat.mortgage.principal_amount;
    }

    return debt;
}


// Возвращает текущую рыночную стоимость всех арендных квартир.
RUB rental_portfolio_market_value()
{
    RUB value = 0;

    for (const RentalFlat &flat : rental_portfolio.flats) {
        value += flat.market_value;
    }

    return value;
}


// Считает квартиры, в которых сейчас проживают арендаторы.
unsigned int rental_flats_with_tenants()
{
    unsigned int occupied_flats = 0;

    for (const RentalFlat &flat : rental_portfolio.flats) {
        if (flat.tenant) {
            ++occupied_flats;
        }
    }

    return occupied_flats;
}


// Создаёт пустой инвестиционный портфель перед началом симуляции.
void rental_portfolio_init()
{
    rental_portfolio = {};
}


// Сбрасывает показатели, которые должны отражать только текущий месяц.
void rental_portfolio_reset_month_stats()
{
    rental_portfolio.month_rent_income = 0;
    rental_portfolio.month_mortgage_payment = 0;
    rental_portfolio.month_maintenance_expenses = 0;
    rental_portfolio.month_repair_expenses = 0;
    rental_portfolio.month_early_repayment = 0;
    rental_portfolio.month_purchase_down_payment = 0;
    rental_portfolio.month_tenants_found = 0;
    rental_portfolio.month_tenants_evicted = 0;
    rental_portfolio.month_damage_cases = 0;
}


// Добавляет платёж арендной квартиры в общий месячный расход Петра.
void rental_flat_add_expense(RUB expense)
{
    peter.month_expenses += expense;
}


// Зачисляет арендную плату в деньги и отчёт за текущий месяц.
void rental_flat_receive_rent(RentalFlat &flat)
{
    peter.month_income += flat.monthly_rent;
    peter.cash += flat.monthly_rent;
    flat.total_rent_income += flat.monthly_rent;
    rental_portfolio.month_rent_income += flat.monthly_rent;
    rental_portfolio.total_rent_income += flat.monthly_rent;

    log_event(
        "арендная квартира №%u: получена аренда %llu",
        flat.id,
        flat.monthly_rent
    );
}


// Имитирует поиск жильца: квартира простаивает от одного до трёх месяцев.
void rental_flat_find_tenant(RentalFlat &flat)
{
    ++flat.vacant_months;

    unsigned int search_period = static_cast<unsigned int>(int_number_generator(
        world.rental_min_tenant_search_months,
        world.rental_max_tenant_search_months
    ));

    if (flat.vacant_months < search_period) {
        return;
    }

    flat.tenant = true;
    flat.tenant_months = 0;
    ++flat.tenant_changes;
    ++rental_portfolio.month_tenants_found;
    ++rental_portfolio.total_tenants_found;

    log_event(
        "арендная квартира №%u: найден жилец, плата %llu/мес.",
        flat.id,
        flat.monthly_rent
    );
}


// Имитирует случайное выселение жильца после срока проживания.
void rental_flat_check_tenant_exit(RentalFlat &flat)
{
    unsigned int tenant_stay = static_cast<unsigned int>(int_number_generator(
        world.rental_min_tenant_stay_months,
        world.rental_max_tenant_stay_months
    ));

    if (flat.tenant_months < tenant_stay) {
        return;
    }

    flat.tenant = false;
    flat.tenant_months = 0;
    flat.vacant_months = 0;
    ++flat.evictions;
    ++rental_portfolio.month_tenants_evicted;
    ++rental_portfolio.total_tenants_evicted;

    log_event(
        "арендная квартира №%u: жилец выселен, квартира выставлена в аренду",
        flat.id
    );
}


// Имитирует ущерб от жильцов и оплачивает восстановительный ремонт.
void rental_flat_check_damage(RentalFlat &flat)
{
    unsigned int damage_period = static_cast<unsigned int>(int_number_generator(
        world.rental_min_damage_period_months,
        world.rental_max_damage_period_months
    ));

    if (int_number_generator(1, static_cast<int>(damage_period)) != 1) {
        return;
    }

    double damage_factor = double_number_generator(
        world.rental_min_damage_factor,
        world.rental_max_damage_factor
    );
    RUB repair_expenses = static_cast<RUB>(flat.market_value * damage_factor);

    rental_flat_add_expense(repair_expenses);
    flat.total_repair_expenses += repair_expenses;
    rental_portfolio.month_repair_expenses += repair_expenses;
    rental_portfolio.total_repair_expenses += repair_expenses;
    ++flat.damage_cases;
    ++rental_portfolio.month_damage_cases;
    ++rental_portfolio.total_damage_cases;

    log_event(
        "арендная квартира №%u: ущерб имуществу, ремонт %llu",
        flat.id,
        repair_expenses
    );
}


// Списывает обязательный ежемесячный платёж по ипотеке квартиры.
void rental_flat_pay_mortgage(RentalFlat &flat)
{
    if (!flat.mortgage.active) {
        return;
    }

    RUB interest = static_cast<RUB>(
        flat.mortgage.principal_amount * flat.mortgage.interest_rate
    );
    RUB payment = std::min(
        flat.mortgage.payment,
        flat.mortgage.principal_amount + interest
    );
    RUB principal_payment = payment - interest;

    rental_flat_add_expense(payment);
    flat.mortgage.total_paid += payment;
    rental_portfolio.month_mortgage_payment += payment;
    rental_portfolio.total_mortgage_payment += payment;

    if (principal_payment >= flat.mortgage.principal_amount) {
        flat.mortgage.principal_amount = 0;
    }
    else {
        flat.mortgage.principal_amount -= principal_payment;
    }

    if (flat.mortgage.months_left > 0) {
        --flat.mortgage.months_left;
    }

    log_event(
        "арендная квартира №%u: ипотечный платёж %llu, долг %llu",
        flat.id,
        payment,
        flat.mortgage.principal_amount
    );

    if (flat.mortgage.principal_amount == 0) {
        flat.mortgage.active = false;
        log_event("арендная квартира №%u: ипотека полностью погашена", flat.id);
    }
}


// Вычисляет денежный резерв, который нельзя тратить на новую покупку.
RUB rental_portfolio_reserve()
{
    RUB personal_mortgage_payment = mortage.active ? mortage.payment : 0;
    RUB monthly_obligations = peter.month_expenses_on_food +
                              personal_mortgage_payment;

    for (const RentalFlat &flat : rental_portfolio.flats) {
        monthly_obligations += flat.mortgage.payment;
    }

    return static_cast<RUB>(
        std::max(world.rental_min_reserve, monthly_obligations) *
        world.rental_purchase_reserve_factor
    );
}


// Направляет часть свободных денег на самый крупный долг после роста портфеля.
void rental_flat_early_payment()
{
    if (rental_portfolio.flats.empty()) {
        return;
    }

    RentalFlat *target_flat = nullptr;

    for (RentalFlat &flat : rental_portfolio.flats) {
        if (!flat.mortgage.active) {
            continue;
        }

        if (target_flat == nullptr ||
            flat.mortgage.principal_amount > target_flat->mortgage.principal_amount) {
            target_flat = &flat;
        }
    }

    if (target_flat == nullptr) {
        return;
    }

    bool portfolio_is_complete =
        rental_portfolio.flats.size() >= world.rental_max_flats;
    bool mortgage_is_near_completion =
        target_flat->mortgage.principal_amount <=
        target_flat->purchase_price * 0.35;

    if (!portfolio_is_complete && !mortgage_is_near_completion) {
        return;
    }

    RUB reserve = rental_portfolio_reserve();
    if (peter.cash <= reserve) {
        return;
    }

    RUB early_payment = static_cast<RUB>(
        (peter.cash - reserve) * world.rental_early_payment_factor
    );
    early_payment = std::min(
        early_payment,
        target_flat->mortgage.principal_amount
    );

    if (early_payment == 0) {
        return;
    }

    // Досрочный платёж также отражается в общем месячном денежном потоке.
    rental_flat_add_expense(early_payment);
    target_flat->mortgage.principal_amount -= early_payment;
    target_flat->mortgage.total_early_repayment += early_payment;
    rental_portfolio.month_early_repayment += early_payment;
    rental_portfolio.total_early_repayment += early_payment;

    log_event(
        "арендная квартира №%u: досрочно погашено %llu, долг %llu",
        target_flat->id,
        early_payment,
        target_flat->mortgage.principal_amount
    );

    if (target_flat->mortgage.principal_amount == 0) {
        target_flat->mortgage.active = false;
        log_event("арендная квартира №%u: ипотека полностью погашена", target_flat->id);
    }
}


// Формирует случайный вариант однокомнатной квартиры для оценки покупки.
RentalFlat rental_flat_candidate()
{
    RentalFlat flat = {};

    flat.id = static_cast<unsigned int>(rental_portfolio.flats.size() + 1);
    flat.quad_meters = static_cast<unsigned int>(int_number_generator(
        static_cast<int>(world.rental_flat_min_quad_meters),
        static_cast<int>(world.rental_flat_max_quad_meters)
    ));
    flat.room_count = 1;
    flat.purchase_price = static_cast<RUB>(
        world.cost_per_quad_meter * flat.quad_meters * double_number_generator(
            world.rental_flat_min_price_factor,
            world.rental_flat_max_price_factor
        )
    );
    flat.market_value = flat.purchase_price;
    flat.monthly_rent = flat.quad_meters * static_cast<RUB>(int_number_generator(
        static_cast<int>(world.rental_min_rent_per_square_meter),
        static_cast<int>(world.rental_max_rent_per_square_meter)
    ));
    flat.mortgage.down_payment = static_cast<RUB>(
        flat.purchase_price * world.rental_down_payment_factor
    );
    flat.mortgage.principal_amount =
        flat.purchase_price - flat.mortgage.down_payment;
    flat.mortgage.interest_rate = world.rental_mortgage_annual_rate / 12.0;
    flat.mortgage.months_left = world.rental_mortgage_months;
    flat.mortgage.payment = rental_mortgage_payment(
        flat.mortgage.principal_amount,
        world.rental_mortgage_annual_rate,
        flat.mortgage.months_left
    );
    flat.mortgage.active = true;

    return flat;
}


// Проверяет резерв, окупаемость и долговую нагрузку перед новой ипотекой.
bool rental_flat_purchase_is_reasonable(const RentalFlat &flat)
{
    if (peter.flat == 0 || mortage.active || peter.age >= 64 ||
        rental_portfolio.flats.size() >= world.rental_max_flats) {
        return false;
    }

    RUB maintenance = static_cast<RUB>(int_number_generator(
        static_cast<int>(world.rental_min_maintenance),
        static_cast<int>(world.rental_max_maintenance)
    ));
    RUB reserve = rental_portfolio_reserve();
    RUB required_cash = flat.mortgage.down_payment + reserve;
    // Month income already includes rent received from existing apartments.
    RUB stable_income = peter.month_income;
    bool rent_covers_mortgage = flat.monthly_rent >= static_cast<RUB>(
        (flat.mortgage.payment + maintenance) * 0.85
    );
    bool payment_is_affordable = flat.mortgage.payment <= static_cast<RUB>(
        std::max<RUB>(1, stable_income) * 0.35
    );

    return peter.cash >= required_cash && rent_covers_mortgage &&
           payment_is_affordable;
}


// Покупает одобренную квартиру и сразу выставляет её в аренду.
void rental_flat_buy(const RentalFlat &candidate)
{
    // Взнос проходит через общий месячный денежный поток.
    rental_flat_add_expense(candidate.mortgage.down_payment);
    rental_portfolio.month_purchase_down_payment += candidate.mortgage.down_payment;
    rental_portfolio.flats.push_back(candidate);

    log_event(
        "купил арендную квартиру №%u в Санкт-Петербурге: %u м2, цена %llu, "
        "взнос %llu, платёж %llu/мес.; выставлена в аренду",
        candidate.id,
        candidate.quad_meters,
        candidate.purchase_price,
        candidate.mortgage.down_payment,
        candidate.mortgage.payment
    );
}


// Выполняет все события одной арендной квартиры за текущий месяц.
void rental_flat_month(RentalFlat &flat)
{
    RUB maintenance = static_cast<RUB>(int_number_generator(
        static_cast<int>(world.rental_min_maintenance),
        static_cast<int>(world.rental_max_maintenance)
    ));

    rental_flat_add_expense(maintenance);
    flat.total_maintenance_expenses += maintenance;
    rental_portfolio.month_maintenance_expenses += maintenance;
    rental_portfolio.total_maintenance_expenses += maintenance;
    log_event("арендная квартира №%u: содержание %llu", flat.id, maintenance);

    if (!flat.tenant) {
        rental_flat_find_tenant(flat);
    }

    if (flat.tenant) {
        ++flat.tenant_months;
        rental_flat_receive_rent(flat);
        rental_flat_check_damage(flat);
        rental_flat_check_tenant_exit(flat);
    }

    rental_flat_pay_mortgage(flat);
}


// Обновляет действующие квартиры, досрочно гасит долг и ищет новую покупку.
void peter_investment_flats()
{
    for (RentalFlat &flat : rental_portfolio.flats) {
        rental_flat_month(flat);
    }

    rental_flat_early_payment();

    RentalFlat candidate = rental_flat_candidate();
    if (rental_flat_purchase_is_reasonable(candidate)) {
        rental_flat_buy(candidate);
    }
}


// Индексирует стоимость активов и арендную плату раз в год.
void rental_portfolio_indexation()
{
    for (RentalFlat &flat : rental_portfolio.flats) {
        flat.market_value = static_cast<RUB>(
            flat.market_value * world.factor_cost_per_quad_meter
        );
        flat.monthly_rent = static_cast<RUB>(
            flat.monthly_rent * world.factor_expenses_entertainment
        );
        if (flat.mortgage.active) {
            flat.mortgage.payment = static_cast<RUB>(
                flat.mortgage.payment * (1.0 + world.inflation)
            );
        }
    }
}
