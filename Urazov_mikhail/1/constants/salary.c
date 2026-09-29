#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../kredit.h"
#include "../utils.h"

CONSTANT_REGISTRATION(salary, CONSTANT_ORDER_INCOME)

CONSTANT_APPLY(salary) {
    if (w->month == 1 && p->ipoteka.mesyatsev_oplacheno > 0) {
        int growth_percent = 2 + (int)w->economy * 2;
        p->salary += (int)((long long)p->salary * growth_percent / 100);
        printf("\n[январь] Индексация зарплаты на %d%%: теперь %s рублей в год.", growth_percent, format_money(p->salary));
    }
    p->money += p->salary / MONTHS_IN_YEAR;
}
