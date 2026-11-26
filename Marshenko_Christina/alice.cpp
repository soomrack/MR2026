#include <stdio.h>
#include <stdlib.h>
#include <random>


using RUB = unsigned long long int;
using namespace std;


random_device rd;
mt19937 global_rng(rd());


int get_random_int(int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(global_rng);
}


bool chance(int percent) {
    return get_random_int(0, 99) < percent;
}


struct Person {
    int birth_year;
    int birth_month;
    int age;
    int family_type;

    int social_level;
    RUB socialization_expense;

    bool has_partner;
    int partner_months;
    RUB relationship_expense;

    bool is_married;
    int marriage_months;
    RUB wedding_cost;

    bool on_maternity_leave;
    int maternity_months;
    RUB partner_salary;

    bool has_child;
    int child_age_months;
    RUB child_expense;
    double inflation_child;

    bool is_studying;
    RUB education_cost;
    bool education_is_free;
    int exam_score;

    bool has_job;
    bool has_second_job;
    RUB salary;
    RUB salary_second_job;
    RUB cash;
    int work_experience;
    bool has_credit;
    RUB credit_amount;
    RUB monthly_payment;

    bool is_renting;
    RUB rent_expense;
    bool has_mortgage;
    bool mortgage_was_taken;
    RUB mortgage_debt;
    RUB mortgage_payment;
    RUB housing_expense;
    double inflation_housing;
    double inflation_utility;

    int health;
    RUB treatment_cost;
    double inflation_medical;
    RUB chronic_cost;
    int stress;
    bool visits_psychologist;
    RUB psychologist_cost;

    RUB ndfl_paid_year;
    RUB ndfl_deductible_medical;  
    RUB ndfl_deductible_psychologist;  
    RUB ndfl_deductible_education;  
    RUB ndfl_deductible_mortgage_principal;
    RUB ndfl_deductible_mortgage_interest;
    RUB ndfl_deductible_child;

    RUB food_expense;
    double inflation_food;

    bool has_car;
    RUB car_expense;
    RUB car_monthly_expense;
    RUB car_repair_cost;  
    int car_last_repair_month;  

    bool has_pet;
    string pet_type;
    int pet_age_months;
    int pet_max_age_months;
    RUB pet_expense;
};


struct Person alice;

void alice_init() {
    alice.birth_year = 2026;
    alice.birth_month = 9;
    alice.age = 0;
    alice.family_type = 0;

    alice.social_level = 50;
    alice.socialization_expense = 0;

    alice.has_partner = false;
    alice.partner_months = 0;
    alice.relationship_expense = 0;

    alice.is_married = false;
    alice.marriage_months = 0;
    alice.wedding_cost = 0;

    alice.on_maternity_leave = false;
    alice.maternity_months = 0;
    alice.partner_salary = 0;

    alice.has_child = false;
    alice.child_age_months = 0;
    alice.child_expense = 0;
    alice.inflation_child = 0.008;

    alice.is_studying = false;
    alice.education_cost = 0;
    alice.education_is_free = false;
    alice.exam_score = 0;

    alice.has_job = false;
    alice.has_second_job = false;
    alice.salary = 0;
    alice.salary_second_job = 0;
    alice.cash = 0;
    alice.work_experience = 0;
    alice.has_credit = false;
    alice.credit_amount = 0;
    alice.monthly_payment = 0;

    alice.is_renting = false;
    alice.rent_expense = 20000;
    alice.has_mortgage = false;
    alice.mortgage_was_taken = false;
    alice.mortgage_debt = 0;
    alice.mortgage_payment = 0;
    alice.housing_expense = 0;
    alice.inflation_housing = 0.01;
    alice.inflation_utility = 0.005;

    alice.health = 100;
    alice.treatment_cost = 5000;
    alice.inflation_medical = 0.01;
    alice.chronic_cost = 2000;
    alice.stress = 0;
    alice.visits_psychologist = false;
    alice.psychologist_cost = 5000;

    alice.ndfl_paid_year = 0;
    alice.ndfl_deductible_medical = 0;
    alice.ndfl_deductible_psychologist = 0;
    alice.ndfl_deductible_education = 0;
    alice.ndfl_deductible_mortgage_principal = 0;
    alice.ndfl_deductible_mortgage_interest = 0;
    alice.ndfl_deductible_child = 0;

    alice.food_expense = 0;
    alice.inflation_food = 0.001;

    alice.has_car = false;
    alice.car_expense = 0;
    alice.car_monthly_expense = 8000;
    alice.car_repair_cost = 0;
    alice.car_last_repair_month = 0;

    alice.has_pet = false;
    alice.pet_type = "";
    alice.pet_age_months = 0;
    alice.pet_max_age_months = 0;
    alice.pet_expense = 3000;
}


void randomize_birth() {
    int rand_val = get_random_int(0, 99);
    if (rand_val < 30) {
        alice.family_type = 3;
        alice.cash = 20000;
        printf("Year %d, Month %d: Alice was born into a average family. Initial capital: %llu RUB.\n", alice.birth_year, alice.birth_month, alice.cash);
    }
    else if (rand_val < 90) {
        alice.family_type = 2;
        alice.cash = 50000;
        printf("Year %d, Month %d: Alice was born into an wealthy family. Starting capital: %llu RUB.\n", alice.birth_year, alice.birth_month, alice.cash);
    }
    else {
        alice.family_type = 1;
        alice.cash = 200000;
        printf("Year %d, Month %d: Alice is the daughter of influential people. Starting capital: %llu RUB.\n", alice.birth_year, alice.birth_month, alice.cash);
    }
}


void alice_exams(const int year, const int month) {
    if (alice.age == 18 and month == 6) {
        int base_score = 50;
        if (alice.family_type == 1) base_score += 20;
        else if (alice.family_type == 2) base_score += 10;
        base_score += alice.social_level / 5;

        alice.exam_score = base_score + get_random_int(-15, 15);

        if (alice.exam_score < 0) alice.exam_score = 0;
        if (alice.exam_score > 100) alice.exam_score = 100;

        printf("Year %d, Month %d: Alice took exams. Score: %d\n", year, month, alice.exam_score);

        if (alice.exam_score >= 80) {
            alice.education_is_free = true;
            printf("Year %d, Month %d: Alice got a budget spot! Free education.\n", year, month);
        }
        else if (alice.exam_score >= 40) {
            alice.education_is_free = false;
            printf("Year %d, Month %d: Alice didn't get budget spot. Paid education only.\n", year, month);
        }
        else {
            alice.education_is_free = false;
            alice.is_studying = false;
            printf("Year %d, Month %d: Alice failed exams. No university this year.\n", year, month);
        }
    }
}


void alice_education(const int year, const int month) {
    if (!alice.is_studying and alice.age == 18 and month == 9 and alice.exam_score >= 40) {
        alice.is_studying = true;
        if (!alice.education_is_free) {
            alice.education_cost = get_random_int(100000, 300000);
            printf("Year %d, Month %d: Alice entered university (paid). Annual cost: %llu RUB.\n", year, month, alice.education_cost);
        }
        else {
            alice.education_cost = 0;
            printf("Year %d, Month %d: Alice entered university (budget). Free!\n", year, month);
        }
    }

    if (alice.is_studying and month == 9 and !alice.education_is_free) {
        if (alice.cash >= alice.education_cost) {
            alice.cash -= alice.education_cost;
            alice.ndfl_deductible_education += alice.education_cost;
            printf("Year %d, Month %d: Alice paid for education: %llu RUB.\n", year, month, alice.education_cost);
        }
        else {
            alice.is_studying = false;
            alice.stress += 10;
            printf("Year %d, Month %d: Alice can't afford education and dropped out. Stress: %d\n", year, month, alice.stress);
        }
    }

    if (alice.is_studying and alice.age == 22 and month == 6) {
        alice.is_studying = false;

        if (alice.has_job) {
            alice.salary += 15000;
        }
        
        printf("Year %d, Month %d: Alice graduated from university!\n", year, month);
    }
}


void alice_socialization(const int year, const int month) {
    if (alice.age >= 16 and alice.cash > 5000) {
        RUB base_social = get_random_int(2000, 5000);
        if (alice.cash >= base_social) {
            alice.cash -= base_social;
            alice.socialization_expense = base_social;
            alice.social_level += get_random_int(1, 3);
        }
    }

    if (alice.has_pet) {
        alice.social_level += 1;
        alice.stress -= 1;
    }

    if (alice.family_type <= 2 and alice.age < 25) {
        alice.social_level += 1;
    }

    if (alice.social_level > 70 and alice.stress > 0) {
        alice.stress -= 1;
    }

    if (alice.social_level < 20) {
        alice.stress += 1;
    }

    if (alice.social_level < 0) {
        alice.social_level = 0;
    }
    if (alice.social_level > 100) {
        alice.social_level = 100;
    }
}


void alice_relationship(const int year, const int month) {
    if (!alice.has_partner and alice.age >= 18 and alice.social_level > 30) {
        if (chance(5)) {
            alice.has_partner = true;
            alice.partner_months = 0;
            printf("Year %d, Month %d: Alice started a relationship.\n", year, month);
        }
    }

    if (alice.has_partner) {
        alice.partner_months++;

        RUB date_cost = get_random_int(2000, 6000);

        if (alice.cash >= date_cost) {
            alice.cash -= date_cost;
            alice.relationship_expense = date_cost;
            alice.stress -= 2;
            alice.social_level += 1;
        }

        if (alice.social_level > 60) {
            alice.stress -= 1;
        }

        int breakup_chance = 2;
        if (alice.stress > 70) breakup_chance += 5;
        if (alice.cash < 10000) breakup_chance += 3;

        if (chance(breakup_chance)) {
            alice.has_partner = false;
            alice.partner_months = 0;
            alice.stress += 15;
            alice.social_level -= 10;
            printf("Year %d, Month %d: Alice broke up with her partner. Stress: %d\n", year, month, alice.stress);
        }
    }
}


void alice_marriage(const int year, const int month) {
    if (alice.has_partner and !alice.is_married and alice.partner_months >= 24) {
        if (chance(10)) {
            alice.wedding_cost = get_random_int(200000, 500000);
            if (alice.cash >= alice.wedding_cost) {
                alice.cash -= alice.wedding_cost;
                alice.is_married = true;
                alice.marriage_months = 0;
                printf("Year %d, Month %d: Alice got married! Wedding cost: %llu RUB.\n", year, month, alice.wedding_cost);
            }
            else {
                printf("Year %d, Month %d: Alice was proposed but can't afford wedding.\n", year, month);
            }
        }
    }

    if (alice.is_married) {
        alice.marriage_months++;
    }
}


void alice_child(const int year, const int month) {
    if (alice.has_partner and !alice.has_child and alice.age >= 22 and alice.partner_months >= 24) {
        int child_chance = 0;
        if (alice.is_married) {
            child_chance = 5; 
        }
        else {
            child_chance = 1;
        }

        if (chance(child_chance)) {
            alice.has_child = true;
            alice.child_age_months = 0;
            alice.child_expense = 15000;
            printf("Year %d, Month %d: Alice had a child! Monthly expense: %llu RUB.\n", year, month, alice.child_expense);
        }
    }

    if (alice.has_child) {
        alice.child_age_months++;

        if (alice.child_age_months < 36) {
            alice.child_expense = 15000;
        }
        else if (alice.child_age_months < 84) {
            alice.child_expense = 20000;
        }
        else if (alice.child_age_months < 240) {
            alice.child_expense = 25000;
        }
        else {
            alice.child_expense = 0;
        }

        alice.child_expense = alice.child_expense * (1.0 + alice.inflation_child);

        if (alice.cash >= alice.child_expense) {
            alice.cash -= alice.child_expense;
            alice.ndfl_deductible_child += 1400;
        }
        else {
            alice.health -= 3;
            alice.stress += 5;
            printf("Year %d, Month %d: Alice can't afford child expenses. Health: %d, Stress: %d\n", year, month, alice.health, alice.stress);
        }

        alice.stress -= 1;
        if (alice.has_job and !alice.on_maternity_leave) {
            alice.stress += 2;
        }
    }
}


void alice_salary(const int year, const int month) {

    if (!alice.has_job) {
        alice.salary = 0;
        if (alice.family_type == 3 and alice.age >= 16) {
            if (chance(70)) {
                alice.has_job = true;
                alice.salary = 20000;
                printf("Year %d, Month %d: Alice started working part-time. Salary: %llu RUB.\n", year, month, alice.salary);
            }
        }
        else if (alice.age >= 18) {
            alice.has_job = true;
            alice.salary = 60000;
            printf("Year %d, Month %d: Alice started her first serious job. Salary: %llu RUB.\n", year, month, alice.salary);
        }
    }
    if (!alice.has_second_job and alice.age >= 20 and alice.cash < 50000) {
        if (chance(30)) {
            alice.has_second_job = true;
            alice.salary_second_job = 30000;
            printf("Year %d, Month %d: Alice started her second job. Salary: %llu RUB.\n", year, month, alice.salary_second_job);
        }
    }
    if (alice.has_job and !alice.on_maternity_leave) {
        alice.cash += alice.salary;
        ++alice.work_experience;
        RUB ndfl_from_salary = alice.salary * 0.13;
        alice.ndfl_paid_year += ndfl_from_salary;
    }
    if (alice.has_second_job and !alice.on_maternity_leave) {
        alice.cash += alice.salary_second_job;
        RUB ndfl_from_second = alice.salary_second_job * 0.13;
        alice.ndfl_paid_year += ndfl_from_second;
        ++alice.work_experience;
    }
}


void alice_promotion(const int year, const int month) {
    if (alice.has_job and alice.work_experience > 24 and chance(5)) {
        alice.salary += get_random_int(1000, 10000);
        printf("Year %d, Month %d: Alice got a promotion at work. New salary: %llu RUB.\n", year, month, alice.salary);
        alice.work_experience = 0;
    }
}


void alice_maternity(const int year, const int month) {
    if (alice.has_child and alice.is_married and !alice.on_maternity_leave and alice.child_age_months == 0) {
        if (chance(70)) {
            alice.on_maternity_leave = true;
            alice.maternity_months = 0;
            alice.partner_salary = get_random_int(60000, 120000);
            printf("Year %d, Month %d: Alice went on maternity leave. Partner salary: %llu RUB.\n", year, month, alice.partner_salary);
        }
    }

    if (alice.on_maternity_leave) {
        alice.maternity_months++;

        alice.cash += alice.partner_salary;
        RUB ndfl_from_partner = alice.partner_salary * 0.13;
        alice.ndfl_paid_year += ndfl_from_partner;

        int return_chance = 0;
        if (alice.maternity_months >= 18) {
            return_chance = 5;
        }
        if (alice.maternity_months >= 36) {
            return_chance = 30;
        }

        if (chance(return_chance)) {
            alice.on_maternity_leave = false;
            printf("Year %d, Month %d: Alice returned to work after maternity leave.\n", year, month);
        }
    }
}


void alice_food(const int year, const int month) {
    if (alice.has_job or alice.is_renting) {
        alice.food_expense = 15000;
    }
    if (alice.cash >= alice.food_expense) {
        alice.cash -= alice.food_expense;
        alice.food_expense = alice.food_expense * (1.0 + alice.inflation_food);
    }
    else {
        alice.health -= 5;
        printf("Year %d, Month %d: Alice is starving. Health: %d \n", year, month, alice.health);
    }
}


void alice_pet(const int year, const int month) {
    if (!alice.has_pet and alice.cash > 10000 and alice.age > 14) {
        if (chance(3)) {
            if (chance(50)) {
                alice.pet_type = "Cat";
                alice.pet_expense = 2500;
                alice.pet_max_age_months = get_random_int(120, 180);
            }
            else if (chance(90)) {
                alice.pet_type = "Dog";
                alice.pet_expense = 4500;
                alice.pet_max_age_months = get_random_int(130, 200);
            }
            else {
                alice.pet_type = "Lizard";
                alice.pet_expense = 7000;
                alice.pet_max_age_months = get_random_int(50, 300);
            }

            alice.has_pet = true;
            alice.pet_age_months = 0;
            printf("Year %d, Month %d: Alice got a %s! Monthly expense: %llu RUB.\n", year, month, alice.pet_type.c_str(), alice.pet_expense);
        }
    }

    if (alice.has_pet) {
        if (alice.cash >= alice.pet_expense) {
            alice.cash -= alice.pet_expense;
            alice.pet_expense = alice.pet_expense * (1.0 + alice.inflation_food);
            alice.pet_age_months++;
            if (alice.pet_age_months >= alice.pet_max_age_months) {
                printf("Year %d, Month %d: Alice's %s passed away at age %d months. She is grieving.\n", year, month, alice.pet_type.c_str(), alice.pet_max_age_months);
                alice.has_pet = false;
                alice.pet_type = "None";
                alice.pet_expense = 0;
                alice.health -= 10;
            }
        }
        else {
            alice.stress += 10;
            printf("Year %d, Month %d: Alice can't afford her %s and had to give it to a shelter. Stress: %d\n", year, month, alice.pet_type.c_str(), alice.stress);
            alice.has_pet = false;
            alice.pet_type = "None";
            alice.pet_expense = 0;
        }
    }
}


void alice_rent(const int year, const int month) {
    RUB base_rent_expense = 25000;
    if (alice.age > 18 and !alice.is_renting and !alice.mortgage_was_taken and !alice.has_mortgage) {
        if (alice.cash >= 12 * base_rent_expense) {
            alice.rent_expense = base_rent_expense;
            alice.cash -= alice.rent_expense;
            alice.is_renting = true;
            printf("Year %d, Month %d: Alice started living on her own.\n", year, month);
        }
    }
    else if (alice.is_renting) {
        if (alice.cash >= alice.rent_expense) {
            alice.cash -= alice.rent_expense;
            alice.rent_expense = alice.rent_expense * (1.0 + alice.inflation_housing);
        }
        else if (alice.is_renting and alice.cash < alice.rent_expense) {
            alice.stress += 5;
            alice.rent_expense = 0;
            alice.is_renting = false;
            printf("Year %d, Month %d: Alice ran out of money and moved back in with her parents. Stress: %d\n", year, month, alice.stress);
        }
    }
}


void alice_mortgage(const int year, const int month) {
    if (!alice.has_mortgage and alice.age > 25 and alice.cash >= 1100000 and !alice.mortgage_was_taken) {
        alice.cash -= 1000000;
        alice.mortgage_debt = 4000000;
        alice.mortgage_payment = 35000;
        alice.has_mortgage = true;
        alice.is_renting = false;
        printf("Year %d, Month %d: Alice took a mortgage! Initial debt: %llu RUB.\n", year, month, alice.mortgage_debt);
    }

    if (alice.has_mortgage) {
        if (alice.cash >= alice.mortgage_payment) {
            alice.cash -= alice.mortgage_payment;

            RUB interest_part = alice.mortgage_debt * 0.08 / 12;
            RUB principal_part = alice.mortgage_payment - interest_part;

            alice.ndfl_deductible_mortgage_principal += principal_part;
            alice.ndfl_deductible_mortgage_interest += interest_part;

            if (alice.mortgage_debt <= alice.mortgage_payment) {
                alice.mortgage_debt = 0;
                printf("Year %d, Month %d: Alice fully paid off the mortgage!\n", year, month);
                alice.mortgage_was_taken = true;
                alice.has_mortgage = false;
            }
            else {
                alice.mortgage_debt -= alice.mortgage_payment;
            }
        }
        else {
            alice.mortgage_debt = alice.mortgage_debt * 1.02;
            printf("Year %d, Month %d: Alice missed mortgage payment. Debt increased!\n", year, month);
        }
    }
}


void alice_bank_income(const int year, const int month) {
    if (alice.cash > 10000) {
        RUB interest = alice.cash * 0.005;
        alice.cash += interest;
    }
}


void alice_credit(const int year, const int month) {

    if (alice.age >= 18 and chance(3) and !alice.has_credit) {
        if (alice.cash < 10000 and alice.has_job) {
            alice.credit_amount = get_random_int(50000, 200000);
            int credit_months = get_random_int(12, 36);
            alice.monthly_payment = alice.credit_amount / credit_months;
            alice.has_credit = true;
            alice.cash += alice.credit_amount;
            printf("Year %d, Month %d: Alice took a credit of %llu RUB. Monthly payment: %llu RUB.\n", year, month, alice.credit_amount, alice.monthly_payment);
        }
    }

    if (alice.has_credit) {

        if (alice.cash >= alice.monthly_payment) { 
            alice.cash -= alice.credit_amount;
            alice.credit_amount -= alice.monthly_payment;

            if (alice.credit_amount <= 0) {
                printf("Year %d, Month %d: Alice paid off the loan.", year, month);
                alice.has_credit = false;
            }
        }
        else{
            alice.credit_amount *= 1.01; 
            alice.health -= 2;
            printf("Year %d, Month %d: Alice missed credit payment. Debt increased! Health: %d\n", year, month, alice.health);
        }
    }
}


void alice_car(const int year, const int month) {
    int rand_val = get_random_int(0, 99);
    
    if (!alice.has_car and alice.has_job and alice.cash > 500000) {
        if (rand_val < 20) {
            int car_choice = get_random_int(0, 1);
            if (car_choice == 0) {
                alice.cash -= 300000;
                alice.has_car = true;
                alice.car_monthly_expense = 8000;
                printf("Year %d, Month %d: Alice bought a Renault Twingo\n", year, month);
            }
            else {
                alice.cash -= 450000;
                alice.has_car = true;
                alice.car_monthly_expense = 12000;
                printf("Year %d, Month %d: Alice bought a Suzuki Swift\n", year, month);
            }
        }
    }

    if (alice.has_car) {
      
        if (alice.cash >= alice.car_monthly_expense) {
            alice.cash -= alice.car_monthly_expense;
            alice.car_monthly_expense = (alice.car_monthly_expense * (1.0 + alice.inflation_utility));
        }
        else {
            int car_cost = 300000 * rand_val / 100;
            printf("Year %d, Month %d: Alice can't afford to keep the car and sells it for %d.\n", year, month, car_cost);
            alice.has_car = false;
            alice.cash += car_cost;
        }

        if (chance(2)) {
            RUB fine = get_random_int(500, 5000);
            if (alice.cash >= fine) {
                alice.cash -= fine;
                printf("Year %d, Month %d: Alice got a traffic fine: %llu RUB.\n", year, month, fine);
            }
            else {
                alice.stress += 3;
                printf("Year %d, Month %d: Alice got a fine but can't pay! Stress: %d\n", year, month, alice.stress);
            }
        }

        if (chance(5)) {
            RUB repair_cost = get_random_int(10000, 50000);
            if (alice.cash >= repair_cost) {
                alice.cash -= repair_cost;
                printf("Year %d, Month %d: Alice repaired her car. Cost: %llu RUB.\n", year, month, repair_cost);
            }
            else {
                alice.stress -= 5;
                printf("Year %d, Month %d: Alice can't afford car repair! Selling car. Stress: %d\n", year, month, alice.stress);
                alice.has_car = false;
                alice.cash += 150000;
            }
        }
    }
}


void alice_stress(const int year, const int month) {
    if (alice.has_job and alice.work_experience > 12) {
        alice.stress += get_random_int(1, 3);
    }

    if (alice.has_second_job) {
        alice.stress += get_random_int(2, 5);
    }
    
    if (alice.has_credit or alice.has_mortgage) {
        alice.stress += get_random_int(1, 2);
    }
    
    if (alice.cash < 10000 and alice.age > 18) {
        alice.stress += 2;
    }

    if (alice.stress > 70) {
        alice.health -= 2;
        printf("Year %d, Month %d: High stress affecting health. Stress: %d, Health: %d\n", year, month, alice.stress, alice.health);
    }

    if (alice.stress > 90 and chance(10)) {
        alice.health -= 5;
        printf("Year %d, Month %d: Severe stress caused health problems! Health: %d\n", year, month, alice.health);
    }

    if (alice.cash > 50000 and alice.stress > 0) {
        alice.stress -= 1;
    }

    if (alice.stress < 0) alice.stress = 0;
    if (alice.stress > 100) alice.stress = 100;
    }


void alice_psychologist(const int year, const int month) {
    if (!alice.visits_psychologist and alice.stress > 60 and alice.age > 18) {
        if (chance(20)) {
            alice.visits_psychologist = true;
            printf("Year %d, Month %d: Alice started seeing a psychologist. Cost: %llu RUB/month.\n", year, month, alice.psychologist_cost);
        }
    }

    if (alice.visits_psychologist) {
        if (alice.cash >= alice.psychologist_cost) {
            alice.cash -= alice.psychologist_cost;
            alice.ndfl_deductible_psychologist += alice.psychologist_cost;
            alice.stress -= get_random_int(10, 20);

            alice.psychologist_cost = alice.psychologist_cost * 1.0005;

            if (alice.stress < 30 and chance(30)) {
                alice.visits_psychologist = false;
                printf("Year %d, Month %d: Alice feels better and stopped seeing the psychologist.\n", year, month);
            }
        }
        else {
            alice.visits_psychologist = false;
            printf("Year %d, Month %d: Alice can't afford the psychologist anymore.\n", year, month);
        }
    }
}


void alice_disease(const int year, const int month) {
    if (alice.family_type == 3 and alice.age == 0 and chance(15)) {
        printf("Year %d, Month %d: Alice was born with a chronic illness. Monthly cost: %llu RUB. \n", year, month, alice.chronic_cost);
        if (alice.cash >= alice.chronic_cost) {
            alice.cash -= alice.chronic_cost;
        }
        else {
            alice.health -= 2;
            printf("Year %d, Month %d: Alice have a chronic illness but can't afford treatment. Health: %d\n", year, month, alice.health);
        }
    }

    if (chance(2)) {
        RUB cost = get_random_int(1000, 3000);
        if (alice.cash >= cost) {
            alice.cash -= cost;
            alice.health -= 1;
            printf("Year %d, Month %d: Alice got sick. Health: %d. Cost: %llu RUB.\n", year, month, alice.health, cost);
        }
        else {
            alice.health -= 20;
            printf("Year %d, Month %d: Alice got sick but can't afford treatment. Health: %d\n", year, month, alice.health);
        }
    }
    if (alice.age > 40 and chance(1) and alice.health < 50) {
        if (alice.cash >= alice.chronic_cost) {
            alice.cash -= alice.chronic_cost;
            alice.health -= 1;
            printf("Year %d, Month %d: Alice developed a chronic illness. Monthly cost: %llu RUB. Health: %d\n", year, month, alice.chronic_cost, alice.health);
        }
        else {
            alice.health -= 6;
            printf("Year %d, Month %d: Alice developed a chronic illness and can't afford it. Health: %d\n", year, month, alice.health);
        }
    }
}


void alice_gifts(const int year, const int month) {
    RUB gift_cost = get_random_int(3000, 10000);
    if (month == alice.birth_month and alice.age > 0 and chance(5)) {
        printf("Year %d, Month %d: People around her remembered Alice's birthday. Gifted: %d\n", year, month, gift_cost, alice.health);
        alice.cash += gift_cost;
    }
    if (chance(1)) {
        if (alice.cash >= gift_cost) {
            alice.cash -= gift_cost;
            printf("Year %d, Month %d: Alice bought gifts for family. Cost: %llu RUB.\n", year, month, gift_cost);
        }
    }
}


void alice_treatment(const int year, const int month) {
    double rand_val = get_random_int(0, 99) / 100.0;
    if (alice.health < 80) {
        if (alice.cash >= alice.treatment_cost) {
            alice.cash -= alice.treatment_cost * (1 + rand_val);
            alice.ndfl_deductible_medical += alice.treatment_cost * (1 + rand_val);
            alice.health += rand_val * 100;

            if (alice.health > 100) {
                alice.health = 100;
            }
            alice.treatment_cost = alice.treatment_cost * (1.0 + alice.inflation_medical);

            printf("Year %d, Month %d: Alice visited a doctor. Health restored to %d. Cost: %llu RUB.\n", year, month, alice.health, alice.treatment_cost);
        }
        else {
            alice.health -= 3;
            printf("Year %d, Month %d: Alice needs medical help but cannot afford it. Health drops to %d.\n", year, month, alice.health);
        }
    }
}


void alice_ndfl_return(const int year, const int month) {
    if (month != alice.birth_month) {
        return;
    }

    RUB social_deductible = alice.ndfl_deductible_medical + alice.ndfl_deductible_psychologist + alice.ndfl_deductible_education;
    RUB max_social_base = 150000;
    RUB actual_social_base;

    if (social_deductible < max_social_base) {
        actual_social_base = social_deductible;
    }
    else {
        actual_social_base = max_social_base;
    }

    RUB social_refund = actual_social_base * 0.13;

    RUB max_principal_base = 2000000;
    RUB max_interest_base = 3000000;
    RUB actual_principal;

    if (alice.ndfl_deductible_mortgage_principal < max_principal_base) {
        actual_principal = alice.ndfl_deductible_mortgage_principal;
    }
    else {
        actual_principal = max_principal_base;
    }

    RUB actual_interest;

    if (alice.ndfl_deductible_mortgage_interest < max_interest_base) {
        actual_interest = alice.ndfl_deductible_mortgage_interest;
    }
    else {
        actual_interest = max_interest_base;
    }

    RUB property_refund = (actual_principal + actual_interest) * 0.13;
    RUB child_refund = alice.ndfl_deductible_child * 0.13;

    RUB total_calculated = social_refund + property_refund + child_refund;
    RUB refund;

    if (total_calculated < alice.ndfl_paid_year) {
        refund = total_calculated;
    }
    else {
        refund = alice.ndfl_paid_year;
    }

    if (refund > 0) {
        alice.cash += refund;
        printf("Year %d, Month %d: Alice received tax refund: %llu RUB.\n", year, month, refund);

        alice.ndfl_deductible_medical = 0;
        alice.ndfl_deductible_psychologist = 0;
        alice.ndfl_deductible_education = 0;
        alice.ndfl_deductible_mortgage_principal = 0;
        alice.ndfl_deductible_mortgage_interest = 0;
        alice.ndfl_deductible_child = 0;
        alice.ndfl_paid_year = 0;
    }
}


void simulation() {
    int year = 2026;
    int month = 9;
    while (not (year == 2080 and month == 9)) {
        if (year > alice.birth_year and month == alice.birth_month) {
            ++alice.age;
        }
        alice_food(year, month);
        alice_salary(year, month);
        alice_promotion(year, month);
        alice_rent(year, month);
        alice_car(year, month); 
        alice_mortgage(year, month); // взависимости от положения
        alice_pet(year, month); // случайности
        alice_bank_income(year, month);
        alice_treatment(year, month);
        alice_exams(year, month);
        alice_education(year, month);
        alice_socialization(year, month);
        alice_relationship(year, month);
        alice_child(year, month);
        alice_credit(year, month); 
        alice_disease(year, month);
        // alice_salary_bonus(year, month); от социализации
        alice_stress(year, month);
        alice_psychologist(year, month);
        // alice_bad_habits(year, month);
        // alice_vacation(year, month);
        // alice_health_insurance(year, month);
        // alice_hobbies(year, month);
        // alice_utilities(year, month); ежемесячная бытовуха
        alice_gifts(year, month); //связь с друзьями
        alice_ndfl_return(year, month);
        // Идеи отделить, между функциями 2 
        
        if (alice.health <= 0) {
            printf("Year %d, Month %d: Alice couldn't take it anymore.\n", year, month);
            break;
        }
        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}


void alice_printf() {
    printf("\n--- Simulation Results ---\n");
    printf("alice cash = %llu\n", alice.cash);
    printf("Current food expense: %llu RUB\n", alice.food_expense);

    if (alice.is_renting) {
        printf("Current housing expense: %llu RUB\n", alice.rent_expense);
    }

    if (alice.has_mortgage) {
        printf("Current mortgage debt: %llu RUB\n", alice.mortgage_debt);
    }

    if (alice.mortgage_was_taken and !alice.has_mortgage) {
        printf("Mortgage was fully paid off!\n");
    }

}


int main()
{
    alice_init();
    randomize_birth();
    simulation();
    alice_printf();
}
