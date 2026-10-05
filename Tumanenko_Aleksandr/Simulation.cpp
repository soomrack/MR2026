#include <iostream>
#include <string>
#include <locale>
#include <random>

struct ApostrophePunct : std::numpunct<char> {
    // Символ-разделитель
    char do_thousands_sep() const override { return '\''; } 
    // Группировка: по 3 цифры
    std::string do_grouping() const override { return "\3"; } 
};

using RUB = int;


struct Person{
    RUB cash;
    RUB salary_job;
    RUB salary_freelance;
    RUB profit;// абсолютно все ежемесечные доходы в валюте
    RUB capital; // абсолютно все сбережения и имущество выраженные в валюте
    RUB buy; // суммарная стоимость всех покупок
    RUB expenses_month;
    RUB rent_home;
    double tax; // сумма выплаченных государству налогов включая НДФЛ,на имущество, земельный, транспортный
};

struct Person oleg;


void oleg_init()
{
    oleg.cash = 20'000;
    oleg.salary_job = 70'000;
    oleg.expenses_month = 40'000;
    oleg.rent_home = 40'000;
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

double ndfl_percent(int finance)
{
    double percent_NDFL;
    int limit1 = 2'400'000 / 12;   // 200 000
    int limit2 = 5'000'000 / 12;   // 416 666
    int limit3 = 20'000'000 / 12;  // 1 666 666
    int limit4 = 50'000'000 / 12;  // 4 166 666

    if (finance <= limit1) {
        percent_NDFL = 0.13;
    } else if (finance <= limit2) {
        percent_NDFL = 0.15;
    } else if (finance <= limit3) {
        percent_NDFL = 0.18;
    } else if (finance <= limit4) {
        percent_NDFL = 0.20;
    } else {
        percent_NDFL = 0.22;
    }

    return percent_NDFL;
}

void oleg_profit(const int year, const int month)
{
    if (year == 2028 and month == 11) {  
        oleg.salary_job = 90'000;
    }

    int min_salary_freelance = 5'000;
    int max_salary_freelance = 50'000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr( min_salary_freelance,  max_salary_freelance);

    oleg.salary_freelance = distr(gen);

    double percent_NDFL = ndfl_percent(oleg.salary_freelance);
    oleg.profit = oleg.salary_job + oleg.salary_freelance*(1-percent_NDFL);

    oleg.cash += oleg.profit;
}

void tax_job (const int year, const int month)
{
    double percent_NDFL = ndfl_percent(oleg.salary_job);

    oleg.tax += oleg.salary_job*(percent_NDFL/(1-percent_NDFL));
}

void tax_freelance (const int year, const int month)
{
    double percent_NDFL = ndfl_percent(oleg.salary_freelance);
    
    oleg.tax += oleg.salary_freelance*percent_NDFL;
}

void oleg_expenses (const int year, const int month)
{
    oleg.cash -= oleg.expenses_month;
}

void oleg_rent_home (const int year, const int month)
{
    oleg.cash -= oleg.rent_home;
}

void simulation()
{
    int year = 2028;
    int month = 7;
    while (not (year == 2029 and month == 7)) {

        
        oleg_profit(year, month);
        tax_job(year, month);
        tax_freelance(year, month);
        oleg_expenses(year, month);
        oleg_rent_home(year, month);
       
        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

void oleg_print()
{
    std::cout.imbue(std::locale(std::cout.getloc(), new ApostrophePunct));

    std::cout << "Oleg cash = " << oleg.cash << '\n';
    std::cout << "Oleg ndfl_tax = " << oleg.tax << '\n';
    std::cout << "Oleg capital = " << oleg.capital << '\n';
}

int main()
{
    oleg_init();

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