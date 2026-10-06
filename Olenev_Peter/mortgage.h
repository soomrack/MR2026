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

extern Mortgage mortgage;

void peter_personal_mortgage();
void personal_mortgage_init(unsigned int room_count, RUB down_payment_funds);
void peter_personal_flat();
void peter_personal_flat_indexation();
