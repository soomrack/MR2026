#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;                                    

using RUB = unsigned long long int;
using Percent = double;


struct Work
{
    RUB salary_month;
    RUB bonus;                 
    string position;
    int experience_years;
    bool has_remote;             
    int vacation_days_used;       
    int vacation_days_total;     
    RUB education_allowance_year;    //бюджет на обучение (от компании)
    RUB transport_compensation_month;
};


struct bank
{
    RUB balance;                  
    RUB deposit;                  
    Percent deposit_rate;        
    RUB credit_card_debt;        
    RUB credit_card_limit;        
    Percent credit_rate;          
    RUB investment;               
    RUB crypto;                  
    RUB gold;                    
    RUB bonds;                    
    RUB pension;                
    RUB life_insurance;          
    RUB debit_card_cashback;    
};


struct Car
{
    RUB value;
    RUB gas_month;
    RUB maintenance_month;
    RUB insurance_year;
    RUB parking_month;
    RUB tax_year;                
    RUB fine_avg;
    int age_months;            
    bool need_repair;           
    RUB repair_cost;     
    RUB washing_month;
    RUB tires_year;              // шины
    RUB diagnostics_year; 
    RUB tolls_month;             
    RUB rental_income;           // каршеринг
    bool has_rental;             
    RUB loan;                    
    RUB loan_month;              // платеж по кредиту
};


struct Property
{
    RUB rent_month;
    RUB utilities_month;          // ЖКХ
    RUB internet_month;
    RUB phone_month;
    RUB insurance_year;           
    bool has_mortgage;           
    RUB mortgage_month;         
    RUB mortgage_debt;           
    RUB property_tax_year;      
    RUB renovation_year;          
    RUB furniture_year;           
    RUB appliances_year;          
    bool owns_apartment;         
    RUB apartment_value;       
};


struct Cat
{
    string name;
    string color;
    int age;
    RUB food_month;
    RUB vet_month;
    RUB toys_month;
    RUB insurance_month;         
    bool sick;                   
    int sick_days;               
    RUB grooming_month;          
    RUB treats_month;            
    RUB bedding_year;            
    RUB carrier_one_time;        
    bool carrier_bought;         // Куплена ли уже переноска
};


struct FoodExpenses
{
    RUB groceries;               
    RUB eating_out;                
    RUB fast_food;           
    RUB delivery;                  
    RUB coffee;                  
    RUB sweets;                  
    RUB fruits;
    RUB vegetables;
    RUB meat;
    RUB fish;
    RUB cottage_cheese;  
    RUB bread;
    RUB alcohol;
    RUB water;                      
    RUB juices;
    RUB snacks;                
    RUB frozen;                     
    RUB canned;                     
    RUB spices;                     
    RUB baby_food;                  
};


struct HealthExpenses
{
    RUB medicine;                 //reg
    RUB pharmacy;                 //one
    RUB dentist;                    
    RUB therapist;                  
    RUB cardiologist;               
    RUB psychologist;               
    RUB massage;                    
    RUB fitness;                   
    RUB beauty;                     
    RUB barber;                      
    RUB cosmetics;                   
    RUB perfume;                     
    RUB ophthalmologist;             
    RUB surgeon;                     
    RUB hospital;                    
    RUB physiotherapy;               
    RUB vitamins;              
    RUB dietary_supplements;     
    RUB medical_tests;         
    RUB ambulance;      
};


struct ClothingExpenses
{
    RUB clothes;                     
    RUB shoes;                       
    RUB accessories;                 
    RUB dry_clean;                   
    RUB repair;                       
    RUB sportswear;                   
    RUB underwear;                    
    RUB socks;                        
    RUB hats;                         
    RUB gloves;                      
    RUB bags;                         
    RUB jewelry;                     
    RUB watches;                      
};


struct TransportExpenses
{
    RUB public_transport; 
    RUB taxi;
    RUB taxi_work;
    RUB taxi_weekend;                
    RUB train;                         
    RUB plane;                         
    RUB bus;                          
    RUB subway;                      
    RUB bicycle;                       
    RUB scooter;                       
    RUB rideshare;                    
    RUB car_sharing;                 
};


struct EntertainmentExpenses
{
    RUB travel;                       
    RUB hotel;                          
    RUB hostel;                         
    RUB camping;                        
    RUB museum;
    RUB theater;
    RUB concert;
    RUB cinema;
    RUB hobbies;                        
    RUB subscriptions;                  
    RUB sport;                        
    RUB souvenirs;                   
    RUB photos;                      
    RUB books;
    RUB music;                          
    RUB games;                           
    RUB streaming;                     
    RUB nightclub;                        
    RUB bar;                              
    RUB karaoke;
    RUB bowling;
    RUB billiard;
    RUB quests;                         
    RUB amusement_park;                 
    RUB zoo;
    RUB aquarium;
};


struct SpecialExpenses
{
    RUB electronics;                    
    RUB hardware;
    RUB software;                      
    RUB gifts;                      
    RUB flowers;                     
    RUB education;
};


struct Person
{
    string name;
    int age;
    bool married;
    bool has_children;
    RUB children_expenses_month;               
    int children_count;
    Work work;
    bank bank;
    Car car;
    Cat cat;
    bool has_pet_dog;
    RUB dog_expenses_month;
    Property property;
    FoodExpenses food;
    HealthExpenses health;
    ClothingExpenses clothing;
    TransportExpenses transport;
    EntertainmentExpenses entertainment;
    SpecialExpenses special;
    RUB deposit_month;                           
    RUB emergency_fund;                        
    int birthday_month;                         
    int wedding_anniversary_month;               
    int child_birth_month;                        
    bool expecting_child;                        
    RUB maternity_payment;                        
    RUB freelance_income_month;                   
    RUB rental_income_month;                       
    RUB dividend_income_year;                      
    RUB student_loan;                              
    RUB student_loan_month;                      
    RUB personal_loan;                            
    RUB personal_loan_month;                     
};

struct Person Alice;
struct Person Bob;


bool random_event(double prob)
{
    return (rand() / (double)RAND_MAX) < prob;     //true с вер prob
}


void alice_salary(const int month, const int year)
{
   
    if (month == 1) {
        if (year >= 2026 && year <= 2035) {
            double index = 1.0;
            if (year == 2026) index = 1.03;     
            else if (year == 2027) index = 1.05;  
            else if (year == 2028) index = 1.04; 
            else if (year == 2029) index = 1.06; 
            else if (year == 2030) index = 1.05;  
            else if (year == 2031) index = 1.04; 
            else if (year == 2032) index = 1.04;  
            else if (year == 2033) index = 1.03;  
            else if (year == 2034) index = 1.03;  
            else if (year == 2035) index = 1.02;
            
            Alice.work.salary_month = (RUB)(Alice.work.salary_month * index);
        }
    }
    
    if (year >= 2026 && year <= 2035) {
        if (month == 6) {
            double bonus_percent = 0.5;
            if (year == 2026) bonus_percent = 0.5;
            else if (year == 2027) bonus_percent = 0.5;
            else if (year == 2028) bonus_percent = 0.6;
            else if (year == 2029) bonus_percent = 0.7;
            else if (year >= 2030) bonus_percent = 0.6;
            
            Alice.bank.balance += (RUB)(Alice.work.salary_month * bonus_percent);
        }
        
        if (month == 12){
            double bonus_percent = 0.3;
            if (year == 2026) bonus_percent = 0.3;
            else if (year == 2027) bonus_percent = 0.4;
            else if (year == 2028) bonus_percent = 0.4;
            else if (year == 2029) bonus_percent = 0.5;
            else if (year >= 2030) bonus_percent = 0.35; 
            
            Alice.bank.balance += (RUB)(Alice.work.salary_month * bonus_percent);
        }
    }
    Alice.bank.balance += Alice.work.salary_month;
}


void bob_salary(const int month, const int year)
{
    if (month == 1) {
        if (year >= 2026 && year <= 2035) {
            double index = 1.0;
            if (year == 2026) index = 1.02;    
            else if (year == 2027) index = 1.1; 
            else if (year == 2028) index = 1.08;
            else if (year == 2029) index = 1.04; 
            else if (year == 2030) index = 1.2;  
            else if (year == 2031) index = 1.02; 
            else if (year == 2032) index = 1.02;  
            else if (year == 2033) index = 1.02;  
            else if (year == 2034) index = 1.1;  
            else if (year == 2035) index = 1.2;  
            
            Bob.work.salary_month = (RUB)(Bob.work.salary_month * index);
        }
    }
    
    if (year >= 2026 && year <= 2035) {
   
        if (month == 9) {
            double bonus_percent = 0.0;
            if (year == 2026) bonus_percent = 0.0;      
            else if (year == 2027) bonus_percent = 0.2;  
            else if (year == 2028) bonus_percent = 0.25; 
            else if (year == 2029) bonus_percent = 0.3;  
            else if (year >= 2030) bonus_percent = 0.25; 
            
            Bob.bank.balance += (RUB)(Bob.work.salary_month * bonus_percent);
        }
        
        if (month == 3) {
            double bonus_percent = 0.0;
            if (year == 2026) bonus_percent = 0.0;     
            else if (year == 2027) bonus_percent = 0.3;  
            else if (year == 2028) bonus_percent = 0.35; 
            else if (year == 2029) bonus_percent = 0.4;  
            else if (year >= 2030) bonus_percent = 0.3;  
            
            Bob.bank.balance += (RUB)(Bob.work.salary_month * bonus_percent);
        }
    }
    Bob.bank.balance += Bob.work.salary_month;
}


void alice_additional_income(int month, int year)
{
    if (random_event(0.3)) {
        Alice.bank.balance += Alice.freelance_income_month;
    }
    if (Alice.car.has_rental && random_event(0.5)) {
        Alice.bank.balance += Alice.car.rental_income;
    }
    if (Alice.property.owns_apartment && random_event(0.1)) {
        Alice.bank.balance += Alice.rental_income_month;
    }
    if (month == 9 && year == 2027) {
        Alice.bank.balance += Alice.dividend_income_year;
    }
    if (month == 9 && year == 2028) {
        Alice.bank.balance += Alice.dividend_income_year * 1.02;
    }
    if (month == 9 && year == 2029) {
        Alice.bank.balance += Alice.dividend_income_year * 1.04;
    }
}


void bob_additional_income(int month, int year)
{
    if (random_event(0.2)) {
        Bob.bank.balance += Bob.freelance_income_month;
    }
    if (Bob.car.has_rental && random_event(0.3)) {
        Bob.bank.balance += Bob.car.rental_income;
    }
    if (month == 11 && year == 2027) {
        Bob.bank.balance += Bob.dividend_income_year;
    }
    if (month == 11 && year == 2028) {
        Bob.bank.balance += Bob.dividend_income_year * 1.01;
    }
}


double random_inflation(double min_percent, double max_percent)
{
    return min_percent + (double)rand() / RAND_MAX * (max_percent - min_percent);

}


void alice_deposit()
{
    if (Alice.bank.balance >= Alice.deposit_month) {
        Alice.bank.deposit += Alice.deposit_month;
        Alice.bank.balance -= Alice.deposit_month;
    }
    else {
        Alice.bank.deposit += Alice.bank.balance;
        Alice.bank.balance = 0;
    }
    Alice.bank.deposit += Alice.bank.deposit * (Alice.bank.deposit_rate / 12 / 100);
    if (Alice.bank.credit_card_debt > 0) {
        Alice.bank.credit_card_debt += Alice.bank.credit_card_debt * (Alice.bank.credit_rate / 12 / 100);
    }
    if (Alice.bank.investment > 0) {
        double change = random_inflation(-5, 8);
        Alice.bank.investment += Alice.bank.investment * (change / 100);
    }
    if (Alice.bank.crypto > 0) {
        double change = random_inflation(-15, 20);
        Alice.bank.crypto += Alice.bank.crypto * (change / 100);
    }
    if (Alice.bank.gold > 0){
        double change = random_inflation(-2, 5);
        Alice.bank.gold += Alice.bank.gold * (change / 100);
    }
    if (Alice.bank.bonds > 0) {
        double change = random_inflation(1, 3);
        Alice.bank.bonds += Alice.bank.bonds * (change / 100);
    }
    if (Alice.bank.pension > 0) {
        Alice.bank.pension += Alice.bank.pension * (0.5 / 12 / 100);
    }
    Alice.bank.balance += Alice.bank.debit_card_cashback;
}


void bob_deposit()
{
    RUB deposit_amt = Bob.deposit_month * 0.8;      //amount
    if (Bob.bank.balance >= deposit_amt) {
        Bob.bank.deposit += deposit_amt;
        Bob.bank.balance -= deposit_amt;
    }
    else {
        Bob.bank.deposit += Bob.bank.balance;
        Bob.bank.balance = 0;
    }
    Bob.bank.deposit += Bob.bank.deposit * (Bob.bank.deposit_rate / 12 / 100);
    if (Bob.bank.credit_card_debt > 0) {
        Bob.bank.credit_card_debt += Bob.bank.credit_card_debt * (Bob.bank.credit_rate / 12 / 100);
    }
    if (Bob.bank.investment > 0) {
        double change = random_inflation(-4, 6);
        Bob.bank.investment += Bob.bank.investment * (change / 100);
    }
    if (Bob.bank.crypto > 0) {
        double change = random_inflation(-10, 15);
        Bob.bank.crypto += Bob.bank.crypto * (change / 100);
    }
    if (Bob.bank.gold > 0) {
        double change = random_inflation(-1, 4);
        Bob.bank.gold += Bob.bank.gold * (change / 100);
    }
    Bob.bank.balance += Bob.bank.debit_card_cashback;
}


void alice_loan_payments()
{ 
    if (Alice.student_loan > 0) {
        Alice.bank.balance -= Alice.student_loan_month;
        Alice.student_loan -= Alice.student_loan_month;
    }
    if (Alice.car.loan > 0) {
        Alice.bank.balance -= Alice.car.loan_month;
        Alice.car.loan -= Alice.car.loan_month;
    }
    if (Alice.personal_loan > 0) {
        Alice.bank.balance -= Alice.personal_loan_month;
        Alice.personal_loan -= Alice.personal_loan_month;
    }
}


void bob_loan_payments()
{
     if (Bob.student_loan > 0)  {
        Bob.bank.balance -= Bob.student_loan_month;
        Bob.student_loan -= Bob.student_loan_month;
    }
    if (Bob.car.loan > 0) {
        Bob.bank.balance -= Bob.car.loan_month;
        Bob.car.loan -= Bob.car.loan_month;
    }
}


RUB apply_monthly_inflation(RUB amount, double inflation_percent)
{
    return amount * (1 + inflation_percent / 100 / 12);
}

RUB apply_yearly_inflation(RUB amount, double inflation_percent)
{
    return amount * (1 + inflation_percent / 100);
}


void alice_property(int month)
{
    double inf_rent = random_inflation(8.5, 9.7);
    double inf_util = random_inflation(7.2, 8.9);
    Alice.bank.balance -= apply_monthly_inflation(Alice.property.rent_month, inf_rent);
    Alice.bank.balance -= apply_monthly_inflation(Alice.property.utilities_month, inf_util);
    Alice.bank.balance -= apply_monthly_inflation(Alice.property.internet_month, inf_util);
    Alice.bank.balance -= apply_monthly_inflation(Alice.property.phone_month, inf_util);
    if (month == 1) {
        Alice.bank.balance -= apply_yearly_inflation(Alice.property.insurance_year, inf_util);
        Alice.bank.balance -= apply_yearly_inflation(Alice.property.property_tax_year, inf_util);
    }
    if (month == 6) {
        Alice.bank.balance -= apply_yearly_inflation(Alice.property.renovation_year, inf_util);
    }
    if (month == 9) {
        Alice.bank.balance -= apply_yearly_inflation(Alice.property.furniture_year, inf_util);
        Alice.bank.balance -= apply_yearly_inflation(Alice.property.appliances_year, inf_util);
    }
    if (Alice.property.has_mortgage) {
        Alice.bank.balance -= Alice.property.mortgage_month;
        Alice.property.mortgage_debt -= Alice.property.mortgage_month * 0.3;
    }
}


void bob_property(int month)
{
    double inf_rent = random_inflation(7.1, 8.3);
    double inf_util = random_inflation(6.2, 7.4);
    Bob.bank.balance -= apply_monthly_inflation(Bob.property.rent_month, inf_rent);
    Bob.bank.balance -= apply_monthly_inflation(Bob.property.utilities_month, inf_util);
    Bob.bank.balance -= apply_monthly_inflation(Bob.property.internet_month, inf_util);
    Bob.bank.balance -= apply_monthly_inflation(Bob.property.phone_month, inf_util);
    if (month == 1) {
        Bob.bank.balance -= apply_yearly_inflation(Bob.property.insurance_year, inf_util);
        Bob.bank.balance -= apply_yearly_inflation(Bob.property.property_tax_year, inf_util);
    }
    if (Bob.property.has_mortgage) {
        Bob.bank.balance -= Bob.property.mortgage_month;
        Bob.property.mortgage_debt -= Bob.property.mortgage_month * 0.25;
    }
}


void alice_food()
{
    double inf = random_inflation(10.5, 11.3);
    RUB total = Alice.food.groceries + Alice.food.eating_out + Alice.food.fast_food + Alice.food.delivery +
                Alice.food.coffee + Alice.food.sweets + Alice.food.fruits + Alice.food.vegetables +
                Alice.food.meat + Alice.food.fish + Alice.food.cottage_cheese + Alice.food.bread + Alice.food.alcohol +
                Alice.food.water + Alice.food.juices + Alice.food.snacks + Alice.food.frozen +
                Alice.food.canned + Alice.food.spices + Alice.food.baby_food;
    Alice.bank.balance -= apply_monthly_inflation(total, inf);
}


void bob_food()
{
    double inf = random_inflation(9.2, 10.1);
    RUB total = Bob.food.groceries + Bob.food.eating_out + Bob.food.fast_food + Bob.food.delivery +
                Bob.food.coffee + Bob.food.sweets + Bob.food.fruits + Bob.food.vegetables +
                Bob.food.meat + Bob.food.fish + Bob.food.cottage_cheese + Bob.food.bread + Bob.food.alcohol +
                Bob.food.water + Bob.food.juices + Bob.food.snacks + Bob.food.frozen +
                Bob.food.canned + Bob.food.spices;
    Bob.bank.balance -= apply_monthly_inflation(total, inf);
}


void alice_cat(int month)
{
    double inf_food = random_inflation(10.5, 11.3);
    double inf_vet = random_inflation(11.5, 12.2);
    double inf_toys = random_inflation(8.5, 9.7);
    double inf_ins = random_inflation(7.2, 8.9);
    double inf_groom = random_inflation(9.0, 10.0);
    Alice.bank.balance -= apply_monthly_inflation(Alice.cat.food_month, inf_food);
    Alice.bank.balance -= apply_monthly_inflation(Alice.cat.vet_month, inf_vet);
    Alice.bank.balance -= apply_monthly_inflation(Alice.cat.toys_month, inf_toys);
    Alice.bank.balance -= apply_monthly_inflation(Alice.cat.insurance_month, inf_ins);
    Alice.bank.balance -= apply_monthly_inflation(Alice.cat.grooming_month, inf_groom);
    Alice.bank.balance -= apply_monthly_inflation(Alice.cat.treats_month, inf_food);
    if (!Alice.cat.carrier_bought) {
        Alice.bank.balance -= Alice.cat.carrier_one_time;
        Alice.cat.carrier_bought = true;
    }
    if (month == 12) {
        Alice.bank.balance -= apply_yearly_inflation(Alice.cat.bedding_year, inf_ins);
    }
    if (Alice.cat.sick) {
        Alice.cat.sick_days--;
        Alice.bank.balance -= apply_monthly_inflation(Alice.cat.vet_month * 2, inf_vet);
        if (Alice.cat.sick_days <= 0) {
            Alice.cat.sick = false;
        }
    }
    else {
        if (random_event(0.02)) {
            Alice.cat.sick = true;
            Alice.cat.sick_days = 1 + rand() % 3;
        }
    }
}


void bob_cat()
{
    double inf_food = random_inflation(9.8, 10.9);
    double inf_vet = random_inflation(10.2, 11.7);
    double inf_toys = random_inflation(7.5, 8.8);
    double inf_ins = random_inflation(6.3, 7.9);
    Bob.bank.balance -= apply_monthly_inflation(Bob.cat.food_month, inf_food);
    Bob.bank.balance -= apply_monthly_inflation(Bob.cat.vet_month, inf_vet);
    Bob.bank.balance -= apply_monthly_inflation(Bob.cat.toys_month, inf_toys);
    Bob.bank.balance -= apply_monthly_inflation(Bob.cat.insurance_month, inf_ins);
    if (Bob.cat.sick) {
        Bob.cat.sick_days--;
        Bob.bank.balance -= apply_monthly_inflation(Bob.cat.vet_month * 1.5, inf_vet);
        if (Bob.cat.sick_days <= 0) {
            Bob.cat.sick = false;
        }
    }
    else {
        if (random_event(0.015)) {
            Bob.cat.sick = true;
            Bob.cat.sick_days = 1 + rand() % 2;
        }
    }
}


void alice_dog()
{
    if (Alice.has_pet_dog)
    {
        double inf = random_inflation(9.0, 10.5);
        Alice.bank.balance -= apply_monthly_inflation(Alice.dog_expenses_month, inf);
    }
}


void alice_car_gas(int month)
{
    double inf = random_inflation(9.8, 10.6);
    Alice.bank.balance -= apply_monthly_inflation(Alice.car.gas_month, inf);
}


void alice_car_maintenance(int month)
{
    double inf = random_inflation(8.2, 9.4);
    Alice.bank.balance -= apply_monthly_inflation(Alice.car.maintenance_month, inf);
}


void alice_car_parking(int month)
{
    double inf = random_inflation(6.8, 7.7);
    Alice.bank.balance -= apply_monthly_inflation(Alice.car.parking_month, inf);
}


void alice_car_wash(int month)
{
    double inf = random_inflation(7.0, 8.0);
    Alice.bank.balance -= apply_monthly_inflation(Alice.car.washing_month, inf);
}


void alice_car_tolls(int month)
{
    double inf = random_inflation(5.0, 6.0);
    if (random_event(0.5)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.car.tolls_month, inf);
    }
}


void alice_car_insurance_tax(int month)
{
    double inf_ins = random_inflation(7.5, 8.9);
    double inf_tax = random_inflation(5.2, 6.8);
    if (month == 1) {
        Alice.bank.balance -= apply_yearly_inflation(Alice.car.insurance_year, inf_ins);
        Alice.bank.balance -= apply_yearly_inflation(Alice.car.tax_year, inf_tax);
    }
}


void alice_car_tires(int month)
{
    double inf = random_inflation(6.0, 7.0);
    if (month == 10) {
        Alice.bank.balance -= apply_yearly_inflation(Alice.car.tires_year, inf);
    }
}


void alice_car_diagnostics(int month)
{
    double inf = random_inflation(5.5, 6.5);
    if (month == 4) {
        Alice.bank.balance -= apply_yearly_inflation(Alice.car.diagnostics_year, inf);
    }
}


void alice_car_fines()
{
    if (random_event(0.05)) {
        Alice.bank.balance -= Alice.car.fine_avg;
    }
}


void alice_car_repair()
{
    Alice.car.age_months++;
    if (!Alice.car.need_repair && Alice.car.age_months > 60) {
        if (random_event(0.02)) {
            Alice.car.need_repair = true;
            Alice.car.repair_cost = 30000 + rand() % 50000;
        }
    }
    if (Alice.car.need_repair && Alice.bank.balance >= Alice.car.repair_cost) {
        Alice.bank.balance -= Alice.car.repair_cost;
        Alice.car.need_repair = false;
    }
}


void bob_car_gas(int month)
{
    double inf = random_inflation(8.5, 9.3);
    Bob.bank.balance -= apply_monthly_inflation(Bob.car.gas_month, inf);
}


void bob_car_maintenance(int month)
{
    double inf = random_inflation(7.1, 8.2);
    Bob.bank.balance -= apply_monthly_inflation(Bob.car.maintenance_month, inf);
}


void bob_car_parking(int month)
{
    double inf = random_inflation(5.1, 6.3);
    Bob.bank.balance -= apply_monthly_inflation(Bob.car.parking_month, inf);
}


void bob_car_wash(int month)
{
    double inf = random_inflation(6.0, 7.0);
    Bob.bank.balance -= apply_monthly_inflation(Bob.car.washing_month, inf);
}


void bob_car_tolls(int month)
{
    double inf = random_inflation(5.0, 6.0);
    if (random_event(0.5)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.car.tolls_month, inf);
    }
}


void bob_car_insurance_tax(int month)
{
    double inf_ins = random_inflation(6.2, 7.4);
    double inf_tax = random_inflation(4.0, 5.5);
    if (month == 1) {
        Bob.bank.balance -= apply_yearly_inflation(Bob.car.insurance_year, inf_ins);
        Bob.bank.balance -= apply_yearly_inflation(Bob.car.tax_year, inf_tax);
    }
}


void bob_car_tires(int month)
{
    double inf = random_inflation(5.0, 6.0);
    if (month == 10) {
        Bob.bank.balance -= apply_yearly_inflation(Bob.car.tires_year, inf);
    }
}


void bob_car_diagnostics(int month)
{
    double inf = random_inflation(5.5, 6.5);
    if (month == 10) {
        Bob.bank.balance -= apply_yearly_inflation(Bob.car.diagnostics_year, inf);
    }
}


void bob_car_fines()
{
    if (random_event(0.03)) {
        Bob.bank.balance -= Bob.car.fine_avg;
    }
}


void bob_car_repair()
{
    Bob.car.age_months++;
    if (!Bob.car.need_repair && Bob.car.age_months > 72) {
        if (random_event(0.015)) {
            Bob.car.need_repair = true;
            Bob.car.repair_cost = 20000 + rand() % 50000;
        }
    }
    if (Bob.car.need_repair && Bob.bank.balance >= Bob.car.repair_cost) {
        Bob.bank.balance -= Bob.car.repair_cost;
        Bob.car.need_repair = false;
    }
}


void alice_transport()
{
    double inf = random_inflation(8.5, 9.7);
    Alice.bank.balance -= apply_monthly_inflation(Alice.transport.public_transport, inf);
    if (random_event(0.3)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.taxi, inf);
    }
    if (random_event(0.1)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.taxi_work, inf);
    }
    if (random_event(0.2)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.taxi_weekend, inf);
    }
    if (random_event(0.05)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.train, inf);
    }
    if (random_event(0.02)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.plane, inf);
    }
    if (random_event(0.03)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.bus, inf);
    }
    if (random_event(0.1)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.subway, inf);
    }
    if (random_event(0.05)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.bicycle, inf);
    }
    if (random_event(0.02)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.scooter, inf);
    }
    if (random_event(0.1)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.rideshare, inf);
    }
    if (random_event(0.05)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.transport.car_sharing, inf);
    }
}


void bob_transport()
{
    double inf = random_inflation(7.1, 8.3);
    Bob.bank.balance -= apply_monthly_inflation(Bob.transport.public_transport, inf);
    if (random_event(0.2)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.transport.taxi, inf);
    }
    if (random_event(0.05)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.transport.taxi_work, inf);
    }
    if (random_event(0.1)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.transport.taxi_weekend, inf);
    }
    if (random_event(0.02)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.transport.train, inf);
}
    if (random_event(0.01)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.transport.plane, inf);
    }
    if (random_event(0.05)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.transport.subway, inf);
    }
}


void alice_car(int month)
{
    alice_car_gas(month);
    alice_car_maintenance(month);
    alice_car_parking(month);
    alice_car_wash(month);
    alice_car_tolls(month);
    alice_car_insurance_tax(month);
    alice_car_tires(month);
    alice_car_diagnostics(month);
    alice_car_fines();
    alice_car_repair();
}


void bob_car(int month)
{
    bob_car_gas(month);
    bob_car_maintenance(month);
    bob_car_parking(month);
    bob_car_wash(month);
    bob_car_tolls(month);
    bob_car_insurance_tax(month);
    bob_car_tires(month);
    bob_car_diagnostics(month);
    bob_car_fines();
    bob_car_repair();
}


void alice_clothing()
{
    double inf = random_inflation(8.5, 9.7);
    if (random_event(0.2)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.clothes, inf);
    }
    if (random_event(0.15)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.shoes, inf);
    }
    if (random_event(0.1)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.accessories, inf);
    }
    if (random_event(0.1)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.dry_clean, inf);
    }
    if (random_event(0.05)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.repair, inf);
    }
    if (random_event(0.1)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.sportswear, inf);
    }
    if (random_event(0.2)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.underwear, inf);
    }
    if (random_event(0.1)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.socks, inf);
    }
    if (random_event(0.05)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.hats, inf);
    }
    if (random_event(0.02)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.gloves, inf);
    }
    if (random_event(0.03)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.bags, inf);
    }
    if (random_event(0.01)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.jewelry, inf);
    }
    if (random_event(0.02)) {
        Alice.bank.balance -= apply_monthly_inflation(Alice.clothing.watches, inf);
    }
}


void bob_clothing()
{
    double inf = random_inflation(7.1, 8.3);
    if (random_event(0.15)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.clothing.clothes, inf);
    }
    if (random_event(0.1)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.clothing.shoes, inf);
    }
    if (random_event(0.05)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.clothing.accessories, inf);
    }
    if (random_event(0.05)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.clothing.dry_clean, inf);
    }
    if (random_event(0.1)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.clothing.underwear, inf);
    }
    if (random_event(0.05)) {
        Bob.bank.balance -= apply_monthly_inflation(Bob.clothing.socks, inf);
    }
}


void simulation_alice()
{
    int year = 2026;
    int month = 9;
    while (not (year == 2027 and month ==9)) {
        alice_salary(month, year);
        alice_additional_income(month, year);
        alice_deposit();
        alice_loan_payments();
        alice_property(month);
        alice_food();
        alice_car(month);
        alice_transport();
        ++month;
        if (month == 13)
        {
            ++year;
            month = 1;
        }
    }
}


void simulation_bob()
{
    int year = 2026;
    int month = 9;
    while (not (year == 2027 and month ==9)) {
        bob_salary(month, year);
        bob_additional_income(month, year);
        bob_deposit();
        bob_loan_payments();
        bob_property(month);
        bob_car(month);
        bob_transport();
        ++month;
        if (month == 13)
        {
            month = 1;
            ++year;
        }
    }
}


void alice_init()
{
    Alice.name = "Alice Djokovitch";
    Alice.age = 28;
    Alice.married = false;
    Alice.has_children = false;
    Alice.children_expenses_month = 0;
    Alice.children_count = 0;
    Alice.birthday_month = 4;
    Alice.wedding_anniversary_month = 2;
    Alice.child_birth_month = 0;
    Alice.expecting_child = false;
    Alice.maternity_payment = 0;
    Alice.freelance_income_month = 5000;
    Alice.rental_income_month = 0;
    Alice.dividend_income_year = 2000;
    Alice.student_loan = 0;
    Alice.student_loan_month = 0;
    Alice.car.loan = 0;
    Alice.car.loan_month = 0;
    Alice.personal_loan = 0;
    Alice.personal_loan_month = 0;

    Alice.work.salary_month = 180'000;
    Alice.work.position = "Manager";
    Alice.work.experience_years = 5;
    Alice.work.has_remote = true;
    Alice.work.vacation_days_used = 0;
    Alice.work.vacation_days_total = 28;
    Alice.work.education_allowance_year = 50'000;
    Alice.work.transport_compensation_month = 3000;

    Alice.bank.balance = 60'000;
    Alice.bank.deposit = 0;
    Alice.bank.deposit_rate = 14.0;
    Alice.bank.credit_card_debt = 0;
    Alice.bank.credit_card_limit = 300'000;
    Alice.bank.credit_rate = 25.0;
    Alice.bank.investment = 0;
    Alice.bank.crypto = 0;
    Alice.bank.gold = 0;
    Alice.bank.bonds = 0;
    Alice.bank.pension = 20000;
    Alice.bank.life_insurance = 0;
    Alice.bank.debit_card_cashback = 500;

    Alice.car.value = 2'400'000;
    Alice.car.gas_month = 5000;
    Alice.car.maintenance_month = 3000;
    Alice.car.insurance_year = 96000;
    Alice.car.parking_month = 2000;
    Alice.car.tax_year = 15000;
    Alice.car.fine_avg = 500;
    Alice.car.age_months = 24;
    Alice.car.need_repair = false;
    Alice.car.washing_month = 1000;
    Alice.car.tires_year = 8000;
    Alice.car.diagnostics_year = 3000;
    Alice.car.tolls_month = 500;
    Alice.car.rental_income = 0;
    Alice.car.has_rental = false;

    Alice.cat.name = "Turbo";
    Alice.cat.color = "gray‑brown‑crimson";  //серо-буро-малиновый
    Alice.cat.age = 3;
    Alice.cat.food_month = 6000;
    Alice.cat.vet_month = 3000;
    Alice.cat.toys_month = 1000;
    Alice.cat.insurance_month = 2000;
    Alice.cat.sick = false;
    Alice.cat.grooming_month = 500;
    Alice.cat.treats_month = 300;
    Alice.cat.bedding_year = 2000;
    Alice.cat.carrier_one_time = 2500;
    Alice.cat.carrier_bought = false;

    Alice.property.rent_month = 40'000;
    Alice.property.utilities_month = 7000;
    Alice.property.internet_month = 1000;
    Alice.property.phone_month = 500;
    Alice.property.insurance_year = 12000;
    Alice.property.has_mortgage = false;
    Alice.property.mortgage_month = 0;
    Alice.property.mortgage_debt = 0;
    Alice.property.property_tax_year = 0;
    Alice.property.renovation_year = 0;
    Alice.property.furniture_year = 0;
    Alice.property.appliances_year = 0;
    Alice.property.owns_apartment = false;
    Alice.property.apartment_value = 0;

    Alice.food.groceries = 15000;
    Alice.food.eating_out = 5000;
    Alice.food.fast_food = 1500;
    Alice.food.delivery = 1000;
    Alice.food.coffee = 300;
    Alice.food.sweets = 500;
    Alice.food.fruits = 1000;
    Alice.food.vegetables = 800;
    Alice.food.meat = 2000;
    Alice.food.fish = 1500;
    Alice.food.cottage_cheese = 700;
    Alice.food.bread = 300;
    Alice.food.alcohol = 2000;
    Alice.food.water = 500;
    Alice.food.juices = 300;
    Alice.food.snacks = 400;
    Alice.food.frozen = 600;
    Alice.food.canned = 200;
    Alice.food.spices = 100;
    Alice.food.baby_food = 0;

    Alice.health.medicine = 5000;
    Alice.health.pharmacy = 1000;
    Alice.health.dentist = 5000;
    Alice.health.therapist = 1500;
    Alice.health.cardiologist = 2500;
    Alice.health.psychologist = 3000;
    Alice.health.massage = 2500;
    Alice.health.fitness = 5000;
    Alice.health.beauty = 3000;
    Alice.health.barber = 0;
    Alice.health.cosmetics = 2000;
    Alice.health.perfume = 3000;
    Alice.health.ophthalmologist = 2000;
    Alice.health.surgeon = 5000;
    Alice.health.hospital = 10'000;
    Alice.health.physiotherapy = 1500;
    Alice.health.vitamins = 1000;
    Alice.health.dietary_supplements = 500;
    Alice.health.medical_tests = 800;
    Alice.health.ambulance = 2000;

    Alice.clothing.clothes = 15000;
    Alice.clothing.shoes = 5000;
    Alice.clothing.accessories = 2000;
    Alice.clothing.dry_clean = 1000;
    Alice.clothing.repair = 500;
    Alice.clothing.sportswear = 2000;
    Alice.clothing.underwear = 1000;
    Alice.clothing.socks = 300;
    Alice.clothing.hats = 500;
    Alice.clothing.gloves = 300;
    Alice.clothing.bags = 2000;
    Alice.clothing.jewelry = 1000;
    Alice.clothing.watches = 1500;

    Alice.transport.public_transport = 3000;
    Alice.transport.taxi = 1000;
    Alice.transport.taxi_work = 1500;
    Alice.transport.taxi_weekend = 1000;
    Alice.transport.train = 3000;
    Alice.transport.plane = 20'000;
    Alice.transport.bus = 1000;
    Alice.transport.subway = 500;
    Alice.transport.bicycle = 200;
    Alice.transport.scooter = 300;
    Alice.transport.rideshare = 400;
    Alice.transport.car_sharing = 600;

    Alice.entertainment.travel = 80'000;
    Alice.entertainment.hotel = 5000;
    Alice.entertainment.hostel = 1500;
    Alice.entertainment.camping = 1000;
    Alice.entertainment.museum = 500;
    Alice.entertainment.theater = 1500;
    Alice.entertainment.concert = 2000;
    Alice.entertainment.cinema = 1000;
    Alice.entertainment.hobbies = 3000;
    Alice.entertainment.subscriptions = 2000;
    Alice.entertainment.sport = 4000;
    Alice.entertainment.souvenirs = 3000;
    Alice.entertainment.photos = 1000;
    Alice.entertainment.books = 1500;
    Alice.entertainment.music = 500;
    Alice.entertainment.games = 1000;
    Alice.entertainment.streaming = 800;
    Alice.entertainment.nightclub = 1500;
    Alice.entertainment.bar = 1000;
    Alice.entertainment.karaoke = 800;
    Alice.entertainment.bowling = 600;
    Alice.entertainment.billiard = 500;
    Alice.entertainment.quests = 1000;
    Alice.entertainment.amusement_park = 2000;
    Alice.entertainment.zoo = 400;
    Alice.entertainment.aquarium = 600;

    Alice.deposit_month = 40'000;
    Alice.emergency_fund = 50'000;
}


void bob_init()
{
    Bob.name = "Bob Bradenburgen";
    Bob.age = 32;
    Bob.married = false;
    Bob.has_children = false;
    Bob.birthday_month = 10;
    Bob.wedding_anniversary_month = 0;
    Bob.child_birth_month = 0;
    Bob.expecting_child = false;
    Bob.maternity_payment = 0;
    Bob.freelance_income_month = 2000;
    Bob.rental_income_month = 0;
    Bob.dividend_income_year = 1000;
    Bob.student_loan = 200'000;
    Bob.student_loan_month = 5000;
    Bob.car.loan = 300'000;
    Bob.car.loan_month = 10000;
    Bob.personal_loan = 0;
    Bob.personal_loan_month = 0;

    Bob.work.salary_month = 200'000;
    Bob.work.position = "Robotics engineer";
    Bob.work.experience_years = 7;
    Bob.work.has_remote = true;
    Bob.work.vacation_days_used = 0;
    Bob.work.vacation_days_total = 28;
    Bob.work.education_allowance_year = 30000;
    Bob.work.transport_compensation_month = 2000;

     Bob.bank.balance = 45000;
    Bob.bank.deposit = 10000;
    Bob.bank.deposit_rate = 13.0;
    Bob.bank.credit_card_debt = 15000;
    Bob.bank.credit_card_limit = 200000;
    Bob.bank.credit_rate = 27.0;
    Bob.bank.investment = 20000;
    Bob.bank.crypto = 5000;
    Bob.bank.gold = 0;
    Bob.bank.bonds = 0;
    Bob.bank.pension = 10000;
    Bob.bank.life_insurance = 0;
    Bob.bank.debit_card_cashback = 200;

    Bob.car.value = 1800000;
    Bob.car.gas_month = 4000;
    Bob.car.maintenance_month = 2000;
    Bob.car.insurance_year = 72000;
    Bob.car.parking_month = 1000;
    Bob.car.tax_year = 10000;
    Bob.car.fine_avg = 300;
    Bob.car.age_months = 36;
    Bob.car.need_repair = false;
    Bob.car.washing_month = 500;
    Bob.car.tires_year = 6000;
    Bob.car.diagnostics_year = 2000;
    Bob.car.tolls_month = 200;
    Bob.car.rental_income = 0;
    Bob.car.has_rental = false;

    Bob.cat.name = "Chairman Meow-ington III";
    Bob.cat.color = "static cling beige";
    Bob.cat.age = 5;
    Bob.cat.food_month = 4000;
    Bob.cat.vet_month = 2000;
    Bob.cat.toys_month = 500;
    Bob.cat.insurance_month = 1000;
    Bob.cat.sick = false;
    Bob.cat.grooming_month = 0;
    Bob.cat.treats_month = 200;
    Bob.cat.bedding_year = 1000;
    Bob.cat.carrier_one_time = 0;
    Bob.cat.carrier_bought = true;

    Bob.property.rent_month = 30'000;
    Bob.property.utilities_month = 5000;
    Bob.property.internet_month = 800;
    Bob.property.phone_month = 400;
    Bob.property.insurance_year = 9600;
    Bob.property.has_mortgage = true;
    Bob.property.mortgage_month = 25000;
    Bob.property.mortgage_debt = 1'500'000;
    Bob.property.property_tax_year = 12000;
    Bob.property.renovation_year = 0;
    Bob.property.furniture_year = 0;
    Bob.property.appliances_year = 0;
    Bob.property.owns_apartment = true;
    Bob.property.apartment_value = 5'000'000;

    Bob.food.groceries = 12000;
    Bob.food.eating_out = 3000;
    Bob.food.fast_food = 800;
    Bob.food.delivery = 500;
    Bob.food.coffee = 150;
    Bob.food.sweets = 200;
    Bob.food.fruits = 500;
    Bob.food.vegetables = 400;
    Bob.food.meat = 1000;
    Bob.food.fish = 800;
    Bob.food.cottage_cheese= 400;
    Bob.food.bread = 150;
    Bob.food.alcohol = 1500;
    Bob.food.water = 300;
    Bob.food.juices = 150;
    Bob.food.snacks = 200;
    Bob.food.frozen = 300;
    Bob.food.canned = 100;
    Bob.food.spices = 50;

    Bob.health.medicine = 3000;
    Bob.health.pharmacy = 500;
    Bob.health.dentist = 3000;
    Bob.health.therapist = 1000;
    Bob.health.cardiologist = 1500;
    Bob.health.psychologist = 0;
    Bob.health.massage = 0;
    Bob.health.fitness = 2000;
    Bob.health.beauty = 0;
    Bob.health.barber = 1000;
    Bob.health.cosmetics = 0;
    Bob.health.perfume = 0;
    Bob.health.ophthalmologist = 1000;
    Bob.health.surgeon = 0;
    Bob.health.hospital = 0;
    Bob.health.physiotherapy = 0;
    Bob.health.vitamins = 500;
    Bob.health.dietary_supplements = 200;
    Bob.health.medical_tests = 300;
    Bob.health.ambulance = 0;

    Bob.clothing.clothes = 10000;
    Bob.clothing.shoes = 3000;
    Bob.clothing.accessories = 1000;
    Bob.clothing.dry_clean = 500;
    Bob.clothing.repair = 300;
    Bob.clothing.sportswear = 1000;
    Bob.clothing.underwear = 500;
    Bob.clothing.socks = 200;
    Bob.clothing.hats = 300;
    Bob.clothing.gloves = 200;
    Bob.clothing.bags = 0;
    Bob.clothing.jewelry = 0;
    Bob.clothing.watches = 0;

    Bob.transport.public_transport = 2500;
    Bob.transport.taxi = 500;
    Bob.transport.taxi_work = 500;
    Bob.transport.taxi_weekend = 300;
    Bob.transport.train = 2000;
    Bob.transport.plane = 15000;
    Bob.transport.bus = 500;
    Bob.transport.subway = 300;
    Bob.transport.bicycle = 0;
    Bob.transport.scooter = 0;
    Bob.transport.rideshare = 0;
    Bob.transport.car_sharing = 0;

    Bob.entertainment.travel = 50'000;
    Bob.entertainment.hotel = 3000;
    Bob.entertainment.hostel = 1000;
    Bob.entertainment.camping = 600;
    Bob.entertainment.museum = 300;
    Bob.entertainment.theater = 800;
    Bob.entertainment.concert = 1200;
    Bob.entertainment.cinema = 500;
    Bob.entertainment.hobbies = 2000;
    Bob.entertainment.subscriptions = 1500;
    Bob.entertainment.sport = 2000;
    Bob.entertainment.souvenirs = 1500;
    Bob.entertainment.photos = 500;
    Bob.entertainment.books = 800;
    Bob.entertainment.music = 200;
    Bob.entertainment.games = 300;
    Bob.entertainment.streaming = 600;
    Bob.entertainment.nightclub = 0;
    Bob.entertainment.bar = 500;
    Bob.entertainment.karaoke = 0;
    Bob.entertainment.bowling = 0;
    Bob.entertainment.billiard = 0;
    Bob.entertainment.quests = 0;
    Bob.entertainment.amusement_park = 0;
    Bob.entertainment.zoo = 0;
    Bob.entertainment.aquarium = 0;

    Bob.deposit_month = 30'000;
    Bob.emergency_fund = 20'000;
}


void print_results(const Person &p)
{
    printf("\n======================= %s =======================\n", p.name.c_str());      //c_str() для str => const char*  
    printf("Age: %d\n", p.age);
    printf("Married: %s\n", p.married ? "Yes" : "No");    // ?: - тернарный оператор, if/else

    printf("Cat: %s, age %d, color %s\n", p.cat.name.c_str(), p.cat.age, p.cat.color.c_str());
    if (p.has_pet_dog) {
        printf("Has dog, expenses: %lld RUB/month\n", p.dog_expenses_month);
    }

    printf("Has children: %s\n", p.has_children ? "Yes" : "No");
    printf("Salary:                 %lld RUB\n", p.work.salary_month);
    printf("Bank balance:           %lld RUB\n", p.bank.balance);
    printf("Deposit:                %lld RUB\n", p.bank.deposit);
    printf("Investments:            %lld RUB\n", p.bank.investment);
    printf("Crypto:                 %lld RUB\n", p.bank.crypto);
    printf("Pension:                %lld RUB\n", p.bank.pension);
    printf("Credit card debt:       %lld RUB\n", p.bank.credit_card_debt);
    printf("Emergency fund:         %lld RUB\n", p.emergency_fund);

    RUB total = p.bank.balance + p.bank.deposit + p.bank.investment + p.bank.crypto + p.bank.pension + p.emergency_fund - p.bank.credit_card_debt;
     
    if (p.property.has_mortgage) {
        printf("Mortgage debt:          %lld RUB\n", p.property.mortgage_debt);
    }
    if (p.property.owns_apartment) {
        printf("Apartment value:        %lld RUB\n", p.property.apartment_value);
    }

}


RUB net_worth(const Person& p) {
    return p.bank.balance + p.bank.deposit + p.bank.investment + p.property.apartment_value + 
           p.bank.gold + p.bank.bonds + p.bank.pension + p.emergency_fund - 
           p.bank.credit_card_debt - p.student_loan - p.car.loan - 
           p.personal_loan - p.property.mortgage_debt;
}


void AB_testing() {
    RUB alice_net = net_worth(Alice);
    RUB bob_net = net_worth(Bob);
    
    printf("\n======================= A/B TESTING =======================\n");
    printf("Strategy A (Alice):     %lld RUB\n", alice_net);
    printf("Strategy B (Bob):       %lld RUB\n", bob_net);
    printf("Difference:             %lld RUB\n", bob_net - alice_net);
    
    if (alice_net > bob_net) {
        printf("\nWINNER: Strategy A (Alice)\n");
        printf("   (%lld RUB more)\n", alice_net - bob_net);
    } else {
        printf("\nWINNER: Strategy B (Bob) - ");
        printf("(%lld RUB more)\n", bob_net - alice_net);
    }
}


int main()
{
    alice_init();
    bob_init();
    
    simulation_alice();
    simulation_bob();

    print_results(Alice);
    print_results(Bob);

    AB_testing();
    return 0;
}
