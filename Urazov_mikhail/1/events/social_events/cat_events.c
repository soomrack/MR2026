#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(kitten_vaccination, STAGE_TEENAGE)

EVENT_CHECK(kitten_vaccination) {
    return p->has_cat && p->money > 500;
}

EVENT_RESULT(kitten_vaccination) {
    p->money -= 500;
    printf("\n%s сделал(а) кошке прививки, это обошлось в 500 рублей.", p->name);
}

EVENT_REGISTRATION(adopt_kitten, STAGE_TEENAGE)

EVENT_CHECK(adopt_kitten) {
    return !p->has_cat && p->money > 3000;
}

EVENT_RESULT(adopt_kitten) {
    p->has_cat = 1;
    p->cat_age = 0;
    p->cat_lifespan = 12 + (int)dice() * 2;
    p->money -= 3000;
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s взял(а) котенка из приюта (3000 рублей на обустройство), настроение улучшилось.", p->name);
}

EVENT_REGISTRATION(adopt_cat, STAGE_YOUTH)

EVENT_CHECK(adopt_cat) {
    return !p->has_cat && p->money > 3000;
}

EVENT_RESULT(adopt_cat) {
    p->has_cat = 1;
    p->cat_age = 0;
    p->cat_lifespan = 12 + (int)dice() * 2;
    p->money -= 3000;
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s завел(а) кошку (3000 рублей на лоток, миски и переноску), в доме стало уютнее.", p->name);
}

EVENT_REGISTRATION(cat_runaway, STAGE_YOUTH)

EVENT_CHECK(cat_runaway) {
    return p->has_cat && dice() == LUCK_CRITICAL;
}

EVENT_RESULT(cat_runaway) {
    p->has_cat = 0;
    p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\nКошка %s сбежала и не вернулась. Расходы на нее прекратились, но на душе грустно.", p->name);
}

EVENT_REGISTRATION(cat_illness, STAGE_MIDDLEAGE)

EVENT_CHECK(cat_illness) {
    return p->has_cat && p->money > 1000;
}

EVENT_RESULT(cat_illness) {
    int cost = 2000 + (int)dice() * 1500;
    if (cost > p->money) {
        cost = p->money;
    }
    p->money -= cost;
    printf("\nКошка %s заболела, лечение у ветеринара стоило %d рублей.", p->name, cost);
}

EVENT_REGISTRATION(cat_show_win, STAGE_MIDDLEAGE)

EVENT_CHECK(cat_show_win) {
    return p->has_cat && dice() >= LUCK_GOOD;
}

EVENT_RESULT(cat_show_win) {
    int prize = 2000 + (int)dice() * 1000;
    p->money += prize;
    printf("\nКошка %s заняла призовое место на выставке, приз %d рублей.", p->name, prize);
}

EVENT_REGISTRATION(adopt_companion, STAGE_OLD)

EVENT_CHECK(adopt_companion) {
    return !p->has_cat && p->money > 3000;
}

EVENT_RESULT(adopt_companion) {
    p->has_cat = 1;
    p->cat_age = 0;
    p->cat_lifespan = 12 + (int)dice() * 2;
    p->money -= 3000;
    p->mood = clamp_int((int)p->mood + 2, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s взял(а) кошку-компаньона, чтобы не было одиноко (3000 рублей), настроение заметно улучшилось.", p->name);
}

EVENT_REGISTRATION(cat_purring, STAGE_OLD)

EVENT_CHECK(cat_purring) {
    return p->has_cat && p->health < 100;
}

EVENT_RESULT(cat_purring) {
    p->health = clamp_int(p->health + 3, 0, 100);
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\nМурлыканье кошки успокаивает %s: самочувствие и настроение лучше.", p->name);
}
