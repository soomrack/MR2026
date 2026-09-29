#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../kredit.h"
#include "../utils.h"

CONSTANT_REGISTRATION(utilities, CONSTANT_ORDER_LIVING)

CONSTANT_APPLY(utilities) {
    if (w->month == 1) {
        int growth_percent = 4 + (ESTATUS_PERFECT - (int)w->economy) * 2;
        p->utilities_tariff += p->utilities_tariff * growth_percent / 100;
        printf("\n[январь] Тарифы на коммунальные услуги выросли на %d%%, теперь это %s рублей в год.", growth_percent, format_money(p->utilities_tariff));
    }

    int due = p->utilities_tariff / MONTHS_IN_YEAR + p->utilities_debt;
    int paid = p->money < due ? p->money : due;
    if (paid < 0) {
        paid = 0;
    }
    p->money -= paid;
    p->utilities_debt = due - paid;

    if (p->utilities_debt > 0) {
        printf("\n[месяц %d] %s не хватило денег на коммуналку, долг: %s рублей.", w->month, p->name, format_money(p->utilities_debt));
    }
}
