#include <iostream>
#include <string>
#include <random>
#include <cstdio>
#include <windows.h>
#include <locale>


//typedef int RUB;
using RUB = unsigned long long int;

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
    RUB total_amount = 0;          //общая сумма ипотеки
    RUB remaining_balance = 0;     //остаток долга
    double annual_rate = 0.0;      //годовая ставка
    int term_months = 0;           //срок в месяцах
    RUB monthly_payment = 0;       //ежемесячный платеж
    int months_paid = 0;           //сколько месяцев уже выплачено
};

struct Loan {
    bool is_active = false;
    RUB total_amount = 0;          //общая сумма кредита
    RUB remaining_balance = 0;     //остаток долга
    double annual_rate = 0.0;      //годовая ставка
    int term_months = 0;           //срок в месяцах
    RUB monthly_payment = 0;       //ежемесячный платеж
    int months_paid = 0;           //сколько месяцев уже выплачено
};

struct Bank {
    double tax_rate = 0.13;
    RUB tax_paid_total = 0;
    RUB scholarship = 0;

    Mortgage mortgage;
    Loan loan;

    static RUB calculate_annuity_payment(RUB amount, double annual_rate, int term_months) {
        double monthly_rate = annual_rate / 100.0 / 12.0;
        if (monthly_rate > 0) {
            return static_cast<RUB>(
                amount *
                (monthly_rate * pow(1 + monthly_rate, term_months)) /
                (pow(1 + monthly_rate, term_months) - 1)
                );
        }
        return amount / term_months;
    }
};

struct Person {
    RUB cash = 0;
    RUB salary = 0;

    int birth_year = 2019;

    bool is_well_off_family = false;
    LifeStatus status = LifeStatus::SCHOOL;
    EducationLevel education = EducationLevel::NONE;
    std::string current_activity = "Учится в школе (1 класс)";

    int work_experience_months = 0;

    Car car;
    Bank bank;

    double oge_average = 0.0;
    int ege_total = 0;

    int get_age(int current_year) const {
        return current_year - birth_year;
    }

    int last_phone_purchase_year = 0;
    bool has_phone = false;
    bool can_buy_new_phone(int current_year) const {
        if (!has_phone) return true;
        return (current_year - last_phone_purchase_year) >= 2;
    }
};

struct Person alice;

std::random_device rd;
std::mt19937 gen(rd());

bool check_chance(int chance_percent) {
    std::uniform_int_distribution<> distrib(1, 100);
    return distrib(gen) <= chance_percent;
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
        std::cout << "Провал ОГЭ. Элис идет работать.\n";
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
        std::cout << "Баллов не хватило. Элис идет работать.\n";
    }
}


void alice_college(const int year, const int month) {
    if (alice.status != LifeStatus::COLLEGE) return;

    int college_year = year - 2035;

    if (year == 2038 && month == 6) {
        alice.status = LifeStatus::WORKING;
        alice.education = EducationLevel::COLLEGE;  
        alice.current_activity = "Работает (колледж)";
        std::cout << "\n " << year << " год: Элис окончила колледж!\n";
    }
}

void alice_university(const int year, const int month) {
    if (alice.status != LifeStatus::UNIVERSITY) return;

    int uni_year = year - 2037;

    if (year == 2041 && month == 6) {
        alice.status = LifeStatus::WORKING;
        alice.education = EducationLevel::UNIVERSITY;  
        alice.current_activity = "Работает (университет)";
        std::cout << "\n" << year << " год: Элис получила диплом ВУЗа!\n";
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

                alice.cash -= alice.car.purchase_price;

                std::cout << "Элис купила " << alice.car.model
                    << " за " << alice.car.purchase_price << "\n";
            }
        }
    }
    
    else {
        alice.cash -= alice.car.monthly_cost;

        alice.car.age_months++;

        int wear_factor = (100 - alice.car.health) / 10;
        int age_factor = alice.car.age_months / 12; 
        int breakdown_chance = 3 + wear_factor + age_factor;

        breakdown_chance = (std::min)(breakdown_chance, 25);

        if (check_chance(breakdown_chance)) {
            int repair_roll = rand() % 100;

            if (repair_roll < 55) {
                alice.cash -= alice.car.light_repair_cost;
                alice.car.health -= 2;
                std::cout << "Лёгкий ремонт " << alice.car.model
                    << " (-" << alice.car.light_repair_cost
                    << "). Состояние: " << alice.car.health << "%" << "\n";
            }
            
            else if (repair_roll < 85) {
                alice.cash -= alice.car.heavy_repair_cost;
                alice.car.health -= 10;
                std::cout << "Сложный ремонт " << alice.car.model
                    << " (-" << alice.car.heavy_repair_cost
                    << "). Состояние: " << alice.car.health << "%" << "\n";
            }
            
            else {
                alice.cash -= alice.car.capital_repair_cost;
                alice.car.health -= 25;
                std::cout << "Капитальный ремонт " << alice.car.model
                    << " (-" << alice.car.capital_repair_cost
                    << "). Состояние: " << alice.car.health << "%" << "\n";
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

RUB calculate_salary_by_education(EducationLevel edu, int experience_months) {
    RUB base_salary = 0;

    switch (edu) {
    case EducationLevel::NONE:
        base_salary = 25000; 
        break;
    case EducationLevel::SCHOOL:
        base_salary = 35000;  
        break;
    case EducationLevel::COLLEGE:
        base_salary = 55000; 
        break;
    case EducationLevel::UNIVERSITY:
        base_salary = 80000; 
        break;
    }

    int years_worked = experience_months / 12;
    double experience_bonus = 1.0 + (years_worked * 0.02);

    return static_cast<RUB>(base_salary * experience_bonus);
}

void alice_salary(const int year, const int month) {
    RUB gross_salary = 0;
    RUB net_salary = 0;

    if (alice.status == LifeStatus::WORKING) {
        alice.salary = calculate_salary_by_education(alice.education, alice.work_experience_months);
        gross_salary = alice.salary;
        alice.work_experience_months++;
    }
    else if (alice.status == LifeStatus::UNIVERSITY) {
        alice.bank.scholarship = 5000;
        alice.cash += alice.bank.scholarship;
        return;
    }
    else if (alice.status == LifeStatus::SCHOOL || alice.status == LifeStatus::COLLEGE) {
        if (alice.is_well_off_family) {
            alice.cash += 3000;
        }
        return;
    }

    if (gross_salary > 0) {
        RUB tax = static_cast<RUB>(gross_salary * alice.bank.tax_rate);
        net_salary = gross_salary - tax;

        alice.cash += net_salary;
        alice.bank.tax_paid_total += tax;

    }
}

void alice_try_phone_loan(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;
    if (alice.bank.loan.is_active) return;

    if (!alice.can_buy_new_phone(year)) {
        return;
    }

    const RUB phone_price = 150000;
    if (alice.cash >= phone_price) return;
    if (!check_chance(5)) return;

    const double loan_rate = 15.0;
    const int loan_term_months = 24;

    RUB monthly_payment = Bank::calculate_annuity_payment(
        phone_price, loan_rate, loan_term_months
    );

    alice.bank.loan.is_active = true;
    alice.bank.loan.total_amount = phone_price;
    alice.bank.loan.remaining_balance = phone_price;
    alice.bank.loan.annual_rate = loan_rate;
    alice.bank.loan.term_months = loan_term_months;
    alice.bank.loan.months_paid = 0;
    alice.bank.loan.monthly_payment = monthly_payment;

    alice.cash -= phone_price;
    alice.has_phone = true;
    alice.last_phone_purchase_year = year;

    std::cout << " Кредит" << year << " год: Элис взяла кредит на телефон! Платеж: "
        << monthly_payment << " руб.\n";
}

void alice_try_mortgage(const int year, const int month) {
    if (alice.status != LifeStatus::WORKING) return;

    if (alice.bank.mortgage.is_active) return;

    int age = alice.get_age(year);
    if (age < 25) return;

    const RUB mortgage_amount = 3000000;
    const double mortgage_rate = 8.5;
    const int mortgage_term_years = 20;

    double monthly_rate = mortgage_rate / 100.0 / 12.0;
    int term_months = mortgage_term_years * 12;

    RUB estimated_payment = Bank::calculate_annuity_payment(
        mortgage_amount, mortgage_rate, term_months
    );

    if (alice.salary < estimated_payment * 2) {
        return;
    }

    if (!check_chance(15)) return;

    alice.bank.mortgage.is_active = true;
    alice.bank.mortgage.total_amount = mortgage_amount;
    alice.bank.mortgage.remaining_balance = mortgage_amount;
    alice.bank.mortgage.annual_rate = mortgage_rate;
    alice.bank.mortgage.term_months = term_months;
    alice.bank.mortgage.months_paid = 0;
    alice.bank.mortgage.monthly_payment = estimated_payment;

    std::cout << year << " год (возраст " << age
        << "): Элис решила взять ипотеку!\n";
    std::cout << " Сумма: " << mortgage_amount << " руб., ставка: "
        << mortgage_rate << "%, срок: " << mortgage_term_years << " лет\n";
    std::cout << " Ежемесячный платеж: " << estimated_payment << " руб.\n";
}

void alice_pay_mortgage(const int year, const int month) {
    if (!alice.bank.mortgage.is_active) return;

    if (alice.cash >= alice.bank.mortgage.monthly_payment) {
        alice.cash -= alice.bank.mortgage.monthly_payment;
        alice.bank.mortgage.remaining_balance -= alice.bank.mortgage.monthly_payment;
        alice.bank.mortgage.months_paid++;

        if (alice.bank.mortgage.remaining_balance <= 0) {
            alice.bank.mortgage.is_active = false;
            alice.bank.mortgage.remaining_balance = 0;
            std::cout << " Ипотека полностью погашена в " << year << " году!\n";
        }
    }
    else {
        std::cout << "Недостаточно средств для оплаты ипотеки в " << year << " году!\n";
    }
}

void alice_pay_loan(const int year, const int month) {
    if (!alice.bank.loan.is_active) return;

    if (alice.cash >= alice.bank.loan.monthly_payment) {
        alice.cash -= alice.bank.loan.monthly_payment;
        alice.bank.loan.remaining_balance -= alice.bank.loan.monthly_payment;
        alice.bank.loan.months_paid++;

        if (alice.bank.loan.remaining_balance <= 0) {
            alice.bank.loan.is_active = false;
            alice.bank.loan.remaining_balance = 0;
            std::cout << "Кредит полностью погашен в " << year << " году!\n";
        }
    }
    else {
        std::cout << "Недостаточно средств для оплаты кредита в " << year << " году!\n";
    }
}


void alice_food(const int year, const int month) {
    const RUB food_cost = 20000;

    alice.cash -= food_cost;
}

void simulation() {
    int year = 2026;
    int month = 9;
    while (not (year == 2060 and month == 1)) {
        alice_education(year, month);
        alice_salary(year, month);
        alice_car(year, month);
        alice_try_mortgage(year, month);
        alice_try_phone_loan(year, month);
        alice_pay_mortgage(year, month);
        alice_pay_loan(year, month);
        //alice_rent();
        //alice_home_bills();
        alice_food(year, month);
        //alice_dog();
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

//разбить образования на отдельные функции,прописать структуру для машины, чтобы там всё было, также сделать для банка, по зарплатам образования тожетуда же, в отдельные, 