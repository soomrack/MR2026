#pragma once

#include <string>

using RUB = unsigned long long int;


struct Flat {
    unsigned int room_count;
    unsigned int quad_meters;
    RUB cost;
};

extern Flat flat;

void peter_personal_mortgage();
void personal_mortgage_init(unsigned int room_count, RUB down_payment_funds);
void peter_personal_flat();
void peter_personal_flat_indexation();
