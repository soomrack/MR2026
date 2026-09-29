#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <random>
#include <vector>
#include <algorithm>

long long int total_expence_tasties = 0; // временна€ переменна€ дл€ проверки случайности расходов на вкусн€шки
long long int kom = 0;  // временна€ переменна€ дл€ проверки расходов на коммунальные услуги

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

struct cat {
	int age = 0;
	int health = 0;
	RUB food;
	std::vector<Disease> diseases;
};

struct Person {
	RUB cash;
	RUB salary;
	struct cat cat;
};

struct Habitation {
	RUB habit;
	RUB rent;
	RUB credit;
};

struct Expences {
	RUB food;
	RUB tasties;
	RUB transport;
	RUB mobile;
	RUB internet;
	RUB feast;
	RUB clothes;
	RUB fees;
	RUB holidays;
	RUB tasties_price;
	struct Habitation habitation;
};

struct Person Glebas;
struct Expences expence;


int r100() {
	static std::mt19937 gen(std::random_device{} ());
	static std::uniform_int_distribution<> dist(1, 100);
	return dist(gen);	
}

void Glebas_salary(const int year, const int month) {
	if (year == 2027 and month == 1) { //Promotion
		Glebas.salary = 100'000;
	}

	if (month == 1) {  //среднегодовой рост заработной платы
		Glebas.salary *= 1.05;
	}

	Glebas.cash += Glebas.salary; //+= прибавл€ет к самому себе значение

}

void ipoteka(const int year, const int month) { //траты на жильЄ, вз€тое в ипотеку + коммуналка
	struct ipoteka {
		RUB full_credit = 10'000'000; //стоимость квартиры
		int percent = 13; //ставка ипотеки
		int period = 30; //срок ипотеки в годах
	};
	
	struct ipoteka i;
	int harea = 60; //площадь жиль€ в м2
		
	double p = i.percent / 12.0 / 100.0;  //ежемес€чна€ процентна€ ставка
	double p1 = pow(1.0 + p, i.period * 12);
	expence.habitation.credit = (RUB)(i.full_credit * ((p * p1) / (p1 - 1)));  //ежемес€чный платЄж по ипотеке

	if (year == 2031 and month == 1) {  
		expence.habitation.habit = expence.habitation.credit;
	}
	
	if (year == 2031 + i.period and month == 1) {
		expence.habitation.habit = 0;
	}

	//расходы на коммунальные услуги


	static std::mt19937 gen(std::random_device{} ());

	RUB hw_rate = 150;
	RUB cw_rate = 45;
	RUB elec_rate = 7;
	RUB security;
	RUB ren = 25; //кап ремонт тариф
	RUB heating = 2350;

	if (month == 1) {  //ежегодные/сезонные изменени€
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

	if (year == 2031 + i.period and month == 1) {  //страховка
		security = 500;
	}
	else {
		security = (RUB)(i.full_credit * 0.008 / 12);
	}

	std::unordered_set<int> month_winter = { 1, 2, 12 }; //распределение мес€цев по временам года
	std::unordered_set<int> month_spring = { 3, 4, 5 };
	std::unordered_set<int> month_summer = { 6, 7, 8 };
	std::unordered_set<int> month_autumn = { 9, 10, 11 };

	if (year >= 2031) {
		if (month_spring.count(month)) {

			std::uniform_int_distribution<> cold_water_consumption(3, 4); //потреблениие воды в м3 в мес€ц
			std::uniform_int_distribution<> hot_water_consumption(4, 5);
			std::uniform_int_distribution<> electricity_consumption(75, 110); //расход электричекства в к¬т*ч в мес€ц
			RUB cw = cold_water_consumption(gen) * cw_rate;
			RUB hw = hot_water_consumption(gen) * hw_rate;
			RUB e = electricity_consumption(gen) * elec_rate;

			if (month != 5) {
				Glebas.cash -= heating;
				kom += heating;
			}

			Glebas.cash -= (cw + hw + e + security + (ren * harea));
			kom += (cw + hw + e + security + (ren * harea));
		}
		if (month_summer.count(month)) {

			std::uniform_int_distribution<> cold_water_consumption(3, 4);
			std::uniform_int_distribution<> hot_water_consumption(4, 5);
			std::uniform_int_distribution<> electricity_consumption(60, 95);
			RUB cw = cold_water_consumption(gen) * cw_rate;
			RUB hw = hot_water_consumption(gen) * hw_rate;
			RUB e = electricity_consumption(gen) * elec_rate;

			Glebas.cash -= (cw + hw + e + security + (ren * harea));
			kom += (cw + hw + e + security + (ren * harea));
		}
		if (month_autumn.count(month)) {

			std::uniform_int_distribution<> cold_water_consumption(3, 4);
			std::uniform_int_distribution<> hot_water_consumption(4, 5);
			std::uniform_int_distribution<> electricity_consumption(75, 110);
			RUB cw = cold_water_consumption(gen) * cw_rate;
			RUB hw = hot_water_consumption(gen) * hw_rate;
			RUB e = electricity_consumption(gen) * elec_rate;

			if (month != 9) {
				Glebas.cash -= heating;
				kom += heating;
			}

			Glebas.cash -= (cw + hw + e + security + (ren * harea));
			kom += (cw + hw + e + security + (ren * harea));
		}
		if (month_winter.count(month)) {

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

void Habitation(const int year, const int month) {  //жильЄ
	if (year == 2028 and month == 9) {
		expence.habitation.habit = expence.habitation.rent;
	}

	if (month == 1) {  // инфл€ци€ на жильЄ
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

void Tasties_expences(const int year, const int month) {  //ћоделирует случайные расходы на еду
	std::unordered_set<int> days_31 = { 1, 3, 5, 7, 8, 10, 12 };
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
		Glebas.cash -= 3000; //ежегодна€ вакцинаци€
	}


	if (Glebas.cat.age <= 5) {
		if (r100() < 10) {
			Glebas.cat.diseases.push_back(Disease::Glisti);
		}
		if (r100() < 20) {
			Glebas.cat.diseases.push_back(Disease::Infection);
		}
	}
	if (Glebas.cat.age > 5 and Glebas.cat.age <= 10) {
		if (r100() < 7) {
			Glebas.cat.diseases.push_back(Disease::Glisti);
		}
		if (r100() < 10) {
			Glebas.cat.diseases.push_back(Disease::Infection);
		}
		if (r100() < 10) {
			Glebas.cat.diseases.push_back(Disease::Heart);
		}
		if (r100() < 10) {
			Glebas.cat.diseases.push_back(Disease::Teeth);
		}
		if (r100() < 10) {
			Glebas.cat.diseases.push_back(Disease::Urinary);
		}
	}
	if (Glebas.cat.age > 10 and Glebas.cat.age <= 15) {
		if (r100() < 5) {
			Glebas.cat.diseases.push_back(Disease::Glisti);
		}
		if (r100() < 5) {
			Glebas.cat.diseases.push_back(Disease::Infection);
		}
		if (r100() < 15) {
			Glebas.cat.diseases.push_back(Disease::Heart);
		}
		if (r100() < 15) {
			Glebas.cat.diseases.push_back(Disease::Teeth);
		}
		if (r100() < 20) {
			Glebas.cat.diseases.push_back(Disease::Urinary);
		}
	}
	if (Glebas.cat.age > 15) {
		if (r100() < 5) {
			Glebas.cat.diseases.push_back(Disease::Cancer);
		}
		if (r100() < 15) {
			Glebas.cat.diseases.push_back(Disease::Kidney);
		}
		if (r100() < 20) {
			Glebas.cat.diseases.push_back(Disease::Heart);
		}
		if (r100() < 15) {
			Glebas.cat.diseases.push_back(Disease::Teeth);
		}
		if (r100() < 20) {
			Glebas.cat.diseases.push_back(Disease::Urinary);
		}
	}

	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Glisti) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 5; //сколько здоровь€ тер€ет питомец
		Glebas.cash -= 1000;  //стоимость лечени€
		Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Glisti), Glebas.cat.diseases.end());
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Infection) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 10;
		Glebas.cash -= 3000;
		Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Infection), Glebas.cat.diseases.end());
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Heart) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 20;
		Glebas.cash -= 8000;
		Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Heart), Glebas.cat.diseases.end());
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Teeth) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 10;
		Glebas.cash -= 6000;
		Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Teeth), Glebas.cat.diseases.end());
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Urinary) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 10;
		Glebas.cash -= 10'000;
		Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Urinary), Glebas.cat.diseases.end());
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Kidney) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 15;
		Glebas.cash -= 10'000;
		Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Kidney), Glebas.cat.diseases.end());
	}
	if (std::find(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Cancer) != Glebas.cat.diseases.end()) {
		Glebas.cat.health -= 30;
		Glebas.cash -= 25'000;
		Glebas.cat.diseases.erase(std::remove(Glebas.cat.diseases.begin(), Glebas.cat.diseases.end(), Disease::Cancer), Glebas.cat.diseases.end());
	}
}

void pet(const int year, const int month) {
	Glebas.cat.health = std::clamp(Glebas.cat.health, 0, 100);  //задание границ здоровь€ кота

	if (year == 2030 and month == 3) {
		Glebas.cat.health += 100;
	}

	if (month == 1 and Glebas.cat.health != 0) {  //¬озраст кота
		Glebas.cat.age += 1;
	}

	if (Glebas.cat.age <= 5 and month == 1) { //¬еро€тность смерти, не св€занной с заболеванием
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

	if (Glebas.cat.age <= 10) { //посещение ветеринара с периодичностью, завис€щей от возраста питомца
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
			Glebas.cat.health += 100;  //ѕо€вл€етс€ новый питомец
			Glebas.cat.age = 0;
		}
	}
}

void Glebas_spendings(const int year, const int month) {
	
	Glebas.cash -= expence.food;
	Glebas.cash -= expence.tasties;
	Glebas.cash -= expence.habitation.habit;
	total_expence_tasties += expence.tasties;
}

void simulation() {
	int year = 2026;
	int month = 9;
	while (not (year == 2036 and month == 9)) {
		
		Glebas_salary(year, month);
		Glebas_spendings(year, month);
		food(year, month);
		Habitation(year, month);
		pet(year, month);

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