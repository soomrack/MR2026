#include "kredit.h"
#include "constants.h"
#include "utils.h"

double kredit_rynochnaya_stavka(World* w) {
    static const double adjustment[] = {5.0, 2.5, 0.0, -1.0, -2.0};
    return IPOTEKA_BAZOVAYA_STAVKA + adjustment[w->economy];
}

int kredit_annuitetny_platezh(int sum, double stavka, int mesyatsy) {
    double mesyachnaya_stavka = stavka / 1200.0;
    if (mesyachnaya_stavka <= 0.0) {
        return sum / mesyatsy;
    }
    double mul = 1.0;
    for (int i = 0; i < mesyatsy; i++) {
        mul *= 1.0 + mesyachnaya_stavka;
    }
    return (int)(sum * mesyachnaya_stavka * mul / (mul - 1.0) + 0.5);
}

int kredit_annuitetny_platezh_dlya(Ipoteka* m) {
    int mesyatsy = m->mesyatsev_ostalos > 0 ? m->mesyatsev_ostalos : 1;
    return kredit_annuitetny_platezh(m->ostatok_dolga, m->stavka, mesyatsy);
}

int kredit_mesyatsev_do_pogasheniya(int ostatok_dolga, double stavka, int platezh) {
    double mesyachnaya_stavka = stavka / 1200.0;
    double ostatok = ostatok_dolga;
    int mesyatsy = 0;
    while (ostatok > 0.5 && mesyatsy < 12 * 100) {
        ostatok = ostatok * (1.0 + mesyachnaya_stavka) - platezh;
        mesyatsy++;
    }
    return mesyatsy;
}

int kredit_dohod_v_mesyats(Person* p) {
    return (int)((long long)p->salary * (100 - INCOME_TAX_PERCENT) / 100 / MONTHS_IN_YEAR);
}

int kredit_dolg(Person* p) {
    return p->ipoteka.ostatok_dolga + p->ipoteka.prosrochka;
}

void kredit_vydat(Person* p, World* w) {
    Ipoteka* m = &p->ipoteka;
    *m = (Ipoteka){0};

    m->srok_mesyatsev = w->ipoteka_let * MONTHS_IN_YEAR;
    m->mesyatsev_ostalos = m->srok_mesyatsev;
    m->stavka = kredit_rynochnaya_stavka(w);
    m->nachalnaya_stavka = m->stavka;

    int sum = IPOTEKA_BAZOVAYA_SUMMA + (int)dice() * IPOTEKA_SHAG_SUMMY;
    int price = sum * 100 / (100 - IPOTEKA_PERVONACHALNY_VZNOS_PROTSENT);
    int platezh = kredit_annuitetny_platezh(sum, m->stavka, m->srok_mesyatsev);

    m->active = 1;
    m->price = price;
    m->home_price = price;
    m->pervonachalny_vznos = price - sum;
    m->summa_kredita = sum;
    m->ostatok_dolga = sum;
    m->platezh = platezh;
    m->vychet_ostalos = (price < VYCHET_LIMIT_POKUPKI ? price : VYCHET_LIMIT_POKUPKI) * INCOME_TAX_PERCENT / 100;
    m->limit_protsentov_ostalos = VYCHET_LIMIT_PROTSENTOV;
}
