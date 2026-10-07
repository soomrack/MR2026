#include <stdio.h>
#include <math.h>

using RUB = unsigned long long int;                             // Важна точка управления       Безнаковый тип прописываем (больше пустых ячеек- экономия памяти)

enum Strategy {mortgage, saving};

const Strategy strategy = mortgage;                           // mortgage или saving
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

struct FoodExpenses {
    RUB groceries;
    RUB eating_out;
    RUB fast_food;
    RUB delivery;
    RUB coffee;
    RUB fruits;
    RUB vegetables;
    RUB meat;
    RUB fish;
    RUB dairy;
    RUB bread;
    RUB sweets;
    RUB water_juices;
    RUB snacks;
    RUB frozen;
    RUB canned;  
    RUB spices;
    RUB baby_food;
};


struct Car {
    bool owns;

    RUB purchase_price_base;
    RUB fuel_monthly_base;
    RUB maintenance_monthly_base;
    RUB insurance_annual_base;

    int purchase_year;
    int purchase_month;

    RUB total_spent;
};


struct Person {
    RUB cash;
    RUB salary;
    Loan mortgage;

    FoodExpenses food;
    Car car;

    bool has_second_job;
    RUB salary_second_job;

    RUB deposit;
    bool owns_apartment;
    int purchase_year;
    int purchase_month;

    RUB total_earned;
    RUB total_spent;
    RUB total_mortgage_interest;
    RUB total_deposit_interest;
    RUB apartment_price_paid;                               // за сколько куплена квартира
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


void danya_food_init()
{
    danya.food.groceries    = 12'000;
    danya.food.eating_out   =  3'500;
    danya.food.fast_food    =  1'000;
    danya.food.delivery     =    800;
    danya.food.coffee       =    600;
    danya.food.fruits       =    900;
    danya.food.vegetables   =    700;
    danya.food.meat         =  2'000;
    danya.food.fish         =  1'200;
    danya.food.dairy        =    900;
    danya.food.bread        =    400;
    danya.food.sweets       =    300;
    danya.food.water_juices =    500;
    danya.food.snacks       =    400;
    danya.food.frozen       =    500;
    danya.food.canned       =    200;
    danya.food.spices       =    150;
    danya.food.baby_food    =      0;
}


void danya_car_init()
{
    danya.car.owns = false;

    danya.car.purchase_price_base = 1'500'000;

    danya.car.fuel_monthly_base = 8'000;
    danya.car.maintenance_monthly_base = 3'000;
    danya.car.insurance_annual_base = 40'000;

    danya.car.purchase_year = 0;
    danya.car.purchase_month = 0;

    danya.car.total_spent = 0;
}


RUB apply_inflation_year(RUB value, Category C, int years_passed)
{
    double factor = pow(1.0 + inflation_annual[C], years_passed);
    return (RUB)((double)value * factor + 0.5);                                                 /// 05???
}


RUB calculate_annuity_payment(RUB loan_amount, double annual_rate, int months)
{
    double monthly_rate = annual_rate / 12.0;

    double factor = pow(1.0 + monthly_rate, months);

    double payment = (double)loan_amount * monthly_rate * factor / (factor - 1.0);

    return (RUB)(payment + 0.5);
}


double loan_pay_month(Loan& loan)
{
    if (loan.months_left <= 0 || loan.remaining <= 0) {
        return 0;
    }

    double interest = loan.remaining * (loan.annual_rate / 12.0);

    double payment = loan.monthly_payment;

    double final_payment = loan.remaining + interest;

    if (payment > final_payment) {
        payment = final_payment;
    }

    double principal_part = payment - interest;

    loan.remaining -= principal_part;
    loan.months_left -= 1;

    if (loan.remaining <= 0) {
        loan.remaining = 0;
        loan.months_left = 0;
    }

    return payment;
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
    danya.total_earned += danya.salary;
}


void danya_setup_mortgage(RUB payment,RUB downpayment,int year,DeepTopic& dt)
{
    RUB apartment_price = apply_inflation_year(
        dt.apartment_price_base,
        realestate,
        year - 2026
    );

    danya.cash -= downpayment;
    danya.total_spent += downpayment;

    danya.mortgage.months_total    = dt.mortgage_months;
    danya.mortgage.months_left     = dt.mortgage_months;
    danya.mortgage.monthly_payment = payment;
    danya.mortgage.annual_rate     = dt.mortgage_rate;

    danya.mortgage.remaining = (double)(apartment_price - downpayment);

    danya.apartment_price_paid = apartment_price;
    danya.owns_apartment = true;
}


void danya_mortgage()
{
    if (danya.mortgage.months_left <= 0) return;

    double interest = danya.mortgage.remaining * (danya.mortgage.annual_rate / 12);
    danya.total_mortgage_interest += (RUB)(interest + 0.5);

    double to_pay = loan_pay_month(danya.mortgage);

    danya.cash        -= (RUB)to_pay;
    danya.total_spent += (RUB)to_pay;
}


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
    danya.total_earned += danya.salary_second_job;

    if (month == 1) {
        danya.salary_second_job = (RUB)((double)danya.salary_second_job * (1.0 + inflation_annual[salary]));
    }
}


bool danya_can_get_mortgage(RUB payment, DeepTopic& dt)
{
    RUB income = danya.salary + danya.salary_second_job;
    return 100 * payment <= dt.dti_limit_percent * income;
}


RUB danya_rent_monthly_base()
{
    return 10'000;   // аренда в ценах 2026
}

RUB danya_food_monthly_base()
{
    return danya.food.groceries
         + danya.food.eating_out
         + danya.food.fast_food
         + danya.food.delivery
         + danya.food.coffee
         + danya.food.fruits
         + danya.food.vegetables
         + danya.food.meat
         + danya.food.fish
         + danya.food.dairy
         + danya.food.bread
         + danya.food.sweets
         + danya.food.water_juices
         + danya.food.snacks
         + danya.food.frozen
         + danya.food.canned
         + danya.food.spices
         + danya.food.baby_food;
}

void danya_rent(int year)
{
    RUB price = apply_inflation_year(danya_rent_monthly_base(), rent, year - 2026);
    danya.cash        -= price;
    danya.total_spent += price;
}


void danya_life_mortgage(int year, int month, DeepTopic& dt)
{
    const RUB reserv = 1'000'000;

    if (danya.mortgage.months_total == 0) {

        RUB apartment_price = apply_inflation_year(
            dt.apartment_price_base,
            realestate,
            year - 2026
        );

        if (danya.cash < dt.mortgage_downpayment + reserv) {
            danya_rent(year);
            return;
        }

        RUB downpayment = danya.cash - reserv;

        if (downpayment > apartment_price) {
            downpayment = apartment_price;
        }

        RUB loan_amount =
            apartment_price - downpayment;

        RUB mortgage_payment = calculate_annuity_payment(
            loan_amount,
            dt.mortgage_rate,
            dt.mortgage_months
        );

        if (danya_can_get_mortgage(mortgage_payment, dt)) {

            danya_setup_mortgage(
                mortgage_payment,
                downpayment,
                year,
                dt
            );

            danya.purchase_year  = year;
            danya.purchase_month = month;

        } else {
            danya_rent(year);
        }

    } else {
        danya_mortgage();
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
    danya.total_spent += (RUB)paid;
}


void danya_saving_deposit(int year, DeepTopic& dt)
{
    RUB food_now = apply_inflation_year(danya_food_monthly_base(), food, year - 2026);
    RUB rent_now = apply_inflation_year(danya_rent_monthly_base(), rent, year - 2026);
    RUB monthly_now = food_now + rent_now;

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
    RUB    earned         = (RUB)(interest_net + 0.5);

    danya.deposit                += earned;
    danya.total_deposit_interest += earned;
}


bool danya_try_buy_apartment(int year, int month, DeepTopic& dt)
{
    if (danya.owns_apartment) return false;

    RUB price_now = apply_inflation_year(dt.apartment_price_base, realestate, year - 2026);

    if (danya.deposit < price_now) return false;

    danya.deposit -= price_now;
    danya.total_spent += price_now;

    danya.owns_apartment = true;
    danya.purchase_year = year;
    danya.purchase_month = month;
    danya.apartment_price_paid = price_now;

    printf(">>> BOUGHT %d-%02d for %llu RUB (leftover %llu)\n",             // Для отладки
       year, month, price_now, danya.deposit);

    danya.cash += danya.deposit;
    danya.deposit = 0;

    return true;
}


void danya_life_saving(int year, int month, DeepTopic& dt)
{
    if (!danya.owns_apartment) {
        danya_rent(year);
    }

    if (month == 1) {
        danya_deposit_income(dt);
    }

    danya_saving_deposit(year, dt);

    if (!danya.owns_apartment) {
        danya_try_buy_apartment(year, month, dt);
    }
}


void danya_food(int year)
{
    RUB base  = danya_food_monthly_base();
    RUB price = apply_inflation_year(base, food, year - 2026);

    danya.cash        -= price;
    danya.total_spent += price;
}


void danya_car(int year, int month)
{
    const int car_wanted_year = 2033;
    const RUB reserve = 1'000'000;

    if (!danya.car.owns) {

        if (year < car_wanted_year) {
            return;
        }

        RUB car_price = apply_inflation_year(
            danya.car.purchase_price_base,
            car,
            year - 2026
        );

        RUB available_money = danya.cash + danya.deposit;

        if (available_money < car_price + reserve) {
            return;
        }

        if (danya.cash >= car_price) {
            danya.cash -= car_price;
        } else {
            RUB from_deposit = car_price - danya.cash;

            danya.cash = 0;
            danya.deposit -= from_deposit;
        }

        danya.car.owns = true;
        danya.car.purchase_year = year;
        danya.car.purchase_month = month;

        danya.car.total_spent += car_price;
        danya.total_spent += car_price;
    }

    RUB fuel = apply_inflation_year(
        danya.car.fuel_monthly_base,
        car,
        year - 2026
    );

    RUB maintenance = apply_inflation_year(
        danya.car.maintenance_monthly_base,
        car,
        year - 2026
    );

    RUB monthly_car_expenses = fuel + maintenance;

    danya.cash -= monthly_car_expenses;

    danya.car.total_spent += monthly_car_expenses;
    danya.total_spent += monthly_car_expenses;

    if (month == danya.car.purchase_month) {

        RUB insurance = apply_inflation_year(
            danya.car.insurance_annual_base,
            car,
            year - 2026
        );

        danya.cash -= insurance;

        danya.car.total_spent += insurance;
        danya.total_spent += insurance;
    }
}


void danya_home_bills(int year)
{
    if (danya.mortgage.months_total == 0 && !danya.owns_apartment) return;

    const RUB base_price = 8'000;
    RUB price = apply_inflation_year(base_price, utilities, year - 2026);
    
    danya.cash -= price;
    danya.total_spent += price;      // ← добавь
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
            danya_life_mortgage(year, month, dt);
            danya_early_repay_maybe(month, dt);
        } else {
            danya_life_saving(year, month, dt);
        }

        danya_car(year, month);                                    
        danya_food(year);                                                   // Добавить вклады, налоги, инфлянцию
        danya_home_bills(year);
        // danya_dog();
        // danya_bank_income();

        /*
        if (month == 9) {
        printf("%d-%02d: cash=%llu, deposit=%llu, owns=%d, mortgage=%d, remaining=%.0f, months_left=%d\n",
        year, month, danya.cash, danya.deposit, danya.owns_apartment,
        danya.mortgage.months_total, danya.mortgage.remaining,
        danya.mortgage.months_left);
        }
        */

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}


void danya_init()
{
    danya = Person{};

    danya.cash = 20'000;
    danya.salary = 80'000;

    danya.deposit = 0;
    danya.owns_apartment = false;
    danya.purchase_year = 0;
    danya.purchase_month = 0;

    danya.total_earned = 0;
    danya.total_spent = 0;
    danya.total_mortgage_interest = 0;
    danya.total_deposit_interest = 0;
    danya.apartment_price_paid = 0;

    danya_food_init();
    danya_car_init();
}


void danya_print()                                          // не используется
{
    printf("Danya cash = %llu\n", danya.cash);
    if (danya.owns_apartment) {
        printf("Apartment bought: %d-%02d\n",
               danya.purchase_year, danya.purchase_month);
    }
}


void danya_print_report(DeepTopic& dt)
{
    printf("\n=============== REPORT ===============\n");
    printf("Strategy:          %s\n",
           strategy == mortgage ? "mortgage" : "saving");

    printf("Final cash:        %llu\n", danya.cash);
    printf("Final deposit:     %llu\n", danya.deposit);

    if (danya.owns_apartment) {
        printf("Apartment bought:  %d-%02d for %llu\n",
               danya.purchase_year, danya.purchase_month,
               danya.apartment_price_paid);
    } else if (danya.mortgage.months_total > 0 && danya.mortgage.months_left == 0) {
        printf("Apartment:         own (mortgage paid off)\n");
    } else {
        printf("Apartment:         none\n");
    }

    if (danya.car.owns) {
    printf("Car bought:        %d-%02d\n",
           danya.car.purchase_year,
           danya.car.purchase_month);

    printf("Car total spent:   %llu\n",
           danya.car.total_spent);
    } else {
        printf("Car:               none\n");
    }

    printf("Total earned:      %llu\n", danya.total_earned);
    printf("Total spent:       %llu\n", danya.total_spent);

    if (strategy == mortgage) {
        printf("Interest paid:     %llu\n", danya.total_mortgage_interest);
        printf("Early repaid:      %.0f\n", danya.mortgage.total_early_paid);
    } else {
        printf("Interest earned:   %llu\n", danya.total_deposit_interest);
    }

    // Net worth = cash + deposit + квартира − долг
    RUB apt_now = 0;
    bool has_apartment = danya.owns_apartment ||
        (danya.mortgage.months_total > 0 && danya.mortgage.months_left == 0);

    if (has_apartment) {
        apt_now = apply_inflation_year(dt.apartment_price_base, realestate, 2076 - 2026);
    }

    RUB net = danya.cash + danya.deposit + apt_now;
    if (danya.mortgage.months_left > 0) {
        net -= (RUB)danya.mortgage.remaining;
    }

    printf("Net worth:         %llu\n", net);
    printf("======================================\n");
}


int main()
{
    DeepTopic dt;
    deep_topic_init(dt);
    danya_init();

    simulation(dt);

    //danya_print();
    danya_print_report(dt);

    return 0;
}