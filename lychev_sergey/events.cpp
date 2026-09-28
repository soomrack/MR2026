#include "events.h"
#include <cstdio>
#include <cstdlib>

EventLog event_log;

void EventLog::print() const
{
    if (lines.empty()) return;
    std::printf("\n=== Лог событий (%d) ===\n", (int)lines.size());
    for (const auto& s : lines)
        std::printf("  %s\n", s.c_str());
}

bool roll_chance(double p)
{
    return (static_cast<double>(std::rand()) / RAND_MAX) < p;
}

int roll_range(int lo, int hi)
{
    return lo + std::rand() % (hi - lo + 1);
}

static std::string stamp(int year, int month, const std::string& who)
{
    char buf[64];
    std::snprintf(buf, sizeof(buf), "[%04d-%02d] %-5s", year, month, who.c_str());
    return std::string(buf);
}

static void treat(Person& p, int year, int month,
                  const std::string& name,
                  RUB cost, int health_loss, int sick_days)
{
    bool can_pay = (p.cash >= cost);

    if (can_pay) {
        p.cash -= cost;
        p.total_expense += cost;
        p.total_medical_spent += cost;
        p.medical_spent_this_year += cost;
        int actual = health_loss / 2;
        person_change_health(p, -actual);
        event_log.add(stamp(year, month, p.name) + " " + name +
                      " (лечение): -" + std::to_string((long long)cost));
    } else {
        p.cash -= cost;
        p.total_expense += cost;
        p.total_medical_spent += cost;
        p.medical_spent_this_year += cost;
        person_change_health(p, -health_loss);
        event_log.add(stamp(year, month, p.name) + " " + name +
                      " (БЕЗ лечения): -" + std::to_string((long long)cost));
    }

    person_change_happiness(p, -3);

    if (sick_days > 0) p.sick_months = sick_days;
}

void event_promotion(Person& p, int year, int month)
{
    if (!p.alive || p.bankrupt || month != 12) return;
    if (p.unemployed_months > 0 || p.sick_months > 0) return;
    if (!roll_chance(0.20)) return;

    p.salary = static_cast<RUB>(static_cast<double>(p.salary) * 1.10);
    event_log.add(stamp(year, month, p.name) +
                  " повышение: " + std::to_string((long long)p.salary));
}

void event_job_loss(Person& p, int year, int month)
{
    if (!p.alive || p.bankrupt || p.unemployed_months > 0) return;
    if (!roll_chance(0.02)) return;
    p.unemployed_months = roll_range(3, 6);
    person_change_happiness(p, -10);
    event_log.add(stamp(year, month, p.name) + " потеря работы");
}

void event_new_job(Person& p, int year, int month)
{
    if (!p.alive || p.bankrupt || p.unemployed_months == 0) return;
    if (!roll_chance(0.25)) return;
    p.unemployed_months = 0;
    p.salary = static_cast<RUB>(static_cast<double>(p.salary) * 0.90);
    person_change_happiness(p, +5);
    event_log.add(stamp(year, month, p.name) + " новая работа");
}

void event_vacation(Person& p, int year, int month)
{
    if (!p.alive || p.bankrupt || month != 7) return;
    if (!roll_chance(0.40)) return;
    RUB cost = 80000;
    p.cash -= cost;
    p.total_expense += cost;
    person_change_happiness(p, +10);
    event_log.add(stamp(year, month, p.name) + " отпуск: -80000");
}

void event_bonus(Person& p, int year, int month)
{
    if (!p.alive || p.bankrupt) return;

    double chance = 0.05;
    if (p.happiness > 70)      chance = 0.08;
    else if (p.happiness > 40) chance = 0.05;
    else                       chance = 0.00;

    if (!roll_chance(chance)) return;

    RUB amount = 50000;
    p.cash += amount;
    p.total_income += amount;
    person_change_happiness(p, +2);
    event_log.add(stamp(year, month, p.name) + " премия: +50000");
}

void event_cold(Person& p, int year, int month)
{
    if (!p.alive) return;
    if (!roll_chance(0.05)) return;
    treat(p, year, month, "простуда", 3000, 2, 0);
    p.cold_count++;
}

void event_flu(Person& p, int year, int month)
{
    if (!p.alive) return;
    if (!roll_chance(0.02)) return;
    treat(p, year, month, "грипп", 7000, 4, 0);
    p.flu_count++;
}

void event_food_poisoning(Person& p, int year, int month)
{
    if (!p.alive) return;
    if (!roll_chance(0.01)) return;
    treat(p, year, month, "отравление", 5000, 3, 0);
}

void event_injury(Person& p, int year, int month)
{
    if (!p.alive) return;
    if (!roll_chance(0.005)) return;
    treat(p, year, month, "травма", 40000, 10, 1);
    p.injury_count++;
}

void event_hospital(Person& p, int year, int month)
{
    if (!p.alive) return;
    if (!roll_chance(0.001)) return;
    treat(p, year, month, "больница", 100000, 20, 2);
    p.hospital_count++;
    p.health_max -= 10;
    if (p.health_max < 30) p.health_max = 30;
}

void event_chronic(Person& p, int year, int month)
{
    if (!p.alive) return;
    if (p.has_chronic_disease) return;
    if (!roll_chance(0.003)) return;

    p.has_chronic_disease = true;
    p.chronic_monthly_cost = 3000;
    p.chronic_count++;
    p.health_max -= 5;
    if (p.health_max < 40) p.health_max = 40;
    person_change_happiness(p, -5);
    event_log.add(stamp(year, month, p.name) +
                  " хроническая болезнь (3000/мес)");
}

void event_illness(Person& p, int year, int month)
{
    if (!p.alive) return;
    if (p.sick_months > 0) return;

    event_cold(p, year, month);
    event_flu(p, year, month);
    event_food_poisoning(p, year, month);
    event_injury(p, year, month);
    event_chronic(p, year, month);
    event_hospital(p, year, month);
}

void event_depression(Person& p, int year, int month)
{
    if (!p.alive) return;

    if (!p.depressed && p.happiness < 20) {
        p.depressed = true;
        p.depressed_months = 0;
        event_log.add(stamp(year, month, p.name) + " ДЕПРЕССИЯ началась");
    }

    if (p.depressed) {
        p.depressed_months++;

        RUB cost = 15000;
        if (p.cash >= cost) {
            p.cash -= cost;
            p.total_expense += cost;
            p.total_therapy_spent += cost;
            person_change_happiness(p, +5);
        } else {
            p.cash -= cost;
            p.total_expense += cost;
            p.total_therapy_spent += cost;
            person_change_happiness(p, -5);
        }

        if (p.happiness > 40) {
            p.depressed = false;
            event_log.add(stamp(year, month, p.name) + " вышел из депрессии");
        }
    }
}

/* ============================================================
 * Восстановление: +2 здоровья и счастья в месяц.
 * ============================================================ */

void event_recovery(Person& p, int year, int month)
{
    (void)year; (void)month;
    if (!p.alive) return;

    if (p.health < p.health_max) person_change_health(p, +2);
    if (p.happiness < 100)       person_change_happiness(p, +2);
}

/* ============================================================
 * "Жизненный дренаж" счастья:
 *   - работа (если не безработный) −1
 *   - своё жильё: +3 (спокойствие)
 *   - съём жилья: −1 (лёгкий стресс)
 *   - активные потреб. кредиты: −2
 *   - активные микрозаймы: −3
 * ============================================================ */

void event_life_drain(Person& p, int year, int month)
{
    (void)year; (void)month;
    if (!p.alive || p.bankrupt) return;

    if (p.unemployed_months == 0)
        person_change_happiness(p, -1);

    /* своё жильё vs съём */
    if (p.has_apartment) person_change_happiness(p, +3);
    else                 person_change_happiness(p, -1);

    /* активные потреб. кредиты */
    bool has_loan = false;
    for (int i = 0; i < p.loan_count; ++i) {
        if (p.loan_accs[i] >= 0 &&
            bank_balance(*p.bank, p.loan_accs[i]) > 0) {
            has_loan = true;
            break;
        }
    }
    if (has_loan) person_change_happiness(p, -2);

    /* активные микрозаймы */
    bool has_mfo = false;
    if (p.mfo) {
        for (int i = 0; i < p.mfo_count; ++i) {
            if (p.mfo_accs[i] >= 0 &&
                bank_balance(*p.mfo, p.mfo_accs[i]) > 0) {
                has_mfo = true;
                break;
            }
        }
    }
    if (has_mfo) person_change_happiness(p, -3);
}

void event_tax_return(Person& p, int year, int month)
{
    (void)year;
    if (!p.alive || p.bankrupt) return;
    if (month != 4) return;

    RUB base = p.medical_spent_this_year + p.mortgage_interest_this_year;
    RUB refund = static_cast<RUB>(static_cast<double>(base) * 0.13);

    RUB max_refund = static_cast<RUB>(
        static_cast<double>(p.salary * 12) * 0.13);
    if (refund > max_refund) refund = max_refund;

    if (refund > 0) {
        p.cash += refund;
        p.total_income += refund;
        event_log.add(stamp(year, month, p.name) + " налоговый вычет: +" +
                      std::to_string((long long)refund));
    }

    p.medical_spent_this_year = 0;
    p.mortgage_interest_this_year = 0;
}

void process_all_events(Person& p, int year, int month)
{
    event_promotion(p, year, month);
    event_job_loss(p, year, month);
    event_new_job(p, year, month);

    event_illness(p, year, month);
    event_depression(p, year, month);

    event_vacation(p, year, month);
    event_bonus(p, year, month);
    event_tax_return(p, year, month);

    event_life_drain(p, year, month);
    event_recovery(p, year, month);
}