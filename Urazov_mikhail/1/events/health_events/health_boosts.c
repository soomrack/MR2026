#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(gym_membership, STAGE_YOUTH)

EVENT_CHECK(gym_membership) {
    return p->health < 100 && p->money > 3000;
}

EVENT_RESULT(gym_membership) {
    int gain = 6 + (int)dice() * 2;
    p->money -= 3000;
    p->health = clamp_int(p->health + gain, 0, 100);
    printf("\n%s купил(а) абонемент в спортзал за 3,000 рублей и окреп(ла) на %d. Здоровье: %d", p->name, gain, p->health);
}

EVENT_REGISTRATION(overwork, STAGE_YOUTH)

EVENT_CHECK(overwork) {
    return p->health > 15;
}

EVENT_RESULT(overwork) {
    int loss = 4 + (int)dice() * 3;
    p->health = clamp_int(p->health - loss, 0, 100);
    printf("\n%s слишком много работал(а) без отдыха, здоровье ухудшилось на %d. Здоровье: %d", p->name, loss, p->health);
}

EVENT_REGISTRATION(medical_checkup, STAGE_MIDDLEAGE)

EVENT_CHECK(medical_checkup) {
    return p->health < 95 && p->money > 2000;
}

EVENT_RESULT(medical_checkup) {
    int gain = 8 + (int)dice() * 2;
    p->money -= 2000;
    p->health = clamp_int(p->health + gain, 0, 100);
    printf("\n%s прошел(ла) диспансеризацию за 2,000 рублей, проблемы выявили вовремя (+%d). Здоровье: %d", p->name, gain, p->health);
}

EVENT_REGISTRATION(healthy_diet, STAGE_MIDDLEAGE)

EVENT_CHECK(healthy_diet) {
    return p->health < 100;
}

EVENT_RESULT(healthy_diet) {
    int gain = 5 + (int)dice() * 2;
    p->health = clamp_int(p->health + gain, 0, 100);
    printf("\n%s перешел(ла) на здоровое питание и почувствовал(а) себя лучше на %d. Здоровье: %d", p->name, gain, p->health);
}

EVENT_REGISTRATION(burnout, STAGE_MIDDLEAGE)

EVENT_CHECK(burnout) {
    return p->health > 20 && p->mood <= MOOD_BAD;
}

EVENT_RESULT(burnout) {
    int loss = 8 + (int)dice() * 3;
    p->health = clamp_int(p->health - loss, 0, 100);
    printf("\nУ %s случилось эмоциональное выгорание, здоровье упало на %d. Здоровье: %d", p->name, loss, p->health);
}

EVENT_REGISTRATION(sanatorium, STAGE_OLD)

EVENT_CHECK(sanatorium) {
    return p->health < 90 && p->money > 5000;
}

EVENT_RESULT(sanatorium) {
    int gain = 10 + (int)dice() * 3;
    p->money -= 5000;
    p->health = clamp_int(p->health + gain, 0, 100);
    printf("\n%s съездил(а) в санаторий за 5,000 рублей и заметно поправил(а) здоровье на %d. Здоровье: %d", p->name, gain, p->health);
}

EVENT_REGISTRATION(daily_walks, STAGE_OLD)

EVENT_CHECK(daily_walks) {
    return p->health < 100;
}

EVENT_RESULT(daily_walks) {
    int gain = 4 + (int)dice() * 2;
    p->health = clamp_int(p->health + gain, 0, 100);
    printf("\n%s завел(а) привычку гулять каждый день, самочувствие улучшилось на %d. Здоровье: %d", p->name, gain, p->health);
}

EVENT_REGISTRATION(bad_fall, STAGE_OLD)

EVENT_CHECK(bad_fall) {
    return p->health > 25;
}

EVENT_RESULT(bad_fall) {
    int loss = 10 + (int)dice() * 4;
    p->health = clamp_int(p->health - loss, 0, 100);
    printf("\n%s неудачно упал(а) и получил(а) тяжелую травму, здоровье ухудшилось на %d. Здоровье: %d", p->name, loss, p->health);
}
