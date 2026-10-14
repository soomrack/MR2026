#include <stdio.h>
#include <stdlib.h>

// RUB и USD — целые денежные единицы; bp — сотые доли процента.
using RUB = long long int;
using USD = long long int;

// Текущие год и месяц меняются после обработки всех событий месяца.
int cur_year = 2026;
int cur_month = 9;

struct Inflation {
    RUB food;
    RUB utilities;
    RUB pet;
    RUB car;
    RUB medicine;
    RUB rent;
    RUB rental_income;
    RUB salary;
    RUB tax;
    RUB clothes;
    RUB home_repair;
    RUB communication;
    RUB leisure;
    RUB insurance;
    RUB education;
    RUB household;
};

struct Car {
    RUB repair;
    RUB fuel;
    RUB service;
    RUB credit;
    RUB credit_payment;
    RUB credit_rate_bp;
    int credit_months;
    RUB value;
    RUB replacement_price;
    RUB total_costs;
    RUB interest_paid;
    int age_months;
    int unavailable_months;
    int accidents;
    int replacements;
};

struct Tax {
    RUB car_tax;
    RUB home_tax;
    RUB rental_home_tax;
    RUB land_tax;
    RUB salary_rate_bp;
    RUB side_rate_bp;
    RUB rental_rate_bp;
    RUB deposit_rate_bp;
    RUB deposit_exempt_income;
    RUB salary_due;
    RUB side_due;
    RUB rental_due;
    RUB year_salary_tax;
    RUB year_medical_spent;
    RUB year_deposit_income;
    RUB standard_refund;
    RUB mortgage_refund_limit;
    RUB mortgage_refund_used;
    RUB paid_income_tax;
    RUB paid_property_tax;
    RUB paid_deposit_tax;
    RUB refund_received;
};

struct Family {
    RUB food;
    RUB dog;
    RUB clothes;
    int dog_life_months;
    int dog_age_months;
    RUB household;
    RUB family_support;
    RUB gifts;
    RUB dog_supplies;
    RUB dog_checkup;
    RUB food_spent;
    RUB dog_spent;
    RUB family_spent;
};

struct medicine {
    RUB remedy;
    RUB teeth;
    RUB disease;
    RUB sport;
    RUB checkup;
    RUB glasses;
    RUB spent;
    int sick_days;
    int illnesses;
    int sick_months;
    bool serious_illness;
};

struct Home {
    RUB utilities;
    RUB repair;
    RUB mortgage;
    RUB rent;
    RUB rental;
    RUB mortgage_payment;
    RUB mortgage_rate_bp;
    int mortgage_months;
    RUB flat_value;
    RUB rental_flat_value;
    RUB land_value;
    RUB rental_repair_budget;
    RUB tenant_search_cost;
    RUB rental_received;
    RUB rental_spent;
    RUB mortgage_interest_paid;
    int vacant_months;
    int vacant_total;
    int tenant_changes;
    bool new_tenant;
    bool refinanced;
};

struct Bank {
    RUB account_RUB;
    USD account_USD;
    RUB check;
    RUB credit;
    RUB credit_payment;
    RUB credit_rate_bp;
    int credit_months;
    RUB deposit_rate_bp;
    RUB usd_deposit_rate_bp;
    RUB card_rate_bp;
    RUB usd_rate;
    RUB consumer_interest_paid;
    RUB interest_received;
    RUB usd_interest_remainder;
};

struct Person {
    RUB cash;
    RUB salary; // Зарплата до удержания налогов.
    RUB rental_income;
};

struct Inflation infl;
struct Car alice_car;
struct Tax alice_taxes;
struct Family alice_family;
struct medicine alice_meds;
struct Home alice_home;
struct Bank alice_bank;
struct Person alice;

RUB total_income = 0;
RUB total_outflow = 0;
RUB total_refund = 0;
RUB total_interest = 0;

RUB year_mortgage_interest = 0;
RUB year_car_interest = 0;
RUB year_consumer_interest = 0;

// Работа влияет не только на доход, но и на транспорт и обучение.
struct Work {
    RUB side_income;
    RUB side_cost;
    RUB commute;
    RUB lunch;
    RUB course;
    RUB equipment;
    RUB unemployment_benefit;
    RUB side_received;
    RUB spent;
    int unemployed_months;
    int months_without_work;
    int job_losses;
    int side_jobs;
    int course_month;
    int equipment_age;
    bool employed;
    bool side_job_this_month;
};

// Отпуск и развлечения планируются отдельно от обязательных расходов.
struct Leisure {
    RUB entertainment;
    RUB hobby;
    RUB books;
    RUB travel_budget;
    RUB weekend_trip;
    RUB holiday_budget;
    RUB spent;
    int vacation_month;
    int vacations;
    int trips;
    bool vacation_this_month;
};

// Небольшие регулярные платежи тоже влияют на бюджет за десять лет.
struct Communication {
    RUB mobile;
    RUB internet;
    RUB subscriptions;
    RUB phone_price;
    RUB computer_price;
    RUB spent;
    int phone_age;
    int computer_age;
};

// Полисы оплачиваются раз в год, а страховой случай требует отдельной проверки.
struct Insurance {
    RUB car_policy;
    RUB home_policy;
    RUB travel_policy;
    RUB car_deductible;
    RUB home_deductible;
    RUB paid;
    RUB compensation;
    int car_cases;
    int home_cases;
    bool car_active;
    bool home_active;
};

// Редкие семейные события не входят в обычную продуктовую корзину.
struct LifeEvents {
    RUB birthday;
    RUB family_visit;
    RUB celebration;
    RUB documents;
    RUB lost_items;
    RUB spent;
    int celebrations;
    int lost_items_count;
};

// Здесь только итоги модели, без больших массивов помесячных отчётов.
struct Budget {
    RUB initial_capital;
    RUB month_income_before;
    RUB month_outflow_before;
    RUB year_income_before;
    RUB year_outflow_before;
    RUB loan_payments;
    RUB loan_principal;
    RUB prepayments;
    RUB card_payments;
    RUB card_interest;
    RUB refinance_fees;
    RUB savings_used;
    RUB largest_month_outflow;
    RUB lowest_savings;
    RUB highest_debt;
    int months;
    int deficit_months;
    int debt_free_month;
    int mortgage_closed_month;
    bool limit_reached;
};

struct Work alice_work;
struct Leisure alice_leisure;
struct Communication alice_communication;
struct Insurance alice_insurance;
struct LifeEvents alice_events;
struct Budget budget;

const int MIN_SIMULATION_MONTHS = 120;
const int MAX_SIMULATION_MONTHS = 600;
unsigned int simulation_seed = 20260901u;

void alice_extra_init();
void alice_extra_inflation();
RUB alice_debts();
RUB alice_net_worth();
RUB loan_payment(RUB balance, RUB rate_bp, int months);
void add_salary_income(RUB sum);
void add_rental_income(RUB sum);

// --- вспомогательные: случайность и рост цен ---

// Выбираем случайное число в заданных границах.
int rand_range(int lo, int hi)
{
    if (hi <= lo)
    {
        return lo;
    }
    return lo + rand() % (hi - lo + 1);
}

// Проверяем вероятность события в процентах.
bool chance(int percent)
{
    return rand() % 100 < percent;
}

// Повышаем базовую цену на один месяц инфляции.
RUB inflate(RUB value, RUB yearly_bp)
{
    if (value <= 0)
    {
        return value;
    }
    return value + value * yearly_bp / 120000;
}

// Применяем годовую индексацию к сумме.
RUB inflate_year(RUB value, RUB yearly_bp)
{
    if (value <= 0)
    {
        return value;
    }
    return value + value * yearly_bp / 10000;
}

// Полученные деньги добавляем в наличные и общий доход.
void add_income(RUB sum)
{
    alice.cash += sum;
    total_income += sum;
}

// Оплачиваем расход и увеличиваем общий отток.
void add_outflow(RUB sum)
{
    alice.cash -= sum;
    total_outflow += sum;
}

// Задаём исходное состояние Алисы и обнуляем прошлый запуск.
void alice_init()
{
    alice_car = {};
    alice_taxes = {};
    alice_family = {};
    alice_meds = {};
    alice_home = {};
    alice_bank = {};
    alice = {};
    cur_year = 2026;
    cur_month = 9;
    total_income = 0;
    total_outflow = 0;
    total_refund = 0;
    total_interest = 0;
    year_mortgage_interest = 0;
    year_car_interest = 0;
    year_consumer_interest = 0;
    infl.food = 1000;
    infl.utilities = 1200;
    infl.pet = 900;
    infl.car = 1100;
    infl.medicine = 1300;
    infl.rent = 800;
    infl.rental_income = 700;
    infl.salary = 600;
    infl.tax = 700;
    infl.clothes = 850;
    infl.home_repair = 950;
    alice_car.repair = 800;
    alice_car.fuel = 5000;
    alice_car.service = 6000;
    alice_car.credit = 400'000;
    alice_car.credit_payment = 9000;
    alice_car.credit_rate_bp = 1200;
    alice_car.credit_months = 60;
    alice_taxes.car_tax = 6000;
    alice_taxes.home_tax = 6000;
    alice_taxes.land_tax = 1500;
    alice_family.food = 15000;
    alice_family.dog = 3500;
    alice_family.clothes = 1200;
    alice_family.dog_life_months = 96;
    alice_meds.remedy = 1000;
    alice_meds.teeth = 4000;
    alice_meds.disease = 800;
    alice_home.utilities = 6000;
    alice_home.repair = 1000;
    alice_home.mortgage = 2000'000;
    alice_home.rent = 0;
    alice_home.rental = 1000;
    alice_home.mortgage_payment = 24500;
    alice_home.mortgage_rate_bp = 800;
    alice_home.mortgage_months = 120;
    alice_home.flat_value = 2'300'000;
    alice_bank.account_RUB = 300'000;
    alice_bank.account_USD = 2000;
    alice_bank.check = 0;
    alice_bank.credit = 100'000;
    alice_bank.credit_payment = 3800;
    alice_bank.credit_rate_bp = 2000;
    alice_bank.credit_months = 36;
    alice.cash = 20'000;
    alice.salary = 80'000;
    alice.rental_income = 20'000;
    alice_extra_init();
}

// --- доходы ---

// Получаем зарплату с поправкой на больничный и возможные премии.
void alice_salary()
{
    if (not alice_work.employed)
    {
        add_income(alice_work.unemployment_benefit);
        ++alice_work.months_without_work;
        return;
    }
    RUB salary = alice.salary;
    RUB sick_reduction = salary * alice_meds.sick_days / 30 * 20 / 100;
    salary -= sick_reduction;
    add_salary_income(salary);
    if (cur_month == 3 or cur_month == 6 or cur_month == 9 or cur_month == 12)
    {
        if (chance(60))
        {
            RUB bonus = rand_range(10'000, 40'000);
            add_salary_income(bonus);
        }
    }
    if (cur_month == 12 and chance(70))
    {
        add_salary_income(alice.salary / 2);
    }
}

// Получаем арендную плату только от заселённой квартиры.
void alice_rental_income()
{
    if (alice_home.vacant_months > 0)
    {
        ++alice_home.vacant_total;
        return;
    }
    add_rental_income(alice.rental_income);
    alice_home.rental_received += alice.rental_income;
}

// Начисляем проценты на накопления в каждой валюте.
void alice_bank_income()
{
    RUB rub_interest = alice_bank.account_RUB * alice_bank.deposit_rate_bp / 120000;
    alice_bank.account_RUB += rub_interest;
    total_income += rub_interest;
    alice_bank.interest_received += rub_interest;
    alice_taxes.year_deposit_income += rub_interest;
    RUB usd_numerator = alice_bank.account_USD * alice_bank.usd_deposit_rate_bp;
    usd_numerator += alice_bank.usd_interest_remainder;
    USD usd_interest = usd_numerator / 120000;
    alice_bank.usd_interest_remainder = usd_numerator % 120000;
    alice_bank.account_USD += usd_interest;
}

// --- долговая карта / овердрафт ---

// Начисляем проценты на непогашенный долг по карте.
void alice_check_interest()
{
    if (alice_bank.check <= 0)
    {
        return;
    }
    RUB interest = alice_bank.check * alice_bank.card_rate_bp / 120000;
    alice_bank.check += interest;
    total_interest += interest;
    budget.card_interest += interest;
}

// Из имеющихся наличных вносим минимальный платёж по карте.
void alice_check_payment()
{
    if (alice.cash < 0)
    {
        alice_bank.check += -alice.cash;
        alice.cash = 0;
    }
    if (alice_bank.check <= 0)
    {
        return;
    }
    RUB min_pay = alice_bank.check * 5 / 100;
    if (min_pay < 1000)
    {
        min_pay = 1000;
    }
    if (min_pay > alice_bank.check)
    {
        min_pay = alice_bank.check;
    }
    if (alice.cash >= min_pay)
    {
        alice.cash -= min_pay;
        alice_bank.check -= min_pay;
        budget.card_payments += min_pay;
    }
    else
    {
        budget.card_payments += alice.cash;
        alice_bank.check -= alice.cash;
        alice.cash = 0;
    }
}

// --- кредиты (детальная тема) ---

// Платёж сначала покрывает проценты, затем уменьшает тело кредита.
void pay_loan(
    RUB &balance,
    RUB payment,
    RUB rate_bp,
    int &months,
    RUB &year_interest
)
{
    if (balance <= 0)
    {
        balance = 0;
        months = 0;
        return;
    }
    RUB interest = balance * rate_bp / 120000;
    RUB total_due = balance + interest;
    RUB due = payment;
    if (months <= 1 or due <= interest)
    {
        due = total_due;
    }
    if (due > total_due)
    {
        due = total_due;
    }
    add_outflow(due);
    budget.loan_payments += due;
    budget.loan_principal += due - interest;
    balance = total_due - due;
    year_interest += interest;
    total_interest += interest;
    if (balance <= 0)
    {
        balance = 0;
        months = 0;
    }
    else if (months > 0)
    {
        --months;
    }
}

// Платим ипотеку и запоминаем стоимость её обслуживания.
void alice_mortgage()
{
    RUB interest_before = year_mortgage_interest;
    pay_loan(
        alice_home.mortgage,
        alice_home.mortgage_payment,
        alice_home.mortgage_rate_bp,
        alice_home.mortgage_months,
        year_mortgage_interest
    );
    alice_home.mortgage_interest_paid += year_mortgage_interest - interest_before;
}

// Платим автокредит и считаем его проценты.
void alice_car_credit()
{
    RUB interest_before = year_car_interest;
    pay_loan(
        alice_car.credit,
        alice_car.credit_payment,
        alice_car.credit_rate_bp,
        alice_car.credit_months,
        year_car_interest
    );
    alice_car.interest_paid += year_car_interest - interest_before;
}

// Платим потребкредит и считаем его проценты.
void alice_consumer_credit()
{
    RUB interest_before = year_consumer_interest;
    pay_loan(
        alice_bank.credit,
        alice_bank.credit_payment,
        alice_bank.credit_rate_bp,
        alice_bank.credit_months,
        year_consumer_interest
    );
    alice_bank.consumer_interest_paid += year_consumer_interest - interest_before;
}

// --- расходы со случайными и сезонными тратами ---

// Оплачиваем продуктовую корзину, кафе и возможных гостей.
void alice_food()
{
    RUB sum = alice_family.food;
    if (chance(40))
    {
        sum += rand_range(500, 2500);
    }
    if (chance(8))
    {
        sum += rand_range(2000, 6000);
    }
    add_outflow(sum);
    alice_family.food_spent += sum;
}

// Содержим собаку в пределах её срока жизни в модели.
void alice_dog()
{
    if (alice_family.dog_life_months <= 0)
    {
        return;
    }
    RUB sum = alice_family.dog;
    if (chance(15))
    {
        sum += rand_range(800, 3000);
    }
    if (chance(3))
    {
        sum += rand_range(5000, 20000);
    }
    add_outflow(sum);
    alice_family.dog_spent += sum;
    --alice_family.dog_life_months;
    ++alice_family.dog_age_months;
}

// Покупаем одежду, учитывая сезонное обновление гардероба.
void alice_clothes()
{
    RUB sum = alice_family.clothes;
    if (cur_month == 3 or cur_month == 9)
    {
        sum += rand_range(4000, 15000);
    }
    add_outflow(sum);
    alice_family.family_spent += sum;
}

// Оплачиваем лекарства, стоматолога и лечение болезни.
void alice_medicine()
{
    RUB sum = alice_meds.remedy + alice_meds.disease;
    if (alice_meds.sick_days > 0)
    {
        sum += rand_range(500, 2500);
    }
    if (cur_month == 6 or cur_month == 12)
    {
        sum += alice_meds.teeth * rand_range(1, 3);
    }
    if (alice_meds.serious_illness)
    {
        sum += rand_range(10000, 50000);
    }
    add_outflow(sum);
    alice_meds.spent += sum;
    alice_taxes.year_medical_spent += sum;
}

// Зимой платим больше за отопление, летом коммуналка дешевле.
void alice_utilities()
{
    RUB sum = alice_home.utilities;
    if (cur_month >= 10 or cur_month <= 4)
    {
        sum = sum * 130 / 100;
    }
    else
    {
        sum = sum * 85 / 100;
    }
    add_outflow(sum);
}

// Покупаем хозяйственные мелочи и устраняем поломки.
void alice_home_repair()
{
    RUB sum = alice_home.repair;
    if (chance(10))
    {
        sum += rand_range(2000, 15000);
    }
    if (chance(2))
    {
        sum += rand_range(20000, 80000);
    }
    add_outflow(sum);
}

// Собственная аренда оплачивается, если Алиса её использует.
void alice_rent()
{
    if (alice_home.rent > 0)
    {
        add_outflow(alice_home.rent);
    }
}

// Сдаваемая квартира требует расходов даже без арендатора.
void alice_rental_expense()
{
    if (alice_home.rental > 0)
    {
        add_outflow(alice_home.rental);
        alice_home.rental_spent += alice_home.rental;
    }
}

// Оплачиваем топливо, мелкий ремонт, ТО и сезонную резину.
void alice_car_costs()
{
    if (alice_car.unavailable_months > 0)
    {
        return;
    }
    RUB sum = alice_car.fuel + alice_car.repair;
    if (chance(30))
    {
        sum += rand_range(1000, 4000);
    }
    if (cur_month == 4 or cur_month == 10)
    {
        sum += alice_car.service;
    }
    if (cur_month == 4 or cur_month == 10)
    {
        sum += rand_range(6000, 12000);
    }
    if (chance(8))
    {
        sum += rand_range(3000, 25000);
    }
    if (chance(2))
    {
        sum += rand_range(15000, 60000);
    }
    add_outflow(sum);
    alice_car.total_costs += sum;
}

// В ноябре платим транспортный, имущественный и земельный налоги.
void alice_tax()
{
    if (cur_month != 11)
    {
        return;
    }
    add_outflow(alice_taxes.car_tax);
    add_outflow(alice_taxes.home_tax);
    add_outflow(alice_taxes.rental_home_tax);
    add_outflow(alice_taxes.land_tax);
    alice_taxes.paid_property_tax += alice_taxes.car_tax;
    alice_taxes.paid_property_tax += alice_taxes.home_tax;
    alice_taxes.paid_property_tax += alice_taxes.rental_home_tax;
    alice_taxes.paid_property_tax += alice_taxes.land_tax;
}

// Налог на проценты считаем после всех начислений календарного года.
void alice_deposit_tax()
{
    if (cur_month != 12)
    {
        return;
    }
    RUB deposit_base = alice_taxes.year_deposit_income;
    deposit_base -= alice_taxes.deposit_exempt_income;
    if (deposit_base > 0)
    {
        RUB tax = deposit_base * alice_taxes.deposit_rate_bp / 10000;
        add_outflow(tax);
        alice_taxes.paid_deposit_tax += tax;
    }
    alice_taxes.year_deposit_income = 0;
}

// Возврат ограничиваем уплаченным налогом и общим лимитом процентов.
void alice_tax_refund()
{
    if (cur_month != 12)
    {
        return;
    }
    RUB refund = alice_taxes.year_medical_spent * 13 / 100;
    if (refund > alice_taxes.standard_refund)
    {
        refund = alice_taxes.standard_refund;
    }
    RUB mortgage_refund = year_mortgage_interest * 13 / 100;
    RUB available = alice_taxes.mortgage_refund_limit;
    available -= alice_taxes.mortgage_refund_used;
    if (mortgage_refund > available)
    {
        mortgage_refund = available;
    }
    if (refund > alice_taxes.year_salary_tax)
    {
        refund = alice_taxes.year_salary_tax;
    }
    RUB available_tax = alice_taxes.year_salary_tax - refund;
    if (mortgage_refund > available_tax)
    {
        mortgage_refund = available_tax;
    }
    alice_taxes.mortgage_refund_used += mortgage_refund;
    refund += mortgage_refund;
    add_income(refund);
    total_refund += refund;
    alice_taxes.refund_received += refund;
    alice_taxes.year_salary_tax = 0;
    alice_taxes.year_medical_spent = 0;
    year_mortgage_interest = 0;
    year_car_interest = 0;
    year_consumer_interest = 0;
}

// --- управление наличными и долгами ---

// Сохраняем наличный буфер, остальное направляем на карту и вклад.
void alice_save_excess()
{
    const RUB cash_buffer = 50'000;
    if (alice.cash <= cash_buffer)
    {
        return;
    }
    RUB free_money = alice.cash - cash_buffer;
    if (alice_bank.check > 0)
    {
        RUB pay_check = free_money;
        if (pay_check > alice_bank.check)
        {
            pay_check = alice_bank.check;
        }
        alice_bank.check -= pay_check;
        budget.card_payments += pay_check;
        alice.cash -= pay_check;
        free_money -= pay_check;
    }
    if (free_money > 0)
    {
        alice_bank.account_RUB += free_money;
        alice.cash -= free_money;
    }
}

// При нехватке наличных используем вклад, затем кредитную карту.
void alice_cover_deficit()
{
    if (alice.cash >= 0)
    {
        return;
    }
    RUB need = -alice.cash;
    if (alice_bank.account_RUB > 0)
    {
        RUB take = need;
        if (take > alice_bank.account_RUB)
        {
            take = alice_bank.account_RUB;
        }
        alice_bank.account_RUB -= take;
        budget.savings_used += take;
        alice.cash += take;
        need -= take;
    }
    if (need > 0)
    {
        alice_bank.check += need;
        alice.cash = 0;
    }
}

// Сверх резерва гасим карту, потребкредит, автокредит и ипотеку.
void alice_prepay_debts()
{
    const RUB savings_reserve = 100'000;
    if (alice_bank.account_RUB <= savings_reserve)
    {
        return;
    }
    RUB free = alice_bank.account_RUB - savings_reserve;
    if (free <= 0)
    {
        return;
    }
    RUB pay = free;
    if (pay > alice_bank.credit)
    {
        pay = alice_bank.credit;
    }
    alice_bank.credit -= pay;
    budget.prepayments += pay;
    alice_bank.account_RUB -= pay;
    free -= pay;
    if (free > 0)
    {
        pay = free;
        if (pay > alice_car.credit)
        {
            pay = alice_car.credit;
        }
        alice_car.credit -= pay;
        budget.prepayments += pay;
        alice_bank.account_RUB -= pay;
        free -= pay;
    }
    if (free > 50'000)
    {
        pay = free - 50'000;
        if (pay > alice_home.mortgage)
        {
            pay = alice_home.mortgage;
        }
        alice_home.mortgage -= pay;
        budget.prepayments += pay;
        alice_bank.account_RUB -= pay;
    }
}

// Обновляем базовые цены для следующего месяца.
void apply_inflation()
{
    alice_family.food = inflate(alice_family.food, infl.food);
    alice_family.clothes = inflate(alice_family.clothes, infl.clothes);
    alice_family.dog = inflate(alice_family.dog, infl.pet);
    alice_meds.remedy = inflate(alice_meds.remedy, infl.medicine);
    alice_meds.teeth = inflate(alice_meds.teeth, infl.medicine);
    alice_meds.disease = inflate(alice_meds.disease, infl.medicine);
    alice_home.utilities = inflate(alice_home.utilities, infl.utilities);
    alice_home.repair = inflate(alice_home.repair, infl.home_repair);
    alice_home.rent = inflate(alice_home.rent, infl.rent);
    alice_home.rental = inflate(alice_home.rental, infl.rent);
    alice.rental_income = inflate(alice.rental_income, infl.rental_income);
    alice_car.repair = inflate(alice_car.repair, infl.car);
    alice_car.fuel = inflate(alice_car.fuel, infl.car);
    alice_car.service = inflate(alice_car.service, infl.car);
    alice_taxes.car_tax = inflate(alice_taxes.car_tax, infl.tax);
    alice_taxes.home_tax = inflate(alice_taxes.home_tax, infl.tax);
    alice_taxes.rental_home_tax = inflate(alice_taxes.rental_home_tax, infl.tax);
    alice_taxes.land_tax = inflate(alice_taxes.land_tax, infl.tax);
    alice_extra_inflation();
}

// Дополнительные суммы заданы в текущих рублях на сентябрь 2026 года.
void alice_extra_init()
{
    alice_work = {};
    alice_leisure = {};
    alice_communication = {};
    alice_insurance = {};
    alice_events = {};
    budget = {};
    infl.communication = 600;
    infl.leisure = 800;
    infl.insurance = 700;
    infl.education = 900;
    infl.household = 800;
    alice_car.value = 650000;
    alice_car.replacement_price = 900000;
    alice_car.age_months = 36;
    alice_family.dog_age_months = 24;
    alice_family.household = 1200;
    alice_family.family_support = 1000;
    alice_family.gifts = 3000;
    alice_family.dog_supplies = 1500;
    alice_family.dog_checkup = 2500;
    alice_meds.sport = 1500;
    alice_meds.checkup = 5000;
    alice_meds.glasses = 6000;
    // Вторая квартира и участок уже принадлежат Алисе на старте.
    alice_home.rental_flat_value = 1800000;
    alice_home.land_value = 200000;
    alice_taxes.rental_home_tax = 4000;
    alice_home.rental_repair_budget = 30000;
    alice_home.tenant_search_cost = 5000;
    alice_bank.deposit_rate_bp = 700;
    alice_bank.usd_deposit_rate_bp = 200;
    alice_bank.card_rate_bp = 3000;
    alice_bank.usd_rate = 90;
    alice_taxes.salary_rate_bp = 1300;
    alice_taxes.side_rate_bp = 1300;
    alice_taxes.rental_rate_bp = 1300;
    alice_taxes.deposit_rate_bp = 1300;
    alice_taxes.deposit_exempt_income = 160000;
    alice_taxes.standard_refund = 15600;
    alice_taxes.mortgage_refund_limit = 390000;
    alice_work.side_income = 7000;
    alice_work.side_cost = 800;
    alice_work.commute = 2000;
    alice_work.lunch = 1500;
    alice_work.course = 12000;
    alice_work.equipment = 10000;
    alice_work.unemployment_benefit = 8000;
    alice_work.course_month = 2;
    alice_work.employed = true;
    alice_leisure.entertainment = 1000;
    alice_leisure.hobby = 800;
    alice_leisure.books = 400;
    alice_leisure.travel_budget = 45000;
    alice_leisure.weekend_trip = 4000;
    alice_leisure.holiday_budget = 5000;
    alice_leisure.vacation_month = 7;
    alice_communication.mobile = 500;
    alice_communication.internet = 700;
    alice_communication.subscriptions = 400;
    alice_communication.phone_price = 25000;
    alice_communication.computer_price = 55000;
    alice_communication.phone_age = 12;
    alice_communication.computer_age = 24;
    alice_insurance.car_policy = 12000;
    alice_insurance.home_policy = 5000;
    alice_insurance.travel_policy = 1500;
    alice_insurance.car_deductible = 5000;
    alice_insurance.home_deductible = 10000;
    alice_insurance.car_active = true;
    alice_insurance.home_active = true;
    alice_events.birthday = 4000;
    alice_events.family_visit = 3000;
    alice_events.celebration = 12000;
    alice_events.documents = 2000;
    alice_events.lost_items = 3000;
    budget.debt_free_month = -1;
    budget.mortgage_closed_month = -1;
    budget.initial_capital = alice_net_worth();
    budget.lowest_savings = alice_bank.account_RUB;
    budget.highest_debt = alice_debts();
    alice_home.mortgage_payment = loan_payment(
        alice_home.mortgage, alice_home.mortgage_rate_bp,
        alice_home.mortgage_months);
    alice_car.credit_payment = loan_payment(
        alice_car.credit, alice_car.credit_rate_bp, alice_car.credit_months);
    alice_bank.credit_payment = loan_payment(
        alice_bank.credit, alice_bank.credit_rate_bp, alice_bank.credit_months);
}

// Сумма аннуитетного платежа зависит от долга, ставки и оставшегося срока.
RUB loan_payment(RUB balance, RUB rate_bp, int months)
{
    if (balance <= 0)
    {
        return 0;
    }
    if (months <= 0)
    {
        return balance;
    }
    if (rate_bp <= 0)
    {
        return balance / months + (balance % months != 0);
    }
    long double rate = static_cast<long double>(rate_bp) / 120000.0L;
    long double growth = 1.0L;
    for (int i = 0; i < months; ++i)
    {
        growth *= 1.0L + rate;
    }
    long double calculated = balance * rate * growth / (growth - 1.0L);
    RUB payment = static_cast<RUB>(calculated);
    if (static_cast<long double>(payment) < calculated)
    {
        ++payment;
    }
    return payment;
}

// Активы включают обе квартиры, машину и деньги в обеих валютах.
RUB alice_assets()
{
    RUB assets = alice.cash + alice_bank.account_RUB;
    assets += alice_bank.account_USD * alice_bank.usd_rate;
    assets += alice_home.flat_value + alice_home.rental_flat_value;
    assets += alice_home.land_value;
    assets += alice_car.value;
    return assets;
}

// Все виды долга собираем в одном месте.
RUB alice_debts()
{
    return alice_home.mortgage + alice_car.credit +
        alice_bank.credit + alice_bank.check;
}

// Чистый капитал отличается от суммы накоплений: из активов вычитаются долги.
RUB alice_net_worth()
{
    return alice_assets() - alice_debts();
}

// Зарплату считаем до удержания налога; сам налог оплатим общей функцией Tax.
void add_salary_income(RUB sum)
{
    add_income(sum);
    alice_taxes.salary_due += sum * alice_taxes.salary_rate_bp / 10000;
}

// Подработка облагается отдельной настраиваемой ставкой модели.
void add_side_income(RUB sum)
{
    add_income(sum);
    alice_taxes.side_due += sum * alice_taxes.side_rate_bp / 10000;
}

// Налог возникает только при фактическом получении арендной платы.
void add_rental_income(RUB sum)
{
    add_income(sum);
    alice_taxes.rental_due += sum * alice_taxes.rental_rate_bp / 10000;
}

// Расходы, связанные с работой, показываем отдельным итогом.
void spend_work(RUB sum)
{
    add_outflow(sum);
    alice_work.spent += sum;
}

// Общий итог отдыха включает развлечения и поездки.
void spend_leisure(RUB sum)
{
    add_outflow(sum);
    alice_leisure.spent += sum;
}

// Устройства и связь относятся к одной бытовой группе.
void spend_communication(RUB sum)
{
    add_outflow(sum);
    alice_communication.spent += sum;
}

// Разовые события не теряются среди регулярных расходов.
void spend_event(RUB sum)
{
    add_outflow(sum);
    alice_events.spent += sum;
}

// Каждый месяц начинаем с чистых флагов событий и запоминаем общие счётчики.
void alice_month_prepare()
{
    budget.month_income_before = total_income;
    budget.month_outflow_before = total_outflow;
    alice_meds.sick_days = 0;
    alice_meds.serious_illness = false;
    alice_work.side_job_this_month = false;
    alice_home.new_tenant = false;
    alice_leisure.vacation_this_month = false;
    ++alice_work.equipment_age;
    ++alice_communication.phone_age;
    ++alice_communication.computer_age;
    if (alice_car.unavailable_months > 0)
    {
        --alice_car.unavailable_months;
    }
}

// Зарплата индексируется независимо от того, работает ли Алиса в январе.
void alice_salary_indexation()
{
    if (cur_month != 1)
    {
        return;
    }
    alice_home.flat_value = inflate_year(alice_home.flat_value, 500);
    alice_home.rental_flat_value = inflate_year(alice_home.rental_flat_value, 500);
    if (cur_year == 2027)
    {
        alice.salary = 100000;
    }
    else
    {
        alice.salary = inflate_year(alice.salary, infl.salary);
    }
    alice_work.side_income = inflate_year(alice_work.side_income, infl.salary);
}

// Болезнь снижает заработок, мешает подработке и увеличивает медицинские расходы.
void alice_health_events()
{
    int illness_chance = 12;
    if (cur_month >= 10 or cur_month <= 3)
    {
        illness_chance = 22;
    }
    if (chance(illness_chance))
    {
        alice_meds.sick_days = rand_range(3, 7);
        ++alice_meds.illnesses;
    }
    if (chance(2))
    {
        alice_meds.sick_days = rand_range(10, 20);
        alice_meds.serious_illness = true;
        ++alice_meds.illnesses;
    }
    if (alice_meds.sick_days > 0)
    {
        ++alice_meds.sick_months;
    }
}

// Потеря работы означает несколько месяцев поиска, а не постоянное увольнение.
void alice_work_events()
{
    if (alice_work.unemployed_months > 0)
    {
        --alice_work.unemployed_months;
        alice_work.employed = alice_work.unemployed_months == 0;
        return;
    }
    if (chance(2))
    {
        alice_work.unemployed_months = rand_range(1, 3);
        alice_work.employed = false;
        ++alice_work.job_losses;
    }
}

// Вторую работу берём не каждый месяц и пропускаем при тяжёлой болезни.
void alice_side_job()
{
    if (alice_meds.sick_days > 7 or cur_month == alice_leisure.vacation_month)
    {
        return;
    }
    int probability = alice_work.employed ? 55 : 75;
    if (not chance(probability))
    {
        return;
    }
    RUB income = alice_work.side_income;
    income = income * rand_range(70, 130) / 100;
    add_side_income(income);
    alice_work.side_received += income;
    alice_work.side_job_this_month = true;
    ++alice_work.side_jobs;
}

// Рабочие поездки, обеды, обучение и оборудование оплачиваются по ситуации.
void alice_work_costs()
{
    if (alice_work.employed)
    {
        int work_days = 22 - alice_meds.sick_days;
        if (alice_leisure.vacation_this_month)
        {
            work_days /= 2;
        }
        RUB transport = alice_work.commute;
        if (alice_car.unavailable_months > 0)
        {
            transport = transport * 150 / 100;
        }
        spend_work(transport * work_days / 22);
        spend_work(alice_work.lunch * work_days / 22);
    }
    if (alice_work.side_job_this_month)
    {
        spend_work(alice_work.side_cost);
    }
    if (cur_month == alice_work.course_month and alice_bank.account_RUB > 150000)
    {
        spend_work(alice_work.course);
        alice_work.side_income = inflate_year(alice_work.side_income, 300);
    }
    if (alice_work.equipment_age >= 36 and alice_work.employed)
    {
        spend_work(alice_work.equipment);
        alice_work.equipment_age = 0;
    }
}

// Ставки здесь фиксированные и учебные, без претензии на точный налоговый расчёт.
void alice_income_tax()
{
    RUB tax = alice_taxes.salary_due + alice_taxes.side_due;
    tax += alice_taxes.rental_due;
    add_outflow(tax);
    alice_taxes.paid_income_tax += tax;
    alice_taxes.year_salary_tax += tax;
    alice_taxes.salary_due = 0;
    alice_taxes.side_due = 0;
    alice_taxes.rental_due = 0;
}

// Между арендаторами квартира может один или несколько месяцев не приносить доход.
void alice_rental_events()
{
    if (alice_home.vacant_months > 0)
    {
        --alice_home.vacant_months;
        if (alice_home.vacant_months == 0)
        {
            alice_home.new_tenant = true;
            ++alice_home.tenant_changes;
        }
        return;
    }
    if (chance(5))
    {
        alice_home.vacant_months = rand_range(1, 2);
    }
}

// После смены жильца оплачиваем поиск и уборку, иногда освежаем ремонт.
void alice_rental_repairs()
{
    RUB cost = 0;
    if (alice_home.new_tenant)
    {
        cost += alice_home.tenant_search_cost;
    }
    if (cur_month == 8 and cur_year % 3 == 0)
    {
        cost += alice_home.rental_repair_budget;
    }
    if (cost > 0)
    {
        add_outflow(cost);
        alice_home.rental_spent += cost;
    }
}

// Фрукты зимой и диетические продукты при болезни дополняют базовую корзину.
void alice_food_habits()
{
    RUB extra = 0;
    if (cur_month <= 3 or cur_month >= 11)
    {
        extra += alice_family.food / 20;
    }
    if (alice_meds.sick_days > 0)
    {
        extra += alice_family.food / 30;
    }
    add_outflow(extra);
    alice_family.food_spent += extra;
}

// Моющие средства, посуда и постельное бельё — отдельные бытовые покупки.
void alice_household_goods()
{
    RUB sum = alice_family.household;
    if (cur_month == 5 or cur_month == 11)
    {
        sum += alice_family.household * 2;
    }
    add_outflow(sum);
    alice_family.family_spent += sum;
}

// Небольшая помощь родителям сохраняется, но при увольнении сокращается.
void alice_family_support()
{
    RUB sum = alice_family.family_support;
    if (not alice_work.employed)
    {
        sum /= 2;
    }
    add_outflow(sum);
    alice_family.family_spent += sum;
}

// Подарки распределены по нескольким праздникам, а не каждый месяц.
void alice_gifts()
{
    if (cur_month != 3 and cur_month != 6 and cur_month != 12)
    {
        return;
    }
    RUB sum = alice_family.gifts;
    if (cur_month == 12)
    {
        sum *= 2;
    }
    add_outflow(sum);
    alice_family.family_spent += sum;
}

// Абонемент приостанавливается на месяц серьёзной болезни.
void alice_sport()
{
    if (alice_meds.serious_illness)
    {
        return;
    }
    add_outflow(alice_meds.sport);
    alice_meds.spent += alice_meds.sport;
}

// Раз в год проходим обследование, раз в несколько лет обновляем очки.
void alice_preventive_medicine()
{
    if (cur_month != 5)
    {
        return;
    }
    RUB sum = alice_meds.checkup;
    if ((cur_year - 2026) % 3 == 0)
    {
        sum += alice_meds.glasses;
    }
    add_outflow(sum);
    alice_meds.spent += sum;
    alice_taxes.year_medical_spent += sum;
}

// Игрушки, лежанка и поводок нужны не каждый месяц и только живой собаке.
void alice_pet_supplies()
{
    if (alice_family.dog_life_months <= 0 or cur_month != 4)
    {
        return;
    }
    add_outflow(alice_family.dog_supplies);
    alice_family.dog_spent += alice_family.dog_supplies;
}

// С возрастом плановый ветеринарный осмотр становится дороже.
void alice_pet_checkup()
{
    if (alice_family.dog_life_months <= 0 or cur_month != 9)
    {
        return;
    }
    RUB sum = alice_family.dog_checkup;
    if (alice_family.dog_age_months >= 84)
    {
        sum = sum * 150 / 100;
    }
    add_outflow(sum);
    alice_family.dog_spent += sum;
}

// Интернет и телефон оплачиваем всегда, подписки сокращаем при потере работы.
void alice_communication_costs()
{
    RUB sum = alice_communication.mobile + alice_communication.internet;
    if (alice_work.employed and alice_bank.check == 0)
    {
        sum += alice_communication.subscriptions;
    }
    spend_communication(sum);
}

// Устройства заменяем по возрасту, но не покупаем их в кредит ради удобства.
void alice_device_replacement()
{
    RUB available = alice.cash + alice_bank.account_RUB - 150000;
    if (alice_communication.phone_age >= 48 and
        available > alice_communication.phone_price)
    {
        spend_communication(alice_communication.phone_price);
        available -= alice_communication.phone_price;
        alice_communication.phone_age = 0;
    }
    if (alice_communication.computer_age >= 72 and
        available > alice_communication.computer_price)
    {
        spend_communication(alice_communication.computer_price);
        alice_communication.computer_age = 0;
    }
}

// При долге по карте или безработице оставляем только недорогие книги.
void alice_leisure_costs()
{
    RUB sum = alice_leisure.books;
    if (alice_work.employed and alice_bank.check == 0)
    {
        sum += alice_leisure.entertainment + alice_leisure.hobby;
    }
    if (cur_month == 12)
    {
        sum += alice_leisure.holiday_budget;
    }
    spend_leisure(sum);
}

// Отпуск разрешаем только при наличии запаса денег и без серьёзной болезни.
void alice_vacation()
{
    if (cur_month != alice_leisure.vacation_month)
    {
        return;
    }
    if (not alice_work.employed or alice_meds.serious_illness)
    {
        return;
    }
    RUB cost = alice_leisure.travel_budget + alice_insurance.travel_policy;
    RUB free_money = alice.cash + alice_bank.account_RUB - 150000;
    if (alice_bank.check > 0 or free_money < cost)
    {
        return;
    }
    spend_leisure(alice_leisure.travel_budget);
    add_outflow(alice_insurance.travel_policy);
    alice_insurance.paid += alice_insurance.travel_policy;
    alice_leisure.vacation_this_month = true;
    ++alice_leisure.vacations;
}

// Короткая поездка возможна весной или осенью, если нет дорогого долга.
void alice_weekend_trip()
{
    if (cur_month != 5 and cur_month != 9)
    {
        return;
    }
    if (alice_bank.check > 0 or alice_meds.sick_days > 0)
    {
        return;
    }
    if (alice_bank.account_RUB > 100000 and chance(40))
    {
        spend_leisure(alice_leisure.weekend_trip);
        ++alice_leisure.trips;
    }
}

// День рождения, семейные встречи и потерянные вещи добавляют разовые расходы.
void alice_life_events()
{
    if (cur_month == 4)
    {
        spend_event(alice_events.birthday);
    }
    if (cur_month == 1 or cur_month == 8)
    {
        spend_event(alice_events.family_visit);
    }
    if (chance(2))
    {
        spend_event(alice_events.celebration);
        ++alice_events.celebrations;
    }
    if (chance(2))
    {
        spend_event(alice_events.lost_items);
        ++alice_events.lost_items_count;
    }
    if (cur_month == 10 and (cur_year - 2026) % 5 == 0)
    {
        spend_event(alice_events.documents);
    }
}

// Полисы на жильё и машину считаем непрерывными, с ежегодным продлением.
void alice_insurance_payments()
{
    if (cur_month != 9)
    {
        return;
    }
    RUB sum = 0;
    if (alice_insurance.car_active)
    {
        sum += alice_insurance.car_policy;
    }
    if (alice_insurance.home_active)
    {
        sum += alice_insurance.home_policy;
    }
    add_outflow(sum);
    alice_insurance.paid += sum;
}

// Часть небольшого страхового случая возмещается, франшизу платит Алиса.
void alice_car_accident()
{
    if (alice_car.unavailable_months > 0 or not chance(1))
    {
        return;
    }
    RUB damage = rand_range(20000, 60000);
    RUB refund = 0;
    if (alice_insurance.car_active and damage > alice_insurance.car_deductible)
    {
        refund = damage - alice_insurance.car_deductible;
    }
    add_outflow(damage);
    add_income(refund);
    alice_car.total_costs += damage;
    alice_insurance.compensation += refund;
    ++alice_insurance.car_cases;
    ++alice_car.accidents;
    alice_car.unavailable_months = 2;
}

// При заливе квартиры страховка покрывает ущерб сверх франшизы.
void alice_home_accident()
{
    if (not chance(1))
    {
        return;
    }
    RUB damage = rand_range(15000, 50000);
    RUB refund = 0;
    if (alice_insurance.home_active and damage > alice_insurance.home_deductible)
    {
        refund = damage - alice_insurance.home_deductible;
    }
    add_outflow(damage);
    add_income(refund);
    alice_insurance.compensation += refund;
    ++alice_insurance.home_cases;
}

// Машина стареет каждый месяц, а её оценка снижается на 8 процентов в год.
void alice_car_depreciation()
{
    ++alice_car.age_months;
    alice_car.value -= alice_car.value * 800 / 120000;
}

// Старую машину меняем только при достаточном резерве, новый кредит не берём.
void alice_car_replacement()
{
    if (alice_car.age_months < 144 or alice_car.credit > 0)
    {
        return;
    }
    RUB net_cost = alice_car.replacement_price - alice_car.value;
    if (net_cost < 0)
    {
        net_cost = 0;
    }
    if (alice.cash + alice_bank.account_RUB < net_cost + 300000)
    {
        return;
    }
    if (alice_bank.check > 0 or alice_bank.credit > 0)
    {
        return;
    }
    add_income(alice_car.value);
    add_outflow(alice_car.replacement_price);
    alice_car.total_costs += alice_car.replacement_price;
    alice_car.value = alice_car.replacement_price;
    alice_car.age_months = 0;
    alice_car.unavailable_months = 0;
    ++alice_car.replacements;
}

// Рефинансируем ипотеку один раз, только если экономия превышает комиссию.
void alice_refinance()
{
    if (cur_year != 2029 or cur_month != 9 or alice_home.refinanced)
    {
        return;
    }
    if (alice_home.mortgage <= 0 or alice_home.mortgage_months < 24)
    {
        return;
    }
    RUB new_rate = 650;
    RUB new_payment = loan_payment(alice_home.mortgage, new_rate,
                                   alice_home.mortgage_months);
    RUB fee = alice_home.mortgage / 100;
    RUB saving = (alice_home.mortgage_payment - new_payment);
    saving *= alice_home.mortgage_months;
    if (new_rate >= alice_home.mortgage_rate_bp or saving <= fee)
    {
        return;
    }
    if (alice_bank.account_RUB < fee + 100000 or alice_bank.check > 0)
    {
        return;
    }
    add_outflow(fee);
    budget.refinance_fees += fee;
    alice_home.mortgage_rate_bp = new_rate;
    alice_home.mortgage_payment = new_payment;
    alice_home.refinanced = true;
}

// После досрочного закрытия остаток срока обнуляется сразу.
void alice_credit_status()
{
    if (alice_home.mortgage == 0)
    {
        alice_home.mortgage_months = 0;
        if (budget.mortgage_closed_month < 0)
        {
            budget.mortgage_closed_month = budget.months + 1;
        }
    }
    if (alice_car.credit == 0)
    {
        alice_car.credit_months = 0;
    }
    if (alice_bank.credit == 0)
    {
        alice_bank.credit_months = 0;
    }
    if (alice_debts() == 0 and budget.debt_free_month < 0)
    {
        budget.debt_free_month = budget.months + 1;
    }
}

// Новые статьи дорожают по своим группам, кредитные платежи не индексируются.
void alice_extra_inflation()
{
    alice_family.household = inflate(alice_family.household, infl.household);
    alice_family.family_support = inflate(alice_family.family_support, infl.food);
    alice_family.gifts = inflate(alice_family.gifts, infl.leisure);
    alice_family.dog_supplies = inflate(alice_family.dog_supplies, infl.pet);
    alice_family.dog_checkup = inflate(alice_family.dog_checkup, infl.pet);
    alice_meds.sport = inflate(alice_meds.sport, infl.leisure);
    alice_meds.checkup = inflate(alice_meds.checkup, infl.medicine);
    alice_meds.glasses = inflate(alice_meds.glasses, infl.medicine);
    alice_home.rental_repair_budget = inflate(alice_home.rental_repair_budget, infl.home_repair);
    alice_home.tenant_search_cost = inflate(alice_home.tenant_search_cost, infl.rent);
    alice_car.replacement_price = inflate(alice_car.replacement_price, infl.car);
    alice_work.side_cost = inflate(alice_work.side_cost, infl.education);
    alice_work.commute = inflate(alice_work.commute, infl.car);
    alice_work.lunch = inflate(alice_work.lunch, infl.food);
    alice_work.course = inflate(alice_work.course, infl.education);
    alice_work.equipment = inflate(alice_work.equipment, infl.education);
    alice_leisure.entertainment = inflate(alice_leisure.entertainment, infl.leisure);
    alice_leisure.hobby = inflate(alice_leisure.hobby, infl.leisure);
    alice_leisure.books = inflate(alice_leisure.books, infl.leisure);
    alice_leisure.travel_budget = inflate(alice_leisure.travel_budget, infl.leisure);
    alice_leisure.weekend_trip = inflate(alice_leisure.weekend_trip, infl.leisure);
    alice_leisure.holiday_budget = inflate(alice_leisure.holiday_budget, infl.leisure);
    alice_communication.mobile = inflate(alice_communication.mobile, infl.communication);
    alice_communication.internet = inflate(alice_communication.internet, infl.communication);
    alice_communication.subscriptions = inflate(alice_communication.subscriptions, infl.communication);
    alice_communication.phone_price = inflate(alice_communication.phone_price, infl.education);
    alice_communication.computer_price = inflate(alice_communication.computer_price, infl.education);
    alice_insurance.car_policy = inflate(alice_insurance.car_policy, infl.insurance);
    alice_insurance.home_policy = inflate(alice_insurance.home_policy, infl.insurance);
    alice_insurance.travel_policy = inflate(alice_insurance.travel_policy, infl.insurance);
    alice_events.birthday = inflate(alice_events.birthday, infl.leisure);
    alice_events.family_visit = inflate(alice_events.family_visit, infl.car);
    alice_events.celebration = inflate(alice_events.celebration, infl.leisure);
    alice_events.documents = inflate(alice_events.documents, infl.household);
    alice_events.lost_items = inflate(alice_events.lost_items, infl.household);
}

// Краткий годовой итог нужен для вывода о жизни, но не хранит большие отчёты.
void alice_year_report()
{
    RUB income = total_income - budget.year_income_before;
    RUB outflow = total_outflow - budget.year_outflow_before;
    printf("Год %d: доход %lld, отток %lld, накопления %lld, долги %lld RUB\n",
           cur_year, income, outflow, alice_bank.account_RUB, alice_debts());
    budget.year_income_before = total_income;
    budget.year_outflow_before = total_outflow;
}

// После всех платежей запоминаем дефициты и наиболее напряжённые месяцы.
void alice_month_finish()
{
    ++budget.months;
    RUB outflow = total_outflow - budget.month_outflow_before;
    RUB income = total_income - budget.month_income_before;
    if (outflow > income)
    {
        ++budget.deficit_months;
    }
    if (outflow > budget.largest_month_outflow)
    {
        budget.largest_month_outflow = outflow;
    }
    if (alice_bank.account_RUB < budget.lowest_savings)
    {
        budget.lowest_savings = alice_bank.account_RUB;
    }
    if (alice_debts() > budget.highest_debt)
    {
        budget.highest_debt = alice_debts();
    }
    if (cur_month == 12)
    {
        alice_year_report();
    }
}

// Итоги показывают и финансовый результат, и произошедшие жизненные события.
void alice_life_print()
{
    printf("\n--- Жизнь Алисы за %d месяцев ---\n", budget.months);
    printf("Начальный капитал = %lld RUB\n", budget.initial_capital);
    printf("Изменение капитала = %lld RUB\n", alice_net_worth() - budget.initial_capital);
    printf("Дефицитных месяцев = %d, минимум накоплений = %lld RUB\n",
           budget.deficit_months, budget.lowest_savings);
    printf("Потерь работы = %d, месяцев без работы = %d\n",
           alice_work.job_losses, alice_work.months_without_work);
    printf("Подработок = %d, заработано до налога = %lld RUB\n",
           alice_work.side_jobs, alice_work.side_received);
    printf("Месяцев болезни = %d, медицина и спорт = %lld RUB\n",
           alice_meds.sick_months, alice_meds.spent);
    printf("Отпусков = %d, коротких поездок = %d, отдых = %lld RUB\n",
           alice_leisure.vacations, alice_leisure.trips, alice_leisure.spent);
    printf("Простой квартиры = %d месяцев, смен арендатора = %d\n",
           alice_home.vacant_total, alice_home.tenant_changes);
    printf("Арендный доход = %lld, расходы квартиры = %lld RUB\n",
           alice_home.rental_received, alice_home.rental_spent);
    printf("ДТП = %d, замен машины = %d\n", alice_car.accidents, alice_car.replacements);
    printf("Страховые взносы = %lld, возмещения = %lld RUB\n",
           alice_insurance.paid, alice_insurance.compensation);
    printf("Налоги с доходов = %lld, имущественные = %lld RUB\n",
           alice_taxes.paid_income_tax, alice_taxes.paid_property_tax);
    printf("Налог на проценты = %lld, возвраты = %lld RUB\n",
           alice_taxes.paid_deposit_tax, alice_taxes.refund_received);
    printf("\n--- Детально: кредитование ---\n");
    printf("Проценты ипотеки = %lld RUB\n", alice_home.mortgage_interest_paid);
    printf("Проценты автокредита = %lld RUB\n", alice_car.interest_paid);
    printf("Проценты потребкредита = %lld RUB\n", alice_bank.consumer_interest_paid);
    printf("Проценты карты = %lld RUB\n", budget.card_interest);
    printf("Регулярные платежи = %lld, из них тело долга = %lld RUB\n",
           budget.loan_payments, budget.loan_principal);
    printf("Досрочное погашение со вклада = %lld RUB\n", budget.prepayments);
    printf("Платежи по карте = %lld RUB\n", budget.card_payments);
    printf("Комиссия рефинансирования = %lld RUB\n", budget.refinance_fees);
    printf("Рефинансирование = %s\n", alice_home.refinanced ? "да" : "нет");
    printf("Ипотека закрыта на месяце = %d\n", budget.mortgage_closed_month);
    printf("Первый месяц без всех долгов = %d\n", budget.debt_free_month);
    if (budget.limit_reached)
    {
        printf("Защитный предел времени достигнут, ипотека не погашена.\n");
    }
    printf("Налоги, пособия и страхование упрощены. Курс USD фиксирован.\n");
    printf("Итоговый капитал номинальный, без поправки на инфляцию.\n");
}

// Считаем минимум десять лет и продолжаем, пока не погашена ипотека.
void simulation()
{
    while (budget.months < MIN_SIMULATION_MONTHS or alice_home.mortgage > 0)
    {
        if (budget.months >= MAX_SIMULATION_MONTHS)
        {
            budget.limit_reached = true;
            break;
        }
        alice_month_prepare();
        alice_salary_indexation();
        alice_health_events();
        alice_work_events();
        alice_rental_events();
        alice_check_interest();
        alice_salary();
        alice_side_job();
        alice_rental_income();
        alice_bank_income();
        alice_income_tax();
        alice_check_payment();
        alice_refinance();
        alice_mortgage();
        alice_car_credit();
        alice_consumer_credit();
        alice_food();
        alice_food_habits();
        alice_pet_supplies();
        alice_pet_checkup();
        alice_dog();
        alice_clothes();
        alice_household_goods();
        alice_family_support();
        alice_gifts();
        alice_medicine();
        alice_sport();
        alice_preventive_medicine();
        alice_vacation();
        alice_work_costs();
        alice_communication_costs();
        alice_device_replacement();

        alice_utilities();
        alice_home_repair();
        alice_rent();
        alice_rental_expense();
        alice_rental_repairs();

        alice_car_costs();
        alice_car_accident();
        alice_car_replacement();
        alice_insurance_payments();
        alice_home_accident();

        alice_leisure_costs();
        alice_weekend_trip();
        alice_life_events();
        alice_tax();
        alice_deposit_tax();
        alice_tax_refund();

        alice_save_excess();
        alice_cover_deficit();
        alice_prepay_debts();
        alice_credit_status();
        alice_car_depreciation();
        alice_month_finish();
        apply_inflation();

        ++cur_month;
        if (cur_month == 13)
        {
            ++cur_year;
            cur_month = 1;
        }
    }
}

// Показываем активы, долги и финансовый результат жизни Алисы.
void alice_print()
{
    RUB debts =
        alice_home.mortgage +
        alice_car.credit +
        alice_bank.credit +
        alice_bank.check;
    RUB usd_in_rub = alice_bank.account_USD * alice_bank.usd_rate;
    RUB assets =
        alice.cash +
        alice_bank.account_RUB +
        alice_home.flat_value +
        alice_home.rental_flat_value +
        alice_home.land_value +
        alice_car.value +
        usd_in_rub;
    RUB net_worth = assets - debts;

    printf("Alice salary = %lld\n", alice.salary);
    printf("Alice cash = %lld\n", alice.cash);

    printf("Savings RUB = %lld\n", alice_bank.account_RUB);
    printf("Savings USD = %lld\n", alice_bank.account_USD);

    printf("Mortgage left = %lld\n", alice_home.mortgage);
    printf("Mortgage months left = %d\n", alice_home.mortgage_months);

    printf("Car credit left = %lld\n", alice_car.credit);
    printf("Car credit months left = %d\n", alice_car.credit_months);

    printf("Consumer credit left = %lld\n", alice_bank.credit);
    printf("Consumer credit months left = %d\n", alice_bank.credit_months);

    printf("Check debt = %lld\n", alice_bank.check);

    printf("Flat value = %lld\n", alice_home.flat_value);

    printf("Total income = %lld\n", total_income);
    printf("Total outflow = %lld\n", total_outflow);
    printf("Total refund = %lld\n", total_refund);
    printf("Total interest = %lld\n", total_interest);

    printf("Assets = %lld\n", assets);
    printf("Debts = %lld\n", debts);
    printf("Net worth = %lld\n", net_worth);
    if (net_worth > 0 and debts == 0 and budget.deficit_months < budget.months / 4)
    {
        printf(" вывод: модель хорошая, кредиты закрыты, есть активы.\n");
    }
    else if (net_worth > 0 and debts < assets / 2)
    {
        printf(" вывод: модель средняя, есть активы, но есть долги или частые дефициты.\n");
    }
    else
    {
        printf(" вывод: модель рискованная, долговая нагрузка слишком велика.\n");
    }
}

// Фиксируем случайность, моделируем жизнь и выводим финансовые итоги.
int main()
{
    srand(simulation_seed);

    alice_init();
    simulation();
    if (cur_month != 1)
    {
        printf("Неполный завершающий год: ");
        alice_year_report();
    }

    alice_print();
    alice_life_print();
    return budget.limit_reached ? 1 : 0;
}
