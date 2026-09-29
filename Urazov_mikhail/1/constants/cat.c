#include <stdio.h>

#include "../types.h"
#include "../constants.h"
#include "../utils.h"

#define CAT_YEARLY_COST 6000

CONSTANT_REGISTRATION(cat, CONSTANT_ORDER_LIVING)

CONSTANT_APPLY(cat) {
    if (!p->has_cat) {
        return;
    }
    p->cat_age++;
    if (p->cat_age > p->cat_lifespan) {
        p->has_cat = 0;
        p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
        printf("\nКошка %s прожила долгую жизнь (%d лет) и ушла на радугу. Настроение ухудшилось.", p->name, p->cat_age - 1);
        return;
    }

    int cost = CAT_YEARLY_COST + ((int)ESTATUS_PERFECT - (int)w->economy) * 500;
    if (p->money >= cost) {
        p->money -= cost;
        printf("\nСодержание кошки (корм, наполнитель, игрушки): -%d рублей. Возраст кошки: %d.", cost, p->cat_age);
    } else {
        int paid = p->money > 0 ? p->money : 0;
        p->money -= paid;
        p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
        printf("\nНа кошку %s не хватило денег: потрачено только %d из %d рублей, питомец недоедает, настроение ухудшилось.", p->name, paid, cost);
    }
}
