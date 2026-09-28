#include "borrow.h"

static int score_to_consumer_rate(int score)
{
    if (score >= 750) return 22;
    if (score >= 650) return 26;
    if (score >= 550) return 30;
    return 35;
}

static RUB score_to_consumer_limit(int score)
{
    if (score >= 750) return 500000;
    if (score >= 650) return 300000;
    if (score >= 550) return 200000;
    return 100000;
}

static void person_bankrupt(Person& p)
{
    p.bankrupt = true;
    person_change_credit_score(p, -200);

    for (int i = 0; i < p.loan_count; ++i) {
        if (p.loan_accs[i] >= 0) {
            Account* a = bank_get(*p.bank, p.loan_accs[i]);
            if (a) { a->balance = 0; a->active = false; }
        }
    }
    if (p.mfo) {
        for (int i = 0; i < p.mfo_count; ++i) {
            if (p.mfo_accs[i] >= 0) {
                Account* a = bank_get(*p.mfo, p.mfo_accs[i]);
                if (a) { a->balance = 0; a->active = false; }
            }
        }
    }
    if (p.deposit_acc >= 0) {
        Account* a = bank_get(*p.bank, p.deposit_acc);
        if (a) { a->balance = 0; a->active = false; }
    }

    p.cash = 0;
    p.mortgage_debt         = 0;
    p.mortgage_payment      = 0;
    p.mortgage_months_left  = 0;
    p.has_mortgage          = false;
    p.has_apartment         = false;

    person_bankrupt_austerity(p);
}

void borrow_handle_deficit(Person& p, int year, int month)
{
    (void)year; (void)month;
    if (!p.alive || p.bankrupt) return;
    if (p.cash >= 0) return;

    person_ensure_cash(p, 50000);
    if (p.cash >= 0) return;

    // Попытка открыть новый кредит в банке
    if (p.credit_score >= 500 && p.loan_count < MAX_CREDITS) {
        RUB need = -p.cash + 5000;
        RUB limit = score_to_consumer_limit(p.credit_score);
        double rate = score_to_consumer_rate(p.credit_score);

        int acc = bank_open_account(*p.bank, AccountKind::Loan, rate, limit, year, month);
        if (acc >= 0) {
            Account* a = bank_get(*p.bank, acc);
            RUB take = (need < limit) ? need : limit;
            if (a) {
                bank_withdraw(*p.bank, acc, take);
                p.cash += take;
                p.loan_accs[p.loan_count++] = acc;
                p.credits_taken++;
                p.total_borrowed += take;
                person_change_credit_score(p, -5);
            }
        }
    }
    if (p.cash >= 0) return;

    // Микрозайм
    if (p.mfo && p.mfo_count < MAX_CREDITS) {
        RUB need = -p.cash + 5000;
        int acc = bank_open_account(*p.mfo, AccountKind::Loan,
                                    MFO_RATE_YEAR, MFO_LIMIT, year, month);
        if (acc >= 0) {
            Account* a = bank_get(*p.mfo, acc);
            RUB take = (need < MFO_LIMIT) ? need : MFO_LIMIT;
            if (a) {
                bank_withdraw(*p.mfo, acc, take);
                p.cash += take;
                p.mfo_accs[p.mfo_count++] = acc;
                p.micro_taken++;
                p.total_borrowed += take;
                person_change_credit_score(p, -20);
            }
        }
    }
    if (p.cash >= 0) return;

    person_bankrupt(p);
}

/* ============================================================
 * Досрочное погашение: если есть свободные деньги —
 * гасим самый дорогой кредит (сначала МФО).
 * ============================================================ */

void borrow_try_early_repay(Person& p, int year, int month)
{
    (void)year; (void)month;
    if (!p.alive || p.bankrupt) return;

    RUB buffer = person_monthly_expenses(p) * 3;
    if (p.cash <= buffer) return;

    RUB extra = p.cash - buffer;

    // сначала МФО (самый дорогой)
    if (p.mfo) {
        for (int i = 0; i < p.mfo_count; ++i) {
            if (p.mfo_accs[i] < 0) continue;
            Account* a = bank_get(*p.mfo, p.mfo_accs[i]);
            if (!a || !a->active || a->balance == 0) continue;
            RUB pay = (extra < a->balance) ? extra : a->balance;
            bank_deposit(*p.mfo, p.mfo_accs[i], pay);
            p.cash -= pay;
            extra -= pay;
            if (a->balance == 0) person_change_credit_score(p, +20);
            if (extra <= 0) return;
        }
    }

    // потом потреб. кредиты
    for (int i = 0; i < p.loan_count; ++i) {
        if (p.loan_accs[i] < 0) continue;
        Account* a = bank_get(*p.bank, p.loan_accs[i]);
        if (!a || !a->active || a->balance == 0) continue;
        RUB pay = (extra < a->balance) ? extra : a->balance;
        bank_deposit(*p.bank, p.loan_accs[i], pay);
        p.cash -= pay;
        extra -= pay;
        if (a->balance == 0) person_change_credit_score(p, +20);
        if (extra <= 0) return;
    }
}