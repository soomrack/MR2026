#include <stdio.h>
#include <random>

using RUB = unsigned long long int;
using SRUB = long long int;
//РАНДОМ///////////////////////////////////////////////////////////////////////////////////////////
std::mt19937 rng(std::random_device{}()); // числа всегда разные

bool chance(int p) {
	return (rng() % 100) < p;
}

int random_range(int lo, int hi) {
	return hi <= lo ? lo : lo + (rng() % (hi - lo + 1));
}
///////////////////////////////////////////////////////////////////////////////////////////////////
enum Lifestyle {
	START_OF_LIFE,  // экономненько, ровненько
	MIDDLE_CLASS,   // можно позволить себе еду подороже
	BOURGEOISIE,    // повод задуматься о внедорожнике
	ELITE           // хороший ремонт, покупка роскоши, спорткар в планах
};

enum HealthStatus {
    HEALTHY,
    SICK
};

enum CarType {
    NO_CAR,       // машины нет — значение по умолчанию
    SMALL_CAR,    // бюджетная
    BIG_CAR,      // крупная
    SPORT_CAR     // спортивная
};

enum InflCategory {
    INF_FOOD, //
    INF_UTILITIES, //
    INF_TRANSPORT, //
    INF_HEALTH,
    INF_ENTERTAINMENT,
    INF_CLOTHES,
    INF_DOG, //
    INF_MISC,
	IND_INCOME,
    INF_CATEGORY_COUNT
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Bank {
	RUB depos;
	SRUB creditka;
	RUB credit_limit;
	int  credit_months;
	bool credit_active;
	//RUB stocks;
	RUB min_reserve;
	RUB max_reserve;
	RUB base_depos;
};

//struct Bank simbank;
//struct Bank denezhka;

struct statistica {
	RUB total_income;
	RUB total_expences;
	RUB max_month_expences;
	RUB max_year_expences;
	RUB max_month_income;
	RUB max_year_income;
	SRUB max_month_profit;
	SRUB max_year_profit;
};

struct statistica stata;
struct statistica statb;

struct short_stats { 
	RUB month_expences;
	RUB year_expences;
	RUB month_income;
	RUB year_income;
	SRUB month_profit;
	SRUB year_profit;
};

struct short_stats shortstat_a;
struct short_stats shortstat_b;

struct Income_stats {
	RUB main_job_ndfl_year;
	RUB part_time_ndfl_year;
	RUB main_job_received_year;
	RUB part_time_received_year;
};

struct Income {
	RUB salary;
	RUB indexed_salary;
	RUB part_time_job;
	RUB indexed_part_time; 
};

struct TaxRefund {
    RUB property_base_left;
    RUB interest_base_left;
    RUB refunded_total;
};

struct Bankruptcy {
    bool bancrupt;
    int  year;
    int  month;
    RUB  deficit;       // сколько не хватило в момент банкротства
    const char* reason; // причина банкротства чисто для статы выводится строкой
};

struct CarBreakdown {
    // Мелкие поломки
    bool electrics_minor;        // лампочки, предохранители
    bool wipers_washer;          // щётки, омыватель
    bool oil_filter_early;       // внеплановая замена масла
    
    // Средние поломки
    bool battery;                // аккумулятор
    bool brakes;                 // тормозные колодки/диски
    bool suspension;             // подвеска
    bool tires;                  // шины
    
    // Серьёзные поломки
    bool electrics_major;        // генератор, стартер, датчики
    bool cooling_system;         // система охлаждения
    bool gearbox_minor;          // коробка передач (мелкий ремонт)
    bool engine_minor;           // двигатель (некрупный ремонт)
    
    // Катастрофические поломки
    bool engine_overhaul;        // капитальный ремонт двигателя
    bool accident;               // ДТП
    
    int broken_months;           // сколько месяцев машина в ремонте
};

struct FoodMenu {
	RUB breakfast;          // завтрак (средний чек)
	RUB lunch;              // обед в кафе рядом с работой
	RUB dinner;             // ужин (продукты)
	RUB snacks;             // перекусы в течение дня
	RUB coffee;             // кофе/чай вне дома
	RUB restaurants;        // поход в ресторан (средний чек)
	RUB groceries_weekly;   // закупка продуктов на неделю
	RUB delivery_monthly;   // доставка еды за месяц
};

struct FoodMenu g_food_menu;

struct FoodPlan {
    int breakfast_times;
    int lunch_times;
    int dinner_times;
    int snacks_times;
    int coffee_times;
    int restaurant_times;
    int groceries_times;
    int delivery_times;
};

struct Expences {
	double lifestyle_coefficient;
	RUB food;
	RUB dog;
	RUB mortgage;
	RUB rent;
	RUB home_taxes;
	RUB home_bills;
};

struct Car {
	CarType car_type;
	RUB price;       // цена машины - при желании выведу в статку
	RUB osago;
	RUB kasco;
    RUB utilsbor;    
	RUB nalog;
	RUB expences;
	RUB repair_cost;
	double fix_coefficient;
	CarBreakdown breakdown;
};

struct Car smallcar;
struct Car bigcar;
struct Car sportcar;

struct Person {
	Lifestyle lyfestile;
	HealthStatus health;
	bool strong_drugs;
	int sick_months_left;
	RUB cash;
	Income income;
	Income_stats income_stats;
	Expences expences;
	RUB food_budget;
	Car car;
	Bank bank;
	RUB car_savings;
	bool mortgage_completed;
	bool lives_with_parents;
	bool have_dog;
	Bankruptcy bankruptcy;
	FoodPlan food_plan;
	TaxRefund tax_refund;
};


struct Inflation {
    double rate[INF_CATEGORY_COUNT];      // годовая ставка
    double multiplier[INF_CATEGORY_COUNT]; // накопленный коэффициент с 2026 года
};

Inflation g_inflation;

struct Person alice;
struct Person bob;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int year = 2026;
int month = 9;


void zero_car(Car& c) {
	c.fix_coefficient = 1;
    c.price    = 0;
    c.utilsbor = 0;
    c.osago    = 0;
    c.kasco    = 0;
    c.nalog    = 0;
    c.expences = 0;
	c.repair_cost = 0;

	c.breakdown.electrics_minor   = false;
    c.breakdown.wipers_washer     = false;
    c.breakdown.oil_filter_early  = false;
    c.breakdown.battery           = false;
    c.breakdown.brakes            = false;
    c.breakdown.suspension        = false;
    c.breakdown.tires             = false;
    c.breakdown.electrics_major   = false;
    c.breakdown.cooling_system    = false;
    c.breakdown.gearbox_minor     = false;
    c.breakdown.engine_minor      = false;
    c.breakdown.engine_overhaul   = false;
    c.breakdown.accident          = false;
    c.breakdown.broken_months     = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//ALICE
void alice_init()
{
	zero_car(alice.car);
	alice.health = HEALTHY;
	alice.strong_drugs = false;
	alice.sick_months_left = 0;
	alice.cash = 20'000;
	alice.car.car_type = NO_CAR;
	alice.car_savings = 0;
	alice.mortgage_completed = false;
	alice.lives_with_parents = true;
	alice.have_dog = false;
}

void alice_income_init()
{
	alice.income.salary = 80'000;
	alice.income.indexed_salary = 80'000;
	alice.income.part_time_job = 0;
	alice.income.indexed_part_time = 0;
}

void alice_bank_init()
{
	alice.bank.depos = 0;
	alice.bank.creditka = 0;
	alice.bank.credit_limit = 200'000;
	alice.bank.credit_months = 0;
	alice.bank.credit_active = false;
}
void alice_base_expences_init()
{
	alice.expences.lifestyle_coefficient = 1.0;
	alice.expences.food = 25'000; // Подвергается инфляции
	alice.expences.dog = 5'000;
	alice.expences.rent = 8'000;
	alice.expences.home_bills = 6'000;    
	alice.expences.home_taxes = 0;

	alice.expences.mortgage = 50'000;  // Не подвергается
}

void alice_bancrupt_init()
{
	alice.bankruptcy.bancrupt = false;
	alice.bankruptcy.year = 0;
	alice.bankruptcy.month = 0;
	alice.bankruptcy.deficit = 0;
	alice.bankruptcy.reason = "";
}

void alice_tax_refund_init()
{
    alice.tax_refund.property_base_left = 2'000'000;
    alice.tax_refund.interest_base_left = 3'000'000;
    alice.tax_refund.refunded_total    = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void alice_stats_init()
{
	stata.total_income = 0;
	stata.total_expences = 0;
	stata.max_month_expences = 0;
	stata.max_year_expences = 0;
	stata.max_month_income = 0;
	stata.max_year_income = 0;
	stata.max_month_profit = 0;
	stata.max_year_profit = 0;
}
void alice_short_stats_init()
{
	shortstat_a.month_expences = 0;
	shortstat_a.year_expences = 0;
	shortstat_a.month_income = 0;
	shortstat_a.year_income = 0;
	shortstat_a.month_profit = 0;
	shortstat_a.year_profit = 0;

	alice.income_stats.main_job_ndfl_year = 0;
	alice.income_stats.part_time_ndfl_year = 0;
	alice.income_stats.main_job_received_year = 0;
	alice.income_stats.part_time_received_year = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//BOB
void bob_init()
{
	zero_car(bob.car);
	bob.health = HEALTHY;
	bob.strong_drugs = false;
	bob.sick_months_left = 0;
	bob.cash = 50'000;
	bob.car.car_type = NO_CAR;
	bob.car_savings = 0;
	bob.mortgage_completed = false;
	bob.lives_with_parents = false;
	bob.have_dog = false;
}

void bob_income_init()
{
	bob.income.salary = 120'000;
	bob.income.indexed_salary = 120'000;
	bob.income.part_time_job = 0;
	bob.income.indexed_part_time = 0;
}

void bob_bank_init()
{
	bob.bank.depos = 0;
	bob.bank.creditka = 0;
	bob.bank.credit_limit = 200'000;
	bob.bank.credit_months = 0;
	bob.bank.credit_active = false;
}
void bob_base_expences_init()
{
	bob.expences.lifestyle_coefficient = 1.0;
	bob.expences.food = 30'000; // Подвергается инфляции
	bob.food_budget = 30'000;
	bob.expences.home_taxes = 0;
	bob.expences.dog = 0;
	bob.expences.rent = 8'500;
	bob.expences.home_bills = 6'000;    

	bob.expences.mortgage = 60'000;  // Не подвергается
}

void bob_bancrupt_init()
{
	bob.bankruptcy.bancrupt = false;
	bob.bankruptcy.year = 0;
	bob.bankruptcy.month = 0;
	bob.bankruptcy.deficit = 0;
	bob.bankruptcy.reason = "";
}

void bob_tax_refund_init()
{
    bob.tax_refund.property_base_left = 2'000'000;
    bob.tax_refund.interest_base_left = 3'000'000;
    bob.tax_refund.refunded_total    = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void bob_stats_init()
{
	statb.total_income = 0;
	statb.total_expences = 0;
	statb.max_month_expences = 0;
	statb.max_year_expences = 0;
	statb.max_month_income = 0;
	statb.max_year_income = 0;
	statb.max_month_profit = 0;
	statb.max_year_profit = 0;
}

void bob_short_stats_init()
{
	shortstat_b.month_expences = 0;
	shortstat_b.year_expences = 0;
	shortstat_b.month_income = 0;
	shortstat_b.year_income = 0;
	shortstat_b.month_profit = 0;
	shortstat_b.year_profit = 0;

	bob.income_stats.main_job_ndfl_year = 0;
	bob.income_stats.part_time_ndfl_year = 0;
	bob.income_stats.main_job_received_year = 0;
	bob.income_stats.part_time_received_year = 0;
}

void bob_food_plan_init()
{
	bob.food_plan.breakfast_times = 0;
	bob.food_plan.lunch_times = 0;
	bob.food_plan.dinner_times = 0;
    bob.food_plan.snacks_times = 0;
    bob.food_plan.coffee_times = 0;
    bob.food_plan.restaurant_times = 0;
    bob.food_plan.groceries_times = 0;
    bob.food_plan.delivery_times = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void food_menu_init()
{
    g_food_menu.breakfast      = 300;
    g_food_menu.lunch          = 500;
    g_food_menu.dinner         = 400;
    g_food_menu.snacks         = 200;
    g_food_menu.coffee         = 200;
    g_food_menu.restaurants    = 3'000;
    g_food_menu.groceries_weekly = 3'000;
    g_food_menu.delivery_monthly = 1'500;
}

void cars_init()
{
	smallcar.fix_coefficient = 1.0;
	smallcar.price = 2'000'000;
	smallcar.kasco = 5'000;
	smallcar.nalog = 3'000;
	smallcar.osago = 650; // взял 7800/12=650
	smallcar.utilsbor = 3'400;
	smallcar.expences = 20'000;

	bigcar.fix_coefficient = 1.25;
	bigcar.price = 5'000'000;
	bigcar.kasco = 7'000;
	bigcar.nalog = 5'000;
	bigcar.osago = 800;
	bigcar.utilsbor = bigcar.price * 0.5;
	bigcar.expences = 25'000;

	sportcar.fix_coefficient = 2.15;
	sportcar.price = 10'000'000;
	sportcar.kasco = 8'000;
	sportcar.nalog = 8'000;
	sportcar.osago = 850;
	sportcar.utilsbor = sportcar.price*0.4;
	sportcar.expences = 35'000;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void inflation_init()
{
    g_inflation.rate[INF_FOOD]          = 0.09;
    g_inflation.rate[INF_UTILITIES]     = 0.07;
    g_inflation.rate[INF_TRANSPORT]     = 0.07;
    g_inflation.rate[INF_HEALTH]        = 0.10;
    g_inflation.rate[INF_ENTERTAINMENT] = 0.08;
    g_inflation.rate[INF_CLOTHES]       = 0.06;
    g_inflation.rate[INF_DOG]           = 0.10;
    g_inflation.rate[INF_MISC]          = 0.08;
	g_inflation.rate[IND_INCOME]        = 0.075;

    for (int i = 0; i < INF_CATEGORY_COUNT; i++) {
        g_inflation.multiplier[i] = 1.0;
    }
}


void all_init()
{
	alice_bank_init();
	alice_income_init();
	alice_base_expences_init();
	alice_bancrupt_init();
	alice_tax_refund_init();
	alice_init();
	alice_stats_init();
	alice_short_stats_init();

	bob_bank_init();
	bob_income_init();
	bob_base_expences_init();
	bob_bancrupt_init();
	bob_tax_refund_init();
	bob_init();
	bob_stats_init();
	bob_short_stats_init();

	food_menu_init();
	bob_food_plan_init();
	cars_init();
	inflation_init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void pay_mandatory(Person& p, RUB amount, const char* reason)
{
    // 1. Наличка
    if (p.cash >= amount) {
        p.cash -= amount;
        return;
    }
    RUB remaining = amount - p.cash;
    p.cash = 0;

    // 2. Вклад
    if (p.bank.depos >= remaining) {
        p.bank.depos -= remaining;
        return;
    }
    remaining -= p.bank.depos;
    p.bank.depos = 0;

    // 3. Кредитка
    SRUB available_signed = (SRUB)p.bank.credit_limit + p.bank.creditka;
    RUB  available        = (available_signed > 0) ? (RUB)available_signed : 0;

    if (available >= remaining) {
        p.bank.creditka -= (SRUB)remaining;
        p.bank.credit_months = 0;
        p.bank.credit_active = true;
        return;
    }

    // 4. Даже кредитка не спасает — банкротство
    RUB deficit = remaining - available;
    p.bank.creditka -= (SRUB)available;   // выгребаем всё, что можно

    if (!p.bankruptcy.bancrupt) {
        p.bankruptcy.bancrupt = true;
        p.bankruptcy.year     = year;
        p.bankruptcy.month    = month;
        p.bankruptcy.deficit  = deficit;
        p.bankruptcy.reason   = reason;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void inflation_update()
{
	if (!(month == 1)) return;

    for (int i = 0; i < INF_CATEGORY_COUNT; i++) {
        g_inflation.multiplier[i] *= (1.0 + g_inflation.rate[i]);
    }
}

RUB inflate(RUB cost, InflCategory cat)
{
    double result = (double)cost * g_inflation.multiplier[cat];
    return (RUB)(result + 0.5);   // округление до рубля
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_lifestyle_bank_savings_upd()
{
	if (alice.lyfestile == START_OF_LIFE) 
	{
		alice.bank.min_reserve = 20'000;
    	alice.bank.max_reserve = 150'000;
    	alice.bank.base_depos = 5'000;
	}
	else if (alice.lyfestile == MIDDLE_CLASS)
	{
		alice.bank.min_reserve = 40'000;
		alice.bank.max_reserve = 200'000;
		alice.bank.base_depos = 7'500;
	}
	else if (alice.lyfestile == BOURGEOISIE)
	{
		alice.bank.min_reserve = 60'000;
		alice.bank.max_reserve = 250'000;
		alice.bank.base_depos = 10'000;
	}
	else if (alice.lyfestile == ELITE)
	{
		alice.bank.min_reserve = 75'000;
		alice.bank.max_reserve = 300'000;
		alice.bank.base_depos = 15'000;
	}
}

void alice_lifestyle_expences_coefficient_upd()
{
	if (alice.lyfestile == START_OF_LIFE) 
	{
		alice.expences.lifestyle_coefficient = 1.0;
	}
	else if (alice.lyfestile == MIDDLE_CLASS)
	{
		alice.expences.lifestyle_coefficient = 1.1;
	}
	else if (alice.lyfestile == BOURGEOISIE)
	{
		alice.expences.lifestyle_coefficient = 1.25;
	}
	else if (alice.lyfestile == ELITE)
	{
		alice.expences.lifestyle_coefficient = 1.4;
	}
}

void bob_lifestyle_expences_coefficient_upd()
{
	if (bob.lyfestile == START_OF_LIFE) 
	{
		bob.expences.lifestyle_coefficient = 1.0;
	}
	else if (bob.lyfestile == MIDDLE_CLASS)
	{
		bob.expences.lifestyle_coefficient = 1.1;
	}
	else if (bob.lyfestile == BOURGEOISIE)
	{
		bob.expences.lifestyle_coefficient = 1.25;
	}
	else if (bob.lyfestile == ELITE)
	{
		bob.expences.lifestyle_coefficient = 1.4;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
RUB ndfl_year(RUB year_income)
{
    RUB tax = 0;
    RUB rest = year_income;

    // Ступени идут СВЕРХУ вниз: сначала считаем налог с самой высокой части
    if (rest > 50'000'000) {
        tax  += (rest - 50'000'000) * 22 / 100;
        rest  = 50'000'000;
    }
    if (rest > 20'000'000) {
        tax  += (rest - 20'000'000) * 20 / 100;
        rest  = 20'000'000;
    }
    if (rest > 5'000'000) {
        tax  += (rest - 5'000'000) * 18 / 100;
        rest  = 5'000'000;
    }
    if (rest > 2'400'000) {
        tax  += (rest - 2'400'000) * 15 / 100;
        rest  = 2'400'000;
    }
    tax += rest * 13 / 100;

    return tax;
}


RUB ndfl_monthly(RUB income_this_month, RUB income_year_before)
{
    RUB total_before = income_year_before;
    RUB total_after  = income_year_before + income_this_month;

    RUB tax_before = ndfl_year(total_before);
    RUB tax_after  = ndfl_year(total_after);

    return tax_after - tax_before;
}


void alice_ndfl_update()
{
	if (month == 1) 
	{
    alice.income_stats.main_job_ndfl_year = 0;
	alice.income_stats.part_time_ndfl_year = 0;
	alice.income_stats.main_job_received_year = 0;
	alice.income_stats.part_time_received_year = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void bob_build_food_plan(RUB budget)
{
    // Базовое количество раз
    bob.food_plan.breakfast_times   = random_range(15, 25);
    bob.food_plan.lunch_times       = random_range(10, 22);
    bob.food_plan.dinner_times      = random_range(20, 30);
    bob.food_plan.snacks_times      = random_range(8, 20);
    bob.food_plan.coffee_times      = random_range(5, 22);
    bob.food_plan.restaurant_times  = random_range(0, 4);
    bob.food_plan.groceries_times   = random_range(3, 5);   // раз в неделю
    bob.food_plan.delivery_times    = random_range(0, 3);

    // Считаем стоимость корзины
    RUB total = 0;
    total += bob.food_plan.breakfast_times  * inflate(g_food_menu.breakfast,  INF_FOOD);
    total += bob.food_plan.lunch_times      * inflate(g_food_menu.lunch,      INF_FOOD);
    total += bob.food_plan.dinner_times     * inflate(g_food_menu.dinner,     INF_FOOD);
    total += bob.food_plan.snacks_times     * inflate(g_food_menu.snacks,     INF_FOOD);
    total += bob.food_plan.coffee_times     * inflate(g_food_menu.coffee,     INF_FOOD);
    total += bob.food_plan.restaurant_times * inflate(g_food_menu.restaurants, INF_FOOD);
    total += bob.food_plan.groceries_times  * inflate(g_food_menu.groceries_weekly, INF_FOOD);
    total += bob.food_plan.delivery_times   * inflate(g_food_menu.delivery_monthly, INF_FOOD);

    //Если вышли за бюджет — уменьшаем самые дорогие категории
    while (total > budget) {
        if (bob.food_plan.restaurant_times > 0) {
            bob.food_plan.restaurant_times--;
        } else if (bob.food_plan.delivery_times > 0) {
            bob.food_plan.delivery_times--;
        } else if (bob.food_plan.lunch_times > 5) {
            bob.food_plan.lunch_times--;
        } else if (bob.food_plan.coffee_times > 3) {
            bob.food_plan.coffee_times--;
        } else if (bob.food_plan.snacks_times > 3) {
            bob.food_plan.snacks_times--;
        } else if (bob.food_plan.dinner_times > 10) {
            bob.food_plan.dinner_times--;
        } else if (bob.food_plan.breakfast_times > 10) {
            bob.food_plan.breakfast_times--;
        } else {
            break;   // нечего урезать
        }
        
        total = 0;
        total += bob.food_plan.breakfast_times  * inflate(g_food_menu.breakfast,  INF_FOOD);
        total += bob.food_plan.lunch_times      * inflate(g_food_menu.lunch,      INF_FOOD);
        total += bob.food_plan.dinner_times     * inflate(g_food_menu.dinner,     INF_FOOD);
        total += bob.food_plan.snacks_times     * inflate(g_food_menu.snacks,     INF_FOOD);
        total += bob.food_plan.coffee_times     * inflate(g_food_menu.coffee,     INF_FOOD);
        total += bob.food_plan.restaurant_times * inflate(g_food_menu.restaurants, INF_FOOD);
        total += bob.food_plan.groceries_times  * inflate(g_food_menu.groceries_weekly, INF_FOOD);
        total += bob.food_plan.delivery_times   * inflate(g_food_menu.delivery_monthly, INF_FOOD);
    }

    //если остались деньши, можем добавить ресторан
    while (total + inflate(g_food_menu.restaurants, INF_FOOD) <= budget and chance(30)) {
        bob.food_plan.restaurant_times++;
        total += inflate(g_food_menu.restaurants, INF_FOOD);
    }

    bob.expences.food = total;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void alice_bank_income(const int year, const int month)
{
	// Начисление 14% годовых, ежемесячно (сложный процент) === 14 / (100 * 12)
	RUB bank_income = alice.bank.depos * 14 / (100 * 12);
	alice.bank.depos += bank_income;
	// Отдельным вкладом лежат накопления на машину, чтобы по ним тоже капал процент
	RUB car_savings_income = alice.car_savings * 14 / (100 * 12);
	alice.car_savings += car_savings_income;

	stata.total_income += (bank_income + car_savings_income);
	shortstat_a.month_income += (bank_income + car_savings_income);
	shortstat_a.year_income += (bank_income + car_savings_income);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_salary(const int year, const int month)
{

	if (year == 2027 and month == 1) {  // Работает по профилю, уже с опытом работы
		alice.income.salary = 150'000;
		alice.income.indexed_salary = alice.income.salary;
	}

	if (month == 1 and year > 2027)
	{
	alice.income.indexed_salary = inflate(alice.income.salary, IND_INCOME);
	}

	RUB gross = alice.income.indexed_salary; 
    
	RUB tax = ndfl_monthly(gross, alice.income_stats.main_job_received_year);

	RUB net = gross - tax;

	alice.cash += net;
    alice.income_stats.main_job_received_year += gross;
	alice.income_stats.main_job_ndfl_year += tax;
	
	stata.total_income += gross;
    shortstat_a.month_income += gross;
    shortstat_a.year_income += gross;

    stata.total_expences += tax;
    shortstat_a.month_expences += tax;
    shortstat_a.year_expences += tax;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_part_time_job(const int year, const int month)
{
	
	// Подработка появляется с февраля 2027
	if ((year == 2027 and month < 2) or (year < 2027)) return;

	if (year == 2027 and month == 2)
	{
		alice.income.part_time_job = 15'000;
		alice.income.indexed_part_time = alice.income.part_time_job;
	}

	if (year == 2030 and month == 4)
	{
		alice.income.part_time_job = random_range(20'000, 40'000);
		alice.income.indexed_part_time = alice.income.part_time_job;
	}

	if (month == 1 and year > 2027)
	{
    	alice.income.indexed_part_time = inflate(alice.income.part_time_job, IND_INCOME);
	}

	RUB gross = alice.income.indexed_part_time;

    RUB tax = ndfl_monthly(gross, alice.income_stats.part_time_received_year);
    RUB net = gross - tax;

	alice.cash += net;
	alice.income_stats.part_time_received_year += gross;
	alice.income_stats.part_time_ndfl_year += tax;

	stata.total_income += gross;
    shortstat_a.month_income += gross;
    shortstat_a.year_income += gross;

    stata.total_expences += tax;
    shortstat_a.month_expences += tax;
    shortstat_a.year_expences += tax;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//===РАСХОДЫ===//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//ALICE//
void alice_tax_audit(const int month)
{
	if (month != 12) return;

    // Годовой доход Alice — это сумма всех её доходов с начала года
    RUB taxable_gross = alice.income_stats.main_job_received_year
                  + alice.income_stats.part_time_received_year;
	RUB tax_due = ndfl_year(taxable_gross);

    RUB tax_paid = alice.income_stats.main_job_ndfl_year + alice.income_stats.part_time_ndfl_year;

    if (tax_due <= tax_paid) return;   // переплаты нет (или есть — не возвращаем)

    RUB extra = tax_due - tax_paid;

    pay_mandatory(alice, extra, "tax_audit");

    stata.total_expences += extra;
    shortstat_a.month_expences += extra;
    shortstat_a.year_expences += extra;
}

//Налоговый вычет логично было бы поставить после уплаты, собственно, налогов
void alice_tax_refund(const int month)
{
    if (month != 12) return;
    if (year < 2027) return;

    RUB total_base_left = alice.tax_refund.property_base_left
                        + alice.tax_refund.interest_base_left;
    if (total_base_left == 0) return;

    RUB year_income = alice.income_stats.main_job_received_year
                    + alice.income_stats.part_time_received_year;
    if (year_income == 0) return;

    RUB tax_without = ndfl_year(year_income);

    RUB apply = (total_base_left < year_income) ? total_base_left : year_income;

    RUB taxable = year_income - apply;
    RUB tax_with = ndfl_year(taxable);

    RUB refund = tax_without - tax_with;

    RUB use_property = (apply < alice.tax_refund.property_base_left)
                     ? apply
                     : alice.tax_refund.property_base_left;
    RUB use_interest = apply - use_property;

    alice.tax_refund.property_base_left -= use_property;
    alice.tax_refund.interest_base_left -= use_interest;
    alice.tax_refund.refunded_total     += refund;

    alice.cash += refund;

    stata.total_income       += refund;
    shortstat_a.month_income += refund;
    shortstat_a.year_income  += refund;
}
//Продолжаем расходы

void alice_food(const int year, const int month)
{
	RUB food_cost = alice.expences.food;

	// Каждый январь и каждый май еда дороже из-за праздников
	if (month == 1 or month == 5) {
		food_cost += 10'000;
	}
	food_cost *= alice.expences.lifestyle_coefficient;
	food_cost = inflate(food_cost, INF_FOOD);
	pay_mandatory(alice, food_cost, "food");

	stata.total_expences += (food_cost);
	shortstat_a.month_expences += (food_cost);
	shortstat_a.year_expences += (food_cost);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_residence(const int year, const int month)
{
	// Элис живёт с родителями до июня 2027, потом съезжает
	if(year == 2027 and month == 6) 
	{ 
	alice.lives_with_parents = false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_mortgage(const int year, const int month)
{
	if (alice.mortgage_completed == true) return;
	RUB local_mortgage = alice.expences.mortgage;
	// Ипотека появляется после переезда от родителей в июне 2027
	if ((year == 2027 and month < 6) or (year < 2027)) return;

	pay_mandatory(alice, local_mortgage, "mortgage");

	if (year == 2036 and month == 9)
	{
		alice.mortgage_completed = true;
	}
	stata.total_expences += local_mortgage;
	shortstat_a.month_expences += local_mortgage;
	shortstat_a.year_expences += local_mortgage;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_rent(const int year, const int month)
{
	RUB local_rent = alice.expences.rent;
	// Квартплата появляется после переезда от родителей в июне 2027
	if ((year == 2027 and month <= 6) or (year < 2027)) return;
	local_rent = inflate(local_rent, INF_UTILITIES);
	pay_mandatory(alice, local_rent, "rent");

	stata.total_expences += local_rent;
	shortstat_a.month_expences += local_rent;
	shortstat_a.year_expences += local_rent;
}

void alice_house_taxes(const int year, const int month)
{
	RUB local_home_taxes = 0;
	if (year == 2027 and month == 6) {
		alice.expences.home_taxes = ((60-20)*100'000*0.001); //Налог на квартиру
	}

	if (!(month == 12)) return;

	local_home_taxes = inflate(alice.expences.home_taxes, INF_UTILITIES);

	if (year == 2027)
	{
		local_home_taxes = local_home_taxes * 7 / 12;
	}

	pay_mandatory(alice, local_home_taxes, "home_taxes");
	stata.total_expences += local_home_taxes;
	shortstat_a.month_expences += local_home_taxes;
	shortstat_a.year_expences += local_home_taxes;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_dog(const int year, const int month)
{
	RUB dog_cost = 0;
	bool was_vet = false;
	// Собака появляется после переезда от родителей в июне 2027

	if ((year == 2027 and month < 6) or year < 2027) return;

	if ((alice.cash >= alice.bank.min_reserve) and (!alice.have_dog))
	{
		alice.have_dog = true;
		RUB first_vet = random_range(3'500, 8'000);
		first_vet = inflate(first_vet, INF_DOG);
		pay_mandatory(alice, first_vet, "vet_for_dog");

		stata.total_expences += first_vet;
		shortstat_a.month_expences += first_vet;
		shortstat_a.year_expences += first_vet;
	}

	if (!alice.have_dog) return;

	dog_cost += alice.expences.dog;
	dog_cost *= alice.expences.lifestyle_coefficient;
	dog_cost = inflate(dog_cost, INF_DOG);

	if (chance(5))
	{
		RUB extra_vet = random_range(8'000, 10'000);
		extra_vet = inflate(extra_vet, INF_DOG);
		dog_cost += extra_vet;
		was_vet = true;
	}

	if ((month == 6 or month == 12) and alice.have_dog and !was_vet)
	{
		RUB regular_vet = random_range(3'000, 5'000);
		regular_vet = inflate(regular_vet, INF_DOG);
		dog_cost += regular_vet;
	}
	
	pay_mandatory(alice, dog_cost, "dog");

	stata.total_expences += dog_cost;
	shortstat_a.month_expences += dog_cost;
	shortstat_a.year_expences += dog_cost;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_home_bills(const int year, const int month)
{
	RUB local_home_bills = alice.expences.home_bills;
	if ((year < 2027) or (year == 2027 and month < 6)) return;

	// Коммунальные платежи появляются после переезда от родителей в июне 2027

	local_home_bills = inflate(local_home_bills, INF_UTILITIES);
	pay_mandatory(alice, local_home_bills, "home_bills");

	stata.total_expences += (local_home_bills);
	shortstat_a.month_expences += (local_home_bills);
	shortstat_a.year_expences += (local_home_bills);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_bank_savings(const int year, const int month)
{
	if (alice.bank.creditka < 0) {
        RUB debt = (RUB)(-alice.bank.creditka);

        RUB available_cash = 0;
        if (alice.cash > alice.bank.min_reserve) {
            available_cash = alice.cash - alice.bank.min_reserve;
        }

        RUB to_pay = (debt < available_cash) ? debt : available_cash;

        alice.cash          -= to_pay;
        alice.bank.creditka += (SRUB)to_pay;

        if (alice.bank.creditka >= 0) {
            alice.bank.creditka      = 0;
            alice.bank.credit_months = 0;
            alice.bank.credit_active = false;
        }
        return;
    }

	if ((alice.cash < alice.bank.min_reserve) or (shortstat_a.month_profit < 0)) return;
	RUB deposit = alice.bank.base_depos;
	deposit += (shortstat_a.month_profit * 0.25);
	//Зачем держать больше налички?

	RUB available = alice.cash - alice.bank.min_reserve;
	if (deposit > available) 
	{
		deposit = available;
	}

	if (alice.cash > alice.bank.max_reserve) {   //surplus - излишек (англ)
        RUB surplus = alice.cash - alice.bank.max_reserve;
        if (surplus > deposit) deposit = surplus;
    }

	alice.bank.depos += deposit;
	alice.cash -= deposit;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_car_service(const int year, const int month)  //ФУНКЦИЯ БУДЕТ СИЛЬНО ДОПОЛНЕНА 4 Пунктами
{
	if (alice.car.car_type == NO_CAR) return;

	RUB fuel_and_consumables = alice.car.expences;
	fuel_and_consumables *= alice.expences.lifestyle_coefficient;
	fuel_and_consumables = inflate(fuel_and_consumables, INF_TRANSPORT);

	if (alice.car.breakdown.broken_months != 0)
	{
		fuel_and_consumables = 0; //Машина стоит сломанной, а вот налоги остаются
	}
	// Расходы на машину: бензин, страховка, ТО
	RUB total_car = alice.car.osago
              + alice.car.kasco
              + alice.car.nalog
              + fuel_and_consumables;
	pay_mandatory(alice, total_car, "car_service");

	stata.total_expences += (total_car);
	shortstat_a.month_expences += (total_car);
	shortstat_a.year_expences += (total_car);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_car_save()
{
	if (alice.car.car_type != NO_CAR) return;

	const RUB monthly_saving = 30'000;
	const RUB accelerated_saving = 50'000;
	const RUB min_cash_reserve = 50'000;

	// Ранний выход: нечего откладывать, если после взноса не останется подушки
	if (alice.cash < min_cash_reserve + monthly_saving) return;

	// Обычно 30к, при большом балансе — 50к
	RUB saving = (alice.cash >= 200'000 + monthly_saving) ? accelerated_saving : monthly_saving;

	// Не откладываем больше, чем остаётся сверх подушки
	RUB max_available = alice.cash - min_cash_reserve;
	saving = (saving < max_available) ? saving : max_available;

	alice.car_savings += saving;
	alice.cash -= saving;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_car_buying(const int year, const int month)
{
	if (alice.car.car_type != NO_CAR) return;

	RUB half_deposit = alice.bank.depos / 2;
	RUB available = alice.car_savings + half_deposit;

    const CarType target = SMALL_CAR;
    const Car& model = (target == SMALL_CAR) ? smallcar
    : (target == BIG_CAR)   ? bigcar
    : sportcar;   // Устанавлваем машину-цель, чтобы взять её ценник

	RUB local_model_price = model.price;
	RUB local_model_utilsbor = model.utilsbor;

	local_model_price = inflate(local_model_price, INF_TRANSPORT);
	local_model_utilsbor = inflate(local_model_utilsbor, INF_TRANSPORT);

	if (available < local_model_price + local_model_utilsbor) return;

	// Сколько берём из копилки и сколько добираем со вклада
	RUB from_car_savings = (alice.car_savings < local_model_price + local_model_utilsbor) ? alice.car_savings : (local_model_price + local_model_utilsbor);
	RUB from_bank = (local_model_price + local_model_utilsbor) - from_car_savings;

	alice.car_savings -= from_car_savings;
	alice.bank.depos -= from_bank;

	stata.total_expences += (local_model_price + local_model_utilsbor);
	shortstat_a.month_expences += (local_model_price + local_model_utilsbor);
	shortstat_a.year_expences += (local_model_price + local_model_utilsbor);

	alice.car = model; 
	alice.car.car_type = target;

	alice.bank.depos += alice.car_savings;    // Остаток копилки — на вклад
	alice.car_savings = 0;

}

void alice_car_break(const int year, const int month)
{
    if (alice.car.car_type == NO_CAR) return;
    
    // Если машина уже сломана — просто увеличиваем счётчик месяцев простоя
    if (alice.car.breakdown.broken_months > 0) {
        alice.car.breakdown.broken_months++;
        return;
    }
    // мелкий ремонт
    if (chance(15) and !alice.car.breakdown.electrics_minor) {
        alice.car.breakdown.electrics_minor = true;
        alice.car.breakdown.broken_months = 1;
    }
    
    if (chance(12) and !alice.car.breakdown.wipers_washer) {
        alice.car.breakdown.wipers_washer = true;
        alice.car.breakdown.broken_months = 1;
    }
    
    if (chance(10) and !alice.car.breakdown.oil_filter_early) {
        alice.car.breakdown.oil_filter_early = true;
        alice.car.breakdown.broken_months = 1;
    }
    // средний ремонт
    if (chance(6) and !alice.car.breakdown.battery) {
        alice.car.breakdown.battery = true;
        alice.car.breakdown.broken_months = 1;
    }
    
    if (chance(5) and !alice.car.breakdown.brakes) {
        alice.car.breakdown.brakes = true;
        alice.car.breakdown.broken_months = 1;
    }
    
    if (chance(4) and !alice.car.breakdown.suspension) {
        alice.car.breakdown.suspension = true;
        alice.car.breakdown.broken_months = 1;
    }
    
    if (chance(4) and !alice.car.breakdown.tires) {
        alice.car.breakdown.tires = true;
        alice.car.breakdown.broken_months = 1;
    }
    // серьёзный ремонт
    if (chance(2) and !alice.car.breakdown.electrics_major) {
        alice.car.breakdown.electrics_major = true;
        alice.car.breakdown.broken_months = 1;
    }
    
    if (chance(2) and !alice.car.breakdown.cooling_system) {
        alice.car.breakdown.cooling_system = true;
        alice.car.breakdown.broken_months = 1;
    }
    
    if (chance(1) and !alice.car.breakdown.gearbox_minor) {
        alice.car.breakdown.gearbox_minor = true;
        alice.car.breakdown.broken_months = 1;
    }
    
    if (chance(1) and !alice.car.breakdown.engine_minor) {
        alice.car.breakdown.engine_minor = true;
        alice.car.breakdown.broken_months = 1;
    }
    // авария или капиталка
    if (random_range(1, 1000) <= 3 and !alice.car.breakdown.engine_overhaul) {
        alice.car.breakdown.engine_overhaul = true;
        alice.car.breakdown.broken_months = 1;
    }
    
    if (random_range(1, 1000) <= 5 and !alice.car.breakdown.accident) {
        alice.car.breakdown.accident = true;
        alice.car.breakdown.broken_months = 1;
    }
}

void alice_calculate_repair_cost()
{
    RUB total = 0;
    
    // Мелкие поломки
    if (alice.car.breakdown.electrics_minor)   total += random_range(1'500, 5'000);
    if (alice.car.breakdown.wipers_washer)     total += random_range(1'000, 3'500);
    if (alice.car.breakdown.oil_filter_early)  total += random_range(4'000, 9'000);
    
    // Средние поломки
    if (alice.car.breakdown.battery)           total += random_range(6'000, 15'000);
    if (alice.car.breakdown.brakes)            total += random_range(8'000, 25'000);
    if (alice.car.breakdown.suspension)        total += random_range(15'000, 45'000);
    if (alice.car.breakdown.tires)             total += random_range(12'000, 40'000);
    
    // Серьёзные поломки
    if (alice.car.breakdown.electrics_major)   total += random_range(25'000, 70'000);
    if (alice.car.breakdown.cooling_system)    total += random_range(20'000, 60'000);
    if (alice.car.breakdown.gearbox_minor)     total += random_range(50'000, 150'000);
    if (alice.car.breakdown.engine_minor)      total += random_range(40'000, 120'000);
    
    // Катастрофические поломки
    if (alice.car.breakdown.engine_overhaul)   total += random_range(150'000, 400'000);
    if (alice.car.breakdown.accident)          total += random_range(80'000, 300'000);
    
    alice.car.repair_cost = total * alice.car.fix_coefficient;
}

void alice_car_repair(const int year, const int month)
{
    if (alice.car.car_type == NO_CAR) return;
    if (alice.car.breakdown.broken_months == 0) return;
    
    RUB repair_cost = alice.car.repair_cost;
	repair_cost = inflate(repair_cost, INF_TRANSPORT);
    
    // Пытаемся оплатить ремонт
    if ((alice.cash+0.5*alice.bank.depos) >= repair_cost) {
        pay_mandatory(alice, repair_cost, "car_repair");
        stata.total_expences += repair_cost;
        shortstat_a.month_expences += repair_cost;
        shortstat_a.year_expences += repair_cost;
        
        // Сбрасываем все поломки
        alice.car.breakdown.electrics_minor   = false;
        alice.car.breakdown.wipers_washer     = false;
        alice.car.breakdown.oil_filter_early  = false;
        alice.car.breakdown.battery           = false;
        alice.car.breakdown.brakes            = false;
        alice.car.breakdown.suspension        = false;
        alice.car.breakdown.tires             = false;
        alice.car.breakdown.electrics_major   = false;
        alice.car.breakdown.cooling_system    = false;
        alice.car.breakdown.gearbox_minor     = false;
        alice.car.breakdown.engine_minor      = false;
        alice.car.breakdown.engine_overhaul   = false;
        alice.car.breakdown.accident          = false;
        alice.car.breakdown.broken_months     = 0;
    }
    // Если денег не хватает — просто ждём, копим
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_car(const int year, const int month)
{
	alice_car_service(year, month);    
	alice_car_break(year, month);
	alice_calculate_repair_cost();
	alice_car_repair(year, month);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_credit_interest()
{
    if (alice.bank.creditka >= 0) return;   // долга нет

    alice.bank.credit_months++;

    if (alice.bank.credit_months <= 3) return;   // grace period

    RUB debt = (RUB)(-alice.bank.creditka);
    RUB interest = debt * 25 / (100 * 12);
    alice.bank.creditka -= (SRUB)interest;   // долг растёт

    stata.total_expences       += interest;
    shortstat_a.month_expences += interest;
    shortstat_a.year_expences  += interest;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_sick_event(const int year, const int month)
{
    if (alice.health == SICK) {

		RUB treatment = random_range(1'000, 5'000);
		treatment = inflate(treatment, INF_HEALTH);

		RUB strong_drugs_buying = 0;

        pay_mandatory(alice, treatment, "health");

        stata.total_expences += treatment;
        shortstat_a.month_expences += treatment;
        shortstat_a.year_expences += treatment;

        alice.sick_months_left--;

		if ((chance(10)) and (!alice.strong_drugs))  //с шансом 10% продолжит болеть ещё месяц
		{
			alice.sick_months_left +=1;
			alice.strong_drugs = true;
			strong_drugs_buying = 3'000;
		}

		strong_drugs_buying = inflate(strong_drugs_buying, INF_HEALTH);
		pay_mandatory(alice, strong_drugs_buying, "extra_health");

        stata.total_expences += strong_drugs_buying;
        shortstat_a.month_expences += strong_drugs_buying;
        shortstat_a.year_expences += strong_drugs_buying;

        if (alice.sick_months_left <= 0) {
            alice.health = HEALTHY;
			alice.strong_drugs = false;
        }
        return;   // пока болеет, новый кубик не бросаем
    }

    // Если здорова — 5% шанс заболеть
    if (chance(5)) 
	{
        alice.health = SICK;
		if (chance(25))
		{
			RUB first_visit = 8'000;
			first_visit = inflate(first_visit, INF_HEALTH);
			
        	pay_mandatory(alice, first_visit, "doctor");

			stata.total_expences += first_visit;
        	shortstat_a.month_expences += first_visit;
        	shortstat_a.year_expences += first_visit;
		}

        alice.sick_months_left = random_range(1, 2);
    }
}


void alice_misc_event(const int year, const int month)
{
    if ((chance(60) and alice.cash > alice.bank.min_reserve * 0.9)) 
	{
        RUB cost = random_range(150, 400);
		cost = inflate(cost, INF_FOOD);
		int coffe_times = random_range(1,30);
        pay_mandatory(alice, coffe_times*cost, "coffee");
        stata.total_expences += coffe_times*cost;
        shortstat_a.month_expences += coffe_times*cost;
        shortstat_a.year_expences += coffe_times*cost;
    }

    if (chance(45) and alice.cash > alice.bank.min_reserve * 0.9) 
	{
        RUB cost = random_range(300, 1'200);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(alice, cost, "subscriptions");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(50) and alice.cash > alice.bank.min_reserve * 0.8) 
	{
        RUB cost = random_range(80, 300);
		cost = inflate(cost, INF_FOOD);
		int snacks_times = random_range(1, 30);
        pay_mandatory(alice, snacks_times*cost, "snacks_home");
        stata.total_expences += snacks_times*cost;
        shortstat_a.month_expences += snacks_times*cost;
        shortstat_a.year_expences += snacks_times*cost;
    }

    if (chance(30) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(400, 1'500);
		int taxi_times = random_range(1, 30);
		cost = inflate(cost, INF_TRANSPORT);
        pay_mandatory(alice, taxi_times*cost, "taxi");
        stata.total_expences += taxi_times*cost;
        shortstat_a.month_expences += taxi_times*cost;
        shortstat_a.year_expences += taxi_times*cost;
    }

    if (chance(25) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(1'500, 4'500);
		cost = inflate(cost, INF_MISC);
        pay_mandatory(alice, cost, "hygiene_household");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(20) and alice.cash > alice.bank.min_reserve * 0.8) 
	{
        RUB cost = random_range(1'000, 5'000);
		int cafe_restaurant_times = random_range(1, 30);
		cost = inflate(cost, INF_FOOD);
        pay_mandatory(alice, cafe_restaurant_times*cost, "cafe_restaurant");
        stata.total_expences += cafe_restaurant_times*cost;
        shortstat_a.month_expences += cafe_restaurant_times*cost;
        shortstat_a.year_expences += cafe_restaurant_times*cost;
    }

    if (chance(15) and alice.cash > alice.bank.min_reserve * 0.9) 
	{
        RUB cost = random_range(1'500, 5'000);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(alice, cost, "entertainment");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(15) and alice.cash > alice.bank.min_reserve * 0.7) 
	{
        RUB cost = random_range(3'000, 9'000);
		cost = inflate(cost, INF_MISC);
        pay_mandatory(alice, cost, "cosmetics");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(12) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(1'500, 5'000);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(alice, cost, "books_games");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(8) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(2'000, 4'000);
		cost = inflate(cost, INF_HEALTH);
        pay_mandatory(alice, cost, "vitamins");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(10) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(1'000, 10'000);
		cost = inflate(cost, INF_CLOTHES);
        pay_mandatory(alice, cost, "clothes");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(5) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(7'000, 25'000);
		cost = inflate(cost, INF_CLOTHES);
        pay_mandatory(alice, cost, "shoes");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(5) and alice.cash > alice.bank.min_reserve * 0.9) 
	{
        RUB cost = random_range(4'000, 20'000);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(alice, cost, "gifts");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(3) and alice.cash > alice.bank.min_reserve * 0.8) 
	{
        RUB cost = random_range(10'000, 35'000);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(alice, cost, "gadgets_accessories");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
void alice_lifestyle_update()
{
    RUB capital = alice.cash + alice.bank.depos;
    SRUB year_profit = shortstat_a.year_profit;

    if (alice.bankruptcy.bancrupt) {
        alice.lyfestile = START_OF_LIFE;
        return;
    }

    if (alice.lyfestile == ELITE) {
        if (capital < 2'000'000 or year_profit < 0) {
            alice.lyfestile = BOURGEOISIE;
            return;
        }
    }
    else if (alice.lyfestile == BOURGEOISIE) {
        if (capital < 700'000 or year_profit < 0) {
            alice.lyfestile = MIDDLE_CLASS;
            return;
        }
    }
    else if (alice.lyfestile == MIDDLE_CLASS) {
        if (capital < 150'000 or year_profit < 0) {
            alice.lyfestile = START_OF_LIFE;
            return;
        }
    }

    if (alice.lyfestile == START_OF_LIFE) {
        if (capital > 400'000 and year_profit > 200'000) {
            alice.lyfestile = MIDDLE_CLASS;
        }
    }
    else if (alice.lyfestile == MIDDLE_CLASS) {
        if (capital > 2'000'000 and year_profit > 800'000) {
            alice.lyfestile = BOURGEOISIE;
        }
    }
    else if (alice.lyfestile == BOURGEOISIE) {
        if (capital > 5'000'000
            and year_profit > 2'000'000
            and (alice.car.car_type == BIG_CAR or alice.car.car_type == SPORT_CAR)) {
            alice.lyfestile = ELITE;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_statsupdate(const int year, const int month)
{

	shortstat_a.month_profit = shortstat_a.month_income - shortstat_a.month_expences;
	shortstat_a.year_profit = shortstat_a.year_income - shortstat_a.year_expences;
if (year == 2026 and month == 9) {
		stata.max_month_profit = shortstat_a.month_profit;
		stata.max_year_expences = shortstat_a.year_expences;
	} //Присвоим максимальный профит с начала отсчёта как первый профит, чтобы не обманываться нулём - ибо значение может оказаться отрицательным

	
	if (stata.max_month_expences < shortstat_a.month_expences)
	{
		stata.max_month_expences = shortstat_a.month_expences;
	}
	shortstat_a.month_expences = 0;

	if (stata.max_month_income < shortstat_a.month_income)
	{
		stata.max_month_income = shortstat_a.month_income;
	}
	shortstat_a.month_income = 0;

	if (stata.max_month_profit < shortstat_a.month_profit)
	{
		stata.max_month_profit = shortstat_a.month_profit;
	}

	if (stata.max_year_expences < shortstat_a.year_expences)
	{
		stata.max_year_expences = shortstat_a.year_expences;
	}

	if (stata.max_year_income < shortstat_a.year_income)
	{
		stata.max_year_income = shortstat_a.year_income;
	}

	if (stata.max_year_profit < shortstat_a.year_profit)
	{
		stata.max_year_profit = shortstat_a.year_profit;
	}

	if (month == 12)  //ОБНУЛЕНИЕ ГОДОВОЙ СТАТЫ РАЗ В ГОД
	{
		shortstat_a.year_expences = 0;
		shortstat_a.year_income = 0;
		shortstat_a.year_profit = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
//BOB//
void bob_food(const int year, const int month)
{
    RUB budget = inflate(bob.food_budget, INF_FOOD);
    budget = (RUB)(budget * bob.expences.lifestyle_coefficient);

    bob_build_food_plan(budget);

    RUB spent = bob.expences.food;
    pay_mandatory(bob, spent, "food");

    // 4. Статистика
    statb.total_expences       += spent;
    shortstat_b.month_expences += spent;
    shortstat_b.year_expences  += spent;
}


void bob_ndfl_update()
{
	if (month == 1) 
	{
    bob.income_stats.main_job_ndfl_year = 0;
	bob.income_stats.part_time_ndfl_year = 0;
	bob.income_stats.main_job_received_year = 0;
	bob.income_stats.part_time_received_year = 0;
	}
}

void bob_lifestyle_bank_savings_upd()
{
	if (bob.lyfestile == START_OF_LIFE) 
	{
		bob.bank.min_reserve = 25'000;
    	bob.bank.max_reserve = 160'000;
    	bob.bank.base_depos = 7'000;
	}
	else if (bob.lyfestile == MIDDLE_CLASS)
	{
		bob.bank.min_reserve = 45'000;
		bob.bank.max_reserve = 210'000;
		bob.bank.base_depos = 10'000;
	}
	else if (bob.lyfestile == BOURGEOISIE)
	{
		bob.bank.min_reserve = 60'000;
		bob.bank.max_reserve = 250'000;
		bob.bank.base_depos = 10'000;
	}
	else if (bob.lyfestile == ELITE)
	{
		bob.bank.min_reserve = 75'000;
		bob.bank.max_reserve = 300'000;
		bob.bank.base_depos = 15'000;
	}
}


void bob_credit_interest()
{
    if (bob.bank.creditka >= 0) return;   // долга нет

    bob.bank.credit_months++;

    if (bob.bank.credit_months <= 3) return;   // grace period (беспроцентный)

    RUB debt = (RUB)(-bob.bank.creditka);
    RUB interest = debt * 25 / (100 * 12);
    bob.bank.creditka -= (SRUB)interest;   // долг растёт

    statb.total_expences       += interest;
    shortstat_b.month_expences += interest;
    shortstat_b.year_expences  += interest;
}

void bob_bank_income(const int year, const int month)
{
	// Начисление 13% годовых, ежемесячно (сложный процент) === 14 / (100 * 12)
	RUB bank_income = bob.bank.depos * 13 / (100 * 12);
	bob.bank.depos += bank_income;
	// Отдельным вкладом лежат накопления на машину, чтобы по ним тоже капал процент
	RUB car_savings_income = bob.car_savings * 13 / (100 * 12);
	bob.car_savings += car_savings_income;

	statb.total_income += (bank_income + car_savings_income);
	shortstat_b.month_income += (bank_income + car_savings_income);
	shortstat_b.year_income += (bank_income + car_savings_income);
}

void bob_salary(const int year, const int month)
{

	if (year == 2029 and month == 3) {
		bob.income.salary = 160'000;
		bob.income.indexed_salary = bob.income.salary;
	}

	if (month == 1 and year > 2029)  //ушел туда где зп индексируют 
	{
	bob.income.indexed_salary = inflate(bob.income.salary, IND_INCOME);
	}

	RUB gross = bob.income.indexed_salary; 
    
	RUB tax = ndfl_monthly(gross, bob.income_stats.main_job_received_year);

	RUB net = gross - tax;

	bob.cash += net;
    bob.income_stats.main_job_received_year += gross;
	bob.income_stats.main_job_ndfl_year += tax;
	
	statb.total_income += gross;
    shortstat_b.month_income += gross;
    shortstat_b.year_income += gross;

    statb.total_expences += tax;
    shortstat_b.month_expences += tax;
    shortstat_b.year_expences += tax;
}

void bob_part_time_job(const int year, const int month)
{
	
	// Подработка появляется с февраля 2028
	if ((year == 2028 and month < 2) or (year < 2028)) return;

	if (year == 2028 and month == 2)
	{
		bob.income.part_time_job = random_range(20'000, 40'000);
		bob.income.indexed_part_time = bob.income.part_time_job;
	}

	if (month == 1 and year > 2028)
	{
    	bob.income.indexed_part_time = inflate(bob.income.part_time_job, IND_INCOME);
	}

	RUB gross = bob.income.indexed_part_time;

    RUB tax = ndfl_monthly(gross, bob.income_stats.part_time_received_year);
    RUB net = gross - tax;

	bob.cash += net;
	bob.income_stats.part_time_received_year += gross;
	bob.income_stats.part_time_ndfl_year += tax;

	statb.total_income += gross;
    shortstat_b.month_income += gross;
    shortstat_b.year_income += gross;

    statb.total_expences += tax;
    shortstat_b.month_expences += tax;
    shortstat_b.year_expences += tax;
}

void bob_tax_audit(const int month)
{
	if (month != 12) return;

    // Годовой доход bob — это сумма всех её доходов с начала года
    RUB taxable_gross = bob.income_stats.main_job_received_year
                  + bob.income_stats.part_time_received_year;
	RUB tax_due = ndfl_year(taxable_gross);

    RUB tax_paid = bob.income_stats.main_job_ndfl_year + bob.income_stats.part_time_ndfl_year;

    if (tax_due <= tax_paid) return;   // переплаты нет (или есть — не возвращаем)

    RUB extra = tax_due - tax_paid;

    pay_mandatory(bob, extra, "tax_audit");

    statb.total_expences += extra;
    shortstat_b.month_expences += extra;
    shortstat_b.year_expences += extra;
}


void bob_tax_refund(const int month)
{
    if (month != 12) return;
    if (year < 2027) return;

    RUB total_base_left = bob.tax_refund.property_base_left
                        + bob.tax_refund.interest_base_left;
    if (total_base_left == 0) return;

    RUB year_income = bob.income_stats.main_job_received_year
                    + bob.income_stats.part_time_received_year;
    if (year_income == 0) return;

    RUB tax_without = ndfl_year(year_income);

    RUB apply = (total_base_left < year_income) ? total_base_left : year_income;

    RUB taxable = year_income - apply;
    RUB tax_with = ndfl_year(taxable);

    RUB refund = tax_without - tax_with;

    RUB use_property = (apply < bob.tax_refund.property_base_left)
                     ? apply
                     : bob.tax_refund.property_base_left;
    RUB use_interest = apply - use_property;

    bob.tax_refund.property_base_left -= use_property;
    bob.tax_refund.interest_base_left -= use_interest;
    bob.tax_refund.refunded_total     += refund;

    bob.cash += refund;

    stata.total_income       += refund;
    shortstat_a.month_income += refund;
    shortstat_a.year_income  += refund;
}


void bob_house_taxes(const int year, const int month)
{
	RUB local_home_taxes = 0;
	if (year == 2027 and month == 6) {
		bob.expences.home_taxes = ((75-20)*100'000*0.001); //Налог на квартиру
	}

	if (!(month == 12)) return;

	local_home_taxes = inflate(bob.expences.home_taxes, INF_UTILITIES);


	pay_mandatory(bob, local_home_taxes, "home_taxes");
	statb.total_expences += local_home_taxes;
	shortstat_b.month_expences += local_home_taxes;
	shortstat_b.year_expences += local_home_taxes;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
void bob_mortgage(const int year, const int month)
{
	if (bob.mortgage_completed == true) return;
	RUB local_mortgage = bob.expences.mortgage;
	// Ипотека появляется когда берёт вторую квартиру в январе 2027
	if (year < 2027) return;

	pay_mandatory(bob, local_mortgage, "mortgage");

	if (year == 2035 and month == 9)
	{
		bob.mortgage_completed = true;
	}
	statb.total_expences += local_mortgage;
	shortstat_b.month_expences += local_mortgage;
	shortstat_b.year_expences += local_mortgage;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void bob_rent(const int year, const int month)
{
	RUB local_rent = bob.expences.rent;
	// Квартплата появляется после переезда от родителей в июне 2027
	if ((year == 2027 and month <= 6) or (year < 2027)) return;
	local_rent = inflate(local_rent, INF_UTILITIES);
	pay_mandatory(bob, local_rent, "rent");

	statb.total_expences += local_rent;
	shortstat_b.month_expences += local_rent;
	shortstat_b.year_expences += local_rent;
}

void bob_home_bills(const int year, const int month)
{
	RUB local_home_bills = bob.expences.home_bills;
	if ((year < 2027) or (year == 2027 and month < 6)) return;

	// Коммунальные платежи появляются после переезда от родителей в июне 2027

	local_home_bills = inflate(local_home_bills, INF_UTILITIES);
	pay_mandatory(bob, local_home_bills, "home_bills");

	statb.total_expences += (local_home_bills);
	shortstat_b.month_expences += (local_home_bills);
	shortstat_b.year_expences += (local_home_bills);
}


void bob_car_service(const int year, const int month)  //ФУНКЦИЯ БУДЕТ СИЛЬНО ДОПОЛНЕНА 4 Пунктами
{
	if (bob.car.car_type == NO_CAR) return;

	RUB fuel_and_consumables = bob.car.expences;
	fuel_and_consumables *= bob.expences.lifestyle_coefficient;
	fuel_and_consumables = inflate(fuel_and_consumables, INF_TRANSPORT);

	if (bob.car.breakdown.broken_months != 0)
	{
		fuel_and_consumables = 0; //Машина стоит сломанной, а вот налоги остаются
	}
	// Расходы на машину: бензин, страховка, ТО
	RUB total_car = bob.car.osago
              + bob.car.kasco
              + bob.car.nalog
              + fuel_and_consumables;
	pay_mandatory(bob, total_car, "car_service");

	statb.total_expences += (total_car);
	shortstat_b.month_expences += (total_car);
	shortstat_b.year_expences += (total_car);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void bob_car_save()
{
	if (bob.car.car_type != NO_CAR) return;

	const RUB monthly_saving = 30'000;
	const RUB accelerated_saving = 50'000;
	const RUB min_cash_reserve = 50'000;

	// Ранний выход: нечего откладывать, если после взноса не останется подушки
	if (bob.cash < min_cash_reserve + monthly_saving) return;

	// Обычно 30к, при большом балансе — 50к
	RUB saving = (bob.cash >= 200'000 + monthly_saving) ? accelerated_saving : monthly_saving;

	// Не откладываем больше, чем остаётся сверх подушки
	RUB max_available = bob.cash - min_cash_reserve;
	saving = (saving < max_available) ? saving : max_available;

	bob.car_savings += saving;
	bob.cash -= saving;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void bob_car_buying(const int year, const int month)
{
	if (bob.car.car_type != NO_CAR) return;

	RUB half_deposit = bob.bank.depos / 2;
	RUB available = bob.car_savings + half_deposit;

    const CarType target = SMALL_CAR;
    const Car& model = (target == SMALL_CAR) ? smallcar
    : (target == BIG_CAR)   ? bigcar
    : sportcar;   // Устанавлваем машину-цель, чтобы взять её ценник

	RUB local_model_price = model.price;
	RUB local_model_utilsbor = model.utilsbor;

	local_model_price = inflate(local_model_price, INF_TRANSPORT);
	local_model_utilsbor = inflate(local_model_utilsbor, INF_TRANSPORT);

	if (available < local_model_price + local_model_utilsbor) return;

	// Сколько берём из копилки и сколько добираем со вклада
	RUB from_car_savings = (bob.car_savings < local_model_price + local_model_utilsbor) ? bob.car_savings : (local_model_price + local_model_utilsbor);
	RUB from_bank = (local_model_price + local_model_utilsbor) - from_car_savings;

	bob.car_savings -= from_car_savings;
	bob.bank.depos -= from_bank;

	statb.total_expences += (local_model_price + local_model_utilsbor);
	shortstat_b.month_expences += (local_model_price + local_model_utilsbor);
	shortstat_b.year_expences += (local_model_price + local_model_utilsbor);

	bob.car = model; 
	bob.car.car_type = target;

	bob.bank.depos += bob.car_savings;    // Остаток копилки — на вклад
	bob.car_savings = 0;

}

void bob_car_break(const int year, const int month)
{
    if (bob.car.car_type == NO_CAR) return;
    
    // Если машина уже сломана — просто увеличиваем счётчик месяцев простоя
    if (bob.car.breakdown.broken_months > 0) {
        bob.car.breakdown.broken_months++;
        return;
    }
    // мелкий ремонт
    if (chance(14) and !bob.car.breakdown.electrics_minor) {
        bob.car.breakdown.electrics_minor = true;
        bob.car.breakdown.broken_months = 1;
    }
    
    if (chance(11) and !bob.car.breakdown.wipers_washer) {
        bob.car.breakdown.wipers_washer = true;
        bob.car.breakdown.broken_months = 1;
    }
    
    if (chance(9) and !bob.car.breakdown.oil_filter_early) {
        bob.car.breakdown.oil_filter_early = true;
        bob.car.breakdown.broken_months = 1;
    }
    // средний ремонт
    if (chance(5) and !bob.car.breakdown.battery) {
        bob.car.breakdown.battery = true;
        bob.car.breakdown.broken_months = 1;
    }
    
    if (chance(4) and !bob.car.breakdown.brakes) {
        bob.car.breakdown.brakes = true;
        bob.car.breakdown.broken_months = 1;
    }
    
    if (chance(3) and !bob.car.breakdown.suspension) {
        bob.car.breakdown.suspension = true;
        bob.car.breakdown.broken_months = 1;
    }
    
    if (chance(3) and !bob.car.breakdown.tires) {
        bob.car.breakdown.tires = true;
        bob.car.breakdown.broken_months = 1;
    }
    // серьёзный ремонт
    if (chance(1) and !bob.car.breakdown.electrics_major) {
        bob.car.breakdown.electrics_major = true;
        bob.car.breakdown.broken_months = 1;
    }
    
    if (chance(1) and !bob.car.breakdown.cooling_system) {
        bob.car.breakdown.cooling_system = true;
        bob.car.breakdown.broken_months = 1;
    }
    
    if (chance(1) and !bob.car.breakdown.gearbox_minor) {
        bob.car.breakdown.gearbox_minor = true;
        bob.car.breakdown.broken_months = 1;
    }
    
    if (chance(1) and !bob.car.breakdown.engine_minor) {
        bob.car.breakdown.engine_minor = true;
        bob.car.breakdown.broken_months = 1;
    }
    // авария или капиталка
    if (random_range(1, 1000) <= 2 and !bob.car.breakdown.engine_overhaul) {
        bob.car.breakdown.engine_overhaul = true;
        bob.car.breakdown.broken_months = 1;
    }
    
    if (random_range(1, 1000) <= 4 and !bob.car.breakdown.accident) {
        bob.car.breakdown.accident = true;
        bob.car.breakdown.broken_months = 1;
    }
}

void bob_calculate_repair_cost()
{
    RUB total = 0;
    
    // Мелкие поломки
    if (bob.car.breakdown.electrics_minor)   total += random_range(1'500, 5'000);
    if (bob.car.breakdown.wipers_washer)     total += random_range(1'000, 3'500);
    if (bob.car.breakdown.oil_filter_early)  total += random_range(4'000, 9'000);
    
    // Средние поломки
    if (bob.car.breakdown.battery)           total += random_range(6'000, 15'000);
    if (bob.car.breakdown.brakes)            total += random_range(8'000, 25'000);
    if (bob.car.breakdown.suspension)        total += random_range(15'000, 45'000);
    if (bob.car.breakdown.tires)             total += random_range(12'000, 40'000);
    
    // Серьёзные поломки
    if (bob.car.breakdown.electrics_major)   total += random_range(25'000, 70'000);
    if (bob.car.breakdown.cooling_system)    total += random_range(20'000, 60'000);
    if (bob.car.breakdown.gearbox_minor)     total += random_range(50'000, 150'000);
    if (bob.car.breakdown.engine_minor)      total += random_range(40'000, 120'000);
    
    // Катастрофические поломки
    if (bob.car.breakdown.engine_overhaul)   total += random_range(150'000, 400'000);
    if (bob.car.breakdown.accident)          total += random_range(80'000, 300'000);
    
    bob.car.repair_cost = total * bob.car.fix_coefficient;
}

void bob_car_repair(const int year, const int month)
{
    if (bob.car.car_type == NO_CAR) return;
    if (bob.car.breakdown.broken_months == 0) return;
    
    RUB repair_cost = bob.car.repair_cost;
	repair_cost = inflate(repair_cost, INF_TRANSPORT);
    
    // Пытаемся оплатить ремонт
    if ((bob.cash+0.5*bob.bank.depos) >= repair_cost) {
        pay_mandatory(bob, repair_cost, "car_repair");
        statb.total_expences += repair_cost;
        shortstat_b.month_expences += repair_cost;
        shortstat_b.year_expences += repair_cost;
        
        // Сбрасываем все поломки
        bob.car.breakdown.electrics_minor   = false;
        bob.car.breakdown.wipers_washer     = false;
        bob.car.breakdown.oil_filter_early  = false;
        bob.car.breakdown.battery           = false;
        bob.car.breakdown.brakes            = false;
        bob.car.breakdown.suspension        = false;
        bob.car.breakdown.tires             = false;
        bob.car.breakdown.electrics_major   = false;
        bob.car.breakdown.cooling_system    = false;
        bob.car.breakdown.gearbox_minor     = false;
        bob.car.breakdown.engine_minor      = false;
        bob.car.breakdown.engine_overhaul   = false;
        bob.car.breakdown.accident          = false;
        bob.car.breakdown.broken_months     = 0;
    }
    // Если денег не хватает — просто ждём, копим
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void bob_car(const int year, const int month)
{
	bob_car_service(year, month);    
	bob_car_break(year, month);
	bob_calculate_repair_cost();
	bob_car_repair(year, month);
}

void bob_bank_savings(const int year, const int month)
{
	if (bob.bank.creditka < 0) {
        RUB debt = (RUB)(-bob.bank.creditka);

        RUB available_cash = 0;
        if (bob.cash > bob.bank.min_reserve) {
            available_cash = bob.cash - bob.bank.min_reserve;
        }

        RUB to_pay = (debt < available_cash) ? debt : available_cash;

        bob.cash          -= to_pay;
        bob.bank.creditka += (SRUB)to_pay;

        if (bob.bank.creditka >= 0) {
            bob.bank.creditka      = 0;
            bob.bank.credit_months = 0;
            bob.bank.credit_active = false;
        }
        return;
    }

	if ((bob.cash < bob.bank.min_reserve) or (shortstat_b.month_profit < 0)) return;
	RUB deposit = bob.bank.base_depos;
	deposit += (shortstat_b.month_profit * 0.25);
	//Зачем держать больше налички?

	RUB available = bob.cash - bob.bank.min_reserve;
	if (deposit > available) 
	{
		deposit = available;
	}

	if (bob.cash > bob.bank.max_reserve) {   //surplus - излишек (англ)
        RUB surplus = bob.cash - bob.bank.max_reserve;
        if (surplus > deposit) deposit = surplus;
    }

	bob.bank.depos += deposit;
	bob.cash -= deposit;
}

void bob_sick_event(const int year, const int month)
{
    if (bob.health == SICK) {

		RUB treatment = random_range(1'000, 5'000);
		treatment = inflate(treatment, INF_HEALTH);

		RUB strong_drugs_buying = 0;

        pay_mandatory(bob, treatment, "health");

        statb.total_expences += treatment;
        shortstat_b.month_expences += treatment;
        shortstat_b.year_expences += treatment;

        bob.sick_months_left--;

		if ((chance(10)) and (!bob.strong_drugs))  //с шансом 10% продолжит болеть ещё месяц
		{
			bob.sick_months_left +=1;
			bob.strong_drugs = true;
			strong_drugs_buying = 3'200;
		}

		strong_drugs_buying = inflate(strong_drugs_buying, INF_HEALTH);
		pay_mandatory(bob, strong_drugs_buying, "extra_health");

        statb.total_expences += strong_drugs_buying;
        shortstat_b.month_expences += strong_drugs_buying;
        shortstat_b.year_expences += strong_drugs_buying;

        if (bob.sick_months_left <= 0) {
            bob.health = HEALTHY;
			bob.strong_drugs = false;
        }
        return;   // пока болеет, новый кубик не бросаем
    }

    // Если здоров — 7% шанс заболеть
    if (chance(7)) 
	{
        bob.health = SICK;
		if (chance(25))
		{
			RUB first_visit = 8'000;
			first_visit = inflate(first_visit, INF_HEALTH);
			
        	pay_mandatory(bob, first_visit, "doctor");

			statb.total_expences += first_visit;
        	shortstat_b.month_expences += first_visit;
        	shortstat_b.year_expences += first_visit;
		}

        bob.sick_months_left = random_range(1, 2);
    }
}


void bob_misc_event(const int year, const int month)
{
    if ((chance(20) and bob.cash > bob.bank.min_reserve * 0.9)) 
	{
        RUB cost = random_range(150, 400);
		cost = inflate(cost, INF_FOOD);
		int coffe_times = random_range(1,30);
        pay_mandatory(bob, coffe_times*cost, "coffee");
        statb.total_expences += coffe_times*cost;
        shortstat_b.month_expences += coffe_times*cost;
        shortstat_b.year_expences += coffe_times*cost;
    }

    if (chance(30) and bob.cash > bob.bank.min_reserve * 0.9) 
	{
        RUB cost = random_range(300, 1'200);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(bob, cost, "subscriptions");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }

    if (chance(50) and bob.cash > bob.bank.min_reserve * 0.8) 
	{
        RUB cost = random_range(80, 300);
		cost = inflate(cost, INF_FOOD);
		int snacks_times = random_range(1, 30);
        pay_mandatory(bob, snacks_times*cost, "snacks_home");
        statb.total_expences += snacks_times*cost;
        shortstat_b.month_expences += snacks_times*cost;
        shortstat_b.year_expences += snacks_times*cost;
    }

    if (chance(15) and bob.cash > bob.bank.min_reserve) 
	{
        RUB cost = random_range(400, 1'500);
		int taxi_times = random_range(1, 30);
		cost = inflate(cost, INF_TRANSPORT);
        pay_mandatory(bob, taxi_times*cost, "taxi");
        statb.total_expences += taxi_times*cost;
        shortstat_b.month_expences += taxi_times*cost;
        shortstat_b.year_expences += taxi_times*cost;
    }

    if (chance(30) and bob.cash > bob.bank.min_reserve) 
	{
        RUB cost = random_range(1'500, 4'500);
		cost = inflate(cost, INF_MISC);
        pay_mandatory(bob, cost, "hygiene_household");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }

    if (chance(5) and bob.cash > bob.bank.min_reserve * 0.8) 
	{
        RUB cost = random_range(1'000, 5'000);
		cost = inflate(cost, INF_FOOD);
        pay_mandatory(bob, cost, "cafe_restaurant");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }

    if (chance(10) and bob.cash > bob.bank.min_reserve * 0.9) 
	{
        RUB cost = random_range(1'500, 5'000);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(bob, cost, "entertainment");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }

    if (chance(35) and bob.cash > bob.bank.min_reserve) 
	{
        RUB cost = random_range(1'500, 5'000);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(bob, cost, "books_games");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }

    if (chance(15) and bob.cash > bob.bank.min_reserve) 
	{
        RUB cost = random_range(2'000, 4'000);
		cost = inflate(cost, INF_HEALTH);
        pay_mandatory(bob, cost, "vitamins");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }

    if (chance(15) and bob.cash > bob.bank.min_reserve) 
	{
        RUB cost = random_range(1'000, 10'000);
		cost = inflate(cost, INF_CLOTHES);
        pay_mandatory(bob, cost, "clothes");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }

    if (chance(8) and bob.cash > bob.bank.min_reserve) 
	{
        RUB cost = random_range(7'000, 25'000);
		cost = inflate(cost, INF_CLOTHES);
        pay_mandatory(bob, cost, "shoes");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }

    if (chance(9) and bob.cash > bob.bank.min_reserve * 0.9) 
	{
        RUB cost = random_range(4'000, 20'000);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(bob, cost, "gifts");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }

    if (chance(5) and bob.cash > bob.bank.min_reserve * 0.8) 
	{
        RUB cost = random_range(10'000, 35'000);
		cost = inflate(cost, INF_ENTERTAINMENT);
        pay_mandatory(bob, cost, "gadgets_accessories");
        statb.total_expences += cost;
        shortstat_b.month_expences += cost;
        shortstat_b.year_expences += cost;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
void bob_lifestyle_update()
{
    RUB capital = bob.cash + bob.bank.depos;
    SRUB year_profit = shortstat_b.year_profit;

    if (bob.bankruptcy.bancrupt) {
        bob.lyfestile = START_OF_LIFE;
        return;
    }

    if (bob.lyfestile == ELITE) {
        if (capital < 2'000'000 or year_profit < 0) {
            bob.lyfestile = BOURGEOISIE;
            return;
        }
    }
    else if (bob.lyfestile == BOURGEOISIE) {
        if (capital < 700'000 or year_profit < 0) {
            bob.lyfestile = MIDDLE_CLASS;
            return;
        }
    }
    else if (bob.lyfestile == MIDDLE_CLASS) {
        if (capital < 150'000 or year_profit < 0) {
            bob.lyfestile = START_OF_LIFE;
            return;
        }
    }

    if (bob.lyfestile == START_OF_LIFE) {
        if (capital > 400'000 and year_profit > 200'000) {
            bob.lyfestile = MIDDLE_CLASS;
        }
    }
    else if (bob.lyfestile == MIDDLE_CLASS) {
        if (capital > 2'000'000 and year_profit > 800'000) {
            bob.lyfestile = BOURGEOISIE;
        }
    }
    else if (bob.lyfestile == BOURGEOISIE) {
        if (capital > 5'000'000
            and year_profit > 2'000'000
            and (bob.car.car_type == BIG_CAR or bob.car.car_type == SPORT_CAR)) {
            bob.lyfestile = ELITE;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void bob_statsupdate(const int year, const int month)
{

	shortstat_b.month_profit = shortstat_b.month_income - shortstat_b.month_expences;
	shortstat_b.year_profit = shortstat_b.year_income - shortstat_b.year_expences;
if (year == 2026 and month == 9) {
		statb.max_month_profit = shortstat_b.month_profit;
		statb.max_year_expences = shortstat_b.year_expences;
	} //Присвоим максимальный профит с начала отсчёта как первый профит, чтобы не обманываться нулём - ибо значение может оказаться отрицательным

	
	if (statb.max_month_expences < shortstat_b.month_expences)
	{
		statb.max_month_expences = shortstat_b.month_expences;
	}
	shortstat_b.month_expences = 0;

	if (statb.max_month_income < shortstat_b.month_income)
	{
		statb.max_month_income = shortstat_b.month_income;
	}
	shortstat_b.month_income = 0;

	if (statb.max_month_profit < shortstat_b.month_profit)
	{
		statb.max_month_profit = shortstat_b.month_profit;
	}

	if (statb.max_year_expences < shortstat_b.year_expences)
	{
		statb.max_year_expences = shortstat_b.year_expences;
	}

	if (statb.max_year_income < shortstat_b.year_income)
	{
		statb.max_year_income = shortstat_b.year_income;
	}

	if (statb.max_year_profit < shortstat_b.year_profit)
	{
		statb.max_year_profit = shortstat_b.year_profit;
	}

	if (month == 12)  //ОБНУЛЕНИЕ ГОДОВОЙ СТАТЫ РАЗ В ГОД
	{
		shortstat_b.year_expences = 0;
		shortstat_b.year_income = 0;
		shortstat_b.year_profit = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void alice_simulation()
{
	printf("===ALICE===\n");
	printf("year %u\n", year);
	printf("month %u\n", month);
	printf("Cash:        %llu rub.\n", alice.cash);
	printf("Deposit:     %llu rub.\n", alice.bank.depos);

	alice_ndfl_update();

	alice_lifestyle_expences_coefficient_upd();

	alice_lifestyle_bank_savings_upd();

	alice_credit_interest();           // Начисление по кредитке, если не погасила

	alice_bank_income(year, month);    // Доходы
	alice_salary(year, month);
	alice_part_time_job(year, month);    

	alice_tax_audit(month);// Налоги
	alice_tax_refund(month);
	alice_house_taxes(year, month);    

	alice_residence(year, month);      // Расходы
	alice_mortgage(year, month);         
	alice_rent(year, month);
	alice_food(year, month);
	alice_dog(year, month);
	alice_home_bills(year, month);
	alice_car(year, month);            // большая функция с рандомом

	alice_bank_savings(year, month);   // отчисление на вклад
	alice_car_save();                  // отчисления на машину
	alice_car_buying(year, month);     // покупка машины

	alice_sick_event(year, month);     // случайные события
	alice_misc_event(year, month);

	alice_lifestyle_update();

	printf("month expences %llu\n", shortstat_a.month_expences);

	alice_statsupdate(year, month);

	if ((alice.cash > 100'000'000'000))
	{
		printf("NEGATIVE BALANCE!!!\n");
		printf("year %u\n", year);
		printf("month %u\n", month);
	}

	if ((alice.bank.depos > 100'000'000'000))
	{
		printf("NEGATIVE BALANCE!!!\n");
		printf("year %u\n", year);
		printf("month %u\n", month);
	}

}


void bob_simulation()
{
	printf("===BOB===\n");
	printf("year %u\n", year);
	printf("month %u\n", month);
	printf("Cash:        %llu rub.\n", bob.cash);
	printf("Deposit:     %llu rub.\n", bob.bank.depos);

	bob_ndfl_update();

	bob_lifestyle_expences_coefficient_upd();

	bob_lifestyle_bank_savings_upd();

	bob_credit_interest();           // Начисление по кредитке, если не погасила

	bob_bank_income(year, month);    // Доходы
	bob_salary(year, month);
	bob_part_time_job(year, month);    

	bob_tax_audit(month);// Налоги
	bob_tax_refund(month);
	bob_house_taxes(year, month);    

	bob_mortgage(year, month);       // Расходы   
	bob_rent(year, month);
	bob_food(year, month);
	bob_home_bills(year, month);
	bob_car(year, month);            // большая функция с рандомом

	bob_bank_savings(year, month);   // отчисление на вклад
	bob_car_save();                  // отчисления на машину
	bob_car_buying(year, month);     // покупка машины

	bob_sick_event(year, month);     // случайные события
	bob_misc_event(year, month);

	bob_lifestyle_update();

	printf("month expences %llu\n", shortstat_b.month_expences);

	bob_statsupdate(year, month);

	if ((bob.cash > 100'000'000'000))
	{
		printf("NEGATIVE BALANCE!!!\n");
		printf("year %u\n", year);
		printf("month %u\n", month);
	}

	if ((bob.bank.depos > 100'000'000'000))
	{
		printf("NEGATIVE BALANCE!!!\n");
		printf("year %u\n", year);
		printf("month %u\n", month);
	}

}


void all_simulation()
{
	
	while (not (year == 2036 and month == 9)) 
	{

		inflation_update();
		alice_simulation();
		bob_simulation();

		++month;
		if (month == 13) {
			++year;
			month = 1;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
void alice_print_bankruptcy() {
	printf("=== Alice's bancruptcy report ===\n");
    if (not alice.bankruptcy.bancrupt) {
        printf(" no bankruptcy\n");
        return;
    }
    printf(" went bankrupt in %d-%02d\n",
           alice.bankruptcy.year, alice.bankruptcy.month);
    printf(" cause : %s\n", alice.bankruptcy.reason);
    printf("  was: %llu rub short.\n", alice.bankruptcy.deficit);
}


void alice_print()
{
	printf("=== Alice's financial report ===\n");
	printf("Cash:        %llu rub.\n", alice.cash);
	printf("Bank account: %llu rub.\n", alice.bank.depos);

	printf("Property:\n");

	switch (alice.car.car_type) {
    	case NO_CAR:      printf("No car\n");          break;
    	case SMALL_CAR:   printf("Small car\n");       break;
    	case BIG_CAR:     printf("Big car\n");         break;
    	case SPORT_CAR:   printf("Sport car\n");       break;
	}

	printf("Residence:\n");

	if (alice.lives_with_parents) {
		printf("lives with parents\n");
	}
	else {
		printf("lives in her own apartment\n");
	}

	printf("Lifestyle: ");
	switch (alice.lyfestile) {
    	case START_OF_LIFE: printf("Start of life\n"); break;
    	case MIDDLE_CLASS:  printf("Middle class\n");  break;
    	case BOURGEOISIE:   printf("Bourgeoisie\n");   break;
    	case ELITE:         printf("Elite\n");         break;
	}
}


void alice_print_tax_refund()
{
    printf("=== Alice's tax refund report ===\n");
    printf("  Property base left: %llu rub.\n",
           alice.tax_refund.property_base_left);
    printf("  Interest base left: %llu rub.\n",
           alice.tax_refund.interest_base_left);
    printf("  Total refunded:     %llu rub.\n",
           alice.tax_refund.refunded_total);
    printf("  Effective rate:     %.2f%%\n",
           alice.tax_refund.refunded_total > 0
           ? 100.0 * alice.tax_refund.refunded_total / 5'000'000.0
           : 0.0);
}


void alice_detailed()
{
	printf("=== Alice's detailed stats ===\n");

	printf("total_income === %llu rub\n", stata.total_income);
	printf("total_expences === %llu rub\n", stata.total_expences);
	printf("max_month_expences === %llu rub\n", stata.max_month_expences);
	printf("max_year_expences === %llu rub\n", stata.max_year_expences);
	printf("max_month_income === %llu rub\n", stata.max_month_income);;
	printf("max_year_income === %llu rub\n", stata.max_year_income);
	printf("max_month_profit === %lld rub\n", stata.max_month_profit);
	printf("max_year_profit === %lld rub\n", stata.max_year_profit);

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void bob_print_bankruptcy() {
	printf("=== Bob's bancruptcy report ===\n");
    if (not bob.bankruptcy.bancrupt) {
        printf(" no bankruptcy\n");
        return;
    }
    printf(" went bankrupt in %d-%02d\n",
           bob.bankruptcy.year, bob.bankruptcy.month);
    printf(" cause : %s\n", bob.bankruptcy.reason);
    printf("  was: %llu rub short.\n", bob.bankruptcy.deficit);
}


void bob_print()
{
	printf("=== Bob's financial report ===\n");
	printf("Cash:        %llu rub.\n", bob.cash);
	printf("Bank account: %llu rub.\n", bob.bank.depos);

	printf("Property:\n");

	switch (bob.car.car_type) {
    	case NO_CAR:      printf("No car\n");          break;
    	case SMALL_CAR:   printf("Small car\n");       break;
    	case BIG_CAR:     printf("Big car\n");         break;
    	case SPORT_CAR:   printf("Sport car\n");       break;
	}

	printf("Residence:\n");

	if (bob.lives_with_parents) {
		printf("lives with parents\n");
	}
	else {
		printf("lives in her own apartment\n");
	}

	printf("Lifestyle: ");
	switch (bob.lyfestile) {
    	case START_OF_LIFE: printf("Start of life\n"); break;
    	case MIDDLE_CLASS:  printf("Middle class\n");  break;
    	case BOURGEOISIE:   printf("Bourgeoisie\n");   break;
    	case ELITE:         printf("Elite\n");         break;
	}
}


void bob_print_tax_refund()
{
    printf("=== bob's tax refund report ===\n");
    printf("  Property base left: %llu rub.\n",
           bob.tax_refund.property_base_left);
    printf("  Interest base left: %llu rub.\n",
           bob.tax_refund.interest_base_left);
    printf("  Total refunded:     %llu rub.\n",
           bob.tax_refund.refunded_total);
    printf("  Effective rate:     %.2f%%\n",
           bob.tax_refund.refunded_total > 0
           ? 100.0 * bob.tax_refund.refunded_total / 5'000'000.0
           : 0.0);
}


void bob_detailed()
{
	printf("=== Bob's detailed stats ===\n");

	printf("total_income === %llu rub\n", statb.total_income);
	printf("total_expences === %llu rub\n", statb.total_expences);
	printf("max_month_expences === %llu rub\n", statb.max_month_expences);
	printf("max_year_expences === %llu rub\n", statb.max_year_expences);
	printf("max_month_income === %llu rub\n", statb.max_month_income);;
	printf("max_year_income === %llu rub\n", statb.max_year_income);
	printf("max_month_profit === %lld rub\n", statb.max_month_profit);
	printf("max_year_profit === %lld rub\n", statb.max_year_profit);

}

void all_print()
{
	alice_print();
	alice_print_bankruptcy();
	alice_print_tax_refund();
	alice_detailed();

	bob_print();
	bob_print_bankruptcy();
	bob_print_tax_refund();
	bob_detailed();
}
///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	all_init();

	all_simulation();

	all_print();
}