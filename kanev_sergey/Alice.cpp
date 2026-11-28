#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>

using rub = long long; // stored in kopecks to avoid floating-point errors

std::vector<std::string> month_name = {
	"January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"
};

int part_of_percent = 4;

struct sim_date {
	int year = 2026;
	int month = 1;
};

sim_date date;

std::string format_date() {
	std::string year_month = "[" + std::to_string(date.year) + ", " + month_name[date.month - 1] + "] ";
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
		std::cout << format_date() << "A crisis year has begun!!!" << std::endl;
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
	rub food = 0;               // total food expenses
	rub vegatebles = 0;
	rub meat = 0;
	rub sweets = 0;
	rub gazuava = 0;            // snacks and treats
	rub utilities = 0;          // total utilities + internet
	rub jkh = 0;                // housing and communal services
	rub internet = 0;
	rub medicine = 0;
	rub education = 0;
	rub sport = 0;
	rub rent = 0;               // apartment rent
	rub household_expenses = 0; // home maintenance and cleaning
	rub taxes = 0;
	rub total_taxes = 0;
	rub clothes = 0;
};

struct disease {
	double chance_to_death = 0;
	double chance_to_heal = 0;
	rub costs_to_heal_per_month = 0;
	std::string name = "";
	int points_of_happiness = 0;
	double fall_sick = 0;
	bool is_sick = false;
};

disease create_caries() {
	disease d;
	d.fall_sick = 4;
	d.chance_to_death = 0;
	d.chance_to_heal = random_range(85, 95);
	d.costs_to_heal_per_month = random_range(rubles_to_kopecks(5000), rubles_to_kopecks(15000));
	d.points_of_happiness = -15;
	d.name = "Caries";
	return d;
}

disease create_cold() {
	disease d;
	d.fall_sick = 50;
	d.chance_to_death = 0;
	d.chance_to_heal = random_range(90, 98);
	d.costs_to_heal_per_month = random_range(rubles_to_kopecks(500), rubles_to_kopecks(2000));
	d.points_of_happiness = -5;
	d.name = "Cold";
	return d;
}

disease create_respiratory_tract_infection() {
	disease d;
	d.fall_sick = 3;
	d.chance_to_death = random_range(0, 1);
	d.chance_to_heal = random_range(75, 85);
	d.costs_to_heal_per_month = random_range(rubles_to_kopecks(3000), rubles_to_kopecks(8000));
	d.points_of_happiness = -15;
	d.name = "Respiratory tract infection";
	return d;
}

disease create_headache() {
	disease d;
	d.fall_sick = 15;
	d.chance_to_death = 0;
	d.chance_to_heal = random_range(90, 98);
	d.costs_to_heal_per_month = random_range(rubles_to_kopecks(300), rubles_to_kopecks(1500));
	d.points_of_happiness = -3;
	d.name = "Headache";
	return d;
}

disease create_stroke() {
	disease d;
	d.fall_sick = 1;
	d.chance_to_death = random_range(15, 35);
	d.chance_to_heal = random_range(70, 85);
	d.costs_to_heal_per_month = random_range(rubles_to_kopecks(15000), rubles_to_kopecks(35000));
	d.points_of_happiness = -50;
	d.name = "Stroke";
	return d;
}

disease caries = create_caries();
disease cold = create_cold();
disease respiratory_tract_infection = create_respiratory_tract_infection();
disease headache = create_headache();
disease stroke = create_stroke();

struct sick {
	bool is_caries = false;
	bool is_cold = false;
	bool is_respiratory_tract_infection = false;
	bool is_headache = false;
	bool is_stroke = false;
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
	rub total_on_clothes = 0;
	int cnt_raises = 0;
	int cnt_decrease = 0;
	int chance_to_raise = 0;
	int chance_to_decrease = 0;
	rub refund = 0;
	bool is_alive = true;
	sick is_sick;
	int points_of_happiness = 0;
	int cnt_disease = 0;
};

bool is_sick(person& p, const disease& d) {
	if (check_chance(d.fall_sick) == true) {
		std::cout << format_date() << p.name << " fell ill with: " << d.name << std::endl;
		if (check_chance(d.chance_to_death) == true) {
			std::cout << format_date() << p.name << " passed away :(" << std::endl;
			p.is_alive = false;
		}
		else {
			p.costs.medicine += d.costs_to_heal_per_month;
			p.points_of_happiness += d.points_of_happiness;
		}
		return true;
	}
	else {
		return false;
	}
}

void check_health(person& p) {
	if (p.is_alive == true) {
		if (p.is_sick.is_caries == false) {
			if (is_sick(p, caries) == true) {
				p.is_sick.is_caries = true;
			}
		}
		else if (check_chance(caries.chance_to_heal) == true) {
			p.is_sick.is_caries = false;
			p.costs.medicine -= caries.costs_to_heal_per_month;
			std::cout << format_date() << p.name << " recovered from: " << caries.name << std::endl;
			p.points_of_happiness += 10;
			p.cnt_disease++;
		}
	}

	if (p.is_alive == true) {
		if (p.is_sick.is_respiratory_tract_infection == false) {
			if (is_sick(p, respiratory_tract_infection) == true) {
				p.is_sick.is_respiratory_tract_infection = true;
			}
		}
		else if (check_chance(respiratory_tract_infection.chance_to_heal) == true) {
			p.is_sick.is_respiratory_tract_infection = false;
			p.costs.medicine -= respiratory_tract_infection.costs_to_heal_per_month;
			std::cout << format_date() << p.name << " recovered from: " << respiratory_tract_infection.name << std::endl;
			p.points_of_happiness += 10;
			p.cnt_disease++;
		}
	}

	if (p.is_alive == true) {
		if (p.is_sick.is_cold == false) {
			if (is_sick(p, cold) == true) {
				p.is_sick.is_cold = true;
			}
		}
		else if (check_chance(cold.chance_to_heal) == true) {
			p.is_sick.is_cold = false;
			p.costs.medicine -= cold.costs_to_heal_per_month;
			std::cout << format_date() << p.name << " recovered from: " << cold.name << std::endl;
			p.points_of_happiness += 10;
			p.cnt_disease++;
		}
	}

	if (p.is_alive == true) {
		if (p.is_sick.is_headache == false) {
			if (is_sick(p, headache) == true) {
				p.is_sick.is_headache = true;
			}
		}
		else if (check_chance(headache.chance_to_heal) == true) {
			p.is_sick.is_headache = false;
			p.costs.medicine -= headache.costs_to_heal_per_month;
			std::cout << format_date() << p.name << " recovered from: " << headache.name << std::endl;
			p.points_of_happiness += 10;
			p.cnt_disease++;
		}
	}

	if (p.is_alive == true) {
		if (p.is_sick.is_stroke == false) {
			if (is_sick(p, stroke) == true) {
				p.is_sick.is_stroke = true;
			}
		}
		else if (check_chance(stroke.chance_to_heal) == true) {
			p.is_sick.is_stroke = false;
			p.costs.medicine -= stroke.costs_to_heal_per_month;
			std::cout << format_date() << p.name << " recovered from: " << stroke.name << std::endl;
			p.points_of_happiness += 10;
			p.cnt_disease++;
		}
	}
}

void apply_monthly_inflation(person& p) {
	p.costs.vegatebles = inflation(p.costs.vegatebles, part_of_percent + random_range(1, 3));
	p.costs.meat = inflation(p.costs.meat, part_of_percent + random_range(2, 6));
	p.costs.sweets = inflation(p.costs.sweets, part_of_percent + random_range(3, 8));
	p.costs.gazuava = inflation(p.costs.gazuava, part_of_percent + random_range(1, 2));
	p.costs.rent = inflation(p.costs.rent, part_of_percent + random_range(4, 8));
	p.costs.household_expenses = inflation(p.costs.household_expenses, part_of_percent + random_range(1, 4));
	p.costs.sport = inflation(p.costs.sport, part_of_percent + random_range(1, 4));
	p.costs.medicine = inflation(p.costs.medicine, part_of_percent + random_range(2, 7));
	p.costs.education = inflation(p.costs.education, part_of_percent + random_range(1, 5));
	p.costs.internet = inflation(p.costs.internet, part_of_percent + random_range(1, 2));
	p.costs.clothes = inflation(p.costs.clothes, part_of_percent + random_range(1, 6));
}

void process_food_expenses(person& p) {
	p.costs.food = p.costs.gazuava + p.costs.meat + p.costs.vegatebles + p.costs.sweets;
	if (date.month == 12) {
		p.costs.food += calc_percent(p.costs.food, random_range(15, 20));
	}
}

void process_utilities_expenses(person& p) {
	p.costs.utilities = p.costs.jkh + p.costs.internet;
	if ((date.month >= 1 && date.month <= 4) || (date.month >= 10 && date.month <= 12)) {
		rub old_jkh = p.costs.jkh;
		p.costs.jkh += calc_percent(old_jkh, random_range(10, 15));
		p.costs.utilities = p.costs.jkh + p.costs.internet;
		p.costs.jkh = old_jkh;
	}
}

void apply_yearly_inflation(person& p) {
	if (date.month == 7) {
		p.costs.jkh = inflation(p.costs.jkh, (part_of_percent + 6) * 12);
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
	if (date.month % 3 == 0) {
		pay_costs(p, p.costs.clothes);
		p.total_on_clothes += p.costs.clothes;
	}
	process_utilities_expenses(p);
	process_food_expenses(p);
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
			std::cout << format_date() << p.name << " got promoted!\n";
			p.points_of_happiness += 20;
		}
		else if (check_chance(decrease)) {
			p.first_job.salary -= std::llround(p.first_job.salary * random_range(8, 12) / 100.0);
			p.cnt_decrease++;
			std::cout << format_date() << p.name << " got demoted :(\n";
			p.points_of_happiness -= 25;
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
	p.costs.meat = rubles_to_kopecks(6000);
	p.costs.vegatebles = rubles_to_kopecks(4000);
	p.costs.sweets = rubles_to_kopecks(3000);
	p.costs.gazuava = rubles_to_kopecks(3000);
	p.costs.household_expenses = rubles_to_kopecks(3500);
	p.costs.jkh = rubles_to_kopecks(3000);
	p.costs.internet = rubles_to_kopecks(1000);
	p.costs.rent = rubles_to_kopecks(20000);
	p.chance_to_raise = 20;
	p.chance_to_decrease = 10;
	p.costs.education = rubles_to_kopecks(3000);
	p.costs.medicine = rubles_to_kopecks(2000);
	p.costs.sport = rubles_to_kopecks(3000);
	p.costs.clothes = rubles_to_kopecks(10000);
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
	p.costs.meat = rubles_to_kopecks(8000);
	p.costs.vegatebles = rubles_to_kopecks(6000);
	p.costs.sweets = rubles_to_kopecks(2000);
	p.costs.gazuava = rubles_to_kopecks(4000);
	p.costs.household_expenses = rubles_to_kopecks(4000);
	p.costs.jkh = rubles_to_kopecks(3200);
	p.costs.internet = rubles_to_kopecks(1500);
	p.costs.rent = rubles_to_kopecks(25000);
	p.chance_to_raise = 18;
	p.chance_to_decrease = 8;
	p.costs.education = rubles_to_kopecks(0);
	p.costs.medicine = rubles_to_kopecks(2000);
	p.costs.sport = rubles_to_kopecks(4000);
	p.costs.clothes = rubles_to_kopecks(12000);
	return p;
}

void update_debt_status(person& p) {
	if (p.savings < 0) {
		p.month_in_debt++;
		p.points_of_happiness -= 5;
		p.has_been_in_debt = true;
	}
}

void simulate_month(person& p) {
	if (date.month >= 4 && date.month <= 9) {
		cold.fall_sick = 4;
	}
	else {
		cold.fall_sick = 12;
	}
	check_health(p);
	if (p.is_alive == true) {
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
		p.points_of_happiness += 10;
	}
}

void print_yearly_checkpoint(const person& p) {
	std::cout << format_date() << p.name << " (age " << p.age << ") | Balance: " << format_money(p.savings) << std::endl;
}

void print_final_report(const person& p) {
	std::cout << "=== Final Report: " << p.name << " ===" << std::endl;
	if (p.is_alive == true) {
		std::cout << "Status: Alive" << std::endl;
	}
	else {
		std::cout << "Status: Deceased" << std::endl;
	}
	std::cout << "Age: " << p.age << std::endl;
	std::cout << "Current balance: " << format_money(p.savings) << std::endl;
	std::cout << "Total income: " << format_money(p.total_income) << std::endl;
	std::cout << "Total expenses: " << format_money(p.total_costs) << std::endl;
	std::cout << "Spent on clothes: " << format_money(p.total_on_clothes) << std::endl;
	std::cout << "Happiness points: " << p.points_of_happiness << std::endl;
	std::cout << "Promotions: " << p.cnt_raises << std::endl;
	std::cout << "Demotions: " << p.cnt_decrease << std::endl;
	std::cout << "Illnesses: " << p.cnt_disease << std::endl;
	std::cout << "Total taxes paid: " << format_money(p.costs.total_taxes) << std::endl;
	std::cout << "Minimum balance: " << format_money(p.min_savings) << std::endl;
	std::cout << "Months in debt: " << p.month_in_debt << std::endl << std::endl;
}

void compare_money(const person& p1, const person& p2, rub p1x, rub p2y, std::string str) {
	if (p1x == p2y) {
		std::cout << str << ": tie" << std::endl;
	}
	if (p1x > p2y) {
		if (p2y == 0) {
			std::cout << str << ": " << p1.name << ", higher by " << format_money(p1x - p2y) << std::endl;
		}
		else {
			long long diff_percent = std::llround(static_cast<double>(p1x - p2y) / p2y * 100);
			if (diff_percent < 0) {
				diff_percent *= -1;
			}
			std::cout << str << ": " << p1.name << ", higher by " << format_money(p1x - p2y) << " (" << diff_percent << "%)" << std::endl;
		}
	}
	if (p1x < p2y) {
		if (p1x == 0) {
			std::cout << str << ": " << p2.name << ", higher by " << format_money(p2y - p1x) << std::endl;
		}
		else {
			long long diff_percent = std::llround(static_cast<double>(p2y - p1x) / p1x * 100);
			if (diff_percent < 0) {
				diff_percent *= -1;
			}
			std::cout << str << ": " << p2.name << ", higher by " << format_money(p2y - p1x) << " (" << diff_percent << "%)" << std::endl;
		}
	}
}

void compare(const person& p1, const person& p2, rub p1x, rub p2y, std::string str) {
	if (p1x == p2y) {
		std::cout << str << ": tie" << std::endl;
	}
	if (p1x > p2y) {
		if (p2y == 0) {
			std::cout << str << ": " << p1.name << ", higher by " << (p1x - p2y) << std::endl;
		}
		else {
			long long diff_percent = std::llround(static_cast<double>(p1x - p2y) / p2y * 100);
			if (diff_percent < 0) {
				diff_percent *= -1;
			}
			std::cout << str << ": " << p1.name << ", higher by " << (p1x - p2y) << " (" << diff_percent << "%)" << std::endl;
		}
	}
	if (p1x < p2y) {
		if (p1x == 0) {
			std::cout << str << ": " << p2.name << ", higher by " << (p2y - p1x) << std::endl;
		}
		else {
			long long diff_percent = std::llround(static_cast<double>(p2y - p1x) / p1x * 100);
			if (diff_percent < 0) {
				diff_percent *= -1;
			}
			std::cout << str << ": " << p2.name << ", higher by " << (p2y - p1x) << " (" << diff_percent << "%)" << std::endl;
		}
	}
}

void print_comparison_summary(const person& p1, const person& p2) {
	std::cout << "=== MODEL COMPARISON ===" << std::endl;
	compare_money(p1, p2, p1.savings, p2.savings, "TOP SAVER");
	compare_money(p1, p2, p1.total_income, p2.total_income, "TOP EARNER");
	compare_money(p1, p2, p1.total_costs, p2.total_costs, "BIGGEST SPENDER");
	compare_money(p1, p2, p1.total_on_clothes, p2.total_on_clothes, "FASHIONISTA");
	compare(p1, p2, p1.points_of_happiness, p2.points_of_happiness, "HAPPIEST");
	compare(p1, p2, p1.cnt_raises, p2.cnt_raises, "CAREER CLIMBER");
	compare(p1, p2, p1.cnt_decrease, p2.cnt_decrease, "MOST DEMOTIONS");
	compare(p1, p2, p1.cnt_disease, p2.cnt_disease, "MOST ILLNESSES");
	compare_money(p1, p2, p1.costs.total_taxes, p2.costs.total_taxes, "TOP TAXPAYER");
	compare_money(p1, p2, p1.min_savings, p2.min_savings, "SAFETY NET");
	compare(p1, p2, p1.month_in_debt, p2.month_in_debt, "MOST MONTHS IN DEBT");
}

int main()
{
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