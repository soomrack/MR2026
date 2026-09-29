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

enum Category { food, utilities, rent, pet, car, salary, COUNT };

const double inflation_annual[COUNT] = {
    0.08,   // food
    0.07,   // utilities
    0.06,   // rent
    0.07,   // pet
    0.05,   // car
    0.07    // salary (индексация)
};


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

void danya_rent()
{
    danya.cash -= 10'000;
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


// void danya_food(const int year)
//{
    // double price = apply_inflation(5'000, food, year - 2026);
    // danya.cash -= (RUB)price;
//}


void simulation()
{
    int year = 2026;
    int month = 9;
    
    while (not (year == 2076 and month == 9)) {

        danya_salary(year, month);

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

        // danya_car();                                         // Задаем ТЗ                                    
        // danya_home_bills();                                  // Добавить вклады, налоги, инфлянцию
        // danya_food();
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