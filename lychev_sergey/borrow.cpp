#include "borrow.h"

namespace {

int score_to_consumer_rate(int score) // годовой % в зависимости от рейтинга
{
    if (score >= 750) return 22;
    if (score >= 650) return 26;
    if (score >= 550) return 30;
    return 35;
}

RUB score_to_consumer_limit(int score)
{
    if (score >= 750) return 500000;
    if (score >= 650) return 300000;
    if (score >= 550) return 200000;
    return 100000;
}

void person_bankrupt(Person& p)
{
    p.bankrupt = true;
    person_change_credit_score(p, -200);

    for (int i = 0; i < p.loan_count; ++i)
        if (p.loan_accs[i] >= 0)
            bank_close_account(*p.bank, p.loan_accs[i]);

    if (p.mfo)
        for (int i = 0; i < p.mfo_count; ++i)
            if (p.mfo_accs[i] >= 0)
                bank_close_account(*p.mfo, p.mfo_accs[i]);

    if (p.deposit_acc >= 0)
        bank_close_account(*p.bank, p.deposit_acc);

    p.cash                  = 0;
    p.mortgage_debt         = 0;
    p.mortgage_payment      = 0;
    p.mortgage_months_left  = 0;
    p.has_mortgage          = false;
    p.has_apartment         = false;

    person_bankrupt_austerity(p);
}

} // namespace

void borrow_handle_deficit(Person& p, int year, int month)
{
    if (!p.alive || p.bankrupt) return;
    if (p.cash >= 0) return;

    person_ensure_cash(p, 50000);
    if (p.cash >= 0) return;

    // Потреб. кредит в банке
    if (p.credit_score >= 500 && p.loan_count < MAX_CREDITS) {
        const RUB    need  = -p.cash + 5000;
        const RUB    limit = score_to_consumer_limit(p.credit_score);
        const double rate  = score_to_consumer_rate(p.credit_score);

        const int acc = bank_open_account(*p.bank, AccountKind::Loan,
                                          rate, limit, year, month);
        if (acc >= 0) {
            const RUB take = (need < limit) ? need : limit;
            if (bank_withdraw(*p.bank, acc, take)) {
                p.cash += take;
                p.loan_accs[p.loan_count++] = acc;
                p.credits_taken++;
                p.total_borrowed += take;
                person_change_credit_score(p, -5);
            } else {
                bank_close_account(*p.bank, acc);
            }
        }
    }
    if (p.cash >= 0) return;

    // Микрозайм
    if (p.mfo && p.mfo_count < MAX_CREDITS) {
        const RUB need = -p.cash + 5000;
        const int acc = bank_open_account(*p.mfo, AccountKind::Loan,
                                          MFO_RATE_YEAR, MFO_LIMIT, year, month);
        if (acc >= 0) {
            const RUB take = (need < MFO_LIMIT) ? need : MFO_LIMIT;
            if (bank_withdraw(*p.mfo, acc, take)) {
                p.cash += take;
                p.mfo_accs[p.mfo_count++] = acc;
                p.micro_taken++;
                p.total_borrowed += take;
                person_change_credit_score(p, -20);
            } else {
                bank_close_account(*p.mfo, acc);
            }
        }
    }
    if (p.cash >= 0) return;

    person_bankrupt(p);
}

void borrow_try_early_repay(Person& p, int year, int month)
{
    (void)year; (void)month;
    if (!p.alive || p.bankrupt) return;

    const RUB buffer = person_monthly_expenses(p) * 3;
    if (p.cash <= buffer) return;

    RUB extra = p.cash - buffer;

    // сначала МФО (самый дорогой)
    if (p.mfo) {
        for (int i = 0; i < p.mfo_count; ++i) {
            if (p.mfo_accs[i] < 0) continue;
            Account* a = bank_get(*p.mfo, p.mfo_accs[i]);
            if (!a || !a->active || a->balance == 0) continue;
            const RUB pay = (extra < a->balance) ? extra : a->balance;
            if (!bank_deposit(*p.mfo, p.mfo_accs[i], pay)) continue;
            p.cash -= pay;
            extra  -= pay;
            if (a->balance == 0) person_change_credit_score(p, +20);
            if (extra <= 0) return;
        }
    }

    // затем потреб. кредиты
    for (int i = 0; i < p.loan_count; ++i) {
        if (p.loan_accs[i] < 0) continue;
        Account* a = bank_get(*p.bank, p.loan_accs[i]);
        if (!a || !a->active || a->balance == 0) continue;
        const RUB pay = (extra < a->balance) ? extra : a->balance;
        if (!bank_deposit(*p.bank, p.loan_accs[i], pay)) continue;
        p.cash -= pay;
        extra  -= pay;
        if (a->balance == 0) person_change_credit_score(p, +20);
        if (extra <= 0) return;
    }
}
