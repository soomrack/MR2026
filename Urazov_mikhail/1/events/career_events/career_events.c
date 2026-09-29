#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(promotion, STAGE_YOUTH)

// bool check(Person* p, World* w)
EVENT_CHECK(promotion) {
    return w->economy >= ESTATUS_NORMAL;
}

// void result(Person* p, World* w)
EVENT_RESULT(promotion) {
    Luck luck = dice();
    if (luck >= LUCK_GOOD) {
        int bonus = 15000 + (int)luck * 2000;
        p->salary += bonus;
        p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
        printf("\n%s получил(а) повышение на работе! Зарплата в год выросла на %s рублей (с %s до %s).", p->name, format_money(bonus), format_money(p->salary - bonus), format_money(p->salary));
    } else {
        printf("\n%s пытался(ась) добиться повышения, но начальство решило иначе.", p->name);
    }
}

EVENT_REGISTRATION(own_business, STAGE_MIDDLEAGE)

// bool check(Person* p, World* w)
EVENT_CHECK(own_business) {
    return p->money > 50000;
}

// void result(Person* p, World* w)
EVENT_RESULT(own_business) {
    Luck luck = dice();
    int change;
    if (luck <= LUCK_BAD) {
        change = -(p->money / 4);
        printf("\nСобственное дело %s не пошло, потери составили %s рублей.", p->name, format_money(-change));
    } else {
        change = p->money / 3;
        printf("\n%s удачно вложился(ась) в собственное дело и заработал(а) %s рублей.", p->name, format_money(change));
    }
    p->money += change;
}

EVENT_REGISTRATION(pension, STAGE_OLD)

// bool check(Person* p, World* w)
EVENT_CHECK(pension) {
    return p->age >= 55;
}

// void result(Person* p, World* w)
EVENT_RESULT(pension) {
    int pension_payment = 15000;
    p->money += pension_payment;
    printf("\n%s получил(а) пенсионные выплаты в размере %s рублей.", p->name, format_money(pension_payment));
}
