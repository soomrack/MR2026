#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <clocale>
#include <cmath>
#include <random>

using rub = long long; // буду считать в копейках, чтоб не было погрешностей

std::vector <std::string> month_name = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

int part_of_percent = 4;

struct sim_date {
	int year = 2026;
	int month = 1;
};

sim_date date;

std::string format_date() {
	std::string year_month = std::to_string(date.year) + ", " + month_name[date.month - 1];
	return year_month;
}

std::random_device rd;
std::mt19937 gen(rd());

double random_range(double min, double max) {
	std::uniform_real_distribution<double> dist(min, max);
	return dist(gen);
}

rub calc_percent(rub amount, double percent) {
	return std::llround(amount * percent / 100.0);
}

bool check_chance(int chance) {
	if (random_range(1, 100) <= chance) {
		return true;
	}
	else {
		return false;
	}
}

rub inflation(rub amount, double max_yearly_percent) {
	double monthly_percent = random_range(0.0, max_yearly_percent / 12.0);
	return std::llround(amount * (1.0 + monthly_percent / 100.0));
}

bool check_crisis() {
	if (check_chance(15) == true) {
		std::cout << "Этот год был кризисным!!!" << std::endl;
		part_of_percent += random_range(2, 4);
		return true;
	}
	else {
		part_of_percent = 4;
		return false;
	}
}

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

struct job_data {
	rub salary = rubles_to_kopecks(0);
	double salary_index = 1;
	double bonus_rate = 0;
	rub bonus = 0;
};

job_data create_manager() {
	job_data j;
	j.salary = rubles_to_kopecks(55000);
	j.bonus_rate = 0.35;
	j.salary_index = random_range(1.06, 1.10);
	return j;
}

job_data create_reporter() {
	job_data j;
	j.salary = rubles_to_kopecks(85000);
	j.bonus_rate = 0.30;
	j.salary_index = random_range(1.06, 1.09);
	return j;
}

job_data create_tutor() {
	job_data j;
	j.salary = rubles_to_kopecks(25000);
	j.salary_index = random_range(1.05, 1.08);
	return j;
}

job_data create_none() {
	job_data j;
	j.salary = rubles_to_kopecks(0);
	return j;
}


job_data manager = create_manager();

job_data reporter = create_reporter();

job_data tutor = create_tutor();

job_data none = create_none();

struct costs_data {
	rub food = 0;
	rub utilities = 0;
	rub medicine = 0;
	rub education = 0;
	rub sport = 0;
	rub rent = 0;
	rub household_expenses = 0;
	rub taxes = 0;
	rub total_taxes = 0;
};

struct person {
	std::string name;
	int age = 25;
	rub savings = 0;
	costs_data costs;
	rub total_income = 0;
	rub total_costs = 0;
	rub min_savings = 0;
	int month_in_debt = 0;
	bool has_been_in_debt = false;
	job_data first_job;
	job_data second_job;
	rub sum_salary_per_year = 0;
	int cnt_raises = 0;
	int cnt_decrease = 0;
	int chance_to_raise = 0;
	int chance_to_decrease = 0;
	rub refund = 0;
};

void apply_monthly_inflation(person& p) {
	p.costs.food = inflation(p.costs.food, part_of_percent + 6);
	p.costs.rent = inflation(p.costs.rent, part_of_percent + 8);
	p.costs.utilities = inflation(p.costs.utilities, part_of_percent + 4);
	p.costs.sport = inflation(p.costs.sport, part_of_percent + 4);
	p.costs.medicine = inflation(p.costs.medicine, part_of_percent + 7);
	p.costs.education = inflation(p.costs.education, part_of_percent + 5);
}

void apply_yearly_inflation(person& p) {
	if (date.month == 7) {
		p.costs.household_expenses = inflation(p.costs.household_expenses, (part_of_percent + 6) * 12);
	}
}


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

void apply_monthly_costs(person& p) {
	pay_costs(p, p.costs.food);
	pay_costs(p, p.costs.household_expenses);
	pay_costs(p, p.costs.rent);
	pay_costs(p, p.costs.utilities);
	pay_costs(p, p.costs.taxes);
	pay_costs(p, p.costs.medicine);
	pay_costs(p, p.costs.sport);
	pay_costs(p, p.costs.education);
}

void update_person_age(person& p) {
	p.age++;
}

void next_month() {
	date.month++;
	if (date.month > 12) {
		date.month = 1;
		date.year++;
	}
}

void calculate_progressive_ndfl(person& p) {
	p.sum_salary_per_year += p.first_job.salary + p.first_job.bonus + p.second_job.salary;
	if (p.sum_salary_per_year <= rubles_to_kopecks(2400000)) {
		p.costs.taxes = (p.first_job.salary + p.first_job.bonus + p.second_job.salary) * 13 / 100;
		p.costs.total_taxes += p.costs.taxes;
	}
	else if (p.sum_salary_per_year <= rubles_to_kopecks(5000000)) {
		p.costs.taxes = (p.first_job.salary + p.first_job.bonus + p.second_job.salary) * 15 / 100;
		p.costs.total_taxes += p.costs.taxes;
	}
	else if (p.sum_salary_per_year <= rubles_to_kopecks(20000000)) {
		p.costs.taxes = (p.first_job.salary + p.first_job.bonus + p.second_job.salary) * 18 / 100;
		p.costs.total_taxes += p.costs.taxes;
	}
	else if (p.sum_salary_per_year <= rubles_to_kopecks(50000000)) {
		p.costs.taxes = (p.first_job.salary + p.first_job.bonus + p.second_job.salary) * 20 / 100;
		p.costs.total_taxes += p.costs.taxes;
	}
	else if (p.sum_salary_per_year > rubles_to_kopecks(50000000)) {
		p.costs.taxes = (p.first_job.salary + p.first_job.bonus + p.second_job.salary) * 22 / 100;
		p.costs.total_taxes += p.costs.taxes;
	}
}

rub process_tax_refund(person& p) {
	p.refund = (p.costs.education + p.costs.medicine + p.costs.sport) * 13 / 100;
	if (p.refund > p.costs.total_taxes) {
		p.refund = p.costs.total_taxes;
	}
	if (p.refund > rubles_to_kopecks(150000)) {
		p.refund = rubles_to_kopecks(150000);
	}
	return p.refund;
}

void index_salary(job_data& job) {
	if (date.month == 12) {
		job.salary = std::llround(job.salary * job.salary_index);
	}
}

void process_career_changes(person& p, int raise, int decrease) {
	if (date.month == 7) {
		if (check_chance(raise)) {
			p.first_job.salary += std::llround(p.first_job.salary * random_range(10, 18) / 100.0);
			p.cnt_raises++;
			std::cout << "[" << format_date() << "] " << p.name << " получил(а) повышение!\n";
		}
		else if (check_chance(decrease)) {
			p.first_job.salary -= std::llround(p.first_job.salary * random_range(8, 12) / 100.0);
			p.cnt_decrease++;
			std::cout << "[" << format_date() << "] " << p.name << " понижен(а) :(\n";
		}
	}
}

person create_vera() {
	person p;
	p.name = "Vera";
	p.age = 22;
	p.first_job = manager;
	p.second_job = tutor;
	p.savings = rubles_to_kopecks(30000);
	p.min_savings = p.savings;
	p.costs.food = rubles_to_kopecks(16000);
	p.costs.household_expenses = rubles_to_kopecks(3500);
	p.costs.utilities = rubles_to_kopecks(4000);
	p.costs.rent = rubles_to_kopecks(20000);
	p.chance_to_raise = 20;
	p.chance_to_decrease = 10;
	p.costs.education = rubles_to_kopecks(3000);
	p.costs.medicine = rubles_to_kopecks(2000);
	p.costs.sport = rubles_to_kopecks(3000);
	return p;

}

person create_anton() {
	person p;
	p.name = "Anton";
	p.age = 26;
	p.first_job = reporter;
	p.second_job = none;
	p.savings = rubles_to_kopecks(80000);
	p.min_savings = p.savings;
	p.costs.food = rubles_to_kopecks(20000);
	p.costs.household_expenses = rubles_to_kopecks(4000);
	p.costs.utilities = rubles_to_kopecks(4500);
	p.costs.rent = rubles_to_kopecks(25000);
	p.chance_to_raise = 18;
	p.chance_to_decrease = 8;
	p.costs.education = rubles_to_kopecks(0);
	p.costs.medicine = rubles_to_kopecks(2000);
	p.costs.sport = rubles_to_kopecks(4000);
	return p;
}

void update_debt_status(person& p) {
	if (p.savings < 0) {
		p.month_in_debt++;
		p.has_been_in_debt = true;
	}
}

void simulate_month(person& p) {
	process_career_changes(p, p.chance_to_raise, p.chance_to_decrease);
	rub monthly_income = p.first_job.salary + p.second_job.salary;
	if (date.month == 12) {
		p.first_job.bonus = std::llround(p.first_job.salary * p.first_job.bonus_rate);
		monthly_income += p.first_job.bonus;
	}
	else {
		p.first_job.bonus = 0;
	}
	add_income(p, monthly_income);
	calculate_progressive_ndfl(p);
	if (date.month == 12) {
		update_person_age(p);
		p.sum_salary_per_year = 0;
		add_income(p, process_tax_refund(p));
		p.refund = 0;
	}
	apply_monthly_costs(p);
	apply_monthly_inflation(p);
	apply_yearly_inflation(p);
	update_debt_status(p);
	index_salary(p.first_job);
	index_salary(p.second_job);
}

void print_yearly_checkpoint(const person& p) {
	std::cout << "[" << format_date() << "] " << p.name << " (age " << p.age << ") | Баланс: " << format_money(p.savings) << std::endl;
}

void print_final_report(const person& p) {
	std::cout << "=== Итоги: " << p.name << " ===" << std::endl;
	std::cout << "Возраст: " << p.age << std::endl;
	std::cout << "Текущий баланс: " << format_money(p.savings) << std::endl;
	std::cout << "Всего заработано: " << format_money(p.total_income) << std::endl;
	std::cout << "Всего потрачено: " << format_money(p.total_costs) << std::endl;
	std::cout << "Кол-во повышений: " << p.cnt_raises << std::endl;
	std::cout << "Кол-во понижений: " << p.cnt_decrease << std::endl;
	std::cout << "Всего налогов выплачено: " << format_money(p.costs.total_taxes) << std::endl;
	std::cout << "Минимальный баланс за все время: " << format_money(p.min_savings) << std::endl;
	std::cout << "Количество месяцев в долгах: " << p.month_in_debt << std::endl << std::endl;
}

void compare_money(const person& p1, const person& p2, rub p1x, rub p2y, std::string str) {
	if (p1x == p2y) {
		std::cout << str << ": ничья " << std::endl;
	}
	if (p1x > p2y) {
		if (p2y == 0) {
			std::cout << str << ": " << p1.name << ", больше на " << format_money(p1x - p2y) << std::endl;
		}
		else {
			long long diff_percent = std::llround(static_cast<double>(p1x - p2y) / p2y * 100);
			if (diff_percent < 0) {
				diff_percent *= -1;
			}
			std::cout << str << ": " << p1.name << ", больше на " << format_money(p1x - p2y) << " (" << diff_percent << "%)" << std::endl;
		}
	}
	if (p1x < p2y) {
		if (p1x == 0) {
			std::cout << str << ": " << p2.name << ", больше на " << format_money(p2y - p1x) << std::endl;
		}
		else {
			long long diff_percent = std::llround(static_cast<double>(p2y - p1x) / p1x * 100);
			if (diff_percent < 0) {
				diff_percent *= -1;
			}
			std::cout << str << ": " << p2.name << ", больше на " << format_money(p2y - p1x) << " (" << diff_percent << "%)" << std::endl;
		}
	}
}

void compare(const person& p1, const person& p2, rub p1x, rub p2y, std::string str) {
	if (p1x == p2y) {
		std::cout << str << ": ничья " << std::endl;
	}
	if (p1x > p2y) {
		if (p2y == 0) {
			std::cout << str << ": " << p1.name << ", больше на " << (p1x - p2y) << std::endl;
		}
		else {
			long long diff_percent = std::llround(static_cast<double>(p1x - p2y) / p2y * 100);
			if (diff_percent < 0) {
				diff_percent *= -1;
			}
			std::cout << str << ": " << p1.name << ", больше на " << (p1x - p2y) << " (" << diff_percent << "%)" << std::endl;
		}
	}
	if (p1x < p2y) {
		if (p1x == 0) {
			std::cout << str << ": " << p2.name << ", больше на " << (p2y - p1x) << std::endl;
		}
		else {
			long long diff_percent = std::llround(static_cast<double>(p2y - p1x) / p1x * 100);
			if (diff_percent < 0) {
				diff_percent *= -1;
			}
			std::cout << str << ": " << p2.name << ", больше на " << (p2y - p1x) << " (" << diff_percent << "%)" << std::endl;
		}
	}
}

void print_comparison_summary(const person& p1, const person& p2) {
	std::cout << "=== СРАВНЕНИЕ МОДЕЛЕЙ ===" << std::endl;
	compare_money(p1, p2, p1.savings, p2.savings, "КОПИЛКА");
	compare_money(p1, p2, p1.total_income, p2.total_income, "РАБОТЯГА");
	compare_money(p1, p2, p1.total_costs, p2.total_costs, "ТРАНЖИРА");
	compare(p1, p2, p1.cnt_raises, p2.cnt_raises, "КАРЬЕРИСТ");
	compare(p1, p2, p1.cnt_decrease, p2.cnt_decrease, "ДАУНГРЕЙД");
	compare_money(p1, p2, p1.costs.total_taxes, p2.costs.total_taxes, "НДФЛ'щик");
	compare_money(p1, p2, p1.min_savings, p2.min_savings, "ЗАПАС ПРОЧНОСТИ");
	compare(p1, p2, p1.month_in_debt, p2.month_in_debt, "ДОЛЖНИК");
}

int main()
{
	setlocale(LC_ALL, "Russian");
	person vera = create_vera();
	person anton = create_anton();

	int total_months = 15 * 12;
	for (int step = 0; step < total_months; step++) {
		if (date.month == 1) {
			check_crisis();
		}
		simulate_month(vera);
		simulate_month(anton);
		if (date.month == 12) {
			print_yearly_checkpoint(vera);
			print_yearly_checkpoint(anton);
			std::cout << std::endl;
		}
		next_month();
	}
	print_final_report(vera);
	std::cout << "\n";
	print_final_report(anton);
	print_comparison_summary(vera, anton);
	return 0;
}