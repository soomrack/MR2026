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
    INF_CATEGORY_COUNT
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Bank {
	RUB depos;
	SRUB creditka;
	RUB credit_limit;
	int  credit_months;
	bool credit_active;
	RUB stocks;
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
//struct statistica statb;

struct short_stats { 
	RUB month_expences;
	RUB year_expences;
	RUB month_income;
	RUB year_income;
	SRUB month_profit;
	SRUB year_profit;
};

struct short_stats shortstat_a;
//struct short_stats shortstat_b;

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

struct Expences {
	RUB food;
	RUB dog;
	RUB mortgage;
	RUB rent;
	RUB home_taxes;
	RUB home_bills;
	RUB health;
	RUB vacation;
	RUB miscosts;
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
	RUB salary;
	Expences expences;
	Car car;
	Bank bank;
	RUB part_time_job;
	RUB car_savings;
	bool lives_with_parents;
	bool have_dog;
	Bankruptcy bankruptcy;
};


struct Inflation {
    double rate[INF_CATEGORY_COUNT];      // годовая ставка
    double multiplier[INF_CATEGORY_COUNT]; // накопленный коэффициент с 2026 года
};

Inflation g_inflation;

struct Person alice;
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
void alice_init()
{
	zero_car(alice.car);
	alice.health = HEALTHY;
	alice.strong_drugs = false;
	alice.sick_months_left = 0;
	alice.cash = 20'000;
	alice.salary = 80'000;
	alice.part_time_job = 0;
	alice.car.car_type = NO_CAR;
	alice.car_savings = 0;
	alice.lives_with_parents = true;
	alice.have_dog = false;
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
	alice.expences.food = 25'000; // Подвергается инфляции
	alice.expences.dog = 5'000;
	alice.expences.rent = 8'000;
	alice.expences.home_bills = 6'000;    

	alice.expences.mortgage = 0;  // Не подвергается
}

void alice_bancrupt_init()
{
	alice.bankruptcy.bancrupt = false;
	alice.bankruptcy.year = 0;
	alice.bankruptcy.month = 0;
	alice.bankruptcy.deficit = 0;
	alice.bankruptcy.reason = "";
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
}
///////////////////////////////////////////////////////////////////////////////////////////////
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

    for (int i = 0; i < INF_CATEGORY_COUNT; i++) {
        g_inflation.multiplier[i] = 1.0;
    }
}


void all_init()
{
	alice_bank_init();
	alice_base_expences_init();
	alice_bancrupt_init();
	alice_init();
	alice_stats_init();
	alice_short_stats_init();
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
void lifestyle_bank_savings_upd()
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

void alice_lifesyle_expences_upd()
{

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
		alice.salary = 150'000;
	}

	alice.cash += alice.salary;
	
	stata.total_income += (alice.salary);
	shortstat_a.month_income += (alice.salary);
	shortstat_a.year_income += (alice.salary);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_part_time_job(const int year, const int month)
{
	// Подработка появляется с февраля 2027
	if (year == 2027 and month == 2) {
		alice.part_time_job = 15'000;
	}
	alice.cash += alice.part_time_job;

	stata.total_income += (alice.part_time_job);
	shortstat_a.month_income += (alice.part_time_job);
	shortstat_a.year_income += (alice.part_time_job);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//===РАСХОДЫ
//налоги///////////////////////////////////////////////////////////////////////////////////////////
void alice_income_taxes_ndfl()
{
	RUB money_score = 0;
	RUB month_taxes = 0;
	RUB year_taxes = 0;
	if (shortstat_a.month_income <= 200'000)
	{
		RUB taxes = shortstat_a.month_income * 0.13;
		month_taxes += taxes;
	}
	money_score = shortstat_a.year_income;
	if (month == 12)
	{
		if ((shortstat_a.year_income > 2'400'000) and (shortstat_a.year_income <= 5'000'000))
		{
			RUB extra_taxes = (shortstat_a.year_income - 2'400'000) * 0.15;
			year_taxes += extra_taxes;
		}
		else if ((shortstat_a.year_income > 5'000'000) and (shortstat_a.year_income <= 20'000'000))
		{
			RUB extra_taxes = (shortstat_a.year_income - 5'000'000) * 0.18;
			money_score = 5'000'000;
			extra_taxes += (money_score - 2'400'000) * 0.15;
			year_taxes += extra_taxes;
		}
		else if ((shortstat_a.year_income > 20'000'000) and (shortstat_a.year_income <= 50'000'000))
		{
			RUB extra_taxes = (shortstat_a.year_income - 20'000'000) * 0.2;
			money_score = 20'000'000;
			extra_taxes += (money_score - 5'000'000) * 0.18;
			money_score = 5'000'000;
			extra_taxes += (money_score - 2'400'000) * 0.15;
			year_taxes += extra_taxes;
		}
		else if (shortstat_a.year_income > 50'000'000)
		{
			RUB extra_taxes = (shortstat_a.year_income - 50'000'000) * 0.22;
			money_score = 50'000'000;
			extra_taxes += (money_score - 20'000'000) * 0.2;
			money_score = 20'000'000;
			extra_taxes += (money_score - 5'000'000) * 0.18;
			money_score = 5'000'000;
			extra_taxes += (money_score - 2'400'000) * 0.15;
			year_taxes += extra_taxes;
		}
	}
	
	alice.cash -= (month_taxes + year_taxes);

	stata.total_expences += (month_taxes + year_taxes);
	shortstat_a.month_expences += (month_taxes + year_taxes);
	shortstat_a.year_expences += (month_taxes + year_taxes);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_food(const int year, const int month)
{
	RUB food_cost = 25'000;

	// Каждый январь и каждый май еда дороже из-за праздников
	if (month == 1 or month == 5) {
		food_cost += 10'000;
	}

	food_cost = inflate(food_cost, INF_FOOD);
	alice.expences.food = food_cost;
	pay_mandatory(alice, alice.expences.food, "food");

	stata.total_expences += (alice.expences.food);
	shortstat_a.month_expences += (alice.expences.food);
	shortstat_a.year_expences += (alice.expences.food);
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
	// Ипотека появляется после переезда от родителей в июне 2027
	if (year == 2027 and month == 6) {
		alice.expences.mortgage = 50'000;
	}
	pay_mandatory(alice, alice.expences.mortgage, "mortgage");

	stata.total_expences += alice.expences.mortgage;
	shortstat_a.month_expences += alice.expences.mortgage;
	shortstat_a.year_expences += alice.expences.mortgage;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_rent(const int year, const int month)
{
	RUB local_rent = 0;
	// Квартплата появляется после переезда от родителей в июне 2027
	if (year == 2027 and month == 6)
	{
		local_rent = alice.expences.rent;
	}
	local_rent = inflate(local_rent, INF_UTILITIES);
	pay_mandatory(alice, alice.expences.rent, "rent");

	stata.total_expences += alice.expences.rent;
	shortstat_a.month_expences += alice.expences.rent;
	shortstat_a.year_expences += alice.expences.rent;
}

void alice_house_taxes(const int year, const int month)
{
	if (year == 2027 and month == 6) {
		alice.expences.home_taxes = ((60-20)*100'000*0.001); //Налог на квартиру
	}
	pay_mandatory(alice, alice.expences.home_taxes, "home_taxes");

	stata.total_expences += alice.expences.home_taxes;
	shortstat_a.month_expences += alice.expences.home_taxes;
	shortstat_a.year_expences += alice.expences.home_taxes;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_dog(const int year, const int month)
{
	RUB dog_cost = 0;
	bool was_vet = false;
	// Собака появляется после переезда от родителей в июне 2027
	if (((year == 2027 and month >= 6) or year >= 2027) and alice.cash >= alice.bank.min_reserve) 
	{
		alice.have_dog = true;
		dog_cost += alice.expences.dog;
		dog_cost = inflate(dog_cost, INF_DOG);
		RUB first_vet = random_range(3'500, 8'000);
		first_vet = inflate(first_vet, INF_DOG);
		pay_mandatory(alice, first_vet, "vet_for_dog");
	}

	if (!alice.have_dog) return;

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

	RUB total_dog = (alice.expences.dog + dog_cost);
	pay_mandatory(alice, total_dog, "dog");

	stata.total_expences += total_dog;
	shortstat_a.month_expences += total_dog;
	shortstat_a.year_expences += total_dog;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_home_bills(const int year, const int month)
{
	RUB local_home_bills = 0;
	if ((year <= 2027) or (year == 2027 and month < 6)) return;

	// Коммунальные платежи появляются после переезда от родителей в июне 2027
	local_home_bills = alice.expences.home_bills;
	local_home_bills = inflate(local_home_bills, INF_UTILITIES);
	pay_mandatory(alice, alice.expences.home_bills, "home_bills");

	stata.total_expences += (alice.expences.home_bills);
	shortstat_a.month_expences += (alice.expences.home_bills);
	shortstat_a.year_expences += (alice.expences.home_bills);
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
	inflate(fuel_and_consumables, INF_TRANSPORT);

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

	if (available < model.price + model.utilsbor) return;

	// Сколько берём из копилки и сколько добираем со вклада
	RUB from_car_savings = (alice.car_savings < model.price + model.utilsbor) ? alice.car_savings : (model.price + model.utilsbor);
	RUB from_bank = (model.price + model.utilsbor) - from_car_savings;

	alice.car_savings -= from_car_savings;
	alice.bank.depos -= from_bank;

	stata.total_expences += (model.price + model.utilsbor);
	shortstat_a.month_expences += (model.price + model.utilsbor);
	shortstat_a.year_expences += (model.price + model.utilsbor);

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
    
    // Пытаемся оплатить ремонт
    if (alice.cash >= repair_cost) {
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
		int coffe_times = random_range(1,30);
        pay_mandatory(alice, coffe_times*cost, "coffee");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(45) and alice.cash > alice.bank.min_reserve * 0.9) 
	{
        RUB cost = random_range(300, 1200);
        pay_mandatory(alice, cost, "subscriptions");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(50) and alice.cash > alice.bank.min_reserve * 0.8) 
	{
        RUB cost = random_range(80, 300);
		int snacks_times = random_range(1, 30);
        pay_mandatory(alice, snacks_times*cost, "snacks_home");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(30) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(400, 1500);
		int taxi_times = random_range(1, 30);
        pay_mandatory(alice, taxi_times*cost, "taxi");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(25) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(1500, 4500);
        pay_mandatory(alice, cost, "hygiene_household");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(20) and alice.cash > alice.bank.min_reserve * 0.8) 
	{
        RUB cost = random_range(1000, 5000);
		int cafe_restaurant_times = random_range(1, 30);
        pay_mandatory(alice, cafe_restaurant_times*cost, "cafe_restaurant");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(15) and alice.cash > alice.bank.min_reserve * 0.9) 
	{
        RUB cost = random_range(1500, 5000);
        pay_mandatory(alice, cost, "entertainment");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(15) and alice.cash > alice.bank.min_reserve * 0.7) 
	{
        RUB cost = random_range(3000, 9000);
        pay_mandatory(alice, cost, "cosmetics");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(12) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(1500, 5000);
        pay_mandatory(alice, cost, "books_games");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(8) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(2000, 7000);
        pay_mandatory(alice, cost, "vitamins");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(10) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(5000, 18000);
        pay_mandatory(alice, cost, "clothes");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(5) and alice.cash > alice.bank.min_reserve) 
	{
        RUB cost = random_range(7000, 25000);
        pay_mandatory(alice, cost, "shoes");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(5) and alice.cash > alice.bank.min_reserve * 0.9) 
	{
        RUB cost = random_range(4000, 20000);
        pay_mandatory(alice, cost, "gifts");
        stata.total_expences += cost;
        shortstat_a.month_expences += cost;
        shortstat_a.year_expences += cost;
    }

    if (chance(3) and alice.cash > alice.bank.min_reserve * 0.8) 
	{
        RUB cost = random_range(10000, 35000);
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
	shortstat_a.month_profit = 0;

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
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_simulation()
{
	if (month == 1) 
	{
    	inflation_update();
	}

	lifestyle_bank_savings_upd();

	alice_credit_interest();           // Начисление по кредитке, если не погасила

	alice_bank_income(year, month);    // Доходы
	alice_salary(year, month);
	alice_part_time_job(year, month);    

	alice_income_taxes_ndfl();         // Налоги
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

void all_simulation()
{
	
	while (not (year == 2036 and month == 9)) 
	{
		alice_simulation();
		//bob_simulation();

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

void alice_detailed()
{
	printf("=== Alice's detailed stats ===\n");

	printf("total_income === %llu rub\n", stata.total_income);
	printf("total_expences === %llu rub\n", stata.total_expences);
	printf("max_month_expences === %llu rub\n", stata.max_month_expences);
	printf("max_year_expences === %llu rub\n", stata.max_year_expences);
	printf("max_month_income === %llu rub\n", stata.max_month_income);;
	printf("max_year_income === %llu rub\n", stata.max_year_income);
	printf("max_month_profit === %llu rub\n", stata.max_month_profit);
	printf("max_year_profit === %llu rub\n", stata.max_year_profit);

}

void all_print()
{
	alice_print();
	alice_print_bankruptcy();
	alice_detailed();
	//bob_print();
}
///////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	all_init();

	all_simulation();

	all_print();
}