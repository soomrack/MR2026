#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long long int RUB;

struct Finances {   // Финансы: деньги, накопления, резерв
    RUB cash;            // наличные / на карте
    RUB savings;         // накопления под % (вклад)
    RUB cash_reserve;    // неприкосновенный резерв
    RUB excess;          // излишек сверх резерва
    RUB total_income;    // статистика за всю жизнь
    RUB total_expenses;
};

struct Job {  // Работа: зарплаты и должности
    RUB salary;              // основная зарплата
    RUB salary2;             // вторая работа
    char const* title;       // должность
    char const* title2;      // вторая должность
    RUB tax_refund_pending;  // накопленный налоговый вычет
};

struct Housing {  // Жильё: аренда / ипотека / собственное
    char const* type;  // "Dormitory", "Rented", "Mortgage", "Owned"
    RUB rent_amount;  // текущая аренда
    RUB utility_base;  // коммуналка
    RUB market_value;  // рыночная стоимость квартиры

    // Ипотека
    RUB mortgage_debt;  // остаток долга
    RUB mortgage_payment;  // месячный платёж
    RUB mortgage_rate_bp;  // ставка в базисных пунктах (900 = 9%)
    int mortgage_months_left;
};

// Еда и базовые траты
struct Living {
    RUB food_base;  // базовая еда
};

// Транспорт
struct Transport {
    int owned;
    RUB value;
    RUB monthly_expenses;  // бензин + ТО
};

// Питомец
struct Pet {
    int alive;
    int age_months;
    RUB monthly_expenses;
    char const* name;
};

// Бизнес: ПВЗ
struct Business {
    int pvz_open;
    int pvz_months;
    RUB pvz_income;
    RUB pvz_costs;
    RUB pvz_invested;  // сколько вложено в открытие
};

// Здоровье
struct Health {
    RUB total_spent;  // сколько потрачено на лечение
    int months_sick;
    int months_seriously_sick;
};

// Агрегатор: сам Зекстер
struct Person {
    struct Finances  money;
    struct Job       job;
    struct Housing   home;
    struct Living    life;
    struct Transport car;
    struct Pet       cat;
    struct Business  biz;
    struct Health    health;

    int use_mortgage;   // 1 = берёт ипотеку, 0 = копит
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
    // Финансы
    Zeckster.money.cash = 20'000;
    Zeckster.money.savings = 0;
    Zeckster.money.cash_reserve = 20'000;
    Zeckster.money.excess = 0;
    Zeckster.money.total_income = 0;
    Zeckster.money.total_expenses = 0;

    // Работа
    Zeckster.job.salary = 30'000;
    Zeckster.job.salary2 = 0;
    Zeckster.job.title = "Student";
    Zeckster.job.title2 = "None";
    Zeckster.job.tax_refund_pending = 0;

    // Жильё
    Zeckster.home.type = "Dormitory";
    Zeckster.home.rent_amount = 2'500;
    Zeckster.home.utility_base = 3'000;
    Zeckster.home.market_value = 0;
    Zeckster.home.mortgage_debt = 0;
    Zeckster.home.mortgage_payment = 0;
    Zeckster.home.mortgage_rate_bp = 900;
    Zeckster.home.mortgage_months_left = 0;

    // Еда
    Zeckster.life.food_base = 12'000;

    // Транспорт
    Zeckster.car.owned = 0;
    Zeckster.car.value = 0;
    Zeckster.car.monthly_expenses = 0;

    // Питомец
    Zeckster.cat.alive = 0;
    Zeckster.cat.age_months = 0;
    Zeckster.cat.monthly_expenses = 4'000;
    Zeckster.cat.name = "Barsik";

    // Бизнес
    Zeckster.biz.pvz_open = 0;
    Zeckster.biz.pvz_months = 0;
    Zeckster.biz.pvz_income = 0;
    Zeckster.biz.pvz_costs = 0;
    Zeckster.biz.pvz_invested = 0;

    // Здоровье
    Zeckster.health.total_spent = 0;
    Zeckster.health.months_sick = 0;
    Zeckster.health.months_seriously_sick = 0;

    // Стратегия
    Zeckster.use_mortgage = use_mortgage;
    Zeckster.months_lived = 0;
}

//  инфляция
void Zeckster_inflation(const int year, const int month) {
    if (month == 1) {
        Zeckster.home.rent_amount = (RUB)(Zeckster.home.rent_amount * 1.08);
        Zeckster.home.utility_base = (RUB)(Zeckster.home.utility_base * 1.07);
        Zeckster.life.food_base = (RUB)(Zeckster.life.food_base * 1.09);
        Zeckster.cat.monthly_expenses = (RUB)(Zeckster.cat.monthly_expenses * 1.10);
        Zeckster.car.monthly_expenses = (RUB)(Zeckster.car.monthly_expenses * 1.06);
    }
}

// ЗДОРОВЬЕ
void Zeckster_health_step(int month) {
    int roll = roll_d100();
    RUB cost = 0;
    int serious = 0;

    if (month == 12 || month == 1 || month == 2) {
        if (roll <= 18)      cost = 5'000;
        else if (roll <= 22) { cost = 15'000; serious = 1; }
    }
    else if (month >= 3 && month <= 5) {
        if (roll <= 20)      cost = 5'000;
        else if (roll <= 25) { cost = 15'000; serious = 1; }
    }
    else if (month >= 6 && month <= 8) {
        if (roll <= 6)       cost = 5'000;
        else if (roll <= 8) { cost = 15'000; serious = 1; }
    }
    else {
        if (roll <= 23)      cost = 5'000;
        else if (roll <= 29) { cost = 15'000; serious = 1; }
    }

    if (cost > 0) {
        Zeckster.money.cash -= cost;
        Zeckster.money.total_expenses += cost;
        Zeckster.health.total_spent += cost;
        Zeckster.health.months_sick++;
        if (serious) Zeckster.health.months_seriously_sick++;
    }
}

// ДОХОДЫ
void Zeckster_salary(const int year, const int month) {
    if (year == 2027 && month == 8) {
        Zeckster.job.salary = 65'000;
        Zeckster.job.title = "Beginning engineer";
    }
    if (year == 2028 && month == 9) {
        Zeckster.job.salary = 80'000;
        Zeckster.job.title = "Engineer";
    }
    if (year == 2029 && month == 11) {
        Zeckster.job.salary = 135'000;
        Zeckster.job.title = "Lead engineer";
    }
    Zeckster.money.cash += Zeckster.job.salary;

    if (year >= 2029) {
        if (Zeckster.job.salary2 == 0) {
            Zeckster.job.salary2 = 25'000;
            Zeckster.job.title2 = "Freelance";
        }
        Zeckster.money.cash += Zeckster.job.salary2;
    }
    Zeckster.money.total_income += Zeckster.job.salary + Zeckster.job.salary2;
}

void Zeckster_savings_interest() {
    RUB interest = (RUB)(Zeckster.money.savings * 0.08 / 12.0);
    Zeckster.money.savings += interest;
    Zeckster.money.total_income += interest;
}

void Zeckster_tax_refund(const int year, const int month) {
    if (month == 4 && Zeckster.job.tax_refund_pending > 0) {
        Zeckster.money.cash += Zeckster.job.tax_refund_pending;
        Zeckster.money.total_income += Zeckster.job.tax_refund_pending;
        Zeckster.job.tax_refund_pending = 0;
    }
    if (Zeckster.home.mortgage_debt > 0) {
        RUB yearly_interest = Zeckster.home.mortgage_debt *
            Zeckster.home.mortgage_rate_bp / 10'000;
        Zeckster.job.tax_refund_pending += (RUB)(yearly_interest * 0.13 / 12.0);
    }
}

// РАСХОДЫ
void Zeckster_expenses() {
    RUB total = 0;
    total += Zeckster.life.food_base;
    total += Zeckster.home.utility_base;

    // Аренда платится только если нет своей квартиры
    if (Zeckster.home.market_value == 0) {
        total += Zeckster.home.rent_amount;
    }
    // Если квартира в ипотеке — платим ипотеку
    if (Zeckster.home.mortgage_debt > 0) {
        total += Zeckster.home.mortgage_payment;
    }
    if (Zeckster.cat.alive == 1) {
        total += Zeckster.cat.monthly_expenses;
    }
    if (Zeckster.car.owned == 1) {
        total += Zeckster.car.monthly_expenses;
    }
    if (Zeckster.biz.pvz_open) {
        total += Zeckster.biz.pvz_costs;
    }
    Zeckster.money.cash -= total;
    Zeckster.money.total_expenses += total;
}

// НАКОПЛЕНИЯ
void Zeckster_savings_step() {
    if (Zeckster.money.cash > Zeckster.money.cash_reserve) {
        Zeckster.money.excess = Zeckster.money.cash - Zeckster.money.cash_reserve;
        RUB deposit = Zeckster.money.excess / 2;
        Zeckster.money.cash -= deposit;
        Zeckster.money.savings += deposit;
    }
}

// ПЕРЕЕЗД
void Zeckster_move(const int year, const int month) {
    if (year == 2028 && month == 8) {
        Zeckster.home.rent_amount = 50'000;
        Zeckster.home.type = "Rented apartment";
        Zeckster.life.food_base = (RUB)(Zeckster.life.food_base * 1.3);
    }
}

// МАШИНА
void Zeckster_car_step(const int year, const int month) {
    if (Zeckster.car.owned == 0 && month == 3 &&
        Zeckster.money.savings + Zeckster.money.cash > 600'000) {
        RUB price = 600'000;
        RUB from_savings;
        if (price < Zeckster.money.savings)
        {
            from_savings = price;
        }
        else
        {
            from_savings = Zeckster.money.savings;
        }
        Zeckster.money.savings -= from_savings;
        Zeckster.money.cash -= (price - from_savings);
        Zeckster.car.owned = 1;
        Zeckster.car.value = price;
        Zeckster.car.monthly_expenses = 15'000;
    }
    if (Zeckster.car.owned == 1) {
        Zeckster.car.value = (RUB)(Zeckster.car.value * 0.99);
    }
}

//КОТ
void Zeckster_cat_step(const int year, const int month) {
    if (Zeckster.cat.alive == 0 && month == 6) {
        Zeckster.cat.alive = 1;
        Zeckster.cat.age_months = 0;
    }
    if (Zeckster.cat.alive == 1) {
        Zeckster.cat.age_months++;
        if (Zeckster.cat.age_months > 180) {  // 15 лет
            Zeckster.cat.alive = 0;
        }
    }
}

// ИПОТЕКА
void Zeckster_take_mortgage(const int year, const int month) {
    if (Zeckster.use_mortgage == 0) return;
    if (year == 2029 && month == 1 && Zeckster.home.mortgage_debt == 0) {
        Zeckster.home.mortgage_debt = 5'000'000;
        Zeckster.home.market_value = 5'000'000;
        double monthly_rate = 0.09 / 12.0;
        int n = 120; // 10 лет
        double payment = Zeckster.home.mortgage_debt * monthly_rate /
            (1 - pow(1 + monthly_rate, -n));
        Zeckster.home.mortgage_payment = (RUB)payment;
        Zeckster.home.mortgage_months_left = n;
        Zeckster.home.type = "Own apartment (mortgage)";
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
        Zeckster.home.mortgage_months_left--;
        if (Zeckster.home.mortgage_debt == 0) {
            Zeckster.home.type = "Own apartment (paid off)";
        }
    }
}

// ПВЗ
void Zeckster_pvz_step(const int year, const int month) {
    if (Zeckster.biz.pvz_open == 0 && year == 2030 && month == 3 &&
        Zeckster.money.savings > 300'000) {
        Zeckster.money.savings -= 300'000;
        Zeckster.biz.pvz_open = 1;
        Zeckster.biz.pvz_invested = 300'000;
        Zeckster.biz.pvz_income = 90'000;
        Zeckster.biz.pvz_costs = 45'000;
        Zeckster.biz.pvz_months = 0;
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
        RUB net = Zeckster.biz.pvz_income - Zeckster.biz.pvz_costs - extra_cost; // штраф/поломка
        Zeckster.money.cash += net;
        Zeckster.money.total_income += Zeckster.biz.pvz_income;
        Zeckster.money.total_expenses += Zeckster.biz.pvz_costs + extra_cost;
    }
}

void simulation(int years) {
    int year = 2026, month = 9;
    int end_year = 2026 + years, end_month = 9;

    while (not (year == end_year && month == end_month)) {
        Zeckster_salary(year, month);
        Zeckster_move(year, month);
        Zeckster_health_step(month);
        Zeckster_take_mortgage(year, month);
        Zeckster_pay_mortgage();
        Zeckster_car_step(year, month);
        Zeckster_cat_step(year, month);
        Zeckster_expenses();
        Zeckster_savings_interest();
        Zeckster_savings_step();
        Zeckster_tax_refund(year, month);
        Zeckster_pvz_step(year, month);
        Zeckster_inflation(year, month);

        Zeckster.months_lived++;

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

    return Zeckster.money.cash
        + Zeckster.money.savings
        + Zeckster.car.value
        + flat_equity;
}

// ВЫВОД
void Zeckster_print() {
    printf("=== Zeckster after %d months (%.1f years) ===\n",
        Zeckster.months_lived, Zeckster.months_lived / 12.0);

    printf("\n[Finances]\n");
    printf("  Cash:            %llu\n", Zeckster.money.cash);
    printf("  Savings:         %llu\n", Zeckster.money.savings);
    printf("  Total income:    %llu\n", Zeckster.money.total_income);
    printf("  Total expenses:  %llu\n", Zeckster.money.total_expenses);

    printf("\n[Job]\n");
    printf("  Salary:          %llu (%s)\n",
        Zeckster.job.salary, Zeckster.job.title);
    printf("  Salary2:         %llu (%s)\n",
        Zeckster.job.salary2, Zeckster.job.title2);

    printf("\n[Housing]\n");
    printf("  Type:            %s\n", Zeckster.home.type);
    printf("  Rent:            %llu\n", Zeckster.home.rent_amount);
    printf("  Utility:         %llu\n", Zeckster.home.utility_base);
    printf("  Market value:    %llu\n", Zeckster.home.market_value);
    printf("  Mortgage debt:   %llu (months left %d)\n",
        Zeckster.home.mortgage_debt, Zeckster.home.mortgage_months_left);

    printf("\n[Living]\n");
    printf("  Food base:       %llu\n", Zeckster.life.food_base);

    printf("\n[Transport]\n");
    printf("  Owned:           %d (value %llu)\n",
        Zeckster.car.owned, Zeckster.car.value);
    printf("  Monthly exp:     %llu\n", Zeckster.car.monthly_expenses);

    printf("\n[Pet]\n");
    printf("  Name:            %s\n", Zeckster.cat.name);
    printf("  Alive:           %d (age %d months)\n",
        Zeckster.cat.alive, Zeckster.cat.age_months);

    printf("\n[Business]\n");
    printf("  PVZ open:        %d (months %d)\n",
        Zeckster.biz.pvz_open, Zeckster.biz.pvz_months);
    printf("  PVZ invested:    %llu\n", Zeckster.biz.pvz_invested);

    printf("\n[Health]\n");
    printf("  Total spent:     %llu\n", Zeckster.health.total_spent);
    printf("  Months sick:     %d (serious %d)\n",
        Zeckster.health.months_sick, Zeckster.health.months_seriously_sick);

    printf("\n[Net worth]:       %llu\n", Zeckster_net_worth());
}


int evaluate_life_quality() {
    RUB nw = Zeckster_net_worth();
    if (nw > 7'000'000 && Zeckster.biz.pvz_open) return 3;
    if (nw > 3'000'000) return 2;
    return 1;
}

void compare_mortgage_vs_saving() {
    printf("\n=== Mortgage vs Saving (same seed) ===\n");

    const int SEED = 42;
    const int YEARS = 15;
    RUB result[2];
    RUB cash_savings[2];
    RUB flat_equity_arr[2];

    for (int strategy = 0; strategy < 2; strategy++) {
        Zeckster_init(strategy);
        srand(SEED);
        simulation(YEARS);

        result[strategy] = Zeckster_net_worth();
        cash_savings[strategy] = Zeckster.money.cash + Zeckster.money.savings;
        flat_equity_arr[strategy] = Zeckster.home.market_value > Zeckster.home.mortgage_debt
            ? Zeckster.home.market_value - Zeckster.home.mortgage_debt
            : 0;
    }

    printf("Strategy A (mortgage):\n");
    printf("  Net worth:     %llu\n", result[1]);
    printf("  Cash+savings:  %llu\n", cash_savings[1]);
    printf("  Flat equity:   %llu\n", flat_equity_arr[1]);

    printf("Strategy B (saving):\n");
    printf("  Net worth:     %llu\n", result[0]);
    printf("  Cash+savings:  %llu\n", cash_savings[0]);
    printf("  Flat equity:   %llu\n", flat_equity_arr[0]);

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
    printf("\n=== A/B test ===\n");
    RUB results[2];
    for (int i = 0; i < 2; i++) {
        Zeckster_init(1);
        srand(time(NULL) + i * 1'000);
        simulation(15);
        results[i] = Zeckster_net_worth();
        printf("Model %c: net worth = %llu\n", 'A' + i, results[i]);
    }
    if (results[0] > results[1])
        printf("Model A is better by %llu\n", results[0] - results[1]);
    else
        printf("Model B is better by %llu\n", results[1] - results[0]);
}


int main() {
    srand(time(NULL));
    Zeckster_init(1);
    simulation(15);
    Zeckster_print();

    printf("\nLife quality: %d (1=poor, 2=average, 3=good)\n",
        evaluate_life_quality());

    compare_mortgage_vs_saving();
    ab_test();
    return 0;
}