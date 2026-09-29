#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(local_conflict, STAGE_TEENAGE)

// bool check(Person* p, World* w)
EVENT_CHECK(local_conflict) {
    return w->status == STATUS_WAR || w->status == STATUS_CRISIS;
}

// void result(Person* p, World* w)
EVENT_RESULT(local_conflict) {
    p->mood = MOOD_AWFUL;
    p->health = clamp_int(p->health - 10, 0, 100);
    printf("\nНапряженная обстановка в мире не проходит бесследно для %s: здоровье и настроение ухудшились.", p->name);
}

EVENT_REGISTRATION(natural_disaster, STAGE_YOUTH)

// bool check(Person* p, World* w)
EVENT_CHECK(natural_disaster) {
    return dice() == LUCK_CRITICAL;
}

// void result(Person* p, World* w)
EVENT_RESULT(natural_disaster) {
    p->money = p->money / 2;
    p->mood = MOOD_BAD;
    printf("\nПриродная катастрофа затронула регион, где живет %s. Пришлось потратить сбережения на восстановление.", p->name);
}

EVENT_REGISTRATION(cultural_boom, STAGE_MIDDLEAGE)

// bool check(Person* p, World* w)
EVENT_CHECK(cultural_boom) {
    return w->status == STATUS_RISE || w->status == STATUS_PERFECT_WORLD;
}

// void result(Person* p, World* w)
EVENT_RESULT(cultural_boom) {
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\nКультурный подъем в стране вдохновляет %s на новые свершения.", p->name);
}

EVENT_REGISTRATION(political_stability, STAGE_OLD)

// bool check(Person* p, World* w)
EVENT_CHECK(political_stability) {
    return w->status == STATUS_NORMAL || w->status == STATUS_RISE;
}

// void result(Person* p, World* w)
EVENT_RESULT(political_stability) {
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s спокойно наблюдает за стабильной политической обстановкой в стране.", p->name);
}
