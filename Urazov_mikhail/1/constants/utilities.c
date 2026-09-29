#include <stdio.h>

#include "../types.h"
#include "../constants.h"

CONSTANT_REGISTRATION(utilities, CONSTANT_ORDER_LIVING)

CONSTANT_APPLY(utilities) {
    int growth_percent = 4 + (ESTATUS_PERFECT - (int)w->economy) * 2;
    p->utilities_tariff += p->utilities_tariff * growth_percent / 100;

    int due = p->utilities_tariff + p->utilities_debt;
    int paid = p->money < due ? p->money : due;
    if (paid < 0) {
        paid = 0;
    }
    p->money -= paid;
    p->utilities_debt = due - paid;

    printf("\nКоммунальные услуги (тарифы выросли на %d%%): %s заплатил(а) %d из %d рублей.",
        growth_percent, p->name, paid, due);
    if (p->utilities_debt > 0) {
        printf("\n    не хватило денег, долг за коммуналку: %d рублей.", p->utilities_debt);
    }
}
