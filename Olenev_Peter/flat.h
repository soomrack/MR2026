#pragma once

#include <string>

using RUB = unsigned long long int;


struct Flat {
    unsigned int room_count;
    unsigned int quad_meters;
    RUB cost;
};

// Изменено: квартиры хранятся в Person, ипотека остаётся отдельно.
Flat flat_init(RUB cost, unsigned int room_count, unsigned int quad_meters);
