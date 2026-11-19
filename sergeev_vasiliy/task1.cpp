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
    RUB loan;                    
    RUB loan_month;  
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
    FoodExpenses food;
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


RUB calculate_go_back_year(RUB annual_income)       // возврат
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


RUB apply_income_tax(Person &p, RUB gross_amount);         //для случаев когда сумма дохода становится > ndfl_limit
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
