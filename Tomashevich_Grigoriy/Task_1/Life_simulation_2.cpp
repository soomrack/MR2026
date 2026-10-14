#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

using RUB = unsigned long long int;

struct Pet {
    bool is_presence;
    bool had_dog;
    int death_year;
    int death_month;

    RUB food_monthly;
    RUB vet_yearly;
    int vet_month;

    RUB purchase_price;
    int last_index_year;
};

struct Inflation {
    double food;
    double utilities;   //коммуналка
    double rent;
    double car;
    double pet;
    double medicine;
    double clothing;
    double education;
    double services;    // бытовое
    
    double salary;
};

struct Credit {
    bool is_active;
    bool is_closed;
    std::string name;
    RUB principal;  // Нач сум
    RUB remaining;  // Текущий остаток
    double rate;    // Годовая ставка
    RUB monthly_payment;    // платёж по графику
    int term_months;        // срок в месяцах
    int months_paid;        // выплачено
    int overdue_months;     //месяцы просрочки
    RUB penalty;            // пеня
    RUB total_overdue;      // общая сумма просрочки
};

struct Bank {
    RUB deposit;                // вклад
    RUB currency;               // деньги на счету в банке count
    RUB year_income;            // количество денег заработанных за последний год
    RUB last_year_income;       // количество денег заработанных за предыдущий год
    RUB last_last_year_income;  // количество денег заработанных за пред предыдущий год
    int credit_score;           //кредитный рейтинг
};

struct Tax {
    RUB ndfl_paid_this_year;
    RUB ndfl_paid_last_year;

    RUB refund_accum;
    RUB mortgage_expenses_this_year;
    RUB education_expenses_this_year;
    RUB medicine_expenses_this_year;
    RUB mortgage_expenses_last_year;
    RUB education_expenses_last_year;
    RUB medicine_expenses_last_year;

    RUB transport_tax_yearly;
    RUB property_tax_yearly;

    //лимиты
    RUB mortgage_body_deduction_cap; // 2 000 000 — тело ипотеки
    RUB mortgage_interest_deduction_cap; // 3 000 000 — проценты по ипотеке
    RUB social_deduction_cap; // 150 000 — социальные вычеты
};

struct Car { 
    bool is_presence;  // наличие
    int power;      // мощность двигателя
    int hp;         // текущее здоровье
    int max_hp;     // максимум здоровья
    bool broken;    // сломана ли в данный момент
    RUB fuel_monthly;   //бензин в месяц
    RUB insurance_monthly;  // страховка в месяц
    RUB maintenance_yearly;  // стоимость ТО раз в год
    int maintenance_month;  // месяц в который ТО
    int age;
    RUB purchase_price;
    int last_index_year;
};

struct Flat {
    bool is_presence;  // наличие
    bool for_rent;  // наличие квартиры под аренду
    int square;     // площадь квартиры
    
    RUB gas_rate;
    int gas_year;                // год последней индексации тарифа
    RUB cold_water_rate;
    int cold_water_year;
    RUB hot_water_rate;
    int hot_water_year;
    RUB electricity_rate;
    int electricity_year;
    RUB internet_rate;
    int internet_year;
    RUB maintenance_rate;
    int maintenance_year;

    RUB property_tax_yearly;    //налог на имущество за год
    int property_tax_year;
    RUB purchase_price;
};

struct SalaryEvent {
    int year;
    int month;
};

struct Employer {
    bool is_active;
    std::string company;
    RUB salary;
    int last_index_year;

    SalaryEvent salary_schedule[5];
    RUB salary_levels[5];
    int salary_schedule_size;
};

struct Person {
    RUB cash;
    RUB salary;
    Pet dog;    
    Bank account;
    Car car_one;
    Flat flat_one;
    Credit mortgage;               // ипотека
    Employer job;
    Employer job_side;
    Credit credits[5];
    int credits_count;

    RUB food_monthly;
    RUB medicine_monthly;
    RUB clothing_monthly;
    RUB education_monthly;

    RUB rent_monthly;
    int rent_tier;
    int rent_last_index_year;

    int health;
    RUB investment;
    int personal_last_index_year;
    Tax tax;
};

struct Country {
    Inflation inflation;
};

struct Person bob;
struct Person sam;
struct Country country;

// функция, чтобы счёт не ушёл в -

bool try_pay(RUB& currency, RUB amount) 
{
    if (currency >= amount) {
        currency -= amount;
        return true;
    }
    return false;
}

//Инфляция к базовой сумме

RUB apply_inflation(RUB base, double rate)
{
    double result = (double)base * (1.0 + rate);
    return (RUB)result;
}

//Повышение всего, согласно инфляции раз в год

void bob_pet_inflation_apply(const int year) {
    if (bob.dog.last_index_year >= year) return;
    bob.dog.food_monthly = apply_inflation(bob.dog.food_monthly, country.inflation.pet);
    bob.dog.vet_yearly = apply_inflation(bob.dog.vet_yearly,  country.inflation.pet);
    bob.dog.last_index_year = year;
}

void bob_car_inflation_apply(const int year) {
    if (bob.car_one.last_index_year >= year) return;
    bob.car_one.fuel_monthly = apply_inflation(bob.car_one.fuel_monthly,       country.inflation.car);
    bob.car_one.insurance_monthly = apply_inflation(bob.car_one.insurance_monthly,  country.inflation.car);
    bob.car_one.maintenance_yearly = apply_inflation(bob.car_one.maintenance_yearly, country.inflation.car);
    bob.car_one.last_index_year = year;
}

void bob_personal_inflation_apply(const int year) {
    if (bob.personal_last_index_year >= year) return;
    bob.food_monthly = apply_inflation(bob.food_monthly, country.inflation.food);
    bob.medicine_monthly = apply_inflation(bob.medicine_monthly, country.inflation.medicine);
    bob.clothing_monthly = apply_inflation(bob.clothing_monthly, country.inflation.clothing);
    bob.education_monthly = apply_inflation(bob.education_monthly,country.inflation.education);
    bob.personal_last_index_year = year;
}

RUB inflate_rate_if_needed(RUB& rate, int& rate_year, int current_year, double inflation) {
    if (rate_year < current_year) {
        rate = apply_inflation(rate, inflation);
        rate_year = current_year;
    }
    return rate;
}

// Коммунальные

RUB bob_utility_gas(const int year) {
    return inflate_rate_if_needed(bob.flat_one.gas_rate, bob.flat_one.gas_year, year, country.inflation.utilities);
}

RUB bob_utility_cold_water(const int year) {
    return inflate_rate_if_needed(bob.flat_one.cold_water_rate, bob.flat_one.cold_water_year, year, country.inflation.utilities);
}

RUB bob_utility_hot_water(const int year) {
    return inflate_rate_if_needed(bob.flat_one.hot_water_rate, bob.flat_one.hot_water_year, year, country.inflation.utilities);
}

RUB bob_utility_electricity(const int year) {
    return inflate_rate_if_needed(bob.flat_one.electricity_rate, bob.flat_one.electricity_year, year, country.inflation.utilities);
}

RUB bob_utility_internet(const int year) {
    return inflate_rate_if_needed(bob.flat_one.internet_rate, bob.flat_one.internet_year, year, country.inflation.utilities);
}

RUB bob_utility_maintenance(const int year) {
    return inflate_rate_if_needed(bob.flat_one.maintenance_rate, bob.flat_one.maintenance_year, year, country.inflation.utilities);
}

void bob_utilities(const int year)
{
    if (!bob.flat_one.is_presence) return;
    RUB total = 0;
    total += bob_utility_gas(year);
    total += bob_utility_cold_water(year);
    total += bob_utility_hot_water(year);
    total += bob_utility_electricity(year);
    total += bob_utility_internet(year);
    total += bob_utility_maintenance(year);
    RUB bills = total;
    try_pay(bob.account.currency, bills);
}

////////////////////////////

void bob_employer_pay(Employer& emp, const int year, const int month)
{
    if (!emp.is_active) return;

    for (int i = 0; i < emp.salary_schedule_size; ++i) {
        const SalaryEvent& ev = emp.salary_schedule[i];
        if (ev.year == year && ev.month == month) {
            emp.salary = emp.salary_levels[i];
        }
    }

    if (emp.last_index_year < year) {
        emp.salary = apply_inflation(emp.salary, country.inflation.salary);
        emp.last_index_year = year;
    }

    bob.account.currency += emp.salary;
    bob.account.year_income += emp.salary;
}

void bob_side_job_start(const int year, const int month)
{
    if (bob.job_side.is_active) return;
    if (year < 2028 || (year == 2028 && month < 3)) return;

    bob.job_side.is_active = true;
    bob.job_side.company = "Фриланс";
    bob.job_side.salary = 40'000;
    bob.job_side.last_index_year = year;

    bob.job_side.salary_schedule[0] = {2030, 6};
    bob.job_side.salary_levels[0]   = 55'000;
    bob.job_side.salary_schedule[1] = {2032, 1};
    bob.job_side.salary_levels[1]   = 70'000;
    bob.job_side.salary_schedule_size = 2;
}

void bob_salary(const int year, const int month)
{
    //bob_side_job_start(year, month);

    bob_employer_pay(bob.job, year, month);
    //bob_employer_pay(bob.job_side, year, month);

    // суммарный доход (для проверок)
    bob.salary = bob.job.salary; //+ bob.job_side.salary;
}

void bob_car_buy(const int year, const int month) 
{
    if (!bob.flat_one.is_presence) return;

    if (bob.car_one.is_presence) {
        return;
    }
    
    if (year > 2030 || (year == 2030 && month >= 6)) {                                                 // появление желания купить авто
        if (bob.account.currency >= 500'000 && bob.salary >= 100'000 && !(bob.car_one.is_presence)) {        // появление возможности купить авто
            bob.car_one.is_presence = true;
            bob.car_one.power = 90;
            bob.car_one.hp = 255;
            bob.car_one.max_hp = 255;
            bob.car_one.broken = false;
            bob.car_one.age = 0;
            bob.car_one.purchase_price = 300'000;
            bob.account.currency -= 300'000;

            }
        }
    else {
        return;
    }    
}

void bob_car_monthly_costs() {

    if (!bob.car_one.is_presence) {
        return;
    }
    RUB cost = bob.car_one.fuel_monthly + bob.car_one.insurance_monthly;
    try_pay(bob.account.currency, cost);
}

RUB bob_car_sale_price() {
    if (!bob.car_one.is_presence) return 0;
    double condition = (double)bob.car_one.hp / bob.car_one.max_hp;
    double age_factor = 1.0 - (bob.car_one.age * 0.1);
    if (age_factor < 0.1) age_factor = 0.1;
    double price = bob.car_one.purchase_price * condition * age_factor;
    return (RUB)price;
}

void bob_car_sell()
{
    if (!bob.car_one.is_presence) return;
    if (bob.account.currency > 50'000 || bob.salary > 100'000) return;

    RUB price = bob_car_sale_price();
    bob.car_one.is_presence = false;
    bob.car_one.power = 0;
    bob.account.currency += price;
    bob.account.year_income += price;
}

void bob_car(const int year, const int month) // добавить возраст авто
{
    bob_car_buy(year, month);
    bob_car_monthly_costs();
    bob_car_sell();
}

void bob_car_use(const int year, const int month) 
{
    if (!bob.car_one.is_presence) {
        return;
    }
    if (bob.car_one.broken) {
        return;
    }
    //Постепенный износ
    bob.car_one.hp -= 1 + rand() % 3;
    if (bob.car_one.hp < 0) {
        bob.car_one.hp = 0;
    }

    //вероятность поломки квадрат износа
    double wear = 1.0 - (double)bob.car_one.hp / bob.car_one.max_hp; // отношение текущего хп к максимального
    double break_chance = wear * wear;

    double roll = (double)rand() / RAND_MAX;
    if (roll < break_chance) {
        bob.car_one.broken = true;
    }
}

void bob_car_repair(const int year, const int month)
{
    if (!bob.car_one.is_presence) return;
    if (!bob.car_one.broken) return;

    RUB cost = 5'000 + rand() % 95'001;

if (!try_pay(bob.account.currency, cost)) return;

    if (bob.account.currency < cost) {
        return;
    }

    // макс хп падает и тем сильнее чем больше стоимость
    int hp_loss = 1 + (int)((double)cost / 100'000.0 * 9);
    bob.car_one.max_hp -= hp_loss;
    
    if (bob.car_one.max_hp < 1) {
        bob.car_one.max_hp = 1;
    }

    bob.car_one.hp = bob.car_one.max_hp;
    bob.car_one.broken = false;
}

//налог на транспорт   !! сделать оплату в марте
RUB bob_transport_tax_yearly() 
{
    if (!bob.car_one.is_presence) return 0;

    int power = bob.car_one.power;

    RUB rate;   // ставка за 1 л.с.
    if (power <= 100) rate = 12;
    else if (power <= 125) rate = 25;
    else if (power <= 150) rate = 35;
    else if (power <= 175) rate = 45;
    else if (power <= 200) rate = 50;
    else if (power <= 225) rate = 65;
    else if (power <= 250) rate = 75;
    else rate = 150;
    return (RUB)(power * rate);
}

//налог на имущество !!сделать оплату в декабре
RUB bob_property_tax_yearly()
{
    if (!bob.flat_one.is_presence) return 0;
    return bob.flat_one.property_tax_yearly;
}

//медицина усложнить, инвестиционный счёт, аккуратно описать налоговую систему

// система вклада
// пополнение из сфободных денег
void bob_deposit_add()
{
    if (bob.account.currency > 200'000) {
        RUB put = (bob.account.currency - 200'000) / 10;
        bob.account.currency -= put;
        bob.account.deposit += put;
    }
}

//проценты раз в месяц
void bob_deposit_interest()
{
    RUB interest = (RUB)(bob.account.deposit * 0.005);
    bob.account.deposit += interest;
    bob.account.year_income += interest;
}

//съём с вклада, если мало денег
void bob_deposit_withdraw()
{
    if (bob.account.currency < 50'000 && bob.account.deposit > 0) {
        RUB need = 50'000 - bob.account.currency;
        if (need > bob.account.deposit) {
            need = bob.account.deposit;
        }
        bob.account.deposit -= need;
        bob.account.currency += need;
    }
}

void bob_food(const int year) 
{
    try_pay(bob.account.currency, bob.food_monthly);
}

void bob_medicine(const int year, const int month)
{
    // плановые траты
    if (try_pay(bob.account.currency, bob.medicine_monthly)) {
        bob.tax.medicine_expenses_this_year += bob.medicine_monthly;
    }

    // случайная болезнь
    double sick_chance = 0.02 + (100 - bob.health) * 0.002;
    double roll = (double)rand() / RAND_MAX;
    if (roll < sick_chance) {
        RUB cost = 10'000 + rand() % 50'000;
        if (try_pay(bob.account.currency, cost)) {
            bob.tax.medicine_expenses_this_year += cost;
        }
        bob.health -= 5 + rand() % 10;
        if (bob.health < 0) bob.health = 0;
    } else {
        if (bob.health < 100) bob.health++;
    }
}

void bob_clothing() {
    try_pay(bob.account.currency, bob.clothing_monthly);
}

void bob_education() {
    if (try_pay(bob.account.currency, bob.education_monthly)) {
        bob.tax.education_expenses_this_year += bob.education_monthly;
    }
}

void bob_rent_inflation_apply(const int year)
{
    if (bob.rent_last_index_year >= year) return;
    if (bob.rent_monthly > 0) {
        bob.rent_monthly = apply_inflation(bob.rent_monthly, country.inflation.rent);
    }
    bob.rent_last_index_year = year;
}

void bob_rent(const int year, const int month) 
{   
    if (bob.flat_one.is_presence) {
        return;
    }

    int tier = 0;
    RUB base = 0;

    if (year < 2027 || (year == 2027 && month <= 8)) {
        tier = 0; base = 0;
    }
    else if (year < 2028 || (year == 2028 && month <= 6)) {
        tier = 1; base = 20'000;
    }
    else {
        tier = 2; base = 40'000;
    }

    if (bob.rent_tier != tier) {
        bob.rent_tier = tier;
        bob.rent_monthly = base;
    }

    try_pay(bob.account.currency, bob.rent_monthly);
}

void bob_home_bills(const int year, const int month)  
{
    RUB bills = 0;
    
    if (year > 2033 || (year == 2033 && month >= 3) ) {
        bills = 25'000;
    }
    else if (year > 2030 || (year == 2030 && month >= 4) ) {
        bills = 20'000;
    }
    else if (year > 2028 || (year == 2028 && month >= 6) ) {
        bills = 10'000;
    }
    else {
        bills = 5'000;
    }

    try_pay(bob.account.currency, bills);
}

void bob_dog_buy(const int year, const int month)
{
    if (bob.dog.is_presence || bob.dog.had_dog) {
        return;
    }
        if (bob.salary <= 100'000 || !bob.flat_one.is_presence) {
        return;
    }

    bob.dog.is_presence = true;
    bob.dog.death_year = year + 12;
    bob.dog.death_month = month;
    try_pay(bob.account.currency, 20'000);
}

void bob_dog_upkeep(const int year, const int month)
{
    if (!bob.dog.is_presence) {
        return;
    }
    
    bool alive = (year < bob.dog.death_year) || (year == bob.dog.death_year && month < bob.dog.death_month);

    if (!alive) {
        bob.dog.is_presence = false;
        bob.dog.had_dog = true;
        try_pay(bob.account.currency, 15'000);
        return;
    }

    try_pay(bob.account.currency, bob.dog.food_monthly);
}

void bob_dog_vet(const int month) {
    if (!bob.dog.is_presence) return;
    if (month == bob.dog.vet_month) {
        try_pay(bob.account.currency, bob.dog.vet_yearly);
    }
}

void bob_dog(const int year, const int month) 
{
    bob_dog_buy(year, month);
    bob_dog_upkeep(year, month);
    bob_dog_vet(month);
}

RUB sum_tax(RUB income)
{
    if (income <= 2'400'000) {
        return (RUB)(income * 0.13);
    }
    if (income <= 5'000'000) {
        return 312'000 + (RUB)((income - 2'400'000) * 0.15);
    }
    if (income <= 20'000'000) {
        return 702'000 + (RUB)((income - 5'000'000) * 0.18);
    }
     if (income <= 50'000'000) {
        return 3'402'000 + (RUB)((income - 20'000'000) * 0.20);
     }
    return 9'402'000 + (RUB)((income - 50'000'000) * 0.22);
}

void bob_ndfl(const int month) 
{
   RUB owed = sum_tax(bob.account.year_income);
   
    if (owed > bob.tax.ndfl_paid_this_year) {
        RUB delta = owed - bob.tax.ndfl_paid_this_year;
        try_pay(bob.account.currency, delta);
    }
   bob.tax.ndfl_paid_this_year = owed;

    // обнуление годовой статистики
    if (month == 12) {
        bob.tax.ndfl_paid_last_year = bob.tax.ndfl_paid_this_year;
        bob.account.last_last_year_income = bob.account.last_year_income;
        bob.account.last_year_income = bob.account.year_income;
        bob.account.year_income = 0;
        bob.tax.ndfl_paid_this_year = 0;

        bob.tax.mortgage_expenses_last_year = bob.tax.mortgage_expenses_this_year;
        bob.tax.education_expenses_last_year = bob.tax.education_expenses_this_year;
        bob.tax.medicine_expenses_last_year = bob.tax.medicine_expenses_this_year;
        bob.tax.mortgage_expenses_this_year = 0;
        bob.tax.education_expenses_this_year = 0;
        bob.tax.medicine_expenses_this_year = 0;
    }
}

RUB tax_refund_by_deduction(RUB annual_income, RUB deduction) 
{
    if (deduction == 0) return 0;
    if (deduction > annual_income) deduction = annual_income;

    RUB tax_before = sum_tax(annual_income);
    RUB tax_after  = sum_tax(annual_income - deduction);    // возврат излишне уплаченных налогов, уменьшение налого облагаемой базы,  annual_income - (медицина и тд с учётом лимитов)
    return tax_before - tax_after;
}

//возвраты в четвёртый месяц раз в год
void bob_tax_refund(const int month)
{
    if (month != 4) return;
    RUB income = bob.account.last_year_income;

    // предел на имуществ выч 2 млн и 3 млн за проценты
    RUB mortgage_cap = bob.tax.mortgage_body_deduction_cap + bob.tax.mortgage_interest_deduction_cap;
    RUB mortgage_ded = bob.tax.mortgage_expenses_last_year;
    if (mortgage_ded > mortgage_cap) {
        mortgage_ded = mortgage_cap;
    }

    // социальные вычеты база 150 тыс
    RUB social_ded = bob.tax.education_expenses_last_year + bob.tax.medicine_expenses_last_year;
    if (social_ded > bob.tax.social_deduction_cap) {
        social_ded = bob.tax.social_deduction_cap;
    }

    // складываем вычеты, проверка превышения
    RUB total_deduction = mortgage_ded + social_ded;
    if (total_deduction > income) total_deduction = income;

    RUB total = tax_refund_by_deduction(income, total_deduction);

    if (total > bob.tax.ndfl_paid_last_year) {
        total = bob.tax.ndfl_paid_last_year;
    }

    bob.tax.refund_accum += total;
    bob.account.currency += total;
}

void bob_property_tax_inflation_apply(const int year)
{
    if (bob.flat_one.property_tax_year >= year) return;
    if (bob.flat_one.is_presence) {
        bob.flat_one.property_tax_yearly =
            apply_inflation(bob.flat_one.property_tax_yearly, country.inflation.utilities);
    }
    bob.flat_one.property_tax_year = year;
}

//кредитная система
//проверка возможности взять кредит
int bob_find_free_credit_slot()
{
    for (int i = 0; i < 5; ++i) {
        if (!bob.credits[i].is_active) {
            return i;
        }
    }
    return -1; ///-1?
}

//создание кредита
void credit_init(Credit& c, const char* name, RUB principal, double rate, int term_months)
{
    if (bob_find_free_credit_slot() == -1) return;

    c.is_active = true;
    c.is_closed = false;
    c.name = name;
    c.principal = principal;
    c.remaining = principal;
    c.rate = rate;
    c.term_months = term_months;
    c.months_paid = 0;
    c.overdue_months = 0;
    c.penalty = 0;
    c.total_overdue = 0;

    //платёж. слизан с аннуитетного, но проще. прям сильно
    double monthly_rate = rate / 12.0;
    double factor = 1.0;
    for (int i = 0; i < term_months; ++i) {
        factor *= (1.0 + monthly_rate);
    }
    double payment = (double)principal * monthly_rate * factor / (factor - 1.0);
    c.monthly_payment = (RUB)payment;
}

//начисление процентов
void credit_accrue_monthly(Credit& c)
{
    if (!c.is_active || c.is_closed) {
        return;
    }
    double monthly_rate = c.rate / 12.0;
    RUB interest = (RUB)((double)c.remaining * monthly_rate);
    c.remaining +=interest;
}

//система пени (штрафа)
void credit_penalty(Credit& c)
{
    if (!c.is_active || c.is_closed) {
        return;
    }
    RUB peny = (RUB)((double)c.monthly_payment * 0.03);
    c.penalty += peny;
    c.remaining += peny;
}

//Платёж если есть деньги, иначе минус рейтинг
void bob_credit_process_payment(Credit& c)
{
    if (!c.is_active || c.is_closed) return;
    RUB need = c.monthly_payment + c.penalty;
    if (try_pay(bob.account.currency, need)) {
        if (c.remaining >= c.monthly_payment) {
            c.remaining -= c.monthly_payment;
        } else {
            c.remaining = 0;
        }
        c.penalty = 0;
        c.months_paid++;
        c.overdue_months = 0;
        if (c.remaining <= c.monthly_payment) {
            c.remaining = 0;
            c.is_closed = true;
            c.is_active = false;
            if (bob.account.credit_score < 100) {
                    bob.account.credit_score += 5;
            }
        }

        if (bob.account.credit_score < 100) {
            bob.account.credit_score += 1;
        }
    }
    else {
        c.overdue_months++;
        c.total_overdue += c.monthly_payment;
        credit_penalty(c);
        
        //просрочка --> - рейтинг
        if (bob.account.credit_score > 0) {
            bob.account.credit_score -= 3;
            if (bob.account.credit_score < 0) {
                bob.account.credit_score = 0;
            }
        }
    }  
}

//прогон всех кредитов
void bob_credit_payments()
{
    for (int i = 0; i < 5; ++i) {
        if (!bob.credits[i].is_active) continue;
        credit_accrue_monthly(bob.credits[i]);
        bob_credit_process_payment(bob.credits[i]);
    }
}

//обновление рейтинга
void bob_credit_score_update()
{
    int is_active_credits = 0;
    int total_overdue = 0;

    for (int i = 0; i < 5; ++i) {
        if (bob.credits[i].is_active) {
            is_active_credits++;
            total_overdue += bob.credits[i].overdue_months;
        }
    }
    if (is_active_credits >= 3) {
        if (bob.account.credit_score > 0) bob.account.credit_score -= 1;
    }
        if (total_overdue > 6) {
        if (bob.account.credit_score > 0) bob.account.credit_score -= 2;
    }
    if (bob.account.credit_score > 100) bob.account.credit_score = 100;
    if (bob.account.credit_score < 0)   bob.account.credit_score = 0;
}

//ипотека
void bob_mortgage_issue(const int year, const int month)
{
    if (bob.flat_one.is_presence) return;

    if (year > 2033 || (year == 2033 && month >= 3)) {
        int slot = bob_find_free_credit_slot();
        if (slot != -1) {
        credit_init(bob.mortgage, "ипотека", 8'000'000, 0.08, 300);
        bob.flat_one.is_presence = true;
        bob.flat_one.purchase_price = 8'000'000;
        bob.flat_one.property_tax_yearly = 8'000;
        }
    }
}

void bob_mortgage_payment(const int year, const int month)
{
    if (bob.mortgage.remaining == 0) return;
    
    credit_accrue_monthly(bob.mortgage);
    bob_credit_process_payment(bob.mortgage);
}

void bob_mortgage(const int year, const int month)
{
    bob_mortgage_issue(year, month);
    bob_mortgage_payment(year, month);
}

//инвестиции около 12% в год
void bob_investment_income()
{
    if (bob.investment == 0) return;
    RUB income = (RUB)((double)bob.investment * 0.01);
    bob.investment += income;
    bob.account.year_income += income;
}

// перекладываем свободные деньги в инвестиции
void bob_investment_add()
{
    if (bob.account.currency > 300'000) {
        RUB put = (bob.account.currency - 300'000) / 5;
        bob.account.currency -= put;
        bob.investment += put;
    }
}

// снимаем, если не хватает на жизнь
void bob_investment_withdraw()
{
    if (bob.account.currency < 30'000 && bob.investment > 0) {
        RUB need = 30'000 - bob.account.currency;
        if (need > bob.investment) need = bob.investment;
        bob.investment -= need;
        bob.account.currency += need;
    }
}
//
void bob_capitalize_assets() {
     if (bob.car_one.is_presence) {
        RUB price = bob_car_sale_price();
        bob.account.currency += price;
        bob.car_one.is_presence = false;
    }
    if (bob.flat_one.is_presence) {
        RUB flat_price = bob.flat_one.purchase_price;
        bob.account.currency += flat_price;
        if (bob.mortgage.remaining > 0) {
            if (bob.account.currency >= bob.mortgage.remaining) {
                bob.account.currency -= bob.mortgage.remaining;
                bob.mortgage.remaining = 0;
                bob.mortgage.is_active = false;
                bob.mortgage.is_closed = true;
            } else {
                bob.mortgage.remaining -= bob.account.currency;
                bob.account.currency = 0;
            }
        }
        bob.flat_one.is_presence = false;
    }
    if (bob.dog.is_presence) {
        bob.account.currency += 5000;
        bob.dog.is_presence = false;
    }
}

void bob_year_taxes_and_inflation(const int year, const int month) 
{
    if (month == 3)  try_pay(bob.account.currency, bob_transport_tax_yearly());
    if (month == 12) try_pay(bob.account.currency, bob_property_tax_yearly());
    if (month == 1)  {
        bob_pet_inflation_apply(year);
        bob_car_inflation_apply(year);
        bob_personal_inflation_apply(year);
        //food_inflation_apply(year);
        bob_property_tax_inflation_apply(year);
        bob_rent_inflation_apply(year); 
        if (bob.car_one.is_presence) bob.car_one.age++;
    }
}

void bob_mortgage_track_expenses(const int year, const int month)
{
    RUB before = bob.mortgage.remaining;
    bob_mortgage(year, month);
    if (before > bob.mortgage.remaining) {
        bob.tax.mortgage_expenses_this_year += before - bob.mortgage.remaining;
    }
}

void BOB(const int year, const int month)
{
    bob_salary(year, month);

        bob_mortgage_track_expenses(year, month);
        bob_rent(year, month);
        bob_utilities(year);
        bob_food(year);
        bob_medicine(year, month);
        bob_clothing();
        bob_education();

        bob_dog(year, month);
       
        bob_ndfl(month);

        bob_car(year, month);
        bob_car_use(year, month);
        bob_car_repair(year, month);
       
        bob_credit_payments();
        bob_credit_score_update();


        //годовые налоги и инфляция
        bob_year_taxes_and_inflation(year, month);

        //возврат налогов
        bob_tax_refund(month);

        bob_deposit_interest();
        bob_deposit_add();
        bob_deposit_withdraw();

        bob_investment_income();
        bob_investment_add();
        bob_investment_withdraw();
}

///////////           sam           //////////////

void sam_pet_inflation_apply(const int year) {
    if (sam.dog.last_index_year >= year) return;
    sam.dog.food_monthly = apply_inflation(sam.dog.food_monthly, country.inflation.pet);
    sam.dog.vet_yearly = apply_inflation(sam.dog.vet_yearly,  country.inflation.pet);
    sam.dog.last_index_year = year;
}

void sam_car_inflation_apply(const int year) {
    if (sam.car_one.last_index_year >= year) return;
    sam.car_one.fuel_monthly = apply_inflation(sam.car_one.fuel_monthly,       country.inflation.car);
    sam.car_one.insurance_monthly = apply_inflation(sam.car_one.insurance_monthly,  country.inflation.car);
    sam.car_one.maintenance_yearly = apply_inflation(sam.car_one.maintenance_yearly, country.inflation.car);
    sam.car_one.last_index_year = year;
}

void sam_personal_inflation_apply(const int year) {
    if (sam.personal_last_index_year >= year) return;
    sam.food_monthly = apply_inflation(sam.food_monthly, country.inflation.food);
    sam.medicine_monthly = apply_inflation(sam.medicine_monthly, country.inflation.medicine);
    sam.clothing_monthly = apply_inflation(sam.clothing_monthly, country.inflation.clothing);
    sam.education_monthly = apply_inflation(sam.education_monthly,country.inflation.education);
    sam.personal_last_index_year = year;
}

// Коммунальные

RUB sam_utility_gas(const int year) {
    return inflate_rate_if_needed(sam.flat_one.gas_rate, sam.flat_one.gas_year, year, country.inflation.utilities);
}

RUB sam_utility_cold_water(const int year) {
    return inflate_rate_if_needed(sam.flat_one.cold_water_rate, sam.flat_one.cold_water_year, year, country.inflation.utilities);
}

RUB sam_utility_hot_water(const int year) {
    return inflate_rate_if_needed(sam.flat_one.hot_water_rate, sam.flat_one.hot_water_year, year, country.inflation.utilities);
}

RUB sam_utility_electricity(const int year) {
    return inflate_rate_if_needed(sam.flat_one.electricity_rate, sam.flat_one.electricity_year, year, country.inflation.utilities);
}

RUB sam_utility_internet(const int year) {
    return inflate_rate_if_needed(sam.flat_one.internet_rate, sam.flat_one.internet_year, year, country.inflation.utilities);
}

RUB sam_utility_maintenance(const int year) {
    return inflate_rate_if_needed(sam.flat_one.maintenance_rate, sam.flat_one.maintenance_year, year, country.inflation.utilities);
}

void sam_utilities(const int year)
{
    if (!sam.flat_one.is_presence) return;
    RUB total = 0;
    total += sam_utility_gas(year);
    total += sam_utility_cold_water(year);
    total += sam_utility_hot_water(year);
    total += sam_utility_electricity(year);
    total += sam_utility_internet(year);
    total += sam_utility_maintenance(year);
    RUB bills = total;
    try_pay(sam.account.currency, bills);
}

////////////////////////////

void sam_employer_pay(Employer& emp, const int year, const int month)
{
    if (!emp.is_active) return;

    for (int i = 0; i < emp.salary_schedule_size; ++i) {
        const SalaryEvent& ev = emp.salary_schedule[i];
        if (ev.year == year && ev.month == month) {
            emp.salary = emp.salary_levels[i];
        }
    }

    if (emp.last_index_year < year) {
        emp.salary = apply_inflation(emp.salary, country.inflation.salary);
        emp.last_index_year = year;
    }

    sam.account.currency += emp.salary;
    sam.account.year_income += emp.salary;
}

void sam_side_job_start(const int year, const int month)
{
    if (sam.job_side.is_active) return;
    if (year < 2028 || (year == 2028 && month < 3)) return;

    sam.job_side.is_active = true;
    sam.job_side.company = "Фриланс";
    sam.job_side.salary = 40'000;
    sam.job_side.last_index_year = year;

    sam.job_side.salary_schedule[0] = {2030, 6};
    sam.job_side.salary_levels[0]   = 55'000;
    sam.job_side.salary_schedule[1] = {2032, 1};
    sam.job_side.salary_levels[1]   = 70'000;
    sam.job_side.salary_schedule_size = 2;
}

void sam_salary(const int year, const int month)
{
    sam_side_job_start(year, month);

    sam_employer_pay(sam.job,      year, month);
    sam_employer_pay(sam.job_side, year, month);

    // суммарный доход (для проверок)
    sam.salary = sam.job.salary + sam.job_side.salary;
}

void sam_car_buy(const int year, const int month) 
{
    if (!sam.flat_one.is_presence) return;

    if (sam.car_one.is_presence) {
        return;
    }
    
    if (year > 2030 || (year == 2030 && month >= 6)) {                                                 // появление желания купить авто
        if (sam.account.currency >= 500'000 && sam.salary >= 100'000 && !(sam.car_one.is_presence)) {        // появление возможности купить авто
            sam.car_one.is_presence = true;
            sam.car_one.power = 90;
            sam.car_one.hp = 255;
            sam.car_one.max_hp = 255;
            sam.car_one.broken = false;
            sam.car_one.age = 0;
            sam.car_one.purchase_price = 300'000;
            sam.account.currency -= 300'000;

            }
        }
    else {
        return;
    }    
}

void sam_car_monthly_costs() {

    if (!sam.car_one.is_presence) {
        return;
    }
    RUB cost = sam.car_one.fuel_monthly + sam.car_one.insurance_monthly;
    try_pay(sam.account.currency, cost);
}

RUB sam_car_sale_price() {
    if (!sam.car_one.is_presence) return 0;
    double condition = (double)sam.car_one.hp / sam.car_one.max_hp;
    double age_factor = 1.0 - (sam.car_one.age * 0.1);
    if (age_factor < 0.1) age_factor = 0.1;
    double price = sam.car_one.purchase_price * condition * age_factor;
    return (RUB)price;
}

void sam_car_sell()
{
    if (!sam.car_one.is_presence) return;
    if (sam.account.currency > 50'000 || sam.salary > 100'000) return;

    RUB price = sam_car_sale_price();
    sam.car_one.is_presence = false;
    sam.car_one.power = 0;
    sam.account.currency += price;
    sam.account.year_income += price;
}

void sam_car(const int year, const int month) // добавить возраст авто
{
    sam_car_buy(year, month);
    sam_car_monthly_costs();
    sam_car_sell();
}

void sam_car_use(const int year, const int month) 
{
    if (!sam.car_one.is_presence) {
        return;
    }
    if (sam.car_one.broken) {
        return;
    }
    //Постепенный износ
    sam.car_one.hp -= 1 + rand() % 3;
    if (sam.car_one.hp < 0) {
        sam.car_one.hp = 0;
    }

    //вероятность поломки квадрат износа
    double wear = 1.0 - (double)sam.car_one.hp / sam.car_one.max_hp; // отношение текущего хп к максимального
    double break_chance = wear * wear;

    double roll = (double)rand() / RAND_MAX;
    if (roll < break_chance) {
        sam.car_one.broken = true;
    }
}

void sam_car_repair(const int year, const int month)
{
    if (!sam.car_one.is_presence) return;
    if (!sam.car_one.broken) return;

    RUB cost = 5'000 + rand() % 95'001;

if (!try_pay(sam.account.currency, cost)) return;

    if (sam.account.currency < cost) {
        return;
    }

    // макс хп падает и тем сильнее чем больше стоимость
    int hp_loss = 1 + (int)((double)cost / 100'000.0 * 9);
    sam.car_one.max_hp -= hp_loss;
    
    if (sam.car_one.max_hp < 1) {
        sam.car_one.max_hp = 1;
    }

    sam.car_one.hp = sam.car_one.max_hp;
    sam.car_one.broken = false;
}

//налог на транспорт   !! сделать оплату в марте
RUB sam_transport_tax_yearly() 
{
    if (!sam.car_one.is_presence) return 0;

    int power = sam.car_one.power;

    RUB rate;   // ставка за 1 л.с.
    if (power <= 100) rate = 12;
    else if (power <= 125) rate = 25;
    else if (power <= 150) rate = 35;
    else if (power <= 175) rate = 45;
    else if (power <= 200) rate = 50;
    else if (power <= 225) rate = 65;
    else if (power <= 250) rate = 75;
    else rate = 150;
    return (RUB)(power * rate);
}

//налог на имущество !!сделать оплату в декабре
RUB sam_property_tax_yearly()
{
    if (!sam.flat_one.is_presence) return 0;
    return sam.flat_one.property_tax_yearly;
}

//медицина усложнить, инвестиционный счёт, аккуратно описать налоговую систему

// система вклада
// пополнение из сфободных денег
void sam_deposit_add()
{
    if (sam.account.currency > 200'000) {
        RUB put = (sam.account.currency - 200'000) / 10;
        sam.account.currency -= put;
        sam.account.deposit += put;
    }
}

//проценты раз в месяц
void sam_deposit_interest()
{
    RUB interest = (RUB)(sam.account.deposit * 0.005);
    sam.account.deposit += interest;
    sam.account.year_income += interest;
}

//съём с вклада, если мало денег
void sam_deposit_withdraw()
{
    if (sam.account.currency < 50'000 && sam.account.deposit > 0) {
        RUB need = 50'000 - sam.account.currency;
        if (need > sam.account.deposit) {
            need = sam.account.deposit;
        }
        sam.account.deposit -= need;
        sam.account.currency += need;
    }
}

void sam_food(const int year) 
{
    try_pay(sam.account.currency, sam.food_monthly);
}

void sam_medicine(const int year, const int month)
{
    // плановые траты
    if (try_pay(sam.account.currency, sam.medicine_monthly)) {
        sam.tax.medicine_expenses_this_year += sam.medicine_monthly;
    }

    // случайная болезнь
    double sick_chance = 0.02 + (100 - sam.health) * 0.002;
    double roll = (double)rand() / RAND_MAX;
    if (roll < sick_chance) {
        RUB cost = 10'000 + rand() % 50'000;
        if (try_pay(sam.account.currency, cost)) {
            sam.tax.medicine_expenses_this_year += cost;
        }
        sam.health -= 5 + rand() % 10;
        if (sam.health < 0) sam.health = 0;
    } else {
        if (sam.health < 100) sam.health++;
    }
}

void sam_clothing() {
    try_pay(sam.account.currency, sam.clothing_monthly);
}

void sam_education() {
    if (try_pay(sam.account.currency, sam.education_monthly)) {
        sam.tax.education_expenses_this_year += sam.education_monthly;
    }
}

void sam_rent_inflation_apply(const int year)
{
    if (sam.rent_last_index_year >= year) return;
    if (sam.rent_monthly > 0) {
        sam.rent_monthly = apply_inflation(sam.rent_monthly, country.inflation.rent);
    }
    sam.rent_last_index_year = year;
}

void sam_rent(const int year, const int month) 
{   
    if (sam.flat_one.is_presence) {
        return;
    }

    int tier = 0;
    RUB base = 0;

    if (year < 2027 || (year == 2027 && month <= 8)) {
        tier = 0; base = 0;
    }
    else if (year < 2028 || (year == 2028 && month <= 6)) {
        tier = 1; base = 20'000;
    }
    else {
        tier = 2; base = 40'000;
    }

    if (sam.rent_tier != tier) {
        sam.rent_tier = tier;
        sam.rent_monthly = base;
    }

    try_pay(sam.account.currency, sam.rent_monthly);
}

void sam_home_bills(const int year, const int month)  
{
    RUB bills = 0;
    
    if (year > 2033 || (year == 2033 && month >= 3) ) {
        bills = 25'000;
    }
    else if (year > 2030 || (year == 2030 && month >= 4) ) {
        bills = 20'000;
    }
    else if (year > 2028 || (year == 2028 && month >= 6) ) {
        bills = 10'000;
    }
    else {
        bills = 5'000;
    }

    try_pay(sam.account.currency, bills);
}

void sam_dog_buy(const int year, const int month)
{
    if (sam.dog.is_presence || sam.dog.had_dog) {
        return;
    }
        if (sam.salary <= 100'000 || !sam.flat_one.is_presence) {
        return;
    }

    sam.dog.is_presence = true;
    sam.dog.death_year = year + 12;
    sam.dog.death_month = month;
    try_pay(sam.account.currency, 20'000);
}

void sam_dog_upkeep(const int year, const int month)
{
    if (!sam.dog.is_presence) {
        return;
    }
    
    bool alive = (year < sam.dog.death_year) || (year == sam.dog.death_year && month < sam.dog.death_month);

    if (!alive) {
        sam.dog.is_presence = false;
        sam.dog.had_dog = true;
        try_pay(sam.account.currency, 15'000);
        return;
    }

    try_pay(sam.account.currency, sam.dog.food_monthly);
}

void sam_dog_vet(const int month) {
    if (!sam.dog.is_presence) return;
    if (month == sam.dog.vet_month) {
        try_pay(sam.account.currency, sam.dog.vet_yearly);
    }
}

void sam_dog(const int year, const int month) 
{
    sam_dog_buy(year, month);
    sam_dog_upkeep(year, month);
    sam_dog_vet(month);
}

void sam_ndfl(const int month) 
{
   RUB owed = sum_tax(sam.account.year_income);
   
    if (owed > sam.tax.ndfl_paid_this_year) {
        RUB delta = owed - sam.tax.ndfl_paid_this_year;
        try_pay(sam.account.currency, delta);
    }
   sam.tax.ndfl_paid_this_year = owed;

    // обнуление годовой статистики
    if (month == 12) {
        sam.tax.ndfl_paid_last_year = sam.tax.ndfl_paid_this_year;
        sam.account.last_last_year_income = sam.account.last_year_income;
        sam.account.last_year_income = sam.account.year_income;
        sam.account.year_income = 0;
        sam.tax.ndfl_paid_this_year = 0;

        sam.tax.mortgage_expenses_last_year = sam.tax.mortgage_expenses_this_year;
        sam.tax.education_expenses_last_year = sam.tax.education_expenses_this_year;
        sam.tax.medicine_expenses_last_year = sam.tax.medicine_expenses_this_year;
        sam.tax.mortgage_expenses_this_year = 0;
        sam.tax.education_expenses_this_year = 0;
        sam.tax.medicine_expenses_this_year = 0;
    }
}

//возвраты в четвёртый месяц раз в год
void sam_tax_refund(const int month)
{
    if (month != 4) return;
    RUB income = sam.account.last_year_income;

    // предел на имуществ выч 2 млн и 3 млн за проценты
    RUB mortgage_cap = sam.tax.mortgage_body_deduction_cap + sam.tax.mortgage_interest_deduction_cap;
    RUB mortgage_ded = sam.tax.mortgage_expenses_last_year;
    if (mortgage_ded > mortgage_cap) {
        mortgage_ded = mortgage_cap;
    }

    // социальные вычеты база 150 тыс
    RUB social_ded = sam.tax.education_expenses_last_year + sam.tax.medicine_expenses_last_year;
    if (social_ded > sam.tax.social_deduction_cap) {
        social_ded = sam.tax.social_deduction_cap;
    }

    // складываем вычеты, проверка превышения
    RUB total_deduction = mortgage_ded + social_ded;
    if (total_deduction > income) total_deduction = income;

    RUB total = tax_refund_by_deduction(income, total_deduction);

    if (total > sam.tax.ndfl_paid_last_year) {
        total = sam.tax.ndfl_paid_last_year;
    }

    sam.tax.refund_accum += total;
    sam.account.currency += total;
}

void sam_property_tax_inflation_apply(const int year)
{
    if (sam.flat_one.property_tax_year >= year) return;
    if (sam.flat_one.is_presence) {
        sam.flat_one.property_tax_yearly =
            apply_inflation(sam.flat_one.property_tax_yearly, country.inflation.utilities);
    }
    sam.flat_one.property_tax_year = year;
}

//кредитная система
//проверка возможности взять кредит
int sam_find_free_credit_slot()
{
    for (int i = 0; i < 5; ++i) {
        if (!sam.credits[i].is_active) {
            return i;
        }
    }
    return -1; ///-1?
}

//создание кредита
void sam_credit_init(Credit& c, const char* name, RUB principal, double rate, int term_months)
{
    if (sam_find_free_credit_slot() == -1) return;

    c.is_active = true;
    c.is_closed = false;
    c.name = name;
    c.principal = principal;
    c.remaining = principal;
    c.rate = rate;
    c.term_months = term_months;
    c.months_paid = 0;
    c.overdue_months = 0;
    c.penalty = 0;
    c.total_overdue = 0;

    //платёж. слизан с аннуитетного, но проще. прям сильно
    double monthly_rate = rate / 12.0;
    double factor = 1.0;
    for (int i = 0; i < term_months; ++i) {
        factor *= (1.0 + monthly_rate);
    }
    double payment = (double)principal * monthly_rate * factor / (factor - 1.0);
    c.monthly_payment = (RUB)payment;
}

//Платёж если есть деньги, иначе минус рейтинг
void sam_credit_process_payment(Credit& c)
{
    if (!c.is_active || c.is_closed) return;
    RUB need = c.monthly_payment + c.penalty;
    if (try_pay(sam.account.currency, need)) {
        if (c.remaining >= c.monthly_payment) {
            c.remaining -= c.monthly_payment;
        } else {
            c.remaining = 0;
        }
        c.penalty = 0;
        c.months_paid++;
        c.overdue_months = 0;
        if (c.remaining <= c.monthly_payment) {
            c.remaining = 0;
            c.is_closed = true;
            c.is_active = false;
            if (sam.account.credit_score < 100) {
                    sam.account.credit_score += 5;
            }
        }

        if (sam.account.credit_score < 100) {
            sam.account.credit_score += 1;
        }
    }
    else {
        c.overdue_months++;
        c.total_overdue += c.monthly_payment;
        credit_penalty(c);
        
        //просрочка --> - рейтинг
        if (sam.account.credit_score > 0) {
            sam.account.credit_score -= 3;
            if (sam.account.credit_score < 0) {
                sam.account.credit_score = 0;
            }
        }
    }  
}

//прогон всех кредитов
void sam_credit_payments()
{
    for (int i = 0; i < 5; ++i) {
        if (!sam.credits[i].is_active) continue;
        credit_accrue_monthly(sam.credits[i]);
        sam_credit_process_payment(sam.credits[i]);
    }
}

//обновление рейтинга
void sam_credit_score_update()
{
    int is_active_credits = 0;
    int total_overdue = 0;

    for (int i = 0; i < 5; ++i) {
        if (sam.credits[i].is_active) {
            is_active_credits++;
            total_overdue += sam.credits[i].overdue_months;
        }
    }
    if (is_active_credits >= 3) {
        if (sam.account.credit_score > 0) sam.account.credit_score -= 1;
    }
        if (total_overdue > 6) {
        if (sam.account.credit_score > 0) sam.account.credit_score -= 2;
    }
    if (sam.account.credit_score > 100) sam.account.credit_score = 100;
    if (sam.account.credit_score < 0)   sam.account.credit_score = 0;
}

//ипотека
void sam_mortgage_issue(const int year, const int month)
{
    if (sam.flat_one.is_presence) return;

    if (year > 2033 || (year == 2033 && month >= 3)) {
        int slot = sam_find_free_credit_slot();
        if (slot != -1) {
        sam_credit_init(sam.mortgage, "ипотека", 8'000'000, 0.08, 300);
        sam.flat_one.is_presence = true;
        sam.flat_one.purchase_price = 8'000'000;
        sam.flat_one.property_tax_yearly = 8'000;
        }
    }
}

void sam_mortgage_payment(const int year, const int month)
{
    if (sam.mortgage.remaining == 0) return;
    
    credit_accrue_monthly(sam.mortgage);
    sam_credit_process_payment(sam.mortgage);
}

void sam_mortgage(const int year, const int month)
{
    sam_mortgage_issue(year, month);
    sam_mortgage_payment(year, month);
}

//инвестиции около 12% в год
void sam_investment_income()
{
    if (sam.investment == 0) return;
    RUB income = (RUB)((double)sam.investment * 0.01);
    sam.investment += income;
    sam.account.year_income += income;
}

// перекладываем свободные деньги в инвестиции
void sam_investment_add()
{
    if (sam.account.currency > 300'000) {
        RUB put = (sam.account.currency - 300'000) / 5;
        sam.account.currency -= put;
        sam.investment += put;
    }
}

// снимаем, если не хватает на жизнь
void sam_investment_withdraw()
{
    if (sam.account.currency < 30'000 && sam.investment > 0) {
        RUB need = 30'000 - sam.account.currency;
        if (need > sam.investment) need = sam.investment;
        sam.investment -= need;
        sam.account.currency += need;
    }
}
//
void sam_capitalize_assets() {
     if (sam.car_one.is_presence) {
        RUB price = sam_car_sale_price();
        sam.account.currency += price;
        sam.car_one.is_presence = false;
    }
    if (sam.flat_one.is_presence) {
        RUB flat_price = sam.flat_one.purchase_price;
        sam.account.currency += flat_price;
        if (sam.mortgage.remaining > 0) {
            if (sam.account.currency >= sam.mortgage.remaining) {
                sam.account.currency -= sam.mortgage.remaining;
                sam.mortgage.remaining = 0;
                sam.mortgage.is_active = false;
                sam.mortgage.is_closed = true;
            } else {
                sam.mortgage.remaining -= sam.account.currency;
                sam.account.currency = 0;
            }
        }
        sam.flat_one.is_presence = false;
    }
    if (sam.dog.is_presence) {
        sam.account.currency += 5000;
        sam.dog.is_presence = false;
    }
}

void sam_year_taxes_and_inflation(const int year, const int month) 
{
    if (month == 3)  try_pay(sam.account.currency, sam_transport_tax_yearly());
    if (month == 12) try_pay(sam.account.currency, sam_property_tax_yearly());
    if (month == 1)  {
        sam_pet_inflation_apply(year);
        sam_car_inflation_apply(year);
        sam_personal_inflation_apply(year);
        //food_inflation_apply(year);
        sam_property_tax_inflation_apply(year);
        sam_rent_inflation_apply(year); 
        if (sam.car_one.is_presence) sam.car_one.age++;
    }
}

void sam_mortgage_track_expenses(const int year, const int month)
{
    RUB before = sam.mortgage.remaining;
    sam_mortgage(year, month);
    if (before > sam.mortgage.remaining) {
        sam.tax.mortgage_expenses_this_year += before - sam.mortgage.remaining;
    }
}

void SAM(const int year, const int month)
{
    sam_salary(year, month);

        sam_mortgage_track_expenses(year, month);
        sam_rent(year, month);
        sam_utilities(year);
        sam_food(year);
        sam_medicine(year, month);
        sam_clothing();
        sam_education();

        sam_dog(year, month);
       
        sam_ndfl(month);

        sam_car(year, month);
        sam_car_use(year, month);
        sam_car_repair(year, month);
       
        sam_credit_payments();
        sam_credit_score_update();


        //годовые налоги и инфляция
        sam_year_taxes_and_inflation(year, month);

        //возврат налогов
        sam_tax_refund(month);

        sam_deposit_interest();
        sam_deposit_add();
        sam_deposit_withdraw();

        sam_investment_income();
        sam_investment_add();
        sam_investment_withdraw();
}

/////////////////////////////////////////////////////////////////////////

void simulation() 
{
    srand((unsigned)time(0));

    int year = 2026;
    int month = 9;

    RUB mortgage_paid_this_year = 0;
    RUB education_paid_this_year = 0;
    RUB medicine_paid_this_year = 0;
    RUB mortgage_paid_last_year = 0;
    RUB education_paid_last_year = 0;
    RUB medicine_paid_last_year = 0;

    while ( !(year == 2060 && month == 9) ) { 

        BOB(year, month);
        SAM(year, month);

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

void inflation_init()
{
    country.inflation.food = 0.08;
    country.inflation.utilities = 0.07;
    country.inflation.rent = 0.06;
    country.inflation.car = 0.05;
    country.inflation.pet = 0.07;
    country.inflation.medicine = 0.09;
    country.inflation.clothing = 0.05;
    country.inflation.education = 0.06;
    country.inflation.services = 0.08;
    country.inflation.salary = 0.08;
}

void bob_init() 
{
    bob.account.currency = 20'000;
    bob.salary = 80'000;

    bob.account.deposit = 0;
    bob.tax.ndfl_paid_this_year = 0;
    bob.account.year_income = 0;
    bob.account.last_year_income = 0;
    bob.account.last_last_year_income = 0;
    bob.tax.refund_accum = 0;
    bob.account.credit_score = 70;
    
    bob.flat_one.is_presence = false;      // наличие квартиры
    bob.flat_one.gas_rate = 500;          //тарифы
    bob.flat_one.gas_year = 2026;
    bob.flat_one.cold_water_rate = 400;
    bob.flat_one.cold_water_year = 2026;
    bob.flat_one.hot_water_rate = 700;
    bob.flat_one.hot_water_year = 2026;
    bob.flat_one.electricity_rate = 900;
    bob.flat_one.electricity_year = 2026;
    bob.flat_one.internet_rate = 700;
    bob.flat_one.internet_year = 2026;
    bob.flat_one.maintenance_rate = 3'000;
    bob.flat_one.maintenance_year = 2026;
    bob.flat_one.property_tax_year = 2026;
    
    bob.dog.is_presence = false;       // наличие собаки
    bob.dog.had_dog = false;
    bob.dog.food_monthly = 7000;
    bob.dog.vet_yearly = 15'000;
    bob.dog.vet_month = 6;

    bob.car_one.is_presence = false;         //наличие машины
    bob.car_one.power = 0;                //мощность двигателя
    bob.car_one.hp = 0;                   //прочность машины
    bob.car_one.max_hp = 255;             //максимальная прочность машины
    bob.car_one.broken = false;           // индикатор поломки
    bob.car_one.fuel_monthly = 10'000;
    bob.car_one.insurance_monthly = 5'000;
    bob.car_one.maintenance_yearly = 30'000;
    bob.car_one.maintenance_month = 4;

    /*bob.job_side.is_active = false;
    bob.job_side.company = "";      // вторая работа
    bob.job_side.salary = 0;*/

    bob.mortgage.is_active = false;
    bob.mortgage.is_closed = false;
    bob.mortgage.name = "ипотека";
    bob.mortgage.principal = 0;
    bob.mortgage.remaining = 0;
    bob.mortgage.rate = 0;
    bob.mortgage.monthly_payment = 0;
    bob.mortgage.term_months = 0;
    bob.mortgage.months_paid = 0;
    bob.mortgage.overdue_months = 0;
    bob.mortgage.penalty = 0;
    bob.mortgage.total_overdue = 0;

     bob.credits_count = 0;
    for (int i = 0; i < 5; ++i) {
        bob.credits[i].is_active = false;
        bob.credits[i].is_closed = false;
        bob.credits[i].name = "";
        bob.credits[i].principal = 0;
        bob.credits[i].remaining = 0;
        bob.credits[i].rate = 0;
        bob.credits[i].monthly_payment = 0;
        bob.credits[i].term_months = 0;
        bob.credits[i].months_paid = 0;
        bob.credits[i].overdue_months = 0;
        bob.credits[i].penalty = 0;
        bob.credits[i].total_overdue = 0;
    }

    bob.medicine_monthly = 3'000;
    bob.health = 90;
    bob.clothing_monthly = 5'000;
    bob.education_monthly = 0;
    bob.food_monthly = 7'000;

    bob.dog.last_index_year = 2026;
    bob.car_one.last_index_year = 2026;

    bob.rent_monthly = 0;
    bob.rent_tier = 0;
    bob.rent_last_index_year = 2026;
    bob.personal_last_index_year = 2026;

    bob.tax.ndfl_paid_last_year = 0;
    bob.flat_one.purchase_price = 0;
    bob.dog.purchase_price = 0;
    bob.car_one.age = 0;
    bob.car_one.purchase_price = 0;
    bob.job.is_active = true;
    bob.job.company = "Рога и копыта";
    bob.investment = 0;

    bob.job_side.is_active = false;
    bob.job_side.company = "";
    bob.job_side.salary = 0;
    bob.job_side.last_index_year = 2026;
    bob.job_side.salary_schedule_size = 0;

    bob.job.salary_schedule[0] = {2027, 1,  };
    bob.job.salary_levels[0]   = 100'000;
    bob.job.salary_schedule[1] = {2029, 5,  };
    bob.job.salary_levels[1]   = 140'000;
    bob.job.salary_schedule[2] = {2030, 1,  };
    bob.job.salary_levels[2]   = 180'000;
    bob.job.salary_schedule[3] = {2030, 12, };
    bob.job.salary_levels[3]   = 150'000;
    bob.job.salary_schedule[4] = {2032, 11, };
    bob.job.salary_levels[4]   = 200'000;
    bob.job.salary_schedule_size = 5;

    bob.tax.ndfl_paid_this_year = 0;
    bob.tax.ndfl_paid_last_year = 0;
    bob.tax.refund_accum = 0;
    bob.tax.mortgage_expenses_this_year  = 0;
    bob.tax.mortgage_expenses_last_year  = 0;
    bob.tax.education_expenses_this_year = 0;
    bob.tax.education_expenses_last_year = 0;
    bob.tax.medicine_expenses_this_year  = 0;
    bob.tax.medicine_expenses_last_year  = 0;
    bob.tax.transport_tax_yearly = 0;
    bob.tax.property_tax_yearly = 0;
    bob.tax.mortgage_body_deduction_cap = 2'000'000;
    bob.tax.mortgage_interest_deduction_cap = 3'000'000;
    bob.tax.social_deduction_cap = 150'000;

    inflation_init();
} 

void sam_init() 
{
    sam.account.currency = 20'000;
    sam.salary = 80'000;

    sam.account.deposit = 0;
    sam.tax.ndfl_paid_this_year = 0;
    sam.account.year_income = 0;
    sam.account.last_year_income = 0;
    sam.account.last_last_year_income = 0;
    sam.tax.refund_accum = 0;
    sam.account.credit_score = 70;
    
    sam.flat_one.is_presence = false;      // наличие квартиры
    sam.flat_one.gas_rate = 500;          //тарифы
    sam.flat_one.gas_year = 2026;
    sam.flat_one.cold_water_rate = 400;
    sam.flat_one.cold_water_year = 2026;
    sam.flat_one.hot_water_rate = 700;
    sam.flat_one.hot_water_year = 2026;
    sam.flat_one.electricity_rate = 900;
    sam.flat_one.electricity_year = 2026;
    sam.flat_one.internet_rate = 700;
    sam.flat_one.internet_year = 2026;
    sam.flat_one.maintenance_rate = 3'000;
    sam.flat_one.maintenance_year = 2026;
    sam.flat_one.property_tax_year = 2026;
    
    sam.dog.is_presence = false;       // наличие собаки
    sam.dog.had_dog = false;
    sam.dog.food_monthly = 7000;
    sam.dog.vet_yearly = 15'000;
    sam.dog.vet_month = 6;

    sam.car_one.is_presence = false;         //наличие машины
    sam.car_one.power = 0;                //мощность двигателя
    sam.car_one.hp = 0;                   //прочность машины
    sam.car_one.max_hp = 255;             //максимальная прочность машины
    sam.car_one.broken = false;           // индикатор поломки
    sam.car_one.fuel_monthly = 10'000;
    sam.car_one.insurance_monthly = 5'000;
    sam.car_one.maintenance_yearly = 30'000;
    sam.car_one.maintenance_month = 4;

    /*sam.job_side.is_active = false;
    sam.job_side.company = "";      // вторая работа
    sam.job_side.salary = 0;*/

    sam.mortgage.is_active = false;
    sam.mortgage.is_closed = false;
    sam.mortgage.name = "ипотека";
    sam.mortgage.principal = 0;
    sam.mortgage.remaining = 0;
    sam.mortgage.rate = 0;
    sam.mortgage.monthly_payment = 0;
    sam.mortgage.term_months = 0;
    sam.mortgage.months_paid = 0;
    sam.mortgage.overdue_months = 0;
    sam.mortgage.penalty = 0;
    sam.mortgage.total_overdue = 0;

     sam.credits_count = 0;
    for (int i = 0; i < 5; ++i) {
        sam.credits[i].is_active = false;
        sam.credits[i].is_closed = false;
        sam.credits[i].name = "";
        sam.credits[i].principal = 0;
        sam.credits[i].remaining = 0;
        sam.credits[i].rate = 0;
        sam.credits[i].monthly_payment = 0;
        sam.credits[i].term_months = 0;
        sam.credits[i].months_paid = 0;
        sam.credits[i].overdue_months = 0;
        sam.credits[i].penalty = 0;
        sam.credits[i].total_overdue = 0;
    }

    sam.medicine_monthly = 3'000;
    sam.health = 90;
    sam.clothing_monthly = 5'000;
    sam.education_monthly = 0;
    sam.food_monthly = 7'000;

    sam.dog.last_index_year = 2026;
    sam.car_one.last_index_year = 2026;

    sam.rent_monthly = 0;
    sam.rent_tier = 0;
    sam.rent_last_index_year = 2026;
    sam.personal_last_index_year = 2026;

    sam.tax.ndfl_paid_last_year = 0;
    sam.flat_one.purchase_price = 0;
    sam.dog.purchase_price = 0;
    sam.car_one.age = 0;
    sam.car_one.purchase_price = 0;
    sam.job.is_active = true;
    sam.job.company = "Рога и копыта";
    sam.investment = 0;

    sam.job_side.is_active = false;
    sam.job_side.company = "";
    sam.job_side.salary = 0;
    sam.job_side.last_index_year = 2026;
    sam.job_side.salary_schedule_size = 0;

    sam.job.salary_schedule[0] = {2027, 1,  };
    sam.job.salary_levels[0]   = 100'000;
    sam.job.salary_schedule[1] = {2029, 5,  };
    sam.job.salary_levels[1]   = 140'000;
    sam.job.salary_schedule[2] = {2030, 1,  };
    sam.job.salary_levels[2]   = 180'000;
    sam.job.salary_schedule[3] = {2030, 12, };
    sam.job.salary_levels[3]   = 150'000;
    sam.job.salary_schedule[4] = {2032, 11, };
    sam.job.salary_levels[4]   = 200'000;
    sam.job.salary_schedule_size = 5;

    sam.tax.ndfl_paid_this_year = 0;
    sam.tax.ndfl_paid_last_year = 0;
    sam.tax.refund_accum = 0;
    sam.tax.mortgage_expenses_this_year  = 0;
    sam.tax.mortgage_expenses_last_year  = 0;
    sam.tax.education_expenses_this_year = 0;
    sam.tax.education_expenses_last_year = 0;
    sam.tax.medicine_expenses_this_year  = 0;
    sam.tax.medicine_expenses_last_year  = 0;
    sam.tax.transport_tax_yearly = 0;
    sam.tax.property_tax_yearly = 0;
    sam.tax.mortgage_body_deduction_cap = 2'000'000;
    sam.tax.mortgage_interest_deduction_cap = 3'000'000;
    sam.tax.social_deduction_cap = 150'000;

    inflation_init();
} 

void sam_print() 
{
    printf("sam cash = %llu\n", sam.account.currency);
    printf("sam deposit = %llu\n", sam.account.deposit);
    printf("sam investment = %llu\n", sam.investment);
    printf("Credit score = %d\n", sam.account.credit_score);
}

void bob_print() 
{
    printf("bob cash = %llu\n", bob.account.currency);
    printf("bob deposit = %llu\n", bob.account.deposit);
    printf("bob investment = %llu\n", bob.investment);
    printf("Credit score = %d\n", bob.account.credit_score);
}


int main () 
{
    bob_init();

    sam_init();

    simulation();

    bob_capitalize_assets(); 

    bob_print();

    sam_capitalize_assets(); 

    sam_print();

    return 0;
}