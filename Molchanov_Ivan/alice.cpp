#include <stdio.h>  
#include <time.h>
#include <random>
#include <iostream>

using RUB = int;

struct Person{
    
    std::string name;

    RUB cash;

    //INCOME
    RUB salary;
    RUB second_salary;

    //EXPENDITURE
    RUB rent;
    RUB product_cost;

    //BANK
    RUB investment;
    RUB loan;
    
    //HAVING
    bool having_car;
    bool having_job;
    bool having_second_job;
    bool having_flat;
    
    //STATUS
    bool ill;
    bool sick_leave;

    //POINTS
    short health;


};

struct Costs{
    RUB products;
    RUB car;
    RUB car_repair;
    RUB fuel;
    RUB medicines;
    RUB hobby;
    RUB traffic_fine;
    RUB car_insurance;
};

struct Inflation{
    float products;
    float fuel;
    float rent;
    float medicines;
    float car;
    float salary;
};

struct Points{
    unsigned short health;
};

struct Costs cost;
struct Person alice;
struct Points points_alice;
struct Inflation inflation;

unsigned short month = 9;
unsigned short year = 2026;

short count_paid_sick_leave = 2;


// FUNCTIONS 

void alice_init()
{
    alice.name = "Alice";

    alice.cash = 20'000; 
    
    //income
    alice.salary = 80'000;
    alice.second_salary = 20'000;

    //expenditure
    alice.rent = 50'000;

    //having
    alice.having_car = false;
    alice.having_job = false;
    alice.having_second_job = false;
    alice.having_flat = false;
    alice.sick_leave = false;

    //points
    alice.health = 100;
    
}

void cost_init()
{
    cost.products = 20'000;
    cost.car = 500'000;
    cost.car_repair = 50'000;
    cost.fuel = 70.62 * 96;
    cost.medicines = 5'000;
    cost.hobby = 5'000;
}

void inflation_init()
{
    inflation.products = 0.08 / 12;
    inflation.fuel = 0.10 / 12;
    inflation.rent = 0.06 / 12;
    inflation.medicines = 0.07 / 12;
    inflation.car = 0.05 / 12;
    inflation.salary = 0.04 / 12;
}

void inflation_calculate()
{
    cost.products += cost.products * inflation.products;
    cost.fuel += cost.fuel * inflation.fuel;
    cost.medicines += cost.medicines * inflation.medicines;
    cost.car += cost.car * inflation.car;
    cost.car_repair += cost.car_repair * inflation.car;
    alice.rent += alice.rent * inflation.rent;
    alice.salary += alice.salary * inflation.salary;
    alice.second_salary += alice.second_salary * inflation.salary;
    std::cout << "Inflation change cost" << std::endl;
}

void print_output()        
{
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Simulation ended!" << std::endl;
    std::cout << "Simulation ended in month: " << month << ", year: " << year << std::endl;
    std::cout << "Alice's cash: " << alice.cash << " RUB" << std::endl;
    std::cout << "Alice's investment: " << alice.investment << " RUB" << std::endl;
}

void print_monthly_output()
{
    std::cout << "   Alice's monthly cash: " << alice.cash << " RUB" << std::endl;
    std::cout << "   Alice's investment: " << alice.investment << " RUB" << std::endl;
}

    //health

void alice_ill()
{
    short a = rand()%100;
    if (alice.ill == false && a < 5) { //5% chance of getting ill
        alice.health -= 20;
        alice.ill = true;
        std::cout << " | " << alice.name << " got ill! Health decreased by 20 points." << std::endl;
    }
        else if (a < 1){ //1% chance of getting seriously ill
            alice.health -= 50;
            alice.ill = true;
            std::cout << " | " << alice.name << " got seriously ill! Health decreased by 50 points." << std::endl;
        }
}

void paid_sick_leave()
{
    alice.cash += alice.salary / 2; // Person receives half of salary during sick leave
    count_paid_sick_leave -= 1;
    std::cout << " | " << alice.name << " received half of salary during paid sick leave." << std::endl;
}

void not_paid_sick_leave() 
{
    if (alice.having_job == true) {
        std::cout << " | " << alice.name << " took a not paid sick leave." << std::endl;
    }
}

void alice_health_check()
{
    if (alice.health <= 50) {
        std::cout << " | " << alice.name << "'s health is low (" << alice.health << ")." << std::endl;
        if (count_paid_sick_leave > 0) {
            std::cout << " | " << alice.name << " took a paid sick leave." << std::endl;
            paid_sick_leave();
        }
            else {
                not_paid_sick_leave();
            }
        }

    if (month == 13){
            count_paid_sick_leave = 2;
    }

}

//BANK

void alice_investing() 
{
    if (alice.cash > 10'000) {
        int amount = alice.cash - 10'000;
        alice.cash -= amount;
        alice.investment += amount;
        std::cout << " | " << alice.name << " invested " << (amount) << " RUB." << std::endl;
    }
}

void alice_investment_profit() 
{
    alice.investment += alice.investment * (13/12)*0.01; // 13% profit per year, calculated monthly
}

void withdraw_investment(Person& p) 
{
    p.cash += p.investment;
    p.investment = 0;

}

void alice_salary() 
{
    if (alice.having_job == true && alice.ill == false) {
        alice.cash += alice.salary;
        std::cout << " | Alice received her salary of " << alice.salary << " RUB." << std::endl;
    }
    
}

void alice_second_salary()
{
    if (alice.having_second_job == true && alice.ill == false) {
        alice.cash += alice.second_salary;
        std::cout << " | Alice received her second job salary of " << alice.second_salary << " RUB." << std::endl;
    }
}

void alice_cash_check()
{
    if (alice.cash < 10'000){
        std::cout << " | " << alice.name << "'s cash is low (" << alice.cash << ")." << std::endl;
        if (alice.investment > 0) {
            alice.investment -= 20'000;
            alice.cash += 20'000;
            std::cout << " | " << alice.name << " withdrew her investment to cover expenses." << std::endl;
        }
        else if (alice.cash < 0){
            std::cout << "! Alise has monetary debt" << std::endl;
        }
    }

}
    //EXPENDITURE

void alice_rent() 
{
    alice.cash -= alice.rent;
    std::cout << " | Alice paid her rent of " << alice.rent << " RUB." << std::endl;
}

void alice_buy_products()
{
    if (alice.cash >= cost.products) {
        alice.cash -= cost.products;
        std::cout << " | Alice bought products for " << cost.products << " RUB." << std::endl;
    }
}



void alice_buying_medicines()
{
    if (alice.ill == true && alice.health <= 50) {
        alice.cash -= cost.medicines;
        alice.health += 30;
        std::cout << " | " << alice.name << " bought medicines for " << cost.medicines << " RUB. Health improved to " << alice.health << "." << std::endl;
    }
}

void alice_hobby()
{
    if (alice.cash >= cost.hobby){
        alice.cash -= cost.hobby;
        std::cout << " | Alice spend "<< cost.hobby <<" to hobby." << std::endl;
    }
}
//Changes in Alice's life

void alice_get_job() 
{
    if (alice.having_job == false) {
        alice.having_job = true;
        std::cout << " | Alice got a job!" << std::endl;
    }
}

void alice_get_second_job()
{
    if (alice.cash <= 5'000){
        alice.having_second_job = true;
        std::cout << " | Alice got a second job, she need more money" << std::endl;
    }
}

void alice_quit_second_job() 
{
    if (alice.having_second_job == true && alice.cash >= 50'000) {
        alice.having_second_job = false;
        std::cout << " | Alice quit her second job, it's enough money." << std::endl;
    }
}

//CAR

short car_age = 1; //age month car
short month_buy_car = 1; //month when Alice buy car

void car_breaking() 
{
    if (rand()%100 < (10+(car_age*0.01))) { //10% chance of breaking
        alice.cash -= cost.car_repair;
        std::cout << " | Alice's car broke down! She had to pay " << cost.car_repair << " RUB for repairs." << std::endl;
    }

}

void car_aging()
{
    if (alice.having_car == true){
        car_age++;
    }
    
}

void alice_car_pay() 
{
    if (alice.having_car == true){
        alice.cash -= cost.fuel;
        std::cout << " | Alice paid for fuel: " << cost.fuel << " RUB." << std::endl;
        car_breaking();
    }
}

void alice_buy_car() 
{
    if (alice.cash + alice.investment >= 600'000) {
        withdraw_investment(alice);
        alice.cash -= cost.car;
        alice.having_car = true;
        month_buy_car = month;
        std::cout << " | Alice bought a new car for " << cost.car << " RUB." << std::endl;
    }
}


void alice_car_insurance_payment(){
    if (alice.having_car && month == month_buy_car){
        alice.cash -= cost.car_insurance;
        std::cout << " | Alice bought insurance for her car" << std::endl;

    }
}

void alice_traffic_fine(){
    if (rand()%100 < 3){
        alice.cash -= cost.traffic_fine;
        std::cout << " | Alice get traffic fine " << cost.traffic_fine <<" RUB."<< std::endl;

    }

}



void simulation()
{

    std::cout << alice.cash << std::endl;

    while (!(year == 2027 && month == 9)){
        
        std::cout << "Month: " << month << ", Year: " << year << std::endl;
        
        alice_get_job();
        alice_get_second_job();
        alice_ill();
        alice_quit_second_job();
        //Income

        alice_salary();
        alice_second_salary();

        //Expenditure

        alice_rent();
        alice_buy_products();
        alice_buying_medicines();
        //alice_hobby();
        
        //health
        alice_health_check();

        //bank
        alice_cash_check();
        alice_investing();
        alice_investment_profit();

        //car
        alice_buy_car();
        car_aging();
        alice_car_pay();
        alice_car_insurance_payment();
        alice_traffic_fine();

        inflation_calculate();

        print_monthly_output();

        month++;
        if (month == 13){
            year++;
            month = 1;
        }
    }

}


int main()
{
    srand(time(NULL));

    inflation_init();
    
    cost_init();

    alice_init();

    simulation();

    print_output();
}
