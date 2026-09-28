#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

using RUB = unsigned long long int; // можно написать на с typedf

struct Person 
{
	RUB cash;
	RUB salary;
	char const* housing;
	char const* work;
};

struct Taxes
{
	RUB 
};

struct Person Zeckster;

int roll_d100()
{
	int limit = RAND_MAX - (RAND_MAX % 100);   // RAND_MAX
	int r;
	do 
	{
		r = rand();    // отбрасываем «хвост»
	} 
	while (r >= limit); 
	
	return r % 100 + 1;
}

void Zeckster_health(int month)
{
	if (month == 12 or month == 1 or month == 2) // Winter
	{
		int roll = roll_d100();
		if (roll <= 18)
		{
			Zeckster.cash -= 5'000;
		}
		else if (18 < roll && roll <= 22) // seriously sick chance 4%
		{
			Zeckster.cash -= 15'000;
		}
	}
	if (month == 3 or month == 4 or month == 5) // Spring
	{
		int roll = roll_d100();
		if (roll <= 20) // sick chance 20%
		{
			Zeckster.cash -= 5'000;
		}
		else if (20 < roll && roll <= 25) // seriously sick chance 5%
		{
			Zeckster.cash -= 15'000;
		}
	}
	if (month == 6 or month == 7 or month == 8) // Summer
	{
		int roll = roll_d100();
		if (roll <= 6)  // sick chance 6%
		{
			Zeckster.cash -= 5'000;
		}
		else if (6 < roll && roll <= 8) // seriously sick chance 3%
		{
			Zeckster.cash -= 15'000;
		}
	}
	if (month == 9 or month == 10 or month == 11)  // Аutumn
	{
		int roll = roll_d100();
		if (roll <= 23)  // sick chance 23%
		{
			Zeckster.cash -= 5'000;
		}
		else if (23 < roll && roll <= 29) // seriously sick chance 7%
		{
			Zeckster.cash -= 15'000;
		}
	}
}

void Zeckster_init() 
{
	Zeckster.cash = 20'000;

	Zeckster.salary = 30'000;  // Cash which Zeckster's parents send him

	Zeckster.housing = "Dormitory";  // Zeckster lives in a dormitory 
}

void Zeckster_salary(const int year, const int month)
{
	if (year == 2027 and month == 8)  // Promotion
	{ 
		Zeckster.salary = 65'000;  // Park-time work + Cash which Zeckster's parents send him
		Zeckster.work = "Beginnig engineer";
	}
	if (year == 2028 and month == 9)
	{
		Zeckster.salary = 80'000;  // Full-time job 
		Zeckster.work = "Engineer";
	}
	if (year == 2029 and month == 11) 
	{
		Zeckster.salary = 135'000;
		Zeckster.work = "Lead ngineer";
	}
	Zeckster.cash += Zeckster.salary;  // += Значит Zeckster.cash + Zeckster.salary = Zeckster.cash
}

void Zeckster_rent(const int year, const int month)
{
	RUB rent_housing = 2500;
	RUB utility_services;

	if (year == 2028 and month == 8) 
	{
		rent_housing = 50000;  // Zeckster moves to a new apartment, because he finished studying at the university
		Zeckster.housing = "Rented apartament"; 
	}
	if (month == 1) 
	{
		rent_housing = (RUB) (rent_housing * 1.05);  // Every year rent increases by 5%
	}

	Zeckster.cash -= rent_housing;
}

void simulation()
{
	int year = 2026;
	int month = 9;

	while (not (year == 2030 and month == 9)) {  // == это сравнение переменную год с числом, = это присваивание 
		// alice_car();
		// alice_mortgage();
		// alice_food();
		// alice_dog();
		// alice_bank_income()

		Zeckster_salary(year, month);
		Zeckster_rent(year,month);
		Zeckster_health(month);

		++month;
		if (month == 13) {
			++year;
			month = 1;
		}
	}
}

void Zeckster_print()
{
	printf("Zeckster cash = %llu\n", Zeckster.cash); // d целое знаковое, u целое незнаковое \n - new line - переход на следующую строку

	printf("Zeckster salary = %llu\n", Zeckster.salary);

	printf("Zeckster rent = %s\n", Zeckster.housing);
}

int main()
{
	srand((unsigned)time(NULL));

	Zeckster_init();

	simulation();

	Zeckster_print();
}