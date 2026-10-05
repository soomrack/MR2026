#include <stdio.h>
#include <math.h>

using RUB = unsigned long long int;                             // Важна точка управления       Безнаковый тип прописываем (больше пустых ячеек- экономия памяти)

enum Strategy {mortgage, saving};

const Strategy strategy = saving;                           // mortgage или saving
// проверить обе стратегии

struct Loan {
    double principal;
    double remaining;
    double annual_rate;
    int months_total;
    int months_left;
    double monthly_payment;
    int month;
    double total_early_paid = 0;
};


struct Person {
    RUB cash;
    RUB salary;
    Loan mortgage;

    bool has_second_job;
    RUB salary_second_job;

    RUB deposit;
    bool owns_apartment;
    int purchase_year;
    int purchase_month;
};

struct Person danya;


enum Category { 
    food, 
    utilities, 
    rent, 
    pet, 
    car, 
    salary,
    realestate, 
    COUNT 
};


const double inflation_annual[COUNT] = {
    0.08,   // food
    0.07,   // utilities
    0.06,   // rent
    0.07,   // pet
    0.05,   // car
    0.07,   // salary (индексация)
    0.07    // realestate
};


struct DeepTopic {
    RUB    apartment_price;

    // ипотека
    RUB    mortgage_downpayment;
    RUB    mortgage_payment;
    double mortgage_rate;
    int    mortgage_months;
    int    dti_limit_percent;       // лимит DTI, например 70

    // досрочка
    RUB    early_repay_amount;      // сколько вносим раз в год
    RUB    early_repay_pad;         // неснижаемая подушка
    int    early_repay_month;       // в каком месяце (12 = декабрь)

    // вклад / стратегия saving
    RUB  monthly_expenses_base;     // еда + аренда в 2026 году
    RUB    apartment_price_base;    // цена квартиры в 2026
    double deposit_rate;            // годовая ставка по вкладу
    double deposit_tax;             // НДФЛ с процентов
    int    saving_pad_months;       // подушка в месяцах расходов
};


void deep_topic_init(DeepTopic& dc)
{
    dc.apartment_price = 10'000'000;
    
    // mortgage
    dc.mortgage_downpayment  = 2'000'000;
    dc.mortgage_payment      = 135'900;
    dc.mortgage_rate         = 0.20;
    dc.mortgage_months       = 240;
    dc.dti_limit_percent     = 70;

    dc.early_repay_amount    = 200'000;
    dc.early_repay_pad       = 1'500'000;
    dc.early_repay_month     = 12;

    dc.monthly_expenses_base = 35'000;   // 25к еда + 10к аренда
    // не редачил
    dc.apartment_price_base  = 10'000'000;
    dc.deposit_rate          = 0.10;
    dc.deposit_tax           = 0.13;
    dc.saving_pad_months     = 6;
    // до сюда
}


RUB apply_inflation_year(RUB value, Category C, int years_passed)
{
    double factor = pow(1.0 + inflation_annual[C], years_passed);
    return (RUB)((double)value * factor + 0.5);                                                 /// 05???
}


double loan_pay_month(Loan& loan)
{
    if (loan.months_left <= 0) {
        return 0;
    }

    double interest = loan.remaining * (loan.annual_rate / 12);
    double principal_part = loan.monthly_payment - interest;

    loan.remaining -= principal_part;
    loan.months_left -= 1;

    if (loan.remaining < 0) {
        loan.remaining = 0;
    }

    if (loan.months_left == 0 || loan.remaining < 0) {
        loan.remaining = 0;
    }

    return loan.monthly_payment;
}


void danya_salary(const int year, const int month)              // const - показываем, что переменная не меняется
{
    if (month == 1) {
        danya.salary = (RUB)((double)danya.salary * (1.0 + inflation_annual[salary]));
    }
    if(year == 2028 and month == 9) {                           //Promotion
        danya.salary = 120'000;
    }
    danya.cash += danya.salary;
}


void danya_setup_mortgage(RUB payment, DeepTopic& dt)
{
    danya.cash -= dt.mortgage_downpayment;

    danya.mortgage.months_total    = dt.mortgage_months;
    danya.mortgage.months_left     = dt.mortgage_months;
    danya.mortgage.monthly_payment = payment;
    danya.mortgage.annual_rate     = dt.mortgage_rate;
    danya.mortgage.remaining       = (double)(dt.apartment_price_base - dt.mortgage_downpayment);
}


void danya_mortgage()
{
    double to_pay = loan_pay_month(danya.mortgage);
    danya.cash -= (RUB)to_pay;
};


void danya_second_job(int year)
{
    if (danya.has_second_job) return;

    const int DECISION_YEAR = 2031;                             // 2026 + 5 лет
    if (year < DECISION_YEAR) return;

    if (danya.owns_apartment) return;
    if (danya.mortgage.months_total > 0) return;

    danya.has_second_job = true;
    danya.salary_second_job = 80'000;
}


void danya_salary_second_job(const int month)
{
    if (!danya.has_second_job) return;

    danya.cash += danya.salary_second_job;

    if (month == 1) {
        danya.salary_second_job = (RUB)((double)danya.salary_second_job * (1.0 + inflation_annual[salary]));
    }
}


bool danya_can_get_mortgage(RUB payment, DeepTopic& dt)
{
    RUB income = danya.salary + danya.salary_second_job;
    return 100 * payment <= dt.dti_limit_percent * income;
}


void danya_rent(int year)
{
    const RUB base_price = 10'000;
    RUB price = apply_inflation_year(base_price, rent, year - 2026);
    danya.cash -= price;
}


void danya_life_mortgage(int year, DeepTopic& dt)
{
    const RUB reserv = 1'000'000;

    const RUB mortgage_threshold = dt.mortgage_downpayment + reserv;

    if (danya.mortgage.months_total == 0) {
        if (danya.cash >= mortgage_threshold &&
            danya_can_get_mortgage(dt.mortgage_payment, dt)) {
            danya_setup_mortgage(dt.mortgage_payment, dt);
        } else {
            danya_rent(year);                                       // пока копим — снимаем
        }
    } else {
        danya_mortgage();                                           // ипотека есть — платим
    }
}


double loan_early_repay(Loan& l, double extra)
{
    if (l.months_left <= 0) return 0;

    if (extra <= 0) return 0;

    if (extra > l.remaining) extra = l.remaining;

    l.remaining -= extra;
    l.total_early_paid += extra;

    if (l.remaining <= 0) {
        l.remaining = 0;
        l.months_left = 0;
        return extra;
    }

    double r = l.annual_rate / 12.0;
    double inner = 1.0 - l.remaining * r / l.monthly_payment;

    if (inner <= 0) return extra;

    double n = -log(inner) / log(1.0 + r);
    l.months_left = (int)ceil(n);

    return extra;
}


void danya_early_repay_maybe(int month, DeepTopic& dt)
{
    if (month != dt.early_repay_month) return;
    if (danya.mortgage.months_left <= 0) return;   // кредит закрыт
    if (danya.cash < dt.early_repay_pad + dt.early_repay_amount) return;  // мало денег

    double paid = loan_early_repay(danya.mortgage, dt.early_repay_amount);

    danya.cash -= (RUB)paid;
}


void danya_saving_deposit(int year, DeepTopic& dt)
{
    RUB monthly_now = apply_inflation_year(dt.monthly_expenses_base, food, year - 2026);
    RUB pad = monthly_now * dt.saving_pad_months;

    if (danya.cash <= pad) return;

    RUB free = danya.cash - pad;
    danya.deposit += free;
    danya.cash = pad;
}


void danya_deposit_income(DeepTopic& dt)
{
    if (danya.deposit == 0) return;

    double interest_gross = (double)danya.deposit * dt.deposit_rate;
    double interest_net   = interest_gross * (1.0 - dt.deposit_tax);

    danya.deposit += (RUB)(interest_net + 0.5);
}


bool danya_try_buy_apartment(int year, int month, DeepTopic& dt)
{
    if (danya.owns_apartment) return false;

    RUB price_now = apply_inflation_year(dt.apartment_price_base, realestate, year - 2026);

    if (danya.deposit < price_now) return false;

    danya.deposit -= price_now;
    danya.owns_apartment = true;
    danya.purchase_year = year;
    danya.purchase_month = month;

    printf(">>> BOUGHT %d-%02d for %llu RUB (leftover %llu)\n",             // Для отладки
       year, month, price_now, danya.deposit);

    danya.cash += danya.deposit;
    danya.deposit = 0;

    return true;
}


void danya_life_saving(int year, int month, DeepTopic& dt)
{
    danya_rent(year);

    if (month == 1) {
        danya_deposit_income(dt);
    }

    danya_saving_deposit(year, dt);
    danya_try_buy_apartment(year, month, dt);
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


void simulation(DeepTopic& dt)
{
    int year = 2026;
    int month = 9;
    
    while (not (year == 2076 and month == 9)) {

        danya_salary(year, month);                                         // Задаем ТЗ

        danya_second_job(year);

        danya_salary_second_job(month);

        if (strategy == mortgage) {
            danya_life_mortgage(year, dt);
            danya_early_repay_maybe(month, dt);
        } else {
            danya_life_saving(year, month, dt);
            if (danya.owns_apartment) break;
        }

        // danya_car();                                    
        danya_food(year);                                                   // Добавить вклады, налоги, инфлянцию
        danya_home_bills(year);
        // danya_dog();
        // danya_bank_income();

        if (month == 9) {
        printf("%d-%02d: cash=%llu, deposit=%llu, owns=%d, mortgage=%d, remaining=%.0f, months_left=%d\n",
        year, month, danya.cash, danya.deposit, danya.owns_apartment,
        danya.mortgage.months_total, danya.mortgage.remaining,
        danya.mortgage.months_left);
}

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

    danya.deposit = 0;
    danya.owns_apartment = false;
    danya.purchase_year = 0;
    danya.purchase_month = 0;
}

void danya_print()
{
    printf("Danya cash = %llu\n", danya.cash);
    if (danya.owns_apartment) {
        printf("Apartment bought: %d-%02d\n",
               danya.purchase_year, danya.purchase_month);
    }
}

int main()
{
    DeepTopic dt;

    deep_topic_init(dt);

    danya_init();

    simulation(dt);

    danya_print();
}