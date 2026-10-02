#include <stdio.h>
#include <iostream>

using RUB = float;


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
	RUB savings_account;
	RUB tax;
	RUB inflation;
	RUB basic_infl_coeff;
	RUB service_infl_coeff;
	RUB luxury_infl_coeff;
};


struct Person vika;


void vika_init()
{
	vika.cash = 0;
	vika.salary = 0;
	vika.from_mother = 25'000;
	vika.food = 15'000;
	vika.dorm = 3300;
	vika.metro = 960;
	vika.merchandise = 500;
	vika.medicine = 1500;
	vika.clothes = 1000;
	vika.deutsch = 9900;
	vika.therapist = 8000;
	vika.presents = 1300;
	vika.household = 2000;
	vika.savings_account = 200'000;
	vika.tax = 0;
	vika.inflation = 0.06;
	vika.basic_infl_coeff = 0.9;
	vika.service_infl_coeff = 1.1;
	vika.luxury_infl_coeff = 1.2;

}



void vika_print()
{
	std::cout << "Vika cash = " << vika.cash << '\n';
	std::cout << "Vika savings = " << vika.savings_account << '\n';
}

void vika_salary(const int year, int month)
{
	if (year == 2026 and month == 10) { //find work
		vika.salary = 36'000;
		vika.tax = vika.salary * 0.13;

	}

	vika.cash += vika.salary-vika.tax;
}

void vika_savings_account(const int year, int month, float account_procent)
{
	if (month == 1) {
		account_procent = account_procent - 0.005;
		vika.savings_account += vika.savings_account * account_procent;
	}
	
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


void vika_inflation(const int year, const int month)
{
	if (year >= 2027 && month == 1) {
		vika.food *= 1 + vika.inflation * vika.basic_infl_coeff;
		vika.dorm *= 1 + vika.inflation * vika.service_infl_coeff;
		vika.metro *= 1 + vika.inflation;
		vika.merchandise *= 1 + vika.inflation * vika.basic_infl_coeff;
		vika.medicine *= 1 + vika.inflation * vika.luxury_infl_coeff;
		vika.clothes *= 1 + vika.inflation * vika.basic_infl_coeff;
		vika.deutsch *= 1 + vika.inflation * vika.service_infl_coeff;
		vika.therapist *= 1 + vika.inflation * vika.service_infl_coeff;
		vika.presents *= 1 + vika.inflation * vika.luxury_infl_coeff;
		vika.household *= 1 + vika.inflation * vika.basic_infl_coeff;
	}
}


void vika_apply_inflation(const int year, const int month)
{
	if (year == 2028 && month == 1) {
		vika.inflation = 0.045;
	}

	if (year == 2029 && month == 1) {
		vika.inflation = 0.04;
	}

	if (year >= 2030 && month == 1) {
		vika.inflation = 0.04 + 0.01* -1 + rand() % (1 + 1 + 1);
	}


}





void simulation()
{
	int year = 2026;
	int month = 9;
	float account_procent = 0.11;

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
		vika_savings_account(year, month, account_procent);
		vika_inflation(year, month);
		vika_apply_inflation(year, month);
	

		//bank_inkome()
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

