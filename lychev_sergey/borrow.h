#ifndef BORROW_H
#define BORROW_H

#include "person.h"
#include "bank.h"

constexpr double MFO_RATE_YEAR = 240.0;
constexpr RUB    MFO_LIMIT     = 100000;

void borrow_handle_deficit (Person& p, int year, int month);
void borrow_try_early_repay(Person& p, int year, int month);

#endif
