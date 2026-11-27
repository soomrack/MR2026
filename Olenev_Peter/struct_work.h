#pragma once

using RUB = unsigned long long int;

struct Work {
    unsigned int number_of_promotion;

    RUB junior_salary_min;
    RUB junior_salary_max;

    RUB middle_salary_min;
    RUB middle_salary_max;

    RUB senior_salary_min;
    RUB senior_salary_max;
    
    RUB teamlead_salary_min;
    RUB teamlead_salary_max;

    bool active;

};