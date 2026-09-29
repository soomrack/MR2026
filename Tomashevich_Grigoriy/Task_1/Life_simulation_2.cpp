#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using RUB = unsigned long long int;

struct Pet {
    bool presence;
    bool had_dog;
    int death_year;
    int death_month;

    RUB food_monthly;
    RUB vet_yearly;
    int vet_month;
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
};

struct Credit {
    bool active;
    bool closed;
    const char* name;
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

struct Job {
    bool active;
    const char* company;
    RUB salary;
};

struct Bank {
    RUB deposit;                // вклад
    RUB currency;               // деньги на счету в банке
    RUB year_income;            // количество денег заработанных за последний год
    RUB last_year_income;       // количество денег заработанных за предыдущий год
    RUB last_last_year_income;  // количество денег заработанных за пред предыдущий год
    RUB tax_paid;               // НДФЛ заплачено
    RUB tax_refund_accum;        //накопленный возвпат налогов
    int credit_score;           //кредитный рейтинг
};

struct car {
    bool presence;  // наличие
    int power;      // мощность двигателя
    int hp;         // текущее здоровье
    int max_hp;     // максимум здоровья
    bool broken;    // сломана ли в данный момент
    RUB fuel_monthly;   //бензин в месяц
    RUB insurance_monthly;  // страховка в месяц
    RUB maintenance_yearly;  // стоимость ТО раз в год
    int maintenance_month;  // месяц в который ТО
};

struct flat {
    bool presence;  // наличие
    bool for_rent;  // наличие квартиры под аренду
    int square;     // площадь квартиры
    
    RUB gas_rate;       // тариф газ
    RUB cold_water_rate;    // холодная вода
    RUB hot_water_rate;     // горячая
    RUB electricity_rate;   //электричество
    RUB internet_rate;      //интернет
    RUB maintenance_rate;   //содержание жилья
    RUB property_tax_yearly;    //налог на имущество за год
};

struct Person {
    RUB cash;
    RUB salary;
    Pet dog;    
    Bank account;
    car car_1;
    flat flat_1;

    Credit mortgage;               // ипотека

    Inflation inflation;
    Job job_side;
    Credit credits[5];
    int credits_count;
    RUB medicine_monthly;
    RUB clothing_monthly;
    RUB education_monthly;

};

struct SalaryEvent {
    int year;
    int month;
    RUB salary;
};

struct Person bob;

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

void bob_inflation_apply_yearly()
{
    bob.flat_1.gas_rate = apply_inflation(bob.flat_1.gas_rate, bob.inflation.utilities);
    bob.flat_1.cold_water_rate = apply_inflation(bob.flat_1.cold_water_rate, bob.inflation.utilities);
    bob.flat_1.hot_water_rate = apply_inflation(bob.flat_1.hot_water_rate,  bob.inflation.utilities);
    bob.flat_1.electricity_rate = apply_inflation(bob.flat_1.electricity_rate, bob.inflation.utilities);
    bob.flat_1.internet_rate = apply_inflation(bob.flat_1.internet_rate,   bob.inflation.utilities);
    bob.flat_1.maintenance_rate = apply_inflation(bob.flat_1.maintenance_rate, bob.inflation.utilities);

    bob.dog.food_monthly = apply_inflation(bob.dog.food_monthly, bob.inflation.pet);
    bob.dog.vet_yearly = apply_inflation(bob.dog.vet_yearly, bob.inflation.pet);

    bob.car_1.fuel_monthly = apply_inflation(bob.car_1.fuel_monthly, bob.inflation.car);
    bob.car_1.insurance_monthly = apply_inflation(bob.car_1.insurance_monthly, bob.inflation.car);
    bob.car_1.maintenance_yearly = apply_inflation(bob.car_1.maintenance_yearly, bob.inflation.car);

    bob.medicine_monthly = apply_inflation(bob.medicine_monthly, bob.inflation.medicine);
    bob.clothing_monthly = apply_inflation(bob.clothing_monthly, bob.inflation.clothing);
    bob.education_monthly = apply_inflation(bob.education_monthly, bob.inflation.education);
}

// Коммунальные

RUB utility_gas()
{
    return bob.flat_1.gas_rate;
}

RUB utility_cold_water()
{
    return bob.flat_1.cold_water_rate;
}

RUB utility_hot_water()
{
    return bob.flat_1.hot_water_rate;
}

RUB utility_electricity()
{
    return bob.flat_1.electricity_rate;
}

RUB utility_internet()
{
    return bob.flat_1.internet_rate;
}

RUB utility_maintenance()
{
    return bob.flat_1.maintenance_rate;
}

RUB utility_total()
{
    RUB total = 0;
    total += utility_gas();
    total += utility_cold_water();
    total += utility_hot_water();
    total += utility_electricity();
    total += utility_internet();
    total += utility_maintenance();
    return total;
}

void bob_utilities()
{
    if (!bob.flat_1.presence) {
        return;
    }
    RUB bills = utility_total();
    try_pay(bob.account.currency, bills);
}

////////////////////////////

const SalaryEvent bob_salary_schedule[] = {
    {2027, 1, 100'000},     // переход со стажировки
    {2029, 5, 140'000},     // смена работы
    {2030, 1, 180'000},     // повышение
    {2030, 12, 150'000},    // понижение
    {2032, 11, 200'000},    // повышение
};

const int bob_salary_schedule_size = sizeof(bob_salary_schedule) / sizeof(bob_salary_schedule[0]);

void bob_salary(const int year, const int month) 
{
        for (int i = 0; i < bob_salary_schedule_size; ++i) {
        if (year == bob_salary_schedule[i].year && month == bob_salary_schedule[i].month) {
            bob.salary = bob_salary_schedule[i].salary;
        }
    }

    bob.account.currency += bob.salary;
    bob.account.year_income += bob.salary;
}

void bob_car_buy(const int year, const int month) 
{
    if (!bob.flat_1.presence) {
        return;
    }

    if (bob.car_1.presence) {
        return;
    }
    
    if (year > 2030 || (year == 2030 && month >= 6)) {                                                 // появление желания купить авто
        if (bob.account.currency >= 500'000 && bob.salary >= 100'000 && !(bob.car_1.presence)) {        // появление возможности купить авто
            bob.car_1.presence = true;
            bob.car_1.power = 90;
            bob.car_1.hp = 255;
            bob.car_1.max_hp = 255;
            bob.car_1.broken = false;
            bob.account.currency -= 300'000;
            }
        }
    else {
        return;
    }    
}

void bob_car_monthly_costs() {

    if (!bob.car_1.presence) {
        return;
    }
    RUB cost = bob.car_1.fuel_monthly + bob.car_1.insurance_monthly;
    try_pay(bob.account.currency, cost);
}

void bob_car_sell()
{
    if (!bob.car_1.presence) {
        return;
    }
    if (bob.account.currency > 50'000 || bob.salary > 100'000) {
        return;
    }

    bob.car_1.presence = false;
    bob.car_1.power = 0;
    bob.account.currency += 200'000;
    bob.account.year_income += 200'000;
}

void bob_car(const int year, const int month)
{
    bob_car_buy(year, month);
    bob_car_monthly_costs();
    bob_car_sell();
}

void bob_car_use(const int year, const int month) 
{
    if (!bob.car_1.presence) {
        return;
    }
    if (bob.car_1.broken) {
        return;
    }
    //Постепенный износ
    bob.car_1.hp -= 1 + rand() % 3;
    if (bob.car_1.hp < 0) {
        bob.car_1.hp = 0;
    }

    //вероятность поломки квадрат износа
    double wear = 1.0 - (double)bob.car_1.hp / bob.car_1.max_hp; // отношение текущего хп к максимального
    double break_chance = wear * wear;

    double roll = (double)rand() / RAND_MAX;
    if (roll < break_chance) {
        bob.car_1.broken = true;
    }
}

void bob_car_repair(const int year, const int month)
{
    if (!bob.car_1.presence) {
        return;
    }
    if (!bob.car_1.broken) {
        return;
    }

    RUB cost = 5'000 + rand() % 95'001;

    if (bob.account.currency < cost) {
        return;
    }

    // макс хп падает и тем сильнее чем больше стоимость
    int hp_loss = 1 + (int)((double)cost / 100'000.0 * 9);
    bob.car_1.max_hp -= hp_loss;
    
    if (bob.car_1.max_hp < 1) {
        bob.car_1.max_hp = 1;
    }

    bob.car_1.hp = bob.car_1.max_hp;
    bob.car_1.broken = false;
}

//налог на транспорт   !! сделать оплату в марте
RUB bob_transport_tax_yearly() 
{
    if (!bob.car_1.presence) {
        return 0;
    }

    int power = bob.car_1.power;

    RUB rate;   // ставка за 1 л.с.
    if (power <= 100) {
        rate = 12;
    }
    else if (power <= 125) {
        rate = 25;
    }
    else if (power <= 150) {
        rate = 35;
    }
    else if (power <= 175) {
        rate = 45;
    }
    else if (power <= 200) {
        rate = 50;
    }
    else if (power <= 225) {
        rate = 65;
    }
    else if (power <= 250) {
        rate = 75;
    }
    else {
        rate = 150;
    }
    return (RUB)(power * rate);
}

//налог на имущество !!сделать оплату в декабре
RUB bob_property_tax_yearly()
{
    if (!bob.flat_1.presence) {
        return 0;
    }
    return bob.flat_1.property_tax_yearly;
}

//возврат налога по ипотеке 13%, но не больше 260'000
RUB tax_refund_mortgage(RUB paid_this_year)
{
    RUB refund = (RUB)(paid_this_year * 0.13);
    if (refund > 260'000) {
        refund = 260'000;
    }
    return refund;
}

//возврат налога за образование, проценты те же но не больше 15'600 в год
RUB tax_refund_education(RUB paid_this_year)
{
    RUB refund = (RUB)(paid_this_year * 0.13);
    if (refund > 15'600) {
        refund = 15'600;
    }
    return refund;
}

//возврат налога на медицину, то же, что в образовании
RUB tax_refund_medicine(RUB paid_this_year)
{
    RUB refund = (RUB)(paid_this_year * 0.13);
    if (refund > 15'600) {
        refund = 15'600;
    }
    return refund;
}

//возвраты, например в четвёртый месяц раз в год
void bob_tax_refund(const int month, RUB mortgage_paid_year, RUB education_paid_year, RUB medicine_paid_year)
{
    if (month != 4) {
        return;
    }
    RUB total = 0;
    total += tax_refund_mortgage(mortgage_paid_year);
    total += tax_refund_education(education_paid_year);
    total += tax_refund_medicine(medicine_paid_year);

    bob.account.tax_refund_accum += total;
    bob.account.currency += total;
    bob.account.year_income += total;
}

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

//вторая работа (надо ли реализовывать?)

void bob_food(const int year) 
{
    bob.account.currency -= (RUB)7000;
}

void bob_rent(const int year, const int month) 
{   
    if (bob.flat_1.presence) {
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
    long int bills = 0;
    
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
    if (bob.dog.presence || bob.dog.had_dog) {
        return;
    }
        if (bob.salary <= 100'000 || !bob.flat_1.presence) {
        return;
    }

    bob.dog.presence = true;
    bob.dog.death_year = year + 12;
    bob.dog.death_month = month;
    try_pay(bob.account.currency, 20'000);
}

void bob_dog_upkeep(const int year, const int month)
{
    if (!bob.dog.presence) {
        return;
    }
    
    bool alive = (year < bob.dog.death_year) || (year == bob.dog.death_year && month < bob.dog.death_month);

    if (!alive) {
        bob.dog.presence = false;
        bob.dog.had_dog = true;
        try_pay(bob.account.currency, 15'000);
        return;
    }

    try_pay(bob.account.currency, bob.dog.food_monthly);
}

void bob_dog(const int year, const int month) 
{
    bob_dog_buy(year, month);
    bob_dog_upkeep(year, month);
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

void bob_NDFL(const int month) {

   RUB owed = sum_tax(bob.account.year_income);
   RUB delta = owed - bob.account.tax_paid;
   bob.account.currency -= delta;
   bob.account.tax_paid = owed;

    if (month == 12) {
        bob.account.last_last_year_income = bob.account.last_year_income;
        bob.account.last_year_income = bob.account.year_income;
        bob.account.year_income = 0;
        bob.account.tax_paid = 0;
    }

}

//кредитная система

//проверка возможности взять кредит
int bob_find_free_credit_slot()
{
    for (int i = 0; i < 5; ++i) {
        if (!bob.credits[i].active) {
            return i;
        }
    }
    return -1; ///-1?
}

//создание кредита
void credit_init(Credit& c, const char* name, RUB principal, double rate, int term_months)
{
    if (bob_find_free_credit_slot() == -1) return;

    c.active = true;
    c.closed = false;
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
    if (!c.active || c.closed) {
        return;
    }
    double monthly_rate = c.rate / 12.0;
    RUB interest = (RUB)((double)c.remaining * monthly_rate);
    c.remaining +=interest;
}

//система пени (штрафа)
void credit_penalty(Credit& c)
{
    if (!c.active || c.closed) {
        return;
    }
    RUB peny = (RUB)((double)c.monthly_payment * 0.03);
    c.penalty += peny;
    c.remaining += peny;
}

//Платёж если есть деньги, иначе минус рейтинг
void credit_process_payment(Credit& c)
{
    if (!c.active || c.closed) {
        return;
    }
    RUB need = c.monthly_payment + c.penalty;
    if (try_pay(bob.account.currency, need)) {
        c.remaining -= c.monthly_payment;
        c.penalty = 0;
        c.months_paid++;
        c.overdue_months = 0;
    

        if (c.remaining <= c.monthly_payment) {
            c.remaining = 0;
            c.closed = true;
            c.active = false;
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
        if (!bob.credits[i].active) {
            continue;
        }
        credit_accrue_monthly(bob.credits[i]);
        credit_process_payment(bob.credits[i]);
    }
}

//обновление рейтинга
void bob_credit_score_update()
{
    int active_credits = 0;
    int total_overdue = 0;

    for (int i = 0; i < 5; ++i) {
        if (bob.credits[i].active) {
            active_credits++;
            total_overdue += bob.credits[i].overdue_months;
        }
    }
    if (active_credits >= 3) {
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
    if (bob.flat_1.presence) return;

    if (year > 2033 || (year == 2033 && month >= 3)) {
        credit_init(bob.mortgage, "ипотека", 8'000'000, 0.08, 300);
        //bob.mortgage.monthly_payment = 70'000;
        bob.flat_1.presence = true;
    }
}

void bob_mortgage_payment(const int year, const int month)
{
    if (bob.mortgage.remaining == 0) {
        return;
    }
    
    credit_accrue_monthly(bob.mortgage);
    credit_process_payment(bob.mortgage);
    
    /*if (year >= 2052) return;

    RUB payment = 70'000;
    if (month == 1) {
        payment *= 2;
    }
    if (bob.mortgage.remaining < payment) {
        payment = bob.mortgage.remaining;
    }

    if (try_pay(bob.account.currency, payment)) {
        bob.mortgage.remaining -= payment;
    }*/
}

void bob_mortgage(const int year, const int month)
{
    bob_mortgage_issue(year, month);
    bob_mortgage_payment(year, month);
}

//Кредитка? Автокредит? Потребкредит?

void simulation() 
{
    srand((unsigned)time(0));

    int year = 2026;
    int month = 9;

    RUB mortgage_paid_year = 0;
    RUB education_paid_year = 0;
    RUB medicine_paid_year = 0;

    while ( !(year == 2040 && month == 9) ) { 

        bob_salary(year, month);
        //bob_side_job_start(year, month);
        //bob_side_job_income();

        RUB before = bob.mortgage.remaining;
        bob_mortgage(year, month);
        mortgage_paid_year += before - bob.mortgage.remaining;

        bob_rent(year, month);
        bob_home_bills(year, month);
        bob_utilities();

        bob_food(year);
        /*bob_medicine();
        bob_clothing();
        bob_education();
        bob_entertainment();
        bob_public_transport();*/

        bob_dog(year, month);
        //bob_dog_vet(month);

        bob_NDFL(month);

        bob_car(year, month);
        bob_car_use(year, month);
        bob_car_repair(year, month);
        //bob_car_bill(year, month);      // функция переехала под другое имя

        /*bob_take_consumer_credit(year, month);
        bob_take_car_credit(year, month);
        bob_take_credit_card();*/
        bob_credit_payments();
        bob_credit_score_update();


        //годовые налоги и инфляция
        if (month == 3)  try_pay(bob.account.currency, bob_transport_tax_yearly());
        if (month == 12) try_pay(bob.account.currency, bob_property_tax_yearly());
        if (month == 1)  bob_inflation_apply_yearly();

        //возврат налогов
        bob_tax_refund(month, mortgage_paid_year, education_paid_year, medicine_paid_year);
        
        /*if (month == 12) {
        print_year_report(year);
        mortgage_paid_year = 0;
        education_paid_year = 0;
        medicine_paid_year = 0;
        }*/

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }

    }

}

void inflation_init()
{
    bob.inflation.food = 0.08;
    bob.inflation.utilities = 0.07;
    bob.inflation.rent = 0.06;
    bob.inflation.car = 0.05;
    bob.inflation.pet = 0.07;
    bob.inflation.medicine = 0.09;
    bob.inflation.clothing = 0.05;
    bob.inflation.education = 0.06;
    bob.inflation.services = 0.08;
}

void bob_init() 
{
    bob.account.currency = 20'000;
    bob.salary = 80'000;

    bob.account.deposit = 0;
    bob.account.tax_paid = 0;
    bob.account.year_income = 0;
    bob.account.last_year_income = 0;
    bob.account.last_last_year_income = 0;
    bob.account.tax_refund_accum = 0;
    bob.account.credit_score = 70;
    
    bob.flat_1.presence = false;      // наличие квартиры
    bob.flat_1.gas_rate = 500;          //тарифы
    bob.flat_1.cold_water_rate = 400;
    bob.flat_1.hot_water_rate = 700;
    bob.flat_1.electricity_rate = 900;
    bob.flat_1.internet_rate = 700;
    bob.flat_1.maintenance_rate = 3'000;
    bob.flat_1.property_tax_yearly = 3'000;
    
    bob.dog.presence = false;       // наличие собаки
    bob.dog.had_dog = false;
    bob.dog.food_monthly = 7000;
    bob.dog.vet_yearly = 15'000;
    bob.dog.vet_month = 6;

    bob.car_1.presence = false;         //наличие машины
    bob.car_1.power = 0;                //мощность двигателя
    bob.car_1.hp = 0;                   //прочность машины
    bob.car_1.max_hp = 255;             //максимальная прочность машины
    bob.car_1.broken = false;           // индикатор поломки
    bob.car_1.fuel_monthly = 10'000;
    bob.car_1.insurance_monthly = 5'000;
    bob.car_1.maintenance_yearly = 30'000;
    bob.car_1.maintenance_month = 4;

    bob.job_side.active = false;
    bob.job_side.company = "";
    bob.job_side.salary = 0;

    bob.mortgage.active = false;
    bob.mortgage.closed = false;
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
        bob.credits[i].active = false;
        bob.credits[i].closed = false;
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
    inflation_init();
} 

void bob_print() 
{
    printf("Bob cash = %llu\n", bob.account.currency);
}


int main () 
{
    bob_init();

    simulation();

    bob_print();

    return 0;
}