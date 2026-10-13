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
    RUB excess;                // излишек сверх резерва
    RUB total_income;
    RUB total_expenses;
    int bankrupt_count;
    int emergency_refills;
};

struct Deposit {
    int active;                // вклад
    RUB principal;             // текущая сумма вклада (тело + проценты)
    RUB rate_bp;               // ставка в bp: 1500 = 15%
    int months_left;
    int start_year;
    int start_month;
    RUB total_interest;
    int deposits_opened;       // сколько раз открывали вклад
    RUB max_principal_seen;
};

struct Investment { // ИИС-3
    int iis_open;
    int iis_closed;
    int iis_months_held;
    int iis_open_year;
    int iis_open_month;
    RUB iis_contributed_total; // всего внесено на ИИС
    RUB iis_contributed_this_year;
    RUB iis_value;
    RUB iis_contribution_base; // база взносов (для вычета)

    // Портфель
    RUB stocks_value;          // стоимость акций
    RUB bonds_value;           // стоимость облигаций
    RUB cash_on_iis;           // свободные деньги на ИИС

    // Доходы
    RUB dividends_this_year;   // дивиденды за год
    RUB dividends_total;
    RUB coupons_this_year;
    RUB coupons_total;
    RUB realized_profit_total; // реализованная прибыль

    RUB dividend_tax_paid_total; // налог на дивиденды (13%)
    RUB iis_tax_refund_total;    // возврат по типу А
    RUB iis_tax_saved;           // экономия по типу Б при закрытии

    // Риски
    int crashes_experienced;   // сколько раз были обвалы
    RUB total_crash_losses;    // суммарные потери от обвалов
    int crisis_years;
    RUB best_month_gain;
    RUB worst_month_loss;

    RUB yearly_contribution[20]; // взносы по годам
    RUB yearly_value[20];        // стоимость портфеля по годам
    int years_recorded;

    // Параметры
    RUB stocks_allocation_bp;    // доля акций: 7000 = 70%
    RUB bonds_allocation_bp;     // доля облигаций: 3000 = 30%
    RUB stock_expected_return_bp; // ожидаемая доходность акций
    RUB bond_expected_return_bp;  // облигаций

    // Механики
    RUB total_dividend_events;   // сколько раз получали дивиденды
    RUB total_coupon_events;     // купоны
    int rebalance_count;         // сколько раз ребалансировали
    RUB total_rebalanced_amount; // сколько денег перекладывали
    int stop_loss_triggers;      // сколько раз срабатывал stop-loss
    RUB total_stop_loss_saved;   // сколько "спасли" через stop-loss
    RUB max_portfolio_value;
    RUB min_portfolio_value;
    int max_drawdown_bp;         // максимальная просадка в bp
    int dividend_reinvest_count; // сколько раз реинвестировали дивиденды
    RUB dividend_reinvest_total; // суммарно реинвестировано
    int deposit_sweep_count;     // сколько раз перекачивали с вклада на ИИС
    RUB deposit_sweep_total;     // суммарно перекачано

    RUB monthly_returns[240];    // история доходностей по месяцам
    int monthly_returns_count;   // сколько месяцев записали

    int bull_months;             // месяцев роста
    int bear_months;             // месяцев падения
    int flat_months;             // месяцев флэта
};

struct Job {
    RUB salary;
    RUB salary2;               // (фриланс)
    RUB bonus;
    RUB bonus_rate_bp;         // ставка премии в bp: 10000 = 100% от месячной
    char const* title;         // должность
    char const* title2;        // вторая 
    RUB ndfl_paid_this_year;   // НДФЛ в этом году
    RUB ndfl_paid_last_year;   // в прошлом году (для вычетов)
    RUB ndfl_paid_total;       // за всю жизнь
    RUB property_deduction_used; // использовано имущественного вычета
    RUB property_deduction_cap;  // лимит 260 000
    RUB mortgage_deduction_used; // использовано вычета по ипотеке
    RUB mortgage_deduction_cap;  // лимит 390 000
    RUB social_deduction_used_total; // социальный вычет за всю жизнь
    RUB social_deduction_used_year;  // социальный вычет за год
    RUB social_deduction_cap;        // лимит 19 500 в год
};

struct Housing {
    char const* type;          // "Dormitory", "Rented", "Mortgage", "Owned"
    RUB rent_amount;
    RUB utility_base;
    RUB market_value;
    RUB mortgage_debt;         // остаток долга
    RUB mortgage_payment;      // месячный платёж
    RUB mortgage_rate_bp;      // ставка: 900 = 9%
    int mortgage_months_left;
    int mortgage_taken;
    RUB total_utility_paid;
    RUB total_rent_paid;
    RUB total_mortgage_paid;
    RUB total_mortgage_interest_paid;
    RUB total_repair_paid;
    int repairs_count;
    int major_repairs_count;
};

struct Living {
    RUB food_base;
    RUB dining_out;            // кафе, доставка
    RUB healthy_food_extra;
    int food_quality;          // 0 = эконом, 1 = обычно, 2 = здоровое
    RUB groceries_extra;       // разовые крупные закупки
    RUB total_food_paid;
    RUB total_dining_out_paid;
    RUB total_groceries_paid;
};

struct Transport {
    int owned;
    int age_months;
    RUB value;
    RUB monthly_expenses;
    char const* model;         // "economy", "mid", "premium"
    int fuel_type;             // 0 = бензин, 1 = электро
    RUB fuel_cost_per_month;   // стоимость топлива в месяц
    RUB maintenance_cost;      // ТО
    RUB insurance_osago;
    RUB insurance_kasko;
    int has_kasko;
    int tech_inspection_due;   // через сколько месяцев техосмотр
    int months_since_purchase; // сколько месяцев с покупки
    RUB total_car_spent;
    int breakdowns_minor;
    int breakdowns_major;
    int license_suspended_months; // сколько месяцев без прав
    int cars_owned_total;
    RUB total_depreciation;    // суммарная амортизация
    RUB tire_change_cost;      // стоимость замены резины
    int tire_change_due_months;
    RUB parking_monthly;
    RUB carwash_monthly;
    RUB total_tire_paid;
    RUB total_parking_paid;
    RUB total_carwash_paid;
    RUB total_fines_paid;

    // Параметры выбора топлива (для сравнения TCO)
    RUB price_gas;             // цена бензиновой версии
    RUB price_electric;        // цена электрической версии
    RUB fuel_gas_monthly;      // топливо в месяц (бензин)
    RUB fuel_electric_monthly; // топливо в месяц (электро)
    RUB maint_gas_yearly;      // ТО в год (бензин)
    RUB maint_electric_yearly; // ТО в год (электро)
    RUB charging_station_cost; // установка зарядки дома (разово)
};

struct Pet {
    int alive;
    int age_months;
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
    int adoption_year;
    int adoption_month;
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
    int pvz_incidents;
};

struct Health {
    RUB total_spent;
    int months_sick;
    int months_seriously_sick;
    int total_illness_events;
    int total_serious_events;
};

struct Events {
    int fired;                 // сколько раз увольняли
    int inheritance;           // сколько раз наследство
    int car_crash;
    int car_crash_severe;      // сколько крупных ДТП
    int promotion;             // сколько внеплановых повышений
    int food_jump;             // сколько скачков цен на еду
    int tax_audit;             // сколько налоговых проверок
    int flat_bought_year;
    int flat_bought_month;
    int vacations_taken;
};

struct Vacation {
    int taken_this_year;
    int total_vacations;
    RUB total_spent;
    RUB base_cost;
    RUB current_cost;
    int quality;               // 0 = эконом, 1 = обычный, 2 = премиум
};

struct Person {
    struct Finances   money;
    struct Deposit    deposit;
    struct Investment invest;
    struct Job        job;
    struct Housing    home;
    struct Living     life;
    struct Transport  car;
    struct Pet        cat;
    struct Business   biz;
    struct Health     health;
    struct Events     events;
    struct Vacation   vacation;
    int use_mortgage;          // 1 = берёт ипотеку, 0 = копит
    int months_lived;
};

struct Person Zeckster;

int roll_d100() {
    int limit = RAND_MAX - (RAND_MAX % 100);   // максимум, кратный 100
    int r;
    do { r = rand(); } while (r >= limit);     // отбрасываем "хвост"
    return r % 100 + 1;
}

int roll_d1000() {
    int limit = RAND_MAX - (RAND_MAX % 1000);
    int r;
    do { r = rand(); } while (r >= limit);
    return r % 1000 + 1;
}

RUB min_rub(RUB a, RUB b) {
    return (a < b) ? a : b;
}

RUB max_rub(RUB a, RUB b) {
    return (a > b) ? a : b;
}

void Zeckster_init(int use_mortgage) {
    // Финансы
    Zeckster.money.cash = 20'000;
    Zeckster.money.savings = 0;
    Zeckster.money.cash_reserve = 80'000;
    Zeckster.money.excess = 0;
    Zeckster.money.total_income = 0;
    Zeckster.money.total_expenses = 0;
    Zeckster.money.bankrupt_count = 0;
    Zeckster.money.emergency_refills = 0;

    // Вклад
    Zeckster.deposit.active = 0;
    Zeckster.deposit.principal = 0;
    Zeckster.deposit.rate_bp = 1500;         // 15% годовых
    Zeckster.deposit.months_left = 0;
    Zeckster.deposit.start_year = 0;
    Zeckster.deposit.start_month = 0;
    Zeckster.deposit.total_interest = 0;
    Zeckster.deposit.deposits_opened = 0;
    Zeckster.deposit.max_principal_seen = 0;

    // ИИС-3
    Zeckster.invest.iis_open = 0;
    Zeckster.invest.iis_closed = 0;
    Zeckster.invest.iis_months_held = 0;
    Zeckster.invest.iis_open_year = 0;
    Zeckster.invest.iis_open_month = 0;
    Zeckster.invest.iis_contributed_total = 0;
    Zeckster.invest.iis_contributed_this_year = 0;
    Zeckster.invest.iis_value = 0;
    Zeckster.invest.iis_contribution_base = 0;
    Zeckster.invest.stocks_value = 0;
    Zeckster.invest.bonds_value = 0;
    Zeckster.invest.cash_on_iis = 0;
    Zeckster.invest.dividends_this_year = 0;
    Zeckster.invest.dividends_total = 0;
    Zeckster.invest.coupons_this_year = 0;
    Zeckster.invest.coupons_total = 0;
    Zeckster.invest.realized_profit_total = 0;
    Zeckster.invest.dividend_tax_paid_total = 0;
    Zeckster.invest.iis_tax_refund_total = 0;
    Zeckster.invest.iis_tax_saved = 0;
    Zeckster.invest.crashes_experienced = 0;
    Zeckster.invest.total_crash_losses = 0;
    Zeckster.invest.crisis_years = 0;
    Zeckster.invest.best_month_gain = 0;
    Zeckster.invest.worst_month_loss = 0;

    for (int i = 0; i < 20; i++) {
        Zeckster.invest.yearly_contribution[i] = 0;
        Zeckster.invest.yearly_value[i] = 0;
    }
    Zeckster.invest.years_recorded = 0;

    Zeckster.invest.stocks_allocation_bp = 7000;  // 70% акции
    Zeckster.invest.bonds_allocation_bp = 3000;   // 30% облигации
    Zeckster.invest.stock_expected_return_bp = 1200;
    Zeckster.invest.bond_expected_return_bp = 900;

    Zeckster.invest.total_dividend_events = 0;
    Zeckster.invest.total_coupon_events = 0;
    Zeckster.invest.rebalance_count = 0;
    Zeckster.invest.total_rebalanced_amount = 0;
    Zeckster.invest.stop_loss_triggers = 0;
    Zeckster.invest.total_stop_loss_saved = 0;
    Zeckster.invest.max_portfolio_value = 0;
    Zeckster.invest.min_portfolio_value = 0;
    Zeckster.invest.max_drawdown_bp = 0;
    Zeckster.invest.dividend_reinvest_count = 0;
    Zeckster.invest.dividend_reinvest_total = 0;
    Zeckster.invest.deposit_sweep_count = 0;
    Zeckster.invest.deposit_sweep_total = 0;

    for (int i = 0; i < 240; i++) {
        Zeckster.invest.monthly_returns[i] = 0;
    }
    Zeckster.invest.monthly_returns_count = 0;

    Zeckster.invest.bull_months = 0;
    Zeckster.invest.bear_months = 0;
    Zeckster.invest.flat_months = 0;

    // Работа
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

    // Жильё
    Zeckster.home.type = "Dormitory";
    Zeckster.home.rent_amount = 2'500;
    Zeckster.home.utility_base = 6'000;
    Zeckster.home.market_value = 0;
    Zeckster.home.mortgage_debt = 0;
    Zeckster.home.mortgage_payment = 0;
    Zeckster.home.mortgage_rate_bp = 900;
    Zeckster.home.mortgage_months_left = 0;
    Zeckster.home.mortgage_taken = 0;
    Zeckster.home.total_utility_paid = 0;
    Zeckster.home.total_rent_paid = 0;
    Zeckster.home.total_mortgage_paid = 0;
    Zeckster.home.total_mortgage_interest_paid = 0;
    Zeckster.home.total_repair_paid = 0;
    Zeckster.home.repairs_count = 0;
    Zeckster.home.major_repairs_count = 0;

    // Еда
    Zeckster.life.food_base = 12'000;
    Zeckster.life.dining_out = 3'000;
    Zeckster.life.healthy_food_extra = 0;
    Zeckster.life.food_quality = 0;
    Zeckster.life.groceries_extra = 0;
    Zeckster.life.total_food_paid = 0;
    Zeckster.life.total_dining_out_paid = 0;
    Zeckster.life.total_groceries_paid = 0;

    // Транспорт
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
    Zeckster.car.cars_owned_total = 0;
    Zeckster.car.total_depreciation = 0;
    Zeckster.car.tire_change_cost = 0;
    Zeckster.car.tire_change_due_months = 0;
    Zeckster.car.parking_monthly = 0;
    Zeckster.car.carwash_monthly = 0;
    Zeckster.car.total_tire_paid = 0;
    Zeckster.car.total_parking_paid = 0;
    Zeckster.car.total_carwash_paid = 0;
    Zeckster.car.total_fines_paid = 0;
    Zeckster.car.price_gas = 0;
    Zeckster.car.price_electric = 0;
    Zeckster.car.fuel_gas_monthly = 0;
    Zeckster.car.fuel_electric_monthly = 0;
    Zeckster.car.maint_gas_yearly = 0;
    Zeckster.car.maint_electric_yearly = 0;
    Zeckster.car.charging_station_cost = 0;

    // Питомец
    Zeckster.cat.alive = 0;
    Zeckster.cat.age_months = 0;
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
    Zeckster.cat.adoption_year = 0;
    Zeckster.cat.adoption_month = 0;

    // Бизнес
    Zeckster.biz.pvz_open = 0;
    Zeckster.biz.pvz_months = 0;
    Zeckster.biz.pvz_open_year = 0;
    Zeckster.biz.pvz_open_month = 0;
    Zeckster.biz.pvz_income = 0;
    Zeckster.biz.pvz_costs = 0;
    Zeckster.biz.pvz_invested = 0;
    Zeckster.biz.total_pvz_net = 0;
    Zeckster.biz.pvz_incidents = 0;

    // Здоровье
    Zeckster.health.total_spent = 0;
    Zeckster.health.months_sick = 0;
    Zeckster.health.months_seriously_sick = 0;
    Zeckster.health.total_illness_events = 0;
    Zeckster.health.total_serious_events = 0;

    // События
    Zeckster.events.fired = 0;
    Zeckster.events.inheritance = 0;
    Zeckster.events.car_crash = 0;
    Zeckster.events.car_crash_severe = 0;
    Zeckster.events.promotion = 0;
    Zeckster.events.food_jump = 0;
    Zeckster.events.tax_audit = 0;
    Zeckster.events.flat_bought_year = 0;
    Zeckster.events.flat_bought_month = 0;
    Zeckster.events.vacations_taken = 0;

    // Отпуск
    Zeckster.vacation.taken_this_year = 0;
    Zeckster.vacation.total_vacations = 0;
    Zeckster.vacation.total_spent = 0;
    Zeckster.vacation.base_cost = 80'000;
    Zeckster.vacation.current_cost = 80'000;
    Zeckster.vacation.quality = 0;

    Zeckster.use_mortgage = use_mortgage;
    Zeckster.months_lived = 0;
}

void Zeckster_check_reserve() {  // Если cash меньше резерва — снимаем из savings, чтобы восстановить резерв
    if (Zeckster.money.cash < Zeckster.money.cash_reserve &&
        Zeckster.money.savings > 0) {
        RUB need = Zeckster.money.cash_reserve - Zeckster.money.cash;
        RUB take = (need < Zeckster.money.savings) ? need : Zeckster.money.savings;
        Zeckster.money.savings -= take;
        Zeckster.money.cash += take;
        Zeckster.money.emergency_refills++;
    }
}

void Zeckster_pay(RUB amount) { // Оплата amount с защитой от ухода cash в минус
    if (amount == 0) return;
    if (amount > Zeckster.money.cash) {
        Zeckster_check_reserve();
    }
    if (amount > Zeckster.money.cash) { // даже после refill не хватает — фиксируем банкротство
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
        Zeckster.home.utility_base = (RUB)(Zeckster.home.utility_base * 1.09);
        Zeckster.life.food_base = (RUB)(Zeckster.life.food_base * 1.11);
        Zeckster.life.dining_out = (RUB)(Zeckster.life.dining_out * 1.11);
        Zeckster.life.healthy_food_extra = (RUB)(Zeckster.life.healthy_food_extra * 1.11);
        Zeckster.cat.food_monthly = (RUB)(Zeckster.cat.food_monthly * 1.12);
        Zeckster.cat.vet_monthly = (RUB)(Zeckster.cat.vet_monthly * 1.10);
        Zeckster.car.fuel_cost_per_month = (RUB)(Zeckster.car.fuel_cost_per_month * 1.08);
        Zeckster.vacation.base_cost = (RUB)(Zeckster.vacation.base_cost * 1.11);

        if (Zeckster.home.market_value == 0) {
            Zeckster.home.rent_amount = (RUB)(Zeckster.home.rent_amount * 1.08);
        }
        else {
            Zeckster.home.market_value = (RUB)(Zeckster.home.market_value * 1.05);
        }
    }
}

void Zeckster_health_step(int month) { // Случайные расходы на здоровье
    int roll = roll_d100();
    RUB cost = 0;
    int serious = 0;
    int sick_modifier = 0;
    if (Zeckster.life.food_quality == 2) sick_modifier -= 5;   // здоровое питание → реже болеем
    if (Zeckster.life.food_quality == 0) sick_modifier += 3;   // экономное → чаще болеем

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
        Zeckster.health.total_illness_events++;
        if (serious) {
            Zeckster.health.months_seriously_sick++;
            Zeckster.health.total_serious_events++;
        }
    }
}

void Zeckster_repair_step(const int year, const int month) {
    if (Zeckster.home.market_value == 0) return;
    if (month != 7) return;

    int roll = roll_d100();
    if (roll <= 40) {   // 40% шанс что-то сломалось
        RUB cost;
        int type = roll_d100();
        if (type <= 60)      cost = 50'000;   // мелкий
        else if (type <= 90) cost = 150'000;  // средний
        else { cost = 300'000; Zeckster.home.major_repairs_count++; }  // крупный
        Zeckster_pay(cost);
        Zeckster.home.total_repair_paid += cost;
        Zeckster.home.repairs_count++;
    }
}

int Zeckster_cat_death_threshold() {
    int age = Zeckster.cat.age_months;
    if (age < 120) return 0;   // 0–10 лет — не умирает
    if (age < 144) return 1;   // 10–12 лет — 1% в месяц
    if (age < 168) return 2;   // 12–14 лет — 2%
    if (age < 192) return 5;   // 14–16 лет — 5%
    return 10;                 // 16+ — 10%
}

void Zeckster_cat_mechanics(const int year, const int month) {
    if (Zeckster.cat.alive == 0 && Zeckster.cat.death_year == 0 &&
        year == 2028 && month == 6) {
        Zeckster.cat.alive = 1;
        Zeckster.cat.age_months = 0;
        Zeckster.cat.adoption_year = year;
        Zeckster.cat.adoption_month = month;
        return;
    }
    if (Zeckster.cat.alive == 0) return;

    // Проверка смерти
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

    if (!Zeckster.cat.sterilized && Zeckster.cat.age_months >= 6 && Zeckster.cat.age_months <= 12) {  // Стерилизация (один раз в первый год жизни)
        RUB cost = 15'000;
        Zeckster_pay(cost);
        Zeckster.cat.total_pet_paid += cost;
        Zeckster.cat.sterilized = 1;
    }

    if (month == 3) {  // Прививки раз в год в марте
        RUB cost = 5'000;
        Zeckster_pay(cost);
        Zeckster.cat.total_pet_paid += cost;
        Zeckster.cat.vaccine_count++;
    }

    if (month != 3) {    // Болезни — 5% шанс в месяц (кроме марта)
        int roll = roll_d100();
        if (roll <= 5) {
            RUB cost = 10'000 + roll_d100() * 200;   // 10 000 – 30 000
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

void Zeckster_car_crash_event(const int year, const int month) {  // ДТП: 2% в год в сентябре, если есть машина
    if (month != 9) return;
    if (Zeckster.car.owned == 0) return;

    int roll = roll_d100();
    if (roll > 2) return;

    int type_roll = roll_d100();
    RUB fine, repair;
    int severe = 0;
    if (type_roll <= 60) { fine = 30'000; repair = 70'000; }         // мелкое
    else if (type_roll <= 90) { fine = 100'000; repair = 200'000; }  // среднее
    else { fine = 300'000; repair = 500'000; severe = 1; }           // крупное

    if (Zeckster.car.has_kasko) fine = fine / 2;   // КАСКО покрывает 50% штрафа

    RUB total = fine + repair;
    Zeckster.car.value = (Zeckster.car.value > repair)
        ? Zeckster.car.value - repair : 50'000;   // ремонт уменьшает стоимость
    Zeckster_pay(total);
    if (severe) {
        Zeckster.car.license_suspended_months = 3; // теряем права на 3 месяца
        Zeckster.events.car_crash_severe++;
    }
    Zeckster.events.car_crash++;
}

void Zeckster_tax_audit_event(const int year, const int month) {  // Налоговая проверка ПВЗ: 2% в год в апреле
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

void Zeckster_random_events(const int year, const int month) {  // Увольнение: 2% в год в июне
    if (month == 6 && year >= 2029) {
        int roll = roll_d100();
        if (roll <= 2) {
            RUB lost = Zeckster.job.salary * 3;   // 3 месяца без зарплаты
            Zeckster_pay(lost);
            Zeckster.events.fired++;
        }
    }
    if (month == 3 && year >= 2032) {   // Наследство: 2% в год в марте
        int roll = roll_d100();
        if (roll <= 2) {
            Zeckster.money.cash += 2'000'000;
            Zeckster.money.total_income += 2'000'000;
            Zeckster.events.inheritance++;
        }
    }
    if (month == 6 && year >= 2029 && Zeckster.job.salary > 0) {  // Внеплановое повышение: 3% в год в июне
        int roll = roll_d100();
        if (roll <= 3) {
            Zeckster.job.salary = (RUB)(Zeckster.job.salary * 1.15);
            Zeckster.events.promotion++;
        }
    }

    if (month == 8 && year >= 2028) {
        int roll = roll_d100();
        if (roll <= 6) {
            Zeckster.life.food_base = (RUB)(Zeckster.life.food_base * 1.10);
            Zeckster.events.food_jump++;
        }
    }
}

void Zeckster_vacation_step(const int year, const int month) {
    // Отпуск: до покупки квартиры — 2 раза, после — каждый год
    if (month == 1) {
        Zeckster.vacation.taken_this_year = 0;   // сбрасываем флаг в январе
    }

    // Уровень отпуска зависит от накоплений
    if (Zeckster.money.savings >= 3'000'000) {
        Zeckster.vacation.quality = 2;
        Zeckster.vacation.base_cost = 250'000;
    }
    else if (Zeckster.money.savings >= 1'000'000) {
        Zeckster.vacation.quality = 1;
        Zeckster.vacation.base_cost = 150'000;
    }
    else {
        Zeckster.vacation.quality = 0;
        Zeckster.vacation.base_cost = 80'000;
    }

    if (Zeckster.home.market_value == 0) {
        if (!((year == 2029 && month == 8) || (year == 2032 && month == 8))) {
            return;
        }
    }
    else {
        if (month != 8) return;
    }

    if (Zeckster.vacation.taken_this_year) return;
    if (year < 2029) return;

    RUB cost = Zeckster.vacation.base_cost;   // Случайное отклонение стоимости ±30%
    int roll = roll_d100();
    if (roll <= 20)      cost = cost * 70 / 100;
    else if (roll >= 81) cost = cost * 130 / 100;

    Zeckster.vacation.current_cost = cost;
    Zeckster_pay(cost);
    Zeckster.vacation.total_spent += cost;
    Zeckster.vacation.total_vacations++;
    Zeckster.vacation.taken_this_year = 1;
    Zeckster.events.vacations_taken++;
}

void Zeckster_salary(const int year, const int month) {
    if (year == 2027 && month == 8) {
        Zeckster.job.salary = 65'000;
        Zeckster.job.title = "Beginning engineer";
        Zeckster.job.bonus_rate_bp = 4'000;   // 40% премии
    }
    if (year == 2028 && month == 9) {
        Zeckster.job.salary = 80'000;
        Zeckster.job.title = "Engineer";
        Zeckster.job.bonus_rate_bp = 6'000;
    }
    if (year == 2030 && month == 1) {
        Zeckster.job.salary = 140'000;
        Zeckster.job.title = "Lead engineer";
        Zeckster.job.bonus_rate_bp = 10'000;   // 100% премии
        Zeckster.job.salary2 = 0;              // отключаем фриланс
        Zeckster.job.title2 = "None";
    }

    if (month == 1 && year >= 2028) {
        Zeckster.job.salary = (RUB)(Zeckster.job.salary * 1.08);
        if (Zeckster.job.salary2 > 0) {
            Zeckster.job.salary2 = (RUB)(Zeckster.job.salary2 * 1.08);
        }
    }

    Zeckster.money.cash += Zeckster.job.salary;

    if (year == 2029) {
        if (Zeckster.job.salary2 == 0) {
            Zeckster.job.salary2 = 25'000;
            Zeckster.job.title2 = "Freelance";
        }
        Zeckster.money.cash += Zeckster.job.salary2;
    }

    Zeckster.money.total_income += Zeckster.job.salary + Zeckster.job.salary2;
}

void Zeckster_ndfl(const int year, const int month) {   // НДФЛ: удерживаем 13% и ведём учёт для вычетов
    if (month == 1) {
        Zeckster.job.ndfl_paid_last_year = Zeckster.job.ndfl_paid_this_year;  // Сброс годовых счётчиков в январе
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
        Zeckster.job.bonus = Zeckster.job.salary * Zeckster.job.bonus_rate_bp / 10'000;
        Zeckster.money.cash += Zeckster.job.bonus;
        Zeckster.money.total_income += Zeckster.job.bonus;
    }
}

void Zeckster_groceries_step(const int year, const int month) {
    int roll = roll_d100();
    if (roll <= 15) {
        RUB extra = 5'000 + roll_d100() * 100;   // 5 000 – 15 000
        Zeckster.life.groceries_extra = extra;
        Zeckster_pay(extra);
        Zeckster.life.total_food_paid += extra;
        Zeckster.life.total_groceries_paid += extra;
    }
    else {
        Zeckster.life.groceries_extra = 0;
    }
}

void Zeckster_food_quality_step(const int year, const int month) {  // Смена качества питания: эконом → обычно → здоровое
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
    Zeckster.life.total_dining_out_paid += Zeckster.life.dining_out;
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
    RUB interest = (RUB)(Zeckster.money.savings * 0.11 / 12.0);
    Zeckster.money.savings += interest;
    Zeckster.money.total_income += interest;
}

void Zeckster_tax_refund(const int year, const int month) {  // Налоговые вычеты: имущественный, ипотечный, социальный
    if (month != 4) return;   // подаём в апреле
    RUB total_refund = 0;

    // 1. Имущественный вычет: 13% от стоимости квартиры, лимит 260 000
    if (Zeckster.home.market_value > 0 &&
        Zeckster.job.property_deduction_used < Zeckster.job.property_deduction_cap) {
        RUB yearly_portion = (RUB)(7'000'000 * 0.13 / 10);
        RUB remaining = Zeckster.job.property_deduction_cap -
            Zeckster.job.property_deduction_used;
        RUB refund = (remaining < yearly_portion) ? remaining : yearly_portion;
        total_refund += refund;
        Zeckster.job.property_deduction_used += refund;
    }

    // 2. Ипотечный вычет: 13% от уплаченных процентов, лимит 390 000
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

    // 3. Социальный вычет на лечение: лимит 19 500 в год
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

    // Общее ограничение: не больше уплаченного НДФЛ за прошлый год
    if (total_refund > Zeckster.job.ndfl_paid_last_year) {
        total_refund = Zeckster.job.ndfl_paid_last_year;
    }
    Zeckster.money.cash += total_refund;
    Zeckster.money.total_income += total_refund;
}

void Zeckster_savings_step() {     // Перекладываем 20% излишка в savings
    if (Zeckster.money.cash > Zeckster.money.cash_reserve) {
        Zeckster.money.excess = Zeckster.money.cash - Zeckster.money.cash_reserve;
        RUB dep = Zeckster.money.excess * 20 / 100;
        Zeckster.money.cash -= dep;
        Zeckster.money.savings += dep;
    }
}

void Zeckster_deposit_step(const int year, const int month) {  // Вклад под 15% на 3 года: открытие, начисление, закрытие
    if (!Zeckster.deposit.active && Zeckster.money.savings >= 2'000'000) {   // Открытие: если savings ≥ 2 млн и вклад не активен
        Zeckster.money.savings -= 1'800'000;
        Zeckster.deposit.active = 1;
        Zeckster.deposit.principal = 1'800'000;
        Zeckster.deposit.rate_bp = 1600;
        Zeckster.deposit.months_left = 36;
        Zeckster.deposit.start_year = year;
        Zeckster.deposit.start_month = month;
        Zeckster.deposit.deposits_opened++;
    }
    if (Zeckster.deposit.active) {  // Начисление процентов и закрытие
        RUB interest = Zeckster.deposit.principal *
            Zeckster.deposit.rate_bp / 10'000 / 12;
        Zeckster.deposit.principal += interest;
        Zeckster.deposit.months_left--;
        if (Zeckster.deposit.principal > Zeckster.deposit.max_principal_seen) {
            Zeckster.deposit.max_principal_seen = Zeckster.deposit.principal;
        }
        if (Zeckster.deposit.months_left <= 0) {
            Zeckster.money.savings += Zeckster.deposit.principal;
            Zeckster.deposit.total_interest += Zeckster.deposit.principal - 1'800'000;
            Zeckster.deposit.active = 0;
            Zeckster.deposit.principal = 0;
        }
    }
}

void Zeckster_invest_sell_high(const int year, const int month) {  // Фиксация прибыли: раз в ~25 месяцев продаём 20% акций
    if (!Zeckster.invest.iis_open) return;
    if (Zeckster.invest.stocks_value < 500'000) return;

    int roll = roll_d100();
    if (roll <= 4) {
        RUB sell_amount = Zeckster.invest.stocks_value * 20 / 100;
        Zeckster.invest.stocks_value -= sell_amount;
        Zeckster.invest.cash_on_iis += sell_amount;
        Zeckster.invest.realized_profit_total += sell_amount / 10;
    }
}

void Zeckster_invest_buy_low(const int year, const int month) { // Докупка акций при падении (усреднение)
    if (!Zeckster.invest.iis_open) return;
    if (Zeckster.invest.cash_on_iis < 100'000) return;

    int roll = roll_d100();
    if (roll <= 5) {
        RUB buy_amount = Zeckster.invest.cash_on_iis * 50 / 100;
        Zeckster.invest.stocks_value += buy_amount;
        Zeckster.invest.cash_on_iis -= buy_amount;
    }
}

void Zeckster_invest_rebalance(const int year, const int month) {   // Ребалансировка портфеля раз в год (июнь): 70/30
    if (!Zeckster.invest.iis_open) return;
    if (month != 6) return;

    RUB total = Zeckster.invest.stocks_value + Zeckster.invest.bonds_value;
    if (total < 100'000) return;

    RUB target_stocks = total * Zeckster.invest.stocks_allocation_bp / 10'000;
    RUB target_bonds = total - target_stocks;

    RUB diff_stocks = (Zeckster.invest.stocks_value > target_stocks)
        ? Zeckster.invest.stocks_value - target_stocks
        : target_stocks - Zeckster.invest.stocks_value;


    if (diff_stocks > total * 5 / 100) { // Ребалансируем только при отклонении > 5%
        if (Zeckster.invest.stocks_value > target_stocks) {
            RUB sell = Zeckster.invest.stocks_value - target_stocks;
            Zeckster.invest.stocks_value -= sell;
            Zeckster.invest.bonds_value += sell;
            Zeckster.invest.total_rebalanced_amount += sell;
        }
        else {
            RUB buy = target_stocks - Zeckster.invest.stocks_value;
            if (buy <= Zeckster.invest.bonds_value) {
                Zeckster.invest.stocks_value += buy;
                Zeckster.invest.bonds_value -= buy;
                Zeckster.invest.total_rebalanced_amount += buy;
            }
        }
        Zeckster.invest.rebalance_count++;
    }
}

void Zeckster_invest_stop_loss(const int year, const int month) {  // Stop-loss: 2% шанс переложить 40% акций в облигации
    if (!Zeckster.invest.iis_open) return;
    if (Zeckster.invest.stocks_value < 200'000) return;

    int roll = roll_d100();
    if (roll <= 2) {
        RUB move = Zeckster.invest.stocks_value * 40 / 100;
        Zeckster.invest.stocks_value -= move;
        Zeckster.invest.bonds_value += move;
        Zeckster.invest.stop_loss_triggers++;
        Zeckster.invest.total_stop_loss_saved += move / 20;
    }
}

void Zeckster_invest_reinvest_dividends(const int year, const int month) {  // Реинвестирование дивидендов и купонов: раз в квартал
    if (!Zeckster.invest.iis_open) return;
    if (Zeckster.invest.cash_on_iis < 30'000) return;

    if (month == 4 || month == 7 || month == 10 || month == 1) {
        RUB amount = Zeckster.invest.cash_on_iis * 80 / 100;
        Zeckster.invest.stocks_value += amount;
        Zeckster.invest.cash_on_iis -= amount;
        Zeckster.invest.dividend_reinvest_count++;
        Zeckster.invest.dividend_reinvest_total += amount;
    }
}

void Zeckster_invest_sweep_deposit(const int year, const int month) { // Перекачка с вклада на ИИС (если есть свободный лимит взноса)
    if (!Zeckster.invest.iis_open) return;
    if (Zeckster.invest.iis_contributed_this_year >= 400'000) return;
    if (!Zeckster.deposit.active) return;
    if (Zeckster.deposit.principal < 500'000) return;

    if (month == 2) {
        RUB available = 400'000 - Zeckster.invest.iis_contributed_this_year;
        RUB from_deposit = (Zeckster.deposit.principal > available)
            ? available : Zeckster.deposit.principal;
        if (from_deposit > 100'000) {
            Zeckster.deposit.principal -= from_deposit;
            Zeckster.invest.cash_on_iis += from_deposit;
            Zeckster.invest.iis_contributed_this_year += from_deposit;
            Zeckster.invest.iis_contributed_total += from_deposit;
            Zeckster.invest.deposit_sweep_count++;
            Zeckster.invest.deposit_sweep_total += from_deposit;
        }
    }
}

void Zeckster_invest_record_year(const int year, const int month) { // Запись стоимости портфеля в конце года
    if (!Zeckster.invest.iis_open) return;
    if (month != 12) return;
    if (Zeckster.invest.years_recorded >= 20) return;

    int idx = Zeckster.invest.years_recorded;
    Zeckster.invest.yearly_value[idx] = Zeckster.invest.iis_value;
    Zeckster.invest.years_recorded++;
}

void Zeckster_invest_step(const int year, const int month) {  // Основной шаг ИИС-3: открытие, взносы, доходность, дивиденды, купоны
    if (!Zeckster.invest.iis_open && !Zeckster.invest.iis_closed &&
        Zeckster.money.savings >= 1'000'000) {
        Zeckster.invest.iis_open = 1;
        Zeckster.invest.iis_months_held = 0;
        Zeckster.invest.iis_open_year = year;
        Zeckster.invest.iis_open_month = month;
    }
    if (!Zeckster.invest.iis_open) return;
    Zeckster.invest.iis_months_held++;

    if (month == 1) { // Взнос раз в год в январе: до 400 000 (лимит по НК РФ)
        RUB available = Zeckster.money.savings;
        RUB max_contrib = 400'000;
        RUB to_contribute = (available < max_contrib) ? available : max_contrib;
        if (to_contribute > 0) {
            Zeckster.money.savings -= to_contribute;
            Zeckster.invest.cash_on_iis += to_contribute;
            Zeckster.invest.iis_contributed_total += to_contribute;
            Zeckster.invest.iis_contributed_this_year += to_contribute;

            int idx = Zeckster.invest.years_recorded;
            if (idx < 20) Zeckster.invest.yearly_contribution[idx] = to_contribute;
        }
    }

    // Инвестируем свободные деньги: 70% акции, 30% облигации
    RUB free_cash = Zeckster.invest.cash_on_iis;
    if (free_cash > 100'000) {
        RUB to_stocks = free_cash * Zeckster.invest.stocks_allocation_bp / 10'000;
        RUB to_bonds = free_cash - to_stocks;
        Zeckster.invest.stocks_value += to_stocks;
        Zeckster.invest.bonds_value += to_bonds;
        Zeckster.invest.cash_on_iis = 0;
    }

    int stock_roll = roll_d100();  // Случайная доходность акций за месяц
    double stock_return;
    if (stock_roll <= 2) { stock_return = -0.18; Zeckster.invest.bear_months++; }
    else if (stock_roll <= 8) { stock_return = -0.08; Zeckster.invest.bear_months++; }
    else if (stock_roll <= 20) { stock_return = -0.03; Zeckster.invest.bear_months++; }
    else if (stock_roll <= 55) { stock_return = 0.010; Zeckster.invest.flat_months++; }
    else if (stock_roll <= 80) { stock_return = 0.022; Zeckster.invest.bull_months++; }
    else if (stock_roll <= 95) { stock_return = 0.040; Zeckster.invest.bull_months++; }
    else { stock_return = 0.070; Zeckster.invest.bull_months++; }

    double bond_return = 0.0075;   // облигации стабильно +0.75% в месяц

    // Считаем через double, чтобы корректно обработать отрицательное значение
    double stock_gain_d = (double)Zeckster.invest.stocks_value * stock_return;
    RUB bond_gain = (RUB)(Zeckster.invest.bonds_value * bond_return);

    if (stock_gain_d < 0) {  // Убыток: берём модуль через знаковое, обрезаем по величине портфеля
        RUB loss = (RUB)(-stock_gain_d);
        if (loss > Zeckster.invest.stocks_value) loss = Zeckster.invest.stocks_value;
        Zeckster.invest.stocks_value -= loss;
        Zeckster.invest.total_crash_losses += loss;
        Zeckster.invest.crashes_experienced++;
        if (loss > Zeckster.invest.worst_month_loss) {
            Zeckster.invest.worst_month_loss = loss;
        }
    }
    else {
        RUB stock_gain = (RUB)stock_gain_d;
        Zeckster.invest.stocks_value += stock_gain;
        if (stock_gain > Zeckster.invest.best_month_gain) {
            Zeckster.invest.best_month_gain = stock_gain;
        }
    }
    Zeckster.invest.bonds_value += bond_gain;

    // Дивиденды по акциям: ~2% раз в квартал
    if (month == 3 || month == 6 || month == 9 || month == 12) {
        RUB div = (RUB)(Zeckster.invest.stocks_value * 0.02);
        Zeckster.invest.dividends_this_year += div;
        Zeckster.invest.dividends_total += div;
        Zeckster.invest.cash_on_iis += div;
        Zeckster.invest.total_dividend_events++;
    }

    // Купоны по облигациям: ~0.75% ежемесячно
    RUB coupons = (RUB)(Zeckster.invest.bonds_value * 0.0075);
    Zeckster.invest.coupons_this_year += coupons;
    Zeckster.invest.coupons_total += coupons;
    Zeckster.invest.cash_on_iis += coupons;
    Zeckster.invest.total_coupon_events++;

    // Налог на дивиденды 13% — удерживается всегда (не входит в льготы ИИС)
    if (month == 12) {
        RUB tax = Zeckster.invest.dividends_this_year * 13 / 100;
        Zeckster.invest.dividend_tax_paid_total += tax;
        Zeckster.invest.cash_on_iis -= tax;
        Zeckster.invest.dividends_this_year = 0;
    }

    Zeckster.invest.iis_value = Zeckster.invest.stocks_value + Zeckster.invest.bonds_value + Zeckster.invest.cash_on_iis;  // Обновляем общую стоимость портфеля

    // Отслеживаем максимум/минимум и максимальную просадку
    if (Zeckster.invest.iis_value > Zeckster.invest.max_portfolio_value) {
        Zeckster.invest.max_portfolio_value = Zeckster.invest.iis_value;
    }
    if (Zeckster.invest.min_portfolio_value == 0 ||
        Zeckster.invest.iis_value < Zeckster.invest.min_portfolio_value) {
        Zeckster.invest.min_portfolio_value = Zeckster.invest.iis_value;
    }
    if (Zeckster.invest.max_portfolio_value > 0) {
        RUB dd = Zeckster.invest.max_portfolio_value - Zeckster.invest.iis_value;
        int dd_bp = (int)(dd * 10'000 / Zeckster.invest.max_portfolio_value);
        if (dd_bp > Zeckster.invest.max_drawdown_bp) {
            Zeckster.invest.max_drawdown_bp = dd_bp;
        }
    }

    if (Zeckster.invest.monthly_returns_count < 240) {  // Записываем доходность месяца в массив (только положительные)
        int idx = Zeckster.invest.monthly_returns_count;
        if (stock_return >= 0) {
            Zeckster.invest.monthly_returns[idx] = (RUB)(stock_return * 10000);
        }
        Zeckster.invest.monthly_returns_count++;
    }
}

void Zeckster_iis_tax_refund(const int year, const int month) {   // Тип А: возврат 13% или 15% от взносов на ИИС (лимит 400 000)
    if (month != 4) return;                             // подаём в апреле
    if (!Zeckster.invest.iis_open) return;
    if (Zeckster.invest.iis_contributed_this_year == 0) return;

    RUB base = Zeckster.invest.iis_contributed_this_year;
    if (base > 400'000) base = 400'000;                // лимит взноса

    RUB annual_gross = Zeckster.job.salary * 12;
    RUB rate_bp = (annual_gross > 2'400'000) ? 1500 : 1300;   // 15% или 13%

    RUB refund = base * rate_bp / 10'000;
    if (refund > Zeckster.job.ndfl_paid_last_year) {
        refund = Zeckster.job.ndfl_paid_last_year;     // не больше уплаченного НДФЛ
    }
    Zeckster.money.cash += refund;
    Zeckster.money.total_income += refund;
    Zeckster.invest.iis_tax_refund_total += refund;
    Zeckster.invest.iis_contributed_this_year = 0;     // сброс годового счётчика
}

void Zeckster_iis_close(const int year, const int month) {  // Тип Б: закрытие ИИС через 9 лет (108 месяцев) без налога с прибыли до 30 млн
    if (!Zeckster.invest.iis_open) return;
    if (Zeckster.invest.iis_months_held < 108) return;

    RUB profit = Zeckster.invest.iis_value -
        Zeckster.invest.iis_contributed_total;
    if (profit > 0 && profit <= 30'000'000) {
        Zeckster.invest.iis_tax_saved = profit * 13 / 100;   // условная экономия
    }
    // Переносим все деньги в savings
    Zeckster.money.savings += Zeckster.invest.iis_value;
    Zeckster.invest.iis_value = 0;
    Zeckster.invest.stocks_value = 0;
    Zeckster.invest.bonds_value = 0;
    Zeckster.invest.cash_on_iis = 0;
    Zeckster.invest.iis_open = 0;
    Zeckster.invest.iis_closed = 1;
}

void Zeckster_market_crisis(const int year, const int month) {   // Кризисы рынка: октябрь, шанс обвала 10% (3% крупный, 7% средний)
    if (month != 10) return;
    if (!Zeckster.invest.iis_open) return;
    if (Zeckster.invest.stocks_value < 100'000) return;

    int roll = roll_d100();
    if (roll <= 3) {
        // Крупный обвал −30%
        RUB loss = Zeckster.invest.stocks_value * 30 / 100;
        Zeckster.invest.stocks_value -= loss;
        Zeckster.invest.total_crash_losses += loss;
        Zeckster.invest.crashes_experienced++;
        Zeckster.invest.crisis_years++;
    }
    else if (roll <= 10) {
        // Средняя коррекция −15%
        RUB loss = Zeckster.invest.stocks_value * 15 / 100;
        Zeckster.invest.stocks_value -= loss;
        Zeckster.invest.total_crash_losses += loss;
        Zeckster.invest.crashes_experienced++;
    }
}

void Zeckster_move(const int year, const int month) {
    if (year == 2028 && month == 8) {
        Zeckster.home.rent_amount = 50'000;
        Zeckster.home.type = "Rented apartment";
        Zeckster.life.food_base = (RUB)(Zeckster.life.food_base * 1.3);
    }
}

void Zeckster_buy_flat_cash(const int year, const int month) {  // Покупка квартиры за наличные (только для saving-стратегии)
    if (Zeckster.use_mortgage == 1) return;            // только для saving
    if (Zeckster.home.market_value > 0) return;        // уже есть квартира

    RUB total_money = Zeckster.money.savings;
    if (Zeckster.deposit.active) total_money += Zeckster.deposit.principal;

    if (total_money >= 7'000'000) {
        RUB needed = 7'000'000;
        // Сначала берём из savings
        RUB from_savings = (needed < Zeckster.money.savings)
            ? needed : Zeckster.money.savings;
        Zeckster.money.savings -= from_savings;
        RUB rest = needed - from_savings;

        // Добираем из вклада, если нужно
        if (rest > 0 && Zeckster.deposit.active) {
            Zeckster.deposit.principal -= rest;
            if (Zeckster.deposit.principal == 0) Zeckster.deposit.active = 0;
        }
        Zeckster.home.market_value = 7'000'000;
        Zeckster.home.type = "Own apartment (bought cash)";
        Zeckster.home.rent_amount = 0;
        Zeckster.events.flat_bought_year = year;
        Zeckster.events.flat_bought_month = month;
    }
}

void Zeckster_choose_car_model() {   // Базовая цена и параметры по модели (для бензина)
    RUB base_price;
    RUB base_fuel, base_maint;

    if (Zeckster.money.savings >= 2'000'000) {
        Zeckster.car.model = "premium";
        base_price = 4'000'000;
        base_fuel = 25'000;
        base_maint = 110'000;
        Zeckster.car.insurance_osago = 15'000;
        Zeckster.car.insurance_kasko = 80'000;
        Zeckster.car.tire_change_cost = 60'000;
        Zeckster.car.parking_monthly = 15'000;
        Zeckster.car.carwash_monthly = 5'000;
    }
    else if (Zeckster.money.savings >= 1'000'000) {
        Zeckster.car.model = "mid";
        base_price = 1'500'000;
        base_fuel = 15'000;
        base_maint = 60'000;
        Zeckster.car.insurance_osago = 10'000;
        Zeckster.car.insurance_kasko = 40'000;
        Zeckster.car.tire_change_cost = 35'000;
        Zeckster.car.parking_monthly = 8'000;
        Zeckster.car.carwash_monthly = 3'000;
    }
    else {
        Zeckster.car.model = "economy";
        base_price = 600'000;
        base_fuel = 8'000;
        base_maint = 30'000;
        Zeckster.car.insurance_osago = 7'000;
        Zeckster.car.insurance_kasko = 0;
        Zeckster.car.tire_change_cost = 20'000;
        Zeckster.car.parking_monthly = 3'000;
        Zeckster.car.carwash_monthly = 1'500;
    }

    // Параметры для сравнения бензин vs электро
    Zeckster.car.price_gas = base_price;
    Zeckster.car.price_electric = (RUB)(base_price * 1.2);          // электро дороже на 20%
    Zeckster.car.fuel_gas_monthly = base_fuel;
    Zeckster.car.fuel_electric_monthly = (RUB)(base_fuel / 3);      // зарядка в 3 раза дешевле
    Zeckster.car.maint_gas_yearly = base_maint;
    Zeckster.car.maint_electric_yearly = (RUB)(base_maint / 2);     // ТО электро в 2 раза дешевле
    Zeckster.car.charging_station_cost = (RUB)(base_price * 0.03);  // 3% от цены — установка зарядки дома

    RUB tco_gas = Zeckster.car.price_gas  // Сравнение TCO за 5 лет (60 месяцев)
        + Zeckster.car.fuel_gas_monthly * 60
        + Zeckster.car.maint_gas_yearly * 5;

    RUB tco_electric = Zeckster.car.price_electric
        + Zeckster.car.charging_station_cost
        + Zeckster.car.fuel_electric_monthly * 60
        + Zeckster.car.maint_electric_yearly * 5;

    // Выбираем то, что дешевле в долгосрочной перспективе
    if (tco_electric < tco_gas) {
        Zeckster.car.fuel_type = 1;   // электро
    }
    else {
        Zeckster.car.fuel_type = 0;   // бензин
    }
}

void Zeckster_car_breakdown_check() {  // Мелкие (12%/мес) и крупные (1%/мес) поломки, КАСКО покрывает 70% ремонта
    int roll = roll_d100();
    if (roll <= 12) {
        RUB cost = 5'000 + roll_d100() * 150;   // 5 000 – 20 000
        Zeckster.car.breakdowns_minor++;
        Zeckster.car.total_car_spent += cost;
        Zeckster_pay(cost);
    }
    if (roll >= 99) {
        RUB cost = 50'000 + roll_d100() * 1'000;   // 50 000 – 150 000
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

void Zeckster_car_insurance_step(const int month) {  // ОСАГО и КАСКО раз в год в апреле
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

void Zeckster_car_tire_step() {
    if (Zeckster.car.owned == 0) return;

    Zeckster.car.tire_change_due_months--;
    if (Zeckster.car.tire_change_due_months <= 0) {
        RUB cost = Zeckster.car.tire_change_cost;
        int roll = roll_d100();
        if (roll >= 81) cost = cost * 130 / 100;   // подорожало
        else if (roll <= 20) cost = cost * 80 / 100;   // подешевело

        Zeckster_pay(cost);
        Zeckster.car.total_car_spent += cost;
        Zeckster.car.total_tire_paid += cost;
        Zeckster.car.tire_change_due_months = 48;   // раз в 4 года
    }
}

void Zeckster_car_parking_carwash_step() {
    if (Zeckster.car.owned == 0) return;

    RUB parking = Zeckster.car.parking_monthly;
    RUB carwash = Zeckster.car.carwash_monthly;

    Zeckster_pay(parking);
    Zeckster_pay(carwash);
    Zeckster.car.total_car_spent += parking + carwash;
    Zeckster.car.total_parking_paid += parking;
    Zeckster.car.total_carwash_paid += carwash;
}

void Zeckster_car_fines_step(const int month) {
    if (Zeckster.car.owned == 0) return;

    if (month == 6 || month == 11) {
        int roll = roll_d100();
        if (roll <= 30) {
            RUB fine = 3'000 + roll_d100() * 100;   // 3 000 – 13 000
            Zeckster_pay(fine);
            Zeckster.car.total_car_spent += fine;
            Zeckster.car.total_fines_paid += fine;
        }
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
        Zeckster.car.tire_change_cost = 0;
        Zeckster.car.tire_change_due_months = 0;
        Zeckster.car.parking_monthly = 0;
        Zeckster.car.carwash_monthly = 0;
    }
}

void Zeckster_car_step(const int year, const int month) {  // Основная функция машины: продажа, покупка, эксплуатация
    Zeckster_car_sell_if_old();

    // Покупка машины в марте, если есть деньги и её нет
    if (Zeckster.car.owned == 0 && month == 3 &&
        Zeckster.money.savings + Zeckster.money.cash > 600'000) {
        Zeckster_choose_car_model();

        // Определяем цену с учётом выбранного типа топлива
        RUB price;
        if (strcmp(Zeckster.car.model, "premium") == 0)      price = 4'000'000;
        else if (strcmp(Zeckster.car.model, "mid") == 0)     price = 1'500'000;
        else                                                  price = 600'000;

        // Если выбрали электро — цена выше на 20%
        if (Zeckster.car.fuel_type == 1) {
            price = (RUB)(price * 1.2);
        }

        // Сначала берём из savings, потом из cash
        RUB from_savings = (price < Zeckster.money.savings) ? price : Zeckster.money.savings;
        RUB from_cash = price - from_savings;
        if (from_cash > Zeckster.money.cash) return;   // не хватает — отмена
        Zeckster.money.savings -= from_savings;
        Zeckster.money.cash -= from_cash;

        // Устанавливаем параметры топлива и ТО с учётом типа
        if (Zeckster.car.fuel_type == 1) {
            Zeckster.car.fuel_cost_per_month = Zeckster.car.fuel_electric_monthly;
            Zeckster.car.maintenance_cost = Zeckster.car.maint_electric_yearly;
            // Разовая установка зарядной станции
            Zeckster_pay(Zeckster.car.charging_station_cost);
            Zeckster.car.total_car_spent += Zeckster.car.charging_station_cost;
        }
        else {
            Zeckster.car.fuel_cost_per_month = Zeckster.car.fuel_gas_monthly;
            Zeckster.car.maintenance_cost = Zeckster.car.maint_gas_yearly;
        }

        Zeckster.car.owned = 1;
        Zeckster.car.age_months = 0;
        Zeckster.car.months_since_purchase = 0;
        Zeckster.car.value = price;
        Zeckster.car.monthly_expenses = Zeckster.car.fuel_cost_per_month;
        Zeckster.car.tech_inspection_due = 24;
        Zeckster.car.tire_change_due_months = 48;   // первая замена через 4 года
        Zeckster.car.has_kasko = (strcmp(Zeckster.car.model, "economy") != 0);
        Zeckster.car.total_car_spent += price;
        Zeckster.car.cars_owned_total++;
    }

    // Эксплуатация
    if (Zeckster.car.owned == 1) {
        Zeckster.car.months_since_purchase++;
        if (Zeckster.car.license_suspended_months > 0) {
            // Машина стоит, расходов нет
            Zeckster.car.license_suspended_months--;
            return;
        }

        // Топливо
        Zeckster_pay(Zeckster.car.fuel_cost_per_month);
        Zeckster.car.total_car_spent += Zeckster.car.fuel_cost_per_month;

        // Прочие траты
        Zeckster_car_tech_inspection_step();
        Zeckster_car_tire_step();
        Zeckster_car_parking_carwash_step();
        Zeckster_car_fines_step(month);
        Zeckster_car_breakdown_check();
        Zeckster_car_insurance_step(month);

        // ТО раз в год в мае
        if (month == 5) {
            Zeckster_pay(Zeckster.car.maintenance_cost);
            Zeckster.car.total_car_spent += Zeckster.car.maintenance_cost;
        }

        // Амортизация: быстрее в первый год, медленнее потом
        RUB old_value = Zeckster.car.value;
        Zeckster.car.age_months++;
        int age_years = Zeckster.car.age_months / 12;
        double rate;
        if (age_years < 1)       rate = 0.981;   // −1.9%/мес первый год
        else if (age_years < 5)  rate = 0.991;   // −0.9%/мес 1–5 лет
        else if (age_years < 10) rate = 0.996;   // −0.4%/мес 5–10 лет
        else                     rate = 0.998;   // −0.2%/мес после 10 лет
        Zeckster.car.value = (RUB)(Zeckster.car.value * rate);
        if (Zeckster.car.value < 50'000) Zeckster.car.value = 50'000;
        Zeckster.car.total_depreciation += old_value - Zeckster.car.value;
    }
}

void Zeckster_take_mortgage(const int year, const int month) {   // Взятие ипотеки в январе 2030 (только для mortgage-стратегии)
    if (Zeckster.use_mortgage == 0) return;
    if (year == 2030 && month == 1 && Zeckster.home.mortgage_debt == 0) {
        Zeckster.home.mortgage_debt = 7'000'000;
        Zeckster.home.market_value = 7'000'000;
        // Аннуитетный платёж: P = S × r / (1 − (1 + r)^(−n))
        double monthly_rate = 0.09 / 12.0;
        int n = 120;
        double payment = Zeckster.home.mortgage_debt * monthly_rate /
            (1 - pow(1 + monthly_rate, -n));
        Zeckster.home.mortgage_payment = (RUB)payment;
        Zeckster.home.mortgage_months_left = n;
        Zeckster.home.type = "Own apartment (mortgage)";
        Zeckster.home.rent_amount = 0;
        Zeckster.home.mortgage_taken = 1;
    }
}

void Zeckster_pay_mortgage() {   // Платёж по ипотеке: проценты на остаток, тело уменьшает долг
    if (Zeckster.home.mortgage_debt > 0) {
        RUB interest = Zeckster.home.mortgage_debt *
            Zeckster.home.mortgage_rate_bp / 10'000 / 12;
        RUB principal = Zeckster.home.mortgage_payment - interest;
        if (principal > Zeckster.home.mortgage_debt) {
            principal = Zeckster.home.mortgage_debt;   // последний платёж
        }
        Zeckster.home.mortgage_debt -= principal;
        Zeckster.home.total_mortgage_interest_paid += interest;
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

void Zeckster_pvz_step(const int year, const int month) { // ПВЗ: открытие, ежемесячная прибыль, случайные инциденты
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


    if (Zeckster.biz.pvz_open == 1) {  // Ежемесячная работа ПВЗ
        Zeckster.biz.pvz_months++;
        int roll = roll_d100();
        RUB extra_cost = (roll <= 10) ? 30'000 : 0;   // 10% шанс штрафа/поломки
        if (extra_cost > 0) Zeckster.biz.pvz_incidents++;

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
        // Доходы и налоги
        Zeckster_salary(year, month);
        Zeckster_ndfl(year, month);
        Zeckster_bonus_step(year, month);
        Zeckster_ndfl(year, month);

        // Еда и качество питания
        Zeckster_food_quality_step(year, month);
        Zeckster_groceries_step(year, month);

        // Жильё
        Zeckster_move(year, month);
        Zeckster_take_mortgage(year, month);
        Zeckster_pay_mortgage();

        // Здоровье, ремонт, машина, кот
        Zeckster_health_step(month);
        Zeckster_repair_step(year, month);
        Zeckster_car_step(year, month);
        Zeckster_cat_mechanics(year, month);

        // Случайные события
        Zeckster_car_crash_event(year, month);
        Zeckster_tax_audit_event(year, month);
        Zeckster_random_events(year, month);
        Zeckster_vacation_step(year, month);

        // Ежемесячные расходы
        Zeckster_food_step(year, month);
        Zeckster_utility_step(year, month);
        Zeckster_housing_step(year, month);

        // Накопления и вклады
        Zeckster_savings_interest();
        Zeckster_savings_step();
        Zeckster_deposit_step(year, month);

        // Инвестиции (ИИС-3)
        Zeckster_invest_step(year, month);
        Zeckster_invest_sell_high(year, month);
        Zeckster_invest_buy_low(year, month);
        Zeckster_invest_rebalance(year, month);
        Zeckster_invest_stop_loss(year, month);
        Zeckster_invest_reinvest_dividends(year, month);
        Zeckster_invest_sweep_deposit(year, month);
        Zeckster_invest_record_year(year, month);

        // Налоговые вычеты и закрытие ИИС
        Zeckster_iis_tax_refund(year, month);
        Zeckster_iis_close(year, month);
        Zeckster_market_crisis(year, month);
        Zeckster_tax_refund(year, month);

        // Бизнес и покупка квартиры за наличные
        Zeckster_pvz_step(year, month);
        Zeckster_buy_flat_cash(year, month);

        // Инфляция
        Zeckster_inflation(year, month);

        // Проверка резерва и счётчик месяцев
        Zeckster_check_reserve();
        Zeckster.months_lived++;

        // Защита от переполнения
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

RUB Zeckster_net_worth() {   // Сумма всех активов: деньги + вклады + ИИС + машина + доля в квартире
    RUB flat_equity = 0;
    if (Zeckster.home.market_value > Zeckster.home.mortgage_debt) {
        flat_equity = Zeckster.home.market_value - Zeckster.home.mortgage_debt;
    }
    RUB deposit_value = Zeckster.deposit.active ? Zeckster.deposit.principal : 0;
    return Zeckster.money.cash
        + Zeckster.money.savings
        + deposit_value
        + Zeckster.invest.iis_value
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
            Zeckster.deposit.start_year, Zeckster.deposit.start_month,
            Zeckster.deposit.months_left);
    }
    printf("\n");
    printf("  Principal:       %llu\n", Zeckster.deposit.principal);
    printf("  Rate:            %llu bp (%llu%%)\n",
        Zeckster.deposit.rate_bp, Zeckster.deposit.rate_bp / 100);
    printf("  Deposits opened: %d\n", Zeckster.deposit.deposits_opened);
    printf("  Total interest:  %llu\n", Zeckster.deposit.total_interest);
    printf("  Max principal:   %llu\n", Zeckster.deposit.max_principal_seen);

    printf("\n[Investments - IIS-3]\n");
    printf("  IIS open:        %d", Zeckster.invest.iis_open);
    if (Zeckster.invest.iis_open) {
        printf(" (since %d/%d, %d months held)",
            Zeckster.invest.iis_open_year, Zeckster.invest.iis_open_month,
            Zeckster.invest.iis_months_held);
    }
    if (Zeckster.invest.iis_closed) printf("  [CLOSED]");
    printf("\n");
    printf("  IIS value:       %llu\n", Zeckster.invest.iis_value);
    printf("  Stocks:          %llu\n", Zeckster.invest.stocks_value);
    printf("  Bonds:           %llu\n", Zeckster.invest.bonds_value);
    printf("  Cash on IIS:     %llu\n", Zeckster.invest.cash_on_iis);
    printf("  Contributed:     %llu\n", Zeckster.invest.iis_contributed_total);
    printf("  Dividends:       %llu\n", Zeckster.invest.dividends_total);
    printf("  Coupons:         %llu\n", Zeckster.invest.coupons_total);
    printf("  Dividend tax:    %llu (13%%)\n", Zeckster.invest.dividend_tax_paid_total);
    printf("  IIS refund (A):  %llu\n", Zeckster.invest.iis_tax_refund_total);
    printf("  IIS saved (B):   %llu\n", Zeckster.invest.iis_tax_saved);
    printf("  Crashes:         %d (lost %llu)\n",
        Zeckster.invest.crashes_experienced, Zeckster.invest.total_crash_losses);
    printf("  Crisis years:    %d\n", Zeckster.invest.crisis_years);
    printf("  Best month:      +%llu\n", Zeckster.invest.best_month_gain);
    printf("  Worst month:     -%llu\n", Zeckster.invest.worst_month_loss);
    printf("  Max portfolio:   %llu\n", Zeckster.invest.max_portfolio_value);
    printf("  Min portfolio:   %llu\n", Zeckster.invest.min_portfolio_value);
    printf("  Max drawdown:    %d bp (%.1f%%)\n",
        Zeckster.invest.max_drawdown_bp,
        Zeckster.invest.max_drawdown_bp / 100.0);
    printf("  Rebalances:      %d (total %llu)\n",
        Zeckster.invest.rebalance_count, Zeckster.invest.total_rebalanced_amount);
    printf("  Stop-loss:       %d triggers (saved %llu)\n",
        Zeckster.invest.stop_loss_triggers, Zeckster.invest.total_stop_loss_saved);
    printf("  Div reinvests:   %d (total %llu)\n",
        Zeckster.invest.dividend_reinvest_count, Zeckster.invest.dividend_reinvest_total);
    printf("  Deposit sweeps:  %d (total %llu)\n",
        Zeckster.invest.deposit_sweep_count, Zeckster.invest.deposit_sweep_total);
    printf("  Bull/Bear/Flat:  %d / %d / %d months\n",
        Zeckster.invest.bull_months, Zeckster.invest.bear_months,
        Zeckster.invest.flat_months);

    printf("\n[Job]\n");
    printf("  Salary:          %llu (%s)\n", Zeckster.job.salary, Zeckster.job.title);
    printf("  Salary2:         %llu (%s)\n", Zeckster.job.salary2, Zeckster.job.title2);
    printf("  Bonus rate:      %llu bp\n", Zeckster.job.bonus_rate_bp);
    printf("  Last bonus:      %llu\n", Zeckster.job.bonus);

    printf("\n[Tax]\n");
    printf("  NDFL total:      %llu\n", Zeckster.job.ndfl_paid_total);
    printf("  Property ded.:   %llu / %llu\n",
        Zeckster.job.property_deduction_used, Zeckster.job.property_deduction_cap);
    printf("  Mortgage ded.:   %llu / %llu\n",
        Zeckster.job.mortgage_deduction_used, Zeckster.job.mortgage_deduction_cap);
    printf("  Social ded.:     %llu total, %llu this year\n",
        Zeckster.job.social_deduction_used_total,
        Zeckster.job.social_deduction_used_year);

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
    printf("  Repairs:         %d times (%d major), %llu total\n",
        Zeckster.home.repairs_count, Zeckster.home.major_repairs_count,
        Zeckster.home.total_repair_paid);
    printf("  Rent paid:       %llu\n", Zeckster.home.total_rent_paid);
    printf("  Mortgage paid:   %llu (interest %llu)\n",
        Zeckster.home.total_mortgage_paid,
        Zeckster.home.total_mortgage_interest_paid);
    printf("  Utility paid:    %llu\n", Zeckster.home.total_utility_paid);

    printf("\n[Living]\n");
    printf("  Food base:       %llu\n", Zeckster.life.food_base);
    printf("  Dining out:      %llu\n", Zeckster.life.dining_out);
    printf("  Healthy extra:   %llu\n", Zeckster.life.healthy_food_extra);
    printf("  Food quality:    %d (0=eco, 1=normal, 2=healthy)\n",
        Zeckster.life.food_quality);
    printf("  Food total:      %llu\n", Zeckster.life.total_food_paid);
    printf("  Dining total:    %llu\n", Zeckster.life.total_dining_out_paid);
    printf("  Groceries total: %llu\n", Zeckster.life.total_groceries_paid);

    printf("\n[Vacation]\n");
    printf("  Total vacations:  %d\n", Zeckster.vacation.total_vacations);
    printf("  Total spent:      %llu\n", Zeckster.vacation.total_spent);
    printf("  Quality:          %d (0=eco, 1=normal, 2=premium)\n",
        Zeckster.vacation.quality);
    printf("  Base cost:        %llu\n", Zeckster.vacation.base_cost);

    printf("\n[Transport]\n");
    printf("  Owned:           %d\n", Zeckster.car.owned);
    printf("  Model:           %s\n", Zeckster.car.model);
    printf("  Fuel type:       %s\n",
        Zeckster.car.fuel_type == 1 ? "electric" : "petrol");
    printf("  Value:           %llu\n", Zeckster.car.value);
    printf("  Age:             %d months\n", Zeckster.car.age_months);
    printf("  Fuel monthly:    %llu\n", Zeckster.car.fuel_cost_per_month);
    printf("  Maintenance:     %llu/year\n", Zeckster.car.maintenance_cost);
    printf("  Breakdowns:      minor %d, major %d\n",
        Zeckster.car.breakdowns_minor, Zeckster.car.breakdowns_major);
    printf("  Cars owned:      %d\n", Zeckster.car.cars_owned_total);
    printf("  Total car spent: %llu\n", Zeckster.car.total_car_spent);
    printf("  Depreciation:    %llu\n", Zeckster.car.total_depreciation);
    printf("  Tires total:     %llu\n", Zeckster.car.total_tire_paid);
    printf("  Parking total:   %llu\n", Zeckster.car.total_parking_paid);
    printf("  Carwash total:   %llu\n", Zeckster.car.total_carwash_paid);
    printf("  Fines total:     %llu\n", Zeckster.car.total_fines_paid);

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
            Zeckster.biz.pvz_open_year, Zeckster.biz.pvz_open_month,
            Zeckster.biz.pvz_months);
    }
    printf("\n");
    printf("  PVZ invested:    %llu\n", Zeckster.biz.pvz_invested);
    printf("  PVZ net total:   %llu\n", Zeckster.biz.total_pvz_net);
    printf("  PVZ incidents:   %d\n", Zeckster.biz.pvz_incidents);

    printf("\n[Health]\n");
    printf("  Total spent:     %llu\n", Zeckster.health.total_spent);
    printf("  Months sick:     %d (serious %d)\n",
        Zeckster.health.months_sick, Zeckster.health.months_seriously_sick);
    printf("  Illness events:  %d (serious %d)\n",
        Zeckster.health.total_illness_events, Zeckster.health.total_serious_events);

    printf("\n[Events]\n");
    printf("  Fired:           %d\n", Zeckster.events.fired);
    printf("  Inheritance:     %d\n", Zeckster.events.inheritance);
    printf("  Car crash:       %d (severe %d)\n",
        Zeckster.events.car_crash, Zeckster.events.car_crash_severe);
    printf("  Promotion:       %d\n", Zeckster.events.promotion);
    printf("  Food jump:       %d\n", Zeckster.events.food_jump);
    printf("  Tax audit:       %d\n", Zeckster.events.tax_audit);
    printf("  Vacations:       %d\n", Zeckster.events.vacations_taken);
    if (Zeckster.biz.pvz_open) {
        printf("  PVZ opened:      %d/%d\n",
            Zeckster.biz.pvz_open_year, Zeckster.biz.pvz_open_month);
    }
    if (Zeckster.use_mortgage == 0) {
        if (Zeckster.events.flat_bought_year > 0) {
            printf("  Flat bought:     %d/%d (cash)\n",
                Zeckster.events.flat_bought_year,
                Zeckster.events.flat_bought_month);
        }
    }
    else {
        printf("  Flat:            mortgage since 2030/1\n");
    }

    printf("\n[Net worth]:       %llu\n", Zeckster_net_worth());
}

int evaluate_life_quality() {  // Оценка качества жизни: 1 — плохо, 2 — средне, 3 — хорошо
    RUB nw = Zeckster_net_worth();
    if (nw > 12'000'000 && Zeckster.biz.pvz_open) return 3;
    if (nw > 5'000'000) return 2;
    return 1;
}

void compare_mortgage_vs_saving() {  // Сравнение стратегий: ипотека vs копить (одинаковый seed)
    printf("\n=== Mortgage vs Saving (same seed) ===\n");
    const int SEED = 42;
    const int YEARS = 15;
    RUB result[2];
    RUB cash_savings[2];
    RUB flat_equity_arr[2];
    int bankrupt[2];

    for (int strategy = 0; strategy < 2; strategy++) {
        Zeckster_init(strategy);
        srand(SEED);   // одинаковые случайные события
        simulation(YEARS);
        result[strategy] = Zeckster_net_worth();
        cash_savings[strategy] = Zeckster.money.cash + Zeckster.money.savings;
        flat_equity_arr[strategy] = Zeckster.home.market_value > Zeckster.home.mortgage_debt
            ? Zeckster.home.market_value - Zeckster.home.mortgage_debt : 0;
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

    if (result[1] > result[0])
        printf("Conclusion: MORTGAGE wins by %llu\n", result[1] - result[0]);
    else if (result[0] > result[1])
        printf("Conclusion: SAVING wins by %llu\n", result[0] - result[1]);
    else
        printf("Conclusion: TIE\n");
}

void ab_test() {  // A/B тест: 10 прогонов с разными seed
    printf("\n=== A/B test: 10 pairs, different seeds ===\n");
    const int N = 10;
    RUB total_saving = 0, total_mortgage = 0;
    int wins_saving = 0, wins_mortgage = 0;

    for (int seed = 0; seed < N; seed++) {
        RUB r[2];
        for (int strategy = 0; strategy < 2; strategy++) {
            Zeckster_init(strategy);
            srand(seed);   // одинаковый seed для пары
            simulation(15);
            r[strategy] = Zeckster_net_worth();
        }
        printf("Seed %2d: saving = %llu, mortgage = %llu\n", seed, r[0], r[1]);
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