#include <stdio.h>
#include <string>
#include <cstdio>
#include <random>
#include <vector>
#include <algorithm>
#include <array>

/////////////////////////////////////////////////////////////////////////

using RUB = unsigned long long;

struct Person {
    std::string profession;
    std::string alt_profession;
    RUB cash = 0;
    RUB debt = 0;
    RUB income = 0;
    RUB salary = 0;
    RUB scholarship = 0;
    RUB side_job = 0;
    RUB side_job_max = 0;
    RUB salary_earned = 0;
    RUB tax = 0;
    RUB parents_support = 0;
    RUB food = 0;
    RUB food_min = 0;
    RUB food_max = 0;
    RUB entertainment = 0;
    RUB entertainment_min = 0;
    RUB entertainment_max = 0;
    RUB clothes = 0;
    RUB clothes_min = 0;
    RUB clothes_max = 0;
    RUB utilities = 0;
    RUB utilities_base = 0;
    RUB dormitory = 0;
    RUB mortgage = 0;
    RUB mortgage_debt = 0;
    RUB rent = 0;
    RUB apartment_cost = 0;
    RUB mortgage_start_cost = 0;
    int side_job_length = 0;
    bool has_side_job = false;
    RUB laptop = 0;
    RUB laptop_cost = 0;
    int new_laptop_year = 0;
    int new_laptop_month = 0;
    int cars_bought_total = 0;
    int cheap_cars_bought = 0;
    int expensive_cars_bought = 0;
    int cheap_cars_sold = 0;
    int months_in_debt = 0;
    RUB debt_to_parents = 0;
    bool is_sick = false;
    int months_healthy = 0;
    RUB health_cost = 0;
    RUB health_cost_base = 0;
    int sickness_count = 0;
    bool used_property_deduction = false;
    RUB property_deduction = 0;
    RUB mortgage_interest_deduction = 0;
    RUB mortgage_interest_paid = 0;
    RUB mortgage_interest_deduction_total = 0;
    RUB medical_deduction = 0;
    RUB medical_paid_this_year = 0;
    int last_medical_deduction_year = 0;
    RUB kettle = 0;
    RUB kettle_cost = 0;
    int new_kettle_year = 0;
    int new_kettle_month = 0;
    RUB stove = 0;
    RUB stove_cost = 0;
    int new_stove_year = 0;
    int new_stove_month = 0;
    RUB fridge = 0;
    RUB fridge_cost = 0;
    int new_fridge_year = 0;
    int new_fridge_month = 0;
    RUB microwave = 0;
    RUB microwave_cost = 0;
    int new_microwave_year = 0;
    int new_microwave_month = 0;
    RUB iron = 0;
    RUB iron_cost = 0;
    int new_iron_year = 0;
    int new_iron_month = 0;
    RUB vacuum_cleaner = 0;
    RUB vacuum_cleaner_cost = 0;
    int new_vacuum_cleaner_year = 0;
    int new_vacuum_cleaner_month = 0;
    bool needs_appliances = false;
    bool has_pet = false;
    std::string pet_type = "";
    int pet_age_months = 0;
    int pet_lifespan_months = 0;
    int pet_danger_age_months = 0;
    RUB pet_buy = 0;
    RUB pet_food = 0;
    RUB pet_vet = 0;
    RUB pet_food_base = 0;
    int last_vet_year = 0;
    bool had_pet_before = false;
    int pets_total = 0;
    RUB small_grant = 0;
    RUB huge_grant = 0;
    RUB grant_income = 0;
    int grant_cooldown_until_year = 0;
    int grant_cooldown_until_month = 0;
    RUB transport = 0;
    RUB public_transport = 0;
    RUB public_transport_min = 0;
    RUB public_transport_max = 0;
    RUB driving_lessons = 0;
    RUB car_cost = 0;
    RUB car = 0;
    RUB gas = 0;
    RUB car_repair = 0;
    int new_car_year = 0;
    int new_car_month = 0;
    bool has_expensive_car = false;
    RUB expensive_car_cost = 0;
    RUB car_service_repair = 0;
    int driving_lessons_months = 0;
    bool has_driving_license = false;
    bool has_car = false;
    bool learning_to_drive = false;
    int mortgage_start_month = 0;
    int mortgage_start_year = 0;
    bool live_in_dormitory = false;
    bool has_mortgage = false;
    bool has_rent = false;
    bool has_apartment = false;
    RUB base_spend = 0;
};

struct Exam {
    std::string name;
    int score;
    int number;
};

struct ExamInfo {
    std::string name1, name2, name3;
    int score1 = 0, score2 = 0, score3 = 0;
};

/////////////////////////////////////////////////////////////////////////

Person ilya;

/////////////////////////////////////////////////////////////////////////

// основной генератор — для зависимых от профессии решений
std::mt19937 gen_main(std::random_device{}());

// общий генератор — для независимых решений
std::mt19937 gen_shared(12345);

std::mt19937& get_gen() { return gen_main; }
std::mt19937& get_gen_shared() { return gen_shared; }

bool success(int difficulty, int modificator) {
    std::uniform_int_distribution<> dist(1, 20);
    int result = dist(get_gen());

    if (result == 20) {
        return true;
    }
    if (result == 1) {
        return false;
    }
    return result >= difficulty + modificator;
}

bool success_shared(int difficulty, int modificator) {
    std::uniform_int_distribution<> dist(1, 20);
    int result = dist(get_gen_shared());

    if (result == 20) {
        return true;
    }
    if (result == 1) {
        return false;
    }
    return result >= difficulty + modificator;
}

/////////////////////////////////////////////////////////////////////////
// БЮДЖЕТ С УЧЁТОМ ДОЛГА
/////////////////////////////////////////////////////////////////////////

void pay(const RUB amount) {
    if (amount == 0) return;

    if (ilya.cash >= amount) {
        ilya.cash -= amount;
    }
    else {
        ilya.debt += amount - ilya.cash;
        ilya.cash = 0;
    }
}

void earn(const RUB amount) {
    if (amount == 0) return;

    if (ilya.debt == 0) {
        ilya.cash += amount;
        return;
    }

    if (amount >= ilya.debt) {
        ilya.cash += amount - ilya.debt;
        ilya.debt = 0;
    }
    else {
        ilya.debt -= amount;
    }
}

/////////////////////////////////////////////////////////////////////////

void choose_future_life(const std::vector<int>& best_numbers, std::string& profession) {
    int a = std::min(best_numbers[0], best_numbers[1]);
    int b = std::max(best_numbers[0], best_numbers[1]);

    if (a == 3 && b == 4) {
        profession = "programmer";
    }
    else if (a == 2 && b == 3) {
        profession = "engineer";
    }
    else if (a == 2 && b == 4) {
        profession = "theoretical physicist";
    }
    else if (a == 1 && b == 3) {
        profession = "web/game designer";
    }
    else if (a == 1 && b == 4) {
        profession = "economist";
    }
    else if (a == 1 && b == 2) {
        profession = "professor";
    }
    else {
        profession = "";
    }
}

/////////////////////////////////////////////////////////////////////////
ExamInfo exam_info;

void ilya_school_exam_results() {
    std::uniform_int_distribution<> dist(40, 100);
    Exam exams[4] = {
        {"humanities", dist(get_gen()), 1},
        {"physics",    dist(get_gen()), 2},
        {"IT",         dist(get_gen()), 3},
        {"math",       dist(get_gen()), 4}
    };

    std::sort(exams, exams + 4,
        [](const Exam& a, const Exam& b) {
            return a.score > b.score;
        });

    std::vector<int> best_exams = { exams[0].number, exams[1].number };
    std::vector<int> second_best_exams = { exams[0].number, exams[2].number };

    choose_future_life(best_exams, ilya.profession);
    choose_future_life(second_best_exams, ilya.alt_profession);

    exam_info.name1 = exams[0].name;
    exam_info.name2 = exams[1].name;
    exam_info.name3 = exams[2].name;
    exam_info.score1 = exams[0].score;
    exam_info.score2 = exams[1].score;
    exam_info.score3 = exams[3].score;
}
/////////////////////////////////////////////////////////////////////////
// ДОХОДЫ
/////////////////////////////////////////////////////////////////////////

void ilya_parents_support(const int year, const int month) {
    if (year == 2024 and month == 5) {
        ilya.parents_support = 20'000;
    }
    if (year == 2026 and month == 9) {
        ilya.parents_support = 0;
    }
}

void ilya_scholarship(const int year, const int month) {
    if (year == 2024 and month == 9) {
        ilya.scholarship = 6'000;
    }
    if (year == 2026 and month == 9) {
        ilya.scholarship = 8'000;
    }
    if (year == 2028 and month == 5) {
        ilya.scholarship = 0;
    }
}

void ilya_side_job() {
    ilya.side_job = 0;

    if (ilya.cash < ilya.base_spend || !ilya.has_apartment) {
        ilya.has_side_job = true;
    }
    else {
        ilya.has_side_job = false;
    }
    if (ilya.side_job_length >= 3) {
        ilya.side_job_max = RUB(ilya.side_job_max * 1.05);
        ilya.side_job_length = 0;
    }

    if (ilya.has_side_job) {
        std::uniform_int_distribution<RUB> dist(10'000, ilya.side_job_max);
        ilya.side_job = dist(get_gen());
        ilya.side_job_length++;
    }
    else {
        ilya.side_job = 0;
        ilya.side_job_length = 0;
    }
}

void ilya_salary(const int year, const int month) {
    if (year == 2028 and month == 9) {
        if (ilya.profession == "programmer") {
            ilya.salary = 75'000;
        }
        if (ilya.profession == "engineer") {
            ilya.salary = 55'000;
        }
        if (ilya.profession == "theoretical physicist") {
            ilya.salary = 35'000;
        }
        if (ilya.profession == "web/game designer") {
            ilya.salary = 65'000;
        }
        if (ilya.profession == "economist") {
            ilya.salary = 65'000;
        }
        if (ilya.profession == "professor") {
            ilya.salary = 40'000;
        }

    }
    if (year == 2029 and month == 9) {
        if (ilya.profession == "programmer") {
            ilya.salary = 140'000;
        }
        if (ilya.profession == "engineer") {
            ilya.salary = 75'000;
        }
        if (ilya.profession == "theoretical physicist") {
            ilya.salary = 45'000;
        }
        if (ilya.profession == "web/game designer") {
            ilya.salary = 100'000;
        }
        if (ilya.profession == "economist") {
            ilya.salary = 90'000;
        }
        if (ilya.profession == "professor") {
            ilya.salary = 65'000;
        }

    }
    if (year == 2031 and month == 9) {
        if (ilya.profession == "programmer") {
            ilya.salary = 190'000;
        }
        if (ilya.profession == "engineer") {
            ilya.salary = 140'000;
        }
        if (ilya.profession == "theoretical physicist") {
            ilya.salary = 95'000;
        }
        if (ilya.profession == "web/game designer") {
            ilya.salary = 110'000;
        }
        if (ilya.profession == "economist") {
            ilya.salary = 140'000;
        }
        if (ilya.profession == "professor") {
            ilya.salary = 85'000;
        }

    }

}

void ilya_grant(const int year, const int month) {
    ilya.grant_income = 0;

    bool after_graduation = (year > 2028 || (year == 2028 && month >= 9));
    bool right_month = (month == 4 || month == 10);
    bool right_profession =
        (ilya.profession == "professor" || ilya.profession == "theoretical physicist");

    if (!(after_graduation && right_month && right_profession)) return;

    if (year < ilya.grant_cooldown_until_year) return;
    if (year == ilya.grant_cooldown_until_year
        && month < ilya.grant_cooldown_until_month) return;

    bool got_huge = success(17, 0);
    bool got_small = success(10, 0);

    if (got_huge) {
        ilya.grant_income = ilya.huge_grant;
    }
    else if (got_small) {
        ilya.grant_income = ilya.small_grant;
    }
    else {
        return;
    }

    ilya.grant_cooldown_until_year = year + 1;
    ilya.grant_cooldown_until_month = month;
}

/////////////////////////////////////////////////////////////////////////
// ЗДОРОВЬЕ
/////////////////////////////////////////////////////////////////////////

// болезнь: шанс заболеть растёт с каждым здоровым месяцем.
// если заболел — платит за лечение, теряет 10% зарплаты и всю подработку.
void ilya_health() {
    ilya.health_cost = 0;

    if (ilya.is_sick) {
        ilya.is_sick = false;
        ilya.months_healthy = 0;
        return;
    }

    double chance = 2.0 + 1.0 * ilya.months_healthy;
    if (chance > 60.0) chance = 60.0;

    std::uniform_real_distribution<double> dist(0.0, 100.0);
    double roll = dist(get_gen_shared());

    if (roll < chance) {
        // заболел
        ilya.is_sick = true;
        ilya.sickness_count++;
        ilya.months_healthy = 0;

        ilya.health_cost = ilya.health_cost_base;

        // зарплата с основной работы минус 10%
        ilya.salary = RUB(ilya.salary * 90 / 100);

        // side_job обнуляется — Илья на больничном
        ilya.side_job = 0;
    }
    else {
        // не заболел — увеличиваем счётчик
        ilya.months_healthy++;
        ilya.health_cost_base = RUB(ilya.health_cost_base * 1.003);
    }
}

/////////////////////////////////////////////////////////////////////////
// НАЛОГИ
/////////////////////////////////////////////////////////////////////////

// имущественный вычет: разово при покупке жилья 13% от стоимости до 2 000 000 (максимум 260 000)
void ilya_property_deduction() {
    ilya.property_deduction = 0;
    if (!ilya.has_mortgage) return;
    if (ilya.used_property_deduction) return;

    RUB base = ilya.mortgage_start_cost;
    if (base > 2'000'000) base = 2'000'000;

    ilya.property_deduction = RUB(base * 13 / 100);
    ilya.used_property_deduction = true;
}

// вычет по процентам ипотеки: 13% от суммы до 3 000 000
void ilya_mortgage_interest_deduction() {
    ilya.mortgage_interest_deduction = 0;
    if (!ilya.has_mortgage) return;

    RUB interest = RUB(ilya.mortgage_debt * 0.005);
    ilya.mortgage_interest_paid += interest;
    RUB base = ilya.mortgage_interest_paid;

    if (base > 3'000'000) base = 3'000'000;

    RUB max_return = RUB(base * 13 / 100);

    RUB can_return = max_return - ilya.mortgage_interest_deduction_total;
    if (can_return > 0) {
        ilya.mortgage_interest_deduction = can_return;
        ilya.mortgage_interest_deduction_total += can_return;
    }
}

// социальный вычет за лечение: 13% от расходов, но не больше 120 000 в год
void ilya_medical_deduction(const int year) {
    ilya.medical_deduction = 0;

    ilya.medical_paid_this_year += ilya.health_cost;

    if (year == ilya.last_medical_deduction_year) return;

    RUB base = ilya.medical_paid_this_year;
    if (base > 120'000) base = 120'000;

    ilya.medical_deduction = RUB(base * 13 / 100);
    ilya.last_medical_deduction_year = year;

    ilya.medical_paid_this_year = 0;
}

void ilya_salary_tax() {
    ilya.salary_earned = ilya.salary + ilya.side_job;
    ilya.tax = RUB(ilya.salary_earned * 13 / 100);
}

/////////////////////////////////////////////////////////////////////////
// РАСХОДЫ
/////////////////////////////////////////////////////////////////////////

void ilya_food(const int month) {
    std::uniform_real_distribution<double> dist_inf(0.2, 0.5);
    double food_inflation = dist_inf(get_gen_shared());
    ilya.food_min = RUB(ilya.food_min * (100.0 + food_inflation) / 100.0);
    ilya.food_max = RUB(ilya.food_max * (100.0 + food_inflation) / 100.0);

    std::uniform_int_distribution<RUB> food_range(ilya.food_min, ilya.food_max);
    ilya.food = food_range(get_gen_shared());
}

void ilya_dormitory(const int year, const int month) {
    if (year == 2024 && month == 9) {
        ilya.live_in_dormitory = true;
        ilya.dormitory = 4'000;
    }
    if (year == 2028 && month == 5) {
        ilya.live_in_dormitory = false;
        ilya.dormitory = 0;
    }
}

void ilya_entertainment(const int month) {
    std::uniform_real_distribution<double> dist_inf(0.5, 2.0);
    double inf = dist_inf(get_gen_shared());
    ilya.entertainment_min = RUB(ilya.entertainment_min * (100.0 + inf) / 100.0);
    ilya.entertainment_max = RUB(ilya.entertainment_max * (100.0 + inf) / 100.0);

    std::uniform_int_distribution<RUB> range(ilya.entertainment_min, ilya.entertainment_max);
    ilya.entertainment = range(get_gen_shared());
}

void ilya_clothes(const int month) {
    std::uniform_real_distribution<double> dist_inf(0.1, 0.4);
    double inf = dist_inf(get_gen_shared());
    ilya.clothes_min = RUB(ilya.clothes_min * (100.0 + inf) / 100.0);
    ilya.clothes_max = RUB(ilya.clothes_max * (100.0 + inf) / 100.0);

    bool need_new_clothes = success_shared(18, 0);
    if (need_new_clothes) {
        std::uniform_int_distribution<RUB> range(ilya.clothes_min, ilya.clothes_max);
        ilya.clothes = range(get_gen_shared());
    }
}

void ilya_utilities(const int month) {
    std::uniform_real_distribution<double> dist_inf(0.1, 0.3);
    double inf = dist_inf(get_gen_shared());
    ilya.utilities_base = RUB(ilya.utilities_base * (100.0 + inf) / 100.0);

    if (ilya.live_in_dormitory) {
        // общежитие: коммуналка включена в 4 000
        ilya.utilities = 0;
    }
    else if (ilya.has_mortgage || ilya.has_apartment) {
        // своя квартира: полная коммуналка
        ilya.utilities = ilya.utilities_base;
    }
    else if (ilya.has_rent) {
        // аренда: половина (мелкий ремонт, счётчики)
        ilya.utilities = ilya.utilities_base / 2;
    }
    else {
        // живёт с родителями — ничего не платит
        ilya.utilities = 0;
    }
}

/////////////////////////////////////////////////////////////////////////
// ТЕХНИКА
/////////////////////////////////////////////////////////////////////////

// покупка ноутбука в начале обучения в вузе
void ilya_laptop_buy(const int year, const int month) {
    ilya.laptop = 0;
    if (year == 2024 && month == 9) {
        ilya.laptop = ilya.laptop_cost;
        ilya.new_laptop_year = year;
        ilya.new_laptop_month = month;
    }
}

// поломка ноутбука
void ilya_laptop_breakdown(const int year, const int month) {
    ilya.laptop = 0;
    ilya.laptop_cost *= 1.001;

    int difficulty = 19;

    bool in_IT_sphere =
        (year > 2028 || (year == 2028 && month >= 9)) &&
        (ilya.profession == "programmer" || ilya.profession == "web/game designer");
    bool in_tech_sphere =
        (year > 2028 || (year == 2028 && month >= 9)) &&
        (ilya.profession == "engineer" || ilya.profession == "theoretical physicist");

    int repair_bonus = 0;
    if (in_tech_sphere) {
        repair_bonus = 4;
    }

    if (in_IT_sphere) {
        bool roll1 = success(difficulty, 0);
        bool roll2 = success(difficulty, 0);
        if (!(roll1 || roll2)) return;
    }
    else {
        if (!success(difficulty, 0)) return;
    }

    int months_worked = (year - ilya.new_laptop_year) * 12
        + (month - ilya.new_laptop_month);

    if (months_worked < 12) {
        // гарантия: получает новый бесплатно
        ilya.new_laptop_year = year;
        ilya.new_laptop_month = month;
        return;
    }

    bool can_fix = success(15, repair_bonus);
    if (can_fix) {
        return;
    }
    else {
        ilya.laptop = ilya.laptop_cost;
        ilya.new_laptop_year = year;
        ilya.new_laptop_month = month;
    }
}

void appliance_breakdown(
    RUB price,
    RUB expense,
    int buy_year,
    int buy_month,
    const int year,
    const int month) {

    expense = 0;

    price = RUB(price * 1.005);

    int months_used = (year - buy_year) * 12 + (month - buy_month);

    double break_chance_percent = 0.5 + 0.05 * months_used;

    std::uniform_real_distribution<double> dist_break_chance(0.0, 100.0);
    double roll = dist_break_chance(get_gen());
    if (roll >= break_chance_percent) return;

    // гарантия 12 месяцев — замена бесплатно
    if (months_used < 12) {
        buy_year = year;
        buy_month = month;
        return;
    }

    int fix_bonus = 0;
    if (ilya.profession == "engineer") {
        fix_bonus = 3;
    }

    bool self_fix = success(16, fix_bonus);
    if (self_fix) {
        return;
    }

    bool service_fixed = success(14, 0);
    if (service_fixed) {
        // сервис починил — платит 30% от цены
        expense = RUB(price * 30 / 100);
        return;
    }

    // сервис не починил - деньги не тратятся, покупает новый
    expense = price;
    buy_year = year;
    buy_month = month;
}

void ilya_kettle_breakdown(const int year, const int month) {
    if (!ilya.has_rent) return;
    appliance_breakdown(ilya.kettle_cost, ilya.kettle,
        ilya.new_kettle_year, ilya.new_kettle_month,
        year, month);
}

void ilya_stove_breakdown(const int year, const int month) {
    if (!ilya.has_rent) return;
    appliance_breakdown(ilya.stove_cost, ilya.stove,
        ilya.new_stove_year, ilya.new_stove_month,
        year, month);
}

void ilya_fridge_breakdown(const int year, const int month) {
    if (!ilya.has_rent) return;
    appliance_breakdown(ilya.fridge_cost, ilya.fridge,
        ilya.new_fridge_year, ilya.new_fridge_month,
        year, month);
}

void ilya_microwave_breakdown(const int year, const int month) {
    if (!ilya.has_rent) return;
    appliance_breakdown(ilya.microwave_cost, ilya.microwave,
        ilya.new_microwave_year, ilya.new_microwave_month,
        year, month);
}

void ilya_iron_breakdown(const int year, const int month) {
    if (!ilya.has_rent) return;
    appliance_breakdown(ilya.iron_cost, ilya.iron,
        ilya.new_iron_year, ilya.new_iron_month,
        year, month);
}

void ilya_vacuum_cleaner_breakdown(const int year, const int month) {
    if (!ilya.has_rent) return;
    appliance_breakdown(ilya.vacuum_cleaner_cost, ilya.vacuum_cleaner,
        ilya.new_vacuum_cleaner_year, ilya.new_vacuum_cleaner_month,
        year, month);
}

// покупка техники при переезде в свою квартиру
void ilya_appliances_buy(const int year, const int month) {
    if (!ilya.needs_appliances) return;
    while (ilya.cash > ilya.base_spend * 3 / 2) {
        if (ilya.new_kettle_year == 0) {
            if (ilya.cash < ilya.kettle_cost) break;
            pay(ilya.kettle_cost);
            ilya.new_kettle_year = year;
            ilya.new_kettle_month = month;
            continue;
        }
        if (ilya.new_iron_year == 0) {
            if (ilya.cash < ilya.iron_cost) break;
            pay(ilya.iron_cost);
            ilya.new_iron_year = year;
            ilya.new_iron_month = month;
            continue;
        }
        if (ilya.new_microwave_year == 0) {
            if (ilya.cash < ilya.microwave_cost) break;
            pay(ilya.microwave_cost);
            ilya.new_microwave_year = year;
            ilya.new_microwave_month = month;
            continue;
        }
        if (ilya.new_vacuum_cleaner_year == 0) {
            if (ilya.cash < ilya.vacuum_cleaner_cost) break;
            pay(ilya.vacuum_cleaner_cost);
            ilya.new_vacuum_cleaner_year = year;
            ilya.new_vacuum_cleaner_month = month;
            continue;
        }
        if (ilya.new_stove_year == 0) {
            if (ilya.cash < ilya.stove_cost) break;
            pay(ilya.stove_cost);
            ilya.new_stove_year = year;
            ilya.new_stove_month = month;
            continue;
        }
        if (ilya.new_fridge_year == 0) {
            if (ilya.cash < ilya.fridge_cost) break;
            pay(ilya.fridge_cost);
            ilya.new_fridge_year = year;
            ilya.new_fridge_month = month;
            continue;
        }

        // всё куплено
        ilya.needs_appliances = false;
        break;
    }
}

/////////////////////////////////////////////////////////////////////////
// ПОМОЩЬ РОДИТЕЛЕЙ
/////////////////////////////////////////////////////////////////////////

// если в конце месяца есть долг, Илья может попросить у родителей
// не больше 100 000, чтобы погасить долг.
void ilya_ask_parents() {
    if (ilya.debt == 0) return;

    RUB request = ilya.debt;
    if (request > 100'000) request = 100'000;

    ilya.debt_to_parents += request;

    if (ilya.debt <= request) {
        ilya.debt = 0;
    }
    else {
        ilya.debt -= request;
    }
}

// если cash > 2 * base_spend — возвращает родителям часть долга
void ilya_repay_parents() {
    if (ilya.debt_to_parents == 0) return;
    if (ilya.cash <= ilya.base_spend * 2) return;

    RUB can_pay = ilya.cash - ilya.base_spend * 2;
    RUB to_pay = can_pay;
    if (to_pay > ilya.debt_to_parents) to_pay = ilya.debt_to_parents;

    ilya.cash -= to_pay;
    ilya.debt_to_parents -= to_pay;
}

/////////////////////////////////////////////////////////////////////////
// ПИТОМЕЦ
/////////////////////////////////////////////////////////////////////////

// покупка питомца: только когда есть своя квартира и нет долгов.
void ilya_pet_buy(const int year, const int month) {
    ilya.pet_buy = 0;
    if (ilya.has_pet) return;
    if (ilya.debt > 0) return;
    if (!ilya.has_apartment) return;

    RUB buy_cost = 5'000;
    if (ilya.cash < buy_cost + ilya.base_spend * 2) return;

    // случайный выбор: кошка или собака
    std::uniform_int_distribution<int> dist_choice(0, 1);
    int choice = dist_choice(get_gen());

    ilya.pet_buy = buy_cost;
    ilya.has_pet = true;
    ilya.pet_age_months = 0;
    ilya.last_vet_year = 0;
    ilya.had_pet_before = true;
    ilya.pets_total++;

    if (choice == 0) {
        ilya.pet_type = "cat";
        ilya.pet_lifespan_months = 180;
        ilya.pet_danger_age_months = 144;
        ilya.pet_food_base = 3'000;
    }
    else {
        ilya.pet_type = "dog";
        ilya.pet_lifespan_months = 144;
        ilya.pet_danger_age_months = 108;
        ilya.pet_food_base = 5'000;
    }
}

void ilya_pet_food(const int month) {
    ilya.pet_food = 0;
    if (!ilya.has_pet) return;

    std::uniform_real_distribution<double> dist_inf(0.2, 0.5);
    double inf = dist_inf(get_gen());
    ilya.pet_food_base = RUB(ilya.pet_food_base * (100.0 + inf) / 100.0);

    ilya.pet_food = ilya.pet_food_base;
}

void ilya_pet_vet(const int year, const int month) {
    ilya.pet_vet = 0;
    if (!ilya.has_pet) return;

    // прививка раз в год
    if (year != ilya.last_vet_year) {
        if (ilya.pet_type == "cat") {
            ilya.pet_vet += 3'000;
        }
        else {
            ilya.pet_vet += 5'000;
        }
        ilya.last_vet_year = year;
    }

    // базовый шанс болезни
    double chance = 0.5 + 0.05 * (ilya.pet_age_months / 12.0);

    // резкое повышение в опасном возрасте
    if (ilya.pet_age_months >= ilya.pet_danger_age_months) {
        int months_over = ilya.pet_age_months - ilya.pet_danger_age_months;
        chance += 3.0 * months_over;
    }

    std::uniform_real_distribution<double> dist(0.0, 100.0);
    double roll = dist(get_gen());
    if (roll < chance) {
        ilya.pet_vet += 10'000;
    }
}

void ilya_pet_age() {
    if (!ilya.has_pet) return;

    double death_chance = 0.00001 * ilya.pet_age_months;
    ilya.pet_age_months++;

    // резкое повышение шанса смерти в опасном возрасте
    if (ilya.pet_age_months >= ilya.pet_danger_age_months) {
        int months_over = ilya.pet_age_months - ilya.pet_danger_age_months;
        double death_chance = 1.0 + 2.0 * months_over;

        std::uniform_real_distribution<double> dist(0.0, 100.0);
        double roll = dist(get_gen());
        if (roll < death_chance) {
            ilya.has_pet = false;
            ilya.pet_age_months = 0;
        }
    }
}

/////////////////////////////////////////////////////////////////////////
// ТРАНСПОРТ
/////////////////////////////////////////////////////////////////////////

void ilya_transport(const int year, const int month) {
    ilya.transport = 0;
    ilya.car = 0;

    // общественный транспорт
    if (!ilya.has_car) {
        std::uniform_int_distribution<RUB> dist(
            ilya.public_transport_min, ilya.public_transport_max);
        ilya.public_transport = dist(get_gen());

        // 2 раза в год (январь и июль) цена растёт на 10%
        if (month == 1 || month == 7) {
            ilya.public_transport_min = RUB(ilya.public_transport_min * 1.10);
            ilya.public_transport_max = RUB(ilya.public_transport_max * 1.10);
        }

        ilya.transport += ilya.public_transport;
    }

    // обучение на права
    if (year == 2026 && month == 9) {
        ilya.learning_to_drive = true;
        ilya.driving_lessons_months = 0;
    }

    if (ilya.learning_to_drive) {
        ilya.driving_lessons_months++;
        RUB monthly_payment = 40'000 / 3;
        ilya.driving_lessons += monthly_payment;
        ilya.transport += monthly_payment;

        if (ilya.driving_lessons_months >= 3) {
            ilya.learning_to_drive = false;
        }
    }

    // сдача на права
    if (!ilya.has_driving_license
        && !ilya.learning_to_drive
        && year > 2026) {
        bool passed = success(12, 0);
        if (passed) {
            ilya.has_driving_license = true;
        }
    }
}

// покупка машины
void ilya_car_buy(const int year, const int month) {
    // апгрейд
    if (ilya.has_car && !ilya.has_expensive_car && ilya.debt == 0) {
        RUB sale_price = ilya.car_cost / 2;
        if (ilya.cash + sale_price >= ilya.expensive_car_cost + ilya.base_spend * 6) {
            // продаёт дешёвую
            ilya.cash += sale_price;
            ilya.has_car = false;
            ilya.has_expensive_car = false;
            ilya.cheap_cars_sold++;

            // покупает дорогую
            ilya.car = ilya.expensive_car_cost;
            ilya.transport += ilya.car;
            ilya.has_car = true;
            ilya.has_expensive_car = true;
            ilya.gas *= 1.5;
            ilya.new_car_year = year;
            ilya.new_car_month = month;
            ilya.cars_bought_total++;
            ilya.expensive_cars_bought++;
            return;
        }
        return;
    }

    if (ilya.has_car) return;
    if (!ilya.has_driving_license) return;
    if (ilya.debt > 0) return;

    // первая покупка машины
    if (ilya.cash >= ilya.expensive_car_cost + ilya.base_spend * 6) {
        ilya.car = ilya.expensive_car_cost;
        ilya.transport += ilya.car;
        ilya.has_car = true;
        ilya.has_expensive_car = true;
        ilya.gas = 5'000;
        ilya.new_car_year = year;
        ilya.new_car_month = month;
        ilya.cars_bought_total++;
        ilya.expensive_cars_bought++;
        return;
    }

    if (ilya.cash >= ilya.car_cost + ilya.base_spend) {
        ilya.car = ilya.car_cost;
        ilya.transport += ilya.car;
        ilya.has_car = true;
        ilya.has_expensive_car = false;
        ilya.gas = 5'000;
        ilya.new_car_year = year;
        ilya.new_car_month = month;
        ilya.cars_bought_total++;
        ilya.cheap_cars_bought++;
    }
}

void ilya_gas() {
    if (!ilya.has_car) return;

    ilya.gas = RUB(ilya.gas * 1.015);
    ilya.transport += ilya.gas;
}

void ilya_car_breakdown(const int year, const int month) {
    ilya.car_repair = 0;
    ilya.car_service_repair = 0;

    // инфляция стоимости дешёвой машины — каждый месяц
    ilya.car_cost = RUB(ilya.car_cost * 1.001);

    // инфляция стоимости дорогой машины — рандомно раз в несколько месяцев
    std::uniform_int_distribution<int> dist_month(1, 6);
    if (dist_month(get_gen()) == 1) {
        std::uniform_real_distribution<double> dist_inf(3.0, 7.0);
        double percent = dist_inf(get_gen());
        ilya.expensive_car_cost = RUB(ilya.expensive_car_cost * (100.0 + percent) / 100.0);
    }

    if (!ilya.has_car) return;

    int months_worked = (year - ilya.new_car_year) * 12 + (month - ilya.new_car_month);

    // дорогая машина
    if (ilya.has_expensive_car) {
        // гарантия 18 месяцев
        if (months_worked < 18) return;

        // базовый шанс поломки 0.1% + 0.01% за каждый месяц
        double break_chance_percent = 0.1 + 0.01 * (months_worked);

        std::uniform_real_distribution<double> dist_break_chance(0.0, 100.0);
        double roll = dist_break_chance(get_gen());
        if (roll >= break_chance_percent) return;

        // стоимость ремонта в сервисе
        RUB base_repair = ilya.expensive_car_cost / 100;
        RUB extra = RUB(ilya.expensive_car_cost * (months_worked - 18) / 800);
        ilya.car_service_repair = base_repair + extra;
        return;
    }

    // дешевая машина
    // базовый шанс поломки 1% + 0.1% за каждый месяц эксплуатации
    double break_chance_percent = 1.0 + 0.1 * months_worked;

    std::uniform_real_distribution<double> dist_break_chance(0.0, 100.0);
    double roll = dist_break_chance(get_gen());
    if (roll >= break_chance_percent) return;

    // пытается починить сам
    int repair_bonus = 0;
    if (ilya.profession == "engineer") {
        repair_bonus = 3;
    }

    bool can_fix = success(17, repair_bonus);
    if (can_fix) {
        return;
    }
    else {
        // не смог починить — покупает новую
        ilya.car_repair = ilya.car_cost;
        ilya.cheap_cars_bought++;
        ilya.cars_bought_total++;
        ilya.new_car_year = year;
        ilya.new_car_month = month;
    }
}

// продажа машины при долге
void ilya_car_sell_for_debt(const int year, const int month) {
    if (!ilya.has_car) return;
    if (ilya.has_expensive_car) return;   // дорогую машину не продаём
    if (ilya.debt <= 100'000) return;

    // не продаём машину в первые 3 месяца после покупки
    int months_worked = (year - ilya.new_car_year) * 12 + (month - ilya.new_car_month);
    if (months_worked < 3) return;

    RUB sale_price = ilya.car_cost / 2;
    ilya.cheap_cars_sold++;

    ilya.cash += sale_price;
    ilya.has_car = false;
    ilya.has_expensive_car = false;
    ilya.gas = 0;

    if (ilya.cash >= ilya.debt) {
        ilya.cash -= ilya.debt;
        ilya.debt = 0;
    }
    else {
        ilya.debt -= ilya.cash;
        ilya.cash = 0;
    }
}

// счетчик месяцев в долгу
void ilya_count_debt_months() {
    if (ilya.debt > 0) {
        ilya.months_in_debt++;
    }
}

/////////////////////////////////////////////////////////////////////////
// ЖИЛЬЁ: ИПОТЕКА ИЛИ АРЕНДА
/////////////////////////////////////////////////////////////////////////

// стоимость квартиры растёт раз в год, пока ипотека не взята
void ilya_apartment_cost(const int month) {
    if (month == 1 && !ilya.has_mortgage) {
        std::uniform_real_distribution<double> dist_inf(6.0, 12.0);
        double percent = dist_inf(get_gen());
        ilya.apartment_cost = RUB(ilya.apartment_cost * (100.0 + percent) / 100.0);
    }
}

// попытка взять ипотеку
void ilya_try_mortgage(const int year, const int month) {
    if (ilya.has_mortgage) return;

    RUB first_payment = ilya.apartment_cost / 5;   // 20% стоимости
    RUB needed = first_payment + ilya.base_spend;

    if (ilya.cash >= needed && !ilya.live_in_dormitory && ilya.debt == 0) {
        // хватает — берёт ипотеку
        ilya.cash -= first_payment;
        ilya.mortgage_debt = ilya.apartment_cost - first_payment;
        ilya.mortgage = ilya.mortgage_debt / 120;   // 10 лет
        ilya.has_mortgage = true;
        ilya.has_rent = false;
        ilya.rent = 0;
        ilya.mortgage_start_cost = ilya.apartment_cost;
        ilya.mortgage_start_month = month;
        ilya.mortgage_start_year = year;

        // переезд в свою квартиру — техника нужна заново
        ilya.needs_appliances = true;
        ilya.new_kettle_year = 0;
        ilya.new_kettle_month = 0;
        ilya.new_stove_year = 0;
        ilya.new_stove_month = 0;
        ilya.new_fridge_year = 0;
        ilya.new_fridge_month = 0;
        ilya.new_microwave_year = 0;
        ilya.new_microwave_month = 0;
        ilya.new_iron_year = 0;
        ilya.new_iron_month = 0;
        ilya.new_vacuum_cleaner_year = 0;
        ilya.new_vacuum_cleaner_month = 0;
    }
    else if (!ilya.has_rent && year >= 2028) {
        // не хватает — снимает квартиру
        ilya.rent = 30'000;
        ilya.has_rent = true;
    }

    if (ilya.has_rent && month == 1) ilya.rent += 3'000;
}

void ilya_mortgage_payment() {
    if (ilya.has_apartment) {
        ilya.mortgage = 0;
        return;
    }

    if (!ilya.has_mortgage) return;
    if (ilya.mortgage_debt <= ilya.cash + ilya.base_spend) {
        ilya.mortgage = ilya.mortgage_debt;
        ilya.mortgage_debt = 0;
        ilya.has_mortgage = false;
        ilya.has_apartment = true;
    }
    else {
        ilya.mortgage_debt -= ilya.mortgage;
        ilya.mortgage_debt *= 1.005;
    }
}

/////////////////////////////////////////////////////////////////////////

// обновление бюджета за месяц
void ilya_cash() {
    // доходы
    earn(ilya.scholarship);
    earn(ilya.parents_support);
    earn(ilya.salary);
    earn(ilya.side_job);
    earn(ilya.grant_income);

    // налоговые вычеты
    earn(ilya.property_deduction);
    earn(ilya.mortgage_interest_deduction);
    earn(ilya.medical_deduction);

    // расходы
    pay(ilya.tax);
    pay(ilya.mortgage);
    pay(ilya.rent);
    pay(ilya.food);
    pay(ilya.dormitory);
    pay(ilya.entertainment);
    pay(ilya.clothes);
    pay(ilya.utilities);
    pay(ilya.health_cost);
    pay(ilya.laptop);
    pay(ilya.car_repair);
    pay(ilya.car_service_repair);
    pay(ilya.kettle);
    pay(ilya.stove);
    pay(ilya.fridge);
    pay(ilya.microwave);
    pay(ilya.iron);
    pay(ilya.vacuum_cleaner);
    pay(ilya.pet_buy);
    pay(ilya.pet_food);
    pay(ilya.pet_vet);
    pay(ilya.transport);
}

void ilya_base_spend() {
    ilya.base_spend = ilya.tax + ilya.mortgage + ilya.rent + ilya.food + ilya.dormitory +
        ilya.entertainment + ilya.clothes + ilya.utilities + ilya.transport +
        ilya.health_cost + ilya.pet_food + ilya.pet_vet;
}

void ilya_init() {
    ilya.cash = 100'000;
    ilya.side_job_max = 50'000;
    ilya.food_min = 8'000;
    ilya.food_max = 12'000;
    ilya.entertainment_min = 8'000;
    ilya.entertainment_max = 10'000;
    ilya.clothes_min = 1'000;
    ilya.clothes_max = 3'000;
    ilya.utilities_base = 5'000;
    ilya.apartment_cost = 5'000'000;
    ilya.laptop_cost = 80'000;
    ilya.health_cost_base = 10'000;
    ilya.kettle_cost = 3'000;
    ilya.stove_cost = 25'000;
    ilya.fridge_cost = 40'000;
    ilya.microwave_cost = 8'000;
    ilya.iron_cost = 4'000;
    ilya.vacuum_cleaner_cost = 15'000;
    ilya.small_grant = 50'000;
    ilya.huge_grant = 300'000;
    ilya.public_transport_min = 2'000;
    ilya.public_transport_max = 3'000;
    ilya.car_cost = 200'000;
    ilya.expensive_car_cost = 1'000'000;
}

/////////////////////////////////////////////////////////////////////////

void simulation() {
    int year = 2024, month = 5;

    while (!(year == 2040 && month == 9)) {
        ilya_parents_support(year, month);
        ilya_scholarship(year, month);
        ilya_side_job();
        ilya_salary(year, month);
        ilya_health();
        ilya_food(month);
        ilya_dormitory(year, month);
        ilya_entertainment(month);
        ilya_clothes(month);
        ilya_utilities(month);
        ilya_salary_tax();
        ilya_grant(year, month);
        ilya_laptop_buy(year, month);
        ilya_laptop_breakdown(year, month);
        ilya_car_breakdown(year, month);
        ilya_transport(year, month);
        ilya_car_buy(year, month);
        ilya_gas();
        ilya_kettle_breakdown(year, month);
        ilya_stove_breakdown(year, month);
        ilya_fridge_breakdown(year, month);
        ilya_microwave_breakdown(year, month);
        ilya_iron_breakdown(year, month);
        ilya_vacuum_cleaner_breakdown(year, month);
        ilya_pet_buy(year, month);
        ilya_pet_food(month);
        ilya_pet_vet(year, month);
        ilya_base_spend();
        ilya_mortgage_payment();
        ilya_property_deduction();
        ilya_mortgage_interest_deduction();
        ilya_medical_deduction(year);

        ilya_cash();

        ilya_appliances_buy(year, month);
        ilya_ask_parents();
        ilya_repay_parents();
        ilya_count_debt_months();
        ilya_car_sell_for_debt(year, month);
        ilya_apartment_cost(month);
        ilya_try_mortgage(year, month);
        ilya_pet_age();

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

/////////////////////////////////////////////////////////////////////////

void print() {
    printf("best exams: %s (%d), %s (%d), %s (%d)\n",
        exam_info.name1.c_str(), exam_info.score1,
        exam_info.name2.c_str(), exam_info.score2,
        exam_info.name3.c_str(), exam_info.score3);
    printf("chosen profession: %s\n", ilya.profession.c_str());
    printf("altenative profession: %s\n", ilya.alt_profession.c_str());
    printf("has mortgage %d\n", ilya.has_mortgage ? 1 : 0);
    printf("has rent %d\n", ilya.has_rent ? 1 : 0);
    printf("has driving license %d\n", ilya.has_driving_license ? 1 : 0);
    printf("has car %d\n", ilya.has_car ? 1 : 0);

    printf("cars bought total: %d\n", ilya.cars_bought_total);
    printf("  cheap cars bought: %d\n", ilya.cheap_cars_bought);
    printf("  expensive cars bought: %d\n", ilya.expensive_cars_bought);
    printf("cheap cars sold: %d\n", ilya.cheap_cars_sold);
    printf("current car: %s\n",
        ilya.has_car ? (ilya.has_expensive_car ? "expensive" : "cheap") : "none");

    printf("months in debt: %d\n", ilya.months_in_debt);
    printf("debt to parents: %llu\n", ilya.debt_to_parents);

    printf("pets total: %d\n", ilya.pets_total);
    printf("has pet: %d\n", ilya.has_pet ? 1 : 0);
    if (ilya.has_pet) {
        printf("  pet type: %s\n", ilya.pet_type.c_str());
        printf("  pet age (months): %d\n", ilya.pet_age_months);
    }

    printf("sickness count: %d\n", ilya.sickness_count);
    printf("months healthy (last streak): %d\n", ilya.months_healthy);

    if (ilya.mortgage_start_year != 0) {
        printf("mortgage taken: %02d.%d\n",
            ilya.mortgage_start_month, ilya.mortgage_start_year);
        printf("the apartment cost, when the mortgage was taken out: %llu\n",
            ilya.mortgage_start_cost);
    }
    else {
        printf("mortgage was not taken\n");
    }

    printf("mortgage debt: %llu\n", ilya.mortgage_debt);
    printf("debt: %llu\n", ilya.debt);
    printf("final cash: %llu\n", ilya.cash);
}

// сброс состояния

void reset_ilya(const std::string& prof, const std::string& alt_prof) {
    ilya.profession = prof;
    ilya.alt_profession = alt_prof;
}

/////////////////////////////////////////////////////////////////////////

int main() {
    ilya_init();

    ilya_school_exam_results();

    std::string main_prof = ilya.profession;
    std::string alt_prof = ilya.alt_profession;

    // СИМУЛЯЦИЯ 1: основная профессия
    gen_shared.seed(12345);
    reset_ilya(main_prof, alt_prof);
    simulation();

    printf("=== SIMULATION 1 (main profession: %s) ===\n", main_prof.c_str());
    print();

    // СИМУЛЯЦИЯ 2: альтернативная профессия
    gen_shared.seed(12345);
    reset_ilya(alt_prof, main_prof);
    simulation();

    printf("\n=== SIMULATION 2 (alternative profession: %s) ===\n", alt_prof.c_str());
    print();
}
