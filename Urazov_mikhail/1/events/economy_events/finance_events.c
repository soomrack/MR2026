#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(first_deposit, STAGE_YOUTH)

EVENT_CHECK(first_deposit) {
    return p->money > 5000 && w->economy >= ESTATUS_NORMAL;
}

EVENT_RESULT(first_deposit) {
    int interest = (int)((long long)p->money * 4 / 100);
    p->money += interest;
    printf("\n%s открыл(а) свой первый вклад, проценты составили %s рублей.", p->name, format_money(interest));
}

EVENT_REGISTRATION(bank_fee, STAGE_YOUTH)

EVENT_CHECK(bank_fee) {
    return p->money > 1000;
}

EVENT_RESULT(bank_fee) {
    p->money -= 300;
    printf("\nБанк списал у %s комиссию за обслуживание карты: 300 рублей.", p->name);
}

EVENT_REGISTRATION(deposit_interest, STAGE_YOUTH)

EVENT_CHECK(deposit_interest) {
    return p->money > 20000 && w->economy >= ESTATUS_NORMAL;
}

EVENT_RESULT(deposit_interest) {
    int interest = (int)((long long)p->money * (3 + (int)dice()) / 100);
    p->money += interest;
    printf("\nНакопления %s принесли процент по вкладу: %s рублей.", p->name, format_money(interest));
}

EVENT_REGISTRATION(traffic_fine, STAGE_YOUTH)

EVENT_CHECK(traffic_fine) {
    return p->money > 3000 && dice() <= LUCK_NORMAL;
}

EVENT_RESULT(traffic_fine) {
    int fine = 1000 + (int)dice() * 500;
    p->money -= fine;
    printf("\n%s получил(а) штраф за нарушение правил: %s рублей.", p->name, format_money(fine));
}

EVENT_REGISTRATION(currency_jump, STAGE_YOUTH)

EVENT_CHECK(currency_jump) {
    return p->money > 10000 && w->economy <= ESTATUS_CRISIS;
}

EVENT_RESULT(currency_jump) {
    int loss = p->money / 10;
    p->money -= loss;
    printf("\nКурс валюты резко изменился, сбережения %s обесценились на %s рублей.", p->name, format_money(loss));
}

EVENT_REGISTRATION(tax_refund, STAGE_MIDDLEAGE)

EVENT_CHECK(tax_refund) {
    return p->salary > 0;
}

EVENT_RESULT(tax_refund) {
    int refund = (int)((long long)p->salary * (2 + (int)dice()) / 100);
    p->money += refund;
    printf("\n%s оформил(а) налоговый вычет и получил(а) %d рублей.", p->name, refund);
}

EVENT_REGISTRATION(stock_investment, STAGE_MIDDLEAGE)

EVENT_CHECK(stock_investment) {
    return p->money > 40000;
}

EVENT_RESULT(stock_investment) {
    int invested = p->money / 5;
    int change;
    if (w->economy >= ESTATUS_NORMAL && dice() >= LUCK_NORMAL) {
        change = invested * 25 / 100;
        printf("\nИнвестиции %s в акции выросли, прибыль составила %s рублей.", p->name, format_money(change));
    } else {
        change = -(invested * 30 / 100);
        printf("\nИнвестиции %s в акции упали в цене, убыток составил %s рублей.", p->name, format_money(-change));
    }
    p->money += change;
}

EVENT_REGISTRATION(insurance_payout, STAGE_MIDDLEAGE)

EVENT_CHECK(insurance_payout) {
    return p->health < 60 && dice() >= LUCK_NORMAL;
}

EVENT_RESULT(insurance_payout) {
    int payout = 4000 + (int)dice() * 1000;
    p->money += payout;
    printf("\nСтраховая компания выплатила %s компенсацию за лечение: %s рублей.", p->name, format_money(payout));
}

EVENT_REGISTRATION(pension_fund_bonus, STAGE_OLD)

EVENT_CHECK(pension_fund_bonus) {
    return w->economy >= ESTATUS_NORMAL;
}

EVENT_RESULT(pension_fund_bonus) {
    int bonus = 2500 + (int)dice() * 500;
    p->money += bonus;
    printf("\nНакопительная часть пенсии %s принесла доход: %s рублей.", p->name, format_money(bonus));
}

EVENT_REGISTRATION(scam_call, STAGE_OLD)

EVENT_CHECK(scam_call) {
    return p->money > 5000 && dice() <= LUCK_BAD;
}

EVENT_RESULT(scam_call) {
    int loss = 3000 + (int)dice() * 1000;
    p->money -= loss;
    p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\nМошенники обманули %s по телефону и выманили %s рублей.", p->name, format_money(loss));
}
