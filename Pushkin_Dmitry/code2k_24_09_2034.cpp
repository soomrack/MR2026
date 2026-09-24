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

struct Bank {
	RUB depos;
	SRUB creditka;
	RUB stocks;
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

//struct Nalog {
//	int power;
//	double rate;
//	RUB 
//};

struct Car {
	RUB osago;
	RUB kasco;
    RUB utilsbor;
	RUB nalog;
};

struct Car smallcar;
struct Car bigcar;
struct Car sportcar;

struct Person {
	RUB cash;
	RUB salary;
	RUB food;
	RUB mortgage;
	RUB rent;
	RUB dog;
	RUB car;
	Bank bank;
	RUB home_bills;
	RUB part_time_job;
	RUB car_savings;
	bool has_car;
	bool lives_with_parents;
};

struct Person alice;


int year = 2026;
int month = 9;
///////////////////////////////////////////////////////////////////////////////////////////////
void alice_init()
{
	alice.cash = 20'000;
	alice.salary = 80'000;
	alice.food = 0;
	alice.mortgage = 0;
	alice.rent = 0;
	alice.dog = 0;
	alice.car = 0;
	alice.bank.depos = 0;
	alice.home_bills = 0;
	alice.part_time_job = 0;
	alice.car_savings = 0;
	alice.has_car = false;
	alice.lives_with_parents = true;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void stats_init()
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
void short_stats_init()

{
	shortstat_a.month_expences = 0;
	shortstat_a.year_expences = 0;
	shortstat_a.month_income = 0;
	shortstat_a.year_income = 0;
	shortstat_a.month_profit = 0;
	shortstat_a.year_profit = 0;
}

void cars_init()
{
	smallcar.kasco = 0;
	smallcar.nalog = 0;
	smallcar.osago = 0;
	smallcar.utilsbor = 0;

	bigcar.kasco = 0;
	bigcar.nalog = 0;
	bigcar.osago = 0;
	bigcar.utilsbor = 0;

	sportcar.kasco = 0;
	sportcar.nalog = 0;
	sportcar.osago = 0;
	sportcar.utilsbor = 0;
}

void all_init()
{
	alice_init();
	stats_init();
	short_stats_init();
	cars_init();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_bank_income(const int year, const int month)
{
	// Начисление 14% годовых, ежемесячно (сложный процент) === 14 / (100 * 12)
	RUB bank_income = alice.bank.depos * 14 / (100 * 12);
	alice.bank.depos += bank_income;
	// Отдельным вкладом лежат накопления на машину, чтобы по ним тоже капал процент
	RUB car_savings_income = alice.car_savings * 14 / (100 * 12);
	alice.car_savings += alice.car_savings * 14 / (100 * 12);

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
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_food(const int year, const int month)
{
	RUB food_cost = 25'000;

	// Каждый январь и каждый май еда дороже из-за праздников
	if (month == 1 or month == 5) {
		food_cost += 10'000;
	}

	alice.food = food_cost;
	alice.cash -= alice.food;

	stata.total_expences += (alice.food);
	shortstat_a.month_expences += (alice.food);
	shortstat_a.year_expences += (alice.food);
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
		alice.mortgage = 50'000;
	}
	alice.cash -= alice.mortgage;

	stata.total_expences += alice.mortgage;
	shortstat_a.month_expences += alice.mortgage;
	shortstat_a.year_expences += alice.mortgage;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_rent(const int year, const int month)
{
	// Квартплата появляется после переезда от родителей в июне 2027
	if (year == 2027 and month == 6) {
		alice.rent = 8'000;
	}
	alice.cash -= alice.rent;

	stata.total_expences += alice.rent;
	shortstat_a.month_expences += alice.rent;
	shortstat_a.year_expences += alice.rent;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_dog(const int year, const int month)
{
	RUB dog_cost = 0;
	// Собака появляется после переезда от родителей в июне 2027
	if (year == 2027 and month == 6) {
		alice.dog = 5'000;
	}
	// Раз в полгода — ветеринар
	if (month == 6 or month == 12) {
		dog_cost = 15'000;
	}
	alice.cash -= (alice.dog + dog_cost);

	stata.total_expences += (alice.dog + dog_cost);
	shortstat_a.month_expences += (alice.dog + dog_cost);
	shortstat_a.year_expences += (alice.dog + dog_cost);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_home_bills(const int year, const int month)
{
	// Коммунальные платежи появляются после переезда от родителей в июне 2027
	if (year == 2027 and month >= 6) {
		alice.home_bills = 6'000;
	}
	alice.cash -= alice.home_bills;

	stata.total_expences += (alice.home_bills);
	shortstat_a.month_expences += (alice.home_bills);
	shortstat_a.year_expences += (alice.home_bills);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_bank_savings(const int year, const int month)
{
	// Элис откладывает каждый месяц 5 тысяч или 10% от чистой прибыли, если это больше
	RUB deposit = 5'000;

	// Чистая прибыль за месяц — это доходы минус расходы
	//RUB income = alice.salary + alice.part_time_job;
	//RUB expenses = alice.food + alice.mortgage + alice.rent + alice.dog + alice.home_bills + alice.car;

	//if (income > expenses) {
	//	RUB ten_percent = (income - expenses) / 10;
	//	if (ten_percent > deposit) 
	//	{
	//		deposit = ten_percent;
	//	}
	//}
	//else //Если терпим убытки лучше не класть в банк
	//{
	//	deposit = 0;
	//}
	// Подушка налички
	if (alice.cash < 20'000) 
	{
		deposit = 0;
	}
	//Зачем держать больше налички?
	else if (alice.cash > 150'000)
	{
		deposit += alice.cash - 150'000;
	}

	alice.bank.depos += deposit;
	alice.cash -= deposit;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_car_service(const int year, const int month)  //ФУНКЦИЯ БУДЕТ СИЛЬНО ДОПОЛНЕНА 4 Пунктами
{
	if (not alice.has_car) return;
	// Расходы на машину: бензин, страховка, ТО
	alice.car = 20'000;
	alice.cash -= alice.car;

	stata.total_expences += (alice.car);
	shortstat_a.month_expences += (alice.car);
	shortstat_a.year_expences += (alice.car);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_car_save()
{
	if (alice.has_car) return;

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
	if (alice.has_car) return;

	RUB car_price = 2'500'000;
	RUB half_deposit = alice.bank.depos / 2;
	RUB available = alice.car_savings + half_deposit;

	if (available < car_price) return;

	// Сколько берём из копилки и сколько добираем со вклада
	RUB from_car_savings = (alice.car_savings < car_price) ? alice.car_savings : car_price;
	RUB from_bank = car_price - from_car_savings;

	alice.car_savings -= from_car_savings;
	alice.bank.depos -= from_bank;
	alice.has_car = true;

	// Остаток копилки — на вклад (нуля тоже касается, это безвредно)
	alice.bank.depos += alice.car_savings;
	alice.car_savings = 0;

	stata.total_expences += (car_price);
	shortstat_a.month_expences += (car_price);
	shortstat_a.year_expences += (car_price);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_car(const int year, const int month)
{
	alice_car_service(year, month);
	alice_car_save();
	alice_car_buying(year, month);              
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void statsupdate(const int year, const int month)
{
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
	
	alice_salary(year, month);
	alice_part_time_job(year, month);    // сначала все доходы
	alice_residence(year, month);
	alice_mortgage(year, month);         // потом обязательные расходы
	alice_rent(year, month);
	alice_food(year, month);
	alice_dog(year, month);
	alice_home_bills(year, month);
	alice_bank_income(year, month);     // проценты по вкладу 
	alice_bank_savings(year, month);      // отчисление на вклад
	alice_car(year, month);

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
void alice_print()
{
	printf("=== Alice's financial report ===\n");
	printf("Cash:        %llu rub.\n", alice.cash);
	printf("Bank account: %llu rub.\n", alice.bank.depos);

	printf("Property:\n");

	if (alice.has_car) {
		printf("Car\n");
	}
	else {
		printf("No car\n");
	}

	printf("Residence:\n");

	if (alice.lives_with_parents) {
		printf("lives with parents\n");
	}
	else {
		printf("lives in her own apartment\n");
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