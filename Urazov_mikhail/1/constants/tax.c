#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../kredit.h"

CONSTANT_REGISTRATION(tax, CONSTANT_ORDER_TAX)

CONSTANT_APPLY(tax) {
    int tax = p->salary / MONTHS_IN_YEAR * INCOME_TAX_PERCENT / 100;
    p->money -= tax;
    p->nalog_uplachen_za_god += tax;

    if (w->month == MONTHS_IN_YEAR) {
        p->nalog_uplachen_v_proshlom_godu = p->nalog_uplachen_za_god;
        p->nalog_uplachen_za_god = 0;
    }
}
