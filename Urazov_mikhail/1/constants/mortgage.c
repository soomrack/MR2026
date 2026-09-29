#include <stdio.h>

#include "../types.h"
#include "../constants.h"

#define MORTGAGE_INTEREST_PERCENT 12
#define MORTGAGE_PENALTY_PERCENT 5

CONSTANT_REGISTRATION(mortgage, CONSTANT_ORDER_PAYMENT)

CONSTANT_APPLY(mortgage) {
    if (p->mortgage_debt <= 0 && p->mortgage_overdue <= 0) {
        return;
    }
    int years_left = p->mortgage_years_left > 0 ? p->mortgage_years_left : 1;
    int principal = p->mortgage_debt / years_left;
    if (years_left == 1) {
        principal = p->mortgage_debt;
    }
    int interest = p->mortgage_debt * MORTGAGE_INTEREST_PERCENT / 100;
    int payment = principal + interest;

    p->mortgage_debt -= principal;
    if (p->mortgage_years_left > 0) {
        p->mortgage_years_left--;
    }

    int total_due = payment + p->mortgage_overdue;
    if (p->money >= total_due) {
        p->money -= total_due;
        p->mortgage_overdue = 0;
        printf("\nИпотека: %s выплатил(а) %d рублей (долг %d + проценты %d)", p->name, payment, principal, interest);
        if (total_due != payment) {
            printf(" и погасил(а) просрочку %d рублей", total_due - payment);
        }
        printf(", остаток долга %d.", p->mortgage_debt);
        if (p->mortgage_debt <= 0) {
            printf("\nИпотека полностью погашена!");
        }
        return;
    }

    int penalty = total_due * MORTGAGE_PENALTY_PERCENT / 100;
    p->mortgage_overdue = total_due + penalty;
    printf("\nИпотека: у %s не хватило денег (нужно %d, есть %d)! Платеж просрочен, штраф %d%% = %d рублей.",
        p->name, total_due, p->money, MORTGAGE_PENALTY_PERCENT, penalty);
    printf("\n    долг по просрочке: %d, остаток основного долга: %d", p->mortgage_overdue, p->mortgage_debt);
}
