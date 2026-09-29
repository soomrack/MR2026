#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../kredit.h"
#include "../utils.h"

CONSTANT_REGISTRATION(ipoteka_dosrochnoe_pogashenie, CONSTANT_ORDER_POSLE_PLATEZHA)

CONSTANT_APPLY(ipoteka_dosrochnoe_pogashenie) {
    Ipoteka* m = &p->ipoteka;
    if (!w->dosrochnoe_vklyucheno || !m->active || m->prosrochka > 0 || w->month != MONTHS_IN_YEAR) {
        return;
    }

    int rezerv = m->platezh * IPOTEKA_DOSROCHNO_REZERV_PLATEZHEY;
    int izlishek = p->money - rezerv;
    if (izlishek < m->platezh) {
        return;
    }

    int sum = izlishek * IPOTEKA_DOSROCHNO_DOLYA_PROTSENT / 100;
    if (sum > m->ostatok_dolga) {
        sum = m->ostatok_dolga;
    }
    p->money -= sum;
    m->ostatok_dolga -= sum;
    m->dosrochno_vneseno += sum;
    m->vsego_vyplacheno += sum;

    if (m->ostatok_dolga <= 0) {
        m->ostatok_dolga = 0;
        m->active = 0;
        m->pogashena = 1;
        printf("\n[декабрь] %s досрочно погасил(а) остаток ипотеки, внеся %s рублей. Ипотека полностью погашена!", p->name, format_money(sum));
        return;
    }
    int mesyatsev_bylo = m->mesyatsev_ostalos;
    m->mesyatsev_ostalos = kredit_mesyatsev_do_pogasheniya(m->ostatok_dolga, m->stavka, m->platezh);
    printf("\n[декабрь] Досрочное погашение ипотеки: внесено %s рублей, срок сократился с %d до %d мес., остаток долга %s.",
        format_money(sum), mesyatsev_bylo, m->mesyatsev_ostalos, format_money(m->ostatok_dolga));
}
