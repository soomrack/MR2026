#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <random>
#include <vector>
#include <algorithm>

long long int total_expence_tasties = 0; // временная переменная для проверки случайности расходов на вкусняшки
long long int kom = 0;  // временная переменная для проверки расходов на коммунальные услуги

using RUB = unsigned long long int;

enum class Disease {
	Glisti,
	Infection,
	Heart,
	Teeth,
	Urinary,
	Kidney,
	Cancer
};

enum class CarBreakdown {
	Battery,
	Tire,
	Brake,
	Suspension,
	Alternator,
	Starter,
	Engine,
	Transmission
};

struct Cat {
	int age = 0;
	int health = 0;
	RUB food;
	std::vector<Disease> diseases;
};

struct Car {
	bool exists = false;
	int age = 0;
	int mileage = 0;
	int condition = 100;
	double fuel_consumption = 8.0;
	RUB fuel_price = 70;
	std::vector<CarBreakdown> breakdowns;  //создаём список поломок
	int horsepower = 110;
	int driving_experience = 0;
	int insurance_months_left = 0;
	int insurance_cases = 0;
	int insurance_class = 3;  //Класс КБМ
};

struct Person {
	RUB cash;
	RUB salary;
	struct Cat cat;
	struct Car car;
};

struct Habitation {
	RUB habit;
	RUB rent;
	RUB credit;
};

struct Expences {
	RUB food;
	RUB tasties;
	RUB fuel;
	RUB mobile;  //ещё не добавлен
	RUB internet;  //ещё не добавлен
	RUB clothes; //ещё не добавлен
	RUB fees; //ещё не добавлен
	RUB vacation;  //ещё не добавлен
	RUB tasties_price;
	struct Habitation habitation;
};

struct Person Glebas;
struct Expences expence;


int r100() {  //генерирует случайное число от 1 до 100
	static std::mt19937 gen(std::random_device{} ());
	static std::uniform_int_distribution<> dist(1, 100);
	return dist(gen);	
}

int r10000() {  //генерирует случайное число от 1 до 10000 для сравнения с долями процента
	static std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<> dist(1, 10'000);

	return dist(gen);
}

void Glebas_salary(const int year, const int month) {
	if (year == 2027 and month == 1) { //Promotion
		Glebas.salary = 100'000;
	}

	if (month == 1) {  //среднегодовой рост заработной платы
		Glebas.salary *= 1.05;
	}

	Glebas.cash += Glebas.salary; //+= прибавляет к самому себе значение

}

void ipoteka(const int year, const int month) { //траты на жильё, взятое в ипотеку + коммуналка
	struct ipoteka {
		RUB full_credit = 10'000'000; //размер ипотеки
		int percent = 13; //ставка ипотеки
		int period = 30; //срок ипотеки в годах
	};
	
	struct ipoteka i;
	int harea = 60; //площадь жилья в м2
		
	double p = i.percent / 12.0 / 100.0;  //ежемесячная процентная ставка
	double p1 = pow(1.0 + p, i.period * 12);
	expence.habitation.credit = (RUB)(i.full_credit * ((p * p1) / (p1 - 1.0)));  //ежемесячный платёж по ипотеке

	if (year == 2031 and month == 1) {  //берётся ипотека
		expence.habitation.habit = expence.habitation.credit;
	}
	
	if (year == 2031 + i.period and month == 1) {  //если ипотека выплачена
		expence.habitation.habit = 0;
	}

	//расходы на коммунальные услуги


	static std::mt19937 gen(std::random_device{} ());

	RUB hw_rate = 150;  //тарифы на коммуналку
	RUB cw_rate = 45;
	RUB elec_rate = 7;
	RUB security;
	RUB ren = 25; //кап ремонт тариф
	RUB heating = 2350;

	if (month == 1) {  //ежегодные/сезонные изменения
		std::uniform_int_distribution<> renovation(25, 50);
		ren = renovation(gen);
		hw_rate = (RUB)(150 * pow(1.04, year - 2031)); 
		cw_rate = (RUB)(45 * pow(1.04, year - 2031));
		elec_rate = (RUB)(7 * pow(1.01, year - 2031));
		heating = (RUB)(2195 * pow(1.05, year - 2031));
	}
	if (month == 10) {
		hw_rate = (RUB)(170 * pow(1.04, year - 2031));
		cw_rate = (RUB)(50 * pow(1.04, year - 2031));
		elec_rate = (RUB)(8 * pow(1.01, year - 2031));
	}
	if (month == 7) {
		heating = (RUB)(2350 * pow(1.05, year - 2031));
	}

	if (year == 2031 + i.period and month == 1) {  //страховка после выплаты ипотеки
		security = 500;
	}
	else {
		security = (RUB)(i.full_credit * 0.008 / 12);  //страховка, пока ипотека не закрыта
	}

	if (year >= 2031) {
		if (month >= 3 and month <=5) {  //если сейчас весна

			std::uniform_int_distribution<> cold_water_consumption(3, 4); //потреблениие воды в м3 в месяц
			std::uniform_int_distribution<> hot_water_consumption(4, 5);
			std::uniform_int_distribution<> electricity_consumption(75, 110); //расход электричекства в кВт*ч в месяц
			RUB cw = cold_water_consumption(gen) * cw_rate;
			RUB hw = hot_water_consumption(gen) * hw_rate;
			RUB e = electricity_consumption(gen) * elec_rate;

			if (month != 5) { //в среднем отопление отключают в мае
				Glebas.cash -= heating;
				kom += heating;
			}

			Glebas.cash -= (cw + hw + e + security + (ren * harea));
			kom += (cw + hw + e + security + (ren * harea));
		}
		else if (month >= 6 and month <= 8) {  //если сейчас лето

			std::uniform_int_distribution<> cold_water_consumption(3, 4);
			std::uniform_int_distribution<> hot_water_consumption(4, 5);
			std::uniform_int_distribution<> electricity_consumption(60, 95);
			RUB cw = cold_water_consumption(gen) * cw_rate;
			RUB hw = hot_water_consumption(gen) * hw_rate;
			RUB e = electricity_consumption(gen) * elec_rate;

			Glebas.cash -= (cw + hw + e + security + (ren * harea));
			kom += (cw + hw + e + security + (ren * harea));
		}
		 else if (month >= 9 and month <= 11) {  //если сейчас осень

			std::uniform_int_distribution<> cold_water_consumption(3, 4);
			std::uniform_int_distribution<> hot_water_consumption(4, 5);
			std::uniform_int_distribution<> electricity_consumption(75, 110);
			RUB cw = cold_water_consumption(gen) * cw_rate;
			RUB hw = hot_water_consumption(gen) * hw_rate;
			RUB e = electricity_consumption(gen) * elec_rate;

			if (month != 9) {  //в среднем отопление включают в октябре
				Glebas.cash -= heating;
				kom += heating;
			}

			Glebas.cash -= (cw + hw + e + security + (ren * harea));
			kom += (cw + hw + e + security + (ren * harea));
		}
		 else {  //если сейчас зима

			std::uniform_int_distribution<> cold_water_consumption(3, 4);
			std::uniform_int_distribution<> hot_water_consumption(4, 5);
			std::uniform_int_distribution<> electricity_consumption(85, 120);
			RUB cw = cold_water_consumption(gen) * cw_rate;
			RUB hw = hot_water_consumption(gen) * hw_rate;
			RUB e = electricity_consumption(gen) * elec_rate;

			Glebas.cash -= (cw + hw + e + security + (ren * harea) + heating);
			kom += (cw + hw + e + security + (ren * harea) + heating);
		}
	}
}

void Habitation(const int year, const int month) {  //жильё
	if (year == 2028 and month == 9) {  //переезд в съёмную квартиру
		expence.habitation.habit = expence.habitation.rent;
	}

	if (month == 1) {  // инфляция на жильё
		if (year > 2028 and year < 2031) {
			if (r100() < 50) {
				expence.habitation.rent = (RUB)(expence.habitation.rent * 1.02);
				expence.habitation.habit = expence.habitation.rent;
			}
			else {
				expence.habitation.rent = (RUB)(expence.habitation.rent * 0.98);
				expence.habitation.habit = expence.habitation.rent;
			}
		}
	}

	ipoteka(year, month);
}

void Tasties_expences(const int year, const int month) {  //Моделирует случайные расходы на еду
	std::unordered_set<int> days_31 = { 1, 3, 5, 7, 8, 10, 12 };  //месяц с 31 днями
	std::unordered_set<int> days_30 = { 4, 6, 9, 11 };
	if (not (month == 2 or days_30.count(month))) {
		for (int day = 1; day <= 31; ++day) {
			if (day == 1) {
				expence.tasties = 0;
			}
			if (r100() < 20) {
				expence.tasties += expence.tasties_price;
			}
		}

	}
	if (not (month == 2 or days_31.count(month))) {
		for (int day = 1; day <= 30; ++day) {
			if (day == 1) {
				expence.tasties = 0;
			}
			if (r100() < 20) {
				expence.tasties += expence.tasties_price;
			}
		}

	}
	if (month == 2) {
		for (int day = 1; day <= 28; ++day) {
			if (day == 1) {
				expence.tasties = 0;
			}
			if (r100() < 20) {
				expence.tasties += expence.tasties_price;
			}
		}
	}

}


void food(const int year, const int month) {  //еда
	expence.food = 11'000;
	expence.tasties_price = 300;

	if (month == 1) {
		expence.food = (RUB)(expence.food * 1.05);
		expence.tasties_price = (RUB)(expence.tasties_price * 1.05);
	}

	Tasties_expences(year, month);

}

void vet(const int year, const int month) {
	if (month == 1) {
		Glebas.cash -= 3000; //ежегодная вакцинация
	}


	if (Glebas.cat.age <= 5) {  //вероятность коту заболеть в разном возрасте
		if (r100() < 10) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Glisti) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Glisti);
			}
		}
		if (r100() < 20) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Infection) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Infection);
			}
		}
	}
	if (Glebas.cat.age > 5 and Glebas.cat.age <= 10) {
		if (r100() < 7) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Glisti) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Glisti);
			}
		}
		if (r100() < 10) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Infection) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Infection);
			}
		}
		if (r100() < 10) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Heart) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Heart);
			}
		}
		if (r100() < 10) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Teeth) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Teeth);
			}
		}
		if (r100() < 10) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Urinary) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Urinary);
			}
		}
	}
	if (Glebas.cat.age > 10 and Glebas.cat.age <= 15) {
		if (r100() < 5) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Glisti) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Glisti);
			}
		}
		if (r100() < 5) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Infection) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Infection);
			}
		}
		if (r100() < 15) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Heart) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Heart);
			}
		}
		if (r100() < 15) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Teeth) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Teeth);
			}
		}
		if (r100() < 20) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Urinary) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Urinary);
			}
		}
	}
	if (Glebas.cat.age > 15) {
		if (r100() < 5) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Cancer) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Cancer);
			}
		}
		if (r100() < 15) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Kidney) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Kidney);
			}
		}
		if (r100() < 20) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Heart) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Heart);
			}
		}
		if (r100() < 15) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Teeth) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Teeth);
			}
		}
		if (r100() < 20) {
			if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Urinary) == Glebas.cat.diseases.end()) {
				Glebas.cat.diseases.push_back(Disease::Urinary);
			}
		}
	}

	int treatment_cost = 0;

	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Glisti) != Glebas.cat.diseases.end()) {  //начало проверки налачия болезни и её лечения
		Glebas.cat.health -= 5; //сколько здоровья теряет питомец
		treatment_cost = 1000;  //стоимость лечения
		if (Glebas.cash >= treatment_cost) {  //болезнь лечится, если хватает денег
			Glebas.cash -= treatment_cost;
			Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Glisti), Glebas.cat.diseases.end());
		}
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Infection) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 10;
		treatment_cost = 3000;  
		if (Glebas.cash >= treatment_cost) {
			Glebas.cash -= treatment_cost;
			Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Infection), Glebas.cat.diseases.end());
		}
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Heart) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 20;
		treatment_cost = 8000;  
		if (Glebas.cash >= treatment_cost) {
			Glebas.cash -= treatment_cost;
			Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Heart), Glebas.cat.diseases.end());
		}
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Teeth) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 10;
		treatment_cost = 6000;  
		if (Glebas.cash >= treatment_cost) {
			Glebas.cash -= treatment_cost;
			Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Teeth), Glebas.cat.diseases.end());
		}
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Urinary) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 10;
		treatment_cost = 10'000;  
		if (Glebas.cash >= treatment_cost) {
			Glebas.cash -= treatment_cost;
			Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Urinary), Glebas.cat.diseases.end());
		}
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Kidney) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 15;
		treatment_cost = 10'000;  
		if (Glebas.cash >= treatment_cost) {
			Glebas.cash -= treatment_cost;
			Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Kidney), Glebas.cat.diseases.end());
		}
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Cancer) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 30;
		treatment_cost = 25'000;  
		if (Glebas.cash >= treatment_cost) {
			Glebas.cash -= treatment_cost;
			Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Cancer), Glebas.cat.diseases.end());
		}
	}

	Glebas.cat.health = std::clamp(Glebas.cat.health, 0, 100);  
}

void pet(const int year, const int month) {  
	Glebas.cat.health = std::clamp(Glebas.cat.health, 0, 100);  //задание границ здоровья кота

	if (year == 2030 and month == 3) {  //кот появляется в первый раз
		Glebas.cat.health += 100;
	}

	if (month == 1 and Glebas.cat.health != 0) {  //Возраст кота
		Glebas.cat.age += 1;
	}

	if (Glebas.cat.age <= 5 and month == 1) { //Вероятность смерти, не связанной с заболеванием
		if (r100() < 5) {
			Glebas.cat.health = 0;
		}
	}
	if (Glebas.cat.age > 5 and Glebas.cat.age <= 10 and month == 1) {
		int r = Glebas.cat.age - 5;
		if (r100() <= 5 + r) {
			Glebas.cat.health = 0;
		}
	}
	if (Glebas.cat.age > 10 and Glebas.cat.age <= 15 and month == 1) {
		int r = Glebas.cat.age - 10;
		if (r100() <= 10 + r * 2) {
			Glebas.cat.health = 0;
		}
	}
	if (Glebas.cat.age > 15 and Glebas.cat.age <= 20 and month == 1) {
		int r = Glebas.cat.age - 15;
		if (r100() <= 20 + r * 4) {
			Glebas.cat.health = 0;
		}
	}
	if (Glebas.cat.age > 20 and month == 1) {
		int r = Glebas.cat.age - 20;
		if (r100() <= 40 + r * 8) {
			Glebas.cat.health = 0;
		}
	}

	if (Glebas.cat.age <= 10) { //посещение ветеринара с периодичностью, зависящей от возраста кота
		if (month == 1) {
			vet(year, month);
		}
	}
	else {
		if (month == 1 or month == 6) {
			vet(year, month);
		}
	}

	Glebas.cat.food = 1500;

	if (Glebas.cat.health != 0) {  //пока кот жив
		Glebas.cash -= Glebas.cat.food;
	}

	if (Glebas.cat.health == 0 and year > 2030){
		if (r100() < 10) {
			Glebas.cat.health += 100;  //Появляется новый питомец
			Glebas.cat.age = 0;
		}
	}
}

int get_random_mileage(const int month) {  //задаём случайный пробег, в зависимости от времени года
	static std::mt19937 gen(std::random_device{}());

	int min_mileage;
	int max_mileage;

	if (month == 12 || month == 1 || month == 2) {
		min_mileage = 800;
		max_mileage = 1200;
	}
	else if (month >= 3 and month <= 5) {
		min_mileage = 600;
		max_mileage = 1000;
	}
	else if (month >= 6 and month <= 8) {
		min_mileage = 400;
		max_mileage = 800;
	}
	else {
		min_mileage = 600;
		max_mileage = 1000;
	}

	std::uniform_int_distribution<> monthly_mileage(min_mileage, max_mileage);

	return monthly_mileage(gen);
}

void car_service(const bool breakdowns_check) {  
	if (breakdowns_check) {  //пробуем чинить старые поломки
		for (auto it = Glebas.car.breakdowns.begin();it != Glebas.car.breakdowns.end(); ) {
			CarBreakdown breakdown = *it;

			RUB repair_cost = 0;
			int condition_recovery = 0;

			switch (breakdown) {

			case CarBreakdown::Battery:
				repair_cost = 8'000;
				condition_recovery = 2;
				break;

			case CarBreakdown::Tire:
				repair_cost = 6'000;
				condition_recovery = 3;
				break;

			case CarBreakdown::Brake:
				repair_cost = 15'000;
				condition_recovery = 5;
				break;

			case CarBreakdown::Suspension:
				repair_cost = 25'000;
				condition_recovery = 5;
				break;

			case CarBreakdown::Alternator:
				repair_cost = 20'000;
				condition_recovery = 5;
				break;

			case CarBreakdown::Starter:
				repair_cost = 18'000;
				condition_recovery = 5;
				break;

			case CarBreakdown::Engine:
				repair_cost = 150'000;
				condition_recovery = 20;
				break;

			case CarBreakdown::Transmission:
				repair_cost = 120'000;
				condition_recovery = 15;
				break;
			}

			if (Glebas.cash >= repair_cost) {
				Glebas.cash -= repair_cost;

				Glebas.car.condition += condition_recovery;
				Glebas.car.condition =std::clamp(Glebas.car.condition, 0, 100);

				it = Glebas.car.breakdowns.erase(it);
			}
			else {
				++it;
			}
		}
	}
	else {
		int probability; //Задаём шанс поломки в целом в зависимости от состояния автомобиля
		if (Glebas.car.condition >= 90) {
			probability = 1;
		}
		else if (Glebas.car.condition >= 80) {
			probability = 1;
		}
		else if (Glebas.car.condition >= 70) {
			probability = 2;
		}
		else if (Glebas.car.condition >= 60) {
			probability = 3;
		}
		else if (Glebas.car.condition >= 50) {
			probability = 5;
		}
		else if (Glebas.car.condition >= 40) {
			probability = 8;
		}
		else if (Glebas.car.condition >= 30) {
			probability = 12;
		}
		else if (Glebas.car.condition >= 20) {
			probability = 18;
		}
		else if (Glebas.car.condition >= 10) {
			probability = 25;
		}
		else {
			probability = 35;
		}

		if (r10000() <= probability * 20) {  //если поломка произошла, определяем, какая именно (шанс поломки в принципе*шанс конкретной поломки)

			if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Battery) == Glebas.car.breakdowns.end()) {

				Glebas.car.breakdowns.push_back(CarBreakdown::Battery);
				Glebas.car.condition -= 2;
			}
		}
		if (r10000() <= probability * 20) {

			if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Tire) == Glebas.car.breakdowns.end()) {

				Glebas.car.breakdowns.push_back(CarBreakdown::Tire);
				Glebas.car.condition -= 3;
			}
		}
		if (r10000() <= probability * 15) {

			if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Brake) == Glebas.car.breakdowns.end()) {

				Glebas.car.breakdowns.push_back(CarBreakdown::Brake);
				Glebas.car.condition -= 5;
			}
		}
		if (r10000() <= probability * 15) {

			if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Suspension) == Glebas.car.breakdowns.end()) {

				Glebas.car.breakdowns.push_back(CarBreakdown::Suspension);
				Glebas.car.condition -= 7;
			}
		}
		if (r10000() <= probability * 10) {

			if (std::find(Glebas.car.breakdowns.begin(), Glebas.car.breakdowns.end(), CarBreakdown::Alternator) == Glebas.car.breakdowns.end()) {

				Glebas.car.breakdowns.push_back(CarBreakdown::Alternator);
				Glebas.car.condition -= 6;
			}
		}
		if (r10000() <= probability * 8) {

			if (std::find(Glebas.car.breakdowns.begin(), Glebas.car.breakdowns.end(), CarBreakdown::Starter) == Glebas.car.breakdowns.end()) {

				Glebas.car.breakdowns.push_back(CarBreakdown::Starter);
				Glebas.car.condition -= 6;
			}
		}
		if (r10000() <= probability * 7) {

			if (std::find(Glebas.car.breakdowns.begin(), Glebas.car.breakdowns.end(), CarBreakdown::Engine) == Glebas.car.breakdowns.end()) {

				Glebas.car.breakdowns.push_back(CarBreakdown::Engine);
				Glebas.car.condition -= 25;
			}
		}
		if (r10000() <= probability * 5) {

			if (std::find(Glebas.car.breakdowns.begin(), Glebas.car.breakdowns.end(), CarBreakdown::Transmission) == Glebas.car.breakdowns.end()) {

				Glebas.car.breakdowns.push_back(CarBreakdown::Transmission);
				Glebas.car.condition -= 20;
			}
		}

		Glebas.car.condition = std::clamp(Glebas.car.condition, 0, 100);
	}
}

void car_accident() {
	int probability;

	if (Glebas.car.driving_experience == 0) { // Вероятность ДТП в зависимости от водительского стажа
		probability = 110; // 1.10%
	}
	else if (Glebas.car.driving_experience <= 2) {
		probability = 80;  
	}
	else if (Glebas.car.driving_experience <= 5) {
		probability = 50;  
	}
	else if (Glebas.car.driving_experience <= 10) {
		probability = 40;  
	}
	else {
		probability = 35;  
	}

	if (r10000() <= probability) {

		int accident_type = r100(); // Определяем тяжесть ДТП
		int insurance_payment = 0;

		if (accident_type <= 70) { // Мелкое ДТП
			insurance_payment = 30'000;

			if (r100() <= 40) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Tire) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Tire);
					Glebas.car.condition -= 3;
				}
			}
			if (r100() <= 20) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Battery) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Battery);
					Glebas.car.condition -= 2;
				}
			}
		}
		else if (accident_type <= 95) {  //Среднее ДТП
			insurance_payment = 100'000;

			if (r100() <= 50) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Tire) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Tire);
					Glebas.car.condition -= 3;
				}
			}
			if (r100() <= 30) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Battery) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Battery);
					Glebas.car.condition -= 2;
				}
			}
			if (r100() <= 30) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Brake) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Brake);
					Glebas.car.condition -= 5;
				}
			}
			if (r100() <= 25) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Suspension) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Suspension);
					Glebas.car.condition -= 7;
				}
			}
			if (r100() <= 15) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Alternator) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Alternator);
					Glebas.car.condition -= 6;
				}
			}
			if (r100() <= 10) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Starter) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Starter);
					Glebas.car.condition -= 6;
				}
			}
		}
		else {  //Крупное ДТП
			insurance_payment = 200'000;

			if (r100() <= 60) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Tire) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Tire);
					Glebas.car.condition -= 3;
				}
			}
			if (r100() <= 40) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Battery) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Battery);
					Glebas.car.condition -= 2;
				}
			}
			if (r100() <= 50) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Brake) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Brake);
					Glebas.car.condition -= 5;
				}
			}
			if (r100() <= 60) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Suspension) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Suspension);
					Glebas.car.condition -= 7;
				}
			}
			if (r100() <= 30) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Alternator) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Alternator);
					Glebas.car.condition -= 6;
				}
			}
			if (r100() <= 25) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Starter) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Starter);
					Glebas.car.condition -= 6;
				}
			}
			if (r100() <= 20) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Engine) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Engine);
					Glebas.car.condition -= 25;
				}
			}
			if (r100() <= 15) {
				if (std::find(Glebas.car.breakdowns.begin(),Glebas.car.breakdowns.end(),CarBreakdown::Transmission) == Glebas.car.breakdowns.end()) {

					Glebas.car.breakdowns.push_back(CarBreakdown::Transmission);
					Glebas.car.condition -= 20;
				}
			}
		}
		Glebas.cash += insurance_payment;
		Glebas.car.insurance_cases++;
		Glebas.car.condition = std::clamp(Glebas.car.condition, 0, 100);
	}
}

double get_KVS(const int driving_experience) {
	if (driving_experience == 0) {
		return 1.67;
	}
	else if (driving_experience == 1) {
		return 1.55;
	}
	else if (driving_experience == 2) {
		return 1.53;
	}
	else if (driving_experience <= 4) {
		return 1.09;
	}
	else if (driving_experience <= 6) {
		return 1.07;
	}
	else if (driving_experience <= 9) {
		return 0.98;
	}
	else if (driving_experience <= 14) {
		return 0.94;
	}
	else {
		return 0.90;
	}
}

double get_KBM(const int month) {

	if (month == 4) {

		switch (Glebas.car.insurance_class) {

		case 14: // класс М
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 0;
			}
			break;

		case 0:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14;
			}
			break;

		case 1:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 2;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 2:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 3;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 3:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 4;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 4:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 5;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 2;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 5:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 6;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 3;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 6:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 7;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 4;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 2;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 7:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 8;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 4;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 2;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 8:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 9;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 5;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 2;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 9:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 10;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 5;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 2;
			}
			else if (Glebas.car.insurance_cases == 3) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 10:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 11;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 6;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 3;
			}
			else if (Glebas.car.insurance_cases == 3) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 11:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 12;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 6;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 3;
			}
			else if (Glebas.car.insurance_cases == 3) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 12:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 13;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 6;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 3;
			}
			else if (Glebas.car.insurance_cases == 3) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;

		case 13:
			if (Glebas.car.insurance_cases == 0) {
				Glebas.car.insurance_class = 13;
			}
			else if (Glebas.car.insurance_cases == 1) {
				Glebas.car.insurance_class = 7;
			}
			else if (Glebas.car.insurance_cases == 2) {
				Glebas.car.insurance_class = 3;
			}
			else if (Glebas.car.insurance_cases == 3) {
				Glebas.car.insurance_class = 1;
			}
			else {
				Glebas.car.insurance_class = 14; 
			}
			break;
		}

		Glebas.car.insurance_cases = 0;
	}

	double KBM;

	switch (Glebas.car.insurance_class) {

	case 14:
		KBM = 3.92; // класс М
		break;
	case 0:
		KBM = 2.94;
		break;
	case 1:
		KBM = 2.25;
		break;
	case 2:
		KBM = 1.76;
		break;
	case 3:
		KBM = 1.17;
		break;
	case 4:
		KBM = 1.00;
		break;
	case 5:
		KBM = 0.91;
		break;
	case 6:
		KBM = 0.83;
		break;
	case 7:
		KBM = 0.78;
		break;
	case 8:
		KBM = 0.74;
		break;
	case 9:
		KBM = 0.68;
		break;
	case 10:
		KBM = 0.63;
		break;
	case 11:
		KBM = 0.57;
		break;
	case 12:
		KBM = 0.52;
		break;
	case 13:
		KBM = 0.46;
		break;
	default:
		KBM = 1.00;
		break;
	}

	return KBM;
}

RUB get_insurance_cost(const double KBM) {
	int TB = 6'700; //базовый тариф ОСАГО
	double KVS = get_KVS(Glebas.car.driving_experience);
	double KT = 1.64;  //территориальный коэффициент в Санкт-Петербурге
	double KM;

	if (Glebas.car.horsepower <= 50) {
		KM = 0.6;
	}
	else if (Glebas.car.horsepower <= 70) {
		KM = 1.0;
	}
	else if (Glebas.car.horsepower <= 100) {
		KM = 1.1;
	}
	else if (Glebas.car.horsepower <= 120) {
		KM = 1.2;
	}
	else if (Glebas.car.horsepower <= 150) {
		KM = 1.4;
	}
	else {
		KM = 1.6;
	}

	return (RUB)(TB * KVS * KT * KBM * KM);
}

bool car_insurance(const double KBM) {  //оплата страховки
	if (Glebas.car.insurance_months_left == 0) {
		RUB insurance_cost = get_insurance_cost(KBM);

		if (Glebas.cash >= insurance_cost) {

			Glebas.cash -= insurance_cost;
			Glebas.car.insurance_months_left = 12;

			return true;
		}
		return false;
	}
	return true;
}

void car(const int year, const int month) {  
	if (!Glebas.car.exists) {
		if (year > 2030 and Glebas.cash >= 800'000) {
			Glebas.cash -= 800'000;

			Glebas.car.exists = true;
			Glebas.car.age = 5;
			Glebas.car.mileage = 90'000;
			Glebas.car.condition = 70;
			Glebas.car.fuel_consumption = 8.0;
			Glebas.car.driving_experience = 0;
			Glebas.car.insurance_months_left = 12;
		}
		else {
			return;
		}
	}

	if (month == 1) {
		Glebas.car.age += 1;
		Glebas.car.driving_experience += 1;
		Glebas.car.fuel_price *= 1.05;
	}
	
	//Страховка
	if (Glebas.car.insurance_months_left > 0) {  //Считаем срок действия страховки
		--Glebas.car.insurance_months_left;
	}

	double KBM = get_KBM(month);

	car_service(true);  //сначала чиним старые поломки

	if (not car_insurance(KBM)) {
		expence.fuel = 0;
		return;
	}

	if (Glebas.car.breakdowns.empty()) {
		int old_mileage = Glebas.car.mileage;

		int monthly_mileage = get_random_mileage(month);

		Glebas.car.mileage += monthly_mileage;

		Glebas.car.condition -= Glebas.car.mileage / 1000 - old_mileage / 1000;
		Glebas.car.condition = std::clamp(Glebas.car.condition, 0, 100);

		car_service(false);  //проверяем на наличие новых поломок

		if (Glebas.car.breakdowns.empty()) {
			car_accident();
		}

		double fuel_liters = monthly_mileage * Glebas.car.fuel_consumption / 100;
		expence.fuel = (RUB)(fuel_liters * Glebas.car.fuel_price);
	}
	else {
		expence.fuel = 0;
	}
}

void Glebas_spendings(const int year, const int month) {
	
	Glebas.cash -= expence.food;
	Glebas.cash -= expence.tasties;
	Glebas.cash -= expence.habitation.habit;
	total_expence_tasties += expence.tasties;
	Glebas.cash -= expence.fuel;
}

void simulation() {
	int year = 2026;
	int month = 9;
	while (not (year == 2036 and month == 9)) {
		
		Glebas_salary(year, month);
		food(year, month);
		Habitation(year, month);
		pet(year, month);
		car(year, month);
		Glebas_spendings(year, month);

		++month;
		if (month == 13) {
			++year;
			month = 1;
		}
	}

}

void Glebas_init() {
	Glebas.cash = 20'000;
	Glebas.salary = 80'000;
	expence.habitation.habit = 2860;
	expence.habitation.rent = 40'000;
}

void Glebas_print() {
	printf("Glebas cash = %lld\n", Glebas.cash);
	printf("Glebas salary = %lld\n", Glebas.salary);
	printf("tasty expences = %lld\n", total_expence_tasties);
	printf("house expences = %lld\n", expence.habitation.habit);
	printf("social service expences = %lld\n", kom);
	printf("Cat health = %d\n", Glebas.cat.health);
}

int main() {
	Glebas_init();

	simulation();

	Glebas_print();

}
