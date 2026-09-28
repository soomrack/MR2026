#include <stdio.h>

using RUB = unsigned long long int;
////////////////
struct Person {
	RUB cash;
	RUB salary;
	RUB food;
	RUB mortgage;
	RUB rent;
	RUB dog;
	RUB car;
	RUB bank_income;
	RUB home_bills;
	RUB part_time_job;
	RUB car_savings;
	bool has_car;
	bool lives_with_parents;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
struct Person alice;
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_salary(const int year, const int month)
{
	if (year == 2027 and month == 1) { // Повышение
		alice.salary = 100'000;
	}

	alice.cash += alice.salary;
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
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_mortgage(const int year, const int month)
{
	// Ипотека появляется после переезда от родителей в июне 2027
	if (year == 2027 and month >= 6) {
		alice.mortgage = 35'000;
		alice.cash -= alice.mortgage;
	}
	else if (year > 2027) {
		alice.mortgage = 35'000;
		alice.cash -= alice.mortgage;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_rent(const int year, const int month)
{
	// Квартплата появляется после переезда от родителей в июне 2027
	if (year == 2027 and month >= 6) {
		alice.rent = 8'000;
		alice.cash -= alice.rent;
	}
	else if (year > 2027) {
		alice.rent = 8'000;
		alice.cash -= alice.rent;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_dog(const int year, const int month)
{
	// Собака появляется после переезда от родителей в июне 2027
	if (year == 2027 and month >= 6) {
		RUB dog_cost = 5'000;

		// Раз в полгода — ветеринар
		if (month == 6 or month == 12) {
			dog_cost += 15'000;
		}

		alice.dog = dog_cost;
		alice.cash -= alice.dog;
	}
	else if (year > 2027) {
		RUB dog_cost = 5'000;

		if (month == 6 or month == 12) {
			dog_cost += 15'000;
		}

		alice.dog = dog_cost;
		alice.cash -= alice.dog;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_home_bills(const int year, const int month)
{
	// Коммунальные платежи появляются после переезда от родителей в июне 2027
	if (year == 2027 and month >= 6) {
		alice.home_bills = 6'000;
		alice.cash -= alice.home_bills;
	}
	else if (year > 2027) {
		alice.home_bills = 6'000;
		alice.cash -= alice.home_bills;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_part_time_job(const int year, const int month)
{
	// Подработка появляется с февраля 2027
	if (year == 2027 and month >= 2) {
		alice.part_time_job = 15'000;
		alice.cash += alice.part_time_job;
	}
	else if (year > 2027) {
		alice.part_time_job = 15'000;
		alice.cash += alice.part_time_job;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_bank_income(const int year, const int month)
{
	// Элис откладывает каждый месяц 5 тысяч или 10% от чистой прибыли, если это больше
	RUB deposit = 5'000;

	// Чистая прибыль за месяц — это доходы минус расходы
	RUB income = alice.salary + alice.part_time_job;
	RUB expenses = alice.food + alice.mortgage + alice.rent + alice.dog + alice.home_bills;

	if (income > expenses) {
		RUB ten_percent = (income - expenses) / 10;
		if (ten_percent > deposit) {
			deposit = ten_percent;
		}
	}

	alice.bank_income = deposit;
	alice.cash -= alice.bank_income;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void alice_car(const int year, const int month)
{
	// Машина стоит 2-3 млн. Копим на неё, пока не накопим
	if (not alice.has_car) {
		RUB car_price = 2'500'000;

		// Если накопили достаточно — покупаем машину
		if (alice.car_savings >= car_price) {
			alice.cash -= car_price;
			alice.has_car = true;
		}
		else {
			// Откладываем на машину 30 тысяч в месяц
			alice.car_savings += 30'000;
			alice.cash -= 30'000;
		}
	}
	else {
		// Расходы на машину: бензин, страховка, ТО
		alice.car = 20'000;
		alice.cash -= alice.car;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void simulation()
{
	int year = 2026;
	int month = 9;

	alice.has_car = false;
	alice.lives_with_parents = true;
	alice.car_savings = 0;

	while (not (year == 2036 and month == 9)) {

		alice_salary(year, month);
		alice_car(year, month);
		alice_mortgage(year, month);
		alice_rent(year, month);
		alice_food(year, month);
		alice_dog(year, month);
		alice_bank_income(year, month);
		alice_home_bills(year, month);
		alice_part_time_job(year, month);

		//bob_salary

		++month;
		if (month == 13) {
			++year;
			month = 1;
		}

	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
void alice_init()
{
	alice.cash = 20'000;
	alice.salary = 80'000;
	alice.food = 0;
	alice.mortgage = 0;
	alice.rent = 0;
	alice.dog = 0;
	alice.car = 0;
	alice.bank_income = 0;
	alice.home_bills = 0;
	alice.part_time_job = 0;
	alice.car_savings = 0;
	alice.has_car = false;
	alice.lives_with_parents = true;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void alice_print()
{
	printf("Alice cash = %llu\n", alice.cash);
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int main()
{

	alice_init();

	simulation();

	alice_print();

}
