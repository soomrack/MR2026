#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(peace_talks, STAGE_YOUTH)

EVENT_CHECK(peace_talks) {
    return w->status < STATUS_PERFECT_WORLD && dice() >= LUCK_NORMAL;
}

EVENT_RESULT(peace_talks) {
    WorldStatus old = w->status;
    shift_world_status(w, 1);
    printf("\nМеждународные переговоры прошли успешно, напряженность спала. Мир в %s состоянии -> в %s.",
        get_world_status_description(old), get_world_status_description(w->status));
}

EVENT_REGISTRATION(border_tension, STAGE_YOUTH)

EVENT_CHECK(border_tension) {
    return w->status > STATUS_WAR && dice() <= LUCK_BAD;
}

EVENT_RESULT(border_tension) {
    WorldStatus old = w->status;
    shift_world_status(w, -1);
    printf("\nНа границах вспыхнули конфликты, мировая обстановка ухудшилась. Мир в %s состоянии -> в %s.",
        get_world_status_description(old), get_world_status_description(w->status));
}

EVENT_REGISTRATION(international_treaty, STAGE_YOUTH)

EVENT_CHECK(international_treaty) {
    return w->status < STATUS_PERFECT_WORLD && dice() >= LUCK_GOOD;
}

EVENT_RESULT(international_treaty) {
    WorldStatus old = w->status;
    shift_world_status(w, 1);
    printf("\nСтраны подписали масштабный договор о сотрудничестве. Мир в %s состоянии -> в %s.",
        get_world_status_description(old), get_world_status_description(w->status));
}

EVENT_REGISTRATION(pandemic, STAGE_YOUTH)

EVENT_CHECK(pandemic) {
    return w->status > STATUS_WAR && dice() == LUCK_CRITICAL;
}

EVENT_RESULT(pandemic) {
    WorldStatus old = w->status;
    shift_world_status(w, -1);
    shift_economy(w, -1);
    p->health = clamp_int(p->health - 15, 0, 100);
    printf("\nПо миру распространилась пандемия: границы закрыты, экономика просела, %s переболел(а). Мир в %s состоянии -> в %s.",
        p->name, get_world_status_description(old), get_world_status_description(w->status));
}

EVENT_REGISTRATION(scientific_breakthrough, STAGE_MIDDLEAGE)

EVENT_CHECK(scientific_breakthrough) {
    return w->status < STATUS_PERFECT_WORLD && dice() >= LUCK_GOOD;
}

EVENT_RESULT(scientific_breakthrough) {
    WorldStatus old = w->status;
    shift_world_status(w, 1);
    shift_economy(w, 1);
    printf("\nУченые совершили прорыв, изменивший жизнь миллионов людей. Мир в %s состоянии -> в %s.",
        get_world_status_description(old), get_world_status_description(w->status));
}

EVENT_REGISTRATION(political_crisis, STAGE_MIDDLEAGE)

EVENT_CHECK(political_crisis) {
    return w->status > STATUS_WAR && dice() <= LUCK_BAD;
}

EVENT_RESULT(political_crisis) {
    WorldStatus old = w->status;
    shift_world_status(w, -1);
    printf("\nВ стране разразился политический кризис. Мир в %s состоянии -> в %s.",
        get_world_status_description(old), get_world_status_description(w->status));
}

EVENT_REGISTRATION(reconstruction, STAGE_OLD)

EVENT_CHECK(reconstruction) {
    return w->status < STATUS_PERFECT_WORLD && dice() >= LUCK_NORMAL;
}

EVENT_RESULT(reconstruction) {
    WorldStatus old = w->status;
    shift_world_status(w, 1);
    printf("\nРегионы восстанавливаются после трудных лет, жизнь налаживается. Мир в %s состоянии -> в %s.",
        get_world_status_description(old), get_world_status_description(w->status));
}

EVENT_REGISTRATION(war_outbreak, STAGE_OLD)

EVENT_CHECK(war_outbreak) {
    return w->status > STATUS_WAR && dice() == LUCK_CRITICAL;
}

EVENT_RESULT(war_outbreak) {
    WorldStatus old = w->status;
    shift_world_status(w, -2);
    shift_economy(w, -1);
    printf("\nНачалась крупная война, мир на грани. Мир в %s состоянии -> в %s.",
        get_world_status_description(old), get_world_status_description(w->status));
}
