#ifndef BANK_H
#define BANK_H

#include <cstdint>
#include <string>
#include <vector>

using RUB = std::int64_t;

enum class AccountKind { Debit, Deposit, Loan };

// Срок потреб. кредита по умолчанию
constexpr int LOAN_TERM_MONTHS = 36;

// На сколько продлеваем кредит при реструктуризации (срок вышел, а долг остался)
constexpr int LOAN_RESTRUCTURE_MONTHS = 60;

// Законный потолок переплаты по МФО: balance ≤ principal × MFO_CAP_FACTOR
// Применяется ТОЛЬКО к счетам в МФО (у Bank стоит флаг is_mfo)
constexpr double MFO_CAP_FACTOR = 3.0;

struct Account {
    AccountKind kind = AccountKind::Debit;

    RUB balance   = 0;  // Debit/Deposit: остаток; Loan: текущий долг
    RUB principal = 0;  // Loan: суммарно взятое тело
    RUB limit     = 0;  // Loan: максимум тела

    // Loan: график платежей (для Debit/Deposit не используются)
    RUB monthly_payment  = 0;   // аннуитетный платёж в месяц
    int term_months_left = 0;   // сколько месяцев осталось платить

    double rate_year    = 0.0;
    bool   active       = true;
    int    opened_year  = 0;
    int    opened_month = 0;
};

struct Bank {
    std::string          name;
    std::vector<Account> accounts;
    RUB total_interest_earned = 0;  // проценты по кредитам
    RUB total_interest_paid   = 0;  // проценты по вкладам

    // true — это МФО. К её кредитам применяется MFO_CAP_FACTOR.
    bool is_mfo = false;
};

double monthly_rate_from_year_percent(double rate_year_percent);

// Аннуитетный платёж: сколько нужно платить в месяц, чтобы закрыть
// body за months месяцев при ставке rate_year_percent.
RUB bank_annuity(RUB body, double rate_year_percent, int months);

void bank_init(Bank& b, const std::string& name);

int      bank_open_account(Bank& b, AccountKind kind,
                           double rate_year_percent, RUB limit,
                           int year, int month);
Account* bank_get(Bank& b, int acc);

bool bank_deposit (Bank& b, int acc, RUB amount);
bool bank_withdraw(Bank& b, int acc, RUB amount);
bool bank_transfer(Bank& b, int from, int to, RUB amount);

void bank_close_account(Bank& b, int acc);
void bank_accrue_monthly(Bank& b);

RUB  bank_balance(const Bank& b, int acc);
RUB  bank_total_by_kind(const Bank& b, AccountKind kind);
void bank_print(const Bank& b);

#endif
