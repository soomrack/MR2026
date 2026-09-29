#ifndef BANK_H
#define BANK_H

#include <cstdint>
#include <string>
#include <vector>

using RUB = std::int64_t;

enum class AccountKind {
    Debit,
    Deposit,
    Loan
};

struct Account {
    AccountKind kind        = AccountKind::Debit;
    RUB         balance     = 0;
    RUB         limit       = 0;
    RUB         initial     = 0;
    double      rate_year   = 0.0;
    bool        active      = true;
    int         opened_year = 0;
    int         opened_month= 0;
};

struct Bank {
    std::string          name;
    std::vector<Account> accounts;
    RUB                  total_interest_earned = 0;
    RUB                  total_interest_paid   = 0;
};

double monthly_rate_from_year_percent(double rate_year_percent);

void bank_init(Bank& b, const std::string& name);

int  bank_open_account(Bank& b, AccountKind kind,
                       double rate_year_percent, RUB limit,
                       int year, int month);
Account* bank_get(Bank& b, int acc);

bool bank_deposit(Bank& b, int acc, RUB amount);
bool bank_withdraw(Bank& b, int acc, RUB amount);
bool bank_transfer(Bank& b, int from, int to, RUB amount);

void bank_accrue_monthly(Bank& b);

RUB  bank_balance(const Bank& b, int acc);
RUB  bank_total_by_kind(const Bank& b, AccountKind kind);
void bank_print(const Bank& b);

#endif