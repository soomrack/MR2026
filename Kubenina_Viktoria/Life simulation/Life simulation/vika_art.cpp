#include "vika.h"
#include <vector>

extern Person vika;

void vika_drawing() {
	float art_price = 0.01 * vika.artist_reputation * vika.drawing_furry;
	vika.cash += art_price;
	vika_bonus_mental(5);
}

void vika_drawing_for_money(const int year, const int month) {
	if (vika.cash < 0 and rand() % 100 < vika.customer_chance) {
		std::cout << '\n' << month << "/" << year << " " << "vika find an art customer" << '\n';
		std::cout << "cash = " << vika.cash << " => ";
		float art_price = 0.01 * vika.artist_reputation * vika.drawing_furry;
		vika.cash += art_price;
		vika_bonus_mental(5);
		std::cout << " vika successfully sold the Art at a price of " << art_price << " => " << " cash = " << vika.cash << '\n';
	}
}


void unemployed_artist(const int year, const int month) {
	if (year == 2027 and (month == 7 or month == 8)) return;
	
	if (!vika.in_hometown and vika.unemployed) return;

	if (vika.in_hometown and vika.unemployed and rand() % 100 < vika.customer_chance) {
		std::cout << '\n' << month << "/" << year << " " << "vika find an art customer" << '\n';
		std::cout << "cash = " << vika.cash << " => ";
		float art_price = 0.01 * vika.artist_reputation * vika.drawing_furry;
		vika.cash += art_price;
		std::cout << " ViKa successfully Sold the art at a price of " << art_price << " => " << " cash = " << vika.cash << '\n';
	}
}


void vika_social_media(const int year, const int month) {
	
	if (vika.engineer_career) {
		if (year == 2028 and month >= 8) {
			if (vika.on_vacation or vika.sick or vika.second_job) vika.monthly_posts = 1;
			else vika.monthly_posts = 5;
		}

		if (year > 2028) {
			if (vika.on_vacation or vika.sick or vika.second_job) vika.monthly_posts = 1;
			else vika.monthly_posts = 5;
		}
	}
	else {
		if (year == 2026 and month >= 8) {
			if (vika.on_vacation or vika.sick or vika.second_job) vika.monthly_posts = 5;
			else vika.monthly_posts = 20;
		}

		if (year > 2026) {
			if (vika.on_vacation or vika.sick or vika.second_job) vika.monthly_posts = 5;
			else vika.monthly_posts = 20;
		}
	}
	

	int new_tiktok_followers = 0;

	if (vika.monthly_posts > 0) {
		for (int i = 0; i < vika.monthly_posts; i++) {
			if (rand() % 100 < 30 + vika.artist_reputation / 5) {
				new_tiktok_followers += 1 + rand() % 4;
			}
		}
	}
	vika.tiktok_followers += new_tiktok_followers;

	vika.tg_followers += rand() % (vika.tiktok_followers / 100 + 1);

	if (vika.tiktok_followers > 300) vika.artist_reputation += vika.tiktok_followers / 300;

	vika.customer_chance = vika.artist_reputation * 5;

	if (vika.tiktok_followers >= 1000) vika.customer_chance += 10;

	if (vika.artist_reputation > 100) vika.artist_reputation = 100;
	if (vika.customer_chance > 100) vika.customer_chance = 100;

	if (year >= 2028) {
		std::cout << "	tt followers = " << vika.tiktok_followers << " reputation = " << vika.artist_reputation << " posts = "
			<< vika.monthly_posts << '\n';
	}
}


float art_coefficients[3][3] = {
	{0.3, 0.45, 0.55}, // скетч
	{0.5, 0.6, 0.73}, // скетч с покрасом
	{0.6, 0.83, 1.0} // лайн с покрасом
};

int art_time[3][3] = {
	{1, 2, 3}, // скетч
	{2, 3, 4}, // скетч с покрасом
	{3, 4, 5}  // лайн с покрасом
};

int correction_chance[3][3] = {
	{10, 15, 20}, // скетч
	{20, 30, 40}, // скетч с покрасом
	{30, 40, 50}  // лайн с покрасом
};

float correction_price[3][3] = {
	{0.05, 0.07, 0.10},
	{0.10, 0.15, 0.20},
	{0.15, 0.20, 0.25}
};

std::vector<Art_order> vika_orders;

void vika_find_art_customer(const int year, const int month) {
	if (rand() % 100 >= vika.customer_chance) return;

	Art_order order;

	order.art_type = rand() % 3;
	order.art_size = rand() % 3;

	order.required_time = art_time[order.art_type][order.art_size];

	order.price = 0.01 * vika.artist_reputation * art_coefficients[order.art_type][order.art_size] * vika.drawing_furry;

	order.corrections = 0;

	if (vika.art_time_available + 5 >= order.required_time) {
		vika.art_time_available -= order.required_time;
		vika_orders.push_back(order);
	}
}

void vika_orders_correction() {
	for (int i = 0; i < vika_orders.size(); i++){
		int chance = correction_chance[vika_orders[i].art_type][vika_orders[i].art_size];

		if (rand() % 100 < chance)
		{
			vika_orders[i].corrections++;

			float increase = correction_price[vika_orders[i].art_type][vika_orders[i].art_size];

			vika_orders[i].price *= (1 + increase);
			vika_orders[i].required_time *= (1 + increase);
		}
	}
}

void vika_orders_process_time() {
	for (int i = 0; i < vika_orders.size(); i++) {
		if (vika.free_time <= 0) return;

		float process_time = vika.art_time_available - vika_orders[i].required_time;
		vika.art_time_available -= vika_orders[i].required_time;
		if (process_time < 0) vika_orders[i].required_time = -process_time;
		if (process_time >= 0) vika_orders[i].required_time = 0;
	}
}


void vika_get_order_money(const int year, const int month){
	for (int i = 0; i < vika_orders.size(); i++) {
		if (vika_orders[i].required_time <= 0) {

			vika.cash += vika_orders[i].price;
			vika.monthly_art_income += vika_orders[i].price;;

			std::cout << '\n' << month << "/" << year << " " << "vika get order money   +" << vika_orders[i].price << '\n';

			vika.artist_reputation += 0.1;
			std::cout << "reputation = " << vika.artist_reputation << '\n';

			vika_orders.erase(vika_orders.begin() + i);
		}
	}
}


void vika_monthly_orders(const int year, const int month) {
	if (vika.engineer_career) {
		if (year > 2028) {
			for (int i = 0; i < 10; i++) vika_find_art_customer(year, month);
			vika_orders_correction();
			vika_orders_process_time();
			vika_get_order_money(year, month);
		}
	}
	else {
		for (int i = 0; i < 10; i++) vika_find_art_customer(year, month);
		vika_orders_correction();
		vika_orders_process_time();
		vika_get_order_money(year,month);
	}
}