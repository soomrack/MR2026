#include <iostream>
#include <string>


using RUB = int;


struct Person{
    RUB cash;
    RUB salary;
    RUB capital; // абсолютно все сбережения и имущество выраженные в валюте
    RUB buy; // суммарная стоимость всех покупок
    double tax; // сумма выплаченных государству налогов включая НДФЛ,на имущество, земельный, транспортный
};

struct Person oleg;


void oleg_init()
{
    oleg.cash = 20'000;
    oleg.salary = 70'000;
}

void oleg_capital()
{
    oleg.capital = oleg.capital + oleg.cash + oleg.buy;

}

void new_buy(std::string purchase, int cost)
{
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


void oleg_print()
{
    std::cout<<(oleg.cash)<<'\n';
    std::cout<<(oleg.tax)<<'\n';
    std::cout<<(oleg.capital)<<'\n';
}

void tax_job (const int year, const int month)
{
    double percent_NDFL;
    int limit1 = 2'400'000 / 12;   // 200 000
    int limit2 = 5'000'000 / 12;   // 416 666
    int limit3 = 20'000'000 / 12;  // 1 666 666
    int limit4 = 50'000'000 / 12;  // 4 166 666

    if (oleg.salary <= limit1) {
        percent_NDFL = 0.13;
    } else if (oleg.salary <= limit2) {
        percent_NDFL = 0.15;
    } else if (oleg.salary <= limit3) {
        percent_NDFL = 0.18;
    } else if (oleg.salary <= limit4) {
        percent_NDFL = 0.20;
    } else {
        percent_NDFL = 0.22;
    }

        oleg.tax += oleg.salary*(percent_NDFL/(1-percent_NDFL));
    
}

void simulation()
{
    int year = 2028;
    int month = 7;
    while (not (year == 2029 and month == 7)) {

        
        oleg_salary(year, month);
        tax_job(year, month);
       
        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

int main()
{
    // std::cout << "1233";
    oleg_init();
    // std::cout << "123";
    simulation();

    oleg_capital();

    std::string purchase = {"laptop"};
    int cost = 90000;
    new_buy(purchase,cost);

    purchase = {"armchair"};
    cost = 32000;
    new_buy(purchase,cost);


    oleg_print();

}