#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(first_love, STAGE_YOUTH)

// bool check(Person* p, World* w)
EVENT_CHECK(first_love) {
    return get_stage_by_age(p->age) == STAGE_YOUTH;
}

// void result(Person* p, World* w)
EVENT_RESULT(first_love) {
    Luck luck = dice();
    if (luck >= LUCK_NORMAL) {
        p->mood = MOOD_PERFECT;
        printf("\n%s влюбился(ась) и теперь просто летает от счастья!", p->name);
    } else {
        p->mood = MOOD_BAD;
        printf("\nОтношения %s не сложились, пришлось пережить непростое расставание.", p->name);
    }
}

EVENT_REGISTRATION(marriage, STAGE_MIDDLEAGE)

// bool check(Person* p, World* w)
EVENT_CHECK(marriage) {
    return get_stage_by_age(p->age) == STAGE_MIDDLEAGE && p->mood >= MOOD_NORMAL && p->money > 20000;
}

// void result(Person* p, World* w)
EVENT_RESULT(marriage) {
    p->mood = MOOD_PERFECT;
    p->money -= 20000;
    printf("\n%s сыграл(а) свадьбу! Праздник обошелся в 20,000 рублей, но воспоминаний осталось на всю жизнь.", p->name);
}

EVENT_REGISTRATION(grandchildren, STAGE_OLD)

// bool check(Person* p, World* w)
EVENT_CHECK(grandchildren) {
    return p->mood < MOOD_PERFECT;
}

// void result(Person* p, World* w)
EVENT_RESULT(grandchildren) {
    p->mood = clamp_int((int)p->mood + 2, MOOD_AWFUL, MOOD_PERFECT);
    printf("\nВнуки приехали в гости к %s, и это лучшее лекарство от грусти.", p->name);
}
