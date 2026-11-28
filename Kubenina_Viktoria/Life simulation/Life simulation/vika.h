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
	int free_time;
	bool romance_active;
	float romance_chance;
	bool break_up;
	int no_romance;

	bool engineer_career;
	bool artist_business;

	RUB cash;
	RUB salary;
	bool second_job;
	bool on_vacation;
	bool unemployed;
	RUB from_mother;

	RUB food;
	RUB cafe;
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
	int customer_chance;
	int art_time_available;
	float artist_reputation;
	int tiktok_followers;
	int tg_followers;
	int monthly_posts;
	RUB monthly_art_income;
	RUB monthly_merch_income;
	RUB market_income;
	int sold_on_market;
	RUB advertising_cost;
	bool market_active;
	int market_count;
	int last_market_month;
	int last_market_year;
	int market_products;
	bool Moskow;
};

extern Person vika;

struct Art_order {
	int art_type;
	int art_size;

	float required_time;
	float price;

	int corrections;
	int months_waiting;

	int customer_type;
	int deadline;
};


enum Merch_type {
	BADGE,       
	KEYCHAIN,    
	POSTCARD,    
	STICKER     
};

struct Merch_design {
	int type;
	int quality;          
	int times_produced;   
};

struct Manufacturer {
	const char* name;
	int min_quantity;
	int min_quality;
	float cost_coeff;     
	int production_time;  
	int defect_chance;   
};

struct Merch_batch {
	int design_id;
	int type;
	int quality;

	bool delivered;

	int quantity;         
	int stock;            
	int online_stock;
	int craftiko_stock;
	int market_stock;

	int months_remaining;
	int defect_chance;

	float production_cost;
	float sale_price;
	float unit_cost;

	int manufacturer_id;
};


void vika_init();
void vika_print();

void vika_damage_mental(int damage, const char* source);
void vika_bonus_mental(int bonus);
void vika_damage_heals(int damage, const char* source);
void vika_bonus_health(int bonus);
void vika_sleep_not_enough();
void vika_free_time();
void vika_get_cold(const int year, const int month);
void vika_sick_day();

void vika_romance_chance();
void vika_find_romance(const int year, int month);
void vika_romance(const int year, int month);

void vika_salary(const int year, int month);
void vika_find_work(const int year, int month);
void vika_second_job();
void vika_vacation(const int year, int month);
void vika_transfer();

void vika_savings_account(const int year, int month);
void vika_from_mother(const int year, int month);
void vika_cash(const int year, int month);

void vika_drawing_for_money(const int year, int month);
void vika_unemployed_artist(const int year, int month);
void vika_social_media(const int year, int month);

void vika_find_art_customer();
void vikas_load(float& time);
void vika_orders_check_deadline();
void vika_orders_correction();
void vika_orders_process_time();
void vika_orders_cancellation();
void vika_get_order_money();
void vika_monthly_orders(const int year, const int month);

void vika_merch_design();
void vika_making_merch();
void vika_find_manufacturer(int quantity, int quality, int type, int& best_manufacturer);
void vika_merch_production_process();
void vika_merch_storage();
void vika_merch_sales(const int year, const int month);
void vika_merch_sell(Merch_batch& batch, int& channel_stock, int chance, float commission, const char* channel);
void vika_merch_print_stock();
void vika_monthly_merch(const int year, const int month);

void vika_markets(const int year, const int month);
void vika_try_to_artmarket(const int year, const int month);
void vika_design_to_artmarket();
void vika_merch_to_artmarket(int i, int quantity);
void vika_merch_to_market_process();
void vika_to_market_merch_storage();


void vika_in_hometown(const int year, int month);
void vika_vacation_hometown(const int year, int month);
void vika_go_home(const int year, int month);
void vika_go_StPb(const int year, int month);

void vika_food(const int year, int month);
void vika_cafe(const int year, int month);
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