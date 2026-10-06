#pragma once

#include <string>

using RUB = unsigned long long int;
using PERCENT = unsigned int;

struct Mortgage {
    RUB debt;
    RUB payment;
    RUB down_payment;
    RUB principal_amount;
    unsigned int month;
    double interest_rate;

    unsigned int quad_meters;
    unsigned int room_count;

    bool active;
};

// Изменено: работаем со списком ипотек, без глобального кредита.
Mortgage mortgage_init(unsigned int room_count);
void checking_readiness();
void peter_personal_mortgage();
void peter_personal_flat();
void peter_personal_flat_indexation();
