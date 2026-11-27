#pragma once

#include <string>

using YEARS = unsigned int;
using MONTHES = unsigned int;

struct Time {
    MONTHES month;
    YEARS year;
};
extern Time time;

void world_tick();
