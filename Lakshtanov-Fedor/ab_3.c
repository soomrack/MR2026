#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned long long RUB;

enum FoodMode {
    FOOD_CHEAP,
    FOOD_NORMAL,
    FOOD_HEALTHY
};

enum FoodStrategy {
    FOOD_DYNAMIC,
    FOOD_FIXED
};

enum EndReason {
    END_NONE = 0,
    END_MORTGAGE_PAID,
    END_APARTMENT_BOUGHT,
    END_DEATH,
    END_BANKRUPTCY
};

enum {
    TERM_1M = 0,
    TERM_3M = 1,
    TERM_6M = 2,
    TERM_12M = 3,
    TERM_COUNT = 4
};

struct Pet {
    int alive;
    int age_months;
    int lifespan_months;
    int sick;
    int sick_months_left;
    RUB monthly_cost;
    RUB vet_cost;
};

struct Mortgage {
    RUB total_due;
    RUB paid_total;
    RUB monthly_payment;
    int months_total;
    int months_paid;
};

struct Food {
    enum FoodMode mode;
    RUB monthly_cost;
    int illness_chance_ppm;
};

struct Bank {
    int id;
    double rate[TERM_COUNT];
};

struct BankDeposit {
    int bank_id;
    RUB amount;
    int term_months;
    int months_left;
    double annual_rate;
    int active;
};

struct BestDeposit {
    int bank_id;
    int term_idx;
    int term_months;
    double rate;
};

struct Person {
    char name[16];
    int age_years;
    int age_months_total;
    RUB cash;
    RUB salary_monthly;
    RUB rent_monthly;
    RUB utilities_cost;

    int has_second_job;
    int second_job_months_low_pad;
    int unemployed_months_left;
    RUB second_job_income;

    int alive;
    int months_sick;
    int illness_count;

    enum FoodStrategy food_strategy;
    struct Food food;

    struct Pet pet;

    struct Mortgage mortgage;

    struct BankDeposit deposits[20];

    RUB ytd_income;
    RUB ytd_medical;

    RUB last_month_expenses;

    RUB shortfall;

    RUB total_earned;
    RUB total_spent;
    RUB total_interest_income;
    RUB total_tax_refund;
    RUB total_rent_paid;

    int finished;
    enum EndReason end_reason;
    int bankrupt;
};

struct Config {
    double inflation_food;
    double inflation_utilities;
    double inflation_rent;
    double inflation_medical;
    double inflation_salary;
    double inflation_realestate;

    double base_deposit_rate;

    RUB salary_start;
    RUB start_cash;
    int start_age;

    RUB mortgage_principal;
    RUB mortgage_total_due;
    RUB mortgage_payment;
    int mortgage_months;

    RUB apartment_target;

    RUB ndfl_threshold;
    RUB medical_deduction_limit;

    int second_job_pad_months;
    int second_job_hold_months;
    RUB second_job_income;

    RUB food_cost_cheap;
    RUB food_cost_normal;
    RUB food_cost_healthy;
    int food_ill_ppm_cheap;
    int food_ill_ppm_normal;
    int food_ill_ppm_healthy;
    int alice_food_pad_cheap;
    int alice_food_pad_healthy;

    int sick_days_min;
    int sick_days_max;
    int death_ppm_per_year;
    int death_ppm_after_60;
    RUB medical_cost_per_sick_day;

    RUB pet_purchase_price;
    RUB pet_monthly_cost;
    RUB pet_vet_cost;
    int pet_ill_ppm_per_month;
    int pet_lifespan_min_months;
    int pet_lifespan_max_months;
    int pet_start_age_months;
    int pet_new_chance_ppm;

    RUB utilities_monthly;
    RUB rent_monthly_start;

    double deposit_rate_min;
    double deposit_rate_max;
    double deposit_rate_drift;
    int bob_pad_months;
    RUB deposit_min_amount;

    struct Bank banks[3];

    int seed;
    int start_year;
    int start_month;
};

unsigned long rnd_u32(void)
{
    unsigned long hi = (unsigned long)rand() & 0x7FFFUL;
    unsigned long lo = (unsigned long)rand() & 0x7FFFUL;
    return (hi << 15) | lo;
}

int rnd_range(int lo, int hi)
{
    unsigned long span;

    if (hi <= lo) {
        return lo;
    }

    span = (unsigned long)((long)hi - (long)lo + 1L);
    return lo + (int)(rnd_u32() % span);
}

int rnd_chance_ppm(int ppm)
{
    if (ppm <= 0) {
        return 0;
    }
    if (ppm >= 1000000) {
        return 1;
    }
    return (rnd_u32() % 1000000UL) < (unsigned long)ppm;
}

void next_month(int *year, int *month)
{
    if (year == NULL || month == NULL) {
        return;
    }

    (*month)++;
    if (*month > 12) {
        *month = 1;
        (*year)++;
    }
}

void print_money(RUB amount)
{
    char buf[32];
    char out[40];
    int len;
    int i;
    int j;
    int group;

    len = sprintf(buf, "%llu", amount);

    j = 0;
    group = 0;

    for (i = len - 1; i >= 0; --i) {
        out[j++] = buf[i];
        group++;
        if (group == 3 && i > 0) {
            out[j++] = ' ';
            group = 0;
        }
    }

    for (i = 0; i < j / 2; ++i) {
        char tmp = out[i];
        out[i] = out[j - 1 - i];
        out[j - 1 - i] = tmp;
    }
    out[j] = '\0';

    printf("%s RUB", out);
}

RUB apply_inflation_month(RUB value, double annual_inflation)
{
    double factor = 1.0 + annual_inflation / 12.0;
    return (RUB)((double)value * factor + 0.5);
}

RUB apply_inflation_year(RUB value, double annual_inflation)
{
    return (RUB)((double)value * (1.0 + annual_inflation) + 0.5);
}

void config_init(struct Config *cfg)
{
    if (cfg == NULL) {
        return;
    }

    cfg->inflation_food       = 0.06;
    cfg->inflation_utilities  = 0.06;
    cfg->inflation_rent       = 0.07;
    cfg->inflation_medical    = 0.07;
    cfg->inflation_salary     = 0.05;
    cfg->inflation_realestate = 0.07;

    cfg->base_deposit_rate = 0.10;

    cfg->salary_start = 100000;
    cfg->start_cash   = 200000;
    cfg->start_age    = 20;

    cfg->mortgage_principal = 6000000;
    cfg->mortgage_total_due = 24000000;
    cfg->mortgage_payment   = 50000;
    cfg->mortgage_months    = 480;

    cfg->apartment_target = 6000000;

    cfg->ndfl_threshold          = 3000000;
    cfg->medical_deduction_limit = 120000;

    cfg->second_job_pad_months  = 3;
    cfg->second_job_hold_months = 5;
    cfg->second_job_income      = 40000;

    cfg->food_cost_cheap    = 15000;
    cfg->food_cost_normal   = 25000;
    cfg->food_cost_healthy  = 40000;
    cfg->food_ill_ppm_cheap   = 40000;
    cfg->food_ill_ppm_normal  = 20000;
    cfg->food_ill_ppm_healthy = 8000;

    cfg->alice_food_pad_cheap   = 3;
    cfg->alice_food_pad_healthy = 6;

    cfg->sick_days_min             = 3;
    cfg->sick_days_max             = 14;
    cfg->death_ppm_per_year        = 200;
    cfg->death_ppm_after_60        = 5000;
    cfg->medical_cost_per_sick_day = 2500;

    cfg->pet_purchase_price      = 10000;
    cfg->pet_monthly_cost        = 3000;
    cfg->pet_vet_cost            = 20000;
    cfg->pet_ill_ppm_per_month   = 40000;
    cfg->pet_lifespan_min_months = 36;
    cfg->pet_lifespan_max_months = 120;
    cfg->pet_start_age_months    = 60;
    cfg->pet_new_chance_ppm      = 2000;

    cfg->utilities_monthly  = 8000;
    cfg->rent_monthly_start = 30000;

    cfg->deposit_rate_min   = 0.08;
    cfg->deposit_rate_max   = 0.15;
    cfg->deposit_rate_drift = 0.002;
    cfg->bob_pad_months     = 2;
    cfg->deposit_min_amount = 50000;

    cfg->banks[0].id = 0;
    cfg->banks[0].rate[TERM_1M]  = 0.09;
    cfg->banks[0].rate[TERM_3M]  = 0.10;
    cfg->banks[0].rate[TERM_6M]  = 0.11;
    cfg->banks[0].rate[TERM_12M] = 0.12;

    cfg->banks[1].id = 1;
    cfg->banks[1].rate[TERM_1M]  = 0.10;
    cfg->banks[1].rate[TERM_3M]  = 0.11;
    cfg->banks[1].rate[TERM_6M]  = 0.12;
    cfg->banks[1].rate[TERM_12M] = 0.13;

    cfg->banks[2].id = 2;
    cfg->banks[2].rate[TERM_1M]  = 0.11;
    cfg->banks[2].rate[TERM_3M]  = 0.12;
    cfg->banks[2].rate[TERM_6M]  = 0.13;
    cfg->banks[2].rate[TERM_12M] = 0.14;

    cfg->seed        = 20260901;
    cfg->start_year  = 2026;
    cfg->start_month = 9;
}

void person_food_update_cost(struct Person *p, struct Config *cfg);
void person_randomize_food_strategy(struct Person *p, struct Config *cfg);

void person_init(struct Person *p, const char *name,
                 struct Config *cfg, int with_mortgage)
{
    int lo, hi;

    memset(p, 0, sizeof(*p));

    snprintf(p->name, sizeof(p->name), "%s", name);

    p->age_years        = cfg->start_age;
    p->age_months_total = cfg->start_age * 12;
    p->cash             = cfg->start_cash;
    p->salary_monthly   = cfg->salary_start;

    p->alive      = 1;
    p->finished   = 0;
    p->end_reason = END_NONE;
    p->bankrupt   = 0;

    if (with_mortgage) {
        p->rent_monthly = 0;
    } else {
        p->rent_monthly = cfg->rent_monthly_start;
    }
    p->utilities_cost = cfg->utilities_monthly;

    lo = cfg->pet_start_age_months;
    if (lo < cfg->pet_lifespan_min_months) {
        lo = cfg->pet_lifespan_min_months;
    }
    hi = cfg->pet_lifespan_max_months;
    if (hi < lo) {
        hi = lo;
    }

    p->pet.alive            = 1;
    p->pet.age_months       = cfg->pet_start_age_months;
    p->pet.lifespan_months  = rnd_range(lo, hi);
    p->pet.sick             = 0;
    p->pet.sick_months_left = 0;
    p->pet.monthly_cost     = cfg->pet_monthly_cost;
    p->pet.vet_cost         = cfg->pet_vet_cost;

    person_randomize_food_strategy(p, cfg);
    person_food_update_cost(p, cfg);

    if (with_mortgage) {
        p->mortgage.total_due       = cfg->mortgage_total_due;
        p->mortgage.paid_total      = 0;
        p->mortgage.monthly_payment = cfg->mortgage_payment;
        p->mortgage.months_total    = cfg->mortgage_months;
        p->mortgage.months_paid     = 0;
    }

    p->has_second_job    = 1;
    p->second_job_income = cfg->second_job_income;
}

void person_randomize_food_strategy(struct Person *p, struct Config *cfg)
{
    (void)cfg;

    if (rnd_range(0, 1) == 0) {
        p->food_strategy = FOOD_DYNAMIC;
        p->food.mode     = FOOD_NORMAL;
    } else {
        p->food_strategy = FOOD_FIXED;

        switch (rnd_range(0, 2)) {
            case 0:
                p->food.mode = FOOD_CHEAP;
                break;
            case 1:
                p->food.mode = FOOD_NORMAL;
                break;
            default:
                p->food.mode = FOOD_HEALTHY;
                break;
        }
    }
}

void person_age_one_month(struct Person *p)
{
    p->age_months_total++;
    p->age_years = p->age_months_total / 12;

    if (p->pet.alive) {
        p->pet.age_months++;
        if (p->pet.age_months >= p->pet.lifespan_months) {
            p->pet.alive = 0;
        }
    }
}

int person_pad_months(struct Person *p)
{
    if (p->last_month_expenses == 0) {
        return 999;
    }
    return (int)(p->cash / p->last_month_expenses);
}

void person_food_update_cost(struct Person *p, struct Config *cfg)
{
    if (p->food.mode == FOOD_CHEAP) {
        p->food.monthly_cost       = cfg->food_cost_cheap;
        p->food.illness_chance_ppm = cfg->food_ill_ppm_cheap;
    } else if (p->food.mode == FOOD_NORMAL) {
        p->food.monthly_cost       = cfg->food_cost_normal;
        p->food.illness_chance_ppm = cfg->food_ill_ppm_normal;
    } else {
        p->food.monthly_cost       = cfg->food_cost_healthy;
        p->food.illness_chance_ppm = cfg->food_ill_ppm_healthy;
    }
}

void food_update_mode(struct Person *p, struct Config *cfg)
{
    int pad;
    enum FoodMode new_mode;

    if (p->food_strategy == FOOD_FIXED) {
        return;
    }

    pad = person_pad_months(p);

    if (pad < cfg->alice_food_pad_cheap) {
        new_mode = FOOD_CHEAP;
    } else if (pad <= cfg->alice_food_pad_healthy) {
        new_mode = FOOD_NORMAL;
    } else {
        new_mode = FOOD_HEALTHY;
    }

    if (new_mode != p->food.mode) {
        p->food.mode = new_mode;
        person_food_update_cost(p, cfg);
    }
}

void food_apply_inflation(struct Person *p, struct Config *cfg)
{
    p->food.monthly_cost = apply_inflation_month(p->food.monthly_cost,
                                                 cfg->inflation_food);
}

RUB food_pay(struct Person *p, struct Config *cfg)
{
    RUB cost = p->food.monthly_cost;
    RUB paid;

    (void)cfg;

    if (p->cash >= cost) {
        paid = cost;
    } else {
        paid = p->cash;
        p->shortfall += cost - paid;
    }

    p->cash                -= paid;
    p->total_spent         += paid;
    p->last_month_expenses += paid;

    return paid;
}

int food_sickness_check(struct Person *p)
{
    return rnd_chance_ppm(p->food.illness_chance_ppm);
}

RUB calc_ndfl(RUB ytd_before, RUB gross, RUB threshold)
{
    RUB below;
    RUB above;
    RUB tax = 0;

    if (gross == 0) {
        return 0;
    }

    if (ytd_before >= threshold) {
        return (RUB)((double)gross * 0.15);
    }

    below = threshold - ytd_before;
    if (below > gross) {
        below = gross;
    }
    above = gross - below;

    tax  = (RUB)((double)below * 0.13);
    tax += (RUB)((double)above * 0.15);

    return tax;
}

RUB income_salary(struct Person *p, struct Config *cfg)
{
    RUB gross;
    RUB tax;
    RUB net;

    if (p->unemployed_months_left > 0) {
        return 0;
    }

    gross = p->salary_monthly;
    tax   = calc_ndfl(p->ytd_income, gross, cfg->ndfl_threshold);
    net   = gross - tax;

    p->ytd_income   += gross;
    p->cash         += net;
    p->total_earned += gross;

    return net;
}

void income_unemployment_tick(struct Person *p)
{
    if (p->unemployed_months_left > 0) {
        p->unemployed_months_left--;
    }
}

void income_second_job_tick(struct Person *p, struct Config *cfg)
{
    int pad = person_pad_months(p);

    if (pad < cfg->second_job_pad_months) {
        p->second_job_months_low_pad++;
    } else {
        p->second_job_months_low_pad = 0;
    }

    if (p->second_job_months_low_pad >= cfg->second_job_hold_months) {
        p->has_second_job = 1;
    }
}

RUB income_second_job(struct Person *p, struct Config *cfg)
{
    RUB gross;
    RUB tax;
    RUB net;

    if (!p->has_second_job) {
        return 0;
    }
    if (p->unemployed_months_left > 0) {
        return 0;
    }

    gross = p->second_job_income;
    tax   = calc_ndfl(p->ytd_income, gross, cfg->ndfl_threshold);
    net   = gross - tax;

    p->ytd_income   += gross;
    p->cash         += net;
    p->total_earned += gross;

    return net;
}

RUB income_deposit_interest_alice(struct Person *p, struct Config *cfg)
{
    RUB pad_target;
    RUB base;
    RUB interest;
    double rate_m;

    pad_target = p->last_month_expenses;

    if (p->cash <= pad_target) {
        return 0;
    }

    base     = p->cash - pad_target;
    rate_m   = cfg->base_deposit_rate / 12.0;
    interest = (RUB)((double)base * rate_m + 0.5);

    p->cash                  += interest;
    p->total_interest_income += interest;
    p->total_earned          += interest;

    return interest;
}

RUB income_tax_refund(struct Person *p, struct Config *cfg)
{
    RUB med_part;
    RUB refund;

    med_part = p->ytd_medical;
    if (med_part > cfg->medical_deduction_limit) {
        med_part = cfg->medical_deduction_limit;
    }

    refund = (RUB)((double)med_part * 0.13 + 0.5);

    p->ytd_medical = 0;

    p->cash             += refund;
    p->total_tax_refund += refund;

    return refund;
}

RUB pay(struct Person *p, RUB cost)
{
    RUB paid;

    if (p->cash >= cost) {
        paid = cost;
    } else {
        paid = p->cash;
        p->shortfall += cost - paid;
    }

    p->cash                -= paid;
    p->total_spent         += paid;
    p->last_month_expenses += paid;

    return paid;
}

void expenses_utilities(struct Person *p, struct Config *cfg)
{
    p->utilities_cost = apply_inflation_month(p->utilities_cost,
                                              cfg->inflation_utilities);
    pay(p, p->utilities_cost);
}

void expenses_rent(struct Person *p, struct Config *cfg)
{
    RUB paid;

    if (p->rent_monthly == 0) {
        return;
    }

    p->rent_monthly = apply_inflation_month(p->rent_monthly,
                                            cfg->inflation_rent);

    paid = pay(p, p->rent_monthly);
    p->total_rent_paid += paid;
}

void expenses_pet_tick(struct Person *p, struct Config *cfg)
{
    if (p->pet.alive) {

        if (p->pet.sick) {
            if (p->pet.sick_months_left > 0) {
                p->pet.sick_months_left--;
            }
            if (p->pet.sick_months_left == 0) {
                p->pet.sick = 0;
            }
        } else {

            if (rnd_chance_ppm(cfg->pet_ill_ppm_per_month)) {
                p->pet.sick = 1;
                p->pet.sick_months_left = rnd_range(1, 2);
            }
        }

        if (p->pet.age_months >= p->pet.lifespan_months) {
            p->pet.alive = 0;
            p->pet.sick  = 0;
        }
        return;
    }

    if (p->cash <= cfg->pet_purchase_price) {
        return;
    }

    if (rnd_chance_ppm(cfg->pet_new_chance_ppm)) {
        pay(p, cfg->pet_purchase_price);

        p->pet.alive            = 1;
        p->pet.age_months       = 0;
        p->pet.lifespan_months  = rnd_range(cfg->pet_lifespan_min_months, cfg->pet_lifespan_max_months);
        p->pet.sick             = 0;
        p->pet.sick_months_left = 0;
        p->pet.monthly_cost     = cfg->pet_monthly_cost;
        p->pet.vet_cost         = cfg->pet_vet_cost;
    }
}

void expenses_pet_pay(struct Person *p, struct Config *cfg)
{
    RUB cost;

    if (!p->pet.alive) {
        return;
    }

    p->pet.monthly_cost = apply_inflation_month(p->pet.monthly_cost,
                                                cfg->inflation_medical);
    p->pet.vet_cost     = apply_inflation_month(p->pet.vet_cost,
                                                cfg->inflation_medical);

    cost = p->pet.monthly_cost;

    if (p->pet.sick) {
        cost += p->pet.vet_cost;
    }

    pay(p, cost);
}

double drift_rate(double rate, struct Config *cfg)
{
    double delta;
    double sign;
    double new_rate;

    if (rnd_range(0, 1) == 0) {
        sign = -1.0;
    } else {
        sign = 1.0;
    }

    delta    = cfg->deposit_rate_drift * sign;
    new_rate = rate + delta;

    if (new_rate < cfg->deposit_rate_min) {
        new_rate = cfg->deposit_rate_min;
    }
    if (new_rate > cfg->deposit_rate_max) {
        new_rate = cfg->deposit_rate_max;
    }

    return new_rate;
}

void deposits_tick_rates(struct Config *cfg)
{
    int b, t;

    for (b = 0; b < 3; ++b) {
        for (t = 0; t < TERM_COUNT; ++t) {
            cfg->banks[b].rate[t] = drift_rate(cfg->banks[b].rate[t], cfg);
        }
    }
}

void best_deposit_option(struct Config *cfg, struct BestDeposit *best)
{
    int b, t;
    int term_months_table[TERM_COUNT] = {1, 3, 6, 12};

    best->bank_id     = 0;
    best->term_idx    = 0;
    best->term_months = 1;
    best->rate        = 0.0;

    for (b = 0; b < 3; ++b) {
        for (t = 0; t < TERM_COUNT; ++t) {
            double r = cfg->banks[b].rate[t];
            int    m = term_months_table[t];

            if (r > best->rate + 1e-9 ||
                (fabs(r - best->rate) < 1e-9 && m > best->term_months)) {
                best->bank_id     = b;
                best->term_idx    = t;
                best->term_months = m;
                best->rate        = r;
            }
        }
    }
}

int find_free_slot(struct Person *p)
{
    int i;
    for (i = 0; i < 20; ++i) {
        if (!p->deposits[i].active) {
            return i;
        }
    }
    return -1;
}

void close_deposit(struct Person *p, struct BankDeposit *d)
{
    RUB payout;
    double rate_m;
    int months = d->term_months;
    RUB interest;

    if (p->bankrupt) {

        payout = d->amount;
    } else {
        rate_m   = d->annual_rate / 12.0;
        payout   = d->amount;
        interest = (RUB)((double)d->amount * rate_m * (double)months + 0.5);
        payout  += interest;
        p->total_interest_income += interest;
        p->total_earned          += interest;
    }

    p->cash += payout;
    d->active = 0;
    d->amount = 0;
    d->months_left = 0;
}

void deposits_tick(struct Person *p, struct Config *cfg)
{
    int i;
    int slot;
    RUB pad_target;
    RUB free_money;
    struct BestDeposit best;

    if (p->last_month_expenses > 0 && p->cash < p->last_month_expenses) {
        RUB need = p->last_month_expenses - p->cash;

        for (i = 0; i < 20 && need > 0; ++i) {
            if (!p->deposits[i].active) {
                continue;
            }

            RUB take = p->deposits[i].amount;
            if (take > need) {
                take = need;
            }

            p->cash += take;
            p->deposits[i].amount -= take;
            need -= take;

            if (p->deposits[i].amount == 0) {
                p->deposits[i].active = 0;
            }
        }
    }

    for (i = 0; i < 20; ++i) {
        if (!p->deposits[i].active) {
            continue;
        }

        p->deposits[i].months_left--;
        if (p->deposits[i].months_left <= 0) {
            close_deposit(p, &p->deposits[i]);
        }
    }

    slot = find_free_slot(p);
    if (slot < 0) {
        return;
    }

    pad_target = p->last_month_expenses * cfg->bob_pad_months;
    if (p->cash <= pad_target) {
        return;
    }

    free_money = p->cash - pad_target;
    if (free_money < cfg->deposit_min_amount) {
        return;
    }

    best_deposit_option(cfg, &best);

    p->deposits[slot].bank_id     = best.bank_id;
    p->deposits[slot].amount      = free_money;
    p->deposits[slot].term_months = best.term_months;
    p->deposits[slot].months_left = best.term_months;
    p->deposits[slot].annual_rate = best.rate;
    p->deposits[slot].active      = 1;

    p->cash -= free_money;
}

int events_fire_check(struct Person *p)
{
    if (p->unemployed_months_left > 0) {
        return 0;
    }

    if (!rnd_chance_ppm(5000)) {
        return 0;
    }

    p->unemployed_months_left = rnd_range(1, 6);
    return 1;
}

int events_promotion_check(struct Person *p)
{
    if (p->unemployed_months_left > 0) {
        return 0;
    }

    if (!rnd_chance_ppm(15000)) {
        return 0;
    }

    p->salary_monthly = (RUB)((double)p->salary_monthly * 1.15);
    return 1;
}

int events_illness_check(struct Person *p, struct Config *cfg)
{
    int days;
    RUB cost;

    if (p->months_sick > 0) {
        return 0;
    }

    if (!food_sickness_check(p)) {
        return 0;
    }

    days = rnd_range(cfg->sick_days_min, cfg->sick_days_max);

    p->months_sick = (days + 29) / 30;
    if (p->months_sick < 1) {
        p->months_sick = 1;
    }
    p->illness_count++;

    cost = (RUB)days * cfg->medical_cost_per_sick_day;
    p->ytd_medical += cost;
    p->total_spent += cost;

    if (p->cash >= cost) {
        p->cash -= cost;
    } else {
        p->shortfall += cost - p->cash;
        p->cash = 0;
    }

    p->last_month_expenses += cost;

    return 1;
}

void events_sickness_tick(struct Person *p)
{
    if (p->months_sick > 0) {
        p->months_sick--;
    }
}

int events_death_check(struct Person *p, struct Config *cfg)
{
    int ppm_year;
    int ppm_month;

    ppm_year = cfg->death_ppm_per_year;

    if (p->age_years > 60) {
        ppm_year += cfg->death_ppm_after_60 * (p->age_years - 60);
    }

    ppm_month = ppm_year / 12;
    if (ppm_month < 1) {
        ppm_month = 1;
    }

    if (rnd_chance_ppm(ppm_month)) {
        p->alive      = 0;
        p->finished   = 1;
        p->end_reason = END_DEATH;
        return 1;
    }

    return 0;
}

void mortgage_pay(struct Person *p, struct Config *cfg)
{
    RUB remaining;
    RUB payment;
    RUB paid;

    (void)cfg;

    if (p->mortgage.total_due == 0) {
        return;
    }

    remaining = p->mortgage.total_due - p->mortgage.paid_total;
    if (remaining == 0) {
        return;
    }

    payment = p->mortgage.monthly_payment;

    if (p->cash >= payment) {
        paid = payment;
    } else {
        paid = p->cash;
        p->shortfall += payment - paid;
    }

    if (paid > remaining) {
        paid = remaining;
    }

    p->mortgage.paid_total += paid;
    p->cash                -= paid;
    p->total_spent         += paid;
    p->last_month_expenses += paid;
    p->mortgage.months_paid++;

    if (p->mortgage.paid_total >= p->mortgage.total_due) {
        p->finished   = 1;
        p->end_reason = END_MORTGAGE_PAID;
    }
}

RUB mortgage_early_repay(struct Person *p, struct Config *cfg)
{
    RUB remaining;
    RUB pad_target;

    (void)cfg;

    if (p->mortgage.total_due == 0) {
        return 0;
    }

    remaining = p->mortgage.total_due - p->mortgage.paid_total;
    if (remaining == 0) {
        return 0;
    }

    pad_target = p->last_month_expenses * 6;

    if (p->cash < remaining + pad_target) {
        return 0;
    }

    p->cash -= remaining;
    p->mortgage.paid_total = p->mortgage.total_due;
    p->total_spent += remaining;

    p->finished   = 1;
    p->end_reason = END_MORTGAGE_PAID;

    return remaining;
}

void alice_month(struct Person *p, struct Config *cfg)
{
    events_fire_check(p);
    events_promotion_check(p);
    events_illness_check(p, cfg);
    events_sickness_tick(p);
    events_death_check(p, cfg);

    if (p->finished) {
        return;
    }

    income_unemployment_tick(p);
    income_second_job_tick(p, cfg);
    income_salary(p, cfg);
    income_second_job(p, cfg);
    income_deposit_interest_alice(p, cfg);

    food_update_mode(p, cfg);
    food_apply_inflation(p, cfg);
    food_pay(p, cfg);

    expenses_utilities(p, cfg);

    expenses_pet_tick(p, cfg);
    expenses_pet_pay(p, cfg);

    mortgage_pay(p, cfg);
    if (p->finished) {
        return;
    }
    mortgage_early_repay(p, cfg);
    if (p->finished) {
        return;
    }

    if (p->shortfall > 0) {
        p->bankrupt   = 1;
        p->finished   = 1;
        p->end_reason = END_BANKRUPTCY;
    }
}

void alice_year_tick(struct Person *p, struct Config *cfg)
{
    p->salary_monthly = apply_inflation_year(p->salary_monthly,
                                             cfg->inflation_salary);
    p->second_job_income = apply_inflation_year(p->second_job_income,
                                                cfg->inflation_salary);
    cfg->ndfl_threshold = apply_inflation_year(cfg->ndfl_threshold,
                                               cfg->inflation_salary);
}

RUB total_deposits(struct Person *p)
{
    RUB total = 0;
    int i;

    for (i = 0; i < 20; ++i) {
        if (p->deposits[i].active) {
            total += p->deposits[i].amount;
        }
    }
    return total;
}

int bob_check_apartment(struct Person *p, struct Config *cfg)
{
    RUB savings = p->cash + total_deposits(p);
    RUB need;
    int i;

    if (savings < cfg->apartment_target) {
        return 0;
    }

    need = cfg->apartment_target;

    for (i = 0; i < 20 && need > 0; ++i) {
        if (!p->deposits[i].active) {
            continue;
        }

        RUB take = p->deposits[i].amount;
        if (take > need) {
            take = need;
        }

        p->deposits[i].amount -= take;
        need -= take;

        if (p->deposits[i].amount == 0) {
            p->deposits[i].active = 0;
        }
    }

    if (need > 0) {
        p->cash -= need;
    }

    p->total_spent += cfg->apartment_target;

    p->finished   = 1;
    p->end_reason = END_APARTMENT_BOUGHT;
    return 1;
}

void bob_month(struct Person *p, struct Config *cfg)
{
    events_fire_check(p);
    events_promotion_check(p);
    events_illness_check(p, cfg);
    events_sickness_tick(p);
    events_death_check(p, cfg);

    if (p->finished) {
        return;
    }

    income_unemployment_tick(p);
    income_second_job_tick(p, cfg);
    income_salary(p, cfg);
    income_second_job(p, cfg);

    food_update_mode(p, cfg);
    food_apply_inflation(p, cfg);
    food_pay(p, cfg);

    expenses_utilities(p, cfg);
    expenses_rent(p, cfg);

    expenses_pet_tick(p, cfg);
    expenses_pet_pay(p, cfg);

    deposits_tick(p, cfg);

    if (p->shortfall > 0) {
        p->bankrupt   = 1;
        p->finished   = 1;
        p->end_reason = END_BANKRUPTCY;
        return;
    }

    bob_check_apartment(p, cfg);
}

void bob_year_tick(struct Person *p, struct Config *cfg)
{
    p->salary_monthly = apply_inflation_year(p->salary_monthly,
                                             cfg->inflation_salary);
    p->second_job_income = apply_inflation_year(p->second_job_income,
                                                cfg->inflation_salary);
    cfg->ndfl_threshold = apply_inflation_year(cfg->ndfl_threshold,
                                               cfg->inflation_salary);
    cfg->apartment_target = apply_inflation_year(cfg->apartment_target,
                                                 cfg->inflation_realestate);
    cfg->deposit_min_amount = apply_inflation_year(cfg->deposit_min_amount,
                                                   0.05);
}

enum EndReason simulation_run(struct Person *p,
                              struct Config *cfg,
                              int is_bob)
{
    int year;
    int month;
    int i;
    int max_months = 700;

    year  = cfg->start_year;
    month = cfg->start_month;

    for (i = 0; i < max_months; ++i) {

        p->shortfall           = 0;
        p->last_month_expenses = 0;

        if (is_bob) {
            deposits_tick_rates(cfg);
        }

        if (is_bob) {
            bob_month(p, cfg);
        } else {
            alice_month(p, cfg);
        }

        if (month == 1 && !p->finished) {
            income_tax_refund(p, cfg);

            if (is_bob) {
                bob_year_tick(p, cfg);
            } else {
                alice_year_tick(p, cfg);
            }
        }

        if (p->finished) {
            return p->end_reason;
        }

        person_age_one_month(p);
        next_month(&year, &month);

        if (p->age_years >= 65) {
            income_tax_refund(p, cfg);
            p->alive      = 0;
            p->finished   = 1;
            p->end_reason = END_DEATH;
            return END_DEATH;
        }
    }

    p->finished   = 1;
    p->end_reason = END_NONE;
    return END_NONE;
}

void simulation_print_result(struct Person *p, struct Config *cfg)
{
    const char *reason_text;
    RUB total_dep;
    int i;

    (void)cfg;

    switch (p->end_reason) {
        case END_MORTGAGE_PAID:
            reason_text = "mortgage paid off";
            break;
        case END_APARTMENT_BOUGHT:
            reason_text = "apartment bought";
            break;
        case END_DEATH:
            reason_text = "death";
            break;
        case END_BANKRUPTCY:
            reason_text = "bankruptcy";
            break;
        default:
            reason_text = "reached simulation limit";
            break;
    }

    printf("  %s\n", p->name);
    printf("  end reason:      %s\n", reason_text);
    printf("  final age:       %d\n", p->age_years);

    printf("  cash:            ");
    print_money(p->cash);
    printf("\n");

    total_dep = 0;
    for (i = 0; i < 20; ++i) {
        if (p->deposits[i].active) {
            total_dep += p->deposits[i].amount;
        }
    }
    printf("  deposits total:  ");
    print_money(total_dep);
    printf("\n");
    printf("  cash + deposits: ");
    print_money(p->cash + total_dep);
    printf("\n");

    printf("  total earned:    ");
    print_money(p->total_earned);
    printf("\n");

    printf("  total spent:     ");
    print_money(p->total_spent);
    printf("\n");

    printf("  total interest:  ");
    print_money(p->total_interest_income);
    printf("\n");

    printf("  total tax refund: ");
    print_money(p->total_tax_refund);
    printf("\n");

    printf("  total rent paid: ");
    print_money(p->total_rent_paid);
    printf("\n");

    printf("  illness count:   %d\n", p->illness_count);

    if (p->mortgage.total_due > 0) {
        printf("  mortgage paid:   ");
        print_money(p->mortgage.paid_total);
        printf(" / ");
        print_money(p->mortgage.total_due);
        printf("\n");
    }

    printf("\n");
}

int main(void)
{
    struct Config cfg;
    struct Person alice;
    struct Person bob;

    config_init(&cfg);
    srand(cfg.seed);

    person_init(&alice, "Alice", &cfg, 1);
    simulation_run(&alice, &cfg, 0);

    config_init(&cfg);
    srand(cfg.seed);

    person_init(&bob, "Bob", &cfg, 0);
    simulation_run(&bob, &cfg, 1);

    printf("\n\n");

    simulation_print_result(&alice, &cfg);
    simulation_print_result(&bob, &cfg);

    return 0;
}
