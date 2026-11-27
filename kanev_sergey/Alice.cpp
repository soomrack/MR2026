#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <clocale>

using rub = long long; // буду считать в копейках, чтоб не было погрешностей

std::vector <std::string> month_name = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };


rub rubles_to_kopecks(rub rubles) {
	rub kopecks = rubles * 100;
	return kopecks;
}

std::string format_money(rub amount) {
	bool is_negative = false;
	if (amount < 0) {
		is_negative = true;
		amount = -amount;
	}
	rub rubles = amount / 100;
	rub kopecks = amount % 100;
	std::string rub_str = std::to_string(rubles);
	std::string grouped_rubles;
	int digits_added = 0;
	for (int i = static_cast<int>(rub_str.size()) - 1; i >= 0; --i) {
		if (digits_added > 0 && digits_added % 3 == 0) {
			grouped_rubles.push_back(' ');
		}
		grouped_rubles.push_back(rub_str[i]);
		digits_added++;
	}
	std::reverse(grouped_rubles.begin(), grouped_rubles.end());
	std::string kop_str = std::to_string(kopecks);
	if (kopecks < 10) {
		kop_str = "0" + kop_str;
	}
	std::string result;
	if (is_negative) {
		result += "-";
	}
	result += grouped_rubles + "." + kop_str + " RUB";

	return result;
}

struct income_data {
	rub salary = rubles_to_kopecks(50000);
	rub tax_deduction = 0;
};

struct costs_data {
	rub food = 0;
	rub utilities = 0;
	rub mortgage = 0;
	rub household_expenses = 0;
};

struct person {
	std::string name;
	int age = 25;
	rub savings = 0;
	income_data income;
	costs_data costs;
	rub total_income = 0;
	rub total_costs = 0;
	rub min_savings = 0;
	int month_in_debt = 0;
	bool has_been_in_debt = false;
};


void add_income(person& p, rub amount) {
	if (amount <= 0) return;
	p.savings += amount;
	p.total_income += amount;
}

void pay_costs(person& p, rub amount) {
	if (amount <= 0) return;
	p.savings -= amount;
	p.total_costs += amount;
	if (p.min_savings > p.savings) {
		p.min_savings = p.savings;
	}
}

void update_person_age(person& p) {
	p.age++;
}

struct sim_date {
	int year = 2026;
	int month = 1;
};

void next_month(sim_date& date) {
	date.month++;
	if (date.month > 12) {
		date.month = 1;
		date.year++;
	}
}

person create_vera() {
	person p;
	p.name = "Vera";
	p.age = 22;
	p.savings = rubles_to_kopecks(5000);
	p.min_savings = p.savings;
	p.income.salary = rubles_to_kopecks(50000);
	p.costs.food = rubles_to_kopecks(15000);
	p.costs.household_expenses = rubles_to_kopecks(3000);
	p.costs.utilities = rubles_to_kopecks(2000);
	p.costs.mortgage = rubles_to_kopecks(10000);
	return p;

}

person create_anton() {
	person p;
	p.name = "Anton";
	p.age = 26;
	p.savings = rubles_to_kopecks(75000);
	p.min_savings = p.savings;
	p.income.salary = rubles_to_kopecks(75000);
	p.costs.food = rubles_to_kopecks(23000);
	p.costs.household_expenses = rubles_to_kopecks(4000);
	p.costs.utilities = rubles_to_kopecks(3000);
	p.costs.mortgage = rubles_to_kopecks(20000);
	return p;
}

std::string format_date(const sim_date& date) {
	std::string year_month = std::to_string(date.year) + ", " + month_name[date.month - 1];
	return year_month;
}

void update_debt_status(person& p) {
	if (p.savings < 0) {
		p.month_in_debt++;
		p.has_been_in_debt = true;
	}
	else {
		p.month_in_debt = 0;
	}
}

void simulate_month(person& p, const sim_date& date) {
	add_income(p, p.income.salary);
	pay_costs(p, p.costs.food);
	pay_costs(p, p.costs.household_expenses);
	pay_costs(p, p.costs.mortgage);
	pay_costs(p, p.costs.utilities);
	update_debt_status(p);
	if (date.month == 12) {
		update_person_age(p);
	}
}

void print_yearly_checkpoint(const person& p, const sim_date& date) {
	std::cout << "[" << format_date(date) << "] " << p.name << " (age " << p.age << ") | Balance: " << format_money(p.savings) << std::endl;;
}

void print_final_report(const person& p) {
	std::cout << "=== Итоги: " << p.name << " ===" << std::endl;
	std::cout << "Возраст: " << p.age << std::endl;
	std::cout << "Текущий баланс: " << format_money(p.savings) <<std::endl;
	std::cout << "Всего заработано: " << format_money(p.total_income) << std::endl;
	std::cout << "Всего потрачено: " << format_money(p.total_costs) << std::endl;
	std::cout << "Минимальный баланс за все время: " << format_money(p.min_savings) << std::endl;
	std::cout << "Количество месяцев в долгах: " << p.month_in_debt << std::endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	person vera = create_vera();
	person anton = create_anton();
	sim_date date;

	int total_months = 15 * 12;
	for (int step = 0; step < total_months; step++) {
		simulate_month(vera, date);
		simulate_month(anton, date);

		if (date.month == 12) {
			print_yearly_checkpoint(vera, date);
			print_yearly_checkpoint(anton, date);
			std::cout << "\n";
		}
		next_month(date);
	}
	print_final_report(vera);
	std::cout << "\n";
	print_final_report(anton);
	return 0;
}
