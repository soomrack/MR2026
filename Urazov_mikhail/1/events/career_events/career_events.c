#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(first_job, STAGE_TEENAGE)

// bool check(Person* p, World* w)
EVENT_CHECK(first_job) {
    return p->money < 20000;
}

// void result(Person* p, World* w)
EVENT_RESULT(first_job) {
    Luck luck = dice();
    int income = 1000 + (int)luck * 500;
    p->money += income;
    printf("\n%s устроился(ась) на свою первую подработку и заработал(а) %d рублей.", p->name, income);
}

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
        printf("\n%s получил(а) повышение на работе! Зарплата в год выросла на %d рублей (с %d до %d).", p->name, bonus, p->salary - bonus, p->salary);
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
        printf("\nСобственное дело %s не пошло, потери составили %d рублей.", p->name, -change);
    } else {
        change = p->money / 3;
        printf("\n%s удачно вложился(ась) в собственное дело и заработал(а) %d рублей.", p->name, change);
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
    printf("\n%s получил(а) пенсионные выплаты в размере %d рублей.", p->name, pension_payment);
}
