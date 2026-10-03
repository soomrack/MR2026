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


void rybka_part_time_jop(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.part_time_jop = 20000;
	}


	rybka.cash += rybka.part_time_jop;
}


void rybka_scholarship(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.scholarship = 5000;
	}


	rybka.cash += rybka.scholarship;
}


void rybka_quarterly_bonys(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.quarterly_bonys = 50000;
	}


	rybka.cash += rybka.quarterly_bonys;
}


void rybka_cashback(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.cashback = 1000;
	}


	rybka.cash += rybka.cashback;
}


void rybka_gifts(const int year, const int month)
{
	if (month % 6 == 0) { //Promotion
		rybka.gifts = 50000;
	}


	rybka.cash += rybka.gifts;
}


void rybka_13_th_salary(const int year, const int month)
{
	if (month % 12 == 0) { //Promotion
		rybka.13_th_salary = 60000;
	}


	rybka.cash += rybka.13_th_salary;
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


void rybka_internet(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.internet = 2000;
	}


	rybka.cash -= rybka.internet;
}


void rybka_mobile_phone(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.mobile_phone = 700;
	}


	rybka.cash -= rybka.mobile_phone;
}


void rybka_subscriptione(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.subscriptione = 1500;
	}


	rybka.cash -= rybka.subscriptione;
}


void rybka_rocket_money(const int year, const int month)
{
	if (year == 2026 and month == 11) { //Promotion
		rybka.rocket_money = 10000;
	}


	rybka.cash -= rybka.rocket_money;
}


void rybka_gym(const int year, const int month)
{
	if (month % 12 == 0) { //Promotion
		rybka.gym =30000;
	}


	rybka.cash -= rybka.gym;
}


void rybka_car_insurance(const int year, const int month)
{
	if (month % 12 == 0) { //Promotion
		rybka.car_insurance =150000;
	}


	rybka.cash -= rybka.car_insurance;
}


void rybka_dantist(const int year, const int month)
{
	if (month % 4 == 0) { //Promotion
		rybka.dantist =10000;
	}


	rybka.cash -= rybka.dantist;
}


void rybka_checup(const int year, const int month)
{
	if (month % 4 == 0) { //Promotion
		rybka.checup =20000;
	}


	rybka.cash -= rybka.checup;
}


void rybka_clothes_shoes(const int year, const int month)
{
	if (month % 4 == 0) { //Promotion
		rybka.clothes_shoes =12000;
	}


	rybka.cash -= rybka.clothes_shoes;
}


void rybka_travel(const int year, const int month)
{
	if (month % 12 == 0) { //Promotion
		rybka.clothes_travel =100000;
	}


	rybka.cash -= rybka.travel;
}


void rybka_tire_change(const int year, const int month)
{
	if (month % 6 == 0) { //Promotion
		rybka.tire_change =5000;
	}


	rybka.cash -= rybka.tire_change;
}


void rybka_education(const int year, const int month)
{
	if (month % 6 == 0) { //Promotion
		rybka.education =130000;
	}


	rybka.cash -= rybka.education;
}


void rybka_charity(const int year, const int month)
{
	if (month % 6 == 0) { //Promotion
		rybka.charity =4000;
	}


	rybka.cash -= rybka.charity;
}



void rybka_car_maintenance(const int year, const int month)
{
	if (month % 6 == 0) { //Promotion
		rybka.car_maintenance =25000;
	}


	rybka.cash -= rybka.car_maintenance;
}


void rybka_pet_food(const int year, const int month)
{
	if (month % 2 == 0) { //Promotion
		rybka.pet_food =7000;
	}


	rybka.cash -= rybka.pet_food;
}


void rybka_haircut(const int year, const int month)
{
	if (month % 3 == 0) { //Promotion
		rybka.haircut =2000;
	}


	rybka.cash -= rybka.haircut;
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
		// rybka_car();
		//rybka_public_transport();
		//rybka_part_time_jop();
		//rybka_cashback();
		//quarterly_bonys();
		//rybka_gifts();
		//rybka_13_th_salary();
		//rybka_internet();
		//rybka_mobile_phone();
		//rybka_subscriptione();
		//rybka_rocket_money();
		//rybka_gym();
		//rybka_car_insurance();
		//rybka_dantist();
		//rybka_checup();
		//rybka_clothes_shoes();
		//rybka_travel();
		//rybka_tire_change();
		//rybka_education();
		//rybka_pet_food();
		//rybka_charity();
		//rybka_car_maintenance();
		//rybka_haircut();


		
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
