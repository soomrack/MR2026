#include "vika.h"

Person vika;

void vika_init()
{
	vika.health = 100;
	vika.sick = false;
	vika.mental = 100;
	vika.romance_active = false;
	vika.romance_chance = 1;
	vika.break_up = false;
	vika.no_romance = 0;

	vika.cash = 0;
	vika.salary = 0;
	vika.second_job = false;
	vika.on_vacation = false;
	vika.unemployed = false;
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

	vika.tiktok_followers = 100;
	vika.tg_followers = 10;
	vika.customer_chance = 20;
	vika.monthly_posts = 0;
	vika.drawing_furry = 30'000;
	vika.artist_reputation = 4;
}