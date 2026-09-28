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


	rybka.salary += rybka.parking;
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
		// rybka_public utillities();
		// rybka_parking();
		// rybka_food();
		// rybka_entertaintment();
		// rybka_


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
