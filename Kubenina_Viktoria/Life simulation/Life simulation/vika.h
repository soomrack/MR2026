#pragma once

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>


using RUB = float;


struct Person {
	int health;
	bool sick;
	const char* last_health_damage_source;
	int mental;
	const char* last_mental_damage_source;
	bool romance_active;
	float romance_chance;
	bool break_up;
	int no_romance;

	RUB cash;
	RUB salary;
	bool second_job;
	bool on_vacation;
	bool unemployed;
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

	RUB drawing_furry;
	int artist_reputation;
	int tiktok_followers;
	int tg_followers;
	int monthly_posts;
	RUB advertising_cost;
};

extern Person vika;

void vika_init();
void vika_print();

void vika_damage_mental(int damage, const char* source);
void vika_bonus_mental(int bonus);
void vika_damage_heals(int damage, const char* source);
void get_cold(const int year, int month);
void vika_sick_day();

void vika_romance_chance();
void vika_find_romance(const int year, int month);
void vika_romance(const int year, int month);

void vika_salary(const int year, int month);
void vika_find_work(const int year, int month);
void vika_second_job();
void vika_vacation(const int year, int month);

void vika_savings_account(const int year, int month);
void vika_from_mother(const int year, int month);
void vika_cash(const int year, int month);

void vika_drawing(const int year, int month);
void unemployed_artist(const int year, int month);
void vika_social_media(const int year, int month);

void vika_in_hometown(const int year, int month);
void vika_vacation_hometown(const int year, int month);
void vika_go_home(const int year, int month);
void vika_go_StPb(const int year, int month);

void vika_food(const int year, int month);
void vika_dorm(const int year, int month);
void vika_metro(const int year, int month);
void vika_rzd(const int year, int month);
void vika_merchandise(const int year, int month);
void vika_medicine(const int year, int month);
void vika_clothes(const int year, int month);
void vika_deutsch(const int year, int month);
void vika_therapist(const int year, int month);
void vika_presents(const int year, int month);
void vika_household(const int year, int month);
void vika_rent(const int year, int month);

void vika_inflation(const int year, const int month);
void vika_apply_inflation(const int year, const int month);

void vika_buy_apartment(const int year, const int month);
void vika_mortgage(const int year, const int month);

void simulation();