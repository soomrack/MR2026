#pragma once

#include <string>

using RUB = unsigned long long int;


struct Flat {
    unsigned int room_count;
    unsigned int quad_meters;
    RUB cost;
    RUB rent;
    bool used_by_peter;
    unsigned int insurance_months_left = 0;
    unsigned int tenant_search_months = 0;
    unsigned int tenant_stay_months = 0;
};

Flat flat_init(RUB cost, unsigned int room_count, unsigned int quad_meters);
