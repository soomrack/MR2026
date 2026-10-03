#include <stdio.h>
#include <iostream>
#include <iomanip>

using RUB = float;


struct Person {
	RUB cash;
	RUB salary;
	RUB from_mother;
	RUB food;
	RUB dorm;
	RUB metro;
	RUB rzd;
	RUB merchandise;
	RUB medicine;
	RUB clothes;
	RUB deutsch;
	RUB therapist;
	RUB presents;
	RUB household;
	RUB savings_account;
	float account_procent;
	float tax;
	float inflation;
	float basic_infl_coeff;
	float service_infl_coeff;
	float luxury_infl_coeff;
	bool mortgage_active;
	RUB apartment_price;
	RUB down_payment;
	RUB mortgage;
	float mortgage_rate;
	int mortgage_months;
	RUB mortgage_payment;
	RUB mortgage_interest;
	RUB mortgage_principal;
	RUB rent;
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
	vika.rzd = 7000;
	vika.merchandise = 500;
	vika.medicine = 1500;
	vika.clothes = 1000;
	vika.deutsch = 9900;
	vika.therapist = 8000;
	vika.presents = 1300;
	vika.household = 2000;
	vika.savings_account = 200'000;
	vika.account_procent = 0.11;
	vika.tax = 0;
	vika.inflation = 0.06;
	vika.basic_infl_coeff = 0.9;
	vika.service_infl_coeff = 1.1;
	vika.luxury_infl_coeff = 1.2;
	vika.mortgage_active = false;
	vika.apartment_price = 4'000'000;
	vika.down_payment = 1'000'000;
	vika.rent = 0;
	vika.mortgage = 3'000'000;
	vika.mortgage_rate = 0.18;
	vika.mortgage_months = 120;
	vika.mortgage_payment = 63'000;
	vika.mortgage_interest = 0;
	vika.mortgage_principal = 0;

}



void vika_print()
{
	std::cout << std::fixed << std::setprecision(0);
	std::cout << "Vika cash = " << vika.cash << '\n';
	std::cout << "Vika savings = " << vika.savings_account << '\n';
	std::cout << "Vika salary = " << vika.salary << '\n';
}

void vika_salary(const int year, int month)
{
	if (year == 2026 and month == 10) { //find work
		vika.salary = 30'000;
		vika.tax = vika.salary * 0.13;

	}

	if (year == 2028 and month == 9) {
		vika.salary = 60000;
		vika.tax = vika.salary * 0.13;

	}

	if (year >= 2028 and month == 1) { //promotion
		vika.salary += 10000;
		vika.tax = vika.salary * 0.13;

	}

	if (year >= 2034 and month == 1) { //promotion
		vika.salary = 140000;
		vika.tax = vika.salary * 0.13;

	}

	vika.cash += vika.salary-vika.tax;
}

void vika_savings_account(const int year, int month)
{
	if (month == 1) {
		vika.account_procent = vika.account_procent - 0.005;
		vika.savings_account += vika.savings_account * vika.account_procent;
	}

	if (year <= 2028) {
		vika.savings_account += vika.cash;
		vika.cash -= vika.cash;
	}
	
	if (year >= 2029) {
		vika.savings_account += 0.1 * vika.salary;
		vika.cash -= 0.1 * vika.salary;
		vika.savings_account += vika.cash;
		vika.cash -= vika.cash;
	}
}

void vika_from_mother(const int year, int month)
{
	if (year >= 2028 and month == 9) { 
		vika.from_mother = 0;
	}
	vika.cash += vika.from_mother;
}


void vika_cash(const int year, int month)
{
	if (year == 2026 and month == 9) {
		vika.cash = 30000;
	}
}


void vika_food(const int year, int month)
{
	vika.cash -= vika.food;
}


void vika_dorm(const int year, int month)
{
	if (year == 2028 and month == 9) { //leave dorm
		vika.dorm = 0;
	}
	vika.cash -= vika.dorm;
}

void vika_metro(const int year, int month)
{
	vika.cash -= vika.metro;
}

void vika_rzd(const int year, int month)
{
	if (((year == 2026 || year == 2027) &&
		(month == 2 || month == 7 || month == 12)) ||
		(year == 2028 && (month == 2 || month == 7)))
	{
		vika.cash -= vika.rzd;
	}
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
	if (year == 2028 and month == 9) { //patriot mod
		vika.deutsch = 0;
		}

	vika.cash -= vika.deutsch;
}

void vika_therapist(const int year, int month)
{
	if (year == 2028 and month == 9) { //minus kukuha
		vika.therapist = 0;
	}
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


void vika_rent(const int year, int month)
{
	if (year == 2028 and month == 9) { //leave dorm
		vika.rent = 30'000;
		}
	vika.cash -= vika.rent;
}


void vika_inflation(const int year, const int month)
{
	if (year >= 2027 and month == 1) {
		vika.food *= 1 + vika.inflation * vika.basic_infl_coeff;
		vika.dorm *= 1 + vika.inflation * vika.service_infl_coeff;
		vika.metro *= 1 + vika.inflation;
		vika.rzd *= 1 + vika.inflation;
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
		vika.inflation = 0.04 + 0.01*(-1 + rand() % (1 + 1 + 1));
	}


}


void vika_buy_apartment(const int year, const int month)
{
	if (vika.mortgage_active) {
		return;
	}

	if (year == 2034 and month == 1) {
			vika.savings_account -= vika.down_payment;
			vika.mortgage = vika.apartment_price - vika.down_payment;
			vika.mortgage_active = true;
		
	}
}


void vika_mortgage(const int year, const int month)
{
	if (!vika.mortgage_active) {
		return;
	}

	if (vika.mortgage <= 0) {
		vika.mortgage_active = false;
		return;
	}

	float monthly_rate = vika.mortgage_rate / 12;

	vika.mortgage_interest = vika.mortgage * monthly_rate;

	vika.mortgage_principal = vika.mortgage_payment - vika.mortgage_interest;

	vika.mortgage -= vika.mortgage_principal;

	vika.cash -= vika.mortgage_payment;

	vika.mortgage_months--;
}


void simulation()
{
	int year = 2026;
	int month = 9;

	while (not(year == 2044 and month == 12)) {

		vika_apply_inflation(year, month);
		vika_inflation(year, month);
		vika_salary(year, month);
		vika_rent(year, month);
		vika_cash(year, month);
		vika_from_mother(year, month);
		vika_food(year, month);
		vika_dorm(year, month);
		vika_metro(year, month);
		vika_rzd(year, month);
		vika_merchandise(year, month);
		vika_medicine(year, month);
		vika_clothes(year, month);
		vika_deutsch(year, month);
		vika_therapist(year, month);
		vika_presents(year, month);
		vika_household(year, month);
		vika_buy_apartment(year, month);
		vika_mortgage(year, month);
		vika_savings_account(year, month);
		
			

		std::cout << year << "." << month
			<< " cash = " << vika.cash
			<< " savings = " << vika.savings_account
			<< " salary = " << vika.salary
			<< '\n';

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

