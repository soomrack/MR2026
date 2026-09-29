#include <stdio.h>


using RUB = int;


struct Person {
	RUB cash;
	RUB salary;
};

struct Person rybka;


void rybka_salary(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.salary = 60000;
	}


	rybka.cash += rybka.salary;
}
void rybka_parking(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.parking = 10000;
	}


	rybka.cash += rybka.parking;
}

void rybka_food(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.food = 25000;
	}


	rybka.cash -= rybka.food;
}


void rybka_entertainment(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.enterteinment = 10000;
	}


	rybka.cash -= rybka.entertainment;
}


void rybka_public_utilities(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.public_utilities = 12000;
	}


	rybka.cash -= rybka.public_utilities;
}


void rybka_car(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.car = 10000;
	}


	rybka.cash -= rybka.car;
}


void rybka_public_transport(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.public_transport = 4000;
	}


	rybka.cash -= rybka.public_transport;
}


void rybka_print()
{
	printf("Rybka cash = %d\n", rybka.cash);
}


void rybka_init()
{
	rybka.cash = 10000;
	rybka.salary = 60000;
}


void simulation()

{
	int year = 2026;
	int month = 9;
	while (not(year == 2027 and month == 9)) {

		rybka_salary(year, month);
		// rybka_public_utillities();
		// rybka_parking();
		// rybka_food();
		// rybka_entertaintment();
		// rybka_car
		//rybka_public_transport


		++month;
		if (month == 13) {
			++year;
			month = 1;
		}
		
	}
}

int main()
{
	rybka_init();

	simulation();
	
	rybka_print();
}
