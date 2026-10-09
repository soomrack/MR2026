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

    RUB food_expense;
    double inflation_food;

    bool has_car;
    RUB car_expense;
    RUB car_monthly_expense;

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

    alice.food_expense = 0;
    alice.inflation_food = 0.001;

    alice.has_car = false;
    alice.car_expense = 0;
    alice.car_monthly_expense = 8000;

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
    if (alice.has_job) {
        alice.cash += alice.salary;
        ++alice.work_experience;
    }
    if (alice.has_second_job) {
        alice.cash += alice.salary_second_job;
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
            alice.health -= 15;
            printf("Year %d, Month %d: Alice can't afford her %s and had to give it to a shelter. Health: %d\n", year, month, alice.pet_type.c_str(), alice.health);
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
            alice.health -= 5;
            alice.rent_expense = 0;
            alice.is_renting = false;
            printf("Year %d, Month %d: Alice ran out of money and moved back in with her parents. Health: %d\n", year, month, alice.health);
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
            alice.cash -= 300000;
            alice.has_car = true;
            printf("Year %d, Month %d: Alice bought a Renault Twingo\n", year, month);
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
        alice_car(year, month); // ремонт и штрафы и сузуку
        alice_mortgage(year, month); // взависимости от положения
        alice_pet(year, month); // случайности
        alice_bank_income(year, month);
        alice_treatment(year, month);
        // alice_education(year, month);
        // alice_socialization(year, month); + от питомца и семьи
        alice_credit(year, month); 
        alice_disease(year, month);
        // alice_salary_bonus(year, month); от социализации
        // alice_stress(year, month); пересмотреть систему здоровья
        // alice_bad_habits(year, month);
        // alice_vacation(year, month);
        // alice_relationship(year, month);
        // alice_health_insurance(year, month);
        // alice_psychologist(year, month);
        // alice_hobbies(year, month);
        // alice_utilities(year, month); ежемесячная бытовуха
        alice_gifts(year, month); //связь с друзьями
        // налоговый вычет возврат ндфл
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

}

int main()
{
    alice_init();
    randomize_birth();
    simulation();
    alice_printf();
}
