#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(economy_rise, STAGE_YOUTH)

// bool check(Person* p, World* w)
EVENT_CHECK(economy_rise) {
    return w->economy > ESTATUS_NORMAL;
}

// void result(Person* p, World* w)
EVENT_RESULT(economy_rise) {
    Luck luck = dice();
    int income = 2000 + (int)luck * 1000;
    p->money += income;
    printf("\nЭкономика на подъеме, и %s удалось неплохо заработать на этом — плюс %s рублей.", p->name, format_money(income));
}

EVENT_REGISTRATION(market_crash, STAGE_MIDDLEAGE)

// bool check(Person* p, World* w)
EVENT_CHECK(market_crash) {
    return w->economy <= ESTATUS_CRISIS && p->money > 0;
}

// void result(Person* p, World* w)
EVENT_RESULT(market_crash) {
    int loss = p->money / 3;
    p->money -= loss;
    p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\nОбвал рынка ударил по сбережениям %s — потеряно %s рублей.", p->name, format_money(loss));
}

EVENT_REGISTRATION(inflation, STAGE_OLD)

// bool check(Person* p, World* w)
EVENT_CHECK(inflation) {
    return true;
}

// void result(Person* p, World* w)
EVENT_RESULT(inflation) {
    Luck luck = dice();
    int loss = (luck <= LUCK_BAD) ? 3000 : 800;
    p->money -= loss;
    printf("\nИнфляция понемногу съедает пенсионные накопления %s — минус %s рублей.", p->name, format_money(loss));
}
