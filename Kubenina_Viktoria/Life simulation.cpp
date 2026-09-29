#include <stdio.h>
#include <iostream>

using RUB = unsigned long long int;


struct Person {
	RUB cash;
	RUB salary;
	RUB from_mother;
	RUB food;
	RUB dorm;
	RUB metro;
	RUB merchandise;
	RUB medicine;
	RUB clothes;
	RUB deutsch;
	RUB therapist;
	RUB presents;
	RUB household;

};


struct Person vika;


void vika_init()
{
	vika.cash = 0;
	vika.salary = 0;
	vika.from_mother = 25'000;
	vika.food = 15'000;
	vika.dorm = 3300;
	vika.merchandise = 500;
	vika.medicine = 1500;
	vika.clothes = 1000;
	vika.deutsch = 9900;
	vika.therapist = 8000;
	vika.presents = 1300;
	vika.household = 2000;


}



void vika_print()
{
	std::cout << "Vika cash = " << vika.cash << '\n';
}

void vika_salary(const int year, int month)
{
	if (year == 2026 and month == 10) { //find work
		vika.salary = 30'000;

	}

	vika.cash += vika.salary;
}

void vika_from_mother(const int year, int month)
{
	vika.cash += vika.from_mother;
}


void vika_food(const int year, int month)
{
	vika.cash -= vika.food;
}


void vika_dorm(const int year, int month)
{
	vika.cash -= vika.dorm;
}

void vika_metro(const int year, int month)
{
	vika.cash -= vika.metro;
}

void vika_merchandise(const int year, int month)
{
	vika.cash -= vika.merchandise;
}

void vika_medicine(const int year, int month)
{
	vika.cash -= vika.medicine;
}

void vika_clothes(const int year, int month)
{
	vika.cash -= vika.clothes;
}

void vika_deutsch(const int year, int month)
{
	vika.cash -= vika.deutsch;
}

void vika_therapist(const int year, int month)
{
	vika.cash -= vika.therapist;
}


void vika_presents(const int year, int month)
{
	vika.cash -= vika.presents;
}


void vika_household(const int year, int month)
{
	vika.cash -= vika.household;
}





void simulation()
{
	int year = 2026;
	int month = 9;
	while (not(year == 2026 and month == 12)) {

		vika_salary(year, month);
		vika_from_mother(year, month);
		vika_food(year, month);
		vika_dorm(year, month);
		vika_metro(year, month);
		vika_merchandise(year, month);
		vika_medicine(year, month);
		vika_clothes(year, month);
		vika_deutsch(year, month);
		vika_therapist(year, month);
		vika_presents(year, month);
		vika_household(year, month);

		//bank_inkome();
		//nalog_vb1chet

		++month;
		if (month == 13) {
			++year;
			month = 1;
		}

	}
}

int main()
{
	vika_init();
	
	simulation();

	vika_print();

}

