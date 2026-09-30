#include <stdio.h>
#include <math.h>

using RUB = unsigned long long int;                             // Важна точка управления       Безнаковый тип прописываем (больше пустых ячеек- экономия памяти)

struct Loan {
    double principal;
    double remaining;
    double annual_rate;
    int months_total;
    int months_left;
    double monthly_payment;
    int month;
};

struct Person {
    RUB cash;
    RUB salary;
    Loan mortgage;
    bool second_job;
    RUB salary_second_job;
};

struct Person danya;

enum Strategy {mortgage, saving};

const Strategy strategy = mortgage;                           // mortgage или saving

enum Category { 
    food, 
    utilities, 
    rent, 
    pet, 
    car, 
    salary, 
    COUNT 
};

struct Config {
    // стартовые
    int start_year, start_month;
    RUB start_cash;
    RUB start_salary;
    
    // карьера
    int promotion_year, promotion_month;
    RUB promotion_salary;
    RUB second_job_threshold;
    RUB second_job_income;
    
    // ипотека
    RUB mortgage_threshold;
    RUB mortgage_payment;
    RUB mortgage_principal;
    RUB mortgage_downpayment;
    double mortgage_rate;
    int mortgage_months;
    int dti_limit_percent;
    
    // аренда
    RUB rent_monthly;
    
    // инфляция
    double inflation_annual[COUNT];
    
    // служебное
    int seed;
    int max_months;
};


const double inflation_annual[COUNT] = {
    0.08,   // food
    0.07,   // utilities
    0.06,   // rent
    0.07,   // pet
    0.05,   // car
    0.07    // salary (индексация)
};


RUB apply_inflation_year(RUB value, Category C, int years_passed)
{
    double factor = pow(1.0 + inflation_annual[C], years_passed);
    return (RUB)((double)value * factor + 0.5);
}


double loan_pay_month(Loan& L)
{
    if (L.months_left <= 0) {
        return 0;
    }

    double interest = L.remaining * (L.annual_rate / 12);
    double principal_part = L.monthly_payment - interest;

    L.remaining -= principal_part;
    L.months_left -= 1;

    if (L.remaining < 0) {
        L.remaining = 0;
    }

    if (L.months_left == 0 || L.remaining < 0) {
        L.remaining = 0;
    }

    return L.monthly_payment;
}


void danya_salary(const int year, const int month)              // const - показываем, что переменная не меняется
{
    if (month == 1) {
        danya.salary = (RUB)((double)danya.salary * (1.0 + inflation_annual[salary]));
    }
    if(year == 2028 and month == 9) {   //Promotion
        danya.salary = 120'000;
    }
    danya.cash += danya.salary;
}


void danya_setup_mortgage(RUB payment)
{
    danya.cash -= 2000000;

    danya.mortgage.remaining = 8000000;
    danya.mortgage.annual_rate = 0.20;
    danya.mortgage.months_total = 240;
    danya.mortgage.months_left = 240;

    danya.mortgage.monthly_payment = payment;
}


void danya_mortgage()
{
    double to_pay = loan_pay_month(danya.mortgage);
    danya.cash -= (RUB)to_pay;
};


void danya_second_job()
{
    if (danya.second_job) return;

    if (danya.cash >= 1'500'000) {
        danya.second_job = 1;
        danya.salary_second_job = 80'000;
    }
}


void danya_salary_second_job()
{
    if (!danya.second_job) return;
    danya.cash += danya.salary_second_job;
}


bool danya_can_get_mortgage(RUB payment)
{
    RUB income = danya.salary + danya.salary_second_job;
    return 10 * payment <= 7 * income;
}


void danya_rent(int year)
{
    const RUB base_price = 10'000;
    RUB price = apply_inflation_year(base_price, rent, year - 2026);
    danya.cash -= price;
}


void danya_life_mortgage()
{
    const RUB mortgage_threshold = 2'300'000;
    const RUB mortgage_payment   = 135'900;

    if (danya.mortgage.months_total == 0) {
        if (danya.cash >= mortgage_threshold &&
            danya_can_get_mortgage(mortgage_payment)) {
            danya_setup_mortgage(mortgage_payment);
        } else {
            danya_rent();      // пока копим — снимаем
        }
    } else {
        danya_mortgage();      // ипотека есть — платим
    }
}


void danya_life_saving()
{
    danya_rent();
    // тут еще будет вклад
}


void danya_food(int year)
{
    const RUB base_price = 25'000;
    RUB price = apply_inflation_year(base_price, food, year - 2026);
    danya.cash -= price;
}


void danya_home_bills(int year)
{
    if (danya.mortgage.months_total == 0) return;

    const RUB base_price = 8'000;
    RUB price = apply_inflation_year(base_price, utilities, year - 2026);
    danya.cash -= price;
}


void simulation()
{
    int year = 2026;
    int month = 9;
    
    while (not (year == 2076 and month == 9)) {

        danya_salary(year, month);                                         // Задаем ТЗ

        danya_second_job();

        danya_salary_second_job();

        if (month == 9) {                                                           // для определения года начала ипотеки - потом уберу
            printf("%d-%02d: cash=%llu, mortgage=%d, remaining=%.0f, months_left=%d\n",
                year, month, danya.cash,
                danya.mortgage.months_total,
                danya.mortgage.remaining,
                danya.mortgage.months_left);
        }

        if (strategy == mortgage) {
            danya_life_mortgage();
        } else {
            danya_life_saving();
        }

        // danya_car();                                    
        danya_food(year);                                       // Добавить вклады, налоги, инфлянцию
        danya_home_bills(year);
        // danya_dog();
        // danya_bank_income();

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

void danya_init()
{
    danya.cash = 20'000;
    danya.salary = 80'000;
}

void danya_print()
{
    printf("Danya cash = %llu\n", danya.cash);
}

int main()

{
    danya_init();

    simulation();

    danya_print();
}