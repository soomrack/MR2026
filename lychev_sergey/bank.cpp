#include "bank.h"
#include <cstdio>
#include <cmath>

double monthly_rate_from_year_percent(double rate_year_percent)
{
    double r = rate_year_percent / 100.0;
    return std::pow(1.0 + r, 1.0 / 12.0) - 1.0;
}

void bank_init(Bank& b, const std::string& name)
{
    b.name = name;
    b.accounts.clear();
    b.total_interest_earned = 0;
    b.total_interest_paid   = 0;
}

int bank_open_account(Bank& b, AccountKind kind,
                      double rate_year_percent, RUB limit,
                      int year, int month)
{
    Account a;
    a.kind         = kind;
    a.balance      = 0;
    a.limit        = limit;
    a.initial      = 0;
    a.rate_year    = rate_year_percent;
    a.active       = true;
    a.opened_year  = year;
    a.opened_month = month;

    b.accounts.push_back(a);
    return static_cast<int>(b.accounts.size()) - 1;
}

Account* bank_get(Bank& b, int acc)
{
    if (acc < 0 || acc >= static_cast<int>(b.accounts.size())) return nullptr;
    return &b.accounts[acc];
}

bool bank_deposit(Bank& b, int acc, RUB amount)
{
    Account* a = bank_get(b, acc);
    if (!a || !a->active) return false;

    if (a->kind == AccountKind::Loan) {
        if (amount > a->balance) amount = a->balance;
        a->balance -= amount;
    } else {
        a->balance += amount;
    }
    return true;
}

bool bank_withdraw(Bank& b, int acc, RUB amount)
{
    Account* a = bank_get(b, acc);
    if (!a || !a->active) return false;

    if (a->kind == AccountKind::Loan) {
        if (a->balance + amount > a->limit) return false;
        a->balance += amount;
        if (a->initial == 0) a->initial = amount;
        return true;
    }

    if (a->balance < amount) return false;
    a->balance -= amount;
    return true;
}

bool bank_transfer(Bank& b, int from, int to, RUB amount)
{
    if (!bank_withdraw(b, from, amount)) return false;
    if (!bank_deposit(b, to, amount)) {
        bank_deposit(b, from, amount);
        return false;
    }
    return true;
}

#define BODY_CAP_FACTOR 3.0

void bank_accrue_monthly(Bank& b)
{
    for (auto& a : b.accounts) {
        if (!a.active) continue;
        if (a.balance == 0) continue;

        double r_month = monthly_rate_from_year_percent(a.rate_year);

        if (a.kind == AccountKind::Deposit) {
            RUB interest = static_cast<RUB>(
                static_cast<double>(a.balance) * r_month);
            a.balance += interest;
            b.total_interest_paid += interest;
        } else if (a.kind == AccountKind::Loan) {
            RUB interest = static_cast<RUB>(
                static_cast<double>(a.balance) * r_month);
            a.balance += interest;
            b.total_interest_earned += interest;

            if (a.initial > 0) {
                double factor = static_cast<double>(a.balance) /
                                static_cast<double>(a.initial);
                if (factor > BODY_CAP_FACTOR) {
                    a.balance = 0;
                    a.active  = false;
                }
            }
        }
    }
}

RUB bank_balance(const Bank& b, int acc)
{
    if (acc < 0 || acc >= static_cast<int>(b.accounts.size())) return 0;
    return b.accounts[acc].balance;
}

RUB bank_total_by_kind(const Bank& b, AccountKind kind)
{
    RUB sum = 0;
    for (const auto& a : b.accounts)
        if (a.kind == kind) sum += a.balance;
    return sum;
}

void bank_print(const Bank& b)
{
    std::printf("\n=== Банк \"%s\" ===\n", b.name.c_str());
    std::printf("Счетов: %d\n", static_cast<int>(b.accounts.size()));

    for (int i = 0; i < static_cast<int>(b.accounts.size()); ++i) {
        const Account& a = b.accounts[i];
        const char* kind_name = "?";
        switch (a.kind) {
            case AccountKind::Debit:   kind_name = "дебет";  break;
            case AccountKind::Deposit: kind_name = "вклад";  break;
            case AccountKind::Loan:    kind_name = "кредит"; break;
        }
        std::printf("  #%d %-7s баланс=%lld лимит=%lld ставка=%.2f%% активен=%s\n",
                    i, kind_name,
                    (long long)a.balance,
                    (long long)a.limit,
                    a.rate_year,
                    a.active ? "да" : "нет");
    }
    std::printf("Заработано на процентах: %lld\n",
                (long long)b.total_interest_earned);
    std::printf("Выплачено по вкладам:    %lld\n",
                (long long)b.total_interest_paid);
}