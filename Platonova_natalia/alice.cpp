#include <iostream>
#include <string>
#include <random>
#include <cstdio>
#include <windows.h>
#include <locale>


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
    RUB total_tax_paid = 0;               

    double property_tax_rate = 0.001;     
    RUB property_tax_annual = 0;          
    bool has_property_tax = false;        

    RUB vehicle_tax_annual = 0;           
    bool has_vehicle_tax = false;         

    double pension_contribution_rate = 0.22;  
    RUB total_pension_contributions = 0;      

    RUB calculate_income_tax(RUB gross_salary) const {
        return static_cast<RUB>(gross_salary * income_tax_rate);
    }

    RUB calculate_property_tax(RUB property_value) const {
        return static_cast<RUB>(property_value * property_tax_rate);
    }

    RUB calculate_vehicle_tax(int horse_power) const {
        return static_cast<RUB>(horse_power * 50);
    }
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

    int calculate_sickness_chance(int age_years, int health) const {
        int base_chance = 2;  

        if (age_years > 40) base_chance += (age_years - 40) / 5;
        if (age_years > 60) base_chance += (age_years - 60) / 3;

        if (health < 50) base_chance += 5;
        if (health < 30) base_chance += 10;

        return (std::min)(base_chance, 50); 
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
        std::cout << "  Не хватает денег на: " << purpose
            << "! Требуется: " << amount << " руб., есть: " << balance << " руб.\n";
        balance = 0; 
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

        gross_salary = alice.employer.calculate_gross_salary(true);

        alice.employer.bonus_chance = original_bonus_chance;

        RUB pension_contribution = alice.employer.calculate_pension_contribution(gross_salary);

        RUB income_tax = alice.taxes.calculate_income_tax(gross_salary);
        RUB net_salary = gross_salary - income_tax;

        alice.cash += net_salary;
        alice.salary = gross_salary;
        alice.work_experience_months++;
        alice.taxes.total_tax_paid += income_tax;

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

    std::cout << "  [КРЕДИТ] " << year << " год: Элис взяла кредит на телефон!\n";
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

void alice_check_health(const int year, const int month) {
    if (!alice.is_alive) return;

    int age_years = alice.get_age(year);

    if (age_years > 40 && month == 1) {
        int health_loss = (age_years - 40) / 10; 
        if (health_loss < 1) health_loss = 1;

        alice.health.health_points -= health_loss;
        if (alice.health.health_points < 0) alice.health.health_points = 0;
    }

    int sickness_chance = alice.health.calculate_sickness_chance(age_years, alice.health.health_points);

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
        int death_chance = (age_years - 85) * 5;  // +5% за каждый год после 85
        if (check_chance(death_chance)) {
            alice.is_alive = false;
            std::cout << " " << year << " год: Элис умерла от старости в возрасте "
                << age_years << " лет.\n";
        }
    }
}

void simulation() {
    int year = 2026;
    int month = 9;
    while (not (year == 2090 and month == 1)) {
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
        //alice_bank_income();
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

    alice.employer.bonus_chance = 0.10; 
    alice.employer.bonus_amount = 50000; 
    alice.employer.pension_rate = 0.22;  
}

void alice_printf() {
    std::cout << "Текущий статус: " << alice.current_activity << "\n";
    std::cout << "Зарплата: " << alice.salary << " руб./мес.\n";
    std::cout << "Накопления: " << alice.cash << " руб.\n";

        if (alice.car.has_car) {
            std::cout << "Машина: " << alice.car.model << "\n";
            std::cout << "  Состояние:  " << alice.car.health << "%\n";
            std::cout << "  Возраст:  " << alice.car.age_months / 12
                << " лет " << alice.car.age_months % 12 << " мес.\n";
        }
        else {
            std::cout << "Машина: Отсутствует\n";
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