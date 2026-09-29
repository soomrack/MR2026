#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"
#include "../../utils.h"

#include "../__event.h"

EVENT_REGISTRATION(birthday_party, STAGE_YOUTH)

EVENT_CHECK(birthday_party) {
    return p->money > 1500;
}

EVENT_RESULT(birthday_party) {
    p->money -= 1500;
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s отметил(а) день рождения с друзьями (1,500 рублей), настроение поднялось.", p->name);
}

EVENT_REGISTRATION(travel_trip, STAGE_YOUTH)

EVENT_CHECK(travel_trip) {
    return p->money > 12000;
}

EVENT_RESULT(travel_trip) {
    int cost = 8000 + (int)dice() * 1000;
    p->money -= cost;
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    p->health = clamp_int(p->health + 3, 0, 100);
    printf("\n%s съездил(а) в путешествие за %s рублей, вернулся(ась) отдохнувшим(ей).", p->name, format_money(cost));
}

EVENT_REGISTRATION(friend_loan, STAGE_YOUTH)

EVENT_CHECK(friend_loan) {
    return p->money > 10000;
}

EVENT_RESULT(friend_loan) {
    int loan = 3000 + (int)dice() * 1000;
    p->money -= loan;
    if (dice() >= LUCK_NORMAL) {
        p->money += loan;
        printf("\n%s одолжил(а) другу %s рублей, и тот вернул долг вовремя.", p->name, format_money(loan));
    } else {
        p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
        printf("\n%s одолжил(а) другу %s рублей, но деньги так и не вернули.", p->name, format_money(loan));
    }
}

EVENT_REGISTRATION(theft, STAGE_YOUTH)

EVENT_CHECK(theft) {
    return p->money > 8000 && dice() <= LUCK_BAD;
}

EVENT_RESULT(theft) {
    int loss = 2000 + (int)dice() * 1500;
    p->money -= loss;
    p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\nУ %s украли кошелек, потеряно %s рублей.", p->name, format_money(loss));
}

EVENT_REGISTRATION(having_child, STAGE_MIDDLEAGE)

EVENT_CHECK(having_child) {
    return p->mood >= MOOD_NORMAL && p->money > 15000;
}

EVENT_RESULT(having_child) {
    int cost = 10000 + (int)dice() * 1500;
    p->money -= cost;
    p->mood = MOOD_PERFECT;
    printf("\nУ %s родился ребенок! Приданое и врачи обошлись в %s рублей, но счастью нет предела.", p->name, format_money(cost));
}

EVENT_REGISTRATION(move_apartment, STAGE_MIDDLEAGE)

EVENT_CHECK(move_apartment) {
    return p->money > 20000 && p->utilities_tariff > 0;
}

EVENT_RESULT(move_apartment) {
    int old_tariff = p->utilities_tariff;
    p->money -= 12000;
    p->utilities_tariff = old_tariff + (int)dice() * 1200 - 2000;
    printf("\n%s переехал(а) в другую квартиру (расходы 12,000 рублей), коммуналка в год: %s -> %s.", p->name, format_money(old_tariff), format_money(p->utilities_tariff));
}

EVENT_REGISTRATION(inheritance, STAGE_MIDDLEAGE)

EVENT_CHECK(inheritance) {
    return dice() >= LUCK_GOOD;
}

EVENT_RESULT(inheritance) {
    int sum = 15000 + (int)dice() * 5000;
    p->money += sum;
    p->mood = clamp_int((int)p->mood - 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s получил(а) наследство от дальнего родственника: %s рублей.", p->name, format_money(sum));
}

EVENT_REGISTRATION(family_dinner, STAGE_OLD)

EVENT_CHECK(family_dinner) {
    return p->mood < MOOD_PERFECT;
}

EVENT_RESULT(family_dinner) {
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\nВся семья собралась за большим столом у %s, на душе стало теплее.", p->name);
}

EVENT_REGISTRATION(charity, STAGE_OLD)

EVENT_CHECK(charity) {
    return p->money > 20000;
}

EVENT_RESULT(charity) {
    int donation = 2000 + (int)dice() * 500;
    p->money -= donation;
    p->mood = clamp_int((int)p->mood + 1, MOOD_AWFUL, MOOD_PERFECT);
    printf("\n%s пожертвовал(а) %s рублей на благотворительность и почувствовал(а) себя нужным(ой).", p->name, format_money(donation));
}

EVENT_REGISTRATION(lottery_win, STAGE_OLD)

EVENT_CHECK(lottery_win) {
    return dice() == LUCK_PERFECT;
}

EVENT_RESULT(lottery_win) {
    int prize = 20000 + (int)dice() * 10000;
    p->money += prize;
    p->mood = MOOD_PERFECT;
    printf("\n%s выиграл(а) в лотерею %s рублей! Такая удача бывает раз в жизни.", p->name, format_money(prize));
}
