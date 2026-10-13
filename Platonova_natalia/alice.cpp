#include <iostream>
#include <string>
#include <random>
#include <cstdio>
#include <windows.h>
#include <locale>
#include <vector>    
#include <algorithm> 


//typedef int RUB;
using RUB = unsigned long long int;

std::random_device rd;
std::mt19937 gen(rd());

bool check_chance(int chance_percent) {
    std::uniform_int_distribution<> distrib(1, 100);
    return distrib(gen) <= chance_percent;
}

enum class LifeStatus {
    SCHOOL,
    COLLEGE,
    UNIVERSITY,
    WORKING
};

enum class EducationLevel {
    NONE,           
    SCHOOL,         
    COLLEGE,        
    UNIVERSITY      
};

enum class VolatilityType {
    LOW,        
    MEDIUM,     
    HIGH,       
    VERY_HIGH   
};

enum class DividendPolicy {
    NONE,           // не платят 
    LOW,            //1-2% годовых
    MEDIUM,         //3-5%
    HIGH,           //6-10%
    VERY_HIGH       //10%+ 
};

enum class EntertainmentType {
    CINEMA,         
    BOOKS,          
    GYM,            
    CAFE,           
    CONCERT,        
    HOBBY,          
    SHOPPING,       
    TRAVEL          
};

enum class MoodLevel {
    DEPRESSED,      // 0-20
    SAD,            // 21-40
    NEUTRAL,        // 41-60
    GOOD,           // 61-80
    HAPPY,          // 81-100
    ECSTATIC        // 100+
};

struct Car {
    bool has_car = false;
    std::string model = "Отсутствует";
    int health = 100;              
    int age_months = 0;            


    RUB purchase_price = 0;
    RUB monthly_cost = 0;         

    RUB light_repair_cost = 15000;     
    RUB heavy_repair_cost = 50000;     
    RUB capital_repair_cost = 150000;  
};

struct Mortgage {
    bool is_active = false;
    RUB total_amount = 0;
    RUB remaining_balance = 0;
    double annual_rate = 0.0;
    int term_months = 0;
    RUB monthly_payment = 0;
    int months_paid = 0;
};

struct Loan {
    bool is_active = false;
    RUB total_amount = 0;
    RUB remaining_balance = 0;
    double annual_rate = 0.0;
    int term_months = 0;
    RUB monthly_payment = 0;
    int months_paid = 0;
};

struct TaxSystem {
    double income_tax_rate = 0.13;       
    double income_tax_rate_high = 0.15; 
    RUB annual_income_threshold = 2400000;  

    RUB total_tax_paid = 0;
    RUB income_this_year = 0;           

    double property_tax_rate = 0.001;
    RUB property_tax_annual = 0;
    bool has_property_tax = false;

    RUB vehicle_tax_annual = 0;
    bool has_vehicle_tax = false;

    double pension_contribution_rate = 0.22;
    RUB total_pension_contributions = 0;

    bool property_deduction_claimed = false;
    RUB property_deduction_amount = 0;
    RUB medical_deduction_received = 0;
    RUB total_deductions_received = 0;

    RUB calculate_income_tax(RUB gross_salary) {
        income_this_year += gross_salary;

        RUB tax = 0;

        if (income_this_year <= annual_income_threshold) {
            tax = static_cast<RUB>(gross_salary * income_tax_rate);
        }
        else if (income_this_year - gross_salary >= annual_income_threshold) {
            tax = static_cast<RUB>(gross_salary * income_tax_rate_high);
        }
        else {
            RUB remaining_threshold = annual_income_threshold - (income_this_year - gross_salary);
            RUB excess = gross_salary - remaining_threshold;

            RUB tax_base = static_cast<RUB>(remaining_threshold * income_tax_rate);
            RUB tax_excess = static_cast<RUB>(excess * income_tax_rate_high);

            tax = tax_base + tax_excess;
        }

        return tax;
    }

    void reset_annual_income() {
        income_this_year = 0;
    }

    RUB calculate_property_tax(RUB property_value) const {
        return static_cast<RUB>(property_value * property_tax_rate);
    }

    RUB calculate_vehicle_tax(int horse_power) const {
        return static_cast<RUB>(horse_power * 50);
    }
};


struct Entertainment {
    std::string name;
    EntertainmentType type;
    RUB cost;
    int happiness_boost;
    int frequency_months;
    int last_enjoyed_month = 0;
    int last_enjoyed_year = 0;
    int total_times = 0;
    RUB total_spent = 0;
};

struct EntertainmentSystem {
    int happiness_level = 50;
    int base_happiness = 50;

    RUB monthly_budget = 0;
    double budget_percent = 0.08;

    RUB total_spent_on_fun = 0;
    RUB total_spent_on_travel = 0;

    int vacations_taken = 0;
    int last_vacation_year = 0;

    std::vector<Entertainment> entertainments;
};

struct Employer {
    RUB base_salary = 0;                    

    double bonus_chance = 0.10;             
    RUB bonus_amount = 0;                   
    RUB total_bonuses_paid = 0;             

    double pension_rate = 0.22;             
    RUB total_pension_contributions = 0;    

    RUB calculate_gross_salary(bool check_bonus) {
        RUB gross = base_salary;

        if (check_bonus && check_chance(static_cast<int>(bonus_chance * 100))) {
            gross += bonus_amount;
            total_bonuses_paid += bonus_amount;
        }

        return gross;
    }

    RUB calculate_pension_contribution(RUB gross_salary) {
        RUB contribution = static_cast<RUB>(gross_salary * pension_rate);
        total_pension_contributions += contribution;
        return contribution;
    }
};

struct SecondEmployer {
    std::string company_name = "ИП";
    RUB base_salary = 0;                   

    double bonus_chance = 0.05;             
    RUB bonus_amount = 0;                
    RUB total_bonuses_paid = 0;             

    double pension_rate = 0.22;           
    RUB total_pension_contributions = 0;   

    RUB calculate_gross_salary(bool check_bonus) {
        RUB gross = base_salary;

        if (check_bonus && check_chance(static_cast<int>(bonus_chance * 100))) {
            gross += bonus_amount;
            total_bonuses_paid += bonus_amount;
        }

        return gross;
    }

    RUB calculate_pension_contribution(RUB gross_salary) {
        RUB contribution = static_cast<RUB>(gross_salary * pension_rate);
        total_pension_contributions += contribution;
        return contribution;
    }
};

struct Bank {
    std::string name;

    const RUB mortgage_amount = 9000000;
    const double mortgage_rate = 0.0;      
    const int mortgage_term_years = 30;
    const RUB mortgage_monthly_payment = 0;

    const RUB loan_amount = 150000;
    const double loan_rate = 0.0;         
    const int loan_term_months = 24;
    const RUB loan_monthly_payment = 0;  

    Mortgage mortgage;
    Loan loan;
};


struct Utilities {
    RUB electricity = 1500;   
    RUB water = 800;         
    RUB heating = 2000;       
    RUB gas = 500;           
    RUB internet = 500;      
    RUB trash = 300;          
    RUB maintenance = 1500;   

    double annual_inflation = 0.03;  

    RUB get_total_monthly_cost(bool is_winter) const {
        RUB total = electricity + water + gas + internet + trash + maintenance;
        if (is_winter) {
            total += heating * 2;
        }
        else {
            total += heating;
        }
        return total;
    }
};

struct Dog {
    bool has_dog = false;
    std::string name = "Нет собаки";
    std::string breed = "Отсутствует";

    int age_months = 0;              
    int max_age_months = 180;        
    int health = 100;                

    RUB purchase_price = 0;          
    RUB monthly_food_cost = 3000;    
    RUB monthly_vet_cost = 1000;     
    RUB emergency_vet_cost = 15000;  

    int happiness_bonus = 5;        

    int get_age_years() const {
        return age_months / 12;
    }

    RUB get_monthly_cost() const {
        return monthly_food_cost + monthly_vet_cost;
    }
};

struct HealthSystem {
    int health_points = 100;         
    int max_health = 100;          

    int total_medical_expenses = 0;  
    int sick_days = 0;               
    bool is_sick = false;           

    RUB doctor_visit = 3000;        
    RUB medicine_cost = 2000;        
    RUB hospital_cost = 50000;       
    RUB surgery_cost = 200000;    

    RUB medical_expenses_this_year = 0;

    int calculate_sickness_chance(int age_years, int health) const {
        int base_chance = 2;  

        if (age_years > 40) base_chance += (age_years - 40) / 5;
        if (age_years > 60) base_chance += (age_years - 60) / 3;

        if (health < 50) base_chance += 5;
        if (health < 30) base_chance += 10;

        return (std::min)(base_chance, 50); 
    }
};

struct SavingsAccount {
    RUB balance = 0;                   
    double annual_rate = 0.12;         
    RUB total_deposited = 0;           
    RUB total_withdrawn = 0;            
    RUB total_interest_earned = 0;      

    void add_monthly_interest() {
        double monthly_rate = annual_rate / 12.0;  
        RUB interest = static_cast<RUB>(balance * monthly_rate);
        balance += interest;
        total_interest_earned += interest;
    }

    void deposit(RUB amount) {
        balance += amount;
        total_deposited += amount;
    }

    bool withdraw(RUB amount) {
        if (balance >= amount) {
            balance -= amount;
            total_withdrawn += amount;
            return true;
        }
        return false;
    }
};

struct Stock {
    std::string name;
    std::string ticker;
    RUB current_price = 0;
    RUB initial_price = 0;
    RUB base_price = 0;

    int shares_owned = 0;
    RUB total_invested = 0;
    RUB total_dividends = 0;

    VolatilityType volatility = VolatilityType::MEDIUM;
    double growth_chance = 40;
    double decline_chance = 35;
    double strong_growth_chance = 15;
    double strong_decline_chance = 10;

    double min_growth = 1.0;
    double max_growth = 4.0;
    double min_decline = -4.0;
    double max_decline = -1.0;
    double strong_growth_min = 5.0;
    double strong_growth_max = 12.0;
    double strong_decline_min = -12.0;
    double strong_decline_max = -5.0;

    DividendPolicy dividend_policy = DividendPolicy::NONE;
    double dividend_yield = 0.0;
    int dividend_months = 0;
    bool pays_dividends = false;
};


struct Bond {
    std::string name;
    std::string ticker;
    RUB face_value;
    RUB current_price;
    double coupon_rate;
    int coupon_months;
    int years_to_maturity;

    int bonds_owned = 0;
    RUB total_invested = 0;
    RUB total_coupons = 0;

    Bond(std::string n, std::string t, RUB face_val, double coup_rate, int coup_months, int maturity)
        : name(n), ticker(t), face_value(face_val), current_price(face_val),
        coupon_rate(coup_rate), coupon_months(coup_months), years_to_maturity(maturity) {
    }
};

struct Portfolio {
    std::vector<Stock> stocks;
    std::vector<Bond> bonds;

    RUB total_stock_value = 0;
    RUB total_bond_value = 0;
    RUB total_dividends_received = 0;
    RUB total_coupons_received = 0;
    RUB total_profit = 0;
    bool is_active = false;
    int months_active = 0;


    static Stock create_stock(std::string name, std::string ticker, RUB price,
        VolatilityType vol, DividendPolicy div_policy,
        double g_chance, double d_chance, double sg_chance, double sd_chance,
        double min_g, double max_g, double min_d, double max_d,
        double sg_min, double sg_max, double sd_min, double sd_max,
        double div_yield, int div_months) {
        Stock s;
        s.name = name;
        s.ticker = ticker;
        s.current_price = price;
        s.initial_price = price;
        s.base_price = price;
        s.volatility = vol;
        s.dividend_policy = div_policy;
        s.growth_chance = g_chance;
        s.decline_chance = d_chance;
        s.strong_growth_chance = sg_chance;
        s.strong_decline_chance = sd_chance;
        s.min_growth = min_g;
        s.max_growth = max_g;
        s.min_decline = min_d;
        s.max_decline = max_d;
        s.strong_growth_min = sg_min;
        s.strong_growth_max = sg_max;
        s.strong_decline_min = sd_min;
        s.strong_decline_max = sd_max;
        s.dividend_yield = div_yield;
        s.dividend_months = div_months;
        s.pays_dividends = (div_policy != DividendPolicy::NONE);
        return s;
    }

    void init_stocks() {
        stocks.push_back(create_stock(
            "Сбербанк", "SBER", 287,
            VolatilityType::MEDIUM, DividendPolicy::MEDIUM,
            45, 30, 15, 10,
            1.0, 4.0, -4.0, -1.0,
            5.0, 12.0, -12.0, -5.0,
            4.5, 7
        ));

        stocks.push_back(create_stock(
            "Газпром", "GAZP", 104,
            VolatilityType::LOW, DividendPolicy::HIGH,
            40, 35, 15, 10,
            0.5, 3.0, -3.0, -0.5,
            3.0, 10.0, -10.0, -3.0,
            7.0, 6
        ));

        stocks.push_back(create_stock(
            "Роснефть", "ROSN", 378,
            VolatilityType::MEDIUM, DividendPolicy::MEDIUM,
            42, 33, 15, 10,
            1.0, 4.5, -4.5, -1.0,
            5.0, 13.0, -13.0, -5.0,
            5.5, 12
        ));

        stocks.push_back(create_stock(
            "Яндекс", "YNDX", 3488,
            VolatilityType::HIGH, DividendPolicy::NONE,
            35, 35, 20, 10,
            2.0, 8.0, -8.0, -2.0,
            10.0, 25.0, -25.0, -10.0,
            0.0, 0
        ));

        stocks.push_back(create_stock(
            "Золото", "GOLD", 4202,
            VolatilityType::LOW, DividendPolicy::NONE,
            50, 30, 12, 8,
            0.5, 2.0, -2.0, -0.5,
            3.0, 8.0, -8.0, -3.0,
            0.0, 0
        ));

        stocks.push_back(create_stock(
            "Озон", "OZON", 3455,
            VolatilityType::VERY_HIGH, DividendPolicy::NONE,
            30, 40, 20, 10,
            3.0, 12.0, -12.0, -3.0,
            15.0, 35.0, -35.0, -15.0,
            0.0, 0
        ));

        stocks.push_back(create_stock(
            "ВТБ", "VTBR", 56,
            VolatilityType::HIGH, DividendPolicy::LOW,
            33, 37, 18, 12,
            1.5, 6.0, -6.0, -1.5,
            8.0, 20.0, -20.0, -8.0,
            1.5, 9
        ));
    }

    void init_bonds() {
        bonds.push_back(Bond("ОФЗ-26230", "OFZ26230", 1000, 7.5, 6, 5));
        bonds.push_back(Bond("ОФЗ-26238", "OFZ26238", 1000, 8.0, 12, 10));
        bonds.push_back(Bond("Сбер КОРП", "SBERCORP", 1000, 9.5, 3, 3));
        bonds.push_back(Bond("Газпром КОРП", "GAZPCORP", 1000, 10.0, 6, 7));
    }

    void calculate_total_value() {
        total_stock_value = 0;
        for (const auto& stock : stocks) {
            total_stock_value += stock.current_price * stock.shares_owned;
        }
        total_bond_value = 0;
        for (const auto& bond : bonds) {
            total_bond_value += bond.current_price * bond.bonds_owned;
        }
    }
};

struct Person {
    RUB cash = 0;
    RUB salary = 0;
    RUB scholarship = 0;

    int birth_year = 2019;

    bool is_well_off_family = false;
    LifeStatus status = LifeStatus::SCHOOL;
    EducationLevel education = EducationLevel::NONE;
    std::string current_activity = "Учится в школе (1 класс)";

    int work_experience_months = 0;

    Car car;
    TaxSystem taxes;
    Employer employer; 
    Bank bank;
    Dog dog;
    HealthSystem health;
    SecondEmployer second_employer; 
    SavingsAccount savings;
    Portfolio portfolio;
    EntertainmentSystem entertainment;

    bool has_second_job = false;    
    int months_since_first_job = 0;  

    int retirement_age = 60;               
    RUB monthly_pension = 0;              
    bool is_retired = false;    
    bool is_alive = true;

    double oge_average = 0.0;
    int ege_total = 0;

    int last_phone_purchase_year = 0;
    bool has_phone = false;
    bool has_own_home = false;
    RUB property_value = 0;         

    Utilities utilities;

    int get_age(int current_year) const {
        return current_year - birth_year;
    }

    bool can_buy_new_phone(int current_year) const {
        if (!has_phone) return true;
        return (current_year - last_phone_purchase_year) >= 2;
    }
};

struct Person alice;

Bank sber = {
    "Sber",
    9000000, 15.0, 30, 90000,
    150000, 22.0, 24, 7800
};

Bank alfa = {
    "Alfa",
    9000000, 17.0, 30, 110000,
    150000, 14.0, 24, 7200
};


void spend_money(RUB& balance, RUB amount, const std::string& purpose) {
    if (balance >= amount) {
        balance -= amount;
    }
    else {
        RUB shortage = amount - balance;

        if (alice.savings.balance >= shortage) {
            alice.savings.balance -= shortage;
            alice.savings.total_withdrawn += shortage;
            balance = 0;

        }
        else {
            std::cout << " Не хватает денег на: " << purpose
                << "! Требуется: " << amount << " руб., есть: " << balance
                << " руб., на вкладе: " << alice.savings.balance << " руб.\n";
            balance = 0;
        }
    }
}


RUB calculate_salary_by_education(EducationLevel edu, int experience_months) {
    RUB base_salary = 0;

    switch (edu) {
    case EducationLevel::NONE:
        base_salary = 30000;
        break;
    case EducationLevel::SCHOOL:
        base_salary = 40000;
        break;
    case EducationLevel::COLLEGE:
        base_salary = 60000;
        break;
    case EducationLevel::UNIVERSITY:
        base_salary = 90000;
        break;
    }

    int years_worked = experience_months / 12;
    double experience_bonus = 1.0 + (years_worked * 0.03);

    return static_cast<RUB>(base_salary * experience_bonus);
}

std::vector<int> get_recommended_stocks(int age) {
    std::vector<int> recommended;

    if (age < 30) {

        recommended.push_back(3);
        recommended.push_back(5);
        recommended.push_back(6);
    }
    else if (age < 45) {

        recommended.push_back(0);
        recommended.push_back(2);
        recommended.push_back(3);
    }
    else if (age < 60) {
        recommended.push_back(0);
        recommended.push_back(1);
        recommended.push_back(4);
    }
    else {
        recommended.push_back(1);
        recommended.push_back(4);

    }

    return recommended;
}


void alice_oge(const int year, const int month) {
    if (year != 2035 || month != 6 || alice.status != LifeStatus::SCHOOL) return;

    int total_oge_score = 0;
    for (int i = 1; i <= 4; ++i) {
        int chance_good_grade = alice.is_well_off_family ? 70 : 40;
        int grade = 2;

        if (check_chance(chance_good_grade)) {
            grade = check_chance(50) ? 5 : 4;
        }
        else {
            grade = check_chance(60) ? 3 : 2;
        }
        total_oge_score += grade;
    }

    alice.oge_average = static_cast<double>(total_oge_score) / 4.0;
    std::cout << "  Средний балл ОГЭ: " << alice.oge_average << "\n";

    if (alice.oge_average > 3.1) {
        alice.current_activity = "Учится в 10 классе";
        std::cout << "Элис проходит в 10 класс!\n";
    }
    else if (alice.oge_average >= 2.5) {
        alice.status = LifeStatus::COLLEGE;
        alice.current_activity = "Учится в колледже";
        std::cout << "Элис поступает в колледж.\n";
    }
    else {
        alice.status = LifeStatus::WORKING;
        alice.education = EducationLevel::NONE;
        alice.current_activity = "Работает (без образования)";

        alice.employer.base_salary = calculate_salary_by_education(alice.education, 0);

        std::cout << "Провал ОГЭ. Элис идет работать.\n";
        std::cout << "  Начальная зарплата: " << alice.employer.base_salary << " руб./мес.\n";
    }
}


void alice_ege(const int year, const int month) {
    if (year != 2037 || month != 6 || alice.status != LifeStatus::SCHOOL) return;

    int total_ege_score = 0;
    for (int i = 1; i <= 3; ++i) {
        int score = alice.is_well_off_family ? (50 + (rand() % 46)) : (30 + (rand() % 56));
        total_ege_score += score;
    }

    alice.ege_total = total_ege_score;
    std::cout << "  Сумма баллов ЕГЭ: " << alice.ege_total << "\n";

    if (alice.ege_total > 170) {
        alice.status = LifeStatus::UNIVERSITY;
        alice.current_activity = "Учится в университете";
        std::cout << "Элис поступает в университет!\n";
    }
    else {
        alice.status = LifeStatus::WORKING;
        alice.education = EducationLevel::SCHOOL;
        alice.current_activity = "Работает (школа)";

        alice.employer.base_salary = calculate_salary_by_education(alice.education, 0);

        std::cout << "Баллов не хватило. Элис идет работать.\n";
        std::cout << "  Начальная зарплата: " << alice.employer.base_salary << " руб./мес.\n";
    }
}


void alice_college(const int year, const int month) {
    if (alice.status != LifeStatus::COLLEGE) return;

    int college_year = year - 2035;

    if (year == 2038 && month == 6) {
        alice.status = LifeStatus::WORKING;
        alice.education = EducationLevel::COLLEGE;
        alice.current_activity = "Работает (колледж)";

        alice.employer.base_salary = calculate_salary_by_education(alice.education, 0);

        std::cout << "\n " << year << " год: Элис окончила колледж!\n";
        std::cout << "  Начальная зарплата: " << alice.employer.base_salary << " руб./мес.\n";
    }
}


void alice_university(const int year, const int month) {
    if (alice.status != LifeStatus::UNIVERSITY) return;

    int uni_year = year - 2037;

    if (year == 2041 && month == 6) {
        alice.status = LifeStatus::WORKING;
        alice.education = EducationLevel::UNIVERSITY;
        alice.current_activity = "Работает (университет)";

        alice.employer.base_salary = calculate_salary_by_education(alice.education, 0);

        std::cout << "\n" << year << " год: Элис получила диплом ВУЗа!\n";
        std::cout << "  Начальная зарплата: " << alice.employer.base_salary << " руб./мес.\n";
    }
}


void alice_education(const int year, const int month) {
    alice_oge(year, month);
    alice_ege(year, month);

    alice_college(year, month);
    alice_university(year, month);
}


void alice_car(const int year, const int month) {
    if (!alice.car.has_car) {
        if (year >= 2038 && alice.cash >= 600000) {
            if (check_chance(15)) {
                alice.car.has_car = true;
                alice.car.model = "Suzuki Swift";
                alice.car.health = 100;
                alice.car.age_months = 0;
                alice.car.purchase_price = 600000;
                alice.car.monthly_cost = 5000;

                spend_money(alice.cash, alice.car.purchase_price, "покупку машины");
                if (alice.cash > 0) { 
                    std::cout << "Элис купила " << alice.car.model
                        << " за " << alice.car.purchase_price << "\n";
                }
            }
        }
    }
    else {
        spend_money(alice.cash, alice.car.monthly_cost, "содержание машины");

        alice.car.age_months++;

        int wear_factor = (100 - alice.car.health) / 10;
        int age_factor = alice.car.age_months / 12;
        int breakdown_chance = 3 + wear_factor + age_factor;
        breakdown_chance = (std::min)(breakdown_chance, 25);

        if (check_chance(breakdown_chance)) {
            std::uniform_int_distribution<> repair_distrib(0, 99);
            int repair_roll = repair_distrib(gen);

            if (repair_roll < 55) {
                spend_money(alice.cash, alice.car.light_repair_cost, "лёгкий ремонт машины");
                if (alice.cash > 0 || alice.car.light_repair_cost == 0) { 
                    alice.car.health -= 2;
                    std::cout << "Лёгкий ремонт " << alice.car.model
                        << " (-" << alice.car.light_repair_cost
                        << "). Состояние: " << alice.car.health << "%" << "\n";
                }
            }
            else if (repair_roll < 85) {
                spend_money(alice.cash, alice.car.heavy_repair_cost, "сложный ремонт машины");
                if (alice.cash > 0 || alice.car.heavy_repair_cost == 0) {
                    alice.car.health -= 10;
                    std::cout << "Сложный ремонт " << alice.car.model
                        << " (-" << alice.car.heavy_repair_cost
                        << "). Состояние: " << alice.car.health << "%" << "\n";
                }
            }
            else {
                spend_money(alice.cash, alice.car.capital_repair_cost, "капитальный ремонт машины");
                if (alice.cash > 0 || alice.car.capital_repair_cost == 0) {
                    alice.car.health -= 25;
                    std::cout << "Капитальный ремонт " << alice.car.model
                        << " (-" << alice.car.capital_repair_cost
                        << "). Состояние: " << alice.car.health << "%" << "\n";
                }
            }

            if (alice.car.health <= 0) {
                std::cout << " Машина " << alice.car.model
                    << " окончательно сломалась и отправлена на свалку." << "\n";
                alice.car.has_car = false;
                alice.car.model = "Отсутствует";
                alice.car.health = 0;
            }
        }
    }
}


void alice_salary(const int year, const int month) {
    RUB gross_salary = 0;

    if (month == 1) {
        alice.taxes.reset_annual_income();
    }

    if (alice.is_retired) {
        alice.cash += alice.monthly_pension;
        if (month == 1) {
            std::cout << "  [ПЕНСИЯ] " << year << " год: получена пенсия "
                << alice.monthly_pension << " руб./мес.\n";
        }
        return;
    }

    if (alice.status == LifeStatus::WORKING) {
        double original_bonus_chance = alice.employer.bonus_chance;

        if (alice.dog.has_dog) {
            alice.employer.bonus_chance += 0.05;
        }

        if (alice.entertainment.happiness_level > 80) {
            alice.employer.bonus_chance += 0.05;
        }
        else if (alice.entertainment.happiness_level < 30) {
            alice.employer.bonus_chance -= 0.05;
        }

        gross_salary = alice.employer.calculate_gross_salary(true);

        alice.employer.bonus_chance = original_bonus_chance;

        RUB pension_contribution = alice.employer.calculate_pension_contribution(gross_salary);
        RUB income_tax = alice.taxes.calculate_income_tax(gross_salary);
        RUB net_salary = gross_salary - income_tax;

        alice.cash += net_salary;
        alice.salary = gross_salary;
        alice.work_experience_months++;
        alice.taxes.total_tax_paid += income_tax;

        if (alice.has_second_job) {
            RUB second_gross = alice.second_employer.calculate_gross_salary(true);
            RUB second_pension = alice.second_employer.calculate_pension_contribution(second_gross);
            RUB second_tax = alice.taxes.calculate_income_tax(second_gross);
            RUB second_net = second_gross - second_tax;

            alice.cash += second_net;
            alice.taxes.total_tax_paid += second_tax;

            if (month == 1) {
                std::cout << "  [ЗАРПЛАТА 2] " << year << " год: подработка "
                    << second_gross << " руб. (налог: " << second_tax
                    << ", на руки: " << second_net << ")\n";
            }
        }
    }
    else if (alice.status == LifeStatus::UNIVERSITY) {
        alice.scholarship = 5000;
        alice.cash += alice.scholarship;
        return;
    }
    else if (alice.status == LifeStatus::SCHOOL || alice.status == LifeStatus::COLLEGE) {
        if (alice.is_well_off_family) {
            alice.cash += 3000;
        }
        return;
    }
}


void alice_try_second_job(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;

    if (alice.has_second_job) return;

    if (alice.is_retired) return;

    if (alice.work_experience_months < 60) return;

    if (sber.mortgage.is_active || alfa.mortgage.is_active || alice.has_own_home) return;

    const RUB down_payment = 4000000;
    if (alice.cash >= down_payment) return;  

    if (!check_chance(2)) return;

    alice.second_employer.base_salary = static_cast<RUB>(alice.employer.base_salary * 0.40);
    alice.second_employer.bonus_chance = 0.05;  
    alice.second_employer.bonus_amount = static_cast<RUB>(alice.second_employer.base_salary * 0.5);  
    alice.second_employer.pension_rate = 0.22;

    alice.has_second_job = true;
}


void simulate_stock_price(Stock& stock) {
    std::uniform_int_distribution<> event_distrib(1, 100);
    int event = event_distrib(gen);

    double change_percent = 0.0;

    if (event <= static_cast<int>(stock.growth_chance)) {
        std::uniform_real_distribution<> growth(stock.min_growth, stock.max_growth);
        change_percent = growth(gen);
    }
    else if (event <= static_cast<int>(stock.growth_chance + stock.decline_chance)) {
        std::uniform_real_distribution<> decline(stock.min_decline, stock.max_decline);
        change_percent = decline(gen);
    }
    else if (event <= static_cast<int>(stock.growth_chance + stock.decline_chance + stock.strong_growth_chance)) {
        std::uniform_real_distribution<> strong_growth(stock.strong_growth_min, stock.strong_growth_max);
        change_percent = strong_growth(gen);
    }
    else {
        std::uniform_real_distribution<> strong_decline(stock.strong_decline_min, stock.strong_decline_max);
        change_percent = strong_decline(gen);
    }

    stock.current_price = static_cast<RUB>(stock.current_price * (1.0 + change_percent / 100.0));

    if (stock.current_price < 1) stock.current_price = 1;

    if (stock.current_price > stock.base_price * 10) {
        stock.current_price = stock.base_price * 10;
    }
}


RUB calculate_stock_dividend(const Stock& stock) {
    if (!stock.pays_dividends || stock.shares_owned == 0) return 0;

    return static_cast<RUB>(stock.current_price * stock.shares_owned * stock.dividend_yield / 100.0 / 12.0);
}


void simulate_bond_price(Bond& bond) {
    std::uniform_int_distribution<> event_distrib(1, 100);
    int event = event_distrib(gen);

    double change_percent = 0.0;

    if (event <= 50) {
        std::uniform_real_distribution<> stable(-0.5, 0.5);
        change_percent = stable(gen);
    }
    else if (event <= 80) {
        std::uniform_real_distribution<> growth(0.5, 2.0);
        change_percent = growth(gen);
    }
    else if (event <= 95) {
        std::uniform_real_distribution<> decline(-2.0, -0.5);
        change_percent = decline(gen);
    }
    else {
        std::uniform_real_distribution<> strong(-3.0, 3.0);
        change_percent = strong(gen);
    }

    bond.current_price = static_cast<RUB>(bond.current_price * (1.0 + change_percent / 100.0));

    if (bond.current_price < bond.face_value * 0.5) {
        bond.current_price = static_cast<RUB>(bond.face_value * 0.5);
    }

    if (bond.current_price > bond.face_value * 1.5) {
        bond.current_price = static_cast<RUB>(bond.face_value * 1.5);
    }
}


RUB calculate_bond_coupon(const Bond& bond) {
    if (bond.bonds_owned == 0) return 0;
    return static_cast<RUB>(bond.face_value * bond.bonds_owned * bond.coupon_rate / 100.0 / 12.0);
}


void simulate_stock_market() {
    for (auto& stock : alice.portfolio.stocks) {
        simulate_stock_price(stock);  
    }

    for (auto& bond : alice.portfolio.bonds) {
        simulate_bond_price(bond);  
    }

    alice.portfolio.months_active++;
}


void alice_receive_dividends(const int year, const int month) {
    if (!alice.portfolio.is_active) return;

    RUB total_dividends = 0;

    for (auto& stock : alice.portfolio.stocks) {
        if (stock.shares_owned > 0 && stock.pays_dividends && stock.dividend_months == month) {
            RUB dividend = calculate_stock_dividend(stock);  

            if (dividend > 0) {
                stock.total_dividends += dividend;
                total_dividends += dividend;

            }
        }
    }

    RUB total_coupons = 0;
    for (auto& bond : alice.portfolio.bonds) {
        if (bond.bonds_owned > 0 && bond.coupon_months == month) {
            RUB coupon = calculate_bond_coupon(bond);  

            if (coupon > 0) {
                bond.total_coupons += coupon;
                total_coupons += coupon;
            }
        }
    }

    if (total_dividends > 0) {
        alice.cash += total_dividends;
        alice.portfolio.total_dividends_received += total_dividends;
    }

    if (total_coupons > 0) {
        alice.cash += total_coupons;
        alice.portfolio.total_coupons_received += total_coupons;
    }
}


void alice_try_phone_loan(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;
    if (sber.loan.is_active || alfa.loan.is_active) return;
    if (!alice.can_buy_new_phone(year)) return;

    if (alice.work_experience_months < 6) return;

    const RUB phone_price = 150000;
    if (alice.cash >= phone_price) return;
    if (!check_chance(5)) return;

    Bank* best_bank;
    if (sber.loan_monthly_payment <= alfa.loan_monthly_payment) {
        best_bank = &sber;
    }
    else {
        best_bank = &alfa;
    }

    if (alice.cash >= phone_price) {
        alice.cash -= phone_price;
    }
    else {
        alice.cash = 0;
    }

    best_bank->loan.is_active = true;
    best_bank->loan.total_amount = best_bank->loan_amount;
    best_bank->loan.remaining_balance = best_bank->loan_amount;
    best_bank->loan.annual_rate = best_bank->loan_rate;
    best_bank->loan.term_months = best_bank->loan_term_months;
    best_bank->loan.months_paid = 0;
    best_bank->loan.monthly_payment = best_bank->loan_monthly_payment;

    alice.has_phone = true;
    alice.last_phone_purchase_year = year;

    std::cout << " " << year << " год: Элис взяла кредит на телефон!\n";
    std::cout << "    Банк: " << best_bank->name << "\n";
    std::cout << "    Ставка: " << best_bank->loan_rate << "%\n";
    std::cout << "    Платёж: " << best_bank->loan_monthly_payment << " руб./мес.\n";
}


void alice_try_mortgage(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;
    if (sber.mortgage.is_active || alfa.mortgage.is_active || alice.has_own_home) return;

    int age = alice.get_age(year);
    if (age < 25) return;

    Bank* best_bank;
    if (sber.mortgage_monthly_payment <= alfa.mortgage_monthly_payment) {
        best_bank = &sber;
    }
    else {
        best_bank = &alfa;
    }

    const RUB down_payment = 4000000;
    const RUB monthly_payment = best_bank->mortgage_monthly_payment;

    if (alice.salary < monthly_payment * 2) return;
    if (alice.cash < down_payment) return;
    if (!check_chance(15)) return;

    if (alice.cash >= down_payment) {
        alice.cash -= down_payment;
    }
    else {
        alice.cash = 0;
        return;
    }

    best_bank->mortgage.is_active = true;
    best_bank->mortgage.total_amount = best_bank->mortgage_amount;
    best_bank->mortgage.remaining_balance = best_bank->mortgage_amount;
    best_bank->mortgage.annual_rate = best_bank->mortgage_rate;
    best_bank->mortgage.term_months = best_bank->mortgage_term_years * 12;
    best_bank->mortgage.months_paid = 0;
    best_bank->mortgage.monthly_payment = monthly_payment;

    std::cout << " " << year << " год (возраст " << age << "):\n";
    std::cout << "    Элис выбрала банк: " << best_bank->name << "\n";
    std::cout << "    Ставка: " << best_bank->mortgage_rate << "%\n";
    std::cout << "    Платёж: " << monthly_payment << " руб./мес.\n";
}


void alice_try_buy_dog(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;

    if (alice.dog.has_dog) return;

    int age = alice.get_age(year);
    if (age < 25) return;

    if (!sber.mortgage.is_active && !alfa.mortgage.is_active && !alice.has_own_home) {
        return;  
    }

    if (!check_chance(3)) return;

    if (alice.cash < 100000) return;

    std::uniform_int_distribution<> breed_distrib(0, 2);
    int breed_roll = breed_distrib(gen);

    std::string breed_name;
    RUB price;
    int max_age;
    RUB monthly_food;

    switch (breed_roll) {
    case 0: 
        breed_name = "Лабрадор";
        price = 50000;
        max_age = 144; 
        monthly_food = 4000;
        break;
    case 1: 
        breed_name = "Корги";
        price = 80000;
        max_age = 156;  
        monthly_food = 3500;
        break;
    case 2:  
        breed_name = "Дворняга (из приюта)";
        price = 5000;
        max_age = 180;
        monthly_food = 2500;
        break;
    }

    alice.dog.has_dog = true;
    alice.dog.breed = breed_name;
    alice.dog.age_months = 24;  
    alice.dog.max_age_months = max_age;
    alice.dog.health = 100;
    alice.dog.purchase_price = price;
    alice.dog.monthly_food_cost = monthly_food;


    spend_money(alice.cash, price, "покупку собаки");

    std::cout << " " << year << " год: Элис завела собаку!\n";
    std::cout << "    Порода: " << breed_name << "\n";
}


void alice_pay_mortgage(const int year, const int month) {
    Bank* banks[] = { &sber, &alfa };

    for (Bank* bank : banks) {
        if (!bank->mortgage.is_active) continue;

        if (alice.cash >= bank->mortgage.monthly_payment) {
            alice.cash -= bank->mortgage.monthly_payment;
            bank->mortgage.remaining_balance -= bank->mortgage.monthly_payment;
            bank->mortgage.months_paid++;

            if (bank->mortgage.remaining_balance <= 0) {
                bank->mortgage.is_active = false;
                bank->mortgage.remaining_balance = 0;
                alice.has_own_home = true;
                alice.property_value = 13000000;
                std::cout << " В банке " << bank->name
                    << " полностью погашена в " << year << " году!\n";
            }
        }
        else {
            std::cout << " Не хватает денег на ипотеку в " << bank->name << "!\n";
        }
    }
}


void alice_pay_loan(const int year, const int month) {
    Bank* banks[] = { &sber, &alfa };

    for (Bank* bank : banks) {
        if (!bank->loan.is_active) continue;

        if (alice.cash >= bank->loan.monthly_payment) {
            alice.cash -= bank->loan.monthly_payment;
            bank->loan.remaining_balance -= bank->loan.monthly_payment;
            bank->loan.months_paid++;

            if (bank->loan.remaining_balance <= 0) {
                bank->loan.is_active = false;
                bank->loan.remaining_balance = 0;
            }
        }
        else {
            std::cout << "Не хватает денег на кредит в " << bank->name << "!\n";
        }
    }
}


void alice_pay_property_taxes(const int year, const int month) {
    if (month != 12) return;

    if (alice.has_own_home && alice.property_value > 0) {
        RUB property_tax = alice.taxes.calculate_property_tax(alice.property_value);

        if (alice.cash >= property_tax) {
            alice.cash -= property_tax;
            alice.taxes.total_tax_paid += property_tax;
        }
        else {
            std::cout << "Не хватает денег на налог на квартиру!\n";
        }
    }

    if (alice.car.has_car) {
        int horse_power = 100;
        RUB vehicle_tax = alice.taxes.calculate_vehicle_tax(horse_power);

        if (alice.cash >= vehicle_tax) {
            alice.cash -= vehicle_tax;
            alice.taxes.total_tax_paid += vehicle_tax;
        }
        else {
            std::cout << "Не хватает денег на транспортный налог!\n";
        }
    }
}


void alice_dog_care(const int year, const int month) {
    if (!alice.dog.has_dog) return;

    alice.dog.age_months++;

    RUB monthly_cost = alice.dog.get_monthly_cost();
    spend_money(alice.cash, monthly_cost, "содержание собаки");

    int age_years = alice.dog.get_age_years();

    int sickness_chance = 2 + (age_years * 2);  
    sickness_chance = (std::min)(sickness_chance, 30); 

    if (check_chance(sickness_chance)) {
        spend_money(alice.cash, alice.dog.emergency_vet_cost, "экстренный ветеринар");
        alice.dog.health -= 10;
    }

    if (alice.dog.age_months >= alice.dog.max_age_months || alice.dog.health <= 0) {
        std::cout << "  [СОБАКА] " << year << " год: питомец умер в возрасте "
            << age_years << " лет. Элис очень грустит...\n";

        alice.dog.has_dog = false;
        alice.dog.name = "Нет собаки";
        alice.dog.breed = "Отсутствует";
        alice.dog.health = 0;
    }

}


void alice_check_retirement(const int year, const int month) {
    if (month != 1) return;

    int age = alice.get_age(year);

    if (age >= alice.retirement_age && !alice.is_retired && alice.status == LifeStatus::WORKING) {
        alice.is_retired = true;
        alice.status = LifeStatus::WORKING; 

        alice.monthly_pension = static_cast<RUB>(alice.salary * 0.30);

        std::cout << "\n   " << year << " год: Элис вышла на пенсию в возрасте "
            << age << " лет!\n";
    }
}

void alice_food(const int year, const int month) {
    RUB food_cost = 0;

    if (alice.status == LifeStatus::WORKING) {
        food_cost = static_cast<RUB>(alice.salary * 0.20);
    }
    else if (alice.status == LifeStatus::UNIVERSITY) {
        food_cost = static_cast<RUB>(alice.scholarship * 0.30);
        if (food_cost < 5000) food_cost = 5000;  
    }
    else {
        food_cost = 0;
    }

    if (food_cost > 0) {
        spend_money(alice.cash, food_cost, "еду");
    }
}


void alice_rent(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;

    if (sber.mortgage.is_active || alfa.mortgage.is_active || alice.has_own_home) return;

    RUB rent_cost = static_cast<RUB>(alice.salary * 0.35);

    if (rent_cost < 20000) rent_cost = 20000;

    if (rent_cost > 80000) rent_cost = 80000;

    if (month == 1 && year > 2038) {
        rent_cost = static_cast<RUB>(rent_cost * 1.05);
    }

    spend_money(alice.cash, rent_cost, "аренду жилья");
}


void alice_home_bills(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;

    bool has_own_place = sber.mortgage.is_active || alfa.mortgage.is_active || alice.has_own_home;
    bool is_winter = (month >= 10 || month <= 3);

    RUB total_cost = 0;

    if (has_own_place) {
        total_cost = alice.utilities.get_total_monthly_cost(is_winter);
    }
    else {
        total_cost = alice.utilities.electricity +
            alice.utilities.water +
            alice.utilities.gas +
            alice.utilities.internet;

        if (is_winter) {
            total_cost += alice.utilities.heating;
        }
    }

    if (month == 1 && year > 2038) {
        alice.utilities.electricity = static_cast<RUB>(alice.utilities.electricity * 1.03);
        alice.utilities.water = static_cast<RUB>(alice.utilities.water * 1.03);
        alice.utilities.heating = static_cast<RUB>(alice.utilities.heating * 1.03);
        alice.utilities.gas = static_cast<RUB>(alice.utilities.gas * 1.03);
        alice.utilities.internet = static_cast<RUB>(alice.utilities.internet * 1.03);
        alice.utilities.trash = static_cast<RUB>(alice.utilities.trash * 1.03);
        alice.utilities.maintenance = static_cast<RUB>(alice.utilities.maintenance * 1.03);

        if (has_own_place) {
            total_cost = alice.utilities.get_total_monthly_cost(is_winter);
        }
        else {
            total_cost = alice.utilities.electricity +
                alice.utilities.water +
                alice.utilities.gas +
                alice.utilities.internet;
            if (is_winter) {
                total_cost += alice.utilities.heating;
            }
        }
    }

    spend_money(alice.cash, total_cost, "ЖКХ");
}


bool is_entertainment_available(const Entertainment& ent, int current_year, int current_month) {
    int months_since_last = (current_year - ent.last_enjoyed_year) * 12 +
        (current_month - ent.last_enjoyed_month);
    return months_since_last >= ent.frequency_months;
}


MoodLevel get_mood_level(int happiness) {
    if (happiness <= 20) return MoodLevel::DEPRESSED;
    if (happiness <= 40) return MoodLevel::SAD;
    if (happiness <= 60) return MoodLevel::NEUTRAL;
    if (happiness <= 80) return MoodLevel::GOOD;
    if (happiness <= 100) return MoodLevel::HAPPY;
    return MoodLevel::ECSTATIC;
}


void init_entertainments(EntertainmentSystem& system) {
    system.entertainments.clear();

    system.entertainments.push_back(Entertainment{
        "Кино", EntertainmentType::CINEMA, 800, 3, 1
        });

    system.entertainments.push_back(Entertainment{
        "Книги", EntertainmentType::BOOKS, 1500, 5, 2
        });

    system.entertainments.push_back(Entertainment{
        "Спортзал", EntertainmentType::GYM, 4000, 8, 1
        });

    system.entertainments.push_back(Entertainment{
        "Кафе и рестораны", EntertainmentType::CAFE, 2500, 6, 1
        });
    system.entertainments.push_back(Entertainment{
        "Концерты и мероприятия", EntertainmentType::CONCERT, 5000, 10, 3
        });
    system.entertainments.push_back(Entertainment{
        "Хобби", EntertainmentType::HOBBY, 2000, 7, 1
        });
    system.entertainments.push_back(Entertainment{
        "Шопинг", EntertainmentType::SHOPPING, 8000, 12, 2
        });
}


void alice_entertainment(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING &&
        alice.status != LifeStatus::UNIVERSITY) {
        return;
    }

    if (alice.status == LifeStatus::WORKING) {
        alice.entertainment.monthly_budget =
            static_cast<RUB>(alice.salary * alice.entertainment.budget_percent);
    }
    else {
        alice.entertainment.monthly_budget = 3000;
    }

    RUB spent_this_month = 0;

    for (auto& ent : alice.entertainment.entertainments) {
        if (!is_entertainment_available(ent, year, month)) continue;

        if (alice.cash < ent.cost) continue;

        if (spent_this_month + ent.cost > alice.entertainment.monthly_budget) {
            continue;
        }

        if (!check_chance(70)) continue;

        alice.cash -= ent.cost;
        spent_this_month += ent.cost;
        alice.entertainment.total_spent_on_fun += ent.cost;
        alice.entertainment.happiness_level += ent.happiness_boost;

        ent.total_times++;
        ent.total_spent += ent.cost;
        ent.last_enjoyed_month = month;
        ent.last_enjoyed_year = year;

    }

    if (alice.entertainment.happiness_level > 150) {
        alice.entertainment.happiness_level = 150;
    }
}


void alice_vacation(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;

    if (month != 6 && month != 7) return;

    if (alice.entertainment.last_vacation_year == year) return;

    int months_since_last_vacation = (year - alice.entertainment.last_vacation_year) * 12;
    if (months_since_last_vacation < 10) return;

    if (!check_chance(80)) return;

    RUB vacation_cost = static_cast<RUB>(alice.salary * 1.5); 

    if (vacation_cost < 50000) vacation_cost = 50000;

    if (alice.cash < vacation_cost) {
        RUB shortage = vacation_cost - alice.cash;
        if (alice.savings.balance >= shortage) {
            alice.savings.balance -= shortage;
            alice.savings.total_withdrawn += shortage;
            alice.cash = 0;
        }
        
    }
    else {
        alice.cash -= vacation_cost;
    }

    alice.entertainment.happiness_level += 30;

    alice.entertainment.total_spent_on_travel += vacation_cost;
    alice.entertainment.vacations_taken++;
    alice.entertainment.last_vacation_year = year;
}


void alice_happiness_decay(const int year, const int month) {
    if (month % 3 != 0) return;

    int decay = 5;

    if (alice.has_second_job) {
        decay += 3;
    }

    if (alice.dog.has_dog) {
        decay -= 2;
    }

    if (alice.health.health_points < 50) {
        decay += 2;
    }

    alice.entertainment.happiness_level -= decay;

    if (alice.entertainment.happiness_level < 0) {
        alice.entertainment.happiness_level = 0;
    }
}


void alice_check_health(const int year, const int month) {
    if (!alice.is_alive) return;

    if (alice.has_second_job && alice.status == LifeStatus::WORKING) {
        if (month % 6 == 0) {
            alice.health.health_points -= 1;
            if (alice.health.health_points < 0) alice.health.health_points = 0;
        }
    }

    if (month == 1) {
        alice.health.medical_expenses_this_year = 0;
    }

    int age_years = alice.get_age(year);

    int happiness_health_modifier = 0;
    if (alice.entertainment.happiness_level < 30) {
        happiness_health_modifier = 3; 
    }
    else if (alice.entertainment.happiness_level > 80) {
        happiness_health_modifier = -2;  
    }

    if (age_years > 40 && month == 1) {
        int health_loss = (age_years - 40) / 10;
        if (health_loss < 1) health_loss = 1;

        alice.health.health_points -= health_loss;
        if (alice.health.health_points < 0) alice.health.health_points = 0;
    }

    int sickness_chance = alice.health.calculate_sickness_chance(age_years, alice.health.health_points);

    if (alice.has_second_job) {
        sickness_chance += 3;
    }

    if (check_chance(sickness_chance)) {
        alice.health.is_sick = true;
        alice.health.sick_days += 30;

        std::uniform_int_distribution<> severity_distrib(0, 99);
        int severity = severity_distrib(gen);

        if (severity < 60) {
            RUB treatment_cost = alice.health.medicine_cost;
            spend_money(alice.cash, treatment_cost, "лечение (лекарства)");
            alice.health.total_medical_expenses += treatment_cost;
            alice.health.health_points -= 2;

        }
        else if (severity < 90) {
            RUB treatment_cost = alice.health.doctor_visit + alice.health.medicine_cost;
            spend_money(alice.cash, treatment_cost, "лечение (врач + лекарства)");
            alice.health.total_medical_expenses += treatment_cost;
            alice.health.health_points -= 5;

        }
        else if (severity < 98) {
            RUB treatment_cost = alice.health.hospital_cost;
            spend_money(alice.cash, treatment_cost, "лечение (госпитализация)");
            alice.health.total_medical_expenses += treatment_cost;
            alice.health.health_points -= 15;

        }
        else {
            RUB treatment_cost = alice.health.surgery_cost;
            spend_money(alice.cash, treatment_cost, "лечение (операция)");
            alice.health.total_medical_expenses += treatment_cost;
            alice.health.health_points -= 30;

        }

        if (alice.health.health_points <= 0) {
            alice.is_alive = false;
            std::cout << " " << year << " год: Элис умерла в возрасте "
                << age_years << " лет от болезни.\n";
        }
    }
    else {
        alice.health.is_sick = false;
    }

    if (!alice.health.is_sick && age_years < 50 && month % 3 == 0) {
        alice.health.health_points += 1;
        if (alice.health.health_points > alice.health.max_health) {
            alice.health.health_points = alice.health.max_health;
        }
    }

    if (age_years >= 85) {
        int death_chance = (age_years - 85) * 5;
        if (check_chance(death_chance)) {
            alice.is_alive = false;
            std::cout << "  [СМЕРТЬ] " << year << " год: Элис умерла от старости в возрасте "
                << age_years << " лет.\n";
        }
    }
}


void alice_claim_property_deduction(const int year, const int month) {
    if (alice.taxes.property_deduction_claimed) return;

    if (!sber.mortgage.is_active && !alfa.mortgage.is_active && !alice.has_own_home) return;

    if (alice.status != LifeStatus::WORKING && !alice.is_retired) return;

    if (!check_chance(20)) return;

    //фиксированная сумма вычета на квартиру (13% от 2 млн + вычет по процентам)
    const RUB property_deduction = 400000;

    alice.taxes.property_deduction_claimed = true;
    alice.taxes.total_deductions_received += property_deduction;

    alice.cash += property_deduction;

    std::cout << "  [ВЫЧЕТ] " << year << " год: Элис получила налоговый вычет на квартиру!\n";
    std::cout << "    Сумма вычета: " << property_deduction << " руб.\n";
}


void alice_claim_medical_deduction(const int year, const int month) {
    if (month != 12) return;

    if (alice.status != LifeStatus::WORKING && !alice.is_retired) return;

    if (alice.health.medical_expenses_this_year == 0) return;

    //13% от расходов на лечение, но максимум с 120 000 руб.
    const RUB max_deduction_base = 120000; 
    RUB deduction_base = (std::min)(alice.health.medical_expenses_this_year, max_deduction_base);
    RUB medical_deduction = static_cast<RUB>(deduction_base * 0.13);

    if (medical_deduction > 0) {
        alice.taxes.medical_deduction_received += medical_deduction;
        alice.taxes.total_deductions_received += medical_deduction;

        alice.cash += medical_deduction;
    }
}


void alice_savings(const int year, const int month) {
    if (alice.savings.balance > 0) {
        alice.savings.add_monthly_interest();
    }

    if (alice.status == LifeStatus::WORKING && alice.salary > 0) {
        RUB monthly_deposit = static_cast<RUB>(alice.salary * 0.05);

        if (monthly_deposit > 0) {
            alice.savings.deposit(monthly_deposit);
        }
    }

}


void alice_buy_stocks(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;

    if (!alice.portfolio.is_active) {
        alice.portfolio.init_stocks();
        alice.portfolio.init_bonds();
        alice.portfolio.is_active = true;
    }

    RUB investment_amount = static_cast<RUB>(alice.salary * 0.05);

    if (investment_amount < 1000) return;
    if (alice.cash < investment_amount) return;

    int age = alice.get_age(year);

    RUB stock_investment = static_cast<RUB>(investment_amount * 0.7);
    RUB bond_investment = static_cast<RUB>(investment_amount * 0.3);

    if (age > 50) {
        stock_investment = static_cast<RUB>(investment_amount * 0.5);
        bond_investment = static_cast<RUB>(investment_amount * 0.5);
    }

    std::vector<int> recommended_stocks = get_recommended_stocks(age);
    RUB per_stock = stock_investment / recommended_stocks.size();

    for (int idx : recommended_stocks) {
        Stock& stock = alice.portfolio.stocks[idx];
        int shares_to_buy = static_cast<int>(per_stock / stock.current_price);

        if (shares_to_buy > 0) {
            RUB cost = shares_to_buy * stock.current_price;

            if (alice.cash >= cost) {
                alice.cash -= cost;
                stock.shares_owned += shares_to_buy;
                stock.total_invested += cost;

                if (month == 1) {

                }
            }
        }
    }

    if (bond_investment >= 1000) {
        Bond& bond = alice.portfolio.bonds[0]; 
        int bonds_to_buy = static_cast<int>(bond_investment / bond.current_price);

        if (bonds_to_buy > 0) {
            RUB cost = bonds_to_buy * bond.current_price;

            if (alice.cash >= cost) {
                alice.cash -= cost;
                bond.bonds_owned += bonds_to_buy;
                bond.total_invested += cost;

            }
        }
    }
}


void alice_sell_stocks(const int year, const int month) {
    if (!alice.portfolio.is_active) return;
    if (month % 3 != 0) return;  

    for (auto& stock : alice.portfolio.stocks) {
        if (stock.shares_owned == 0) continue;

        RUB current_value = stock.current_price * stock.shares_owned;
        double profit_percent = ((double)stock.current_price - stock.initial_price) / stock.initial_price * 100.0;

        if (profit_percent >= 100) {
            RUB sale_value = current_value;
            RUB profit = sale_value - stock.total_invested;

            alice.cash += sale_value;
            alice.portfolio.total_profit += profit;

            stock.shares_owned = 0;
            stock.total_invested = 0;
            stock.initial_price = stock.current_price;
        }

        else if (profit_percent <= -50) {
            RUB sale_value = current_value;
            RUB loss = stock.total_invested - sale_value;

            alice.cash += sale_value;
            alice.portfolio.total_profit -= loss;

            stock.shares_owned = 0;
            stock.total_invested = 0;
            stock.initial_price = stock.current_price;
        }
    }

    for (auto& bond : alice.portfolio.bonds) {
        if (bond.bonds_owned > 0 && bond.years_to_maturity <= 0) {
            RUB redemption_value = bond.face_value * bond.bonds_owned;
            alice.cash += redemption_value;


            bond.bonds_owned = 0;
            bond.total_invested = 0;
        }
    }
}


void simulation() {
    int year = 2026;
    int month = 9;
    while (not (year == 2090 and month == 1)) {
        if (!alice.is_alive) break;

        alice_education(year, month);
        alice_salary(year, month);
        alice_car(year, month);
        alice_try_mortgage(year, month);
        alice_try_phone_loan(year, month);
        alice_pay_mortgage(year, month);
        alice_pay_loan(year, month);
        alice_rent(year, month);
        alice_home_bills(year, month);
        alice_food(year, month);
        alice_check_retirement(year, month);
        alice_pay_property_taxes(year, month);
        alice_try_buy_dog(year, month);  
        alice_dog_care(year, month);
        alice_claim_property_deduction(year, month);
        alice_claim_medical_deduction(year, month);
        alice_try_second_job(year, month);
        alice_savings(year, month);
        alice_entertainment(year, month);
        alice_vacation(year, month);
        alice_happiness_decay(year, month);

        if (alice.status == LifeStatus::WORKING) {
            simulate_stock_market();        
            alice_buy_stocks(year, month);     
            alice_sell_stocks(year, month);    
            alice_receive_dividends(year, month); 
        }

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}



void alice_init() {
    alice.cash = 20000;
    alice.salary = 0;

    alice.is_well_off_family = check_chance(50);

    if (alice.is_well_off_family) {
        std::cout << "Элис родилась в благополучной семье. Родители помогают с учебой.\n";
        alice.cash += 50000;
    }
    else {
        std::cout << "Элис родилась в обычной семье. Всего придется добиваться самой.\n";
    }

    if (alice.cash >= 10000) {
        alice.savings.deposit(10000);
        alice.cash -= 10000;
        std::cout << " Элис открыла накопительный вклад под 12% годовых.\n";
    }

    alice.employer.bonus_chance = 0.10;
    alice.employer.bonus_amount = 50000;
    alice.employer.pension_rate = 0.22;

    alice.second_employer.company_name = "ИП";
    alice.second_employer.bonus_chance = 0.05;
    alice.second_employer.bonus_amount = 25000;
    alice.second_employer.pension_rate = 0.22;
}


void alice_printf() {
    if (!alice.is_alive) {
        std::cout << "Статус: МЕРТВА\n";
    }
    else {
        std::cout << "Текущий статус: " << alice.current_activity << "\n";
        std::cout << "Зарплата (основная): " << alice.salary << " руб./мес.\n";

        if (alice.has_second_job) {
            std::cout << "Зарплата (подработка): " << alice.second_employer.base_salary << " руб./мес.\n";
            std::cout << "Всего доходов: " << (alice.salary + alice.second_employer.base_salary) << " руб./мес.\n";
        }

        std::cout << "Накопления: " << alice.cash << " руб.\n";
    }

    std::cout << "Здоровье: " << alice.health.health_points << "/100\n";
    std::cout << "Всего на лечение: " << alice.health.total_medical_expenses << " руб.\n";
    std::cout << "Дней на больничном: " << alice.health.sick_days << "\n";

    std::cout << "Вычет на квартиру: " << (alice.taxes.property_deduction_claimed ? "Получен" : "Не получен") << "\n";
    if (alice.taxes.property_deduction_claimed) {
        std::cout << "  Сумма: " << alice.taxes.property_deduction_amount << " руб.\n";
    }
    std::cout << "Вычет на лечение: " << alice.taxes.medical_deduction_received << " руб.\n";
    std::cout << "Всего получено вычетов: " << alice.taxes.total_deductions_received << " руб.\n";

    if (alice.car.has_car) {
        std::cout << "\nМашина: " << alice.car.model << " (" << alice.car.health << "%)\n";
    }
    else {
        std::cout << "\nМашина: Отсутствует\n";
    }

    if (alice.dog.has_dog) {
        std::cout << "Собака: " << alice.dog.name << " (" << alice.dog.breed << ")\n";
    }
    else {
        std::cout << "Собака: Отсутствует\n";
    }

    if (alice.has_second_job) {
        std::cout << "\nВторая работа: " << alice.second_employer.company_name << "\n";
        std::cout << "  Премии получено: " << alice.second_employer.total_bonuses_paid << " руб.\n";
    }

}


int main()
{
    SetConsoleOutputCP(1251);

    alice_init();
    simulation();
    alice_printf();
}

//разбить образования на отдельные функции,прописать структуру для машины, 
// чтобы там всё было, также сделать для банка, по зарплатам образования тожетуда же, в отдельные, 
 

//дать название банку, сделать отдельную структуру налогов, сделать структуру работодателя
//ипотеку по калькулятору

//сделать налоговый вычет