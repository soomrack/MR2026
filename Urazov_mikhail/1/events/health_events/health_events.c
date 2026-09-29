#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(illness, STAGE_YOUTH)

// bool check(Person* p, World* w)
EVENT_CHECK(illness) {
    return p->health < 95;
}

// void result(Person* p, World* w)
EVENT_RESULT(illness) {
    Luck luck = dice();
    int delta = ((int)luck - 2) * 8;
    p->health = clamp_int(p->health + delta, 0, 100);
    if (delta < 0) {
        printf("\n%s подхватил(а) простуду и пару дней провел(а) в постели. Здоровье: %d", p->name, p->health);
    } else {
        printf("\n%s быстро оправился(ась) после недомогания. Здоровье: %d", p->name, p->health);
    }
}

EVENT_REGISTRATION(sport_injury, STAGE_YOUTH)

// bool check(Person* p, World* w)
EVENT_CHECK(sport_injury) {
    return p->health > 15;
}

// void result(Person* p, World* w)
EVENT_RESULT(sport_injury) {
    Luck luck = dice();
    int delta = ((int)luck - 3) * 10;
    p->health = clamp_int(p->health + delta, 0, 100);
    if (delta < 0) {
        printf("\n%s получил(а) травму во время активного отдыха. Здоровье: %d", p->name, p->health);
    } else {
        printf("\n%s всерьез увлекся(лась) спортом и заметно окреп(ла). Здоровье: %d", p->name, p->health);
    }
}

EVENT_REGISTRATION(chronic_illness, STAGE_MIDDLEAGE)

// bool check(Person* p, World* w)
EVENT_CHECK(chronic_illness) {
    return p->health < 90;
}

// void result(Person* p, World* w)
EVENT_RESULT(chronic_illness) {
    Luck luck = dice();
    int delta = (luck <= LUCK_BAD) ? -15 : 5;
    p->health = clamp_int(p->health + delta, 0, 100);
    p->money -= (delta < 0) ? 4000 : 0;
    if (delta < 0) {
        printf("\nУ %s обнаружили хроническое заболевание, лечение обошлось в 4,000 рублей. Здоровье: %d", p->name, p->health);
    } else {
        printf("\n%s пересмотрел(а) образ жизни и стал(а) чувствовать себя лучше. Здоровье: %d", p->name, p->health);
    }
}

EVENT_REGISTRATION(hospitalization, STAGE_OLD)

// bool check(Person* p, World* w)
EVENT_CHECK(hospitalization) {
    return true;
}

// void result(Person* p, World* w)
EVENT_RESULT(hospitalization) {
    Luck luck = dice();
    int delta = ((int)luck - 2) * 12;
    p->health = clamp_int(p->health + delta, 0, 100);
    if (delta < 0) {
        p->money -= 3000;
        printf("\n%s попал(а) в больницу на обследование, лечение стоило 3,000 рублей. Здоровье: %d", p->name, p->health);
    } else {
        printf("\nПлановое обследование прошло успешно, врачи довольны состоянием %s. Здоровье: %d", p->name, p->health);
    }
}
