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
	int health;
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

void Zeckster_sick()
{
	char choise[32];
	printf("Zeckster is sick. Do you want bay a medicins?: (Y/N) \n");
	scanf_s("%s", choise, 32);

	if (choise[0] == 'N')
	{
		Zeckster.health -= 15;
	}
	else if (choise[0] == 'Y')
	{
		Zeckster.cash -= 5'000;
		printf("Zeckster is recovered!\n");
	}
	else {
		printf("Error enter!\n");
	}
}

void Zeckster_health(int month)
{
	if (month == 12 or month == 1 or month == 2) // Winter
	{
		//int roll = roll_d100();
		if (roll_d100() <= 20) 
		{
			Zeckster_sick();
		}
	}
	if (month == 3 or month == 4 or month == 5) // Spring
	{
		int roll = roll_d100();
		if (roll <= 25) // sick chance 25%
		{
			Zeckster_sick();
		}
	}
	if (month == 6 or month == 7 or month == 8) // Summer
	{
		int roll = roll_d100();
		if (roll <= 5) // sick chance 5%
		{
			Zeckster_sick();
		}
	}
	if (month == 9 or month == 10 or month == 11) // Аutumn
	{
		int roll = roll_d100();
		if (roll <= 35) // sick chance 35%
		{
			Zeckster_sick();
		}
	}
}

void Zeckster_init()
{
	Zeckster.cash = 20'000;

	Zeckster.salary = 80'000;

	Zeckster.housing = "Dormitory"; // Zeckster lives in a dormitory 

	Zeckster.health = 90;
}


void Zeckster_salary(const int year, const int month)
{
	if (year == 2027 and month == 1) { // Promotion
		Zeckster.salary = 100'000;
	}
	Zeckster.cash += Zeckster.salary;  // += Значит Zeckster.cash + Zeckster.salary = Zeckster.cash
}

void Zeckster_housing(const int year, const int month)
{
	if (year == 2028 and month == 8) {
		Zeckster.housing = "Rented apartament"; // Zeckster moves to a new apartment, because he finished studying at the university
	}
}

void Zeckster_rent(const int year, const int month)
{
	RUB rent_housing = 2500;

	if (year == 2028 and month == 8) 
	{
		rent_housing = 50000;  // Zeckster moves to a new apartment, because he finished studying at the university
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
		Zeckster_housing(year, month);
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

	printf("Zeckster housing = %s\n", Zeckster.housing);

	printf("Zeckster health = %d\n", Zeckster.health);
}

int main()
{
	srand((unsigned)time(NULL));

	Zeckster_init();

	simulation();

	Zeckster_print();
}