#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../kredit.h"
#include "../utils.h"

CONSTANT_REGISTRATION(ipoteka, CONSTANT_ORDER_PLATEZH)

static void kopit_protsenty_dlya_vycheta(Ipoteka* m, int protsenty) {
    int counted = protsenty < m->limit_protsentov_ostalos ? protsenty : m->limit_protsentov_ostalos;
    m->protsenty_dlya_vycheta += counted;
    m->limit_protsentov_ostalos -= counted;
}

static void izyat_kvartiru(Person* p) {
    Ipoteka* m = &p->ipoteka;
    int summa_prodazhi = (int)((long long)m->home_price * (100 - IPOTEKA_IZYATIE_SKIDKA_PROTSENT) / 100);
    int dolg = kredit_dolg(p);

    m->tsena_prodazhi = summa_prodazhi;
    m->kvartira_izyata = 1;
    m->active = 0;
    printf("\nБанк потерял терпение: %d месяцев подряд нет платежей. Квартира %s продана с торгов за %s рублей (со скидкой %d%%), долг банку %s рублей.",
        m->propuskov_podryad, p->name, format_money(summa_prodazhi), IPOTEKA_IZYATIE_SKIDKA_PROTSENT, format_money(dolg));
    if (summa_prodazhi > dolg) {
        p->money += summa_prodazhi - dolg;
        printf("\nОстаток от продажи, %s рублей, вернули владельцу.", format_money(summa_prodazhi - dolg));
    } else {
        printf("\nВырученных денег не хватило, оставшийся долг %s рублей банк списал.", format_money(dolg - summa_prodazhi));
    }
    m->ostatok_dolga = 0;
    m->prosrochka = 0;
}

CONSTANT_APPLY(ipoteka) {
    Ipoteka* m = &p->ipoteka;
    if (!m->active) {
        return;
    }

    int protsenty = (int)(m->ostatok_dolga * m->stavka / 1200.0);
    int po_grafiku = m->platezh;
    if (m->mesyatsev_ostalos <= 1 || po_grafiku > m->ostatok_dolga + protsenty) {
        po_grafiku = m->ostatok_dolga + protsenty;
    }

    m->ostatok_dolga -= po_grafiku - protsenty;
    m->mesyatsev_ostalos--;
    m->mesyatsev_oplacheno++;
    m->protsenty_vyplacheno += protsenty;
    m->protsenty_za_god += protsenty;
    kopit_protsenty_dlya_vycheta(m, protsenty);

    int k_oplate = po_grafiku + m->prosrochka;
    if (p->money >= k_oplate) {
        p->money -= k_oplate;
        m->vsego_vyplacheno += k_oplate;
        if (m->prosrochka > 0) {
            printf("\n[месяц %d] Ипотека: %s погасил(а) просрочку %s рублей вместе с очередным платежом.", w->month, p->name, format_money(m->prosrochka));
        }
        m->prosrochka = 0;
        m->propuskov_podryad = 0;
    } else {
        int oplacheno = p->money > 0 ? p->money : 0;
        int ne_oplacheno = k_oplate - oplacheno;
        int shtraf = ne_oplacheno * IPOTEKA_SHTRAF_PROTSENT / 100;

        p->money -= oplacheno;
        m->vsego_vyplacheno += oplacheno;
        m->prosrochka = ne_oplacheno + shtraf;
        m->shtrafy += shtraf;
        m->propuskov_podryad++;
        printf("\n[месяц %d] Ипотека: у %s не хватило денег (нужно %s, выплачено %s). Просрочка %s, штраф %d%% = %s рублей. Месяцев просрочки подряд: %d.",
            w->month, p->name, format_money(k_oplate), format_money(oplacheno), format_money(ne_oplacheno), IPOTEKA_SHTRAF_PROTSENT, format_money(shtraf), m->propuskov_podryad);
        if (m->propuskov_podryad >= IPOTEKA_IZYATIE_MESYATSEV) {
            izyat_kvartiru(p);
            return;
        }
    }

    if (m->ostatok_dolga <= 0 && m->prosrochka <= 0) {
        m->ostatok_dolga = 0;
        m->active = 0;
        m->pogashena = 1;
        printf("\nИпотека полностью погашена!");
    }
}
