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
    RUB last_net_salary;               // c НДФЛ
};

struct SecondWork
{
    bool has_second_job;
    RUB salary_month;
    string position;
    int hours_per_week;
    RUB last_net_salary;
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

struct Taxes
{
    RUB ytd_taxable_income;
    RUB ytd_tax_paid;
    RUB ytd_fitness_paid;
    RUB ytd_social_deduction;
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

struct HealthExpenses
{
    RUB medicine;
    RUB pharmacy;
    RUB dentist;                    
    RUB therapist;                  
    RUB cardiologist;               
    RUB psychologist;               
    RUB massage;                    
    RUB fitness;                   
    RUB barber;                     
    RUB beauty;                      
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
    RUB tires_year; 
    RUB diagnostics_year; 
    RUB tolls_month;             
    RUB rental_income; 
    bool has_rental;             
    RUB loan;                    
    RUB loan_month;  
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

struct Dog
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

struct Person
{
    string name;
    int age;
    bool married;
    bool has_children;             
    int children_count;
    Work work;
    SecondWork second_work; 
    bank bank;
    Taxes taxes;
    Car car;
    Property property;
    TransportExpenses transport;
    Dog dog;
    bool has_pet_cat;
    RUB cat_expenses_month;
    FoodExpenses food;
    ClothingExpenses clothing;
    EntertainmentExpenses entertainment;
    RUB children_expenses_month;
    HealthExpenses health;
    RUB deposit_month;                           
    RUB emergency_fund;                   
    int birthday_month;                         
    int wedding_anniversary_month;               
    int child_birth_month;                        
    bool expecting_child;    
    RUB student_loan;                              
    RUB student_loan_month;                      
    RUB personal_loan;                            
    RUB personal_loan_month;    
    RUB freelance_income_month;                   
    RUB rental_income_month;                       
    RUB dividend_income_year;
    RUB maternity_payment;                 
};

struct Person ulya;
struct Person vasya;


const RUB ndfl_limit1 = 2'400'000;
const RUB ndfl_limit2 = 5'000'000;
const RUB ndfl_limit3 = 20'000'000;
const RUB ndfl_limit4 = 50'000'000;

const Percent ndfl_rate1 = 13.0;
const Percent ndfl_rate2 = 15.0;
const Percent ndfl_rate3 = 18.0;
const Percent ndfl_rate4 = 20.0;
const Percent ndfl_rate5 = 22.0;

const Percent dividend_tax_rate = 13.0;
const Percent rental_tax_rate = 13.0;

RUB calculate_ndfl_year(RUB annual_income)       // ндфл
{
    RUB tax = 0;
    RUB remaining = annual_income;

    RUB part1 = (remaining < ndfl_limit1) ? remaining : ndfl_limit1-1;
    tax += (RUB)((double)part1 * ndfl_rate1 / 100);
    remaining -= part1;
    if (remaining == 0) {
        return tax;
    }

    RUB range2 = ndfl_limit2 - ndfl_limit1;
    RUB part2 = (remaining < range2) ? remaining : range2 - 1;
    tax += (RUB)((double)part2 * ndfl_rate2 / 100);
    remaining -= part2;
    if (remaining == 0) {
        return tax;
    }

    RUB range3 = ndfl_limit3 - ndfl_limit2;
    RUB part3 = (remaining < range3) ? remaining : range3-1;
    tax += (RUB)((double)part3 * ndfl_rate3 / 100);
    remaining -= part3;
    if (remaining == 0) {
        return tax;
    }


    RUB range4 = ndfl_limit4 - ndfl_limit3;
    RUB part4 = (remaining < range4) ? remaining : range4-1;
    tax += (RUB)((double)part4 * ndfl_rate4 / 100);
    remaining -= part4;
    if (remaining == 0) {
        return tax;
    }

    tax += (RUB)((double)remaining * ndfl_rate5 / 100);
    return tax;
}

const RUB taxes_go_back_limit = 120'000;                           //deduction_fitness
const Percent taxes_go_back_rate = 13.0;

RUB calculate_taxes_go_back(RUB expences) {
    RUB taxable_base = (expences < taxes_go_back_limit) ? expences : taxes_go_back_limit;
    return (RUB)((double)taxable_base * taxes_go_back_rate / 100.0);
}

void apply_social_deduction(Person &p, int month)
{
    if (month != 12) return;
    RUB deduction = calculate_taxes_go_back(p.taxes.ytd_fitness_paid);
    if (deduction > p.taxes.ytd_tax_paid) {
        deduction = p.taxes.ytd_tax_paid;
    }
    p.bank.balance += deduction;
    p.taxes.ytd_social_deduction = deduction;
    p.taxes.ytd_tax_paid -= deduction;
}


RUB calculate_go_back_year(Person &p, RUB annual_income)       // возврат
{
    RUB tax = 0;
    RUB remaining = annual_income - p.taxes.ytd_tax_paid;

    RUB part1 = (remaining < ndfl_limit1) ? remaining : ndfl_limit1-1;
    tax += (RUB)((double)part1 * ndfl_rate1 / 100);
    remaining -= part1;
    if (remaining == 0) {
        return tax;
    }

    RUB range2 = ndfl_limit2 - ndfl_limit1;
    RUB part2 = (remaining < range2) ? remaining : range2 - 1;
    tax += (RUB)((double)part2 * ndfl_rate2 / 100);
    remaining -= part2;
    if (remaining == 0) {
        return tax;
    }

    RUB range3 = ndfl_limit3 - ndfl_limit2;
    RUB part3 = (remaining < range3) ? remaining : range3-1;
    tax += (RUB)((double)part3 * ndfl_rate3 / 100);
    remaining -= part3;
    if (remaining == 0) {
        return tax;
    }

    RUB range4 = ndfl_limit4 - ndfl_limit3;
    RUB part4 = (remaining < range4) ? remaining : range4-1;
    tax += (RUB)((double)part4 * ndfl_rate4 / 100);
    remaining -= part4;
    if (remaining == 0) {
        return tax;
    }

    tax += (RUB)((double)remaining * ndfl_rate5 / 100);
    return tax;
}

RUB true_tax = calculate_ndfl_year(RUB annual_income) - calculate_go_back_year(RUB annual_income);


RUB apply_income_tax(Person &p, RUB gross_amount)        //для случаев когда сумма дохода становится > ndfl_limit
{
    p.taxes.ytd_taxable_income += gross_amount;
    const RUB tax_total = calculate_ndfl_year(p.taxes.ytd_taxable_income);
    const RUB tax_this_year = tax_total - p.taxes.ytd_tax_paid;
    p.taxes.ytd_tax_paid = tax_total;
    return gross_amount - tax_this_year;
}


RUB apply_apartment_tax(RUB gross_amount, Percent rate_percent)
{
    const RUB tax = (RUB)((double)gross_amount * rate_percent / 100.0);
    return gross_amount - tax;
}


void reset_yearly_taxes(Person &p)
{
    p.taxes.ytd_taxable_income = 0;
    p.taxes.ytd_tax_paid = 0;
}


bool random_event(double prob)
{
    return (rand() / (double)RAND_MAX) < prob;     //true с вер prob
}

void ulya_salary(const int month, const int year)
{
   
    if (month == 1) {
        if (year >= 2026 && year <= 2035) {
            double index = 1.0;
            if (year == 2026) index = 1.03;     
            if (year == 2027) index = 1.05;  
            if (year == 2028) index = 1.04; 
            if (year == 2029) index = 1.06; 
            if (year == 2030) index = 1.05;  
            if (year == 2031) index = 1.04; 
            if (year == 2032) index = 1.04;  
            if (year == 2033) index = 1.03;  
            if (year == 2034) index = 1.03;  
            if (year == 2035) index = 1.02;

            if (year == 2030 && month == 3) index = 2;
            
            ulya.work.salary_month = (RUB)(ulya.work.salary_month * index);
        }
    }
    
    if (year >= 2026 && year <= 2035) {
        if (month == 6) {
            double bonus_percent = 0.5;
            if (year == 2026) bonus_percent = 0.5;
            if (year == 2027) bonus_percent = 0.5;
            if (year == 2028) bonus_percent = 0.6;
            if (year == 2029) bonus_percent = 0.7;
            if (year >= 2030) bonus_percent = 0.6;
            
            ulya.bank.balance += apply_income_tax(ulya, (RUB)(ulya.work.salary_month * bonus_percent));
        }
        
        if (month == 12){
            double bonus_percent = 0.3;
            if (year == 2026) bonus_percent = 0.3;
            if (year == 2027) bonus_percent = 0.4;
            if (year == 2028) bonus_percent = 0.4;
            if (year == 2029) bonus_percent = 0.5;
            if (year >= 2030) bonus_percent = 0.35; 
            
            ulya.bank.balance += apply_income_tax(ulya, (RUB)(ulya.work.salary_month * bonus_percent));
        }
    }
    const RUB net_salary = apply_income_tax(ulya, ulya.work.salary_month);
    ulya.bank.balance += net_salary;
    ulya.work.last_net_salary = net_salary;
}

void vasya_salary(const int month, const int year)
{
    if (month == 1) {
        if (year >= 2026 && year <= 2035) {
            double index = 1.0;
            if (year == 2026) index = 1.02;    
            if (year == 2027) index = 1.1; 
            if (year == 2028) index = 1.08;
            if (year == 2029) index = 1.04; 
            if (year == 2030) index = 1.2;  
            if (year == 2031) index = 1.02; 
            if (year == 2032) index = 1.02;  
            if (year == 2033) index = 1.02;  
            if (year == 2034) index = 1.1;  
            if (year == 2035) index = 1.2;  
            
            vasya.work.salary_month = (RUB)(vasya.work.salary_month * index);
        }
    }
    if (year >= 2026 && year <= 2035) {
   
        if (month == 9) {
            double bonus_percent = 0.0;
            if (year == 2026) bonus_percent = 0.0;      
            if (year == 2027) bonus_percent = 0.2;  
            if (year == 2028) bonus_percent = 0.25; 
            if (year == 2029) bonus_percent = 0.3;  
            if (year >= 2030) bonus_percent = 0.25; 
            
            vasya.bank.balance += apply_income_tax(vasya, (RUB)(vasya.work.salary_month * bonus_percent));
        }
        
        if (month == 3) {
            double bonus_percent = 0.0;
            if (year == 2026) bonus_percent = 0.0;     
            if (year == 2027) bonus_percent = 0.3;  
            if (year == 2028) bonus_percent = 0.35; 
            if (year == 2029) bonus_percent = 0.4;  
            if (year >= 2030) bonus_percent = 0.3;  
            
            vasya.bank.balance += apply_income_tax(vasya, (RUB)(vasya.work.salary_month * bonus_percent));
        }
    }
    const RUB net_salary = apply_income_tax(vasya, vasya.work.salary_month);
    vasya.bank.balance += net_salary;
    vasya.work.last_net_salary = net_salary;
}

void ulya_second_job()
{
    if (!ulya.second_work.has_second_job) {
        return;
    }
    const RUB net_salary = apply_income_tax(ulya, vasya.second_work.salary_month);
    ulya.bank.balance += net_salary;
    ulya.second_work.last_net_salary = net_salary;
}

void vasya_second_job()
{
    if (!vasya.second_work.has_second_job) {
        return;
    }
    const RUB net_salary = apply_income_tax(vasya, vasya.second_work.salary_month);
    vasya.bank.balance += net_salary;
    vasya.second_work.last_net_salary = net_salary;
}

double random_inflation(double min_percent, double max_percent)
{
    return min_percent + (double)rand() / RAND_MAX * (max_percent - min_percent);

}

void ulya_deposit()
{
    if (ulya.bank.balance >= ulya.deposit_month) {
        ulya.bank.deposit += ulya.deposit_month;
        ulya.bank.balance -= ulya.deposit_month;
    }
    else {
        ulya.bank.deposit += ulya.bank.balance;
        ulya.bank.balance = 0;
    }
    ulya.bank.deposit += ulya.bank.deposit * (ulya.bank.deposit_rate / 12 / 100);
    if (ulya.bank.credit_card_debt > 0) {
        ulya.bank.credit_card_debt += ulya.bank.credit_card_debt * (ulya.bank.credit_rate / 12 / 100);
    }
    if (ulya.bank.investment > 0) {
        double change = random_inflation(-5, 8);
        ulya.bank.investment += ulya.bank.investment * (change / 100);
    }
    if (ulya.bank.crypto > 0) {
        double change = random_inflation(-15, 20);
        ulya.bank.crypto += ulya.bank.crypto * (change / 100);
    }
    if (ulya.bank.gold > 0){
        double change = random_inflation(-2, 5);
        ulya.bank.gold += ulya.bank.gold * (change / 100);
    }
    if (ulya.bank.bonds > 0) {
        double change = random_inflation(1, 3);
        ulya.bank.bonds += ulya.bank.bonds * (change / 100);
    }
    if (ulya.bank.pension > 0) {
        ulya.bank.pension += ulya.bank.pension * (0.5 / 12 / 100);
    }
    ulya.bank.balance += ulya.bank.debit_card_cashback;
}

void vasya_deposit()
{
    RUB deposit_amt = vasya.deposit_month * 0.8;      //amount
    if (vasya.bank.balance >= deposit_amt) {
        vasya.bank.deposit += deposit_amt;
        vasya.bank.balance -= deposit_amt;
    }
    else {
        vasya.bank.deposit += vasya.bank.balance;
        vasya.bank.balance = 0;
    }
    vasya.bank.deposit += vasya.bank.deposit * (vasya.bank.deposit_rate / 12 / 100);
    if (vasya.bank.credit_card_debt > 0) {
        vasya.bank.credit_card_debt += vasya.bank.credit_card_debt * (vasya.bank.credit_rate / 12 / 100);
    }
    if (vasya.bank.investment > 0) {
        double change = random_inflation(-4, 6);
        vasya.bank.investment += vasya.bank.investment * (change / 100);
    }
    if (vasya.bank.crypto > 0) {
        double change = random_inflation(-10, 15);
        vasya.bank.crypto += vasya.bank.crypto * (change / 100);
    }
    if (vasya.bank.gold > 0) {
        double change = random_inflation(-1, 4);
        vasya.bank.gold += vasya.bank.gold * (change / 100);
    }
    vasya.bank.balance += vasya.bank.debit_card_cashback;
}

void ulya_loan_payments()
{ 
    if (ulya.student_loan > 0) {
        ulya.bank.balance -= ulya.student_loan_month;
        ulya.student_loan -= ulya.student_loan_month;
    }
    if (ulya.car.loan > 0) {
        ulya.bank.balance -= ulya.car.loan_month;
        ulya.car.loan -= ulya.car.loan_month;
    }
    if (ulya.personal_loan > 0) {
        ulya.bank.balance -= ulya.personal_loan_month;
        ulya.personal_loan -= ulya.personal_loan_month;
    }
}


void vasya_loan_payments()
{
     if (vasya.student_loan > 0)  {
        vasya.bank.balance -= vasya.student_loan_month;
        vasya.student_loan -= vasya.student_loan_month;
    }
    if (vasya.car.loan > 0) {
        vasya.bank.balance -= vasya.car.loan_month;
        vasya.car.loan -= vasya.car.loan_month;
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

void ulya_property(int month)
{
    double inf_rent = random_inflation(8.5, 9.7);
    double inf_util = random_inflation(7.2, 8.9);
    ulya.bank.balance -= apply_monthly_inflation(ulya.property.rent_month, inf_rent);
    ulya.bank.balance -= apply_monthly_inflation(ulya.property.utilities_month, inf_util);
    ulya.bank.balance -= apply_monthly_inflation(ulya.property.internet_month, inf_util);
    ulya.bank.balance -= apply_monthly_inflation(ulya.property.phone_month, inf_util);
    if (month == 1) {
        ulya.bank.balance -= apply_yearly_inflation(ulya.property.insurance_year, inf_util);
        ulya.bank.balance -= apply_yearly_inflation(ulya.property.property_tax_year, inf_util);
    }
    if (month == 6) {
        ulya.bank.balance -= apply_yearly_inflation(ulya.property.renovation_year, inf_util);
    }
    if (month == 9) {
        ulya.bank.balance -= apply_yearly_inflation(ulya.property.furniture_year, inf_util);
        ulya.bank.balance -= apply_yearly_inflation(ulya.property.appliances_year, inf_util);
    }
    if (ulya.property.has_mortgage) {
        ulya.bank.balance -= ulya.property.mortgage_month;
        ulya.property.mortgage_debt -= ulya.property.mortgage_month * 0.3;
    }
}

void vasya_property(int month)
{
    double inf_rent = random_inflation(7.1, 8.3);
    double inf_util = random_inflation(6.2, 7.4);
    vasya.bank.balance -= apply_monthly_inflation(vasya.property.rent_month, inf_rent);
    vasya.bank.balance -= apply_monthly_inflation(vasya.property.utilities_month, inf_util);
    vasya.bank.balance -= apply_monthly_inflation(vasya.property.internet_month, inf_util);
    vasya.bank.balance -= apply_monthly_inflation(vasya.property.phone_month, inf_util);
    if (month == 1) {
        vasya.bank.balance -= apply_yearly_inflation(vasya.property.insurance_year, inf_util);
        vasya.bank.balance -= apply_yearly_inflation(vasya.property.property_tax_year, inf_util);
    }
    if (vasya.property.has_mortgage) {
        vasya.bank.balance -= vasya.property.mortgage_month;
        vasya.property.mortgage_debt -= vasya.property.mortgage_month * 0.25;
    }
}

void vasya_food()
{
    double inf = random_inflation(9.2, 10.1);

    vasya.food.groceries = apply_monthly_inflation(vasya.food.groceries, inf);
    vasya.food.eating_out = apply_monthly_inflation(vasya.food.eating_out, inf);
    vasya.food.fast_food = apply_monthly_inflation(vasya.food.fast_food, inf);
    vasya.food.delivery = apply_monthly_inflation(vasya.food.delivery, inf);
    vasya.food.coffee = apply_monthly_inflation(vasya.food.coffee, inf);
    vasya.food.sweets = apply_monthly_inflation(vasya.food.sweets, inf);
    vasya.food.fruits = apply_monthly_inflation(vasya.food.fruits, inf);
    vasya.food.vegetables = apply_monthly_inflation(vasya.food.vegetables, inf);
    vasya.food.meat = apply_monthly_inflation(vasya.food.meat, inf);
    vasya.food.fish = apply_monthly_inflation(vasya.food.fish, inf);
    vasya.food.cottage_cheese = apply_monthly_inflation(vasya.food.cottage_cheese, inf);
    vasya.food.bread = apply_monthly_inflation(vasya.food.bread, inf);
    vasya.food.alcohol = apply_monthly_inflation(vasya.food.alcohol, inf);
    vasya.food.water = apply_monthly_inflation(vasya.food.water, inf);
    vasya.food.juices = apply_monthly_inflation(vasya.food.juices, inf);
    vasya.food.snacks = apply_monthly_inflation(vasya.food.snacks, inf);
    vasya.food.frozen = apply_monthly_inflation(vasya.food.frozen, inf);
    vasya.food.canned = apply_monthly_inflation(vasya.food.canned, inf);
    vasya.food.spices = apply_monthly_inflation(vasya.food.spices, inf);

    RUB total = vasya.food.groceries + vasya.food.eating_out + vasya.food.fast_food + vasya.food.delivery +
                vasya.food.coffee + vasya.food.sweets + vasya.food.fruits + vasya.food.vegetables +
                vasya.food.meat + vasya.food.fish + vasya.food.cottage_cheese + vasya.food.bread + vasya.food.alcohol +
                vasya.food.water + vasya.food.juices + vasya.food.snacks + vasya.food.frozen +
                vasya.food.canned + vasya.food.spices;
    vasya.bank.balance -= total;
}

void ulya_food()
{
    double inf = random_inflation(10.5, 11.3);

    ulya.food.groceries = apply_monthly_inflation(ulya.food.groceries, inf);
    ulya.food.eating_out = apply_monthly_inflation(ulya.food.eating_out, inf);
    ulya.food.fast_food = apply_monthly_inflation(ulya.food.fast_food, inf);
    ulya.food.delivery = apply_monthly_inflation(ulya.food.delivery, inf);
    ulya.food.coffee = apply_monthly_inflation(ulya.food.coffee, inf);
    ulya.food.sweets = apply_monthly_inflation(ulya.food.sweets, inf);
    ulya.food.fruits = apply_monthly_inflation(ulya.food.fruits, inf);
    ulya.food.vegetables = apply_monthly_inflation(ulya.food.vegetables, inf);
    ulya.food.meat = apply_monthly_inflation(ulya.food.meat, inf);
    ulya.food.fish = apply_monthly_inflation(ulya.food.fish, inf);
    ulya.food.cottage_cheese = apply_monthly_inflation(ulya.food.cottage_cheese, inf);
    ulya.food.bread = apply_monthly_inflation(ulya.food.bread, inf);
    ulya.food.alcohol = apply_monthly_inflation(ulya.food.alcohol, inf);
    ulya.food.water = apply_monthly_inflation(ulya.food.water, inf);
    ulya.food.juices = apply_monthly_inflation(ulya.food.juices, inf);
    ulya.food.snacks = apply_monthly_inflation(ulya.food.snacks, inf);
    ulya.food.frozen = apply_monthly_inflation(ulya.food.frozen, inf);
    ulya.food.canned = apply_monthly_inflation(ulya.food.canned, inf);
    ulya.food.spices = apply_monthly_inflation(ulya.food.spices, inf);
    ulya.food.baby_food = apply_monthly_inflation(ulya.food.baby_food, inf);

    RUB total = ulya.food.groceries + ulya.food.eating_out + ulya.food.fast_food + ulya.food.delivery +
                ulya.food.coffee + ulya.food.sweets + ulya.food.fruits + ulya.food.vegetables +
                ulya.food.meat + ulya.food.fish + ulya.food.cottage_cheese + ulya.food.bread + ulya.food.alcohol +
                ulya.food.water + ulya.food.juices + ulya.food.snacks + ulya.food.frozen +
                ulya.food.canned + ulya.food.spices + ulya.food.baby_food;
    ulya.bank.balance -= total;
}

void ulya_dog(int month)
{
    double inf_food = random_inflation(10.5, 11.3);
    double inf_vet = random_inflation(11.5, 12.2);
    double inf_toys = random_inflation(8.5, 9.7);
    double inf_ins = random_inflation(7.2, 8.9);
    double inf_groom = random_inflation(9.0, 10.0);
    ulya.bank.balance -= apply_monthly_inflation(ulya.dog.food_month, inf_food);
    ulya.bank.balance -= apply_monthly_inflation(ulya.dog.vet_month, inf_vet);
    ulya.bank.balance -= apply_monthly_inflation(ulya.dog.toys_month, inf_toys);
    ulya.bank.balance -= apply_monthly_inflation(ulya.dog.insurance_month, inf_ins);
    ulya.bank.balance -= apply_monthly_inflation(ulya.dog.grooming_month, inf_groom);
    ulya.bank.balance -= apply_monthly_inflation(ulya.dog.treats_month, inf_food);
    if (!ulya.dog.carrier_bought) {
        ulya.bank.balance -= ulya.dog.carrier_one_time;
        ulya.dog.carrier_bought = true;
    }
    if (month == 12) {
        ulya.bank.balance -= apply_yearly_inflation(ulya.dog.bedding_year, inf_ins);
    }
    if (ulya.dog.sick) {
        ulya.dog.sick_days--;
        ulya.bank.balance -= apply_monthly_inflation(ulya.dog.vet_month * 2, inf_vet);
        if (ulya.dog.sick_days <= 0) {
            ulya.dog.sick = false;
        }
    }
    else {
        if (random_event(0.02)) {
            ulya.dog.sick = true;
            ulya.dog.sick_days = 1 + rand() % 3;
        }
    }
}


void vasya_dog()
{
    double inf_food = random_inflation(9.8, 10.9);
    double inf_vet = random_inflation(10.2, 11.7);
    double inf_toys = random_inflation(7.5, 8.8);
    double inf_ins = random_inflation(6.3, 7.9);
    vasya.bank.balance -= apply_monthly_inflation(vasya.dog.food_month, inf_food);
    vasya.bank.balance -= apply_monthly_inflation(vasya.dog.vet_month, inf_vet);
    vasya.bank.balance -= apply_monthly_inflation(vasya.dog.toys_month, inf_toys);
    vasya.bank.balance -= apply_monthly_inflation(vasya.dog.insurance_month, inf_ins);
    if (vasya.dog.sick) {
        vasya.dog.sick_days--;
        vasya.bank.balance -= apply_monthly_inflation(vasya.dog.vet_month * 1.5, inf_vet);
        if (vasya.dog.sick_days <= 0) {
            vasya.dog.sick = false;
        }
    }
    else {
        if (random_event(0.015)) {
            vasya.dog.sick = true;
            vasya.dog.sick_days = 1 + rand() % 2;
        }
    }
}

void ulya_cat()
{
    if (ulya.has_pet_cat)
    {
        double inf = random_inflation(9.0, 10.5);
        ulya.bank.balance -= apply_monthly_inflation(ulya.cat_expenses_month, inf);
    }
}

void ulya_car_gas(int const month)
{
    double inf = random_inflation(9.8, 10.6);
    ulya.bank.balance -= apply_monthly_inflation(ulya.car.gas_month, inf);
}


void ulya_car_maintenance(int const month)
{
    double inf = random_inflation(8.2, 9.4);
    ulya.bank.balance -= apply_monthly_inflation(ulya.car.maintenance_month, inf);
}


void ulya_car_parking(int const month)
{
    double inf = random_inflation(6.8, 7.7);
    ulya.bank.balance -= apply_monthly_inflation(ulya.car.parking_month, inf);
}


void ulya_car_wash(int const month)
{
    double inf = random_inflation(7.0, 8.0);
    ulya.bank.balance -= apply_monthly_inflation(ulya.car.washing_month, inf);
}


void ulya_car_tolls(int const month)
{
    double inf = random_inflation(5.0, 6.0);
    if (random_event(0.5)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.car.tolls_month, inf);
    }
}


void ulya_car_insurance_tax(int const month)
{
    double inf_ins = random_inflation(7.5, 8.9);
    double inf_tax = random_inflation(5.2, 6.8);
    if (month == 1) {
        ulya.bank.balance -= apply_yearly_inflation(ulya.car.insurance_year, inf_ins);
        ulya.bank.balance -= apply_yearly_inflation(ulya.car.tax_year, inf_tax);
    }
}


void ulya_car_tires(int const month)
{
    double inf = random_inflation(6.0, 7.0);
    if (month == 10) {
        ulya.bank.balance -= apply_yearly_inflation(ulya.car.tires_year, inf);
    }
}


void ulya_car_diagnostics(int const month)
{
    double inf = random_inflation(5.5, 6.5);
    if (month == 4) {
        ulya.bank.balance -= apply_yearly_inflation(ulya.car.diagnostics_year, inf);
    }
}


void ulya_car_fines()
{
    if (random_event(0.05)) {
        ulya.bank.balance -= ulya.car.fine_avg;
    }
}


void ulya_car_repair()
{
    ulya.car.age_months++;
    if (!ulya.car.need_repair && ulya.car.age_months > 60) {
        if (random_event(0.02)) {
            ulya.car.need_repair = true;
            ulya.car.repair_cost = 30000 + rand() % 50000;
        }
    }
    if (ulya.car.need_repair && ulya.bank.balance >= ulya.car.repair_cost) {
        ulya.bank.balance -= ulya.car.repair_cost;
        ulya.car.need_repair = false;
    }
}


void vasya_car_gas(int const month)
{
    double inf = random_inflation(8.5, 9.3);
    vasya.bank.balance -= apply_monthly_inflation(vasya.car.gas_month, inf);
}


void vasya_car_maintenance(int const month)
{
    double inf = random_inflation(7.1, 8.2);
    vasya.bank.balance -= apply_monthly_inflation(vasya.car.maintenance_month, inf);
}


void vasya_car_parking(int const month)
{
    double inf = random_inflation(5.1, 6.3);
    vasya.bank.balance -= apply_monthly_inflation(vasya.car.parking_month, inf);
}


void vasya_car_wash(int const month)
{
    double inf = random_inflation(6.0, 7.0);
    vasya.bank.balance -= apply_monthly_inflation(vasya.car.washing_month, inf);
}


void vasya_car_tolls(int const month)
{
    double inf = random_inflation(5.0, 6.0);
    if (random_event(0.5)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.car.tolls_month, inf);
    }
}


void vasya_car_insurance_tax(int const month)
{
    double inf_ins = random_inflation(6.2, 7.4);
    double inf_tax = random_inflation(4.0, 5.5);
    if (month == 1) {
        vasya.bank.balance -= apply_yearly_inflation(vasya.car.insurance_year, inf_ins);
        vasya.bank.balance -= apply_yearly_inflation(vasya.car.tax_year, inf_tax);
    }
}


void vasya_car_tires(int const month)
{
    double inf = random_inflation(5.0, 6.0);
    if (month == 10) {
        vasya.bank.balance -= apply_yearly_inflation(vasya.car.tires_year, inf);
    }
}


void vasya_car_diagnostics(int const month)
{
    double inf = random_inflation(5.5, 6.5);
    if (month == 10) {
        vasya.bank.balance -= apply_yearly_inflation(vasya.car.diagnostics_year, inf);
    }
}


void vasya_car_fines()
{
    if (random_event(0.03)) {
        vasya.bank.balance -= vasya.car.fine_avg;
    }
}


void vasya_car_repair()
{
    vasya.car.age_months++;
    if (!vasya.car.need_repair && vasya.car.age_months > 72) {
        if (random_event(0.015)) {
            vasya.car.need_repair = true;
            vasya.car.repair_cost = 20000 + rand() % 50000;
        }
    }
    if (vasya.car.need_repair && vasya.bank.balance >= vasya.car.repair_cost) {
        vasya.bank.balance -= vasya.car.repair_cost;
        vasya.car.need_repair = false;
    }
}


void ulya_car(int month)
{
    ulya_car_gas(month);
    ulya_car_maintenance(month);
    ulya_car_parking(month);
    ulya_car_wash(month);
    ulya_car_tolls(month);
    ulya_car_insurance_tax(month);
    ulya_car_tires(month);
    ulya_car_diagnostics(month);
    ulya_car_fines();
    ulya_car_repair();
}


void vasya_car(int month)
{
    vasya_car_gas(month);
    vasya_car_maintenance(month);
    vasya_car_parking(month);
    vasya_car_wash(month);
    vasya_car_tolls(month);
    vasya_car_insurance_tax(month);
    vasya_car_tires(month);
    vasya_car_diagnostics(month);
    vasya_car_fines();
    vasya_car_repair();
}

void ulya_health()
{
    double inf = random_inflation(7.5, 9.0);

    ulya.health.medicine = apply_monthly_inflation(ulya.health.medicine, inf);
    ulya.health.pharmacy = apply_monthly_inflation(ulya.health.pharmacy, inf);
    ulya.health.vitamins = apply_monthly_inflation(ulya.health.vitamins, inf);
    ulya.health.dietary_supplements = apply_monthly_inflation(ulya.health.dietary_supplements, inf);
    ulya.bank.balance -= ulya.health.medicine + ulya.health.pharmacy +
                           ulya.health.vitamins + ulya.health.dietary_supplements;

    if (random_event(0.2)) {
        ulya.health.fitness = apply_monthly_inflation(ulya.health.fitness, inf);
        ulya.bank.balance -= ulya.health.fitness;
    }
    if (random_event(0.1)) {
        ulya.health.dentist = apply_monthly_inflation(ulya.health.dentist, inf);
        ulya.bank.balance -= ulya.health.dentist;
    }
    if (random_event(0.1)) {
        ulya.health.massage = apply_monthly_inflation(ulya.health.massage, inf);
        ulya.bank.balance -= ulya.health.massage;
    }
    if (random_event(0.15)) {
        ulya.health.beauty = apply_monthly_inflation(ulya.health.beauty, inf);
        ulya.bank.balance -= ulya.health.beauty;
    }
    if (random_event(0.05)) {
        ulya.health.cosmetics = apply_monthly_inflation(ulya.health.cosmetics, inf);
        ulya.bank.balance -= ulya.health.cosmetics;
    }
    if (random_event(0.03)) {
        ulya.health.therapist = apply_monthly_inflation(ulya.health.therapist, inf);
        ulya.bank.balance -= ulya.health.therapist;
    }
    if (random_event(0.02)) {
        ulya.health.cardiologist = apply_monthly_inflation(ulya.health.cardiologist, inf);
        ulya.bank.balance -= ulya.health.cardiologist;
    }
    if (random_event(0.02)) {
        ulya.health.psychologist = apply_monthly_inflation(ulya.health.psychologist, inf);
        ulya.bank.balance -= ulya.health.psychologist;
    }
    if (random_event(0.02)) {
        ulya.health.ophthalmologist = apply_monthly_inflation(ulya.health.ophthalmologist, inf);
        ulya.bank.balance -= ulya.health.ophthalmologist;
    }
    if (random_event(0.01)) {
        ulya.health.surgeon = apply_monthly_inflation(ulya.health.surgeon, inf);
        ulya.bank.balance -= ulya.health.surgeon;
    }
    if (random_event(0.01)) {
        ulya.health.hospital = apply_monthly_inflation(ulya.health.hospital, inf);
        ulya.bank.balance -= ulya.health.hospital;
    }
    if (random_event(0.03)) {
        ulya.health.physiotherapy = apply_monthly_inflation(ulya.health.physiotherapy, inf);
        ulya.bank.balance -= ulya.health.physiotherapy;
    }
    if (random_event(0.03)) {
        ulya.health.medical_tests = apply_monthly_inflation(ulya.health.medical_tests, inf);
        ulya.bank.balance -= ulya.health.medical_tests;
    }
    if (random_event(0.005)) {
        ulya.health.ambulance = apply_monthly_inflation(ulya.health.ambulance, inf);
        ulya.bank.balance -= ulya.health.ambulance;
    }
    if (random_event(0.1)) {
        ulya.health.barber = apply_monthly_inflation(ulya.health.barber, inf);
        ulya.bank.balance -= ulya.health.barber;
    }
    if (random_event(0.02)) {
        ulya.health.perfume = apply_monthly_inflation(ulya.health.perfume, inf);
        ulya.bank.balance -= ulya.health.perfume;
    }
}


void vasya_health()
{
    double inf = random_inflation(6.5, 8.0);

    vasya.health.medicine = apply_monthly_inflation(vasya.health.medicine, inf);
    vasya.health.pharmacy = apply_monthly_inflation(vasya.health.pharmacy, inf);
    vasya.health.vitamins = apply_monthly_inflation(vasya.health.vitamins, inf);
    vasya.health.dietary_supplements = apply_monthly_inflation(vasya.health.dietary_supplements, inf);
    vasya.bank.balance -= vasya.health.medicine + vasya.health.pharmacy +
                         vasya.health.vitamins + vasya.health.dietary_supplements;

    if (random_event(0.15)) {
        vasya.health.fitness = apply_monthly_inflation(vasya.health.fitness, inf);
        vasya.bank.balance -= vasya.health.fitness;
    }
    if (random_event(0.08)) {
        vasya.health.dentist = apply_monthly_inflation(vasya.health.dentist, inf);
        vasya.bank.balance -= vasya.health.dentist;
    }
    if (random_event(0.05)) {
        vasya.health.massage = apply_monthly_inflation(vasya.health.massage, inf);
        vasya.bank.balance -= vasya.health.massage;
    }
    if (random_event(0.05)) {
        vasya.health.beauty = apply_monthly_inflation(vasya.health.beauty, inf);
        vasya.bank.balance -= vasya.health.beauty;
    }
    if (random_event(0.03)) {
        vasya.health.cosmetics = apply_monthly_inflation(vasya.health.cosmetics, inf);
        vasya.bank.balance -= vasya.health.cosmetics;
    }
    if (random_event(0.02)) {
        vasya.health.therapist = apply_monthly_inflation(vasya.health.therapist, inf);
        vasya.bank.balance -= vasya.health.therapist;
    }
    if (random_event(0.02)) {
        vasya.health.cardiologist = apply_monthly_inflation(vasya.health.cardiologist, inf);
        vasya.bank.balance -= vasya.health.cardiologist;
    }
    if (random_event(0.01)) {
        vasya.health.psychologist = apply_monthly_inflation(vasya.health.psychologist, inf);
        vasya.bank.balance -= vasya.health.psychologist;
    }
    if (random_event(0.02)) {
        vasya.health.ophthalmologist = apply_monthly_inflation(vasya.health.ophthalmologist, inf);
        vasya.bank.balance -= vasya.health.ophthalmologist;
    }
    if (random_event(0.01)) {
        vasya.health.surgeon = apply_monthly_inflation(vasya.health.surgeon, inf);
        vasya.bank.balance -= vasya.health.surgeon;
    }
    if (random_event(0.01)) {
        vasya.health.hospital = apply_monthly_inflation(vasya.health.hospital, inf);
        vasya.bank.balance -= vasya.health.hospital;
    }
    if (random_event(0.02)) {
        vasya.health.physiotherapy = apply_monthly_inflation(vasya.health.physiotherapy, inf);
        vasya.bank.balance -= vasya.health.physiotherapy;
    }
    if (random_event(0.03)) {
        vasya.health.medical_tests = apply_monthly_inflation(vasya.health.medical_tests, inf);
        vasya.bank.balance -= vasya.health.medical_tests;
    }
    if (random_event(0.005)) {
        vasya.health.ambulance = apply_monthly_inflation(vasya.health.ambulance, inf);
        vasya.bank.balance -= vasya.health.ambulance;
    }
    if (random_event(0.15)) {
        vasya.health.barber = apply_monthly_inflation(vasya.health.barber, inf);
        vasya.bank.balance -= vasya.health.barber;
    }
    if (random_event(0.01)) {
        vasya.health.perfume = apply_monthly_inflation(vasya.health.perfume, inf);
        vasya.bank.balance -= vasya.health.perfume;
    }
}

void vasya_transport()
{
    double inf = random_inflation(8.5, 9.7);
    vasya.bank.balance -= apply_monthly_inflation(vasya.transport.public_transport, inf);
    if (random_event(0.3)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.taxi, inf);
    }
    if (random_event(0.1)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.taxi_work, inf);
    }
    if (random_event(0.2)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.taxi_weekend, inf);
    }
    if (random_event(0.05)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.train, inf);
    }
    if (random_event(0.02)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.plane, inf);
    }
    if (random_event(0.03)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.bus, inf);
    }
    if (random_event(0.1)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.subway, inf);
    }
    if (random_event(0.05)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.bicycle, inf);
    }
    if (random_event(0.02)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.scooter, inf);
    }
    if (random_event(0.1)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.rideshare, inf);
    }
    if (random_event(0.05)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.transport.car_sharing, inf);
    }
}


void ulya_transport()
{
    double inf = random_inflation(7.1, 8.3);
    ulya.bank.balance -= apply_monthly_inflation(ulya.transport.public_transport, inf);
    if (random_event(0.2)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.transport.taxi, inf);
    }
    if (random_event(0.05)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.transport.taxi_work, inf);
    }
    if (random_event(0.1)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.transport.taxi_weekend, inf);
    }
    if (random_event(0.02)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.transport.train, inf);
}
    if (random_event(0.01)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.transport.plane, inf);
    }
    if (random_event(0.05)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.transport.subway, inf);
    }
}

void ulya_clothing()
{
    double inf = random_inflation(8.5, 9.7);
    if (random_event(0.2)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.clothes, inf);
    }
    if (random_event(0.15)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.shoes, inf);
    }
    if (random_event(0.1)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.accessories, inf);
    }
    if (random_event(0.1)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.dry_clean, inf);
    }
    if (random_event(0.05)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.repair, inf);
    }
    if (random_event(0.1)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.sportswear, inf);
    }
    if (random_event(0.2)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.underwear, inf);
    }
    if (random_event(0.1)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.socks, inf);
    }
    if (random_event(0.05)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.hats, inf);
    }
    if (random_event(0.02)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.gloves, inf);
    }
    if (random_event(0.03)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.bags, inf);
    }
    if (random_event(0.01)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.jewelry, inf);
    }
    if (random_event(0.02)) {
        ulya.bank.balance -= apply_monthly_inflation(ulya.clothing.watches, inf);
    }
}


void vasya_clothing()
{
    double inf = random_inflation(7.1, 8.3);
    if (random_event(0.15)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.clothing.clothes, inf);
    }
    if (random_event(0.1)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.clothing.shoes, inf);
    }
    if (random_event(0.05)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.clothing.accessories, inf);
    }
    if (random_event(0.05)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.clothing.dry_clean, inf);
    }
    if (random_event(0.1)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.clothing.underwear, inf);
    }
    if (random_event(0.05)) {
        vasya.bank.balance -= apply_monthly_inflation(vasya.clothing.socks, inf);
    }
}

void ulya_entertainment()
{
    double inf = random_inflation(8.5, 9.7);  //reg
    ulya.entertainment.subscriptions = apply_monthly_inflation(ulya.entertainment.subscriptions, inf);
    ulya.entertainment.streaming     = apply_monthly_inflation(ulya.entertainment.streaming, inf);
    ulya.entertainment.hobbies       = apply_monthly_inflation(ulya.entertainment.hobbies, inf);
    ulya.entertainment.sport         = apply_monthly_inflation(ulya.entertainment.sport, inf);
    ulya.entertainment.books         = apply_monthly_inflation(ulya.entertainment.books, inf);
    ulya.entertainment.music         = apply_monthly_inflation(ulya.entertainment.music, inf);

    RUB regular_total = ulya.entertainment.subscriptions + ulya.entertainment.streaming +
                     ulya.entertainment.hobbies + ulya.entertainment.sport +
                     ulya.entertainment.books + ulya.entertainment.music;
    ulya.bank.balance -= regular_total;

    if (random_event(0.3)) {      //unreg
        ulya.entertainment.cinema = apply_monthly_inflation(ulya.entertainment.cinema, inf);
        ulya.bank.balance -= ulya.entertainment.cinema;
    }
    if (random_event(0.2)) {
        ulya.entertainment.theater = apply_monthly_inflation(ulya.entertainment.theater, inf);
        ulya.bank.balance -= ulya.entertainment.theater;
    }
    if (random_event(0.2)) {
        ulya.entertainment.concert = apply_monthly_inflation(ulya.entertainment.concert, inf);
        ulya.bank.balance -= ulya.entertainment.concert;
    }
    if (random_event(0.15)) {
        ulya.entertainment.museum = apply_monthly_inflation(ulya.entertainment.museum, inf);
        ulya.bank.balance -= ulya.entertainment.museum;
    }
    if (random_event(0.1)) {
        ulya.entertainment.bar = apply_monthly_inflation(ulya.entertainment.bar, inf);
        ulya.bank.balance -= ulya.entertainment.bar;
    }
    if (random_event(0.1)) {
        ulya.entertainment.nightclub = apply_monthly_inflation(ulya.entertainment.nightclub, inf);
        ulya.bank.balance -= ulya.entertainment.nightclub;
    }
    if (random_event(0.05)) {
        ulya.entertainment.karaoke = apply_monthly_inflation(ulya.entertainment.karaoke, inf);
        ulya.bank.balance -= ulya.entertainment.karaoke;
    }
    if (random_event(0.05)) {
        ulya.entertainment.bowling = apply_monthly_inflation(ulya.entertainment.bowling, inf);
        ulya.bank.balance -= ulya.entertainment.bowling;
    }
    if (random_event(0.05)) {
        ulya.entertainment.billiard = apply_monthly_inflation(ulya.entertainment.billiard, inf);
        ulya.bank.balance -= ulya.entertainment.billiard;
    }
    if (random_event(0.05)) {
        ulya.entertainment.quests = apply_monthly_inflation(ulya.entertainment.quests, inf);
        ulya.bank.balance -= ulya.entertainment.quests;
    }
    if (random_event(0.05)) {
        ulya.entertainment.amusement_park = apply_monthly_inflation(ulya.entertainment.amusement_park, inf);
        ulya.bank.balance -= ulya.entertainment.amusement_park;
    }
    if (random_event(0.03)) {
        ulya.entertainment.zoo = apply_monthly_inflation(ulya.entertainment.zoo, inf);
        ulya.bank.balance -= ulya.entertainment.zoo;
    }
    if (random_event(0.03)) {
        ulya.entertainment.aquarium = apply_monthly_inflation(ulya.entertainment.aquarium, inf);
        ulya.bank.balance -= ulya.entertainment.aquarium;
    }
    if (random_event(0.1)) {
        ulya.entertainment.games = apply_monthly_inflation(ulya.entertainment.games, inf);
        ulya.bank.balance -= ulya.entertainment.games;
    }
    if (random_event(0.1)) {
        ulya.entertainment.photos = apply_monthly_inflation(ulya.entertainment.photos, inf);
        ulya.bank.balance -= ulya.entertainment.photos;
    }
    if (random_event(0.15)) {
        ulya.entertainment.souvenirs = apply_monthly_inflation(ulya.entertainment.souvenirs, inf);
        ulya.bank.balance -= ulya.entertainment.souvenirs;
    }
}


void vasya_entertainment()
{
    double inf = random_inflation(7.1, 8.3);
    vasya.entertainment.subscriptions = apply_monthly_inflation(vasya.entertainment.subscriptions, inf);
    vasya.entertainment.streaming     = apply_monthly_inflation(vasya.entertainment.streaming, inf);
    vasya.entertainment.hobbies       = apply_monthly_inflation(vasya.entertainment.hobbies, inf);
    vasya.entertainment.sport         = apply_monthly_inflation(vasya.entertainment.sport, inf);
    vasya.entertainment.books         = apply_monthly_inflation(vasya.entertainment.books, inf);
    vasya.entertainment.music         = apply_monthly_inflation(vasya.entertainment.music, inf);

    RUB regular_total = vasya.entertainment.subscriptions + vasya.entertainment.streaming +
                     vasya.entertainment.hobbies + vasya.entertainment.sport +
                     vasya.entertainment.books + vasya.entertainment.music;
    vasya.bank.balance -= regular_total;

    if (random_event(0.2)) {
        vasya.entertainment.cinema = apply_monthly_inflation(vasya.entertainment.cinema, inf);
        vasya.bank.balance -= vasya.entertainment.cinema;
    }
    if (random_event(0.15)) {
        vasya.entertainment.theater = apply_monthly_inflation(vasya.entertainment.theater, inf);
        vasya.bank.balance -= vasya.entertainment.theater;
    }
    if (random_event(0.15)) {
        vasya.entertainment.concert = apply_monthly_inflation(vasya.entertainment.concert, inf);
        vasya.bank.balance -= vasya.entertainment.concert;
    }
    if (random_event(0.1)) {
        vasya.entertainment.museum = apply_monthly_inflation(vasya.entertainment.museum, inf);
        vasya.bank.balance -= vasya.entertainment.museum;
    }
    if (random_event(0.2)) {
        vasya.entertainment.bar = apply_monthly_inflation(vasya.entertainment.bar, inf);
        vasya.bank.balance -= vasya.entertainment.bar;
    }
    if (random_event(0.05)) {
        vasya.entertainment.games = apply_monthly_inflation(vasya.entertainment.games, inf);
        vasya.bank.balance -= vasya.entertainment.games;
    }
    if (random_event(0.05)) {
        vasya.entertainment.photos = apply_monthly_inflation(vasya.entertainment.photos, inf);
        vasya.bank.balance -= vasya.entertainment.photos;
    }
    if (random_event(0.1)) {
        vasya.entertainment.souvenirs = apply_monthly_inflation(vasya.entertainment.souvenirs, inf);
        vasya.bank.balance -= vasya.entertainment.souvenirs;
    }
}

void vasya_additional_income(int month, int year)
{
    if (random_event(0.2)) {
    vasya.bank.balance += apply_income_tax(vasya, vasya.freelance_income_month);
    }
    if (vasya.car.has_rental && random_event(0.3)) {
    vasya.bank.balance += apply_apartment_tax(vasya.car.rental_income, rental_tax_rate);
    }
    if (month == 11 && year == 2027) {
    vasya.bank.balance += apply_apartment_tax(vasya.dividend_income_year, dividend_tax_rate);
    }
    if (month == 11 && year == 2028) {
    vasya.bank.balance += apply_apartment_tax((RUB)(vasya.dividend_income_year * 1.01), dividend_tax_rate);
    }
}

void ulya_additional_income(int month, int year)
{
    if (random_event(0.3)) {
    ulya.bank.balance += apply_income_tax(ulya, ulya.freelance_income_month);
    }
    if (ulya.car.has_rental && random_event(0.5)) {
    ulya.bank.balance += apply_apartment_tax(ulya.car.rental_income, rental_tax_rate);
    }
    if (ulya.property.owns_apartment && random_event(0.1)) {
    ulya.bank.balance += apply_apartment_tax(ulya.rental_income_month, rental_tax_rate);
    }
    if (month == 9 && year == 2027) {
    ulya.bank.balance += apply_apartment_tax(ulya.dividend_income_year, dividend_tax_rate);
    }
    if (month == 9 && year == 2028) {
    ulya.bank.balance += apply_apartment_tax((RUB)(ulya.dividend_income_year * 1.02), dividend_tax_rate);
    }
    if (month == 9 && year == 2029) {
    ulya.bank.balance += apply_apartment_tax((RUB)(ulya.dividend_income_year * 1.04), dividend_tax_rate);
    }
}


void simulation_ulya()
{
    int year = 2026;
    int month = 9;
    while (not (year == 2027 and month ==9)) {
        if (month == 1) {
        reset_yearly_taxes(ulya);
        }
        ulya_salary(month, year);
        ulya_second_job();
        apply_social_deduction(ulya, month);
        ulya_additional_income(month, year);
        ulya_deposit();
        ulya_loan_payments();
        ulya_property(month);
        ulya_food();
        ulya_car(month);
        ulya_health();
        ulya_transport();
        ulya_entertainment();

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

void simulation_vasya()
{
    int year = 2026;
    int month = 9;
    while (not (year == 2027 and month ==9)) {
        if (month == 1) {
        reset_yearly_taxes(vasya);
        }
        vasya_salary(month, year);
        vasya_second_job();
        apply_social_deduction(vasya, month);
        vasya_additional_income(month, year);
        vasya_deposit();
        vasya_loan_payments();
        vasya_property(month);
        vasya_car(month);
        vasya_health();
        vasya_transport();
        vasya_entertainment();

        ++month;
        if (month == 13) {
            month = 1;
            ++year;
        }
    }
}

void vasya_init()
{
    vasya.name = "Sergeev Vasiliy";
    vasya.age = 32;
    vasya.married = false;
    vasya.has_children = false;
    vasya.birthday_month = 10;
    vasya.wedding_anniversary_month = 0;
    vasya.child_birth_month = 0;
    vasya.expecting_child = false;
    vasya.maternity_payment = 0;
    vasya.freelance_income_month = 2000;
    vasya.rental_income_month = 0;
    vasya.dividend_income_year = 1000;
    vasya.student_loan = 200'000;
    vasya.student_loan_month = 5000;
    vasya.car.loan = 300'000;
    vasya.car.loan_month = 10000;
    vasya.personal_loan = 0;
    vasya.personal_loan_month = 0;
    vasya.work.salary_month = 200'000;
    vasya.work.position = "Mechanical engineer";
    vasya.work.experience_years = 7;
    vasya.work.has_remote = true;
    vasya.work.vacation_days_used = 0;
    vasya.work.vacation_days_total = 28;
    vasya.work.education_allowance_year = 30000;
    vasya.work.transport_compensation_month = 2000;
    vasya.work.last_net_salary = 0;
    vasya.second_work.has_second_job = false;
    vasya.second_work.salary_month = 0;
    vasya.second_work.position = "";
    vasya.second_work.hours_per_week = 0;
    vasya.second_work.last_net_salary = 0;
    vasya.bank.balance = 45000;
    vasya.bank.deposit = 10000;
    vasya.bank.deposit_rate = 13.0;
    vasya.bank.credit_card_debt = 15000;
    vasya.bank.credit_card_limit = 200000;
    vasya.bank.credit_rate = 27.0;
    vasya.bank.investment = 20000;
    vasya.bank.crypto = 5000;
    vasya.bank.gold = 0;
    vasya.bank.bonds = 0;
    vasya.bank.pension = 10000;
    vasya.bank.life_insurance = 0;
    vasya.bank.debit_card_cashback = 200;
    vasya.car.value = 1800000;
    vasya.car.gas_month = 4000;
    vasya.car.maintenance_month = 2000;
    vasya.car.insurance_year = 72000;
    vasya.car.parking_month = 1000;
    vasya.car.tax_year = 10000;
    vasya.car.fine_avg = 300;
    vasya.car.age_months = 36;
    vasya.car.need_repair = false;
    vasya.car.washing_month = 500;
    vasya.car.tires_year = 6000;
    vasya.car.diagnostics_year = 2000;
    vasya.car.tolls_month = 200;
    vasya.car.rental_income = 0;
    vasya.car.has_rental = false;
    vasya.dog.name = "Barbos";
    vasya.dog.color = "white with a pint of gray";
    vasya.dog.age = 5;
    vasya.dog.food_month = 4000;
    vasya.dog.vet_month = 2000;
    vasya.dog.toys_month = 500;
    vasya.dog.insurance_month = 1000;
    vasya.dog.sick = false;
    vasya.dog.grooming_month = 0;
    vasya.dog.treats_month = 200;
    vasya.dog.bedding_year = 1000;
    vasya.dog.carrier_one_time = 0;
    vasya.dog.carrier_bought = true;
    vasya.property.rent_month = 30'000;
    vasya.property.utilities_month = 5000;
    vasya.property.internet_month = 800;
    vasya.property.phone_month = 400;
    vasya.property.insurance_year = 9600;
    vasya.property.has_mortgage = true;
    vasya.property.mortgage_month = 25000;
    vasya.property.mortgage_debt = 1'500'000;
    vasya.property.property_tax_year = 12000;
    vasya.property.renovation_year = 0;
    vasya.property.furniture_year = 0;
    vasya.property.appliances_year = 0;
    vasya.property.owns_apartment = true;
    vasya.property.apartment_value = 5'000'000;
    vasya.food.groceries = 12000;
    vasya.food.eating_out = 3000;
    vasya.food.fast_food = 800;
    vasya.food.delivery = 500;
    vasya.food.coffee = 150;
    vasya.food.sweets = 200;
    vasya.food.fruits = 500;
    vasya.food.vegetables = 400;
    vasya.food.meat = 1000;
    vasya.food.fish = 800;
    vasya.food.cottage_cheese= 400;
    vasya.food.bread = 150;
    vasya.food.alcohol = 1500;
    vasya.food.water = 300;
    vasya.food.juices = 150;
    vasya.food.snacks = 200;
    vasya.food.frozen = 300;
    vasya.food.canned = 100;
    vasya.food.spices = 50;
    vasya.health.medicine = 3000;
    vasya.health.pharmacy = 500;
    vasya.health.dentist = 3000;
    vasya.health.therapist = 1000;
    vasya.health.cardiologist = 1500;
    vasya.health.psychologist = 0;
    vasya.health.massage = 0;
    vasya.health.fitness = 2000;
    vasya.health.beauty = 0;
    vasya.health.barber = 1000;
    vasya.health.cosmetics = 0;
    vasya.health.perfume = 0;
    vasya.health.ophthalmologist = 1000;
    vasya.health.surgeon = 0;
    vasya.health.hospital = 0;
    vasya.health.physiotherapy = 0;
    vasya.health.vitamins = 500;
    vasya.health.dietary_supplements = 200;
    vasya.health.medical_tests = 300;
    vasya.health.ambulance = 0;
    vasya.clothing.clothes = 10000;
    vasya.clothing.shoes = 3000;
    vasya.clothing.accessories = 1000;
    vasya.clothing.dry_clean = 500;
    vasya.clothing.repair = 300;
    vasya.clothing.sportswear = 1000;
    vasya.clothing.underwear = 500;
    vasya.clothing.socks = 200;
    vasya.clothing.hats = 300;
    vasya.clothing.gloves = 200;
    vasya.clothing.bags = 0;
    vasya.clothing.jewelry = 0;
    vasya.clothing.watches = 0;
    vasya.transport.public_transport = 2500;
    vasya.transport.taxi = 500;
    vasya.transport.taxi_work = 500;
    vasya.transport.taxi_weekend = 300;
    vasya.transport.train = 2000;
    vasya.transport.plane = 15000;
    vasya.transport.bus = 500;
    vasya.transport.subway = 300;
    vasya.transport.bicycle = 0;
    vasya.transport.scooter = 0;
    vasya.transport.rideshare = 0;
    vasya.transport.car_sharing = 0;
    vasya.entertainment.travel = 50'000;
    vasya.entertainment.hotel = 3000;
    vasya.entertainment.hostel = 1000;
    vasya.entertainment.camping = 600;
    vasya.entertainment.museum = 300;
    vasya.entertainment.theater = 800;
    vasya.entertainment.concert = 1200;
    vasya.entertainment.cinema = 500;
    vasya.entertainment.hobbies = 2000;
    vasya.entertainment.subscriptions = 1500;
    vasya.entertainment.sport = 2000;
    vasya.entertainment.souvenirs = 1500;
    vasya.entertainment.photos = 500;
    vasya.entertainment.books = 800;
    vasya.entertainment.music = 200;
    vasya.entertainment.games = 300;
    vasya.entertainment.streaming = 600;
    vasya.entertainment.nightclub = 0;
    vasya.entertainment.bar = 500;
    vasya.entertainment.karaoke = 0;
    vasya.entertainment.bowling = 0;
    vasya.entertainment.billiard = 0;
    vasya.entertainment.quests = 0;
    vasya.entertainment.amusement_park = 0;
    vasya.entertainment.zoo = 0;
    vasya.entertainment.aquarium = 0;
    vasya.deposit_month = 30'000;
    vasya.emergency_fund = 20'000;
    vasya.taxes.ytd_taxable_income = 0;
    vasya.taxes.ytd_tax_paid = 0;
    vasya.taxes.ytd_fitness_paid = 30'000;
    vasya.taxes.ytd_social_deduction = 0;
}

void ulya_init()
{
    ulya.name = "Sergeeva Ulyana";
    ulya.age = 28;
    ulya.married = false;
    ulya.has_children = false;
    ulya.children_expenses_month = 0;
    ulya.children_count = 0;
    ulya.birthday_month = 4;
    ulya.wedding_anniversary_month = 2;
    ulya.child_birth_month = 0;
    ulya.expecting_child = false;
    ulya.maternity_payment = 0;
    ulya.freelance_income_month = 5000;
    ulya.rental_income_month = 0;
    ulya.dividend_income_year = 2000;
    ulya.student_loan = 0;
    ulya.student_loan_month = 0;
    ulya.car.loan = 0;
    ulya.car.loan_month = 0;
    ulya.personal_loan = 0;
    ulya.personal_loan_month = 0;
    
    ulya.work.salary_month = 180'000;
    ulya.work.position = "HR manager";
    ulya.work.experience_years = 5;
    ulya.work.has_remote = true;
    ulya.work.vacation_days_used = 0;
    ulya.work.vacation_days_total = 28;
    ulya.work.education_allowance_year = 50'000;
    ulya.work.transport_compensation_month = 3000;
    ulya.work.last_net_salary = 0;

    ulya.second_work.has_second_job = true;
    ulya.second_work.salary_month = 40'000;
    ulya.second_work.position = "Freelance tutor";
    ulya.second_work.hours_per_week = 8;
    ulya.second_work.last_net_salary = 0;

    ulya.bank.balance = 60'000;
    ulya.bank.deposit = 0;
    ulya.bank.deposit_rate = 14.0;
    ulya.bank.credit_card_debt = 0;
    ulya.bank.credit_card_limit = 300'000;
    ulya.bank.credit_rate = 25.0;
    ulya.bank.investment = 0;
    ulya.bank.crypto = 0;
    ulya.bank.gold = 0;
    ulya.bank.bonds = 0;
    ulya.bank.pension = 20000;
    ulya.bank.life_insurance = 0;
    ulya.bank.debit_card_cashback = 500;

    ulya.car.value = 2'400'000;
    ulya.car.gas_month = 5000;
    ulya.car.maintenance_month = 3000;
    ulya.car.insurance_year = 96000;
    ulya.car.parking_month = 2000;
    ulya.car.tax_year = 15000;
    ulya.car.fine_avg = 500;
    ulya.car.age_months = 24;
    ulya.car.need_repair = false;
    ulya.car.washing_month = 1000;
    ulya.car.tires_year = 8000;
    ulya.car.diagnostics_year = 3000;
    ulya.car.tolls_month = 500;
    ulya.car.rental_income = 0;
    ulya.car.has_rental = false;
    ulya.dog.name = "Sharik";
    ulya.dog.color = "brownish-black"; 
    ulya.dog.age = 2;
    ulya.dog.food_month = 6000;
    ulya.dog.vet_month = 3000;
    ulya.dog.toys_month = 1000;
    ulya.dog.insurance_month = 2000;
    ulya.dog.sick = false;
    ulya.dog.grooming_month = 500;
    ulya.dog.treats_month = 300;
    ulya.dog.bedding_year = 2000;
    ulya.dog.carrier_one_time = 2500;
    ulya.dog.carrier_bought = false;

    ulya.property.rent_month = 40'000;
    ulya.property.utilities_month = 7000;
    ulya.property.internet_month = 1000;
    ulya.property.phone_month = 500;
    ulya.property.insurance_year = 12000;
    ulya.property.has_mortgage = false;
    ulya.property.mortgage_month = 0;
    ulya.property.mortgage_debt = 0;
    ulya.property.property_tax_year = 0;
    ulya.property.renovation_year = 0;
    ulya.property.furniture_year = 0;
    ulya.property.appliances_year = 0;
    ulya.property.owns_apartment = false;
    ulya.property.apartment_value = 0;

    ulya.food.groceries = 15000;
    ulya.food.eating_out = 5000;
    ulya.food.fast_food = 1500;
    ulya.food.delivery = 1000;
    ulya.food.coffee = 300;
    ulya.food.sweets = 500;
    ulya.food.fruits = 1000;
    ulya.food.vegetables = 800;
    ulya.food.meat = 2000;
    ulya.food.fish = 1500;
    ulya.food.cottage_cheese = 700;
    ulya.food.bread = 300;
    ulya.food.alcohol = 2000;
    ulya.food.water = 500;
    ulya.food.juices = 300;
    ulya.food.snacks = 400;
    ulya.food.frozen = 600;
    ulya.food.canned = 200;
    ulya.food.spices = 100;
    ulya.food.baby_food = 0;

    ulya.health.medicine = 5000;
    ulya.health.pharmacy = 1000;
    ulya.health.dentist = 5000;
    ulya.health.therapist = 1500;
    ulya.health.cardiologist = 2500;
    ulya.health.psychologist = 3000;
    ulya.health.massage = 2500;
    ulya.health.fitness = 5000;
    ulya.health.beauty = 3000;
    ulya.health.barber = 0;
    ulya.health.cosmetics = 2000;
    ulya.health.perfume = 3000;
    ulya.health.ophthalmologist = 2000;
    ulya.health.surgeon = 5000;
    ulya.health.hospital = 10'000;
    ulya.health.physiotherapy = 1500;
    ulya.health.vitamins = 1000;
    ulya.health.dietary_supplements = 500;
    ulya.health.medical_tests = 800;
    ulya.health.ambulance = 2000;

    ulya.clothing.clothes = 15000;
    ulya.clothing.shoes = 5000;
    ulya.clothing.accessories = 2000;
    ulya.clothing.dry_clean = 1000;
    ulya.clothing.repair = 500;
    ulya.clothing.sportswear = 2000;
    ulya.clothing.underwear = 1000;
    ulya.clothing.socks = 300;
    ulya.clothing.hats = 500;
    ulya.clothing.gloves = 300;
    ulya.clothing.bags = 2000;
    ulya.clothing.jewelry = 1000;
    ulya.clothing.watches = 1500;

    ulya.transport.public_transport = 3000;
    ulya.transport.taxi = 1000;
    ulya.transport.taxi_work = 1500;
    ulya.transport.taxi_weekend = 1000;
    ulya.transport.train = 3000;
    ulya.transport.plane = 20'000;
    ulya.transport.bus = 1000;
    ulya.transport.subway = 500;
    ulya.transport.bicycle = 200;
    ulya.transport.scooter = 300;
    ulya.transport.rideshare = 400;
    ulya.transport.car_sharing = 600;

    ulya.entertainment.travel = 80'000;
    ulya.entertainment.hotel = 5000;
    ulya.entertainment.hostel = 1500;
    ulya.entertainment.camping = 1000;
    ulya.entertainment.museum = 500;
    ulya.entertainment.theater = 1500;
    ulya.entertainment.concert = 2000;
    ulya.entertainment.cinema = 1000;
    ulya.entertainment.hobbies = 3000;
    ulya.entertainment.subscriptions = 2000;
    ulya.entertainment.sport = 4000;
    ulya.entertainment.souvenirs = 3000;
    ulya.entertainment.photos = 1000;
    ulya.entertainment.books = 1500;
    ulya.entertainment.music = 500;
    ulya.entertainment.games = 1000;
    ulya.entertainment.streaming = 800;
    ulya.entertainment.nightclub = 1500;
    ulya.entertainment.bar = 1000;
    ulya.entertainment.karaoke = 800;
    ulya.entertainment.bowling = 600;
    ulya.entertainment.billiard = 500;
    ulya.entertainment.quests = 1000;
    ulya.entertainment.amusement_park = 2000;
    ulya.entertainment.zoo = 400;
    ulya.entertainment.aquarium = 600;
    ulya.deposit_month = 40'000;
    ulya.emergency_fund = 50'000;
    ulya.taxes.ytd_taxable_income = 0;
    ulya.taxes.ytd_tax_paid = 0;
    ulya.taxes.ytd_fitness_paid = 18'000;
    ulya.taxes.ytd_social_deduction = 0;
}

void print_results(const Person &p)
{
    printf("\n======================= %s =======================\n", p.name.c_str());      //c_str() для str => const char*  
    printf("Age: %d\n", p.age);
    printf("Married: %s\n", p.married ? "Yes" : "No");    // ?: - тернарный оператор, if/else
    printf("Has children: %s\n", p.has_children ? "Yes" : "No");
    
    printf("Cat: %s, age %d, color %s\n", p.dog.name.c_str(), p.dog.age, p.dog.color.c_str());
    printf("Dog: %s\n", p.has_pet_cat ? "Yes" : "No");
    if (p.has_pet_cat) {
        printf("Has dog, expenses:   %lld RUB/month\n", p.cat_expenses_month);
    }

    printf("Position:                          %s\n", p.work.position.c_str());
    printf("Salary (gross):                    %lld RUB\n", p.work.salary_month);
    printf("Salary (after tax):                %lld RUB\n", p.work.last_net_salary);
    if (p.second_work.has_second_job) {
        printf("Second job:                        %s\n", p.second_work.position.c_str());
        printf("Second salary (gross):             %lld RUB\n", p.second_work.salary_month);
        printf("Second salary (after tax):         %lld RUB\n", p.second_work.last_net_salary);
    }
    printf("NDFL paid this year:               %lld RUB\n", p.taxes.ytd_tax_paid);
    printf("Bank balance:                      %lld RUB\n", p.bank.balance);
    printf("Deposit:                           %lld RUB\n", p.bank.deposit);
    printf("Investments:                       %lld RUB\n", p.bank.investment);
    printf("Crypto:                            %lld RUB\n", p.bank.crypto);
    printf("Pension:                           %lld RUB\n", p.bank.pension);
    printf("Credit card debt:                  %lld RUB\n", p.bank.credit_card_debt);
    printf("Emergency fund:                    %lld RUB\n", p.emergency_fund);
    printf("Fitness expenses this year:        %lld RUB\n", p.taxes.ytd_fitness_paid);
    printf("Social deduction applied:          %lld RUB\n", p.taxes.ytd_social_deduction);

    RUB total = p.bank.balance + p.bank.deposit + p.bank.investment 
                + p.bank.crypto + p.bank.pension + p.emergency_fund - p.bank.credit_card_debt;
     
    if (p.property.has_mortgage) {
        printf("Mortgage debt:                     %lld RUB\n", p.property.mortgage_debt);
    }
    if (p.property.owns_apartment) {
        printf("Apartment value:                   %lld RUB\n", p.property.apartment_value);
    }

}


RUB total_total(const Person& p) {
    return p.bank.balance + p.bank.deposit + p.bank.investment + p.property.apartment_value + 
           p.bank.gold + p.bank.bonds + p.bank.pension + p.emergency_fund - 
           p.bank.credit_card_debt - p.student_loan - p.car.loan - 
           p.personal_loan - p.property.mortgage_debt;
}



int main()
{
    ulya_init();
    vasya_init();
    
    simulation_ulya();
    simulation_vasya();

    print_results(ulya);
    print_results(vasya);

    return 0;
}