#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long long int RUB;

struct Finances {
    RUB cash;
    RUB savings;
    RUB cash_reserve;
    RUB excess;
    RUB total_income;
    RUB total_expenses;
    int bankrupt_count;
    int emergency_refills;
};

struct Deposit {
    int active;
    RUB principal;
    RUB rate_bp;
    int months_left;
    int start_year;
    int start_month;

    RUB total_interest;
    int deposits_opened;
};

struct Job {
    RUB salary;
    RUB salary2;
    RUB bonus;
    RUB bonus_rate_bp;
    char const* title;
    char const* title2;

    RUB ndfl_paid_this_year;
    RUB ndfl_paid_last_year;
    RUB ndfl_paid_total;

    RUB property_deduction_used;
    RUB property_deduction_cap;
    RUB mortgage_deduction_used;
    RUB mortgage_deduction_cap;
    RUB social_deduction_used_total;
    RUB social_deduction_used_year;
    RUB social_deduction_cap;
};

struct Housing {
    char const* type;
    RUB rent_amount;
    RUB utility_base;
    RUB market_value;

    RUB mortgage_debt;
    RUB mortgage_payment;
    RUB mortgage_rate_bp;
    int mortgage_months_left;

    RUB total_utility_paid;
    RUB total_rent_paid;
    RUB total_mortgage_paid;

    RUB total_repair_paid;
    int repairs_count;
};

struct Living {
    RUB food_base;
    RUB dining_out;
    RUB healthy_food_extra;
    int food_quality;
    RUB groceries_extra;
    RUB total_food_paid;
};

struct Transport {
    int owned;
    int age_months;
    RUB value;
    RUB monthly_expenses;

    char const* model;
    int fuel_type;
    RUB fuel_cost_per_month;
    RUB maintenance_cost;
    RUB insurance_osago;
    RUB insurance_kasko;
    int has_kasko;
    int tech_inspection_due;
    int months_since_purchase;
    RUB total_car_spent;
    int breakdowns_minor;
    int breakdowns_major;
    int license_suspended_months;
};

struct Pet {
    int alive;
    int age_months;
    RUB monthly_expenses;
    char const* name;

    RUB total_pet_paid;
    int illness_count;
    int vaccine_count;
    int sterilized;

    RUB food_monthly;
    RUB vet_monthly;

    int death_year;
    int death_month;
    int lifespan_months;
};

struct Business {
    int pvz_open;
    int pvz_months;
    int pvz_open_year;
    int pvz_open_month;
    RUB pvz_income;
    RUB pvz_costs;
    RUB pvz_invested;
    RUB total_pvz_net;
};

struct Health {
    RUB total_spent;
    int months_sick;
    int months_seriously_sick;
};

struct Events {
    int fired;
    int inheritance;
    int car_crash;
    int car_crash_severe;
    int promotion;
    int food_jump;
    int tax_audit;
    int flat_bought_year;
    int flat_bought_month;
};

struct Person {
    struct Finances  money;
    struct Deposit   deposit;
    struct Job       job;
    struct Housing   home;
    struct Living    life;
    struct Transport car;
    struct Pet       cat;
    struct Business  biz;
    struct Health    health;
    struct Events    events;

    int use_mortgage;
    int months_lived;
};

struct Person Zeckster;

int roll_d100() {
    int limit = RAND_MAX - (RAND_MAX % 100);
    int r;
    do { r = rand(); } while (r >= limit);
    return r % 100 + 1;
}

void Zeckster_init(int use_mortgage) {
    Zeckster.money.cash = 20'000;
    Zeckster.money.savings = 0;
    Zeckster.money.cash_reserve = 80'000;
    Zeckster.money.excess = 0;
    Zeckster.money.total_income = 0;
    Zeckster.money.total_expenses = 0;
    Zeckster.money.bankrupt_count = 0;
    Zeckster.money.emergency_refills = 0;

    Zeckster.deposit.active = 0;
    Zeckster.deposit.principal = 0;
    Zeckster.deposit.rate_bp = 1500;
    Zeckster.deposit.months_left = 0;
    Zeckster.deposit.start_year = 0;
    Zeckster.deposit.start_month = 0;
    Zeckster.deposit.total_interest = 0;
    Zeckster.deposit.deposits_opened = 0;

    Zeckster.job.salary = 30'000;
    Zeckster.job.salary2 = 0;
    Zeckster.job.bonus = 0;
    Zeckster.job.bonus_rate_bp = 0;
    Zeckster.job.title = "Student";
    Zeckster.job.title2 = "None";

    Zeckster.job.ndfl_paid_this_year = 0;
    Zeckster.job.ndfl_paid_last_year = 0;
    Zeckster.job.ndfl_paid_total = 0;

    Zeckster.job.property_deduction_used = 0;
    Zeckster.job.property_deduction_cap = 260'000;
    Zeckster.job.mortgage_deduction_used = 0;
    Zeckster.job.mortgage_deduction_cap = 390'000;
    Zeckster.job.social_deduction_used_total = 0;
    Zeckster.job.social_deduction_used_year = 0;
    Zeckster.job.social_deduction_cap = 19'500;

    Zeckster.home.type = "Dormitory";
    Zeckster.home.rent_amount = 2'500;
    Zeckster.home.utility_base = 6'000;
    Zeckster.home.market_value = 0;
    Zeckster.home.mortgage_debt = 0;
    Zeckster.home.mortgage_payment = 0;
    Zeckster.home.mortgage_rate_bp = 900;
    Zeckster.home.mortgage_months_left = 0;

    Zeckster.home.total_utility_paid = 0;
    Zeckster.home.total_rent_paid = 0;
    Zeckster.home.total_mortgage_paid = 0;
    Zeckster.home.total_repair_paid = 0;
    Zeckster.home.repairs_count = 0;

    Zeckster.life.food_base = 12'000;
    Zeckster.life.dining_out = 3'000;
    Zeckster.life.healthy_food_extra = 0;
    Zeckster.life.food_quality = 0;
    Zeckster.life.groceries_extra = 0;
    Zeckster.life.total_food_paid = 0;

    Zeckster.car.owned = 0;
    Zeckster.car.age_months = 0;
    Zeckster.car.value = 0;
    Zeckster.car.monthly_expenses = 0;
    Zeckster.car.model = "none";
    Zeckster.car.fuel_type = 0;
    Zeckster.car.fuel_cost_per_month = 0;
    Zeckster.car.maintenance_cost = 0;
    Zeckster.car.insurance_osago = 0;
    Zeckster.car.insurance_kasko = 0;
    Zeckster.car.has_kasko = 0;
    Zeckster.car.tech_inspection_due = 0;
    Zeckster.car.months_since_purchase = 0;
    Zeckster.car.total_car_spent = 0;
    Zeckster.car.breakdowns_minor = 0;
    Zeckster.car.breakdowns_major = 0;
    Zeckster.car.license_suspended_months = 0;

    Zeckster.cat.alive = 0;
    Zeckster.cat.age_months = 0;
    Zeckster.cat.monthly_expenses = 4'000;
    Zeckster.cat.name = "Barsik";
    Zeckster.cat.total_pet_paid = 0;
    Zeckster.cat.illness_count = 0;
    Zeckster.cat.vaccine_count = 0;
    Zeckster.cat.sterilized = 0;
    Zeckster.cat.food_monthly = 4'000;
    Zeckster.cat.vet_monthly = 1'500;
    Zeckster.cat.death_year = 0;
    Zeckster.cat.death_month = 0;
    Zeckster.cat.lifespan_months = 0;

    Zeckster.biz.pvz_open = 0;
    Zeckster.biz.pvz_months = 0;
    Zeckster.biz.pvz_open_year = 0;
    Zeckster.biz.pvz_open_month = 0;
    Zeckster.biz.pvz_income = 0;
    Zeckster.biz.pvz_costs = 0;
    Zeckster.biz.pvz_invested = 0;
    Zeckster.biz.total_pvz_net = 0;

    Zeckster.health.total_spent = 0;
    Zeckster.health.months_sick = 0;
    Zeckster.health.months_seriously_sick = 0;

    Zeckster.events.fired = 0;
    Zeckster.events.inheritance = 0;
    Zeckster.events.car_crash = 0;
    Zeckster.events.car_crash_severe = 0;
    Zeckster.events.promotion = 0;
    Zeckster.events.food_jump = 0;
    Zeckster.events.tax_audit = 0;
    Zeckster.events.flat_bought_year = 0;
    Zeckster.events.flat_bought_month = 0;

    Zeckster.use_mortgage = use_mortgage;
    Zeckster.months_lived = 0;
}

void Zeckster_check_reserve() {
    if (Zeckster.money.cash < Zeckster.money.cash_reserve &&
        Zeckster.money.savings > 0) {
        RUB need = Zeckster.money.cash_reserve - Zeckster.money.cash;
        RUB take = (need < Zeckster.money.savings) ? need : Zeckster.money.savings;
        Zeckster.money.savings -= take;
        Zeckster.money.cash += take;
        Zeckster.money.emergency_refills++;
    }
}

void Zeckster_pay(RUB amount) {
    if (amount == 0) return;

    if (amount > Zeckster.money.cash) {
        Zeckster_check_reserve();
    }

    if (amount > Zeckster.money.cash) {
        Zeckster.money.bankrupt_count++;
        Zeckster.money.total_expenses += Zeckster.money.cash;
        Zeckster.money.cash = 0;
    }
    else {
        Zeckster.money.cash -= amount;
        Zeckster.money.total_expenses += amount;
    }
}

void Zeckster_inflation(const int year, const int month) {
    if (month == 1) {
        Zeckster.home.utility_base = (RUB)(Zeckster.home.utility_base * 1.07);
        Zeckster.life.food_base = (RUB)(Zeckster.life.food_base * 1.09);
        Zeckster.life.dining_out = (RUB)(Zeckster.life.dining_out * 1.09);
        Zeckster.life.healthy_food_extra = (RUB)(Zeckster.life.healthy_food_extra * 1.09);
        Zeckster.cat.food_monthly = (RUB)(Zeckster.cat.food_monthly * 1.10);
        Zeckster.cat.vet_monthly = (RUB)(Zeckster.cat.vet_monthly * 1.08);
        Zeckster.car.fuel_cost_per_month = (RUB)(Zeckster.car.fuel_cost_per_month * 1.06);

        if (Zeckster.home.market_value == 0) {
            Zeckster.home.rent_amount = (RUB)(Zeckster.home.rent_amount * 1.08);
        }
        else {
            Zeckster.home.market_value = (RUB)(Zeckster.home.market_value * 1.05);
        }
    }
}

void Zeckster_health_step(int month) {
    int roll = roll_d100();
    RUB cost = 0;
    int serious = 0;

    int sick_modifier = 0;
    if (Zeckster.life.food_quality == 2) sick_modifier = -5;
    if (Zeckster.life.food_quality == 0) sick_modifier = +3;

    if (month == 12 || month == 1 || month == 2) {
        if (roll <= 18 + sick_modifier)      cost = 5'000;
        else if (roll <= 22 + sick_modifier) { cost = 15'000; serious = 1; }
    }
    else if (month >= 3 && month <= 5) {
        if (roll <= 20 + sick_modifier)      cost = 5'000;
        else if (roll <= 25 + sick_modifier) { cost = 15'000; serious = 1; }
    }
    else if (month >= 6 && month <= 8) {
        if (roll <= 6 + sick_modifier)       cost = 5'000;
        else if (roll <= 8 + sick_modifier) { cost = 15'000; serious = 1; }
    }
    else {
        if (roll <= 23 + sick_modifier)      cost = 5'000;
        else if (roll <= 29 + sick_modifier) { cost = 15'000; serious = 1; }
    }

    if (cost > 0) {
        Zeckster_pay(cost);
        Zeckster.health.total_spent += cost;
        Zeckster.health.months_sick++;
        if (serious) Zeckster.health.months_seriously_sick++;
    }
}

void Zeckster_repair_step(const int year, const int month) {
    if (Zeckster.home.market_value == 0) return;
    if (month != 7) return;

    int roll = roll_d100();
    if (roll <= 40) {
        RUB cost;
        int type = roll_d100();
        if (type <= 60)      cost = 50'000;
        else if (type <= 90) cost = 150'000;
        else                 cost = 300'000;

        Zeckster_pay(cost);
        Zeckster.home.total_repair_paid += cost;
        Zeckster.home.repairs_count++;
    }
}

int Zeckster_cat_death_threshold() {
    int age = Zeckster.cat.age_months;
    if (age < 120) return 0;
    if (age < 144) return 1;
    if (age < 168) return 2;
    if (age < 192) return 5;
    return 10;
}

void Zeckster_cat_mechanics(const int year, const int month) {
    if (Zeckster.cat.alive == 0 && Zeckster.cat.death_year == 0 &&
        year == 2028 && month == 6) {
        Zeckster.cat.alive = 1;
        Zeckster.cat.age_months = 0;
        return;
    }

    if (Zeckster.cat.alive == 0) return;

    int death_threshold = Zeckster_cat_death_threshold();
    if (death_threshold > 0) {
        int roll = roll_d100();
        if (roll <= death_threshold) {
            Zeckster.cat.alive = 0;
            Zeckster.cat.death_year = year;
            Zeckster.cat.death_month = month;
            Zeckster.cat.lifespan_months = Zeckster.cat.age_months;
            return;
        }
    }

    if (!Zeckster.cat.sterilized && Zeckster.cat.age_months >= 6 && Zeckster.cat.age_months <= 12) {
        RUB cost = 15'000;
        Zeckster_pay(cost);
        Zeckster.cat.total_pet_paid += cost;
        Zeckster.cat.sterilized = 1;
    }

    if (month == 3) {
        RUB cost = 5'000;
        Zeckster_pay(cost);
        Zeckster.cat.total_pet_paid += cost;
        Zeckster.cat.vaccine_count++;
    }

    if (month != 3) {
        int roll = roll_d100();
        if (roll <= 5) {
            RUB cost = 10'000 + roll_d100() * 200;
            Zeckster_pay(cost);
            Zeckster.cat.total_pet_paid += cost;
            Zeckster.cat.illness_count++;
        }
    }

    RUB monthly_food = Zeckster.cat.food_monthly;
    RUB monthly_vet = Zeckster.cat.vet_monthly;
    if (Zeckster.cat.age_months > 120) {
        monthly_food = (RUB)(monthly_food * 1.5);
        monthly_vet = (RUB)(monthly_vet * 1.5);
    }

    Zeckster_pay(monthly_food + monthly_vet);
    Zeckster.cat.total_pet_paid += monthly_food + monthly_vet;

    Zeckster.cat.age_months++;
}

void Zeckster_car_crash_event(const int year, const int month) {
    if (month != 9) return;
    if (Zeckster.car.owned == 0) return;

    int roll = roll_d100();
    if (roll > 2) return;

    int type_roll = roll_d100();
    RUB fine, repair;
    int severe = 0;

    if (type_roll <= 60) {
        fine = 30'000;
        repair = 70'000;
    }
    else if (type_roll <= 90) {
        fine = 100'000;
        repair = 200'000;
    }
    else {
        fine = 300'000;
        repair = 500'000;
        severe = 1;
    }

    if (Zeckster.car.has_kasko) {
        fine = fine / 2;
    }

    RUB total = fine + repair;

    Zeckster.car.value = (Zeckster.car.value > repair)
        ? Zeckster.car.value - repair
        : 50'000;

    Zeckster_pay(total);

    if (severe) {
        Zeckster.car.license_suspended_months = 3;
        Zeckster.events.car_crash_severe++;
    }

    Zeckster.events.car_crash++;
}

void Zeckster_tax_audit_event(const int year, const int month) {
    if (month != 4) return;
    if (Zeckster.biz.pvz_open == 0) return;
    if (year < 2030) return;

    int roll = roll_d100();
    if (roll <= 2) {
        RUB fine = 50'000;
        RUB extra_tax = 100'000;
        RUB total = fine + extra_tax;
        Zeckster_pay(total);
        Zeckster.events.tax_audit++;
    }
}

void Zeckster_random_events(const int year, const int month) {
    if (month == 6 && year >= 2029) {
        int roll = roll_d100();
        if (roll <= 1) {
            RUB lost = Zeckster.job.salary * 3;
            Zeckster_pay(lost);
            Zeckster.events.fired++;
        }
    }

    if (month == 3 && year >= 2032) {
        int roll = roll_d100();
        if (roll <= 1) {
            Zeckster.money.cash += 2'000'000;
            Zeckster.money.total_income += 2'000'000;
            Zeckster.events.inheritance++;
        }
    }

    if (month == 6 && year >= 2029 && Zeckster.job.salary > 0) {
        int roll = roll_d100();
        if (roll <= 3) {
            Zeckster.job.salary = (RUB)(Zeckster.job.salary * 1.15);
            Zeckster.events.promotion++;
        }
    }

    if (month == 8 && year >= 2028) {
        int roll = roll_d100();
        if (roll <= 5) {
            Zeckster.life.food_base = (RUB)(Zeckster.life.food_base * 1.10);
            Zeckster.events.food_jump++;
        }
    }
}

// Lead engineer с 2030/1, зарплата 140 000
void Zeckster_salary(const int year, const int month) {
    if (year == 2027 && month == 8) {
        Zeckster.job.salary = 65'000;
        Zeckster.job.title = "Beginning engineer";
        Zeckster.job.bonus_rate_bp = 4'000;
    }
    if (year == 2028 && month == 9) {
        Zeckster.job.salary = 80'000;
        Zeckster.job.title = "Engineer";
        Zeckster.job.bonus_rate_bp = 6'000;
    }

    // ← повышение до Lead engineer с 2030/1, зарплата 140 000
    if (year == 2030 && month == 1) {
        Zeckster.job.salary = 140'000;
        Zeckster.job.title = "Lead engineer";
        Zeckster.job.bonus_rate_bp = 10'000;

        // После повышения фриланс больше не нужен
        Zeckster.job.salary2 = 0;
        Zeckster.job.title2 = "None";
    }

    if (month == 1 && year >= 2028) {
        Zeckster.job.salary = (RUB)(Zeckster.job.salary * 1.08);
        if (Zeckster.job.salary2 > 0) {
            Zeckster.job.salary2 = (RUB)(Zeckster.job.salary2 * 1.08);
        }
    }

    Zeckster.money.cash += Zeckster.job.salary;

    // Фриланс — только в 2029
    if (year == 2029) {
        if (Zeckster.job.salary2 == 0) {
            Zeckster.job.salary2 = 25'000;
            Zeckster.job.title2 = "Freelance";
        }
        Zeckster.money.cash += Zeckster.job.salary2;
    }

    Zeckster.money.total_income += Zeckster.job.salary + Zeckster.job.salary2;
}

void Zeckster_ndfl(const int year, const int month) {
    if (month == 1) {
        Zeckster.job.ndfl_paid_last_year = Zeckster.job.ndfl_paid_this_year;
        Zeckster.job.ndfl_paid_this_year = 0;
        Zeckster.job.social_deduction_used_year = 0;
    }

    RUB gross = Zeckster.job.salary + Zeckster.job.salary2;
    RUB ndfl = (RUB)(gross * 0.13);

    Zeckster.job.ndfl_paid_this_year += ndfl;
    Zeckster.job.ndfl_paid_total += ndfl;
}

void Zeckster_bonus_step(const int year, const int month) {
    if (month == 12 && Zeckster.job.bonus_rate_bp > 0) {
        Zeckster.job.bonus = Zeckster.job.salary *
            Zeckster.job.bonus_rate_bp / 10'000;

        Zeckster.money.cash += Zeckster.job.bonus;
        Zeckster.money.total_income += Zeckster.job.bonus;
    }
}

void Zeckster_groceries_step(const int year, const int month) {
    int roll = roll_d100();
    if (roll <= 15) {
        RUB extra = 5'000 + roll_d100() * 100;
        Zeckster.life.groceries_extra = extra;
        Zeckster_pay(extra);
        Zeckster.life.total_food_paid += extra;
    }
    else {
        Zeckster.life.groceries_extra = 0;
    }
}

void Zeckster_food_quality_step(const int year, const int month) {
    if (year == 2030 && month == 1 && Zeckster.life.food_quality < 2) {
        Zeckster.life.food_quality = 2;
        Zeckster.life.healthy_food_extra = 6'000;
        Zeckster.life.dining_out = 8'000;
    }
    if (year == 2027 && month == 1 && Zeckster.life.food_quality < 1) {
        Zeckster.life.food_quality = 1;
        Zeckster.life.healthy_food_extra = 2'000;
    }
}

void Zeckster_food_step(const int year, const int month) {
    RUB total = Zeckster.life.food_base
        + Zeckster.life.dining_out
        + Zeckster.life.healthy_food_extra;
    Zeckster_pay(total);
    Zeckster.life.total_food_paid += total;
}

void Zeckster_utility_step(const int year, const int month) {
    Zeckster_pay(Zeckster.home.utility_base);
    Zeckster.home.total_utility_paid += Zeckster.home.utility_base;
}

void Zeckster_housing_step(const int year, const int month) {
    RUB total = 0;
    RUB rent = 0;
    RUB mortgage = 0;

    if (Zeckster.home.market_value == 0) {
        rent = Zeckster.home.rent_amount;
        total += rent;
    }
    if (Zeckster.home.mortgage_debt > 0) {
        mortgage = Zeckster.home.mortgage_payment;
        total += mortgage;
    }
    Zeckster_pay(total);
    Zeckster.home.total_rent_paid += rent;
    Zeckster.home.total_mortgage_paid += mortgage;
}

void Zeckster_savings_interest() {
    RUB interest = (RUB)(Zeckster.money.savings * 0.10 / 12.0);
    Zeckster.money.savings += interest;
    Zeckster.money.total_income += interest;
}

void Zeckster_tax_refund(const int year, const int month) {
    if (month != 4) return;

    RUB total_refund = 0;

    if (Zeckster.home.market_value > 0 &&
        Zeckster.job.property_deduction_used < Zeckster.job.property_deduction_cap) {

        RUB yearly_portion = (RUB)(7'000'000 * 0.13 / 10);
        RUB remaining = Zeckster.job.property_deduction_cap -
            Zeckster.job.property_deduction_used;
        RUB refund = (remaining < yearly_portion) ? remaining : yearly_portion;

        total_refund += refund;
        Zeckster.job.property_deduction_used += refund;
    }

    if (Zeckster.home.mortgage_debt > 0 &&
        Zeckster.job.mortgage_deduction_used < Zeckster.job.mortgage_deduction_cap) {

        RUB yearly_interest = Zeckster.home.mortgage_debt *
            Zeckster.home.mortgage_rate_bp / 10'000;
        RUB year_refund = (RUB)(yearly_interest * 0.13);

        RUB remaining = Zeckster.job.mortgage_deduction_cap -
            Zeckster.job.mortgage_deduction_used;
        RUB refund = (remaining < year_refund) ? remaining : year_refund;

        total_refund += refund;
        Zeckster.job.mortgage_deduction_used += refund;
    }

    if (Zeckster.health.total_spent > 0) {
        RUB year_medical = 150'000;
        RUB refund = (RUB)(year_medical * 0.13);
        RUB remaining = Zeckster.job.social_deduction_cap -
            Zeckster.job.social_deduction_used_year;
        if (refund > remaining) refund = remaining;

        total_refund += refund;
        Zeckster.job.social_deduction_used_year += refund;
        Zeckster.job.social_deduction_used_total += refund;
    }

    if (total_refund > Zeckster.job.ndfl_paid_last_year) {
        total_refund = Zeckster.job.ndfl_paid_last_year;
    }

    Zeckster.money.cash += total_refund;
    Zeckster.money.total_income += total_refund;
}

void Zeckster_savings_step() {
    if (Zeckster.money.cash > Zeckster.money.cash_reserve) {
        Zeckster.money.excess = Zeckster.money.cash - Zeckster.money.cash_reserve;
        RUB deposit = Zeckster.money.excess * 15 / 100;
        Zeckster.money.cash -= deposit;
        Zeckster.money.savings += deposit;
    }
}

void Zeckster_deposit_step(const int year, const int month) {
    if (!Zeckster.deposit.active && Zeckster.money.savings >= 2'000'000) {
        Zeckster.money.savings -= 1'800'000;
        Zeckster.deposit.active = 1;
        Zeckster.deposit.principal = 1'800'000;
        Zeckster.deposit.rate_bp = 1500;
        Zeckster.deposit.months_left = 36;
        Zeckster.deposit.start_year = year;
        Zeckster.deposit.start_month = month;
        Zeckster.deposit.deposits_opened++;
    }

    if (Zeckster.deposit.active) {
        RUB monthly_interest = Zeckster.deposit.principal *
            Zeckster.deposit.rate_bp / 10'000 / 12;
        Zeckster.deposit.principal += monthly_interest;
        Zeckster.deposit.months_left--;

        if (Zeckster.deposit.months_left <= 0) {
            Zeckster.money.savings += Zeckster.deposit.principal;
            Zeckster.deposit.total_interest +=
                Zeckster.deposit.principal - 1'800'000;
            Zeckster.deposit.active = 0;
            Zeckster.deposit.principal = 0;
        }
    }
}

void Zeckster_move(const int year, const int month) {
    if (year == 2028 && month == 8) {
        Zeckster.home.rent_amount = 50'000;
        Zeckster.home.type = "Rented apartment";
        Zeckster.life.food_base = (RUB)(Zeckster.life.food_base * 1.3);
    }
}

void Zeckster_buy_flat_cash(const int year, const int month) {
    if (Zeckster.use_mortgage == 1) return;
    if (Zeckster.home.market_value > 0) return;

    RUB total_money = Zeckster.money.savings;
    if (Zeckster.deposit.active) {
        total_money += Zeckster.deposit.principal;
    }

    if (total_money >= 7'000'000) {
        RUB needed = 7'000'000;
        RUB from_savings = (needed < Zeckster.money.savings)
            ? needed : Zeckster.money.savings;
        Zeckster.money.savings -= from_savings;
        RUB rest = needed - from_savings;

        if (rest > 0 && Zeckster.deposit.active) {
            Zeckster.deposit.principal -= rest;
            if (Zeckster.deposit.principal == 0) {
                Zeckster.deposit.active = 0;
            }
        }

        Zeckster.home.market_value = 7'000'000;
        Zeckster.home.type = "Own apartment (bought cash)";
        Zeckster.home.rent_amount = 0;
        Zeckster.events.flat_bought_year = year;
        Zeckster.events.flat_bought_month = month;
    }
}

void Zeckster_choose_car_model() {
    if (Zeckster.money.savings >= 2'000'000) {
        Zeckster.car.model = "premium";
        Zeckster.car.fuel_cost_per_month = 25'000;
        Zeckster.car.maintenance_cost = 80'000;
        Zeckster.car.insurance_osago = 15'000;
        Zeckster.car.insurance_kasko = 80'000;
    }
    else if (Zeckster.money.savings >= 1'000'000) {
        Zeckster.car.model = "mid";
        Zeckster.car.fuel_cost_per_month = 15'000;
        Zeckster.car.maintenance_cost = 40'000;
        Zeckster.car.insurance_osago = 10'000;
        Zeckster.car.insurance_kasko = 40'000;
    }
    else {
        Zeckster.car.model = "economy";
        Zeckster.car.fuel_cost_per_month = 8'000;
        Zeckster.car.maintenance_cost = 20'000;
        Zeckster.car.insurance_osago = 7'000;
        Zeckster.car.insurance_kasko = 0;
    }
}

void Zeckster_car_breakdown_check() {
    int roll = roll_d100();

    if (roll <= 12) {
        RUB cost = 5'000 + roll_d100() * 150;
        Zeckster.car.breakdowns_minor++;
        Zeckster.car.total_car_spent += cost;
        Zeckster_pay(cost);
    }

    if (roll >= 99) {
        RUB cost = 50'000 + roll_d100() * 1'000;
        Zeckster.car.breakdowns_major++;

        if (Zeckster.car.has_kasko) {
            RUB covered = (RUB)(cost * 0.7);
            RUB to_pay = cost - covered;
            Zeckster.car.total_car_spent += to_pay;
            Zeckster_pay(to_pay);
        }
        else {
            Zeckster.car.total_car_spent += cost;
            Zeckster_pay(cost);
        }
    }
}

void Zeckster_car_insurance_step(const int month) {
    if (month == 4) {
        Zeckster_pay(Zeckster.car.insurance_osago);
        Zeckster.car.total_car_spent += Zeckster.car.insurance_osago;

        if (Zeckster.car.has_kasko) {
            Zeckster_pay(Zeckster.car.insurance_kasko);
            Zeckster.car.total_car_spent += Zeckster.car.insurance_kasko;
        }
    }
}

void Zeckster_car_tech_inspection_step() {
    Zeckster.car.tech_inspection_due--;
    if (Zeckster.car.tech_inspection_due <= 0) {
        RUB cost = 3'000;
        Zeckster_pay(cost);
        Zeckster.car.total_car_spent += cost;
        Zeckster.car.tech_inspection_due = 24;
    }
}

void Zeckster_car_sell_if_old() {
    if (Zeckster.car.owned == 1 && Zeckster.car.age_months > 120) {
        Zeckster.money.cash += Zeckster.car.value;

        Zeckster.car.owned = 0;
        Zeckster.car.value = 0;
        Zeckster.car.age_months = 0;
        Zeckster.car.months_since_purchase = 0;
        Zeckster.car.fuel_cost_per_month = 0;
        Zeckster.car.maintenance_cost = 0;
        Zeckster.car.insurance_osago = 0;
        Zeckster.car.insurance_kasko = 0;
        Zeckster.car.has_kasko = 0;
        Zeckster.car.model = "none";
        Zeckster.car.license_suspended_months = 0;
    }
}

void Zeckster_car_step(const int year, const int month) {
    Zeckster_car_sell_if_old();

    if (Zeckster.car.owned == 0 && month == 3 &&
        Zeckster.money.savings + Zeckster.money.cash > 600'000) {

        Zeckster_choose_car_model();

        RUB price;
        if (strcmp(Zeckster.car.model, "premium") == 0)      price = 4'000'000;
        else if (strcmp(Zeckster.car.model, "mid") == 0)     price = 1'500'000;
        else                                                  price = 600'000;

        if (Zeckster.car.fuel_type == 1) {
            price = (RUB)(price * 1.2);
            Zeckster.car.maintenance_cost = (RUB)(Zeckster.car.maintenance_cost * 0.5);
        }

        RUB from_savings = (price < Zeckster.money.savings) ? price : Zeckster.money.savings;
        RUB from_cash = price - from_savings;

        if (from_cash > Zeckster.money.cash) return;

        Zeckster.money.savings -= from_savings;
        Zeckster.money.cash -= from_cash;

        Zeckster.car.owned = 1;
        Zeckster.car.age_months = 0;
        Zeckster.car.months_since_purchase = 0;
        Zeckster.car.value = price;
        Zeckster.car.monthly_expenses = Zeckster.car.fuel_cost_per_month;
        Zeckster.car.tech_inspection_due = 24;
        Zeckster.car.has_kasko = (strcmp(Zeckster.car.model, "economy") != 0);
        Zeckster.car.total_car_spent += price;
    }

    if (Zeckster.car.owned == 1) {
        Zeckster.car.months_since_purchase++;

        if (Zeckster.car.license_suspended_months > 0) {
            Zeckster.car.license_suspended_months--;
            return;
        }

        Zeckster_pay(Zeckster.car.fuel_cost_per_month);
        Zeckster.car.total_car_spent += Zeckster.car.fuel_cost_per_month;

        Zeckster_car_tech_inspection_step();
        Zeckster_car_breakdown_check();
        Zeckster_car_insurance_step(month);

        if (month == 5) {
            Zeckster_pay(Zeckster.car.maintenance_cost);
            Zeckster.car.total_car_spent += Zeckster.car.maintenance_cost;
        }

        Zeckster.car.age_months++;
        int age_years = Zeckster.car.age_months / 12;
        double rate;
        if (age_years < 1)       rate = 0.981;
        else if (age_years < 5)  rate = 0.991;
        else if (age_years < 10) rate = 0.996;
        else                     rate = 0.998;

        Zeckster.car.value = (RUB)(Zeckster.car.value * rate);
        if (Zeckster.car.value < 50'000) {
            Zeckster.car.value = 50'000;
        }
    }
}

void Zeckster_take_mortgage(const int year, const int month) {
    if (Zeckster.use_mortgage == 0) return;
    if (year == 2030 && month == 1 && Zeckster.home.mortgage_debt == 0) {
        Zeckster.home.mortgage_debt = 7'000'000;
        Zeckster.home.market_value = 7'000'000;
        double monthly_rate = 0.09 / 12.0;
        int n = 120;
        double payment = Zeckster.home.mortgage_debt * monthly_rate /
            (1 - pow(1 + monthly_rate, -n));
        Zeckster.home.mortgage_payment = (RUB)payment;
        Zeckster.home.mortgage_months_left = n;
        Zeckster.home.type = "Own apartment (mortgage)";
        Zeckster.home.rent_amount = 0;
    }
}

void Zeckster_pay_mortgage() {
    if (Zeckster.home.mortgage_debt > 0) {
        RUB interest = Zeckster.home.mortgage_debt *
            Zeckster.home.mortgage_rate_bp / 10'000 / 12;
        RUB principal = Zeckster.home.mortgage_payment - interest;
        if (principal > Zeckster.home.mortgage_debt) {
            principal = Zeckster.home.mortgage_debt;
        }
        Zeckster.home.mortgage_debt -= principal;

        if (Zeckster.home.mortgage_months_left > 0) {
            Zeckster.home.mortgage_months_left--;
        }

        if (Zeckster.home.mortgage_debt == 0) {
            Zeckster.home.mortgage_months_left = 0;
            Zeckster.home.rent_amount = 0;
            Zeckster.home.type = "Own apartment (paid off)";
        }
    }
}

void Zeckster_pvz_step(const int year, const int month) {
    if (Zeckster.biz.pvz_open == 0 &&
        Zeckster.money.savings >= 300'000 &&
        Zeckster.money.cash >= Zeckster.money.cash_reserve) {

        Zeckster.money.savings -= 300'000;
        Zeckster.biz.pvz_open = 1;
        Zeckster.biz.pvz_invested = 300'000;
        Zeckster.biz.pvz_income = 90'000;
        Zeckster.biz.pvz_costs = 45'000;
        Zeckster.biz.pvz_months = 0;
        Zeckster.biz.pvz_open_year = year;
        Zeckster.biz.pvz_open_month = month;
    }

    if (Zeckster.biz.pvz_open == 1) {
        Zeckster.biz.pvz_months++;
        int roll = roll_d100();
        RUB extra_cost;
        if (roll <= 10) {
            extra_cost = 30'000;
        }
        else {
            extra_cost = 0;
        }

        RUB total_pvz_income = Zeckster.biz.pvz_income;
        RUB total_pvz_cost = Zeckster.biz.pvz_costs + extra_cost;

        if (total_pvz_cost > total_pvz_income) {
            RUB loss = total_pvz_cost - total_pvz_income;
            Zeckster_pay(loss);
            Zeckster.biz.total_pvz_net -= loss;
        }
        else {
            RUB net = total_pvz_income - total_pvz_cost;
            Zeckster.money.cash += net;
            Zeckster.biz.total_pvz_net += net;
        }

        Zeckster.money.total_income += Zeckster.biz.pvz_income;
    }
}

void simulation(int years) {
    int year = 2026, month = 9;
    int end_year = 2026 + years, end_month = 9;

    while (not (year == end_year && month == end_month)) {
        Zeckster_salary(year, month);
        Zeckster_ndfl(year, month);
        Zeckster_bonus_step(year, month);
        Zeckster_ndfl(year, month);
        Zeckster_food_quality_step(year, month);
        Zeckster_groceries_step(year, month);
        Zeckster_move(year, month);
        Zeckster_health_step(month);
        Zeckster_take_mortgage(year, month);
        Zeckster_pay_mortgage();
        Zeckster_repair_step(year, month);
        Zeckster_car_step(year, month);
        Zeckster_cat_mechanics(year, month);
        Zeckster_car_crash_event(year, month);
        Zeckster_tax_audit_event(year, month);
        Zeckster_random_events(year, month);

        Zeckster_food_step(year, month);
        Zeckster_utility_step(year, month);
        Zeckster_housing_step(year, month);

        Zeckster_savings_interest();
        Zeckster_savings_step();
        Zeckster_deposit_step(year, month);
        Zeckster_tax_refund(year, month);
        Zeckster_pvz_step(year, month);
        Zeckster_buy_flat_cash(year, month);
        Zeckster_inflation(year, month);

        Zeckster_check_reserve();

        Zeckster.months_lived++;

        if (Zeckster.money.cash > 1'000'000'000'000'000ULL ||
            Zeckster.money.savings > 1'000'000'000'000'000ULL) {
            printf("[FATAL] Overflow detected at %d/%d. Simulation stopped.\n",
                year, month);
            return;
        }

        ++month;
        if (month == 13) {
            ++year; month = 1;
        }
    }
}

RUB Zeckster_net_worth() {
    RUB flat_equity = 0;
    if (Zeckster.home.market_value > Zeckster.home.mortgage_debt) {
        flat_equity = Zeckster.home.market_value - Zeckster.home.mortgage_debt;
    }

    RUB deposit_value = Zeckster.deposit.active ? Zeckster.deposit.principal : 0;

    return Zeckster.money.cash
        + Zeckster.money.savings
        + deposit_value
        + Zeckster.car.value
        + flat_equity;
}

void Zeckster_print() {
    printf("=== Zeckster after %d months (%.1f years) ===\n",
        Zeckster.months_lived, Zeckster.months_lived / 12.0);

    printf("\n[Finances]\n");
    printf("  Cash:            %llu\n", Zeckster.money.cash);
    printf("  Savings:         %llu\n", Zeckster.money.savings);
    printf("  Cash reserve:    %llu\n", Zeckster.money.cash_reserve);
    printf("  Total income:    %llu\n", Zeckster.money.total_income);
    printf("  Total expenses:  %llu\n", Zeckster.money.total_expenses);
    printf("  Bankrupt months: %d\n", Zeckster.money.bankrupt_count);
    printf("  Emergency refills: %d\n", Zeckster.money.emergency_refills);

    printf("\n[Deposit]\n");
    printf("  Active:          %d", Zeckster.deposit.active);
    if (Zeckster.deposit.active) {
        printf(" (since %d/%d, %d months left)",
            Zeckster.deposit.start_year,
            Zeckster.deposit.start_month,
            Zeckster.deposit.months_left);
    }
    printf("\n");
    printf("  Principal:       %llu\n", Zeckster.deposit.principal);
    printf("  Rate:            %llu bp (%llu%%)\n",
        Zeckster.deposit.rate_bp, Zeckster.deposit.rate_bp / 100);
    printf("  Deposits opened: %d\n", Zeckster.deposit.deposits_opened);
    printf("  Total interest:  %llu\n", Zeckster.deposit.total_interest);

    printf("\n[Job]\n");
    printf("  Salary:          %llu (%s)\n",
        Zeckster.job.salary, Zeckster.job.title);
    printf("  Salary2:         %llu (%s)\n",
        Zeckster.job.salary2, Zeckster.job.title2);
    printf("  Bonus rate:      %llu bp (from monthly)\n",
        Zeckster.job.bonus_rate_bp);
    printf("  Last bonus:      %llu\n", Zeckster.job.bonus);

    printf("\n[Tax]\n");
    printf("  NDFL total:      %llu\n", Zeckster.job.ndfl_paid_total);
    printf("  Property ded.:   %llu / %llu\n",
        Zeckster.job.property_deduction_used,
        Zeckster.job.property_deduction_cap);
    printf("  Mortgage ded.:   %llu / %llu\n",
        Zeckster.job.mortgage_deduction_used,
        Zeckster.job.mortgage_deduction_cap);
    printf("  Social ded.:     %llu total, %llu this year (cap %llu)\n",
        Zeckster.job.social_deduction_used_total,
        Zeckster.job.social_deduction_used_year,
        Zeckster.job.social_deduction_cap);

    printf("\n[Housing]\n");
    printf("  Type:            %s\n", Zeckster.home.type);
    if (Zeckster.home.market_value == 0) {
        printf("  Rent:            %llu\n", Zeckster.home.rent_amount);
    }
    else {
        printf("  Rent:            (own flat)\n");
    }
    printf("  Utility:         %llu\n", Zeckster.home.utility_base);
    printf("  Market value:    %llu\n", Zeckster.home.market_value);
    printf("  Mortgage debt:   %llu (months left %d)\n",
        Zeckster.home.mortgage_debt, Zeckster.home.mortgage_months_left);
    printf("  Repairs:         %d times, %llu total\n",
        Zeckster.home.repairs_count, Zeckster.home.total_repair_paid);
    printf("  Rent paid:       %llu\n", Zeckster.home.total_rent_paid);
    printf("  Mortgage paid:   %llu\n", Zeckster.home.total_mortgage_paid);
    printf("  Utility paid:    %llu\n", Zeckster.home.total_utility_paid);

    printf("\n[Living]\n");
    printf("  Food base:       %llu\n", Zeckster.life.food_base);
    printf("  Dining out:      %llu\n", Zeckster.life.dining_out);
    printf("  Healthy extra:   %llu\n", Zeckster.life.healthy_food_extra);
    printf("  Food quality:    %d (0=eco, 1=normal, 2=healthy)\n",
        Zeckster.life.food_quality);
    printf("  Food total:      %llu\n", Zeckster.life.total_food_paid);

    printf("\n[Transport]\n");
    printf("  Owned:           %d\n", Zeckster.car.owned);
    printf("  Model:           %s\n", Zeckster.car.model);
    printf("  Value:           %llu\n", Zeckster.car.value);
    printf("  Age:             %d months\n", Zeckster.car.age_months);
    printf("  Fuel monthly:    %llu\n", Zeckster.car.fuel_cost_per_month);
    printf("  Breakdowns:      minor %d, major %d\n",
        Zeckster.car.breakdowns_minor, Zeckster.car.breakdowns_major);
    printf("  Total car spent: %llu\n", Zeckster.car.total_car_spent);

    printf("\n[Pet]\n");
    printf("  Name:            %s\n", Zeckster.cat.name);
    if (Zeckster.cat.alive) {
        printf("  Alive:           yes (age %d months)\n", Zeckster.cat.age_months);
    }
    else if (Zeckster.cat.death_year > 0) {
        printf("  Alive:           no (died at %d months = %.1f years)\n",
            Zeckster.cat.lifespan_months, Zeckster.cat.lifespan_months / 12.0);
    }
    else {
        printf("  Alive:           never born\n");
    }
    printf("  Pet total:       %llu\n", Zeckster.cat.total_pet_paid);
    printf("  Illnesses:       %d\n", Zeckster.cat.illness_count);
    printf("  Vaccines:        %d\n", Zeckster.cat.vaccine_count);
    printf("  Sterilized:      %s\n", Zeckster.cat.sterilized ? "yes" : "no");

    printf("\n[Business]\n");
    printf("  PVZ open:        %d", Zeckster.biz.pvz_open);
    if (Zeckster.biz.pvz_open) {
        printf(" (since %d/%d, %d months)",
            Zeckster.biz.pvz_open_year,
            Zeckster.biz.pvz_open_month,
            Zeckster.biz.pvz_months);
    }
    printf("\n");
    printf("  PVZ invested:    %llu\n", Zeckster.biz.pvz_invested);
    printf("  PVZ net total:   %llu\n", Zeckster.biz.total_pvz_net);

    printf("\n[Health]\n");
    printf("  Total spent:     %llu\n", Zeckster.health.total_spent);
    printf("  Months sick:     %d (serious %d)\n",
        Zeckster.health.months_sick, Zeckster.health.months_seriously_sick);

    printf("\n[Events]\n");
    printf("  Fired:           %d\n", Zeckster.events.fired);
    printf("  Inheritance:     %d\n", Zeckster.events.inheritance);
    printf("  Car crash:       %d (severe %d)\n",
        Zeckster.events.car_crash, Zeckster.events.car_crash_severe);
    printf("  Promotion:       %d\n", Zeckster.events.promotion);
    printf("  Food jump:       %d\n", Zeckster.events.food_jump);
    printf("  Tax audit:       %d\n", Zeckster.events.tax_audit);
    if (Zeckster.biz.pvz_open) {
        printf("  PVZ opened:      %d/%d\n",
            Zeckster.biz.pvz_open_year, Zeckster.biz.pvz_open_month);
    }
    else {
        printf("  PVZ opened:      never\n");
    }
    if (Zeckster.use_mortgage == 0) {
        if (Zeckster.events.flat_bought_year > 0) {
            printf("  Flat bought:     %d/%d (cash)\n",
                Zeckster.events.flat_bought_year,
                Zeckster.events.flat_bought_month);
        }
        else {
            printf("  Flat bought:     never\n");
        }
    }
    else {
        printf("  Flat:            mortgage since 2030/1\n");
    }

    printf("\n[Net worth]:       %llu\n", Zeckster_net_worth());
}

int evaluate_life_quality() {
    RUB nw = Zeckster_net_worth();
    if (nw > 10'000'000 && Zeckster.biz.pvz_open) return 3;
    if (nw > 4'000'000) return 2;
    return 1;
}

void compare_mortgage_vs_saving() {
    printf("\n=== Mortgage vs Saving (same seed) ===\n");

    const int SEED = 42;
    const int YEARS = 15;
    RUB result[2];
    RUB cash_savings[2];
    RUB flat_equity_arr[2];
    int bankrupt[2];

    for (int strategy = 0; strategy < 2; strategy++) {
        Zeckster_init(strategy);
        srand(SEED);
        simulation(YEARS);

        result[strategy] = Zeckster_net_worth();
        cash_savings[strategy] = Zeckster.money.cash + Zeckster.money.savings;
        flat_equity_arr[strategy] = Zeckster.home.market_value > Zeckster.home.mortgage_debt
            ? Zeckster.home.market_value - Zeckster.home.mortgage_debt
            : 0;
        bankrupt[strategy] = Zeckster.money.bankrupt_count;
    }

    printf("Strategy A (mortgage):\n");
    printf("  Net worth:     %llu\n", result[1]);
    printf("  Cash+savings:  %llu\n", cash_savings[1]);
    printf("  Flat equity:   %llu\n", flat_equity_arr[1]);
    printf("  Bankrupt:      %d months\n", bankrupt[1]);

    printf("Strategy B (saving):\n");
    printf("  Net worth:     %llu\n", result[0]);
    printf("  Cash+savings:  %llu\n", cash_savings[0]);
    printf("  Flat equity:   %llu\n", flat_equity_arr[0]);
    printf("  Bankrupt:      %d months\n", bankrupt[0]);

    if (result[1] > result[0]) {
        printf("Conclusion: MORTGAGE wins by %llu\n", result[1] - result[0]);
    }
    else if (result[0] > result[1]) {
        printf("Conclusion: SAVING wins by %llu\n", result[0] - result[1]);
    }
    else {
        printf("Conclusion: TIE\n");
    }
}

void ab_test() {
    printf("\n=== A/B test: 10 pairs, different seeds ===\n");
    const int N = 10;
    RUB total_saving = 0, total_mortgage = 0;
    int wins_saving = 0, wins_mortgage = 0;

    for (int seed = 0; seed < N; seed++) {
        RUB r[2];
        for (int strategy = 0; strategy < 2; strategy++) {
            Zeckster_init(strategy);
            srand(seed);
            simulation(15);
            r[strategy] = Zeckster_net_worth();
        }

        printf("Seed %2d: saving = %llu, mortgage = %llu\n",
            seed, r[0], r[1]);

        total_saving += r[0];
        total_mortgage += r[1];

        if (r[0] > r[1]) wins_saving++;
        else if (r[1] > r[0]) wins_mortgage++;
    }

    printf("\nAverage saving:   %llu\n", total_saving / N);
    printf("Average mortgage: %llu\n", total_mortgage / N);
    printf("Wins: saving %d, mortgage %d\n", wins_saving, wins_mortgage);

    if (total_mortgage > total_saving)
        printf("Conclusion: MORTGAGE wins on average\n");
    else
        printf("Conclusion: SAVING wins on average\n");
}

int main() {
    srand((unsigned int)time(NULL));
    Zeckster_init(1);
    simulation(15);
    Zeckster_print();

    printf("\nLife quality: %d (1=poor, 2=average, 3=good)\n",
        evaluate_life_quality());

    compare_mortgage_vs_saving();
    ab_test();
    return 0;
}