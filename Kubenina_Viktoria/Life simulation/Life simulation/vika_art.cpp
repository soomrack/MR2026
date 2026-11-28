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


void vika_unemployed_artist(const int year, const int month) {
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
	
	bool busy = vika.on_vacation or vika.sick or vika.second_job;

	if (vika.engineer_career) {
		if (year > 2028 or (year == 2028 and month >= 8)) {
			vika.monthly_posts = busy ? 1 : 5;
			vika.art_time_available = busy ? 0 : 10;
		}
	}
	else {
		if (year > 2026 or (year == 2026 and month >= 8)) {
			vika.monthly_posts = busy ? 5 : 20;
			vika.art_time_available = busy ? 5 : 20;
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
	if (vika.artist_reputation <= 0) vika.artist_reputation = 0;
	if (vika.customer_chance > 100) vika.customer_chance = 100;
	if (vika.customer_chance <= 0) vika.customer_chance = 0;

	if (year >= 2028) {
		std::cout << "	tt followers = " << vika.tiktok_followers << " reputation = " << vika.artist_reputation << " posts = " << vika.monthly_posts << '\n';
	}
}


float customer_price[5] = {
	1.0,  // обычный
	1.1,  // постоянник
	1.4,  // ценитель
	1.0,  // афигевший
	1.5   // срочный
};

int customer_correction_bonus[5] = {
	0,  // обычный
	-5, // постоянник
	0,  // ценитель
	25, // афигевший
	10  // срочный
};

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

float correction_time[3][3] = {
	{0.5, 0.7, 1.0},
	{0.7, 1.0, 1.5},
	{1.0, 1.5, 2.0}
};

std::vector<Art_order> vika_orders;

int vika_choose_customer_type() {
	int chance = rand() % 100;

	if (chance < 50) return 0;
	if (chance < 70) return 1;
	if (chance < 80) return 2;
	if (chance < 95) return 3;
	return 4;
}

void vika_find_art_customer() {
	if (rand() % 100 >= vika.customer_chance) return;

	Art_order order;

	order.art_type = rand() % 3;
	order.art_size = rand() % 3;

	order.required_time = art_time[order.art_type][order.art_size];

	order.customer_type = vika_choose_customer_type();

	order.price = 0.01 * vika.artist_reputation * art_coefficients[order.art_type][order.art_size] * vika.drawing_furry;
	order.price *= customer_price[order.customer_type];

	order.corrections = 0;
	order.months_waiting = 0;

	order.deadline = 2;

	float current_load;
	vikas_load(current_load);

	float max_load = vika.art_time_available * 2;

	if (current_load + order.required_time <= max_load) vika_orders.push_back(order);
}


void vikas_load(float& time) {
	for (int i = 0; i < vika_orders.size(); i++) {
		time += vika_orders[i].required_time;
	}
}


void vika_orders_correction() {

    for (int i = 0; i < vika_orders.size(); i++) {
        int chance = correction_chance[vika_orders[i].art_type][vika_orders[i].art_size] + customer_correction_bonus[vika_orders[i].customer_type];

        if (vika_orders[i].corrections >= 2) continue;

        if (rand() % 100 < chance) {

            vika_orders[i].corrections++;
			vika_orders[i].required_time += correction_time[vika_orders[i].art_type][vika_orders[i].art_size];
            vika_orders[i].price *= 2 + correction_price[vika_orders[i].art_type][vika_orders[i].art_size];
        }
    }
}


void vika_orders_check_deadline() {

	for (int i = 0; i < vika_orders.size(); i++) {

		vika_orders[i].months_waiting++;

		if (vika_orders[i].months_waiting > vika_orders[i].deadline) {
			vika.artist_reputation -= 0.2;
		}
	}
}


void vika_orders_cancellation() {

	int i = 0;

	while (i < vika_orders.size()) {

		if (vika_orders[i].months_waiting > vika_orders[i].deadline + 1) {

			if (rand() % 100 < 30) {

				std::cout << "customer cancelled order " << '\n';
				vika.artist_reputation -= 0.5;

				vika_orders.erase(vika_orders.begin() + i);
				continue;
			}
		}

		i++;
	}
}


void vika_orders_process_time() {

	for (int i = 0; i < vika_orders.size(); i++) {
		if (vika.art_time_available <= 0) break;

		if (vika.art_time_available >= vika_orders[i].required_time) {

			vika.art_time_available -= vika_orders[i].required_time;
			vika_orders[i].required_time = 0;
		}
		else {
			vika_orders[i].required_time -= vika.art_time_available;
			vika.art_time_available = 0;
		}
	}
}


void vika_get_order_money() {

	int i = 0;

	while (i < vika_orders.size()) {

		if (vika_orders[i].required_time <= 0) {

			vika.cash += vika_orders[i].price;
			vika.monthly_art_income += vika_orders[i].price;
			vika_bonus_mental(5);

			vika.artist_reputation += 0.1;

			std::cout << '\n' << " order completed     +" << vika_orders[i].price << '\n';

			vika_orders.erase(vika_orders.begin() + i);
		}
		else {
			i++;
		}
	}
}


void vika_monthly_orders(const int year, const int month) {

	if (!vika.artist_business) return;

	if (vika.engineer_career and year <= 2028) return;
	vika_orders_check_deadline();
	vika_orders_correction();
	vika_orders_process_time();
	vika_get_order_money();
	vika_orders_cancellation();

	for (int i = 0; i < 10; i++) {
		vika_find_art_customer();
	}
}


const char* merch_names[4] = { "badge", "keychain", "postcard", "sticker" };

RUB merch_base_cost[4] = {
	35,     // значок
	90,     // брелок
	20,     // открытка
	12      // наклейка
};
float merch_base_price[4] = { 120, 300, 80, 50 };

float merch_quality_cost[3] = { 1, 1.25, 1.6 };

int merch_quality_bonus[3] = { 0, 5, 10 };


Manufacturer merch_manufacturers[3] = {
	{ "cheap_print",  
	50,              //минимальный тираж
	0,               //минимальное качество
	1,               //ценовой коэффициент
	2,               //время производства
	12 },            //шанс брака

	{ "norm_print", 30, 1, 1.3, 1, 5 },

	{ "lega_print", 20, 2, 1.8, 2, 1 }
};


std::vector<Merch_design> vika_merch_designs;

void vika_merch_design() {
	if (rand() % 100 >= 40) return;

	Merch_design design;

	design.type = rand() % 4;

	design.quality = 0;
	if (vika.artist_reputation >= 20) design.quality = 1;
	if (vika.artist_reputation >= 50) design.quality = 2;

	int required_time = design.quality + 1;

	if (vika.art_time_available < required_time) return;                                             

	vika.art_time_available -= required_time;

	design.times_produced = 0;

	vika_merch_designs.push_back(design);

	std::cout << "\n"  << " vika created a new merch design: " << merch_names[design.type] << " quality = " << design.quality << '\n';
}


std::vector<Merch_batch> vika_merch_batches;

void vika_making_merch() {
	if (rand() % 100 < 30) return;
	if (vika_merch_designs.empty()) return;

	Merch_design& design = vika_merch_designs[rand() % vika_merch_designs.size ()];

	Merch_batch batch;

	batch.type = design.type;
	batch.quality = design.quality;

	float wanted_merch = vika.tiktok_followers / 20 + vika.tg_followers / 10;
	batch.quantity = wanted_merch * 2;

	int the_best_manufactory;
	vika_find_manufacturer(batch.quantity, batch.quality, batch.type, the_best_manufactory);
	if (the_best_manufactory == -1) return;

	Manufacturer& manufacturer = merch_manufacturers[the_best_manufactory];

	batch.defect_chance = manufacturer.defect_chance;
	batch.months_remaining = manufacturer.production_time;
	batch.manufacturer_id = the_best_manufactory;

	batch.unit_cost = merch_base_price[batch.type] * merch_quality_cost[batch.quality] * manufacturer.cost_coeff;
	batch.production_cost = batch.quantity * batch.unit_cost;
	batch.sale_price = merch_base_price[batch.type] * (1 + batch.quality * 0.1);

	if (vika.cash < batch.production_cost) return;

	batch.stock = 0;
	batch.online_stock = 0;
	batch.craftiko_stock = 0;
	batch.market_stock = 0;
	batch.delivered = false;

	vika.cash -= batch.production_cost;

	vika_merch_batches.push_back(batch);
	design.times_produced++;
	std::cout << "\n" << " vika ordered merch: " << merch_names[batch.type]  << '\n';
}


void vika_find_manufacturer(int quantity, int quality, int type, int& best_manufacturer) {
	best_manufacturer = -1;
	float best_cost = 1000000000;

	for (int i = 0; i < 3; i++) {
		if (quantity < merch_manufacturers[i].min_quantity) continue;
		if (quality < merch_manufacturers[i].min_quality) continue;

		float unit_cost = merch_base_cost[type] * merch_quality_cost[quality] * merch_manufacturers[i].cost_coeff;
		float total_cost = unit_cost * quantity;

		if (total_cost < best_cost) {
			best_cost = total_cost;
			best_manufacturer = i;
		}
	}
}


void vika_merch_production_process() {
	for (int i = 0; i < vika_merch_batches.size(); i++) {
		if (vika_merch_batches[i].delivered) continue;

		if (rand() % 100 < 10) {
			std::cout << " merch production delayed\n";
			continue;
		}

		vika_merch_batches[i].months_remaining--;
	}
}


void vika_merch_storage() {
	for (int i = 0; i < vika_merch_batches.size(); i++) {
		Merch_batch& batch = vika_merch_batches[i];

		if (batch.delivered) continue;
		if (batch.months_remaining > 0) continue;

		int good_products = 0;

		for (int j = 0; j < batch.quantity; j++) {
			if (rand() % 100 >= batch.defect_chance) {
				good_products++;
			}
		}

		batch.stock = good_products;
		batch.online_stock = good_products * 60 / 100;
		batch.craftiko_stock = good_products * 25 / 100;
		batch.market_stock = good_products - batch.online_stock - batch.craftiko_stock;

		batch.delivered = true;

		if (good_products > 0) {
			batch.unit_cost = batch.production_cost / good_products;
		}

		std::cout << "\n" << " merch arrived: " << merch_names[batch.type] << " ordered = " << batch.quantity << " good = " << good_products << '\n';
	}
}


void vika_merch_sales(const int year, const int month) {
	for (int i = 0; i < vika_merch_batches.size(); i++) {
		Merch_batch& batch = vika_merch_batches[i];

		if (!batch.delivered) continue;
		if (batch.stock <= 0) continue;

		int online_chance = 5 + vika.tiktok_followers / 100 + vika.tg_followers / 50 + vika.artist_reputation / 2 + merch_quality_bonus[batch.quality];

		int craftiko_chance = 10 + merch_quality_bonus[batch.quality];
		int market_chance = 35 + merch_quality_bonus[batch.quality];

		if (online_chance > 70) online_chance = 70;
		if (craftiko_chance > 100) craftiko_chance = 100;
		if (market_chance > 100) market_chance = 100;

		vika_merch_sell(batch, batch.online_stock, online_chance, 0, "online");
		vika_merch_sell(batch, batch.craftiko_stock, craftiko_chance, 0.3, "Craftiko");

		if (vika.market_active) {
			vika_merch_sell(batch, batch.market_stock, market_chance, 0, "market");
		}
	}

	if (vika.market_active) {
		std::cout << "vika participated in a market!!\n";
	}
}


void vika_merch_sell(Merch_batch& batch, int& channel_stock, int chance, float commission, const char* channel) {
	int sold = 0;

	for (int i = 0; i < channel_stock; i++) {
		if (rand() % 100 < chance) {
			sold++;
		}
	}

	channel_stock -= sold;
	batch.stock -= sold;

	float income = sold * batch.sale_price * (1 - commission);
	vika.cash += income;
	vika.monthly_merch_income += income;

	if (sold > 0) {
		std::cout << merch_names[batch.type] << " sold through " << channel << ": " << sold << " income = " << income << '\n';
	}
}


void vika_merch_print_stock() {
	std::cout << "\n  alo chto s merchom  \n";

	for (int i = 0; i < vika_merch_batches.size(); i++) {
		Merch_batch& batch = vika_merch_batches[i];

		std::cout << merch_names[batch.type] << '\n';

		if (!batch.delivered) {
			std::cout << " vprocesse, at least " << batch.months_remaining << " months \n";
		}
		else {
			std::cout << " all = " << batch.stock << '\n';
			std::cout << " online = " << batch.online_stock << '\n';
			std::cout << " Craftiko = " << batch.craftiko_stock << '\n';
			std::cout << " market = " << batch.market_stock << '\n';
		}
	}
}


void vika_monthly_merch(const int year, const int month) {
	if (!vika.artist_business) return;
	if (vika.engineer_career and year <= 2028) return;

	vika_merch_design();
	vika_merch_production_process();
	vika_merch_storage();
	vika_merch_sales(year, month);
	vika_making_merch();
}


void vika_try_to_artmarket(const int year, const int month) {
	if (vika.artist_reputation < 20 and rand() % 100 > 30) return;
	if (vika.artist_reputation >= 20 and rand() % 100 > 80) return;
	if (vika.market_count > 4) return;

	int months_passed = (year - vika.last_market_year) * 12 + (month - vika.last_market_month);
	if (months_passed < 3) return;

	if (month == 1) vika.market_count = 0;

	vika.market_active = true;
	std::cout << "vika participated in a market!!\n";
	vika_bonus_mental(10);
	vika.market_count++;
	vika.last_market_year = year;
	vika.last_market_month = month;
}


void vika_design_to_artmarket() {
	if (!vika.market_active) return;

	Merch_design design;

	design.type = rand() % 4;
	design.quality = 1;

	int required_time = design.quality + 1;
	vika.art_time_available -= required_time;

	design.times_produced = 0;

	vika_merch_designs.push_back(design);
}


std::vector<Merch_batch> vika_merch_to_market_batches;

void vika_merch_to_artmarket(int i, int quantity) {
	
	Merch_design& design = vika_merch_designs[i];
	Merch_batch batch;

	batch.type = design.type;
	batch.quality = design.quality;
	batch.quantity = quantity;

	int the_best_manufactory;
	vika_find_manufacturer(batch.quantity, batch.quality, batch.type, the_best_manufactory);
	if (the_best_manufactory == -1) return;

	Manufacturer& manufacturer = merch_manufacturers[the_best_manufactory];

	batch.defect_chance = manufacturer.defect_chance;
	batch.months_remaining = manufacturer.production_time;
	batch.manufacturer_id = the_best_manufactory;

	batch.unit_cost = merch_base_price[batch.type] * merch_quality_cost[batch.quality] * manufacturer.cost_coeff;
	batch.production_cost = batch.quantity * batch.unit_cost;
	batch.sale_price = merch_base_price[batch.type] * (1 + batch.quality * 0.1);

	if (vika.cash < batch.production_cost) return;

	batch.stock = 0;
	batch.online_stock = 0;
	batch.craftiko_stock = 0;
	batch.market_stock = 0;
	batch.delivered = false;

	vika.cash -= batch.production_cost;

	vika_merch_to_market_batches.push_back(batch);
	design.times_produced++;
}


void vika_merch_to_market_process() {
	for (int i = 0; i < vika_merch_to_market_batches.size(); i++) {
		if (vika_merch_to_market_batches[i].delivered) continue;

		if (rand() % 100 < 10) {
			std::cout << " merch production delayed\n";
			continue;
		}

		vika_merch_to_market_batches[i].months_remaining--;
	}
}


void vika_to_market_merch_storage() {
	for (int i = 0; i < vika_merch_to_market_batches.size(); i++) {
		Merch_batch& batch = vika_merch_to_market_batches[i];

		if (batch.delivered) continue;
		if (batch.months_remaining > 0) continue;

		int good_products = 0;

		for (int j = 0; j < batch.quantity; j++) {
			if (rand() % 100 >= batch.defect_chance) {
				good_products++;
			}
		}
		batch.delivered = true;

		if (good_products > 0) {
			batch.unit_cost = batch.production_cost / good_products;
		}
			
		int old_merch = 0;

		for (int i = 0; i < vika_merch_batches.size(); i++) {
			Merch_batch& batch_old = vika_merch_batches[i];
			old_merch += batch_old.market_stock;
		}

		vika.market_products = good_products + old_merch;
		std::cout << "\n" << " merch to market arrived: " << merch_names[batch.type] << " ordered = " << batch.quantity << " good = " << good_products << '\n';
	}
}


void vika_travel_to_artmarket() {
	if (rand() % 100 > 50) vika.cash -= 5000; //маркет в Мосвке
	vika.Moskow = true;
}


void vika_during_artmarket() {
	RUB dekor = 1000;
	RUB packaging = 300;
	RUB table_rental = 800;
	RUB eating = 600;

	if (vika.Moskow) {
		vika.cash -= 0.1 * dekor;
		vika.cash -= 0.1 * packaging;
		vika.cash -= 0.1 * table_rental;
		vika.cash -= 0.1 * eating;
	}
	else {
		vika.cash -= dekor;
		vika.cash -= packaging;
		vika.cash -= table_rental;
		vika.cash -= eating;
	}
}


void vika_sells_artmarket(int& products) {
	for (int i = 0; i < vika_merch_to_market_batches.size(); i++) {
		Merch_batch& batch = vika_merch_to_market_batches[i];
		int sold = 0;

		for (int i = 0; i < products; i++) {
			if (rand() % 100 < 70) {
				sold++;
				vika.sold_on_market++;
			}
		}

		products -= sold;

		float income = sold * batch.sale_price;
		vika.cash += income;
		vika.market_income += income;

		if (sold > 0) {
			std::cout << merch_names[batch.type] << " sold on market: " << sold << " income = " << income << '\n';
		}
	}
}


void vika_populirity() {
	for (int i = 0; i < vika.sold_on_market; i++) {
		if (rand() % 100 < 70) {
			vika.tg_followers++;
		}
		if (rand() % 100 < 40) {
			vika.tiktok_followers++;
		}
	}
}


void vika_markets (const int year, const int month) {
	vika_try_to_artmarket(year, month);
	for (int i = 0; i < 4; i++) {
		vika_design_to_artmarket();
		vika_merch_to_artmarket(i, 30);
	}
	vika_merch_to_market_process();
	vika_to_market_merch_storage();
	vika_travel_to_artmarket();
	vika_during_artmarket();
	vika_sells_artmarket(vika.market_products);
	vika_populirity();
	
	vika.market_active = false;
	vika.Moskow = false;
}