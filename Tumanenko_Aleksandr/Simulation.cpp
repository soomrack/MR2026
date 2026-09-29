#include <iostream>
#include <string>


using RUB = int;


struct Person{
    RUB cash;
    RUB salary;
    RUB savings; // абсолютно все сбережения и имущество выраженные в валюте
    RUB buy; // суммарная стоимость всех покупок
};

struct Person oleg;


void oleg_init()
{
    oleg.cash = 20'000;
    oleg.salary = 70'000;
}

void oleg_savings()
{
    oleg.savings = oleg.savings + oleg.cash + oleg.buy;

}

void new_buy(std::string purchase, int cost)
{
    int year = 2028;
    int month = 7;
    std::string old_purchase;
    while (purchase != old_purchase){
        
        oleg.cash -= cost;
        oleg.buy += cost;
        old_purchase = purchase;
    
    }
}

void oleg_salary(const int year, const int month)
{
    if (year == 2028 and month == 11) {  
        oleg.salary = 90'000;
    }

    oleg.cash += oleg.salary;
}


void simulation()
{
    int year = 2028;
    int month = 7;
    while (not (year == 2029 and month == 7)) {

        oleg_salary(year, month);
       
        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

void oleg_print()
{
    std::cout<<(oleg.cash)<<'\n';

}


int main()
{
    std::string purchase = {"laptop"};
    int cost = 90000;
    oleg_init();
    
    simulation();

    oleg_savings();

    new_buy(purchase,cost);

    oleg_print();

}