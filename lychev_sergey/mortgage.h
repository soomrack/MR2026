#ifndef MORTGAGE_H
#define MORTGAGE_H

#include "person.h"
#include "bank.h"

#define MORTGAGE_DOWN_PERCENT   25.0
#define MORTGAGE_RATE_YEAR      11.0
#define MORTGAGE_YEARS          20
#define MORTGAGE_BUFFER_MONTHS  3

RUB mortgage_annuity(RUB body, double rate_year_percent, int months);

bool mortgage_take(Person& p, RUB price, int year, int month);
void mortgage_pay(Person& p);
void mortgage_try_early_repay(Person& p, int year, int month);

#endif