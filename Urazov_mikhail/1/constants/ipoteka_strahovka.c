#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../kredit.h"
#include "../utils.h"

CONSTANT_REGISTRATION(ipoteka_strahovka, CONSTANT_ORDER_PERED_PLATEZHOM)

CONSTANT_APPLY(ipoteka_strahovka) {
    Ipoteka* m = &p->ipoteka;
    if (!m->active || w->month != 1) {
        return;
    }

    int stoimost = (int)(m->ostatok_dolga * IPOTEKA_STRAHOVKA_PROTSENT / 100.0);
    int oplacheno = p->money < stoimost ? p->money : stoimost;
    if (oplacheno < 0) {
        oplacheno = 0;
    }
    p->money -= oplacheno;
    m->strahovka_vyplacheno += oplacheno;
    printf("\n[январь] Страховка по ипотеке (%.1f%% от остатка долга): %s рублей.", IPOTEKA_STRAHOVKA_PROTSENT, format_money(oplacheno));
    if (oplacheno < stoimost) {
        printf(" Не хватило денег на полный взнос (%s), банк может повысить ставку.", format_money(stoimost));
    }
}
