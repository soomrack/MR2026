#include "person.h"
#include <cmath>
#include <cstdio>

static int clamp_points(int v)
{
    if (v < 0)   return 0;
    if (v > 100) return 100;
    return v;
}

static int clamp_score(int v)
{
    if (v < 300) return 300;
    if (v > 850) return 850;
    return v;
}

static RUB inflate_monthly(RUB value, double rate_year_percent)
{
    double r_month = monthly_rate_from_year_percent(rate_year_percent);
    return static_cast<RUB>(static_cast<double>(value) * (1.0 + r_month));
}

static RUB safe_sub(RUB a, RUB b)
{
    if (b > a) {
        if (b > 1000000000000000LL) return 0;
        return a - b;
    }
    return a - b;
}

void person_init(Person& p, const std::string& name, Bank& bank)
{
    p.name = name;
    p.bank = &bank;
    p.mfo  = nullptr;

    p.debit_acc    = -1;
    p.deposit_acc  = -1;
    p.mortgage_acc = -1;

    p.loan_count = 0;
    p.mfo_count  = 0;
    for (int i = 0; i < MAX_CREDITS; ++i) {
        p.loan_accs[i] = -1;
        p.mfo_accs[i]  = -1;
    }

    p.cash   = 200000;
    p.salary = 100000;
    p.salary2 = 0;
    p.salary_indexation = 6.0;

    p.has_side_job         = false;
    p.side_job_months_left = 0;
    p.months_in_deficit    = 0;
    p.side_job_income      = 30000;

    p.food      = 25000;
    p.utilities = 8000;
    p.rent      = 0;
    p.pet       = 3000;

    p.infl_food      = 6.0;
    p.infl_utilities = 8.0;
    p.infl_rent      = 6.0;
    p.infl_pet       = 7.0;

    p.health     = 90;
    p.health_max = 100;
    p.happiness  = 80;

    p.alive    = true;
    p.bankrupt = false;

    p.unemployed_months = 0;
    p.sick_months       = 0;

    p.depressed = false;
    p.depressed_months = 0;
    p.total_therapy_spent = 0;

    p.cold_count = 0;
    p.flu_count  = 0;
    p.injury_count = 0;
    p.chronic_count = 0;
    p.hospital_count = 0;
    p.has_chronic_disease = false;
    p.chronic_monthly_cost = 0;
    p.total_medical_spent = 0;
    p.medical_spent_this_year = 0;
    p.mortgage_interest_this_year = 0;

    p.credit_score = 700;

    p.has_apartment   = false;
    p.apartment_price = 8000000;

    p.current_year  = 2026;
    p.current_month = 9;

    p.has_mortgage         = false;
    p.mortgage_debt        = 0;
    p.mortgage_payment     = 0;
    p.mortgage_months_left = 0;
    p.total_mortgage_interest = 0;

    p.has_cat            = true;
    p.cat_age_months     = 36;
    p.months_without_cat = 0;
    p.months_until_new_cat = 0;
    p.cats_total         = 1;

    car_init(p.car);

    p.total_income = 0;
    p.total_expense = 0;
    p.total_interest_paid = 0;

    p.credits_taken = 0;
    p.micro_taken   = 0;
    p.total_borrowed = 0;
}

void person_attach_accounts(Person& p, int debit, int deposit, int mortgage)
{
    p.debit_acc    = debit;
    p.deposit_acc  = deposit;
    p.mortgage_acc = mortgage;
}

void person_attach_mfo(Person& p, Bank& mfo)
{
    p.mfo = &mfo;
}

void person_apply_salary(Person& p, int month)
{
    if (!p.alive) return;

    if (p.unemployed_months > 0) { p.unemployed_months--; return; }
    if (p.sick_months > 0)       { p.sick_months--;       return; }

    if (month == 12) {
        p.salary = static_cast<RUB>(
            static_cast<double>(p.salary) * (1.0 + p.salary_indexation / 100.0));
    }

    RUB income = p.salary + p.salary2;
    if (p.depressed) {
        income = static_cast<RUB>(static_cast<double>(income) * 0.8);
    }

    p.cash += income;
    p.total_income += income;
}

/* ============================================================
 * Подработка при дефиците.
 *
 * Если cash < 0 два месяца подряд — берёт вторую работу.
 * Работает 6 месяцев. Доход 30 000/мес.
 * ============================================================ */

void person_update_side_job(Person& p, int month)
{
    (void)month;
    if (!p.alive || p.bankrupt) return;

    if (p.cash < 0) p.months_in_deficit++;
    else            p.months_in_deficit = 0;

    if (!p.has_side_job && p.months_in_deficit >= 2) {
        p.has_side_job = true;
        p.side_job_months_left = 6;
        p.salary2 = p.side_job_income;
    }

    if (p.has_side_job) {
        p.side_job_months_left--;
        if (p.side_job_months_left <= 0) {
            p.has_side_job = false;
            p.salary2 = 0;
        }
    }
}

static void pay_expense(Person& p, RUB value)
{
    if (value == 0) return;
    p.cash -= value;
    p.total_expense += value;
}

void person_pay_food(Person& p)      { pay_expense(p, p.food);      p.food      = inflate_monthly(p.food, p.infl_food); }
void person_pay_utilities(Person& p) { pay_expense(p, p.utilities); p.utilities = inflate_monthly(p.utilities, p.infl_utilities); }

void person_pay_rent(Person& p)
{
    pay_expense(p, p.rent);
    if (p.rent > 0) p.rent = inflate_monthly(p.rent, p.infl_rent);
}

void person_pay_car(Person& p, int year, int month)
{
    car_monthly(p.car, p.cash, p.total_expense, year, month, p.name.c_str());
}

void person_pay_chronic(Person& p)
{
    if (!p.has_chronic_disease) return;
    pay_expense(p, p.chronic_monthly_cost);
    p.total_medical_spent += p.chronic_monthly_cost;
    p.medical_spent_this_year += p.chronic_monthly_cost;
}

void person_pay_pet(Person& p, int year, int month)
{
    (void)year; (void)month;

    if (!p.has_cat) {
        p.months_without_cat++;
        if (p.months_until_new_cat == 0)
            p.months_until_new_cat = 2 + std::rand() % 4;

        if (p.months_without_cat >= p.months_until_new_cat) {
            p.has_cat = true;
            p.cat_age_months = 0;
            p.pet = 3000;
            p.months_without_cat = 0;
            p.months_until_new_cat = 0;
            p.cats_total++;
            person_change_happiness(p, +15);
        }
        return;
    }

    pay_expense(p, p.pet);
    p.pet = inflate_monthly(p.pet, p.infl_pet);
    p.cat_age_months++;

    if (p.cat_age_months >= 15 * 12) {
        p.has_cat = false;
        p.months_without_cat = 0;
        person_change_happiness(p, -30);
    }
}

void person_move_to_deposit(Person& p, RUB amount)
{
    if (amount > p.cash) amount = p.cash;
    if (amount <= 0) return;
    if (p.deposit_acc < 0) return;

    p.cash -= amount;
    bank_deposit(*p.bank, p.deposit_acc, amount);
}

void person_ensure_cash(Person& p, RUB target)
{
    if (p.cash >= target) return;
    if (p.deposit_acc < 0) return;

    RUB need = target - p.cash;
    RUB have = bank_balance(*p.bank, p.deposit_acc);
    RUB take = (need < have) ? need : have;

    if (take > 0) {
        bank_withdraw(*p.bank, p.deposit_acc, take);
        p.cash += take;
    }
}

void person_bankrupt_austerity(Person& p)
{
    if (p.food > 15000)      p.food = 15000;
    if (p.utilities > 5000)  p.utilities = 5000;
    if (p.pet > 1500)        p.pet = 1500;
    p.car.has_car = false;
    if (p.rent < 20000) p.rent = 20000;
}

void person_change_health(Person& p, int delta)
{
    p.health += delta;
    if (p.health > p.health_max) p.health = p.health_max;
    if (p.health < 0) p.health = 0;
    if (p.health == 0) p.alive = false;
}

void person_change_happiness(Person& p, int delta)
{
    p.happiness = clamp_points(p.happiness + delta);
}

void person_change_credit_score(Person& p, int delta)
{
    p.credit_score = clamp_score(p.credit_score + delta);
}

RUB person_monthly_expenses(const Person& p)
{
    RUB total = p.food + p.utilities + p.rent + p.pet + p.mortgage_payment;
    if (p.has_chronic_disease) total += p.chronic_monthly_cost;
    return total;
}

RUB person_apartment_price(const Person& p, int year, int month)
{
    (void)p;
    int months = (year - 2026) * 12 + (month - 9);
    if (months < 0) months = 0;
    double r_month = monthly_rate_from_year_percent(6.0);
    double k = std::pow(1.0 + r_month, months);
    return static_cast<RUB>(8000000.0 * k);
}

RUB person_liquid_capital(const Person& p)
{
    RUB net = p.cash;

    if (p.deposit_acc >= 0)  net += bank_balance(*p.bank, p.deposit_acc);
    if (p.debit_acc >= 0)    net += bank_balance(*p.bank, p.debit_acc);

    for (int i = 0; i < p.loan_count; ++i) {
        if (p.loan_accs[i] >= 0)
            net = safe_sub(net, bank_balance(*p.bank, p.loan_accs[i]));
    }
    if (p.mfo) {
        for (int i = 0; i < p.mfo_count; ++i) {
            if (p.mfo_accs[i] >= 0)
                net = safe_sub(net, bank_balance(*p.mfo, p.mfo_accs[i]));
        }
    }

    net = safe_sub(net, p.mortgage_debt);
    if (p.car.has_car) net += 1000000;

    return net;
}

RUB person_net_worth(const Person& p)
{
    RUB net = person_liquid_capital(p);
    if (p.has_apartment) {
        RUB market = person_apartment_price(p, p.current_year, p.current_month);
        net += market;
    }
    return net;
}

void person_sell_apartment(Person& p)
{
    if (!p.has_apartment) return;
    p.cash += p.apartment_price;
    p.has_apartment = false;
    p.has_mortgage  = false;
    p.mortgage_debt = 0;
}

void person_print(const Person& p)
{
    std::printf("\n=== %s ===\n", p.name.c_str());
    std::printf("Жив: %s, банкрот: %s, депрессия: %s\n",
                p.alive ? "да" : "нет",
                p.bankrupt ? "да" : "нет",
                p.depressed ? "да" : "нет");
    std::printf("Наличные:          %lld\n", (long long)p.cash);
    if (p.deposit_acc >= 0)
        std::printf("Вклад:             %lld\n",
                    (long long)bank_balance(*p.bank, p.deposit_acc));
    for (int i = 0; i < p.loan_count; ++i) {
        if (p.loan_accs[i] >= 0) {
            RUB d = bank_balance(*p.bank, p.loan_accs[i]);
            if (d > 0) std::printf("Потреб. кредит #%d: %lld\n", i, (long long)d);
        }
    }
    if (p.mfo) {
        for (int i = 0; i < p.mfo_count; ++i) {
            if (p.mfo_accs[i] >= 0) {
                RUB d = bank_balance(*p.mfo, p.mfo_accs[i]);
                if (d > 0) std::printf("Микрозайм #%d:      %lld\n", i, (long long)d);
            }
        }
    }
    if (p.has_mortgage)
        std::printf("Ипотека (долг):    %lld (осталось %d мес)\n",
                    (long long)p.mortgage_debt, p.mortgage_months_left);
    std::printf("Квартира:          %s\n", p.has_apartment ? "да" : "нет");
    if (p.has_apartment)
        std::printf("Стоимость квартиры (рынок): %lld\n",
                    (long long)person_apartment_price(p, p.current_year, p.current_month));
    std::printf("Машина:            %s\n",
                !p.car.has_car ? "нет" : p.car.broken ? "сломана" : "рабочая");
    std::printf("Котик:             %s (возраст %d мес, всего %d)\n",
                p.has_cat ? "жив" : "нет", p.cat_age_months, p.cats_total);
    std::printf("Зарплата:          %lld", (long long)p.salary);
    if (p.has_side_job)
        std::printf(" + подработка %lld (осталось %d мес)",
                    (long long)p.side_job_income, p.side_job_months_left);
    std::printf("\n");
    std::printf("Без работы: %d мес, болеет: %d мес\n",
                p.unemployed_months, p.sick_months);
    std::printf("Здоровье: %d / %d, счастье: %d\n",
                p.health, p.health_max, p.happiness);
    std::printf("Кредитный рейтинг: %d\n", p.credit_score);
    std::printf("Хроническая: %s", p.has_chronic_disease ? "да" : "нет");
    if (p.has_chronic_disease)
        std::printf(" (лечение %lld/мес)", (long long)p.chronic_monthly_cost);
    std::printf("\n");
    std::printf("Простуд:%d гриппов:%d травм:%d больниц:%d\n",
                p.cold_count, p.flu_count, p.injury_count, p.hospital_count);
    std::printf("Потрачено медицина: %lld\n", (long long)p.total_medical_spent);
    std::printf("Всего доходов:     %lld\n", (long long)p.total_income);
    std::printf("Всего расходов:    %lld\n", (long long)p.total_expense);
    std::printf("Проценты:          %lld\n", (long long)p.total_interest_paid);
    std::printf("Кредитов: %d (микро: %d)\n", p.credits_taken, p.micro_taken);
    std::printf("Ликвидный капитал: %lld\n", (long long)person_liquid_capital(p));
    std::printf("Чистый капитал:    %lld\n", (long long)person_net_worth(p));
}