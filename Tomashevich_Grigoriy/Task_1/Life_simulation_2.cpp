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
    RUB mortgage_deduction_cap;
    RUB social_deduction_cap;
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

    // inflation inflation; // вынести в страну

    Employer job;
    Credit credits[5];
    int credits_count;
    RUB medicine_monthly;
    RUB clothing_monthly;
    RUB education_monthly;
    RUB investment;
    int personal_last_index_year;
    Tax tax;
};

struct Country {
    Inflation inflation;
};

struct Person bob;
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

void pet_inflation_apply(const int year) {
    if (bob.dog.last_index_year >= year) return;
    bob.dog.food_monthly = apply_inflation(bob.dog.food_monthly, country.inflation.pet);
    bob.dog.vet_yearly = apply_inflation(bob.dog.vet_yearly,  country.inflation.pet);
    bob.dog.last_index_year = year;
}

void car_inflation_apply(const int year) {
    if (bob.car_one.last_index_year >= year) return;
    bob.car_one.fuel_monthly = apply_inflation(bob.car_one.fuel_monthly,       country.inflation.car);
    bob.car_one.insurance_monthly = apply_inflation(bob.car_one.insurance_monthly,  country.inflation.car);
    bob.car_one.maintenance_yearly = apply_inflation(bob.car_one.maintenance_yearly, country.inflation.car);
    bob.car_one.last_index_year = year;
}

void personal_inflation_apply(const int year) {
    if (bob.personal_last_index_year >= year) return;
    bob.medicine_monthly = apply_inflation(bob.medicine_monthly,  country.inflation.medicine);
    bob.clothing_monthly = apply_inflation(bob.clothing_monthly,  country.inflation.clothing);
    bob.education_monthly = apply_inflation(bob.education_monthly, country.inflation.education);
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

RUB utility_gas(const int year) {
    return inflate_rate_if_needed(bob.flat_one.gas_rate, bob.flat_one.gas_year, year, country.inflation.utilities);
}

RUB utility_cold_water(const int year) {
    return inflate_rate_if_needed(bob.flat_one.cold_water_rate, bob.flat_one.cold_water_year, year, country.inflation.utilities);
}

RUB utility_hot_water(const int year) {
    return inflate_rate_if_needed(bob.flat_one.hot_water_rate, bob.flat_one.hot_water_year, year, country.inflation.utilities);
}

RUB utility_electricity(const int year) {
    return inflate_rate_if_needed(bob.flat_one.electricity_rate, bob.flat_one.electricity_year, year, country.inflation.utilities);
}

RUB utility_internet(const int year) {
    return inflate_rate_if_needed(bob.flat_one.internet_rate, bob.flat_one.internet_year, year, country.inflation.utilities);
}

RUB utility_maintenance(const int year) {
    return inflate_rate_if_needed(bob.flat_one.maintenance_rate, bob.flat_one.maintenance_year, year, country.inflation.utilities);
}

void bob_utilities(const int year)
{
    if (!bob.flat_one.is_presence) return;
    RUB total = 0;
    total += utility_gas(year);
    total += utility_cold_water(year);
    total += utility_hot_water(year);
    total += utility_electricity(year);
    total += utility_internet(year);
    total += utility_maintenance(year);
    RUB bills = total;
    try_pay(bob.account.currency, bills);
}

////////////////////////////

/*const SalaryEvent bob_salary_schedule[] = {
    {2027, 1, 100'000},     // переход со стажировки
    {2029, 5, 140'000},     // смена работы
    {2030, 1, 180'000},     // повышение
    {2030, 12, 150'000},    // понижение
    {2032, 11, 200'000},    // повышение
};*/

//const int bob_salary_schedule_size = sizeof(bob_salary_schedule) / sizeof(bob_salary_schedule[0]);

void bob_salary(const int year, const int month) 
{
        for (int i = 0; i < bob.job.salary_schedule_size; ++i) {
        const SalaryEvent& ev = bob.job.salary_schedule[i];
        if (ev.year == year && ev.month == month) {
            bob.job.salary = bob.job.salary_levels[i];
        }
    }
    
   if (bob.job.last_index_year < year) {
        bob.job.salary = apply_inflation(bob.job.salary, country.inflation.salary);
        bob.job.last_index_year = year;
   }

    bob.account.currency += bob.salary;
    bob.account.year_income += bob.salary;
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

RUB car_sale_price() {
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
    RUB price = car_sale_price();
    bob.car_one.is_presence = false;
    bob.car_one.power = 0;
    bob.account.currency += 200'000;
    bob.account.year_income += 200'000;
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


//медецина усложнить, инвестиционный счёт, аккуратно описать налоговую систему

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
    bob.account.currency -= (RUB)7000;
}

void bob_medicine() {
    try_pay(bob.account.currency, bob.medicine_monthly);
}

void bob_clothing() {
    try_pay(bob.account.currency, bob.clothing_monthly);
}

void bob_education() {
    try_pay(bob.account.currency, bob.education_monthly);
}

void bob_rent(const int year, const int month) 
{   
    if (bob.flat_one.is_presence) {
        return;
    }

    RUB rent = 0;

    if ((year < 2027 || (year == 2027 && month <= 8))) {
        rent = 0;
    }
    else if (year < 2028 || (year == 2028 && month <= 6)) {
        rent = 20'000;
    }
    else {
        rent = 40'000;
    }
    
    try_pay(bob.account.currency, rent);
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
    RUB tax_after  = sum_tax(annual_income - deduction);
    return tax_before - tax_after;
}

//возвраты в четвёртый месяц раз в год
void bob_tax_refund(const int month)
{
    if (month != 4) return;
    RUB income = bob.account.last_year_income;

    // предел на имуществ выч 2 млн
    RUB mortgage_ded = bob.tax.mortgage_expenses_last_year;
    if (mortgage_ded > bob.tax.mortgage_deduction_cap) {
        mortgage_ded = bob.tax.mortgage_deduction_cap;
    }

    //социальные вычеты база 120 тыс
    RUB social_ded = bob.tax.education_expenses_last_year
                   + bob.tax.medicine_expenses_last_year;
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
void credit_process_payment(Credit& c)
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
    if (bob_find_free_credit_slot() == -1) return;

    for (int i = 0; i < 5; ++i) {
        if (!bob.credits[i].is_active) continue;
        credit_accrue_monthly(bob.credits[i]);
        credit_process_payment(bob.credits[i]);
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
        }
    }
}

void bob_mortgage_payment(const int year, const int month)
{
    if (bob.mortgage.remaining == 0) return;
    
    credit_accrue_monthly(bob.mortgage);
    credit_process_payment(bob.mortgage);
}

void bob_mortgage(const int year, const int month)
{
    bob_mortgage_issue(year, month);
    bob_mortgage_payment(year, month);
}

//Кредитка? Автокредит? Потребкредит?

void bob_liquidate_assets() {
     if (bob.car_one.is_presence) {
        RUB price = car_sale_price();
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

    while ( !(year == 2040 && month == 9) ) { 

        bob_salary(year, month);
        //bob_side_job_start(year, month);
        //bob_side_job_income();

        RUB before = bob.mortgage.remaining;
        bob_mortgage(year, month);
        if (before > bob.mortgage.remaining) {
            bob.tax.mortgage_expenses_this_year += before - bob.mortgage.remaining;
        }
        bob_rent(year, month);
                        //bob_home_bills(year, month);
        bob_utilities(year);
        bob_food(year);
        bob_medicine();
        bob_clothing();
        bob_education();
        /*bob_entertainment();
        bob_public_transport();*/

        bob_dog(year, month);
       
        bob_ndfl(month);

        bob_car(year, month);
        bob_car_use(year, month);
        bob_car_repair(year, month);
       
        /*bob_take_consumer_credit(year, month);
        bob_take_car_credit(year, month);
        bob_take_credit_card();*/
        bob_credit_payments();
        bob_credit_score_update();


        //годовые налоги и инфляция
        if (month == 3)  try_pay(bob.account.currency, bob_transport_tax_yearly());
        if (month == 12) try_pay(bob.account.currency, bob_property_tax_yearly());
        if (month == 1)  {
            pet_inflation_apply(year);
            car_inflation_apply(year);
            personal_inflation_apply(year);
            if (bob.car_one.is_presence) bob.car_one.age++;
        }

        //возврат налогов
        if (month == 4) {
        bob_tax_refund(month);
    }
        
        /*if (month == 12) {
        mortgage_paid_last_year = mortgage_paid_this_year;
        education_paid_last_year = education_paid_this_year;
        medicine_paid_last_year = medicine_paid_this_year;
        mortgage_paid_this_year = 0;
        education_paid_this_year = 0;
        medicine_paid_this_year = 0;
        }*/

        //уточнить корректно ли работает депозит
        bob_deposit_interest();
        bob_deposit_add();
        bob_deposit_withdraw();

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
    country.inflation.salary = 0;
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
    bob.clothing_monthly = 5'000;
    bob.education_monthly = 0;

    bob.dog.last_index_year = 2026;
    bob.car_one.last_index_year = 2026;
    bob.personal_last_index_year = 2026;

    bob.tax.ndfl_paid_last_year = 0;
    bob.flat_one.purchase_price = 0;
    bob.dog.purchase_price = 0;
    bob.car_one.age = 0;
    bob.car_one.purchase_price = 0;
    bob.job.is_active = true;
    bob.job.company = "Рога и копыта";
    bob.investment = 0;

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
    bob.tax.mortgage_deduction_cap = 2'000'000;
    bob.tax.social_deduction_cap   =   120'000;

    inflation_init();
} 

void bob_print() 
{
    printf("Bob cash = %llu\n", bob.account.currency);
    printf("Bob deposit = %llu\n", bob.account.deposit);
    printf("Bob investment = %llu\n", bob.investment);
    printf("Credit score = %d\n", bob.account.credit_score);
}


int main () 
{
    bob_init();

    simulation();

    bob_liquidate_assets();

    bob_print();

    return 0;
}