#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../kredit.h"
#include "../utils.h"

CONSTANT_REGISTRATION(home_price, CONSTANT_ORDER_YEAR_END)

CONSTANT_APPLY(home_price) {
    Ipoteka* m = &p->ipoteka;
    if (m->kvartira_izyata || w->month != MONTHS_IN_YEAR) {
        return;
    }

    static const int rost_protsent[] = {-5, 0, 3, 5, 7};
    int rost = rost_protsent[w->economy];
    m->home_price += (int)((long long)m->home_price * rost / 100);
    printf("\n[декабрь] Рынок недвижимости: квартира %s %s на %d%%, рыночная цена %s рублей.",
        p->name, rost >= 0 ? "подорожала" : "подешевела", rost >= 0 ? rost : -rost, format_money(m->home_price));
}
