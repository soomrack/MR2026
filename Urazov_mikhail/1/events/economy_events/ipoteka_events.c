#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"
#include "../../kredit.h"

#include "../__event.h"

EVENT_REGISTRATION(credit_holidays, STAGE_YOUTH)

EVENT_CHECK(credit_holidays) {
    return p->ipoteka.active && p->ipoteka.prosrochka > 0 && dice() >= LUCK_NORMAL;
}

EVENT_RESULT(credit_holidays) {
    int spisano = (int)((long long)p->ipoteka.prosrochka * 30 / 100);
    p->ipoteka.prosrochka -= spisano;
    p->ipoteka.propuskov_podryad = 0;
    printf("\nБанк предоставил %s кредитные каникулы: списано %s рублей штрафов, счетчик просрочек обнулен.", p->name, format_money(spisano));
}

EVENT_REGISTRATION(loyalty_program, STAGE_YOUTH)

EVENT_CHECK(loyalty_program) {
    return p->ipoteka.active && p->ipoteka.prosrochka == 0 && p->ipoteka.stavka > 6.0 && dice() >= LUCK_GOOD;
}

EVENT_RESULT(loyalty_program) {
    double staraya_stavka = p->ipoteka.stavka;
    p->ipoteka.stavka -= 0.5;
    p->ipoteka.platezh = kredit_annuitetny_platezh_dlya(&p->ipoteka);
    printf("\nБанк снизил ставку по ипотеке %s за безупречную историю платежей: %.1f%% -> %.1f%%, платеж %s рублей в месяц.",
        p->name, staraya_stavka, p->ipoteka.stavka, format_money(p->ipoteka.platezh));
}

EVENT_REGISTRATION(property_boom, STAGE_YOUTH)

EVENT_CHECK(property_boom) {
    return !p->ipoteka.kvartira_izyata && w->economy >= ESTATUS_NORMAL && dice() >= LUCK_GOOD;
}

EVENT_RESULT(property_boom) {
    int prirost = (int)((long long)p->ipoteka.home_price * 12 / 100);
    p->ipoteka.home_price += prirost;
    printf("\nЦены на жилье в районе %s резко выросли: квартира подорожала на %s рублей (теперь %s).", p->name, format_money(prirost), format_money(p->ipoteka.home_price));
}

EVENT_REGISTRATION(property_bust, STAGE_MIDDLEAGE)

EVENT_CHECK(property_bust) {
    return !p->ipoteka.kvartira_izyata && w->economy <= ESTATUS_CRISIS && dice() <= LUCK_NORMAL;
}

EVENT_RESULT(property_bust) {
    int poterya = (int)((long long)p->ipoteka.home_price * 12 / 100);
    p->ipoteka.home_price -= poterya;
    printf("\nЦены на жилье в районе %s рухнули: квартира подешевела на %s рублей (теперь %s).", p->name, format_money(poterya), format_money(p->ipoteka.home_price));
}

EVENT_REGISTRATION(flat_repair, STAGE_YOUTH)

EVENT_CHECK(flat_repair) {
    return !p->ipoteka.kvartira_izyata && p->money > 60000;
}

EVENT_RESULT(flat_repair) {
    int stoimost = 30000 + (int)dice() * 8000;
    p->money -= stoimost;
    p->ipoteka.home_price += stoimost / 2;
    printf("\n%s сделал(а) ремонт в ипотечной квартире за %s рублей, рыночная цена выросла на %s.", p->name, format_money(stoimost), format_money(stoimost / 2));
}

EVENT_REGISTRATION(roommate_rent, STAGE_MIDDLEAGE)

EVENT_CHECK(roommate_rent) {
    return p->ipoteka.active && p->mood >= MOOD_NORMAL;
}

EVENT_RESULT(roommate_rent) {
    int dohod = 15000 + (int)dice() * 4000;
    p->money += dohod;
    printf("\n%s сдал(а) комнату в ипотечной квартире на несколько месяцев и получил(а) %s рублей.", p->name, format_money(dohod));
}

EVENT_REGISTRATION(year_end_bonus, STAGE_YOUTH)

EVENT_CHECK(year_end_bonus) {
    return p->ipoteka.active && dice() >= LUCK_GOOD;
}

EVENT_RESULT(year_end_bonus) {
    int premiya = 60000 + (int)dice() * 25000;
    p->money += premiya;
    printf("\n%s получил(а) большую премию %s рублей, которую можно направить на досрочное погашение ипотеки.", p->name, format_money(premiya));
}

EVENT_REGISTRATION(neighbors_flood, STAGE_MIDDLEAGE)

EVENT_CHECK(neighbors_flood) {
    return !p->ipoteka.kvartira_izyata && p->money > 30000 && dice() <= LUCK_BAD;
}

EVENT_RESULT(neighbors_flood) {
    int stoimost = 20000 + (int)dice() * 6000;
    p->money -= stoimost;
    printf("\nСоседи затопили квартиру %s, ремонт обошелся в %s рублей (страховка покрыла часть, но не всё).", p->name, format_money(stoimost));
}

EVENT_REGISTRATION(rate_hike_notice, STAGE_MIDDLEAGE)

EVENT_CHECK(rate_hike_notice) {
    return p->ipoteka.active && p->ipoteka.strahovka_vyplacheno > 0 && p->ipoteka.prosrochka > 0;
}

EVENT_RESULT(rate_hike_notice) {
    double staraya_stavka = p->ipoteka.stavka;
    p->ipoteka.stavka += 1.0;
    p->ipoteka.platezh = kredit_annuitetny_platezh_dlya(&p->ipoteka);
    printf("\nБанк повысил ставку по ипотеке %s из-за просрочек: %.1f%% -> %.1f%%, платеж теперь %s рублей в месяц.",
        p->name, staraya_stavka, p->ipoteka.stavka, format_money(p->ipoteka.platezh));
}
