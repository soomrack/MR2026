#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(career_courses, STAGE_YOUTH)

EVENT_CHECK(career_courses) {
    return p->money > 4000;
}

EVENT_RESULT(career_courses) {
    int raise = 2000 + (int)dice() * 1000;
    p->money -= 4000;
    p->salary += raise;
    printf("\n%s окончил(а) платные курсы за 4,000 рублей, зарплата в год выросла на %s.", p->name, format_money(raise));
}

EVENT_REGISTRATION(freelance_order, STAGE_YOUTH)

EVENT_CHECK(freelance_order) {
    return p->mood >= MOOD_NORMAL;
}

EVENT_RESULT(freelance_order) {
    int income = 5000 + (int)dice() * 2500;
    p->money += income;
    printf("\n%s взял(а) выгодный заказ на фрилансе и получил(а) %s рублей.", p->name, format_money(income));
}

EVENT_REGISTRATION(overtime_work, STAGE_YOUTH)

EVENT_CHECK(overtime_work) {
    return p->health > 25;
}

EVENT_RESULT(overtime_work) {
    int income = 3000 + (int)dice() * 1000;
    p->money += income;
    p->health = clamp_int(p->health - 4, 0, 100);
    p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s часто задерживался(ась) на работе, доплата составила %s рублей, но силы на исходе.", p->name, format_money(income));
}

EVENT_REGISTRATION(job_change, STAGE_YOUTH)

EVENT_CHECK(job_change) {
    return w->economy >= ESTATUS_NORMAL && dice() >= LUCK_NORMAL;
}

EVENT_RESULT(job_change) {
    int raise = 4000 + (int)dice() * 1500;
    p->salary += raise;
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s перешел(ла) на новую работу с более высокой оплатой: зарплата в год +%s (теперь %s).", p->name, format_money(raise), format_money(p->salary));
}

EVENT_REGISTRATION(layoff, STAGE_MIDDLEAGE)

EVENT_CHECK(layoff) {
    return w->economy <= ESTATUS_CRISIS && p->salary > 10000;
}

EVENT_RESULT(layoff) {
    int cut = p->salary / 5;
    p->salary -= cut;
    p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\nВо время кризиса компания сократила зарплаты: %s теперь получает на %s рублей в год меньше.", p->name, format_money(cut));
}

EVENT_REGISTRATION(annual_indexation, STAGE_MIDDLEAGE)

EVENT_CHECK(annual_indexation) {
    return w->economy >= ESTATUS_NORMAL;
}

EVENT_RESULT(annual_indexation) {
    int raise = (int)((long long)p->salary * 6 / 100);
    p->salary += raise;
    printf("\nКомпания проиндексировала зарплаты, %s стал(а) получать на %s рублей в год больше.", p->name, format_money(raise));
}

EVENT_REGISTRATION(startup_grant, STAGE_MIDDLEAGE)

EVENT_CHECK(startup_grant) {
    return p->money > 30000 && dice() >= LUCK_GOOD;
}

EVENT_RESULT(startup_grant) {
    int grant = 10000 + (int)dice() * 5000;
    p->money += grant;
    printf("\n%s выиграл(а) государственный грант на развитие проекта: %s рублей.", p->name, format_money(grant));
}

EVENT_REGISTRATION(part_time_consulting, STAGE_OLD)

EVENT_CHECK(part_time_consulting) {
    return p->health > 40;
}

EVENT_RESULT(part_time_consulting) {
    int income = 2000 + (int)dice() * 800;
    p->money += income;
    printf("\n%s подрабатывал(а) консультантом на пенсии и получил(а) %s рублей.", p->name, format_money(income));
}

EVENT_REGISTRATION(pension_indexation, STAGE_OLD)

EVENT_CHECK(pension_indexation) {
    return p->age >= 55 && w->economy >= ESTATUS_NORMAL;
}

EVENT_RESULT(pension_indexation) {
    int extra = 1500 + (int)dice() * 300;
    p->money += extra;
    printf("\nГосударство проиндексировало пенсии, %s получил(а) доплату %s рублей.", p->name, format_money(extra));
}
