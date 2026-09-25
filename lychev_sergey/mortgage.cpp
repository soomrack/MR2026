#include "mortgage.h"
#include <cmath>

RUB mortgage_annuity(RUB body, double rate_year_percent, int months)
{
    double r = monthly_rate_from_year_percent(rate_year_percent);
    double pow_rn = std::pow(1.0 + r, (double)months);
    double coef = (r * pow_rn) / (pow_rn - 1.0);
    return static_cast<RUB>(static_cast<double>(body) * coef);
}

bool mortgage_take(Person& p, RUB price, int year, int month)
{
    (void)year; (void)month;

    if (p.has_mortgage || p.has_apartment) return false;

    RUB down = static_cast<RUB>(
        static_cast<double>(price) * MORTGAGE_DOWN_PERCENT / 100.0);

    RUB total = p.cash;
    if (p.deposit_acc >= 0) total += bank_balance(*p.bank, p.deposit_acc);

    if (total < down) return false;

    RUB body = price - down;
    RUB payment = mortgage_annuity(body, MORTGAGE_RATE_YEAR,
                                   MORTGAGE_YEARS * 12);
    RUB monthly_income = p.salary + p.salary2;
    if (monthly_income == 0) return false;
    if (payment > monthly_income * 5 / 10) return false;

    if (p.cash < down) person_ensure_cash(p, down);
    if (p.cash < down) return false;

    p.cash -= down;

    p.mortgage_debt         = body;
    p.mortgage_payment      = payment;
    p.mortgage_months_left  = MORTGAGE_YEARS * 12;
    p.has_mortgage          = true;
    p.has_apartment         = true;
    p.apartment_price       = price;
    p.rent                  = 0;
    return true;
}

void mortgage_pay(Person& p)
{
    if (!p.has_mortgage) return;
    if (!p.alive || p.bankrupt) return;

    double r_month = monthly_rate_from_year_percent(MORTGAGE_RATE_YEAR);
    RUB interest = static_cast<RUB>(
        static_cast<double>(p.mortgage_debt) * r_month);
    RUB body_part = p.mortgage_payment - interest;
    if (body_part > p.mortgage_debt) body_part = p.mortgage_debt;

    p.mortgage_debt -= body_part;
    p.total_mortgage_interest += interest;
    p.total_interest_paid += interest;
    p.mortgage_interest_this_year += interest;
    p.cash -= p.mortgage_payment;
    p.total_expense += p.mortgage_payment;
    p.mortgage_months_left--;

    if (p.mortgage_debt <= 0 || p.mortgage_months_left <= 0) {
        p.has_mortgage = false;
        p.mortgage_debt = 0;
    }
}

void mortgage_try_early_repay(Person& p, int year, int month)
{
    (void)year; (void)month;
    if (!p.has_mortgage) return;
    if (!p.alive || p.bankrupt) return;

    RUB buffer = person_monthly_expenses(p) * MORTGAGE_BUFFER_MONTHS;
    if (p.cash <= buffer) return;

    RUB extra = p.cash - buffer;
    RUB to_repay = static_cast<RUB>(static_cast<double>(extra) * 0.30);
    if (to_repay > p.mortgage_debt) to_repay = p.mortgage_debt;

    p.mortgage_debt -= to_repay;
    p.cash -= to_repay;

    if (p.mortgage_debt <= 0) {
        p.has_mortgage = false;
        p.mortgage_debt = 0;
    }
}