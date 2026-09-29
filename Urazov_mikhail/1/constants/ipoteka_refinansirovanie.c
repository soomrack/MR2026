#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../kredit.h"
#include "../utils.h"

CONSTANT_REGISTRATION(ipoteka_refinansirovanie, CONSTANT_ORDER_PERED_PLATEZHOM)

CONSTANT_APPLY(ipoteka_refinansirovanie) {
    Ipoteka* m = &p->ipoteka;
    if (!m->active || w->month != 6 || m->prosrochka > 0) {
        return;
    }
    if (m->mesyatsev_ostalos < IPOTEKA_REFINANS_MIN_MESYATSEV) {
        return;
    }

    double rynochnaya_stavka = kredit_rynochnaya_stavka(w);
    if (rynochnaya_stavka > m->stavka - IPOTEKA_REFINANS_MIN_VYIGRYSH) {
        return;
    }

    int komissiya = m->ostatok_dolga * IPOTEKA_REFINANS_KOMISSIYA_PROTSENT / 100;
    if (p->money < komissiya) {
        printf("\n[июнь] Ставки упали до %.1f%%, но у %s нет %s рублей на оформление рефинансирования.", rynochnaya_stavka, p->name, format_money(komissiya));
        return;
    }

    int staryy_platezh = m->platezh;
    p->money -= komissiya;
    m->vsego_vyplacheno += komissiya;
    m->stavka = rynochnaya_stavka;
    m->platezh = kredit_annuitetny_platezh(m->ostatok_dolga, rynochnaya_stavka, m->mesyatsev_ostalos);
    m->chislo_refinansirovaniy++;
    printf("\n[июнь] Рефинансирование ипотеки: ставка снижена до %.1f%%, комиссия %s рублей, платеж в месяц %s -> %s.",
        rynochnaya_stavka, format_money(komissiya), format_money(staryy_platezh), format_money(m->platezh));
}
