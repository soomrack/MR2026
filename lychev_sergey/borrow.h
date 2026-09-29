#ifndef BORROW_H
#define BORROW_H

#include "person.h"
#include "bank.h"

#define CONSUMER_RATE_YEAR   30.0
#define CONSUMER_LIMIT       300000
#define MFO_RATE_YEAR        240.0
#define MFO_LIMIT            100000
#define BODY_CAP_FACTOR      3.0

void borrow_handle_deficit(Person& p, int year, int month);
void borrow_try_early_repay(Person& p, int year, int month);

#endif