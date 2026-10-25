#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

#include "vika.h"

extern Person vika;

void vika_print()
{
	if (vika.mental > 0) {
		std::cout << std::fixed << std::setprecision(0);
		std::cout << "Vika cash = " << vika.cash << '\n';
		std::cout << "Vika savings = " << vika.savings_account << '\n';
		std::cout << "Vika salary = " << vika.salary << '\n';
		std::cout << "Vika health = " << vika.health << '\n';
		std::cout << "Vika mental = " << vika.mental << '\n';
	}

}

void vika_damage_mental(int damage, const char* source)
{
	vika.mental -= damage;
	vika.last_mental_damage_source = source;
}

void vika_bonus_mental(int bonus)
{
	vika.mental += bonus;
	
	if (vika.mental > 100) vika.mental = 100;
}

void vika_damage_heals(int damage, const char* source)
{
	vika.health -= damage;
}

void vika_romance_chance() 
{
	if (vika.mental < 70) {
		vika.romance_chance += 10;
	}
}

void vika_find_romance(const int year, int month)
{
	if (vika.romance_active or vika.in_hometown or vika.no_romance > 0) {
		return;
	}

	if (vika.break_up) {
		vika.no_romance = 8;
	}

	vika.no_romance -= 1;

	if (vika.no_romance < 0) {
		vika.no_romance = 0;
	}

	if (vika.no_romance > 0) {
		vika.break_up = false;
	}

	if (rand() % 100 < vika.romance_chance) {
		vika.romance_active = true;
		vika_bonus_mental(10);
		std::cout << '\n' << month << "/" << year << " vika start relationships " << '\n';
	}
}

void vika_romance(const int year, int month) {
	if (!vika.romance_active) {
		return;
	}

	vika_damage_mental(1, " dies from cringe (romance)");
	vika.expenses = 0.6;

	if (vika.mental < 50 and rand() % 100 < 20) {
		vika_damage_mental(5, " trigger to childhood");
	}

	if (rand() % 100 < 20) {
		vika.romance_active = false;
		vika.break_up = true;

		vika_damage_mental(10, "her boyfriend dumped her");
		vika.expenses = 1;
		std::cout << '\n' << month << "/" << year << " dorogaya mi rasstaemsa " << '\n';
	}

	if (vika.mental < 20) {
		vika.romance_active = false;
		vika.break_up = true;
		vika_bonus_mental(10);
		vika.expenses = 1;
		std::cout << '\n' << month << "/" << year << " dorogoi mi rasstaemsa " << '\n';
	}
}

void vika_get_cold(const int year, int month) 
{
	if (rand() % 100 < 0.2 * (100-vika.mental)) {
		vika.sick = true;
		vika_damage_heals(5, "get cold");
	}
}

void vika_sick_day()
{
	if (vika.sick) {
		std::cout << "vika get cold\n";
		vika.sick = false;
	}
}


void vika_salary(const int year, int month)
{
	if (vika.unemployed) {
		return;
	}

	if (!vika.unemployed and year == 2026 and month == 10) { //find work
		vika.salary = 30'000;
		vika.tax = vika.salary * 0.13;
	}

	if (!vika.unemployed and year == 2028 and month == 9) {
		vika.salary = 60000;
		vika.tax = vika.salary * 0.13;
	}

	if (!vika.unemployed and year >= 2028 and month == 1) { //promotion
		if (rand() % 100 < 30) {
			vika.salary += 3000 + rand() % 7001;
			vika.tax = vika.salary * 0.13;
		}
	}

	if (vika.sick) {
		vika.cash += (vika.salary - vika.tax) * 0.5;
		return;
	}
	else {
		vika.cash += vika.salary - vika.tax;
	}

	if (!vika.unemployed and rand() % 100 < 10) {
		vika.unemployed = true;
		vika_damage_mental(10, "vika was fired");
		vika.cash += 2 * vika.salary;
	}
}


void vika_vacation(const int year, int month)
{
	if (month == 8 and !vika.in_hometown and !vika.unemployed) {
		vika.on_vacation = true;
		vika_bonus_mental(10);
		vika.expenses = 0.5;
		std::cout << "vika went to hometown on vacation\n";
	}
	else {
		vika.on_vacation = false;
		vika.expenses = 1;
	}

	if (month == 12 and !vika.in_hometown and !vika.unemployed) {
		vika.on_vacation = true;
		vika_bonus_mental(10);
		vika.expenses = 1; //pay for presents
		std::cout << "vika went to neww year on vacation\n";
	}
	else {
		vika.on_vacation = false;
		vika.expenses = 1;
	}
}

void vika_find_work(const int year, int month)
{
	if (!vika.unemployed or vika.in_hometown) {
		return;
	}

	if (rand() % 100 < 60) {
		vika.unemployed = false;
		vika_bonus_mental(7);
	}
}

void vika_second_job() {
	if (vika.cash > 0) {
		vika.second_job = false;
	}
	if (vika.mental > 20 and !vika.in_hometown) {
		if (vika.cash < 0) {
			vika.second_job = true;
			vika.cash += 20'000;
			vika_damage_mental(5, "2nd work");
			std::cout << "vika go to 2nd work\n";
		}
	}

	if (vika.cash < 0) {
		vika.savings_account += vika.cash;
		vika.cash = 0;
		std::cout << " vika take money from savings_account AGAIN " << " = > " << " cash = " << vika.cash << '\n' << '\n';
	}
}

void vika_leave_second_job() {
	if (vika.second_job = true) {
		vika.second_job = false;
	}
}

void vika_savings_account(const int year, int month)
{
	if (month == 1) {
		vika.account_procent = vika.account_procent - 0.005;
		vika.savings_account += vika.savings_account * vika.account_procent;
	}

	if (year >= 2029 and vika.cash >= 43000) {
		vika.savings_account += 0.1 * vika.salary;
		vika.cash -= 0.1 * vika.salary;
	}


	if (year >= 2029 and vika.cash >= 63000) {
		vika.savings_account += vika.cash;
		vika.cash = 0;
	}


	if (!vika.in_hometown and vika.cash >= 20000) {
		vika.savings_account += 20000;
		vika.cash -= 20000;
	}
	
	if (vika.cash < 0) {
		std::cout << '\n' << month << "/" << year << " " << "cash = " << vika.cash << " => ";
		if (vika.savings_account > 1'200'000) {
			vika.savings_account -= 20000;
			vika.cash += 20000;
		}
		else {
			vika.savings_account -= 2000;
			vika.cash += 2000;
		}
		std::cout  << " vika take money from savings_account " << " => " << " cash = " << vika.cash << '\n' << '\n';
	}	
}

void vika_from_mother(const int year, int month)
{
	if (year >= 2028 and month == 9) { 
		vika.from_mother = 0;
	}
	vika.cash += vika.from_mother * vika.expenses;
}


void vika_cash(const int year, int month)
{
	if (year == 2026 and month == 9) {
		vika.cash = 30000;
	}
}


void vika_in_hometown(const int year, int month) {
	if (vika.in_hometown){
		vika.expenses = 0;
		vika_damage_mental(5, "the ancient forest is calling you (being home quite retraumatizing)");
	}
}

void vika_vacation_hometown(const int year, int month) {
	if (year == 2027 and (month == 7 || month == 8)) {
		vika.on_vacation = true; 
		vika.in_hometown = true;
		vika.unemployed = true;
	}
	if (year == 2027 and (month == 9)) {
		vika.on_vacation = false;
		vika.in_hometown = false;
		vika.unemployed = false;
		vika.expenses = 1;
		std::cout << '\n' << " end of vika's last vacantion(( " << '\n';
	}

}

void vika_go_home(const int year, int month)  // TODO: const
{
	if (vika.in_hometown) return;

	if (vika.cash < 0 and vika.unemployed) {
		vika.in_hometown = true;
		vika.expenses = 0;

		std::cout << " vika loh and return to Velsk\n";
	}
}


void vika_go_StPb(const int year, int month) {
	if (year == 2027 and (month == 7 or month == 8)) return;
	if (!vika.in_hometown) return;

	if (vika.cash < 50'000) {
		return;
	}

	vika.in_hometown = false;
	vika.expenses = 1;

	std::cout << " vika returned to StPb\n";
}


void vika_food(const int year, int month)
{
	vika.cash -= vika.food * vika.expenses;
}


void vika_dorm(const int year, int month)
{
	if (year == 2028 and month == 9) { //leave dorm
		vika.dorm = 0;
	}
	vika.cash -= vika.dorm * vika.expenses;
}

void vika_metro(const int year, int month)
{
	vika.cash -= vika.metro * vika.expenses;
}

void vika_rzd(const int year, int month)
{
	if (((year == 2026 or year == 2027) and
		(month == 2 or month == 7 or month == 12)) ||
		(year == 2028 and (month == 2 or month == 7)))
	{
		vika.cash -= vika.rzd * vika.expenses;
	}
}

void vika_merchandise(const int year, int month)
{
	vika.cash -= vika.merchandise * vika.expenses;
}

void vika_medicine(const int year, int month)
{
	vika.cash -= vika.medicine * vika.expenses;
}

void vika_clothes(const int year, int month)
{
	vika.cash -= vika.clothes * vika.expenses;
}

void vika_deutsch(const int year, int month)
{
	if (vika.cash < 18'000) {
		return;
	}
	vika.cash -= vika.deutsch * vika.expenses;
}

void vika_therapist(const int year, int month)
{
	if (vika.cash < 18'000) {
		std::cout << " net deneg na psyho\n";
		vika_damage_mental(5, " net deneg na psyho");
		return;
	}
	vika.cash -= vika.therapist * vika.expenses;
}


void vika_presents(const int year, int month)
{
	vika.cash -= vika.presents * vika.expenses;
}


void vika_household(const int year, int month)
{
	vika.cash -= vika.household * vika.expenses;
}


void vika_rent(const int year, int month)
{
	if (year == 2028 and month == 9) { //leave dorm
		vika.rent = 30'000;
		}

	if (vika.cash < 30'000) { //find Mitbewohnerin
		vika.rent = 15'000;
	}
	vika.cash -= vika.rent * vika.expenses;
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
		vika.down_payment *= 1 + vika.inflation * vika.luxury_infl_coeff;
		vika.apartment_price *= 1 + vika.inflation * vika.luxury_infl_coeff;
		vika.salary *= 1 + vika.inflation;
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

	if (vika.savings_account >= vika.down_payment and vika.salary > 120'000) {
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

	while (not(year == 2036 and month == 2)) {

		vika_apply_inflation(year, month);
		vika_inflation(year, month);
		vika_vacation_hometown(year, month);
		vika_get_cold(year, month);
		vika_salary(year, month);
		vika_vacation(year, month);
		vika_find_work(year, month);
		vika_cash(year, month);
		vika_from_mother(year, month);
		vika_go_home(year, month);
		unemployed_artist(year, month);
		vika_in_hometown(year, month);
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
		vika_rent(year, month);
		vika_romance_chance();
		vika_find_romance(year, month);
		vika_romance(year, month);

		vika_drawing(year, month);
		vika_savings_account(year, month);
		vika_second_job();

		vika_social_media(year, month);
		vika_buy_apartment(year, month);
		vika_mortgage(year, month);
		vika_go_StPb(year, month);
		vika_sick_day();
		vika_leave_second_job();

		
		if (vika.mental <= 0) {
			std::cout << '\n' << month << "/" << year << "vika is decomposing in the forest" << '\n' << "reason: " << vika.last_mental_damage_source << '\n';
			return;
		}

		if (vika.health <= 0) {
			std::cout << '\n' << month << "/" << year << "vika is dead" << '\n' << "reason: " << vika.last_health_damage_source << '\n';
			return;
		}


		std::cout << month << "/" << year << " Vika cash = " << vika.cash << " Vika savings = " << vika.savings_account << " Vika salary = " << vika.salary << '\n';
		
		
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
	srand(time(0));

	vika_init();

	simulation();

	vika_print();
}

