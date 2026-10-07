#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using RUB = float;


struct Person {
	int health;
	int mental;
	const char* last_mental_damage_source;
	bool romance_active;
	float romance_chance;

	RUB cash;
	RUB salary;
	bool unemployed;
	RUB from_mother;
	RUB drawing_furry;
	int artist_reputation;

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
	RUB expenses;
	bool in_hometown;

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
	vika.health = 100;
	vika.mental = 100;
	vika.romance_active = false;
	vika.romance_chance = 1;

	vika.cash = 0;
	vika.salary = 0;
	vika.unemployed = false;
	vika.from_mother = 25'000;
	vika.drawing_furry = 30'000;
	vika.artist_reputation = 4;

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
	vika.expenses = 1;
	vika.in_hometown = false;

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
	if (vika.mental > 0) {
		std::cout << std::fixed << std::setprecision(0);
		std::cout << "Vika cash = " << vika.cash << '\n';
		std::cout << "Vika savings = " << vika.savings_account << '\n';
		std::cout << "Vika salary = " << vika.salary << '\n';
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
}

void vika_damage_heals(int damage)
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
	if (vika.romance_active) {
		return;
	}

	if (vika.in_hometown) {
		return;
	}

	if (rand() % 100 < vika.romance_chance) {
		vika.romance_active = true;
		vika_bonus_mental(10);
		std::cout << '\n' << month << "/" << year << "vika start relationships " << '\n';
	}
}

void vika_romance(const int year, int month) {
	if (!vika.romance_active) {
		return;
	}
	vika_damage_mental(1, "dies from cringe (romance)");
	vika.expenses = 0.6;

	if (vika.mental < 50 and rand() % 100 < 20) {
		vika_damage_mental(5, "trigger to childhood");
	}

	if (rand() % 100 < 20) {
		vika.romance_active = false;
		std::cout << '\n' << month << "/" << year << "dorogaya mi rasstaemsa " << '\n';
	}

	if (vika.mental < 20) {
		vika.romance_active = false;
		std::cout << '\n' << month << "/" << year << "dorogoi mi rasstaemsa " << '\n';
	}
}


void vika_salary(const int year, int month)
{
	if (vika.unemployed) {
		return;
	}

	if (year == 2026 and month == 10) { //find work
		vika.salary = 30'000;
		vika.tax = vika.salary * 0.13;
	}

	if (year == 2028 and month == 9) {
		vika.salary = 60000;
		vika.tax = vika.salary * 0.13;
	}

	if (year >= 2028 and month == 1) { //promotion
		if (rand() % 100 < 30) {
			vika.salary += 3000 + rand() % 7001;
			vika.tax = vika.salary * 0.13;
		}
	}

	vika.cash += vika.salary-vika.tax;

	if (rand() % 100 < 10) {
		vika.unemployed = true;
		vika_damage_mental(10, "vika was fired");
		vika.cash += 2 * vika.salary;
	}
}

void vika_find_work(const int year, int month)
{
	if (!vika.unemployed) {
		return;
	}

	if (rand() % 100 < 60) {
		vika.unemployed = false;
	}
}

void vika_second_job() {
	if (vika.mental < 20) {
		return;
	}
	if (vika.cash < 0) {
		vika.cash += 20'000;
		vika_damage_mental(5, "2nd work");
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

	if (!vika.in_hometown and vika.cash >= 20000) {
		vika.savings_account += 20000;
		vika.cash -= 20000;
	}
	
	if (vika.cash < 0) {
		std::cout << '\n' << month << "/" << year << " " << "cash = " << vika.cash << " => ";
		vika.savings_account -= 2000;
		vika.cash += 2000;
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


void vika_drawing(const int year, int month) {
	if (vika.cash < 0 and vika.artist_reputation > 3) {
		std::cout << '\n' << month << "/" << year << " " << "vika find an art customer" << '\n';
		std::cout << "cash = " << vika.cash << " => ";
		float art_price = 0.01 * vika.artist_reputation * vika.drawing_furry;
		vika.cash += art_price;
		std::cout << " vika successfully sold the art at a price of " << art_price << " => " << " cash = " << vika.cash  << '\n';
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
		vika.in_hometown = true;
		vika.unemployed = true;
	}
	if (year == 2027 and (month == 9)) {
		vika.in_hometown = false;
		vika.unemployed = false;
		std::cout << '\n' << " end of vika's last vacantion(( " << '\n';
	}

}

void vika_go_home(const int year, int month)
{
	if (vika.in_hometown) {
		return;
	}

	if (vika.cash < 0 and vika.unemployed) {
		vika.in_hometown = true;
		vika.expenses = 0;

		std::cout << "vika loh and return to Velsk\n";
	}
}

void unemployed_arter(const int year, int month) {
	if (!vika.in_hometown and vika.unemployed) {
		return;
	}
	if (vika.artist_reputation > 3) {
		std::cout << '\n' << month << "/" << year << " " << "vika find an art customer" << '\n';
		std::cout << "cash = " << vika.cash << " => ";
		float art_price = 0.01 * vika.artist_reputation * vika.drawing_furry;
		vika.cash += art_price;
		std::cout << " vika successfully sold the art at a price of " << art_price << " => " << " cash = " << vika.cash << '\n';
	}
}


void vika_go_StPb(const int year, int month) {
	if (!vika.in_hometown) {
		return;
	}

	if (vika.cash < 50'000) {
		return;
	}

	vika.in_hometown = false;
	vika.expenses = 1;

	std::cout << "vika saved 50k and returned to StPb\n";
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
	if (((year == 2026 || year == 2027) and
		(month == 2 || month == 7 || month == 12)) ||
		(year == 2028 and (month == 2 || month == 7)))
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
		std::cout << "net deneg na psyho\n";
		vika_damage_mental(5, "net deneg na psyho");
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

	while (not(year == 2030 and month == 2)) {

		vika_apply_inflation(year, month);
		vika_inflation(year, month);
		vika_vacation_hometown(year, month);
		vika_in_hometown(year, month);
		vika_salary(year, month);
		vika_find_work(year, month);
		vika_go_home(year, month);
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
		vika_find_romance(year, month);
		vika_romance(year, month);
		vika_drawing(year, month);
		vika_savings_account(year, month);
		vika_second_job();
		vika_go_StPb(year, month);

		
		if (vika.mental <= 0) {
			std::cout << '\n' << month << "/" << year << "vika is decomposing in the forest" << '\n' << "reason: " << vika.last_mental_damage_source << '\n';
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

