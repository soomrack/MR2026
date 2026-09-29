#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(vitamins, STAGE_YOUTH)

EVENT_CHECK(vitamins) {
    return p->health < 95 && p->money > 800;
}

EVENT_RESULT(vitamins) {
    int gain = 3 + (int)dice();
    p->money -= 800;
    p->health = clamp_int(p->health + gain, 0, 100);
    printf("\n%s пропил(а) курс витаминов за 800 рублей и окреп(ла) на %d. Здоровье: %d", p->name, gain, p->health);
}

EVENT_REGISTRATION(broken_arm, STAGE_YOUTH)

EVENT_CHECK(broken_arm) {
    return p->health > 20 && dice() <= LUCK_BAD;
}

EVENT_RESULT(broken_arm) {
    int loss = 6 + (int)dice() * 2;
    p->health = clamp_int(p->health - loss, 0, 100);
    printf("\n%s сломал(а) руку, здоровье ухудшилось на %d. Здоровье: %d", p->name, loss, p->health);
}

EVENT_REGISTRATION(marathon, STAGE_YOUTH)

EVENT_CHECK(marathon) {
    return p->health > 50 && p->mood >= MOOD_NORMAL;
}

EVENT_RESULT(marathon) {
    int gain = 5 + (int)dice() * 2;
    p->health = clamp_int(p->health + gain, 0, 100);
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s пробежал(а) марафон и стал(а) выносливее на %d. Здоровье: %d", p->name, gain, p->health);
}

EVENT_REGISTRATION(insomnia, STAGE_YOUTH)

EVENT_CHECK(insomnia) {
    return p->health > 15 && p->mood <= MOOD_NORMAL;
}

EVENT_RESULT(insomnia) {
    int loss = 3 + (int)dice() * 2;
    p->health = clamp_int(p->health - loss, 0, 100);
    printf("\nНеделями не мог(ла) уснуть, %s измучился(ась): здоровье -%d. Здоровье: %d", p->name, loss, p->health);
}

EVENT_REGISTRATION(dentist_visit, STAGE_MIDDLEAGE)

EVENT_CHECK(dentist_visit) {
    return p->money > 6000;
}

EVENT_RESULT(dentist_visit) {
    int cost = 3000 + (int)dice() * 800;
    p->money -= cost;
    p->health = clamp_int(p->health + 4, 0, 100);
    printf("\n%s вылечил(а) зубы за %s рублей и перестал(а) испытывать боль. Здоровье: %d", p->name, format_money(cost), p->health);
}

EVENT_REGISTRATION(back_pain, STAGE_MIDDLEAGE)

EVENT_CHECK(back_pain) {
    return p->health > 20;
}

EVENT_RESULT(back_pain) {
    int loss = 5 + (int)dice() * 2;
    p->health = clamp_int(p->health - loss, 0, 100);
    printf("\nУ %s разболелась спина от сидячей работы: здоровье -%d. Здоровье: %d", p->name, loss, p->health);
}

EVENT_REGISTRATION(blood_donation, STAGE_MIDDLEAGE)

EVENT_CHECK(blood_donation) {
    return p->health > 60 && p->mood >= MOOD_NORMAL;
}

EVENT_RESULT(blood_donation) {
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    p->money += 800;
    printf("\n%s стал(а) донором крови, получил(а) компенсацию 800 рублей и заряд хорошего настроения.", p->name);
}

EVENT_REGISTRATION(physiotherapy, STAGE_OLD)

EVENT_CHECK(physiotherapy) {
    return p->health < 85 && p->money > 3500;
}

EVENT_RESULT(physiotherapy) {
    int gain = 6 + (int)dice() * 2;
    p->money -= 3500;
    p->health = clamp_int(p->health + gain, 0, 100);
    printf("\n%s прошел(ла) курс физиотерапии за 3,500 рублей: здоровье +%d. Здоровье: %d", p->name, gain, p->health);
}

EVENT_REGISTRATION(flu_epidemic, STAGE_OLD)

EVENT_CHECK(flu_epidemic) {
    return p->health > 25 && dice() <= LUCK_NORMAL;
}

EVENT_RESULT(flu_epidemic) {
    int loss = 8 + (int)dice() * 3;
    p->health = clamp_int(p->health - loss, 0, 100);
    printf("\nВ городе началась эпидемия гриппа, %s тяжело переболел(а): здоровье -%d. Здоровье: %d", p->name, loss, p->health);
}

EVENT_REGISTRATION(heart_surgery, STAGE_OLD)

EVENT_CHECK(heart_surgery) {
    return p->health < 45 && p->money > 12000;
}

EVENT_RESULT(heart_surgery) {
    int gain = 15 + (int)dice() * 3;
    p->money -= 12000;
    p->health = clamp_int(p->health + gain, 0, 100);
    printf("\n%s перенес(ла) операцию на сердце за 12,000 рублей и почувствовал(а) себя гораздо лучше: здоровье +%d. Здоровье: %d", p->name, gain, p->health);
}
