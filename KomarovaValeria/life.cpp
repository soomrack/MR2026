#include <stdio.h>
#include <random>
#include <iostream>
#include <cmath>
#include <algorithm>


std::random_device rd;
std::mt19937 gen(rd());
using RUB = unsigned long long int;
using percent = double;
struct Bank;



struct Country
{
    double inflation_rate;
    percent inflation_percent;
    percent interest_rate;
    percent deposite_input;
    
};

struct Person
{
    RUB cash;
    RUB salary;
    RUB black_salary;
    RUB savings;

    RUB food_expenses;
    RUB gym_cost;
    RUB health_expenses;
    RUB psychologist_cost;
    RUB qualification_cost;

    RUB health_cash;
    RUB deposit_cash;

    bool financial_condition;
    Bank* mortgage_bank = nullptr;
    bool mortage_flag;
    

    int stress_point;
    //int energy;

    int qualification_rate;
    int education_month_number;
    bool education_flag;

    

    int flat_number;

    
    void freelance(){
        cash += salary*0.5;
        stress_point += 15;
    }


    void pay(const RUB sum){
        if(cash >= sum){
            cash -= sum;
        }
        else{
            printf("MALO MONEY OTSTUPAEM");
        }

    }


    void gym(){
        pay(gym_cost);
        stress_point -= 3;
    }


    void psychologist(){
        pay(psychologist_cost);
        stress_point -= 5;
    }

    
};


struct Flat
{
    RUB rent_expenses;
    RUB summer_utilities;
    RUB winter_utilities;
    RUB start;
    RUB repair_expences;
    RUB medium_repair_expences;
    RUB flat_price;

    void repair()
    {
        std::uniform_int_distribution<int> probab_repair(1,20);
        std::uniform_real_distribution<double> importance_rate(0.8,2);
        if(probab_repair(gen) == 19){
            repair_expences = std::lround(medium_repair_expences*importance_rate(gen));
        }
        else{
            repair_expences = 0;
        }
    }
};


struct Bank
{
    percent interest_bank_rate;
    percent deposit_bank_rate;
    percent start_pay_percent;
    percent requirements_to_salary;
    

    struct Mortgage
    {
        Bank* bank = nullptr;
        RUB mortgage_sum;
        percent mortgage_rate;
        RUB payment;
        RUB start_payment;
        RUB wanted_sum;
        int deposit_term;
        int start_month;
        int start_year;


        bool requirements(const int year,const int month,Person &person,const RUB sum)
        {   
            start_payment = std::lround(wanted_sum * ((bank->start_pay_percent)/100.0));
            mortgage_rate = bank->interest_bank_rate;
            mortgage_sum = wanted_sum - start_payment;
            int number_month = (std::abs(9-month)+(2036-year)*12);
            double month_rate_pow = std::pow((1+mortgage_rate/1200.0),number_month);
            payment = std::lround(mortgage_sum*(((mortgage_rate/1200.0)*(month_rate_pow))/((1+month_rate_pow)-1)));

            std::uniform_int_distribution<int> probability_approval(1,3);
            double approval_part = bank->requirements_to_salary/100.0;
            
            if(person.cash > start_payment and (bank->requirements_to_salary/100.0)*person.salary > payment){
                return true;
            }
            if(approval_part*person.salary < payment and payment <(approval_part+0.15)*person.salary and person.cash > start_payment){
                if(not(probability_approval(gen) == 3)){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }

        }


        void init_mortage(const int year,const int month, const RUB sum,Person &person)
        {
            wanted_sum = sum;
            start_month = month;
            start_year = year;
            person.mortage_flag = true;
            person.stress_point += 20;
               
        }  
        
        
        void alice_mortage(Person &person,const int year,const int month)
            {
                if(not(year == start_year) and month==start_month){
                    mortgage_sum *= (1+(bank->interest_bank_rate/100.0));
                }

                if(not(mortgage_sum == 0)){
                    if(mortgage_sum >= payment and person.cash >= payment){
                        mortgage_sum -= payment;
                        person.pay(payment);
                    }
                    if(mortgage_sum < payment and person.cash >= mortgage_sum){
                        person.pay(mortgage_sum);
                        mortgage_sum = 0;
                    }    
                }
                else{
                    printf("mortage closed\n");
                    person.mortage_flag = false;
                }
            }
        
        
    };


    struct Deposite
    {
        Bank* bank = nullptr;
        percent deposit_rate;
        RUB deposite_bank_cash;


    };


    Mortgage mortgage;
    Deposite deposit;

    //Функции добавления банка для депозита и ипотеки
    void add_bank_mortgage()
    {
        mortgage.bank = this;
    }


    void add_bank_deposit()
    {
        deposit.bank = this;

    }

    // функция изменения условий банка
    double banks_salary_requirements_change()
    {
        if(interest_bank_rate <= 14.5){
            return interest_bank_rate * 2.3;
        }
        if(14.5 < interest_bank_rate && interest_bank_rate < 17){
            return interest_bank_rate * 2.6;
        }
        else{
            return interest_bank_rate * 2.8;
        }

    }

    double banks_start_pay_change()
    {
        if(interest_bank_rate <= 15){
            return 20 + (interest_bank_rate*2.0)/100.0 ;
        }
        else{
            return 20 - (interest_bank_rate*2.0)/100.0;
        }

    }

    void banks_change(Country &Russia)
    {
        std::uniform_real_distribution<double> interest_bank_rate_rand(Russia.interest_rate+0.3,Russia.interest_rate+3);
        std::uniform_real_distribution<double> deposit_rate_rand(Russia.deposite_input-2,Russia.deposite_input-0.1);

        deposit_bank_rate = deposit_rate_rand(gen);
        interest_bank_rate = interest_bank_rate_rand(gen);
        requirements_to_salary = banks_salary_requirements_change();
        start_pay_percent = banks_start_pay_change();
    }


};


struct Tax_system
{
    percent NDFL;
    percent upper_NDFL;
    RUB revenue_roof;
    percent property_tax;
    percent transport_tax;
    RUB tax_sum;


    int ndfl_sum(const Person &person)
    {
        if(person.salary*12 <= revenue_roof){
            tax_sum += std::lround((NDFL/100.00)*person.salary);
            return std::lround((NDFL/100.00)*person.salary);
        }
        else{
            tax_sum += std::lround((NDFL/100.0)*person.salary+(((person.salary*12-revenue_roof))*(upper_NDFL/100.0))); 
            return std::lround((NDFL/100.0)*person.salary+(((person.salary*12-revenue_roof))*(upper_NDFL/100.0)));
        }
    }


    //void property_sum(){


    //}

    void alice_deduction()
    {



    }
};




struct Person alice;
struct Country Russia;
struct Flat flat;
struct Bank Alfa;
struct Bank VTB;
struct Bank sber;
struct Tax_system tax_system;




//ввод стартовых данных 
void flat_init()
{
    flat.rent_expenses = 25000;
    flat.start = 20000;
    flat.summer_utilities = 6000;
    flat.winter_utilities = 10000;
    flat.medium_repair_expences = 3000;

}


void alice_init()
{
    alice.cash = 40000;
    alice.black_salary = 90000;
    alice.food_expenses = 17000;
    alice.health_expenses = 1500;
    alice.stress_point = 20;
    alice.deposit_cash = 0;
    alice.mortage_flag = false;
    alice.financial_condition = true;
    alice.gym_cost = 2500;
    alice.psychologist_cost = 3000;
    alice.qualification_cost = 15000;
    alice.education_flag = false;

}


void country_init()
{
    Russia.inflation_rate = 1.0;
    Russia.inflation_percent = 7.0;
    Russia.interest_rate = 14.0;

}


void tax_system_init(){
    tax_system.NDFL = 13;
    tax_system.upper_NDFL = 15;
    tax_system.revenue_roof = 2400000;
    tax_system.property_tax = 0.15;
    tax_system.tax_sum = 0;

}


void banks_init()
{  
    Alfa.deposit_bank_rate = Russia.deposite_input - 0.5;
    Alfa.interest_bank_rate = Russia.interest_rate + 1;
    Alfa.requirements_to_salary = Alfa.interest_bank_rate/100.0*2.7;
    Alfa.start_pay_percent = 20;

    VTB.deposit_bank_rate = Russia.deposite_input - 0.8;
    VTB.interest_bank_rate = Russia.interest_rate + 1.2;
    VTB.requirements_to_salary = VTB.interest_bank_rate/100.0*2.7;
    VTB.start_pay_percent = 20;

    sber.deposit_bank_rate = Russia.deposite_input - 0.4;
    sber.interest_bank_rate = Russia.interest_rate + 0.8;
    sber.requirements_to_salary = sber.interest_bank_rate/100.0*2.7;
    sber.start_pay_percent = 20;

}


// блок изменения страны/доходов
void alice_salary(const int year,const int month)
{
if(year <= 2028 and month == 1){
alice.black_salary += 30000;
}

if(year>2029 and month == 1 and not(year % 3 == 0)){
    alice.black_salary = alice.black_salary * 1.1;
}
}


void alice_education(int& time)
{
    std::uniform_real_distribution<double> level_education(1.05, 1.10);
    std::uniform_real_distribution<double> cost_rate(0.8, 2);
    double level = level_education(gen);
    int cost = std::lround(cost_rate(gen)*alice.qualification_cost);
    
    if(level <= 1.08 and time == 0){
        alice.education_month_number = 2;
        alice.pay(cost);
        alice.education_flag = true;
    }
    if(level > 1.08 and time == 0){
        alice.education_month_number = 4;
        alice.pay(cost);
        alice.education_flag = true;
    }

    time += 1;
    alice.stress_point += 10;
    printf("%d\n",time);
    if(time > alice.education_month_number){
        alice.black_salary = std::lround(alice.black_salary * level);
        time = 0;
        alice.education_flag = false;
    }


}

void country_change(const int month){
    if(month == 1){
        std::uniform_real_distribution<double> inflation(5.0,9.0);
        std::uniform_real_distribution<double> interest_rate(13.0,17.0);

        Russia.inflation_percent = std::round(inflation(gen)*10)/10;
        Russia.interest_rate = std::round(interest_rate(gen)*10)/10;
        Russia.inflation_rate = 1 + Russia.inflation_percent/100;
        
    }
    
}


int inflation(int mean, double rate)
{
    return std::lround(mean*rate);
}


void cost_change(const int month)
{
    if(month == 1){
        alice.food_expenses = inflation(alice.food_expenses,Russia.inflation_rate);
        alice.health_expenses = inflation(alice.health_expenses,Russia.inflation_rate);
        alice.gym_cost = inflation(alice.gym_cost,Russia.inflation_rate);
        alice.qualification_cost = inflation(alice.qualification_cost,Russia.inflation_rate);
        alice.psychologist_cost = inflation(alice.psychologist_cost,Russia.inflation_rate);
     }
}
//функции связанные с банком
void alice_find_bank(const int year, const int month, const RUB flat_price)
{
    Bank* banks[] = {&Alfa, &VTB, &sber};
    Bank* target = banks[0];

    for (Bank* b : banks){
        b->add_bank_mortgage();

        if(b->interest_bank_rate < target->interest_bank_rate){
            target = b;
        }
    }

    if(target->mortgage.requirements(year,month,alice,flat_price)){
        alice.mortgage_bank = target;
    }
    else{

    }

    
}


int alice_deposite(){
    int deposit_month = 1;
    long int transfer;
    RUB bank_deposite_cash;
    percent actually_deposite_rate;

    if(deposit_month % 6 == 1){
        actually_deposite_rate = Russia.deposite_input;
    }

    if(alice.cash >= 3*alice.salary){
        transfer = std::lround(0.1*alice.salary);
        alice.pay(transfer);
        alice.deposit_cash += transfer;
    }
    
    
    alice.deposit_cash += std::lround((alice.deposit_cash*(actually_deposite_rate/100.00))/12);
    
    ++deposit_month;
    
    return deposit_month;
}

// алиса прикидывает хватит ли ей имеющихся денег на жизнь
void alice_deposit_plans(const int deposit_month){
    if(alice.cash <= (alice.food_expenses + flat.rent_expenses + Alfa.mortgage.payment+flat.winter_utilities)*2 and deposit_month % 6 == 0 ){
        int backup_transfer;
        backup_transfer = std::lround(0.7*alice.deposit_cash);
        alice.cash += backup_transfer;
        alice.deposit_cash -= backup_transfer;
    }


}


void cash_plans(){
        RUB mandatory_expenses;

        if(alice.mortage_flag == false){ 
            mandatory_expenses = alice.food_expenses + flat.rent_expenses;
        }
        else{
           mandatory_expenses = alice.mortgage_bank->mortgage.payment + alice.food_expenses; 
        }

        if(mandatory_expenses*3> alice.cash){
            alice.financial_condition = false;
        }
        else{
            alice.financial_condition = true;
        }
        
}


// функции базовых расходов на жизнь
void alice_food(const int year,const int month)
{
    

    if(year == 2027 and month == 1){
        alice.food_expenses += 4000;
    }

    if(alice.cash <= alice.food_expenses){
        printf("You died. No food");
        exit(0);
    }

    alice.pay(alice.food_expenses);
    
}


void alice_rent(const int year, const int month)
{
    if(month == 1 and ((year-2026) % 3 == 0) and not(year == 2026)){
        flat.rent_expenses = inflation(flat.rent_expenses,Russia.inflation_rate);
    }
    // стартовый взнос
    if(year == 2026 and month == 9){
        alice.pay(flat.start);
    }

    alice.pay(flat.rent_expenses + flat.repair_expences);
}


void alice_utilities(const int mounth)
{

if(4 >= mounth >= 1 or mounth == 12){
    alice.pay(flat.winter_utilities);
}
else{
    alice.pay(flat.summer_utilities);
}
}


void alice_health(const int month)
{ 
    
    //в сезон простуд вероятность заболеть выше
    int ceiling_probability;
    if(9 <= month <= 12 or 1 <= month <= 3){
        ceiling_probability = 15;
    }
    else{
        ceiling_probability = 25;
    }

    // если заболели - тратимся на лечение
    std::uniform_int_distribution<int> ver(1,ceiling_probability);
    int probability_healht = ver(gen);
    if(probability_healht == 2){
        alice.pay(alice.health_expenses);
        alice.health_cash += alice.health_expenses;
    }

// шанс серьезно заболеть  если высокий уровень стресса
    if(alice.stress_point >= 55){
        ceiling_probability = 30;
        if(ver(gen) == 2){
            alice.cash -= alice.health_expenses*10;

        }

    }
}



void alice_print(){
	printf("Alice cash = %llu\nAlice salary = %llu\n", alice.cash,alice.salary);
}
 


void simulation()
{
int year = 2026;
int month = 9;
int education_time = 0;
while(not(year == 2036 and month ==9)){
    
    alice.salary = alice.black_salary - tax_system.ndfl_sum(alice);
    alice.cash += alice.salary;
    alice_salary(year,month);

    cost_change(month);
    alice_food(year,month);
    if(alice.flat_number == 0){
        alice_rent(year,month);
    }
    alice_utilities(month);
    flat.repair();
    
    if(alice.financial_condition == true){
        alice.gym();
    }
    if(alice.stress_point >=35 and alice.stress_point <=55 and alice.financial_condition == true){
        alice.psychologist();
    }
    if(alice.stress_point > 55){
        alice.psychologist();
    }

    
    if((month == 6 and year % 5 ==0) or not(education_time == 0)){
        alice_education(education_time);
    }

    //alice_deduction();
    //alice_cat();
    alice_health(month);
    //alice_deposite();
    country_change(month);
    

    /*if(alice.cash >= 2500000 and mortage.mortage_sum == 0){
        init_mortage(year,month,alice);
        alice.cash -= mortage.start_payment;
        alice.flat_number += 1;
        printf("ATTENTION, MORTAGE COMING!! mortage_sum %llu, mortage_payment %llu\n",mortage.mortage_sum,mortage.payment);
    }
    
    if(alice.mortage_flag){
        alice_mortage();
    }*/
    
    if(alice.stress_point >= 2){
        alice.stress_point -= 2;
    }

    ++month;
    if(month == 13){
        ++year;
        month = 1;
}
}
}


int main()
{
    flat_init();
    country_init();
    banks_init();
    alice_init();
    tax_system_init();
    simulation();
    alice_print();

}

