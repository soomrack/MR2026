#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../kredit.h"
#include "../utils.h"

CONSTANT_REGISTRATION(nalogovy_vychet, CONSTANT_ORDER_INCOME)

CONSTANT_APPLY(nalogovy_vychet) {
    Ipoteka* m = &p->ipoteka;
    if (w->month != 3 || p->nalog_uplachen_v_proshlom_godu <= 0) {
        return;
    }
    if (m->vychet_ostalos <= 0 && m->protsenty_dlya_vycheta <= 0) {
        return;
    }

    int dostupny_nalog = p->nalog_uplachen_v_proshlom_godu;
    int chast_za_pokupku = m->vychet_ostalos < dostupny_nalog ? m->vychet_ostalos : dostupny_nalog;
    m->vychet_ostalos -= chast_za_pokupku;
    dostupny_nalog -= chast_za_pokupku;

    int nalog_s_protsentov = m->protsenty_dlya_vycheta * INCOME_TAX_PERCENT / 100;
    int chast_za_protsenty = nalog_s_protsentov < dostupny_nalog ? nalog_s_protsentov : dostupny_nalog;
    m->protsenty_dlya_vycheta -= chast_za_protsenty * 100 / INCOME_TAX_PERCENT;

    int vozvrat = chast_za_pokupku + chast_za_protsenty;
    if (vozvrat <= 0) {
        return;
    }
    p->money += vozvrat;
    m->vychety_polucheno += vozvrat;
    printf("\n[март] Налоговый вычет за ипотечную квартиру: %s вернул(а) %s рублей (за покупку %s, за проценты %s). Осталось вычета за покупку: %s.",
        p->name, format_money(vozvrat), format_money(chast_za_pokupku), format_money(chast_za_protsenty), format_money(m->vychet_ostalos));
}
