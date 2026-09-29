#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(rate_cut, STAGE_YOUTH)

EVENT_CHECK(rate_cut) {
    return w->economy < ESTATUS_PERFECT && dice() >= LUCK_NORMAL;
}

EVENT_RESULT(rate_cut) {
    EconomyStatus old = w->economy;
    shift_economy(w, 1);
    printf("\nЦентробанк снизил ставку, кредиты стали доступнее. Экономика: %s -> %s.",
        get_economy_status_description(old), get_economy_status_description(w->economy));
}

EVENT_REGISTRATION(price_spike, STAGE_YOUTH)

EVENT_CHECK(price_spike) {
    return w->economy > ESTATUS_DEFOLT && dice() <= LUCK_BAD;
}

EVENT_RESULT(price_spike) {
    EconomyStatus old = w->economy;
    shift_economy(w, -1);
    printf("\nЦены в стране резко выросли, экономика замедлилась. Экономика: %s -> %s.",
        get_economy_status_description(old), get_economy_status_description(w->economy));
}

EVENT_REGISTRATION(tech_boom, STAGE_YOUTH)

EVENT_CHECK(tech_boom) {
    return w->economy < ESTATUS_PERFECT && dice() >= LUCK_GOOD;
}

EVENT_RESULT(tech_boom) {
    EconomyStatus old = w->economy;
    shift_economy(w, 2);
    printf("\nВ стране начался технологический бум, появились тысячи новых рабочих мест. Экономика: %s -> %s.",
        get_economy_status_description(old), get_economy_status_description(w->economy));
}

EVENT_REGISTRATION(banking_crisis, STAGE_YOUTH)

EVENT_CHECK(banking_crisis) {
    return w->economy > ESTATUS_DEFOLT && dice() <= LUCK_BAD;
}

EVENT_RESULT(banking_crisis) {
    EconomyStatus old = w->economy;
    shift_economy(w, -2);
    printf("\nНачался банковский кризис, вклады заморожены. Экономика: %s -> %s.",
        get_economy_status_description(old), get_economy_status_description(w->economy));
}

EVENT_REGISTRATION(trade_deal, STAGE_MIDDLEAGE)

EVENT_CHECK(trade_deal) {
    return w->economy < ESTATUS_PERFECT && dice() >= LUCK_NORMAL;
}

EVENT_RESULT(trade_deal) {
    EconomyStatus old = w->economy;
    shift_economy(w, 1);
    printf("\nСтрана заключила выгодное торговое соглашение, экспорт вырос. Экономика: %s -> %s.",
        get_economy_status_description(old), get_economy_status_description(w->economy));
}

EVENT_REGISTRATION(sanctions, STAGE_MIDDLEAGE)

EVENT_CHECK(sanctions) {
    return w->economy > ESTATUS_DEFOLT && dice() <= LUCK_BAD;
}

EVENT_RESULT(sanctions) {
    EconomyStatus old = w->economy;
    shift_economy(w, -1);
    printf("\nПротив страны ввели санкции, импорт подорожал. Экономика: %s -> %s.",
        get_economy_status_description(old), get_economy_status_description(w->economy));
}

EVENT_REGISTRATION(reforms_success, STAGE_OLD)

EVENT_CHECK(reforms_success) {
    return w->economy < ESTATUS_PERFECT && dice() >= LUCK_NORMAL;
}

EVENT_RESULT(reforms_success) {
    EconomyStatus old = w->economy;
    shift_economy(w, 1);
    printf("\nДавние экономические реформы наконец принесли плоды. Экономика: %s -> %s.",
        get_economy_status_description(old), get_economy_status_description(w->economy));
}

EVENT_REGISTRATION(budget_deficit, STAGE_OLD)

EVENT_CHECK(budget_deficit) {
    return w->economy > ESTATUS_DEFOLT && dice() <= LUCK_BAD;
}

EVENT_RESULT(budget_deficit) {
    EconomyStatus old = w->economy;
    shift_economy(w, -1);
    printf("\nДефицит бюджета вынудил государство урезать расходы. Экономика: %s -> %s.",
        get_economy_status_description(old), get_economy_status_description(w->economy));
}
