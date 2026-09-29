#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(water_meters, STAGE_TEENAGE)

EVENT_CHECK(water_meters) {
    return p->utilities_tariff > 4000;
}

EVENT_RESULT(water_meters) {
    int saved = p->utilities_tariff * 8 / 100;
    p->utilities_tariff -= saved;
    printf("\n%s поставил(а) счетчики воды, коммуналка стала дешевле на %d рублей в год.", p->name, saved);
}

EVENT_REGISTRATION(tariff_hike, STAGE_YOUTH)

EVENT_CHECK(tariff_hike) {
    return w->economy <= ESTATUS_NORMAL && dice() <= LUCK_NORMAL;
}

EVENT_RESULT(tariff_hike) {
    int extra = p->utilities_tariff * 15 / 100;
    p->utilities_tariff += extra;
    printf("\nГород резко поднял тарифы на коммунальные услуги: плюс %d рублей в год для %s.", extra, p->name);
}

EVENT_REGISTRATION(energy_saving, STAGE_YOUTH)

EVENT_CHECK(energy_saving) {
    return p->utilities_tariff > 4000 && dice() >= LUCK_GOOD;
}

EVENT_RESULT(energy_saving) {
    int saved = p->utilities_tariff * 10 / 100;
    p->utilities_tariff -= saved;
    printf("\n%s заменил(а) лампы и технику на энергосберегающие, коммуналка стала меньше на %d рублей в год.", p->name, saved);
}

EVENT_REGISTRATION(pipe_burst, STAGE_MIDDLEAGE)

EVENT_CHECK(pipe_burst) {
    return p->money > 6000;
}

EVENT_RESULT(pipe_burst) {
    int repair = 3000 + (int)dice() * 800;
    p->money -= repair;
    printf("\nУ %s прорвало трубу, аварийный ремонт обошелся в %d рублей.", p->name, repair);
}

EVENT_REGISTRATION(utility_recalculation, STAGE_MIDDLEAGE)

EVENT_CHECK(utility_recalculation) {
    return p->utilities_debt > 0;
}

EVENT_RESULT(utility_recalculation) {
    int forgiven = p->utilities_debt / 2;
    p->utilities_debt -= forgiven;
    printf("\nУправляющая компания сделала перерасчет: долг %s за коммуналку уменьшился на %d рублей.", p->name, forgiven);
}

EVENT_REGISTRATION(utility_subsidy, STAGE_OLD)

EVENT_CHECK(utility_subsidy) {
    return p->utilities_tariff > 0;
}

EVENT_RESULT(utility_subsidy) {
    int saved = p->utilities_tariff * 20 / 100;
    p->utilities_tariff -= saved;
    printf("\n%s оформил(а) субсидию на оплату коммунальных услуг, платеж уменьшился на %d рублей в год.", p->name, saved);
}
