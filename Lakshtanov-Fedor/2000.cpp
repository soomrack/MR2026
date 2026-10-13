/*
git add .
git commit -m "."
git push
*/

#include <iostream>  
#include <array>
#include <string>
#include <algorithm>


using RUB = unsigned long long int;


// классы
struct Job{
    bool is_job = false;
    RUB salary = 105'000;
};


struct Automobile
{
    bool is_have = false;
};


struct Animal
{
    bool is_life =false;
};




struct Apartment{
    bool is_have = false;
    RUB now_cost = 6'000'000;
    RUB mortgage_pay = 24'000'000;
    double area_apartment = 32.2;
    double body_payments = static_cast<double>(now_cost) / mortgage_pay;  // какая часть от выплат это тело кредита
};


struct Statistic{
    RUB pay_year = 0;
    RUB salary_year = 0;
    RUB tax_year = 0;

    RUB pay_life = 0;
    RUB salary_life = 0;
    RUB tax_life = 0;

    RUB mortgage_year_pay = 0;
};


struct World_tax{
    std::array <RUB,5> individual_tax_limit= {0, 2'400'000,  5'000'000,  20'000'000,  50'000'000};
    std::array <double,5> NDFL= {0.13,0.15,0.18,0.20,0.22};

    double area_not_tax = 20;
    double area_rate = 0.002;

    RUB summ_refund_pay_mortgage_tax_max = 3'000'000; // максимальная сумма по процентам ипотеки предьявляемая к вычету
    RUB save_NDFL = 0;
};

struct Expenses_standart {
    // Обязтельные
    RUB utilities = 5000; // коммуналка
    RUB internet_mobile = 1000; // интернет + связь
    RUB food = 20000; // питание
    RUB clothes = 3000; // одежда
    RUB leisure = 3000; // досуг
    RUB other = 2000; // прочее

    //необязательные
    RUB rent = 30000; // аренда 
    RUB mortgage = 50000; //ипотека
    RUB transport = 5000; // транспорт
    RUB pet_food = 3000; // еда для питомца
};

struct Person {
    bool is_life  = true;
    int age_years  = 20;
    int age_months = 5;

    RUB cash = 200'000;
    RUB debt = 0;

    std::array<Job,4> works;

    Statistic data;

    RUB tax_duty = 0;
    World_tax tax_book;

    std::string reason_end = "";

    Apartment home_alise;

    Expenses_standart expenses;

    Automobile car;

    Animal pet;

    
};


// функции 
void man_print(const Person& man) {
    std::cout << "Person" << "\n"
        << "year = "  << man.age_years <<  "\n"
        << "month = " << man.age_months << "\n"
        << "life = "  << man.is_life << "\n"
        << "cash = " << man.cash << "\n"
        << "debt = " << man.debt << "\n"
        << "mortgage_pay = " << man.home_alise.mortgage_pay << "\n"
        << "reason_end = " << man.reason_end << "\n";
}


void man_print_data_year(const Person& man){
    std::cout << "data_year" << "\n"
    << "pay = "<<man.data.pay_year << "\n"
    << "salary = "<<man.data.salary_year << "\n"
    << "tax = "<<man.data.tax_year << "\n";
}


void man_print_data_life(const Person& man){
    std::cout << "data_life" << "\n"
    << "pay = "<<man.data.pay_life << "\n"
    << "salary = "<<man.data.salary_life << "\n"
    << "tax = "<<man.data.tax_life << "\n";
}


void man_print_end_simulation(const Person& man, const int month_end, const int year_end){
    std::cout << "        SIMULATION END" << "\n"
    << " DATE  " << year_end << " year " << month_end << " month " <<"\n"; 
}


RUB calc_ndfl_summ(const RUB profit, const Person& man){
    double tax = 0;

    for (size_t id = 0; id < man.tax_book.individual_tax_limit.size(); id++)
    {
        RUB low = man.tax_book.individual_tax_limit[id];
        if (profit <= low) break;

        bool is_last = (id + 1 == man.tax_book.individual_tax_limit.size());
        RUB high = is_last ? profit : std::min(profit, man.tax_book.individual_tax_limit[id + 1]);

        tax += (high - low) * man.tax_book.NDFL[id];
    }

    return tax ; 
}

double calc_ndfl_rate(const RUB profit, const Person& man){
    double rate = man.tax_book.NDFL[0];

    for (size_t id = 0; id < man.tax_book.individual_tax_limit.size(); id++)
    {
        if (profit <= man.tax_book.individual_tax_limit[id]) break;

        rate = man.tax_book.NDFL[id];
    }

    return rate;
}

RUB refund_tax_mortgage(Person& man){
    if (man.tax_book.summ_refund_pay_mortgage_tax_max == 0) return 0;

    RUB max_refund = man.data.mortgage_year_pay *(1- man.home_alise.body_payments); //упрощенная формула

    max_refund = std::min( man.tax_book.summ_refund_pay_mortgage_tax_max , max_refund);

    return max_refund;
    
}




void get_NDFL_tax(Person& man){  //переписано как в жизни
    RUB profit = man.data.salary_year;

    RUB refund_motgage = refund_tax_mortgage(man);
    man.tax_book.save_NDFL += refund_motgage * calc_ndfl_rate(profit, man);
    man.tax_book.summ_refund_pay_mortgage_tax_max -= refund_motgage;

    RUB ndfl_year = calc_ndfl_summ(profit, man);
    RUB ndfl_underpaid = 0;
    if (ndfl_year > man.data.tax_year) ndfl_underpaid = ndfl_year - man.data.tax_year;

    RUB refund = std::min(man.tax_book.save_NDFL, ndfl_year);
    man.tax_book.save_NDFL -= refund;

    if (refund >= ndfl_underpaid){
        man.cash += refund - ndfl_underpaid;
    } else {
        man.tax_duty += ndfl_underpaid - refund;
    }
}


void get_apartment_tax(Person& man){
    if (man.home_alise.is_have)
    { 
        man.tax_duty += std::max(0.0, man.home_alise.now_cost * man.tax_book.area_rate *
        ((man.home_alise.area_apartment - man.tax_book.area_not_tax ) / man.home_alise.area_apartment ));
    }
}





void get_last_year_tax_alise( Person& man){
    get_NDFL_tax(man);
    get_apartment_tax(man);


}


void check_end_alise(bool& is_running, const int year, Person& man){
    if (year == 2100)
    {
        man.reason_end += "Time out ";
        is_running = false;
    }

    if (man.debt > 0 )
    {
        man.reason_end += "Bankruptcy (No money) ";
        is_running = false;
    }

    if (man.home_alise.mortgage_pay <= 0)
    {
        man.reason_end += "WIN Mortgage(0) ";
        is_running = false;
    }
    
    
    
}


void next_month_year(int& month, int&year){
    month++;
    if (month > 12) {
        month = 1;
        year++;
    }
}


void time_step(int& month, int&year, Person& man, const bool is_running){
    if (!is_running) return;

    next_month_year(month,year);

    next_month_year(man.age_months,man.age_years);
}


void pay_rub( const RUB money, Person& man){
    if (man.cash >= money)
    {
        man.cash -= money;
        man.data.pay_year += money;
        return;
    }
    man.debt += money - man.cash;
    man.data.pay_year += money;
    man.cash = 0;
}


void init_alise(Person& man){
    man.works[0].is_job = true;
    man.home_alise.is_have = true;
}


void get_salary(Person& man){
    for ( size_t id_job = 0; id_job < man.works.size(); id_job++)
    {
        if (man.works[id_job].is_job)
        {
            RUB tax = man.works[id_job].salary * man.tax_book.NDFL[0];
            man.cash += man.works[id_job].salary - tax;

            man.data.tax_year += tax;
            man.data.salary_year += man.works[id_job].salary;
            
        }
    }
}


void reload_data_yaer_life(Person& man){
    man.data.pay_life += man.data.pay_year;
    man.data.salary_life += man.data.salary_year;
    man.data.tax_life += man.data.tax_year;

    man.data.pay_year = 0;
    man.data.salary_year = 0;
    man.data.tax_year = 0;

    man.data.mortgage_year_pay = 0;
}


void month_pay_alise(Person& man) {
    
    pay_rub(man.expenses.utilities, man);
    pay_rub(man.expenses.internet_mobile, man);
    pay_rub(man.expenses.food, man);
    pay_rub(man.expenses.clothes, man);
    pay_rub(man.expenses.leisure, man);
    pay_rub(man.expenses.other, man);
    
    if (man.home_alise.is_have)
    {
        pay_rub(man.expenses.mortgage, man);
        man.home_alise.mortgage_pay -= man.expenses.mortgage;
        man.data.mortgage_year_pay += man.expenses.mortgage;
    };

    if (man.pet.is_life)
    {
        pay_rub(man.expenses.pet_food, man);
    }

    if (!man.car.is_have)
    {
        pay_rub(man.expenses.transport, man);
    }
    
    
}

void debt_pay_alise(Person& man){//будет доставать деньги не только с cash
    if (man.cash >= man.debt)
    {
        man.cash -= man.debt;
        man.debt = 0;
    } else
    {
        man.debt -=  man.cash;
        man.cash = 0;
    }
    
    
}

void tax_duty_pay(Person& man){
    pay_rub(man.tax_duty, man);
    man.tax_duty = 0;
}


void alise_try_pay_mortgage(Person& man){// переписать когда появится деньги не в cash
    if (man.cash >= man.home_alise.mortgage_pay){
        pay_rub(man.home_alise.mortgage_pay, man);
        man.home_alise.mortgage_pay = 0;
    }
}


void simulation_alise() {
    Person alise;

    //man_print(alise);
    
    int month = 10;
    int year = 2026;
    bool is_running = true;
    
    init_alise(alise);
    

    while (is_running)
    {
        // события года
        if (month == 1) get_last_year_tax_alise(alise);

        if (month == 1) reload_data_yaer_life(alise);

        if (month == 11) tax_duty_pay(alise);

        // события порядок не важен
        month_pay_alise(alise);
        get_salary(alise);

        // после ничего не добавлять
        debt_pay_alise(alise);
        alise_try_pay_mortgage(alise);
        check_end_alise(is_running,year,alise);
        time_step(month,year,alise,is_running);
        
    }
    
    man_print_end_simulation(alise,month,year);
    man_print(alise);
    man_print_data_year(alise);
    man_print_data_life(alise);
}


int main() {
    
    simulation_alise();

    return 0;
}
