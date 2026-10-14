/*
git add .
git commit -m "проверил отступы"
git push
*/


#include <iostream>
#include <array>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>


using RUB = unsigned long long int;


enum class EXPENSE_CATEGORY {
    other, 
    mortgage,
    medical,
    car
};


// классы
struct JOB {
    bool does_exist = false;
    RUB salary = 105'000;
    RUB year_salary_pay = 0;
    double reduce_salary = 0.95;// уменьшить
    double chance_reduce_salary = 0.1;
    double increase_salary = 1.1;   // увеличить
    double chance_increase_salary = 0.2;
    int sick_day_pay = 3;
};


struct LOTTERY_BIG {
    RUB prize_1 = 100;
    RUB prize_2 = 200;
    RUB prize_3 = 1'000;
    RUB prize_4 = 10'000;
    RUB prize_5 = 100'000;
    RUB prize_6 = 1'000'000;
    RUB prize_7 = 100'000'000;

    double chance_1 = 0.2;
    double chance_2 = 0.1;
    double chance_3 = 0.01;
    double chance_4 = 0.001;
    double chance_5 = 0.0001;
    double chance_6 = 0.00001;
    double chance_7 = 0.0000001;
};


struct INFLATION{
    double LOW = 0.03;
    double NORMAL = 0.07;
    double HIGT  = 0.1;

    // EXPENSES_STANDARD
    double utilities_inflation = NORMAL;
    double internet_mobile_inflation = NORMAL;
    double food_inflation = NORMAL;
    double clothes_inflation = NORMAL;
    double leisure_inflation = NORMAL;
    double other_inflation = NORMAL;
    double over_the_counter_drugs_inflation = NORMAL;
    double rent_inflation = NORMAL;
    double transport_inflation = NORMAL;
    double benzin_inflation = NORMAL;
    double pet_food_inflation = NORMAL;
    double lottery_ticket_inflation = NORMAL;

    // ILLNESS
    double doctor_visit_inflation = NORMAL;
    double medicine_inflation = NORMAL;

    // AUTOMOBILE
    double car_now_cost_inflation = NORMAL;
    double car_insurance_inflation = NORMAL;
    double car_tax_inflation = NORMAL;

    // APARTMENT
    double apartment_now_cost_inflation = NORMAL;

    // RUSSIA_TAX
    double individual_tax_limit_inflation = NORMAL;
    double interest_individual_tax_limit_inflation = NORMAL;
    double savings_free_base_inflation = NORMAL;
    double sum_max_year_refund_medical_inflation = NORMAL;
    double lottery_no_tax_inflation = NORMAL;

    // STATISTIC
    double standart_salary_inflation = NORMAL;

    // JOB
    double salary_inflation = NORMAL;

    // BANKING_ORGANIZATION
    double min_sum_deposits_inflation = LOW;

    // LOTTERY_BIG
    double lottery_prize_inflation = NORMAL;

    // WHEELS
    double wheels_change_inflation = NORMAL;
    double wheels_set_inflation = NORMAL;

    // MAINTENANCE
    double maintenance_work_inflation = NORMAL;

    double maintenance_engine_oil_inflation   = NORMAL;   // моторное масло + фильтр
    double maintenance_cabin_filter_inflation = NORMAL;   // салонный фильтр
    double maintenance_air_filter_inflation   = NORMAL;   // воздушный фильтр
    double maintenance_brake_fluid_inflation  = NORMAL;   // тормозная жидкость
    double maintenance_spark_plugs_inflation  = NORMAL;   // свечи зажигания
    double maintenance_brake_pads_inflation   = NORMAL;   // тормозные колодки
    double maintenance_coolant_inflation      = NORMAL;   // антифриз
    double maintenance_battery_inflation      = NORMAL;   // аккумулятор
    double maintenance_timing_belt_inflation  = NORMAL;   // ремень ГРМ + ролики
};


struct TIME_DEPOSITS{
    bool does_exist = false;
    RUB sum = 0;
    double rate = 0;
    int start_year = 0;
    int start_month = 0;
    int time_month = 0;
    int waiting_time = 0;
    bool closed_early = false;
};


struct BANKING_ORGANIZATION{
    std::string name;

    std::vector<TIME_DEPOSITS> deposits; 

    RUB min_sum_deposits = 50'000;

    double max_rate_deposit = 0.14;
    double min_rate_deposit = 0.06;

    std::array<int,5> time_deposit = {1, 3, 6, 9, 12};

    std::array<double,5> rate_deposit = {0.1, 0.1, 0.1, 0.1, 0.1};

    double rate_change_step = 0.004;
};


struct ILLNESS {
    bool is_sick = false;
    int sick_work_days = 0;
    double chance_get_sick = 0.1;
    int sick_days_min = 3;
    RUB doctor_visit = 2500;
    RUB medicine = 2000; //лекарства 
};


struct WHEELS {
    bool does_exist = true;

    RUB change_cost = 1'000;
    RUB set_cost = 10'000;

    int summer_life_max = 6;
    int winter_life_max = 5;

    int summer_life_left = 6;
    int winter_life_left = 5;

    bool is_winter_now = false;
};


struct SERVICE_PART {   // деталь, которую меняют при ТО
    std::string name;

    RUB cost = 0;

    int life_max = 1;
    int life_left = 1;
};


struct MAINTENANCE {
    RUB work_cost = 3'000;

    SERVICE_PART engine_oil   {"Engine oil + filter",   4'000, 1, 1};  // моторное масло + масляный фильтр
    SERVICE_PART cabin_filter {"Cabin filter",            500, 1, 1};  // салонный фильтр
    SERVICE_PART air_filter   {"Air filter",              600, 2, 2};  // воздушный фильтр двигателя
    SERVICE_PART brake_fluid  {"Brake fluid",             800, 2, 2};  // тормозная жидкость
    SERVICE_PART spark_plugs  {"Spark plugs",           1'200, 3, 3};  // свечи зажигания
    SERVICE_PART brake_pads   {"Brake pads",            3'500, 3, 3};  // тормозные колодки
    SERVICE_PART coolant      {"Coolant",               2'500, 5, 5};  // охлаждающая жидкость (антифриз)
    SERVICE_PART battery      {"Battery",               6'500, 5, 5};  // аккумулятор
    SERVICE_PART timing_belt  {"Timing belt + rollers", 7'000, 6, 6};  // ремень ГРМ + ролики
};


struct AUTOMOBILE { // все для данного автомобиля
    bool does_exist = true;
    std::string name = "LADA Granta";
    std::string configuration = "Standard Plus";
    RUB now_cost = 850'000;
    RUB car_tax = 2'160;

    int yaer_start_have;  //задается обязательно в инициальзации
    int month_start_have;

    RUB car_insurance = 8'300; // ОСАГО 

    RUB all_cost = 0;

    std::string breakdown_history;

    WHEELS wheels;

    MAINTENANCE maintenance;
};


struct ANIMAL {
    bool is_alive = true;
    int life_months_left = 80;
    double chance_get_new = 0.05;
    int life_months_min = 12;
    int life_months_max = 120;
};


struct APARTMENT {
    bool does_exist = false;
    RUB now_cost = 6'000'000;  // начальная стоимость квартиры из условий ипотеки
    RUB mortgage_pay = 24'000'000; // сколько нужно заплатить для выплаты ипотеки из условий
    double area_apartment = 32.2;  // площадь квартиры из условий
    double body_payments = static_cast<double>(now_cost) / mortgage_pay;  // какая часть от выплат это тело кредита
};


struct STATISTIC {
    RUB pay_year = 0;
    RUB pay_life = 0;

    RUB salary_year = 0;
    RUB salary_life = 0;

    RUB tax_year_ndfl = 0;

    RUB tax_life = 0;

    RUB mortgage_year_pay = 0;

    RUB profit_interest_on_savings_year = 0;
    RUB profit_interest_on_savings_life = 0;

    RUB pay_year_last_month = 0;
    RUB pay_month = 0;
    int financial_cushion = 1; // финансовая подушка, сколько сможет прожить без поступлений

    int month_want_get_job = 0;
    int month_want_quit_job = 0;

    RUB standart_salary = 100'000;

    int sick_once_life = 0;
    int sick_work_day_life = 0;

    RUB medical_life = 0;
    RUB medical_year = 0;

    int working_days_in_month  = 21; //среднее значение из интернета

    RUB sum_sell_car_year = 0;
    RUB sum_sell_car_life = 0;

    RUB lottery_win_year = 0;
    RUB lottery_win_life = 0;

    RUB tax_year_ndfl_lottery = 0;
};


struct RUSSIA_TAX {
    std::array<RUB, 5> individual_tax_limit = {0, 2'400'000, 5'000'000, 20'000'000, 50'000'000};
    std::array<double, 5> NDFL = {0.13, 0.15, 0.18, 0.20, 0.22};

    std::array<RUB, 2> interest_individual_tax_limit = {0, 2'400'000 };
    std::array<double, 2> interest_NDFL = {0.13, 0.15};

    double area_not_tax = 20;
    double area_rate = 0.002;

    RUB sum_refund_pay_mortgage_tax_max = 3'000'000; // максимальная сумма по процентам ипотеки предъявляемая к вычету
    RUB save_deduction = 0;  // сохраненный налоговый вычет

    RUB savings_free_base = 1'000'000;
    double key_rate_max = 0.16;  // 
    double savings_rate = 0.1;

    RUB sum_max_year_refund_medical = 150'000;

    int min_ownership_years_no_tax_car = 3;
    RUB max_cost_car_no_tax = 250'000;

    RUB lottery_no_tax = 4'000;
};


struct EXPENSES_STANDARD {
    // Обязательные
    RUB utilities = 5000; // коммуналка
    RUB internet_mobile = 1000; // интернет + связь
    RUB food = 20000; // питание
    RUB clothes = 3000; // одежда
    RUB leisure = 3000; // досуг
    RUB other = 2000; // прочее
    RUB over_the_counter_drugs = 500; // зеленка, активированный уголь ...  (препараты без рецепта), не подлежат налговому вычету

    // Необязательные
    RUB rent = 30000; // аренда
    RUB mortgage = 50000; // ипотека, ежемесячный платёж из условий

    RUB transport = 5000; // транспорт
    RUB benzin = 7000; // бензин

    RUB pet_food = 3000; // еда для питомца

    RUB lottery_ticket = 200;
};


struct PERSON {
    std::string name;

    bool is_alive  = true;
    int age_years  = 20;
    int age_months = 5;

    RUB cash = 200'000;
    RUB debt = 0;

    RUB tax_duty = 0;

    int simulation_start_year = 2026;
    int simulation_start_month = 10;
    int simulation_start_seed = 12345678;

    int month_now = 0;
    int year_now = 0;

    std::string reason_end = "";

    std::array<JOB, 4> works;

    STATISTIC data;

    RUSSIA_TAX tax_book;

    INFLATION inflation_book;

    APARTMENT home_alise;

    EXPENSES_STANDARD expenses;

    AUTOMOBILE car;

    std::array<BANKING_ORGANIZATION, 4> banks;

    ANIMAL pet;

    ILLNESS sick;

    LOTTERY_BIG lottery_big;
};


// функции
RUB calc_ndfl_sum(const RUB profit, const PERSON& man)
{
    double tax = 0;

    for (size_t id = 0; id < man.tax_book.individual_tax_limit.size(); id++){
        RUB low = man.tax_book.individual_tax_limit[id];
        if (profit <= low) break;

        bool is_last = (id + 1 == man.tax_book.individual_tax_limit.size());
        
        RUB high = 0;

        if (is_last) {
            high = profit;
        } else {
            high = std::min(profit, man.tax_book.individual_tax_limit[id + 1]);
        }

        tax += (high - low) * man.tax_book.NDFL[id];
    }

    return tax;
}


RUB calc_ndfl_interest_sum(const RUB profit, const PERSON& man)
{
    double tax = 0;

    for (size_t id = 0; id < man.tax_book.interest_individual_tax_limit.size(); id++){
        RUB low = man.tax_book.interest_individual_tax_limit[id];
        if (profit <= low) break;

        bool is_last = (id + 1 == man.tax_book.interest_individual_tax_limit.size());
        
        RUB high = 0;

        if (is_last) {
            high = profit;
        } else {
            high = std::min(profit, man.tax_book.interest_individual_tax_limit[id + 1]);
        }

        tax += (high - low) * man.tax_book.interest_NDFL[id];
    }

    return tax;
}


RUB refund_tax_mortgage(PERSON& man)
{
    if (man.tax_book.sum_refund_pay_mortgage_tax_max == 0) return 0;

    RUB max_deduction = man.data.mortgage_year_pay * (1 - man.home_alise.body_payments); // упрощённая формула

    max_deduction = std::min(man.tax_book.sum_refund_pay_mortgage_tax_max, max_deduction);

    return max_deduction; // сколько можно вычесть из налогооблагаемой базы
}


RUB refund_tax_medical(PERSON& man)
{
    return std::min(man.data.medical_year, man.tax_book.sum_max_year_refund_medical);
}


RUB get_profit_sell_car(PERSON& man)
{
    if (man.data.sum_sell_car_year == 0){
        return 0;
    } else {
        return man.data.sum_sell_car_year - man.tax_book.max_cost_car_no_tax;
    }
}


void get_refund_NDFL_tax(PERSON& man, const int month)
{
    if (month != 1) return;

    RUB salary_base = man.data.salary_year;
    RUB car_base = get_profit_sell_car(man);

    RUB mortgage_deduction = refund_tax_mortgage(man);
    man.tax_book.sum_refund_pay_mortgage_tax_max -= mortgage_deduction;

    RUB medical_deduction = refund_tax_medical(man);

    RUB sum_deduction = 0;
    sum_deduction += mortgage_deduction;
    sum_deduction += medical_deduction;
    sum_deduction += man.tax_book.save_deduction;

    RUB salary_deduction = std::min(sum_deduction, salary_base);
    sum_deduction -= salary_deduction;

    RUB car_deduction = std::min(sum_deduction, car_base);
    sum_deduction -= car_deduction;

    man.tax_book.save_deduction = sum_deduction;

    RUB NDFL_reduced_year = 0;
    NDFL_reduced_year +=calc_ndfl_sum(salary_base - salary_deduction, man);
    NDFL_reduced_year += calc_ndfl_sum(car_base - car_deduction, man);

    if (NDFL_reduced_year >= man.data.tax_year_ndfl) {
        man.tax_duty += NDFL_reduced_year - man.data.tax_year_ndfl;
    } else {
        man.cash += man.data.tax_year_ndfl - NDFL_reduced_year;
    }
}


void get_apartment_tax(PERSON& man,const int month)
{
    if (month != 1) return;

    if (!man.home_alise.does_exist) return;

    man.tax_duty += std::max(0.0, man.home_alise.now_cost * man.tax_book.area_rate *
        ((man.home_alise.area_apartment - man.tax_book.area_not_tax ) / man.home_alise.area_apartment ));
}


void get_car_tax(PERSON& man, const int month)
{
    if (month != 1) return;

    if (!man.car.does_exist) return;

    man.tax_duty += man.car.car_tax;

    man.car.all_cost += man.car.car_tax;
}


void check_end_alise(bool& flag_end, const int year, PERSON& man)
{
    if (man.debt > 0 ){
        man.reason_end += "Bankruptcy (No money) ";
        flag_end = true;
        if (man.home_alise.mortgage_pay == 0){
        man.reason_end += "WIN Mortgage(0) ";  
        }
    }

    if (year == 2067){
        man.reason_end += "Time out ";
        flag_end = true;
        if (man.home_alise.mortgage_pay == 0){
        man.reason_end += "WIN Mortgage(0) ";  
        }
    }
}


void check_end_bob(bool& flag_end, const int year, PERSON& man)
{
    if (year == 2067){
        man.reason_end += "Time out ";
        flag_end = true;
    }

    if (man.debt > 0 ){
        man.reason_end += "Bankruptcy (No money) ";
        flag_end = true;
    }
}


void next_month_year(int& month, int& year)
{
    month++;
    if (month > 12) {
        month = 1;
        year++;
    }
}


void pay_rub(const RUB money, PERSON& man, const EXPENSE_CATEGORY category = EXPENSE_CATEGORY::other)
{
    if (man.cash >= money){
        man.cash -= money;
        man.data.pay_year += money;
    } else {
        man.debt += money - man.cash;
        man.data.pay_year += money;
        man.cash = 0;
    }

    switch (category) {
        case EXPENSE_CATEGORY::mortgage:
            man.home_alise.mortgage_pay -= money;
            man.data.mortgage_year_pay += money;
            break;
        
        case EXPENSE_CATEGORY::medical:
            man.data.medical_year += money;
            break;

        case EXPENSE_CATEGORY::other:
            break;

        case EXPENSE_CATEGORY::car:
            man.car.all_cost += money;
            break;
    }
}


void init_wheels(PERSON& man)
{
    int month = man.simulation_start_month;

    bool is_winter_season = (month >= 11 || month <= 3);

    man.car.wheels.is_winter_now = is_winter_season;
}


void init_alise(PERSON& man)
{
    man.name = "ALISE";

    man.works[0].does_exist = true;

    man.home_alise.does_exist = true;

    man.car.yaer_start_have = man.simulation_start_year;
    man.car.month_start_have = man.simulation_start_month;

    init_wheels(man);
}


void get_salary(PERSON& man)
{
    const int work_days = man.data.working_days_in_month;

    for (size_t id_job = 0; id_job < man.works.size(); id_job++){
        JOB& job = man.works[id_job];

        if (!job.does_exist) continue;

        RUB month_salary = job.salary;

        if (man.sick.is_sick) {
            int paid_days = work_days - man.sick.sick_work_days + job.sick_day_pay;

            paid_days = std::clamp(paid_days, 0, work_days);//защита от невозможных значений 

            month_salary = job.salary * paid_days / work_days;
        }

        RUB tax_before = calc_ndfl_sum(job.year_salary_pay, man);
        RUB tax_now = calc_ndfl_sum(job.year_salary_pay + month_salary, man);

        RUB need_pay_tax = tax_now - tax_before;

        man.cash += month_salary - need_pay_tax;

        man.data.tax_year_ndfl += need_pay_tax;

        man.data.salary_year += month_salary;
        job.year_salary_pay += month_salary;
    }
}

void reload_data(PERSON& man, int month)
{
    man.data.pay_month = man.data.pay_year - man.data.pay_year_last_month;

    if (man.data.pay_month != 0 ) man.data.financial_cushion = (man.cash) / man.data.pay_month;

    if (month == 1){
        man.data.pay_life += man.data.pay_year;
        man.data.pay_year = 0;

        man.data.salary_life += man.data.salary_year;
        man.data.salary_year = 0;

        man.data.tax_life += man.data.tax_year_ndfl;
        man.data.tax_year_ndfl = 0;

        man.data.mortgage_year_pay = 0;

        man.data.medical_life += man.data.medical_year;
        man.data.medical_year = 0;

        for (size_t id_job = 0; id_job < man.works.size(); id_job++){
            man.works[id_job].year_salary_pay = 0;
        }

        man.data.profit_interest_on_savings_life += man.data.profit_interest_on_savings_year;
        man.data.profit_interest_on_savings_year = 0;

        man.data.sum_sell_car_life += man.data.sum_sell_car_year;
        man.data.sum_sell_car_year = 0;

        man.data.lottery_win_life += man.data.lottery_win_year;
        man.data.lottery_win_year = 0;

        man.data.tax_life += man.data.tax_year_ndfl_lottery;
        man.data.tax_year_ndfl_lottery = 0;
    }

    man.data.pay_year_last_month = man.data.pay_year;
}


void tax_duty_pay(PERSON& man, int month)
{
    if (month != 11) return;
    
    pay_rub(man.tax_duty, man);
    
    man.data.tax_life += man.tax_duty;
    man.tax_duty = 0;         
}


void interest_on_savings_alise(PERSON& man)
{
    RUB interest = man.cash * man.tax_book.savings_rate / 12;

    man.cash += interest;
    man.data.profit_interest_on_savings_year += interest;
}


void get_interest_on_savings_tax(PERSON& man, const int month)
{
    if (month != 1) return;

    RUB tax_free_limit = man.tax_book.savings_free_base * man.tax_book.key_rate_max;

    if (man.data.profit_interest_on_savings_year <= tax_free_limit) return;

    RUB taxable_interest = man.data.profit_interest_on_savings_year - tax_free_limit;


    man.tax_duty += calc_ndfl_interest_sum(taxable_interest, man);
}


void get_quit_job(PERSON& man)
{
    if (man.data.financial_cushion >= 2){
        man.data.month_want_quit_job += 1;
        man.data.month_want_get_job = 0;
    } else {
        man.data.month_want_quit_job = 0;
        man.data.month_want_get_job += 1;
    }

    if (man.data.month_want_get_job >= 3){
        for (size_t id_job = 0; id_job < man.works.size(); id_job++){
            if (!man.works[id_job].does_exist){
                man.works[id_job].does_exist = true;
                man.works[id_job].salary = man.data.standart_salary;
                man.works[id_job].year_salary_pay = 0;

                man.data.month_want_get_job = 0;

                break;
            }
        }
    } else if (man.data.month_want_quit_job >= 3){
        RUB sum_salary = 0;
        size_t id_min = man.works.size();

        for (size_t id_job = 0; id_job < man.works.size(); id_job++){
            if (!man.works[id_job].does_exist) continue;

            sum_salary += man.works[id_job].salary;

            if (id_min == man.works.size() || man.works[id_job].salary < man.works[id_min].salary){
                id_min = id_job;
            }
        }

        if (id_min == man.works.size()) return;

        RUB sum_other_salary = sum_salary - man.works[id_min].salary;

        if (sum_other_salary > man.data.pay_month){
            man.works[id_min].does_exist = false;
            man.data.month_want_quit_job = 0;
        }
    }
}


void set_random_seed(const int seed)
{
    std::srand(seed);
}


bool random_chance(const double chance)
{
    double number = std::rand() / (RAND_MAX + 1.0);

    return number < chance;
}


int random_int(int min_value, int max_value)
{
    return min_value + std::rand() % (max_value - min_value + 1);
}


void change_salary_random(PERSON& man, const int month)
{
    if (month != 1) return;

    for (size_t id_job = 0; id_job < man.works.size(); id_job++){
        if (!man.works[id_job].does_exist) continue;

        double coefficient = 1.0;

        if (random_chance(man.works[id_job].chance_increase_salary)) {
            coefficient *= man.works[id_job].increase_salary;
        }

        if (random_chance(man.works[id_job].chance_reduce_salary)) {
            coefficient *= man.works[id_job].reduce_salary;
        }

        man.works[id_job].salary = man.works[id_job].salary * coefficient;
    }
}


void get_pet(PERSON& man)
{
    if (man.pet.is_alive) return;

    if (random_chance(man.pet.chance_get_new)) {
        man.pet.is_alive = true;
        man.pet.life_months_left = random_int(man.pet.life_months_min, man.pet.life_months_max);
    }
}


void age_pet(PERSON& man)
{
    if (!man.pet.is_alive) return;

    man.pet.life_months_left -= 1;

    if (man.pet.life_months_left == 0) man.pet.is_alive = false;
}


void sell_car(PERSON& man, const int month, const int year)
{
    if (!man.car.does_exist) return;

    int year_have_car = ( (year - man.car.yaer_start_have) * 12 + month - man.car.month_start_have ) / 12;

    if ( man.car.now_cost <= man.tax_book.max_cost_car_no_tax || year_have_car >= 3){
        man.car.does_exist = false;

        man.cash += man.car.now_cost;

        man.data.sum_sell_car_life += man.car.now_cost;
    } else {
        man.car.does_exist = false;

        RUB tax_before =  0;

        if ( man.data.sum_sell_car_year > man.tax_book.max_cost_car_no_tax ) tax_before =  calc_ndfl_sum(man.data.sum_sell_car_year - man.tax_book.max_cost_car_no_tax, man);

        RUB tax_now = calc_ndfl_sum(man.car.now_cost + man.data.sum_sell_car_year - man.tax_book.max_cost_car_no_tax, man);

        RUB need_pay_tax = tax_now - tax_before;

        man.cash += man.car.now_cost - need_pay_tax;

        man.data.tax_year_ndfl += need_pay_tax;

        man.data.sum_sell_car_year += man.car.now_cost;
    }
}


void get_sick(PERSON& man)
{
    man.sick.is_sick = false;
    man.sick.sick_work_days = 0;

    if (!random_chance(man.sick.chance_get_sick)) return;

    const int max_sick_days = man.data.working_days_in_month; // возможно добавить функцию для реального расчета
    const int min_sick_days = man.sick.sick_days_min;

    man.sick.is_sick = true;
    man.sick.sick_work_days = random_int(min_sick_days, max_sick_days);

    man.data.sick_once_life += 1;
    man.data.sick_work_day_life += man.sick.sick_work_days;

    pay_rub(man.sick.doctor_visit, man, EXPENSE_CATEGORY::medical);
    pay_rub(man.sick.medicine, man, EXPENSE_CATEGORY::medical);
}


void time_step(int& month, int& year, PERSON& man, const bool flag_end)
{
    if (flag_end) return;

    next_month_year(month, year);

    man.month_now = month;
    man.year_now = year;

    next_month_year(man.age_months, man.age_years);

    age_pet(man);
}


void cash_go_debt(PERSON& man)
{
    if (man.debt == 0) return;

    if (man.cash >= man.debt) {
        man.cash -= man.debt;
        man.debt = 0;
        return;
    } else {
        man.debt -= man.cash;
        man.cash = 0;
    }
}


void debt_pay_alise(PERSON& man,const int month, const int year)
{
    if (man.debt == 0) return;

    cash_go_debt(man);

    if (man.debt == 0) return;

    sell_car(man, month, year);

    cash_go_debt(man);
}


bool deposits_close_longest_waiting(PERSON& man)
{
    size_t best_bank = man.banks.size();
    size_t best_deposit = 0;

    for (size_t id_bank = 0; id_bank < man.banks.size(); id_bank++){
        const BANKING_ORGANIZATION& bank = man.banks[id_bank];

        for (size_t id_deposit = 0; id_deposit < bank.deposits.size(); id_deposit++){
            if (!bank.deposits[id_deposit].does_exist) continue;

            if (best_bank == man.banks.size() || bank.deposits[id_deposit].waiting_time > man.banks[best_bank].deposits[best_deposit].waiting_time){
                best_bank = id_bank;
                best_deposit = id_deposit;
            }
        }
    }

    if (best_bank == man.banks.size()) return false;

    TIME_DEPOSITS& deposit = man.banks[best_bank].deposits[best_deposit];

    man.cash += deposit.sum;

    deposit.does_exist = false;
    deposit.closed_early = true;
    deposit.sum = 0;

    return true;
}


void debt_pay_bob(PERSON& man, const int month, const int year)
{
    if (man.debt == 0) return;

    cash_go_debt(man);

    while (man.debt > 0 && deposits_close_longest_waiting(man)){
        cash_go_debt(man);
    }

    if (man.debt == 0) return;

    sell_car(man, month, year);

    cash_go_debt(man);
}


RUB inflate(const RUB base, const double rate, const int year_inflation, const int month_inflation)
{
    double coefficient = std::pow(1 + rate, year_inflation) * (1 + rate * month_inflation / 12.0);

    return (base * coefficient);
}


void inflation_month(PERSON& man, const PERSON& clone, const int month, const int year)
{
    int all_month_inflation = (year - man.simulation_start_year) * 12 + month - man.simulation_start_month;

    int year_inflation = all_month_inflation / 12;
    int month_inflation = all_month_inflation % 12;

    // EXPENSES_STANDARD
    man.expenses.utilities = inflate(clone.expenses.utilities, man.inflation_book.utilities_inflation, year_inflation, month_inflation);
    man.expenses.internet_mobile = inflate(clone.expenses.internet_mobile, man.inflation_book.internet_mobile_inflation, year_inflation, month_inflation);
    man.expenses.food = inflate(clone.expenses.food, man.inflation_book.food_inflation, year_inflation, month_inflation);
    man.expenses.clothes = inflate(clone.expenses.clothes, man.inflation_book.clothes_inflation, year_inflation, month_inflation);
    man.expenses.leisure = inflate(clone.expenses.leisure, man.inflation_book.leisure_inflation, year_inflation, month_inflation);
    man.expenses.other = inflate(clone.expenses.other, man.inflation_book.other_inflation, year_inflation, month_inflation);
    man.expenses.over_the_counter_drugs = inflate(clone.expenses.over_the_counter_drugs, man.inflation_book.over_the_counter_drugs_inflation, year_inflation, month_inflation);
    man.expenses.rent = inflate(clone.expenses.rent, man.inflation_book.rent_inflation, year_inflation, month_inflation);
    man.expenses.transport = inflate(clone.expenses.transport, man.inflation_book.transport_inflation, year_inflation, month_inflation);
    man.expenses.benzin = inflate(clone.expenses.benzin, man.inflation_book.benzin_inflation, year_inflation, month_inflation);
    man.expenses.pet_food = inflate(clone.expenses.pet_food, man.inflation_book.pet_food_inflation, year_inflation, month_inflation);
    man.expenses.lottery_ticket = inflate(clone.expenses.lottery_ticket, man.inflation_book.lottery_ticket_inflation, year_inflation, month_inflation);

    // ILLNESS
    man.sick.doctor_visit = inflate(clone.sick.doctor_visit, man.inflation_book.doctor_visit_inflation, year_inflation, month_inflation);
    man.sick.medicine = inflate(clone.sick.medicine, man.inflation_book.medicine_inflation, year_inflation, month_inflation);

    // STATISTIC
    man.data.standart_salary = inflate(clone.data.standart_salary, man.inflation_book.standart_salary_inflation, year_inflation, month_inflation);

    // AUTOMOBILE
    man.car.now_cost = inflate(clone.car.now_cost, man.inflation_book.car_now_cost_inflation, year_inflation, month_inflation);

    // WHEELS
    man.car.wheels.change_cost = inflate(clone.car.wheels.change_cost, man.inflation_book.wheels_change_inflation, year_inflation, month_inflation);
    man.car.wheels.set_cost    = inflate(clone.car.wheels.set_cost,    man.inflation_book.wheels_set_inflation,    year_inflation, month_inflation);

    // MAINTENANCE
    man.car.maintenance.work_cost = inflate(clone.car.maintenance.work_cost, man.inflation_book.maintenance_work_inflation, year_inflation, month_inflation);

    man.car.maintenance.engine_oil.cost   = inflate(clone.car.maintenance.engine_oil.cost,   man.inflation_book.maintenance_engine_oil_inflation,   year_inflation, month_inflation);
    man.car.maintenance.cabin_filter.cost = inflate(clone.car.maintenance.cabin_filter.cost, man.inflation_book.maintenance_cabin_filter_inflation, year_inflation, month_inflation);
    man.car.maintenance.air_filter.cost   = inflate(clone.car.maintenance.air_filter.cost,   man.inflation_book.maintenance_air_filter_inflation,   year_inflation, month_inflation);
    man.car.maintenance.brake_fluid.cost  = inflate(clone.car.maintenance.brake_fluid.cost,  man.inflation_book.maintenance_brake_fluid_inflation,  year_inflation, month_inflation);
    man.car.maintenance.spark_plugs.cost  = inflate(clone.car.maintenance.spark_plugs.cost,  man.inflation_book.maintenance_spark_plugs_inflation,  year_inflation, month_inflation);
    man.car.maintenance.brake_pads.cost   = inflate(clone.car.maintenance.brake_pads.cost,   man.inflation_book.maintenance_brake_pads_inflation,   year_inflation, month_inflation);
    man.car.maintenance.coolant.cost      = inflate(clone.car.maintenance.coolant.cost,      man.inflation_book.maintenance_coolant_inflation,      year_inflation, month_inflation);
    man.car.maintenance.battery.cost      = inflate(clone.car.maintenance.battery.cost,      man.inflation_book.maintenance_battery_inflation,      year_inflation, month_inflation);
    man.car.maintenance.timing_belt.cost  = inflate(clone.car.maintenance.timing_belt.cost,  man.inflation_book.maintenance_timing_belt_inflation,  year_inflation, month_inflation);
}


void inflation_year(PERSON& man, const PERSON& clone, const int month, const int year)
{
    if (month !=  man.simulation_start_month) return;

    int year_inflation = year - man.simulation_start_year;

    if (year_inflation <= 0) return;

    // RUSSIA_TAX
    for (size_t id = 0; id < man.tax_book.individual_tax_limit.size(); id++){
        man.tax_book.individual_tax_limit[id] = inflate(clone.tax_book.individual_tax_limit[id], man.inflation_book.individual_tax_limit_inflation, year_inflation, 0);
    }

    for (size_t id = 0; id < man.tax_book.interest_individual_tax_limit.size(); id++){
        man.tax_book.interest_individual_tax_limit[id] = inflate(clone.tax_book.interest_individual_tax_limit[id], man.inflation_book.interest_individual_tax_limit_inflation, year_inflation, 0);
    }

    man.tax_book.savings_free_base = inflate(clone.tax_book.savings_free_base, man.inflation_book.savings_free_base_inflation, year_inflation, 0);

    man.tax_book.sum_max_year_refund_medical = inflate(clone.tax_book.sum_max_year_refund_medical, man.inflation_book.sum_max_year_refund_medical_inflation, year_inflation, 0);

    man.tax_book.lottery_no_tax = inflate(clone.tax_book.lottery_no_tax, man.inflation_book.lottery_no_tax_inflation, year_inflation, 0);


    // APARTMENT
    man.home_alise.now_cost = inflate(clone.home_alise.now_cost, man.inflation_book.apartment_now_cost_inflation, year_inflation, 0);

    // JOB
    for (size_t id_job = 0; id_job < man.works.size(); id_job++){
        if (!man.works[id_job].does_exist) continue;

        man.works[id_job].salary = inflate(man.works[id_job].salary, man.inflation_book.salary_inflation, 1, 0);
    }

    //AUTOMOBILE
    man.car.car_insurance = inflate(clone.car.car_insurance, man.inflation_book.car_insurance_inflation, year_inflation, 0);

    man.car.car_tax = inflate(clone.car.car_tax, man.inflation_book.car_tax_inflation, year_inflation, 0);

    //BANKING_ORGANIZATION
    for (size_t id = 0; id < man.banks.size(); id++){
        man.banks[id].min_sum_deposits = inflate(clone.banks[id].min_sum_deposits, man.inflation_book.min_sum_deposits_inflation, year_inflation, 0);
    }

    // LOTTERY_BIG
    man.lottery_big.prize_1 = inflate(clone.lottery_big.prize_1, man.inflation_book.lottery_prize_inflation, year_inflation, 0);
    man.lottery_big.prize_2 = inflate(clone.lottery_big.prize_2, man.inflation_book.lottery_prize_inflation, year_inflation, 0);
    man.lottery_big.prize_3 = inflate(clone.lottery_big.prize_3, man.inflation_book.lottery_prize_inflation, year_inflation, 0);
    man.lottery_big.prize_4 = inflate(clone.lottery_big.prize_4, man.inflation_book.lottery_prize_inflation, year_inflation, 0);
    man.lottery_big.prize_5 = inflate(clone.lottery_big.prize_5, man.inflation_book.lottery_prize_inflation, year_inflation, 0);
    man.lottery_big.prize_6 = inflate(clone.lottery_big.prize_6, man.inflation_book.lottery_prize_inflation, year_inflation, 0);
    man.lottery_big.prize_7 = inflate(clone.lottery_big.prize_7, man.inflation_book.lottery_prize_inflation, year_inflation, 0);
}


void pay_car(PERSON& man, const int month)
{
    if (!man.car.does_exist) return;

    pay_rub(man.expenses.benzin, man, EXPENSE_CATEGORY::car);

    if (month == 12 ) pay_rub(man.car.car_insurance, man, EXPENSE_CATEGORY::car);
}


void pay_pet_food(PERSON& man)
{
    if (man.pet.is_alive) pay_rub(man.expenses.pet_food, man);
}


void pay_transport(PERSON& man)
{
    if (man.car.does_exist) return;
    
    pay_rub(man.expenses.transport, man);
}


void init_bob(PERSON& man)
{
    man.name = "BOB";

    man.works[0].does_exist = true;

    man.car.yaer_start_have = man.simulation_start_year;
    man.car.month_start_have = man.simulation_start_month;

    man.banks[0].name = "Sberbank";
    man.banks[1].name = "VTB Bank";
    man.banks[2].name = "Gazprombank";
    man.banks[3].name = "Alfa-Bank";

    init_wheels(man);
}


void deposits_pass_month(PERSON& man)
{
    for (BANKING_ORGANIZATION& bank : man.banks){
        for (TIME_DEPOSITS& deposit : bank.deposits){
            if (!deposit.does_exist) continue;

            deposit.waiting_time -= 1;
        }
    }
}


void deposits_pay_out(PERSON& man)
{
    for (BANKING_ORGANIZATION& bank : man.banks){
        for (TIME_DEPOSITS& deposit : bank.deposits){
            if (!deposit.does_exist) continue;
            if (deposit.waiting_time != 0) continue;

            const RUB profit = (deposit.sum * deposit.rate * deposit.time_month / 12);

            man.cash += deposit.sum + profit;
            man.data.profit_interest_on_savings_year += profit;

            deposit.does_exist = false;
        }
    }
}


bool is_better_deposit_offer(const double rate, const int time_month, const double best_rate, const int best_time_month)
{
    if (rate > best_rate) return true;

    if (rate < best_rate) return false;

    return time_month < best_time_month;
}


size_t find_best_offer_in_bank(const BANKING_ORGANIZATION& bank)
{
    size_t best_offer = 0;

    for (size_t id_offer = 1; id_offer < bank.rate_deposit.size(); id_offer++){
        if (is_better_deposit_offer(bank.rate_deposit[id_offer], bank.time_deposit[id_offer], bank.rate_deposit[best_offer], bank.time_deposit[best_offer])){
            best_offer = id_offer;
        }
    }

    return best_offer;
}


void deposit_open_new(PERSON& man)
{
    if (man.data.pay_month == 0) return;

    const RUB reserve = man.data.pay_month * 4;

    if (man.cash <= reserve) return;

    const RUB free_money = man.cash - reserve;

    size_t best_bank = man.banks.size();
    size_t best_offer = 0;

    for (size_t id_bank = 0; id_bank < man.banks.size(); id_bank++){
        const BANKING_ORGANIZATION& bank = man.banks[id_bank];

        if (free_money < bank.min_sum_deposits) continue;

        const size_t offer = find_best_offer_in_bank(bank);

        if (best_bank == man.banks.size() || is_better_deposit_offer(bank.rate_deposit[offer], bank.time_deposit[offer],man.banks[best_bank].rate_deposit[best_offer], man.banks[best_bank].time_deposit[best_offer])){
            best_bank = id_bank;
            best_offer = offer;
        }
    }

    if (best_bank == man.banks.size()) return;

    BANKING_ORGANIZATION& bank = man.banks[best_bank];

    TIME_DEPOSITS deposit;
    deposit.does_exist = true;
    deposit.sum = free_money;
    deposit.rate = bank.rate_deposit[best_offer];
    deposit.start_year = man.year_now;
    deposit.start_month = man.month_now;
    deposit.time_month = bank.time_deposit[best_offer];
    deposit.waiting_time = deposit.time_month;

    bank.deposits.push_back(deposit);

    man.cash -= free_money;
}


void change_deposit_rates_random(PERSON& man)
{
    for (BANKING_ORGANIZATION& bank : man.banks){
        for (double& rate : bank.rate_deposit){
            if (random_chance(0.5)){
                rate += bank.rate_change_step;
            } else {                    
                rate -= bank.rate_change_step;
            }

            rate = std::clamp(rate, bank.min_rate_deposit, bank.max_rate_deposit);
        }
    }
}


void interest_on_savings_bob(PERSON& man)
{
    deposits_pass_month(man);

    deposits_pay_out(man);

    change_deposit_rates_random(man);

    deposit_open_new(man);
}


void pay_mortgage(PERSON& man)
{
    if (man.home_alise.mortgage_pay > 0) pay_rub(man.expenses.mortgage, man, EXPENSE_CATEGORY::mortgage);
}


RUB play_lottery_BIG(const LOTTERY_BIG& lottery)
{
    if (random_chance(lottery.chance_7)) return lottery.prize_7;
    if (random_chance(lottery.chance_6)) return lottery.prize_6;
    if (random_chance(lottery.chance_5)) return lottery.prize_5;
    if (random_chance(lottery.chance_4)) return lottery.prize_4;
    if (random_chance(lottery.chance_3)) return lottery.prize_3;
    if (random_chance(lottery.chance_2)) return lottery.prize_2;
    if (random_chance(lottery.chance_1)) return lottery.prize_1;

    return 0;
}


void go_lottery(PERSON& man) 
{
    if (man.cash < man.expenses.lottery_ticket) return;

    pay_rub(man.expenses.lottery_ticket , man);

    RUB sum_win = play_lottery_BIG(man.lottery_big);

    if (sum_win == 0) return;

    RUB no_tax = std::min(sum_win, man.tax_book.lottery_no_tax);

    RUB tax = calc_ndfl_sum(sum_win - no_tax, man);

    man.cash += sum_win - tax;

    man.data.tax_year_ndfl_lottery += tax;

    man.data.lottery_win_year += sum_win;
}


void get_lottery_tax(PERSON& man, const int month)
{
    if (month != 1) return;

    RUB total_win = man.data.lottery_win_year;

    if (total_win <= man.tax_book.lottery_no_tax) {
        RUB overpaid = man.data.tax_year_ndfl_lottery;
        man.cash += overpaid;
        return;
    }

    RUB taxable_base = total_win - man.tax_book.lottery_no_tax;

    RUB correct_tax = calc_ndfl_sum(taxable_base, man);

    RUB real_tax = man.data.tax_year_ndfl_lottery;

    if (correct_tax >= real_tax) {
        man.tax_duty += correct_tax - real_tax;
    } else {
        man.cash += real_tax - correct_tax;
    }
}


void change_wheels(PERSON& man, const int month)
{
    WHEELS& car_wheels = man.car.wheels;

    if (!car_wheels.does_exist) return;
    if (!man.car.does_exist) return;

    bool need_winter = (month >= 11 || month <= 3);
    bool need_summer = (month >= 4 && month <= 10);

    if (need_winter && !car_wheels.is_winter_now) {
        if (car_wheels.winter_life_left <= 0) {
            pay_rub(car_wheels.set_cost, man, EXPENSE_CATEGORY::car);
            car_wheels.winter_life_left = car_wheels.winter_life_max;
        }

        pay_rub(car_wheels.change_cost, man, EXPENSE_CATEGORY::car);

        car_wheels.winter_life_left -= 1;
        car_wheels.is_winter_now = true;

    } else if (need_summer && car_wheels.is_winter_now) {
        if (car_wheels.summer_life_left <= 0) {
            pay_rub(car_wheels.set_cost, man, EXPENSE_CATEGORY::car);
            car_wheels.summer_life_left = car_wheels.summer_life_max;
        }

        pay_rub(car_wheels.change_cost, man, EXPENSE_CATEGORY::car);

        car_wheels.summer_life_left -= 1;
        car_wheels.is_winter_now = false;
    }
}


void car_service_part(PERSON& man, SERVICE_PART& part)
{
    part.life_left -= 1;

    if (part.life_left <= 0){
        pay_rub(part.cost, man, EXPENSE_CATEGORY::car);
        part.life_left = part.life_max;
    }
}


void car_maintenance(PERSON& man, const int month, const int year)
{
    MAINTENANCE& maintenance = man.car.maintenance;

    if (!man.car.does_exist) return;

    if (month != man.car.month_start_have) return;
     
    if (year == man.car.yaer_start_have) return;     // в год получения ТО не нужно

    pay_rub(maintenance.work_cost, man, EXPENSE_CATEGORY::car);

    car_service_part(man, maintenance.engine_oil);
    car_service_part(man, maintenance.cabin_filter);
    car_service_part(man, maintenance.air_filter);
    car_service_part(man, maintenance.brake_fluid);
    car_service_part(man, maintenance.spark_plugs);
    car_service_part(man, maintenance.brake_pads);
    car_service_part(man, maintenance.coolant);
    car_service_part(man, maintenance.battery);
    car_service_part(man, maintenance.timing_belt);
}


RUB get_bob_deposits_sum(const PERSON& bob)
{
    RUB total = 0;

    for (const BANKING_ORGANIZATION& bank : bob.banks) {
        for (const TIME_DEPOSITS& deposit : bank.deposits) {
            if (!deposit.does_exist) continue;

            total += deposit.sum;
        }
    }

    return total;
}


RUB get_bob_money(const PERSON& bob)
{
    RUB total = 0;

    total += bob.cash;

    if (bob.car.does_exist) {
        total += bob.car.now_cost;
    }

    total += get_bob_deposits_sum(bob);

    return total;
}


RUB get_alise_money(const PERSON& alise)
{
    RUB total = 0;

    total += alise.cash;

    if (alise.car.does_exist) {
        total += alise.car.now_cost;
    }

    if (alise.home_alise.does_exist) {
        total += alise.home_alise.now_cost;

        total -= alise.home_alise.mortgage_pay;
    }

    return total;
}


void who_win(const PERSON& alise, const PERSON& bob)
{
    RUB alise_money = get_alise_money(alise);
    RUB bob_money = get_bob_money(bob);

    std::cout   << "            WHO WIN\n"
                << "ALISE money = " << alise_money << "\n"
                << "BOB money   = " << bob_money   << "\n";

    if (alise_money > bob_money) {
        std::cout << "WINNER = ALISE\n";
    } else if (bob_money > alise_money) {
        std::cout << "WINNER =  BOB\n";
    } else {
        std::cout << "DRAW\n";
    }
}


void man_print(const PERSON& man)
{
    std::cout   << "##############################################################################\n"
                << "            PERSON\n"
                << "name = "  << man.name << "\n"
                << "life = "  << man.is_alive << "\n"
                << "age_years = "  << man.age_years << "\n"
                << "age_months = " << man.age_months << "\n"
                << "cash = " << man.cash << "\n"
                << "debt = " << man.debt << "\n"
                << "tax_duty = " << man.tax_duty << "\n"
                << "reason_end = " << man.reason_end << "\n"
                << "        SIMULATION\n"
                << " START  " << man.simulation_start_year << " year " << man.simulation_start_month << " month " << "\n"
                << " NOW    " << man.year_now << " year " << man.month_now << " month " << "\n"
                << " SEED   " << man.simulation_start_seed << "\n";
}


void man_print_works(const PERSON& man)
{
    std::cout << "            WORKS\n";

    for (size_t id_job = 0; id_job < man.works.size(); id_job++) {
        const JOB& job = man.works[id_job];

        if (!job.does_exist) continue;

        std::cout   << "job[" << id_job << "]\n"
                    << "salary = " << job.salary << "\n"
                    << "year_salary_pay = " << job.year_salary_pay << "\n"
                    << "reduce_salary = " << job.reduce_salary << "\n"
                    << "chance_reduce_salary = " << job.chance_reduce_salary << "\n"
                    << "increase_salary = " << job.increase_salary << "\n"
                    << "chance_increase_salary = " << job.chance_increase_salary << "\n"
                    << "sick_day_pay = " << job.sick_day_pay << "\n";
    }
}


void man_print_statistic(const PERSON& man)
{
    std::cout   << "            DATA YEAR\n"
                << "pay_year = " << man.data.pay_year << "\n"
                << "salary_year = " << man.data.salary_year << "\n"
                << "tax_year_ndfl = " << man.data.tax_year_ndfl << "\n"
                << "mortgage_year_pay = " << man.data.mortgage_year_pay << "\n"
                << "profit_interest_on_savings_year = " << man.data.profit_interest_on_savings_year << "\n"
                << "medical_year = " << man.data.medical_year << "\n"
                << "sum_sell_car_year = " << man.data.sum_sell_car_year << "\n"
                << "lottery_win_year = " << man.data.lottery_win_year << "\n"
                << "tax_year_ndfl_lottery = " << man.data.tax_year_ndfl_lottery << "\n"
                << "            DATA LIFE\n"
                << "pay_life = " << man.data.pay_life << "\n"
                << "salary_life = " << man.data.salary_life << "\n"
                << "tax_life = " << man.data.tax_life << "\n"
                << "profit_interest_on_savings_life = " << man.data.profit_interest_on_savings_life << "\n"
                << "medical_life = " << man.data.medical_life << "\n"
                << "sick_once_life = " << man.data.sick_once_life << "\n"
                << "sick_work_day_life = " << man.data.sick_work_day_life << "\n"
                << "sum_sell_car_life = " << man.data.sum_sell_car_life << "\n"
                << "lottery_win_life = " << man.data.lottery_win_life << "\n"
                << "            OTHER\n"
                << "pay_year_last_month = " << man.data.pay_year_last_month << "\n"
                << "pay_month = " << man.data.pay_month << "\n"
                << "financial_cushion = " << man.data.financial_cushion << "\n"
                << "month_want_get_job = " << man.data.month_want_get_job << "\n"
                << "month_want_quit_job = " << man.data.month_want_quit_job << "\n"
                << "standart_salary = " << man.data.standart_salary << "\n"
                << "working_days_in_month = " << man.data.working_days_in_month << "\n";
}


void man_print_tax_book(const PERSON& man)
{
    const RUSSIA_TAX& tax = man.tax_book;

    std::cout   << "            RUSSIA TAX\n";

    for (size_t id = 0; id < tax.individual_tax_limit.size(); id++) {
        std::cout   << "individual_tax_limit[" << id << "] = " << tax.individual_tax_limit[id] << "\n"
                    << "NDFL[" << id << "] = " << tax.NDFL[id] << "\n";
    }

    for (size_t id = 0; id < tax.interest_individual_tax_limit.size(); id++) {
        std::cout   << "interest_individual_tax_limit[" << id << "] = " << tax.interest_individual_tax_limit[id] << "\n"
                    << "interest_NDFL[" << id << "] = " << tax.interest_NDFL[id] << "\n";
    }

    std::cout   << "area_not_tax = " << tax.area_not_tax << "\n"
                << "area_rate = " << tax.area_rate << "\n"
                << "sum_refund_pay_mortgage_tax_max = " << tax.sum_refund_pay_mortgage_tax_max << "\n"
                << "save_deduction = " << tax.save_deduction << "\n"
                << "savings_free_base = " << tax.savings_free_base << "\n"
                << "key_rate_max = " << tax.key_rate_max << "\n"
                << "savings_rate = " << tax.savings_rate << "\n"
                << "sum_max_year_refund_medical = " << tax.sum_max_year_refund_medical << "\n"
                << "min_ownership_years_no_tax_car = " << tax.min_ownership_years_no_tax_car << "\n"
                << "max_cost_car_no_tax = " << tax.max_cost_car_no_tax << "\n"
                << "lottery_no_tax = " << tax.lottery_no_tax << "\n";
}


void man_print_inflation(const PERSON& man)
{
    const INFLATION& inf = man.inflation_book;

    std::cout   << "            INFLATION\n"
                << "LOW = " << inf.LOW << "\n"
                << "NORMAL = " << inf.NORMAL << "\n"
                << "HIGT = " << inf.HIGT << "\n"
                << "utilities_inflation = " << inf.utilities_inflation << "\n"
                << "internet_mobile_inflation = " << inf.internet_mobile_inflation << "\n"
                << "food_inflation = " << inf.food_inflation << "\n"
                << "clothes_inflation = " << inf.clothes_inflation << "\n"
                << "leisure_inflation = " << inf.leisure_inflation << "\n"
                << "other_inflation = " << inf.other_inflation << "\n"
                << "over_the_counter_drugs_inflation = " << inf.over_the_counter_drugs_inflation << "\n"
                << "rent_inflation = " << inf.rent_inflation << "\n"
                << "transport_inflation = " << inf.transport_inflation << "\n"
                << "benzin_inflation = " << inf.benzin_inflation << "\n"
                << "pet_food_inflation = " << inf.pet_food_inflation << "\n"
                << "lottery_ticket_inflation = " << inf.lottery_ticket_inflation << "\n"
                << "doctor_visit_inflation = " << inf.doctor_visit_inflation << "\n"
                << "medicine_inflation = " << inf.medicine_inflation << "\n"
                << "car_now_cost_inflation = " << inf.car_now_cost_inflation << "\n"
                << "car_insurance_inflation = " << inf.car_insurance_inflation << "\n"
                << "car_tax_inflation = " << inf.car_tax_inflation << "\n"
                << "apartment_now_cost_inflation = " << inf.apartment_now_cost_inflation << "\n"
                << "individual_tax_limit_inflation = " << inf.individual_tax_limit_inflation << "\n"
                << "interest_individual_tax_limit_inflation = " << inf.interest_individual_tax_limit_inflation << "\n"
                << "savings_free_base_inflation = " << inf.savings_free_base_inflation << "\n"
                << "sum_max_year_refund_medical_inflation = " << inf.sum_max_year_refund_medical_inflation << "\n"
                << "lottery_no_tax_inflation = " << inf.lottery_no_tax_inflation << "\n"
                << "standart_salary_inflation = " << inf.standart_salary_inflation << "\n"
                << "salary_inflation = " << inf.salary_inflation << "\n"
                << "min_sum_deposits_inflation = " << inf.min_sum_deposits_inflation << "\n"
                << "lottery_prize_inflation = " << inf.lottery_prize_inflation << "\n"
                << "wheels_change_inflation = " << inf.wheels_change_inflation << "\n"
                << "wheels_set_inflation = " << inf.wheels_set_inflation << "\n"
                << "maintenance_work_inflation = " << inf.maintenance_work_inflation << "\n"
                << "maintenance_engine_oil_inflation = " << inf.maintenance_engine_oil_inflation << "\n"
                << "maintenance_cabin_filter_inflation = " << inf.maintenance_cabin_filter_inflation << "\n"
                << "maintenance_air_filter_inflation = " << inf.maintenance_air_filter_inflation << "\n"
                << "maintenance_brake_fluid_inflation = " << inf.maintenance_brake_fluid_inflation << "\n"
                << "maintenance_spark_plugs_inflation = " << inf.maintenance_spark_plugs_inflation << "\n"
                << "maintenance_brake_pads_inflation = " << inf.maintenance_brake_pads_inflation << "\n"
                << "maintenance_coolant_inflation = " << inf.maintenance_coolant_inflation << "\n"
                << "maintenance_battery_inflation = " << inf.maintenance_battery_inflation << "\n"
                << "maintenance_timing_belt_inflation = " << inf.maintenance_timing_belt_inflation << "\n";
}


void man_print_apartment(const PERSON& man)
{
    const APARTMENT& home = man.home_alise;

    std::cout   << "            APARTMENT\n"
                << "does_exist = " << home.does_exist << "\n"
                << "now_cost = " << home.now_cost << "\n"
                << "mortgage_pay = " << home.mortgage_pay << "\n"
                << "area_apartment = " << home.area_apartment << "\n"
                << "body_payments = " << home.body_payments << "\n";
}


void man_print_expenses(const PERSON& man)
{
    const EXPENSES_STANDARD& exp = man.expenses;

    std::cout   << "            EXPENSES\n"
                << "utilities = " << exp.utilities << "\n"
                << "internet_mobile = " << exp.internet_mobile << "\n"
                << "food = " << exp.food << "\n"
                << "clothes = " << exp.clothes << "\n"
                << "leisure = " << exp.leisure << "\n"
                << "other = " << exp.other << "\n"
                << "over_the_counter_drugs = " << exp.over_the_counter_drugs << "\n"
                << "rent = " << exp.rent << "\n"
                << "mortgage = " << exp.mortgage << "\n"
                << "transport = " << exp.transport << "\n"
                << "benzin = " << exp.benzin << "\n"
                << "pet_food = " << exp.pet_food << "\n"
                << "lottery_ticket = " << exp.lottery_ticket << "\n";
}


void man_print_wheels(const PERSON& man)
{
    const WHEELS& wheels = man.car.wheels;

    std::cout   << "            WHEELS\n"
                << "does_exist = " << wheels.does_exist << "\n"
                << "change_cost = " << wheels.change_cost << "\n"
                << "set_cost = " << wheels.set_cost << "\n"
                << "summer_life_max = " << wheels.summer_life_max << "\n"
                << "winter_life_max = " << wheels.winter_life_max << "\n"
                << "summer_life_left = " << wheels.summer_life_left << "\n"
                << "winter_life_left = " << wheels.winter_life_left << "\n"
                << "is_winter_now = " << wheels.is_winter_now << "\n";
}


void man_print_service_part(const SERVICE_PART& part)
{
    std::cout   << "            SERVICE PART\n"
                << "name = " << part.name << "\n"
                << "cost = " << part.cost << "\n"
                << "life_max = " << part.life_max << "\n"
                << "life_left = " << part.life_left << "\n";
}


void man_print_maintenance(const PERSON& man)
{
    const MAINTENANCE& m = man.car.maintenance;

    std::cout   << "            MAINTENANCE\n"
                << "work_cost = " << m.work_cost << "\n";

    man_print_service_part(m.engine_oil);
    man_print_service_part(m.cabin_filter);
    man_print_service_part(m.air_filter);
    man_print_service_part(m.brake_fluid);
    man_print_service_part(m.spark_plugs);
    man_print_service_part(m.brake_pads);
    man_print_service_part(m.coolant);
    man_print_service_part(m.battery);
    man_print_service_part(m.timing_belt);
}


void man_print_car(const PERSON& man)
{
    const AUTOMOBILE& car = man.car;

    std::cout   << "            CAR\n"
                << "does_exist = " << car.does_exist << "\n"
                << "name = " << car.name << "\n"
                << "configuration = " << car.configuration << "\n"
                << "now_cost = " << car.now_cost << "\n"
                << "car_tax = " << car.car_tax << "\n"
                << "yaer_start_have = " << car.yaer_start_have << "\n"
                << "month_start_have = " << car.month_start_have << "\n"
                << "car_insurance = " << car.car_insurance << "\n"
                << "all_cost = " << car.all_cost << "\n"
                << "breakdown_history = " << car.breakdown_history << "\n";

    man_print_wheels(man);

    man_print_maintenance(man);
}


void man_print_bank_deposit(const TIME_DEPOSITS& deposit)
{
    std::cout   << "            DEPOSIT\n"
                << "sum = " << deposit.sum << "\n"
                << "rate = " << deposit.rate << "\n"
                << "start_year = " << deposit.start_year << "\n"
                << "start_month = " << deposit.start_month << "\n"
                << "time_month = " << deposit.time_month << "\n"
                << "waiting_time = " << deposit.waiting_time << "\n"
                << "closed_early = " << deposit.closed_early << "\n";
}


void man_print_bank(const BANKING_ORGANIZATION& bank)
{
    if (bank.name == "") return;

    std::cout   << "            BANK\n"
                << "name = " << bank.name << "\n"
                << "min_sum_deposits = " << bank.min_sum_deposits << "\n"
                << "max_rate_deposit = " << bank.max_rate_deposit << "\n"
                << "min_rate_deposit = " << bank.min_rate_deposit << "\n"
                << "rate_change_step = " << bank.rate_change_step << "\n";

    for (size_t id = 0; id < bank.time_deposit.size(); id++) {
        std::cout   << "time_deposit[" << id << "] = " << bank.time_deposit[id] << "\n"
                    << "rate_deposit[" << id << "] = " << bank.rate_deposit[id] << "\n";
    }

    for (const TIME_DEPOSITS& deposit : bank.deposits) {
        if (!deposit.does_exist) continue;

        man_print_bank_deposit(deposit);
    }
}


void man_print_bank_all(const PERSON& man)
{
    std::cout << "            BANKS\n";

    for (const BANKING_ORGANIZATION& bank : man.banks) {
        man_print_bank(bank);
    }
}


void man_print_pet(const PERSON& man)
{
    const ANIMAL& pet = man.pet;

    std::cout   << "            PET\n"
                << "is_alive = " << pet.is_alive << "\n"
                << "life_months_left = " << pet.life_months_left << "\n"
                << "chance_get_new = " << pet.chance_get_new << "\n"
                << "life_months_min = " << pet.life_months_min << "\n"
                << "life_months_max = " << pet.life_months_max << "\n";
}


void man_print_sick(const PERSON& man)
{
    const ILLNESS& sick = man.sick;

    std::cout   << "            SICK\n"
                << "is_sick = " << sick.is_sick << "\n"
                << "sick_work_days = " << sick.sick_work_days << "\n"
                << "chance_get_sick = " << sick.chance_get_sick << "\n"
                << "sick_days_min = " << sick.sick_days_min << "\n"
                << "doctor_visit = " << sick.doctor_visit << "\n"
                << "medicine = " << sick.medicine << "\n";
}


void man_print_lottery_big(const PERSON& man)
{
    const LOTTERY_BIG& lot = man.lottery_big;

    std::cout   << "            LOTTERY BIG\n"
                << "prize_1 = " << lot.prize_1 << "\n"
                << "prize_2 = " << lot.prize_2 << "\n"
                << "prize_3 = " << lot.prize_3 << "\n"
                << "prize_4 = " << lot.prize_4 << "\n"
                << "prize_5 = " << lot.prize_5 << "\n"
                << "prize_6 = " << lot.prize_6 << "\n"
                << "prize_7 = " << lot.prize_7 << "\n"
                << "chance_1 = " << lot.chance_1 << "\n"
                << "chance_2 = " << lot.chance_2 << "\n"
                << "chance_3 = " << lot.chance_3 << "\n"
                << "chance_4 = " << lot.chance_4 << "\n"
                << "chance_5 = " << lot.chance_5 << "\n"
                << "chance_6 = " << lot.chance_6 << "\n"
                << "chance_7 = " << lot.chance_7 << "\n";
}


void print_man_all(const PERSON& man)
{
    man_print(man);

    man_print_works(man);

    man_print_statistic(man);

    man_print_tax_book(man);

    man_print_inflation(man);

    man_print_apartment(man);

    man_print_expenses(man);

    man_print_car(man);

    man_print_bank_all(man);

    man_print_pet(man);

    man_print_sick(man);

    man_print_lottery_big(man);
}


void simulation_alise(PERSON& alise)
{
    init_alise(alise);

    PERSON const_man_clone;

    init_alise(const_man_clone);

    int month = alise.simulation_start_month;
    int year = alise.simulation_start_year;
    bool end = false;

    set_random_seed(alise.simulation_start_seed);

    while (!end)
    {
        // события года
        get_refund_NDFL_tax(alise, month);
        get_apartment_tax(alise, month);
        get_car_tax(alise, month);
        get_interest_on_savings_tax(alise, month);
        get_lottery_tax(alise, month);

        reload_data(alise, month);

        tax_duty_pay(alise, month);

        inflation_month(alise, const_man_clone, month, year);

        inflation_year(alise, const_man_clone, month, year);
        
        pay_rub(alise.expenses.utilities, alise);
        pay_rub(alise.expenses.internet_mobile, alise);
        pay_rub(alise.expenses.food, alise);
        pay_rub(alise.expenses.clothes, alise);
        pay_rub(alise.expenses.leisure, alise);
        pay_rub(alise.expenses.other, alise);
        pay_rub(alise.expenses.over_the_counter_drugs, alise);
        
        pay_mortgage(alise);
        
        get_pet(alise);

        pay_pet_food(alise);

        pay_transport(alise);

        pay_car(alise, month);

        change_wheels(alise, month);

        car_maintenance(alise, month, year);

        change_salary_random(alise,month);

        get_sick(alise);

        get_salary(alise);
        
        get_quit_job(alise);

        go_lottery(alise);

        // после ничего не добавлять
        debt_pay_alise(alise, month, year);

        interest_on_savings_alise(alise);

        check_end_alise(end, year, alise);

        time_step(month, year, alise, end);
    }

    print_man_all(alise);
}


void simulation_bob(PERSON& bob) 
{
    init_bob(bob);

    PERSON const_man_clone;

    init_bob(const_man_clone);

    int month = bob.simulation_start_month;
    int year = bob.simulation_start_year;
    bool end = false;

    set_random_seed(bob.simulation_start_seed);

    while (!end)
    {
        // события года
        get_refund_NDFL_tax(bob, month);
        get_apartment_tax(bob, month);
        get_car_tax(bob, month);
        get_interest_on_savings_tax(bob, month);
        get_lottery_tax(bob, month);

        reload_data(bob, month);

        tax_duty_pay(bob, month);

        inflation_month(bob, const_man_clone, month, year);

        inflation_year(bob, const_man_clone, month, year);
        
        pay_rub(bob.expenses.utilities, bob);
        pay_rub(bob.expenses.internet_mobile, bob);
        pay_rub(bob.expenses.food, bob);
        pay_rub(bob.expenses.clothes, bob);
        pay_rub(bob.expenses.leisure, bob);
        pay_rub(bob.expenses.other, bob);
        pay_rub(bob.expenses.over_the_counter_drugs, bob);
       
        get_pet(bob);

        pay_pet_food(bob);

        pay_transport(bob);

        pay_car(bob, month);

        change_wheels(bob, month);

        car_maintenance(bob, month, year);

        change_salary_random(bob,month);

        get_sick(bob);

        get_salary(bob);
        
        get_quit_job(bob);

        go_lottery(bob);
        // после ничего не добавлять
        debt_pay_bob(bob, month, year);

        interest_on_savings_bob(bob);

        check_end_bob(end, year, bob);

        time_step(month, year, bob, end);
    }

    print_man_all(bob);
}


int main()
{
    PERSON alise;

    simulation_alise(alise);

    PERSON bob;

    simulation_bob(bob);

    who_win(alise, bob);

    return 0;
}
