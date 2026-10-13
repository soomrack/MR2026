#include <iostream>
#include <string>
#include <locale>
#include <random>
#include <algorithm>
#include <cmath>

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
    RUB months_worked;
    RUB accumulation; // могут быть вклады в банке, накопления под процент
    RUB returnable_costs; // сумма расходов с которых можно сделать налоговый вычет
    RUB fitness_expenses;
    RUB tax; // сумма выплаченных государству налогов включая НДФЛ,на имущество, земельный, транспортный
    RUB summ_contribution_hypothec;
    RUB summ_hypothec;
    std::vector<std::string> history_purchase;
    std::string priority;


    void init();
    void whole_capital();  
    void profit_month(const int year, const int month);
    void tax_job(const int year, const int month);
    void tax_freelance(const int year, const int month);
    void expenses(const int year, const int month);
    void rent_home_month(const int year, const int month);
    void accumulation_deposit(const int year, const int month);
    void medical_expenses_month(const int year, const int month);
    void fitness_expenses_month (const int year, const int month);
    void returnable_costs_year (const int year, const int month);
    void new_buy (std::string purchase, int cost, const int year, const int month);
    void hypothec_month (const int year, const int month, const int time_month, const double credit_rate);
    void compensator(const int year, const int month);
};

struct Person oleg, eva;

// другие глобальные переменные
int date_year[2] = {-1, -1};
int date_month[2] = {-1, -1};

// функции
void Person::init()
{
    oleg.cash = 20'000;
    oleg.salary_job = 70'000;
    oleg.expenses_month = 40'000;
    oleg.rent_home = 40'000;
    oleg.fitness_expenses = 4'500;
    oleg.months_worked = 0;
    oleg.summ_hypothec = 12'000'000;
    oleg.summ_contribution_hypothec = 0;
}

void Person::whole_capital()
{
    capital =  cash + buy + accumulation;
}

void Person::new_buy (std::string purchase, int cost, const int year, const int month)
{
    std::string old_purchase;

    std::string value;
    int old_month;
    int old_year;

    bool exists = std::find(history_purchase.begin(), history_purchase.end(), purchase) != history_purchase.end();
    if (exists == false)
    {
        while (purchase != old_purchase and month != old_month and year != old_year)
        {
            if (cash >= cost)
            {
                cash -= cost;
                buy += cost;
                old_purchase = purchase;
                old_month = month;
                old_year = year;

                history_purchase.push_back(purchase);

                std::cout<< " purchase ok  " << purchase << " - " << month << " : "<< year << '\n'; 
            }

            else
            {
                old_month = month;
                old_year = year;
                std::cout<< "Insufficient funds to purchase  " << purchase<< " - " << month << " : "<< year << '\n';
            } 
        }
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

void Person::profit_month(const int year, const int month)
{
    months_worked++; 
        
        if (months_worked == 6 and salary_job<=200'000 ) {
            salary_job += 20'000; 
        }

        if (months_worked==12){
            months_worked = 0;
        }

    int min_salary_freelance = 5'000;
    int max_salary_freelance = 50'000;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min_salary_freelance,  max_salary_freelance);

    salary_freelance = distr(gen);

    double percent_NDFL = ndfl_percent(salary_freelance);
    profit = salary_job + salary_freelance*(1-percent_NDFL);

    cash += profit;
}

void Person::tax_job (const int year, const int month)
{
    double percent_NDFL = ndfl_percent(salary_job);

    tax += salary_job*(percent_NDFL/(1-percent_NDFL));
}

void Person::tax_freelance (const int year, const int month)
{
    double percent_NDFL = ndfl_percent(salary_freelance);
    
    tax += salary_freelance*percent_NDFL;
}

void Person::expenses (const int year, const int month)
{
    cash -= expenses_month;
}

void Person::rent_home_month (const int year, const int month)
{
    cash -= rent_home;
}

void Person::accumulation_deposit (const int year, const int month)
{
    double percent_deposit = 0.15;
    double percent_growth = 0.015;
    int n_deposit = 0;

    if(cash >= profit*percent_deposit){
    accumulation += profit*percent_deposit + accumulation*percent_growth;
    cash -= profit*percent_deposit;
    }
    if (cash >= 200'000)
    {
        accumulation += cash*percent_deposit;
        cash -= cash*percent_deposit;
    }
    //else (n_deposit += 1);
    //std::cout << "not deposit month = " << n_deposit << '\n';
    
}

void Person::hypothec_month(const int year, const int month, const int time_month, const double credit_rate)
{
    int payment = summ_hypothec * (credit_rate*pow((1+credit_rate),time_month))/(pow((1+credit_rate),time_month)-1);
    int down_payment = summ_hypothec * 0.3;
    int summ_finish_hypothec = payment * time_month;

    if ( priority == "hypothec" and summ_contribution_hypothec < summ_finish_hypothec) // потом заменить priority на динамический массив, чтобы вписывать и удалять новые приоритеты
    {
        if (accumulation >= down_payment and summ_contribution_hypothec == 0)
        {
            accumulation -= down_payment;
            summ_contribution_hypothec += down_payment;
            history_purchase.push_back("flat_hypothec");
        }
        else
        {
            cash -= payment;
            summ_contribution_hypothec += payment;
            buy += summ_hypothec/time_month;

            if (summ_contribution_hypothec >= summ_finish_hypothec)
            {
                history_purchase.push_back("flat");
                std::cout<< "finish_hypothec_flat  "<< month << " : "<< year << '\n';
            }
        }
    }
}

void Person::medical_expenses_month(const int year, const int month)
{
    int min_medical_expenses = 5'000/6;
    int max_medical_expenses = 80'000/6;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr( min_medical_expenses,  max_medical_expenses);

    RUB medical_expenses = distr(gen);
    returnable_costs += medical_expenses;
    cash -= medical_expenses;
}

void Person::fitness_expenses_month (const int year, const int month)
{
    cash -= fitness_expenses;
    returnable_costs += fitness_expenses;
}

void Person::returnable_costs_year (const int year, const int month)//добавить потом налоговый вычет на ипотеку и на покупку недвижки 
{
    if (month == 7 ){
        double percent_NDFL = ndfl_percent(profit);
        if (returnable_costs*percent_NDFL<= 15'600)
        {
        cash += returnable_costs*percent_NDFL;
        returnable_costs = 0;
        }

        else
        {
            cash += 15600;
            returnable_costs = 0;
        } 
    }
}

int total_month (const int year, const int month)
{
    if (date_year[1] == -1)
    {
        date_year[1] = year;
    }
    if (date_year[1] != -1)
    {
        date_year[2] = year;
    }

    if (date_month[1] == -1)
    {
        date_month[1] = month;
    }
    if (date_month[1] != -1)
    {
        date_month[2] = month;
    }

    int total_date = (date_year[2] - date_year[1]) * 12 + (date_month[2] - date_month[1]);

    return total_date;
}

void Person::compensator(const int year, const int month)
{
    while (cash < 40'000)
    {
        cash += 1;
        accumulation -= 1;
    }
}
void simulation()
{
    int year = 2028;
    int month = 7;
    while (not (year == 2038 and month == 7)) {

        
        oleg.profit_month(year, month);
        oleg.tax_job(year, month);
        oleg.tax_freelance(year, month);
        oleg.expenses(year, month);
        oleg.rent_home_month(year, month);
        
        oleg.medical_expenses_month(year, month);
        oleg.fitness_expenses_month (year, month);
        oleg.returnable_costs_year (year, month);
        oleg.whole_capital();

        oleg.accumulation_deposit(year, month);
        
        
        if(month == 12 and year == 2032 or year > 2032)
        {
            std::string purchase = {"laptop"};
            int cost = 90000;
            oleg.new_buy(purchase,cost,year,month);
        }

        if(month >= 6 and year >= 2030 or year > 2030)
        {
            std::string purchase = {"armchair"};
            int cost = 34000;
            oleg.new_buy(purchase,cost,year,month);
        }

        if(month >= 8 and year >= 2034 or year > 2034)
        {
            std::string purchase = {"shelf"};
            int cost = 40000;
            oleg.new_buy(purchase,cost,year,month);
        }

        if(month >= 10 and year >= 2029 or year > 2029)
        {
            std::string purchase = {"dishwasher"};
            int cost = 50000;
            oleg.new_buy(purchase,cost,year,month);
        }
        
        if(month >= 5 and year >= 2034 or year > 2034)
        {
            oleg.priority = "hypothec";
        }

        int time_month =180;
        double credit_rate = 0.155/12.0; //ставка по кредиту делить на 12 месяцев
        oleg.hypothec_month (year, month, time_month, credit_rate);

        oleg.compensator(year, month);

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

void print()
{
    std::cout.imbue(std::locale(std::cout.getloc(), new ApostrophePunct));

    std::cout << "Oleg cash = " << oleg.cash << '\n';
    std::cout << "Oleg ndfl_tax = " << oleg.tax << '\n';
    std::cout << "Oleg capital = " << oleg.capital << '\n';
    std::cout << "Oleg accum = " << oleg.accumulation << '\n';

    std::cout << "Oleg history_purchase ... " ;

    for (const std::string & item : oleg.history_purchase)
    {
        std::cout << item << ", ";
    }
}

int main()
{
    oleg.init();

    simulation();

    print();

}