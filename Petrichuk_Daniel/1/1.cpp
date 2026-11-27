#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using RUB = unsigned long long int;                         // Денежные значения в рублях

enum Strategy {mortgage, saving};

enum LoanType {
    no_loan, 
    mortgage_loan, 
    car_loan, 
    consumer_loan
};

const Strategy strategy = mortgage;                         // mortgage или saving

struct Loan {
    double principal;
    double remaining;
    double annual_rate;
    int months_total;
    int months_left;
    double monthly_payment;
    double total_early_paid = 0;
};


struct ConsumerCredit {
    bool expense_required;
    bool expense_paid;
    bool loan_taken;

    RUB expense_amount;
    RUB reserve;

    double loan_rate;
    int loan_months;

    int event_year;
    int event_month;

    int loan_start_year;
    int loan_start_month;
    int loan_close_year;
    int loan_close_month;

    int bank_refusals;

    RUB total_loan_interest;

    Loan loan;
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
    bool bought_on_credit;

    RUB purchase_price_base;
    RUB purchase_price_paid;

    RUB fuel_monthly_base;
    RUB maintenance_monthly_base;
    RUB insurance_annual_base;

    int purchase_year;
    int purchase_month;

    int loan_downpayment_percent;                               // Параметры по автокредиту
    double loan_rate;
    int loan_months;

    RUB loan_downpayment_paid;
    RUB total_loan_interest;

    int loan_close_year;
    int loan_close_month;

    Loan loan;

    RUB total_spent;
};


struct Pet {
    bool had_pet;
    bool alive;

    RUB purchase_price_base;
    RUB food_monthly_base;
    RUB vet_annual_base;

    int lifetime_months;
    int age_months;

    int purchase_year;
    int purchase_month;

    int death_year;
    int death_month;

    RUB total_spent;
};


struct Taxes {
    RUB wages_this_year;
    RUB wages_tax_this_year;
    RUB regular_net_this_month;
    RUB deposit_interest_this_year;

    RUB housing_base_used;
    RUB mortgage_interest_base_used;
    RUB refund_due_in_april;
    RUB deposit_tax_due_in_november;

    RUB total_wages_tax;
    RUB total_deposit_tax_paid;
    RUB total_refund;
};

struct Person {
    long long cash;  // Остаток может стать отрицательным после списания расходов.
    RUB salary;
    Loan mortgage;
    ConsumerCredit consumer_credit;

    FoodExpenses food;
    Car car;
    Pet pet;

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

    bool mortgage_refinanced;
    int refinance_year;
    int refinance_month;

    double mortgage_rate_before_refinance;
    RUB mortgage_payment_before_refinance;
    RUB refinance_cost_paid;

    int mortgage_close_year;
    int mortgage_close_month;

    RUB mortgage_balance_at_refinance;
    RUB refinance_projected_savings;

    Taxes taxes;

    int random_car_repairs;
    int random_pet_vet_visits;
    int random_home_repairs;

    RUB random_car_repairs_spent;
    RUB random_pet_vet_spent;
    RUB random_home_repairs_spent;
    RUB total_random_expenses;

    int career_break_months_left;
    int career_break_count;
    int career_break_months_total;

    int career_bonus_count;
    RUB career_bonus_total;
};

struct Person danya;

enum Category { 
    food, 
    utilities, 
    rent, 
    pet, 
    car, 
    salary,
    realestate,                                                 // Недвижимость
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



double inflation_random_factor[COUNT] = {                       // Годовое отклонение от базовой инфляции.
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

double inflation_current_rate[COUNT] = {
    0.08, 0.07, 0.06, 0.07, 0.05, 0.07, 0.07
};


void random_inflation_update(int year)
{
    if (year <= 2026) return;

    for (int i = 0; i < COUNT; ++i) {
        int shift_percent = rand() % 5 - 2;                     // от -2 до +2 процентных пунктов

        if (i == salary) {                                      // Зарплата с обычной индексацией, чтобы случайность влияла именно на цены
            inflation_current_rate[i] = inflation_annual[i];
            continue;
        }

        double actual_rate = inflation_annual[i] + shift_percent / 100.0;
        if (actual_rate < 0) actual_rate = 0;

        inflation_current_rate[i] = actual_rate;

        inflation_random_factor[i] *=                           // Отличие реальной инфляции за прошедшие годы от базовой
            (1.0 + actual_rate) / (1.0 + inflation_annual[i]);
    }
}


struct DeepTopic {

    RUB    apartment_price_base;

    // ипотека
    RUB    mortgage_downpayment;
    double mortgage_rate;
    int    mortgage_months;
    int    dti_limit_percent;                                   // лимит DTI

    // рефинансирование ипотеки
    double refinance_rate;
    int refinance_start_year;
    RUB refinance_cost;

    // досрочка
    RUB    early_repay_amount;
    RUB    early_repay_pad;
    int    early_repay_month;

    // вклад / стратегия saving
    double deposit_rate;
    double assumed_key_rate;                                     // Допущение для годового лимита по вкладу
    int    saving_pad_months;

    double deposit_rate_min;
    double deposit_rate_max;
    int deposit_rate_changes;
};


void deep_topic_init(DeepTopic& dc)
{
    dc.apartment_price_base  = 10'000'000;

    // mortgage
    dc.mortgage_downpayment  = 2'000'000;
    dc.mortgage_rate         = 0.20;
    dc.mortgage_months       = 240;
    dc.dti_limit_percent     = 70;

    dc.refinance_rate = 0.12;
    dc.refinance_start_year = 2037;
    dc.refinance_cost = 50'000;

    dc.early_repay_amount    = 200'000;
    dc.early_repay_pad       = 1'500'000;
    dc.early_repay_month     = 12;

    dc.deposit_rate          = 0.10;
    dc.assumed_key_rate      = 0.10;
    dc.saving_pad_months     = 6;

    dc.deposit_rate_min      = dc.deposit_rate;
    dc.deposit_rate_max      = dc.deposit_rate;
    dc.deposit_rate_changes  = 0;
}



void deposit_rate_update(int year, DeepTopic& dt)
{
    double base_rate = 0.10;

    if (year >= 2031) base_rate = 0.08;
    if (year >= 2041) base_rate = 0.07;
    if (year >= 2051) base_rate = 0.06;

    int shift_percent = rand() % 3 - 1;                         // Небольшое отклонение от базовой ставки
    double new_rate = base_rate + shift_percent / 100.0;

    if (new_rate < 0.03) new_rate = 0.03;

    if (fabs(new_rate - dt.deposit_rate) > 0.0001) {
        dt.deposit_rate_changes++;
    }

    dt.deposit_rate = new_rate;

    if (dt.deposit_rate < dt.deposit_rate_min) {
        dt.deposit_rate_min = dt.deposit_rate;
    }

    if (dt.deposit_rate > dt.deposit_rate_max) {
        dt.deposit_rate_max = dt.deposit_rate;
    }
}


void danya_food_init()
{
    danya.food.groceries = 12'000;
    danya.food.eating_out = 3'500;
    danya.food.fast_food = 1'000;
    danya.food.delivery = 800;
    danya.food.coffee = 600;
    danya.food.fruits = 900;
    danya.food.vegetables = 700;
    danya.food.meat = 2'000;
    danya.food.fish = 1'200;
    danya.food.dairy = 900;
    danya.food.bread = 400;
    danya.food.sweets =  300;
    danya.food.water_juices = 500;
    danya.food.snacks = 400;
    danya.food.frozen = 500;
    danya.food.canned = 200;
    danya.food.spices = 150;
    danya.food.baby_food = 0;
}


void danya_car_init()
{
    danya.car.owns = false;
    danya.car.bought_on_credit = false;

    danya.car.purchase_price_base = 1'500'000;
    danya.car.purchase_price_paid = 0;

    danya.car.fuel_monthly_base = 8'000;
    danya.car.maintenance_monthly_base = 3'000;
    danya.car.insurance_annual_base = 40'000;

    danya.car.purchase_year = 0;
    danya.car.purchase_month = 0;

    danya.car.loan_downpayment_percent = 30;                 // первоначальный взнос 30%
    danya.car.loan_rate = 0.18;
    danya.car.loan_months = 60;

    danya.car.loan_downpayment_paid = 0;
    danya.car.total_loan_interest = 0;

    danya.car.loan_close_year = 0;
    danya.car.loan_close_month = 0;

    danya.car.loan = Loan{};

    danya.car.total_spent = 0;
}


void danya_consumer_credit_init()
{
    danya.consumer_credit.expense_required = false;
    danya.consumer_credit.expense_paid = false;
    danya.consumer_credit.loan_taken = false;

    danya.consumer_credit.expense_amount = 500'000;
    danya.consumer_credit.reserve = 1'500'000;

    danya.consumer_credit.loan_rate = 0.28;
    danya.consumer_credit.loan_months = 36;

    danya.consumer_credit.event_year = 2036;
    danya.consumer_credit.event_month = 6;

    danya.consumer_credit.loan_start_year = 0;
    danya.consumer_credit.loan_start_month = 0;
    danya.consumer_credit.loan_close_year = 0;
    danya.consumer_credit.loan_close_month = 0;

    danya.consumer_credit.bank_refusals = 0;
    danya.consumer_credit.total_loan_interest = 0;

    danya.consumer_credit.loan = Loan{};
}


void danya_pet_init()
{
    danya.pet.had_pet = false;
    danya.pet.alive = false;

    danya.pet.purchase_price_base = 30'000;
    danya.pet.food_monthly_base = 4'000;
    danya.pet.vet_annual_base = 15'000;

    danya.pet.lifetime_months = 12 * 12;                            // 12 * 12 = 144 месяца жизни
    danya.pet.age_months = 0;

    danya.pet.purchase_year = 0;
    danya.pet.purchase_month = 0;

    danya.pet.death_year = 0;
    danya.pet.death_month = 0;

    danya.pet.total_spent = 0;
}


RUB apply_inflation_year(RUB value, Category category, int years_passed)
{
    double factor = pow(1.0 + inflation_annual[category], years_passed);
    factor *= inflation_random_factor[category];

    return (RUB)((double)value * factor + 0.5);                     // +0.5 для округления до целого рубля
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


// Ставки и пороги модели фиксированы на все 50 лет.
RUB wages_tax(RUB annual_income)
{
    // Добавка по каждой ступени начисляется только на превышение порога.
    RUB tax_kopecks = annual_income * 13;

    if (annual_income > 2'400'000) {
        tax_kopecks += (annual_income - 2'400'000) * 2;
    }
    if (annual_income > 5'000'000) {
        tax_kopecks += (annual_income - 5'000'000) * 3;
    }
    if (annual_income > 20'000'000) {
        tax_kopecks += (annual_income - 20'000'000) * 2;
    }
    if (annual_income > 50'000'000) {
        tax_kopecks += (annual_income - 50'000'000) * 2;
    }

    return (tax_kopecks + 50) / 100;
}


RUB receive_work_income(RUB gross)
{
    Taxes& taxes = danya.taxes;
    taxes.wages_this_year += gross;

    RUB year_tax = wages_tax(taxes.wages_this_year);
    RUB withheld = year_tax - taxes.wages_tax_this_year;
    taxes.wages_tax_this_year = year_tax;
    taxes.total_wages_tax += withheld;

    RUB net = gross - withheld;
    danya.cash += static_cast<long long>(net);
    danya.total_earned += gross;
    danya.total_spent += withheld;
    return net;
}


void danya_career_random(int year, int month)
{
    int break_event = rand() % 1000;                            // Случайные числа каждый месяц
    int break_length = 1 + rand() % 3;
    int bonus_event = rand() % 1000;
    int bonus_percent = 10 + rand() % 21;

    
    if (year >= 2030 &&                                         // После нескольких лет работы  перерыв без основной зарплаты
        danya.career_break_months_left == 0 &&
        break_event < 3) {

        danya.career_break_months_left = break_length;
        danya.career_break_count++;
    }


    if (year >= 2029 && month == 12 &&                          // В декабре иногда премия (10-30% месячной зарплаты)
        danya.career_break_months_left == 0 &&
        bonus_event < 350) {

        RUB bonus = danya.salary * (RUB)bonus_percent / 100;
        receive_work_income(bonus);
        danya.career_bonus_count++;
        danya.career_bonus_total += bonus;
    }
}


void danya_salary(const int year, const int month)              // const - показываем, что переменная не меняется
{
    if (month == 1) {
        danya.salary = (RUB)((double)danya.salary * (1.0 + inflation_annual[salary]));
    }
    if(year == 2028 && month == 9) {                           // Повышение
        danya.salary = 120'000;
    }

    if (danya.career_break_months_left > 0) {
        danya.career_break_months_left--;
        danya.career_break_months_total++;
        return;
    }

    danya.taxes.regular_net_this_month += receive_work_income(danya.salary);
}


void danya_setup_mortgage(RUB payment,RUB downpayment,int year,DeepTopic& dt)
{
    RUB apartment_price = apply_inflation_year(
        dt.apartment_price_base,
        realestate,
        year - 2026
    );

    danya.cash -= static_cast<long long>(downpayment);
    danya.total_spent += downpayment;

    danya.mortgage.months_total    = dt.mortgage_months;
    danya.mortgage.months_left     = dt.mortgage_months;
    danya.mortgage.monthly_payment = payment;
    danya.mortgage.annual_rate     = dt.mortgage_rate;

    danya.mortgage.principal =
        (double)(apartment_price - downpayment);

    danya.mortgage.remaining =
        (double)(apartment_price - downpayment);

    danya.apartment_price_paid = apartment_price;
    danya.owns_apartment = true;
}


void danya_mortgage_refinance(int year, int month, DeepTopic& dt)
{
    if (danya.mortgage_refinanced) return;
    if (danya.mortgage.months_left <= 0) return;
    if (year < dt.refinance_start_year) return;

    if (dt.refinance_rate >= danya.mortgage.annual_rate) return;

    RUB remaining = (RUB)(danya.mortgage.remaining + 0.5);

    RUB new_payment = calculate_annuity_payment(
        remaining,
        dt.refinance_rate,
        danya.mortgage.months_left
    );

    RUB old_payment = (RUB)danya.mortgage.monthly_payment;

    RUB old_remaining_payments =
        old_payment * danya.mortgage.months_left;

    RUB new_remaining_payments =
        new_payment * danya.mortgage.months_left
        + dt.refinance_cost;

    RUB projected_savings = 0;

    if (old_remaining_payments > new_remaining_payments) {
        projected_savings =
            old_remaining_payments - new_remaining_payments;
    }

    if (new_payment >= old_payment) return;

    if (danya.cash < static_cast<long long>(dt.refinance_cost)) return;

    danya.cash -= static_cast<long long>(dt.refinance_cost);
    danya.total_spent += dt.refinance_cost;

    danya.mortgage_rate_before_refinance = danya.mortgage.annual_rate;

    danya.mortgage_payment_before_refinance = old_payment;

    danya.refinance_cost_paid = dt.refinance_cost;

    danya.mortgage_balance_at_refinance = remaining;

    danya.refinance_projected_savings = projected_savings;

    danya.mortgage.annual_rate = dt.refinance_rate;

    danya.mortgage.monthly_payment = new_payment;

    danya.mortgage_refinanced = true;
    danya.refinance_year = year;
    danya.refinance_month = month;
}


void danya_mortgage(int year, int month)
{
    if (danya.mortgage.months_left <= 0) return;

    double interest = danya.mortgage.remaining * (danya.mortgage.annual_rate / 12);
    danya.total_mortgage_interest += (RUB)(interest + 0.5);

    double to_pay = loan_pay_month(danya.mortgage);

    danya.cash -= static_cast<long long>(to_pay);
    danya.total_spent += (RUB)to_pay;

    if (danya.mortgage.months_left == 0 &&
        danya.mortgage_close_year == 0) {

        danya.mortgage_close_year = year;
        danya.mortgage_close_month = month;
    }
}


void danya_second_job(int year)
{
    if (danya.has_second_job) return;

    const int decision_year = 2031;                                 // Первый год после выпуска из магистратуры
    if (year < decision_year) return;

    if (danya.owns_apartment) return;
    if (danya.mortgage.months_total > 0) return;

    danya.has_second_job = true;
    danya.salary_second_job = 80'000;
}


void danya_salary_second_job(const int month)
{
    if (!danya.has_second_job) return;

    if (month == 1) {
        danya.salary_second_job = (RUB)((double)danya.salary_second_job * (1.0 + inflation_annual[salary]));
    }

    danya.taxes.regular_net_this_month += receive_work_income(danya.salary_second_job);
}


RUB danya_current_credit_payments()
{
    RUB total_payment = 0;

    if (danya.mortgage.months_left > 0) {
        total_payment += (RUB)danya.mortgage.monthly_payment;
    }

    if (danya.car.loan.months_left > 0) {
        total_payment += (RUB)danya.car.loan.monthly_payment;
    }

    if (danya.consumer_credit.loan.months_left > 0) {
    total_payment += (RUB)danya.consumer_credit.loan.monthly_payment;
    }

    return total_payment;
}


bool danya_can_get_new_loan(RUB new_payment, DeepTopic& dt)
{
    RUB income =
        danya.taxes.regular_net_this_month;
    if (income <= 0) return false;

    RUB current_credit_payments =
        danya_current_credit_payments();

    RUB all_credit_payments =
        current_credit_payments +
        new_payment;

    return
        100 * all_credit_payments
        <=
        dt.dti_limit_percent * income;
}


RUB danya_rent_monthly_base()
{
    return 10'000;                                              // аренда в ценах 2026
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
    danya.cash -= static_cast<long long>(price);
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

        if (danya.cash < static_cast<long long>(dt.mortgage_downpayment + reserv)) {
            danya_rent(year);
            return;
        }

        RUB downpayment = static_cast<RUB>(danya.cash) - reserv;

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

        if (danya_can_get_new_loan(mortgage_payment, dt)) {

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
        danya_mortgage(year, month);
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


LoanType danya_choose_loan_for_early_repay()
{
    LoanType selected = no_loan;
    double highest_rate = 0.0;

    if (danya.mortgage.months_left > 0) {
        highest_rate = danya.mortgage.annual_rate;
        selected = mortgage_loan;
    }

    if (danya.car.loan.months_left > 0 &&
        danya.car.loan.annual_rate > highest_rate) {
        highest_rate = danya.car.loan.annual_rate;
        selected = car_loan;
    }

    if (danya.consumer_credit.loan.months_left > 0 &&
        danya.consumer_credit.loan.annual_rate > highest_rate) {
        selected = consumer_loan;
    }

    return selected;
}


void danya_early_repay_maybe(int year, int month, DeepTopic& dt)
{
    if (month != dt.early_repay_month) return;
    if (danya.cash < static_cast<long long>(dt.early_repay_pad + dt.early_repay_amount)) return;

    LoanType loan_type = danya_choose_loan_for_early_repay();

    if (loan_type == no_loan) return;

    double paid = 0;

    if (loan_type == mortgage_loan) {
        paid = loan_early_repay(
            danya.mortgage,
            dt.early_repay_amount
        );

        if (danya.mortgage.months_left == 0 &&
            danya.mortgage_close_year == 0) {

            danya.mortgage_close_year = year;
            danya.mortgage_close_month = month;
        }
    }

    if (loan_type == car_loan) {
        paid = loan_early_repay(danya.car.loan, dt.early_repay_amount);
        danya.car.total_spent += (RUB)paid;

        if (danya.car.loan.months_left == 0) {
            danya.car.loan_close_year = year;
            danya.car.loan_close_month = month;
        }
    }

    if (loan_type == consumer_loan) {
    paid = loan_early_repay(
        danya.consumer_credit.loan,
        dt.early_repay_amount
    );

    if (danya.consumer_credit.loan.months_left == 0) {
        danya.consumer_credit.loan_close_year = year;
        danya.consumer_credit.loan_close_month = month;
    }
    }

    danya.cash -= static_cast<long long>(paid);
    danya.total_spent += (RUB)paid;
}


void danya_saving_deposit(int year, DeepTopic& dt)
{
    RUB food_now = apply_inflation_year(
        danya_food_monthly_base(), food, year - 2026);

    RUB rent_now = apply_inflation_year(
        danya_rent_monthly_base(), rent, year - 2026);

    RUB utilities_now = apply_inflation_year(
        8'000, utilities, year - 2026);

    RUB monthly_now = food_now + rent_now;

    if (danya.owns_apartment) {
        monthly_now = food_now + utilities_now;
    }

    RUB pad = monthly_now * dt.saving_pad_months;

    if (danya.cash <= static_cast<long long>(pad)) return;

    RUB free = static_cast<RUB>(danya.cash) - pad;
    danya.deposit += free;
    danya.cash = static_cast<long long>(pad);
}


void danya_deposit_income(DeepTopic& dt)
{
    if (danya.deposit == 0) return;

    double monthly_rate = dt.deposit_rate / 12.0;

    double interest_gross =
        (double)danya.deposit * monthly_rate;

    RUB earned = (RUB)(interest_gross + 0.5);
    danya.taxes.deposit_interest_this_year += earned;

    danya.deposit += earned;

    danya.total_deposit_interest += earned;
    danya.total_earned += earned;
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

    danya.cash += static_cast<long long>(danya.deposit);
    danya.deposit = 0;

    return true;
}


void danya_life_saving(int year, int month, DeepTopic& dt)
{
    if (!danya.owns_apartment) {
        danya_rent(year);
    }

    danya_deposit_income(dt);

    danya_saving_deposit(year, dt);

    if (!danya.owns_apartment) {
        danya_try_buy_apartment(year, month, dt);
    }
}


void danya_food(int year)
{
    RUB base  = danya_food_monthly_base();
    RUB price = apply_inflation_year(base, food, year - 2026);

    danya.cash -= static_cast<long long>(price);
    danya.total_spent += price;
}


void danya_car_loan_payment(int year, int month)
{
    if (danya.car.loan.months_left <= 0) {
        return;
    }

    double interest =
        danya.car.loan.remaining *
        (danya.car.loan.annual_rate / 12.0);

    danya.car.total_loan_interest +=
        (RUB)(interest + 0.5);

    double payment =
        loan_pay_month(danya.car.loan);

    danya.cash -= static_cast<long long>(payment);

    danya.car.total_spent +=
        (RUB)payment;

    danya.total_spent +=
        (RUB)payment;

    if (danya.car.loan.months_left == 0) {

        danya.car.loan_close_year = year;
        danya.car.loan_close_month = month;
    }
}


void danya_consumer_credit(int year, int month, DeepTopic& dt)
{
    ConsumerCredit& credit = danya.consumer_credit;

    if (!credit.expense_required &&
        year == credit.event_year &&
        month == credit.event_month) {
        credit.expense_required = true;
    }

    if (credit.expense_required &&
        !credit.expense_paid &&
        !credit.loan_taken) {

        long long available_money = danya.cash + static_cast<long long>(danya.deposit);

        // Сначала пытаемся обойтись собственными деньгами.
        if (available_money >= static_cast<long long>(credit.expense_amount + credit.reserve)) {

            if (danya.cash >= static_cast<long long>(credit.expense_amount)) {
                danya.cash -= static_cast<long long>(credit.expense_amount);
            } else {
                RUB from_deposit = static_cast<RUB>(static_cast<long long>(credit.expense_amount) - danya.cash);
                danya.cash = 0;
                danya.deposit -= from_deposit;
            }

            danya.total_spent += credit.expense_amount;
            credit.expense_paid = true;

        } else {

            RUB payment = calculate_annuity_payment(
                credit.expense_amount,
                credit.loan_rate,
                credit.loan_months
            );

            if (danya_can_get_new_loan(payment, dt)) {

                credit.loan.principal = credit.expense_amount;
                credit.loan.remaining = credit.expense_amount;
                credit.loan.annual_rate = credit.loan_rate;
                credit.loan.months_total = credit.loan_months;
                credit.loan.months_left = credit.loan_months;
                credit.loan.monthly_payment = payment;
                credit.loan.total_early_paid = 0;

                credit.loan_taken = true;
                credit.expense_paid = true;

                credit.loan_start_year = year;
                credit.loan_start_month = month;

            } else {
                credit.bank_refusals++;
            }
        }
    }

    if (!credit.loan_taken || credit.loan.months_left <= 0) {
        return;
    }

    if (year == credit.loan_start_year &&
        month == credit.loan_start_month) {                         // Первый платёж — со следующего месяца
        return;
    }

    double interest =
        credit.loan.remaining * (credit.loan.annual_rate / 12.0);

    credit.total_loan_interest += (RUB)(interest + 0.5);

    double payment = loan_pay_month(credit.loan);

    danya.cash -= static_cast<long long>(payment);
    danya.total_spent += (RUB)payment;

    if (credit.loan.months_left == 0) {
        credit.loan_close_year = year;
        credit.loan_close_month = month;
    }
}


void danya_car(int year, int month, DeepTopic& dt)
{
    const int car_wanted_year = 2033;
    const RUB reserve = 1'000'000;

    bool car_bought_this_month = false;


    if (!danya.car.owns) {

        if (year < car_wanted_year) {
            return;
        }


        RUB car_price =
            apply_inflation_year(
                danya.car.purchase_price_base,
                car,
                year - 2026
            );


        RUB downpayment =
            car_price *
            danya.car.loan_downpayment_percent /
            100;


        long long available_money = danya.cash + static_cast<long long>(danya.deposit);


        if (available_money < static_cast<long long>(downpayment + reserve)) {      // На первоначальный взнос денег должно хватать с сохранением денежной подушки
            return;
        }


        RUB loan_amount =
            car_price -
            downpayment;


        RUB car_loan_payment =
            calculate_annuity_payment(
                loan_amount,
                danya.car.loan_rate,
                danya.car.loan_months
            );


        if (!danya_can_get_new_loan(car_loan_payment,dt)) {
            return;
        }


        if (danya.cash >= static_cast<long long>(downpayment)) {                // Первоначальный взнос

            danya.cash -= static_cast<long long>(downpayment);

        } else {

            RUB from_deposit = static_cast<RUB>(static_cast<long long>(downpayment) - danya.cash);

            danya.cash = 0;

            danya.deposit -=
                from_deposit;
        }


        danya.total_spent +=
            downpayment;

        danya.car.total_spent +=
            downpayment;


        danya.car.loan.principal =                          // Берем автокредит
            (double)loan_amount;

        danya.car.loan.remaining =
            (double)loan_amount;

        danya.car.loan.annual_rate =
            danya.car.loan_rate;

        danya.car.loan.months_total =
            danya.car.loan_months;

        danya.car.loan.months_left =
            danya.car.loan_months;

        danya.car.loan.monthly_payment =
            (double)car_loan_payment;

        danya.car.loan.total_early_paid = 0;


        danya.car.owns = true;
        danya.car.bought_on_credit = true;

        danya.car.purchase_price_paid =
            car_price;

        danya.car.loan_downpayment_paid =
            downpayment;

        danya.car.purchase_year =
            year;

        danya.car.purchase_month =
            month;


        car_bought_this_month = true;
    }


    if (!car_bought_this_month) {                           // Ежемесячный по автокредиту

        danya_car_loan_payment(
            year,
            month
        );
    }


    RUB fuel =
        apply_inflation_year(
            danya.car.fuel_monthly_base,
            car,
            year - 2026
        );


    RUB maintenance =                                       // Обслуживание
        apply_inflation_year(
            danya.car.maintenance_monthly_base,
            car,
            year - 2026
        );


    RUB monthly_car_expenses =
        fuel +
        maintenance;


    danya.cash -= static_cast<long long>(monthly_car_expenses);


    danya.car.total_spent +=
        monthly_car_expenses;

    danya.total_spent +=
        monthly_car_expenses;


    if (month ==                                            // Страховка
        danya.car.purchase_month) {

        RUB insurance =
            apply_inflation_year(
                danya.car.insurance_annual_base,
                car,
                year - 2026
            );

        danya.cash -= static_cast<long long>(insurance);

        danya.car.total_spent += insurance;

        danya.total_spent += insurance;
    }
}


void danya_pet(int year, int month)
{
    const int pet_wanted_year = 2034;
    const RUB reserve = 300'000;

    if (!danya.pet.had_pet) {

        if (year < pet_wanted_year) {
            return;
        }

        RUB pet_price = apply_inflation_year(
            danya.pet.purchase_price_base,
            pet,
            year - 2026
        );

        if (danya.cash < static_cast<long long>(pet_price + reserve)) {
            return;
        }

        danya.cash -= static_cast<long long>(pet_price);

        danya.pet.had_pet = true;
        danya.pet.alive = true;

        danya.pet.purchase_year = year;
        danya.pet.purchase_month = month;

        danya.pet.total_spent += pet_price;
        danya.total_spent += pet_price;
    }

    if (!danya.pet.alive) {
        return;
    }

    RUB pet_food = apply_inflation_year(
        danya.pet.food_monthly_base,
        pet,
        year - 2026
    );

    danya.cash -= static_cast<long long>(pet_food);

    danya.pet.total_spent += pet_food;
    danya.total_spent += pet_food;

    if (month == danya.pet.purchase_month) {

        RUB vet = apply_inflation_year(
            danya.pet.vet_annual_base,
            pet,
            year - 2026
        );

        danya.cash -= static_cast<long long>(vet);

        danya.pet.total_spent += vet;
        danya.total_spent += vet;
    }

    danya.pet.age_months += 1;

    if (danya.pet.age_months >= danya.pet.lifetime_months) {

        danya.pet.alive = false;

        danya.pet.death_year = year;
        danya.pet.death_month = month;
    }
}


bool danya_pay_random_expense(RUB amount)
{
    long long available_money = danya.cash + static_cast<long long>(danya.deposit);
    if (available_money < static_cast<long long>(amount)) return false;

    if (danya.cash >= static_cast<long long>(amount)) {
        danya.cash -= static_cast<long long>(amount);
    } else {
        RUB from_deposit = static_cast<RUB>(static_cast<long long>(amount) - danya.cash);
        danya.cash = 0;
        danya.deposit -= from_deposit;
    }

    danya.total_spent += amount;
    danya.total_random_expenses += amount;
    return true;
}


void danya_random_expenses(int year, int month)
{
    if (month < 1 || month > 12) return;

    int car_event = rand() % 1000;
    int pet_event = rand() % 1000;
    int home_event = rand() % 1000;

    int car_amount_random = rand() % 81;
    int pet_amount_random = rand() % 21;
    int home_amount_random = rand() % 66;

    if (danya.car.owns && car_event < 15) {                             // примерно 1.5% в месяц
        RUB base = 20'000 + (RUB)car_amount_random * 1'000;
        RUB amount = apply_inflation_year(base, car, year - 2026);

        if (danya_pay_random_expense(amount)) {
            danya.car.total_spent += amount;
            danya.random_car_repairs++;
            danya.random_car_repairs_spent += amount;
        }
    }

    if (danya.pet.alive && pet_event < 12) {                            // примерно 1.2% в месяц
        RUB base = 5'000 + (RUB)pet_amount_random * 1'000;
        RUB amount = apply_inflation_year(base, pet, year - 2026);

        if (danya_pay_random_expense(amount)) {
            danya.pet.total_spent += amount;
            danya.random_pet_vet_visits++;
            danya.random_pet_vet_spent += amount;
        }
    }

    if (danya.owns_apartment && home_event < 8) {                       // примерно 0.8% в месяц
        RUB base = 15'000 + (RUB)home_amount_random * 1'000;
        RUB amount = apply_inflation_year(base, realestate, year - 2026);

        if (danya_pay_random_expense(amount)) {
            danya.random_home_repairs++;
            danya.random_home_repairs_spent += amount;
        }
    }
}


void danya_home_bills(int year)
{
    if (danya.mortgage.months_total == 0 && !danya.owns_apartment) return;

    const RUB base_price = 8'000;
    RUB price = apply_inflation_year(base_price, utilities, year - 2026);
    
    danya.cash -= static_cast<long long>(price);
    danya.total_spent += price;
}


const RUB housing_deduction_limit = 2'000'000;
const RUB interest_deduction_limit = 3'000'000;


RUB smaller_amount(RUB first, RUB second)
{
    if (first < second) return first;
    return second;
}


RUB deposit_income_tax(RUB interest, double assumed_key_rate)
{
    // Максимальная ключевая ставка для каждого года задана условием сценария.
    RUB exempt_interest = (RUB)(1'000'000 * assumed_key_rate + 0.5);
    if (interest <= exempt_interest) return 0;

    RUB base = interest - exempt_interest;
    RUB tax_kopecks = base * 13;

    if (base > 2'400'000) {
        tax_kopecks += (base - 2'400'000) * 2;
    }

    return (tax_kopecks + 50) / 100;
}


void calculate_housing_refund()
{
    if (!danya.owns_apartment) return;

    Taxes& taxes = danya.taxes;
    RUB base = taxes.wages_this_year;

    // Право на вычет возникает в год покупки для обеих стратегий.
    RUB housing_limit = smaller_amount(danya.apartment_price_paid,
                                       housing_deduction_limit);
    RUB available = housing_limit - taxes.housing_base_used;
    RUB used = smaller_amount(available, base);

    taxes.housing_base_used += used;
    base -= used;

    // Заёмщик учитывает только уплаченные к концу года проценты.
    RUB interest_limit = smaller_amount(danya.total_mortgage_interest,
                                        interest_deduction_limit);
    available = interest_limit - taxes.mortgage_interest_base_used;
    used = smaller_amount(available, base);

    taxes.mortgage_interest_base_used += used;
    base -= used;

    taxes.refund_due_in_april = taxes.wages_tax_this_year - wages_tax(base);
}


void finish_tax_year(const DeepTopic& dt)
{
    Taxes& taxes = danya.taxes;
    calculate_housing_refund();

    taxes.deposit_tax_due_in_november =
        deposit_income_tax(taxes.deposit_interest_this_year, dt.assumed_key_rate);

    taxes.wages_this_year = 0;
    taxes.wages_tax_this_year = 0;
    taxes.deposit_interest_this_year = 0;
}


void settle_last_year_taxes(int month)
{
    Taxes& taxes = danya.taxes;

    if (month == 4) {
        danya.cash += static_cast<long long>(taxes.refund_due_in_april);
        danya.total_earned += taxes.refund_due_in_april;
        taxes.total_refund += taxes.refund_due_in_april;
        taxes.refund_due_in_april = 0;
    }

    if (month == 11) {
        danya.cash -= static_cast<long long>(taxes.deposit_tax_due_in_november);
        danya.total_spent += taxes.deposit_tax_due_in_november;
        taxes.total_deposit_tax_paid += taxes.deposit_tax_due_in_november;
        taxes.deposit_tax_due_in_november = 0;
    }
}


void cover_cash_shortfall()
{
    // Для обязательных расходов можно снять часть вклада.
    // Непокрытый дефицит переносится на следующий месяц.
    if (danya.cash >= 0) return;

    RUB withdrawal = smaller_amount(-danya.cash, danya.deposit);
    danya.deposit -= withdrawal;
    danya.cash += static_cast<long long>(withdrawal);
}


void simulation(DeepTopic& dt)
{
    int year = 2026;
    int month = 9;
    
    while (!(year == 2076 && month == 9)) {

        if (month == 1) {
            random_inflation_update(year);
            deposit_rate_update(year, dt);
        }

        danya.taxes.regular_net_this_month = 0;
        settle_last_year_taxes(month);
        danya_career_random(year, month);

        danya_salary(year, month);                                         // Задаем ТЗ

        danya_second_job(year);

        danya_salary_second_job(month);

        if (strategy == mortgage) {
            danya_life_mortgage(year, month, dt);
            danya_mortgage_refinance(year, month, dt);
            danya_early_repay_maybe(year, month, dt);
        }
        else {
            danya_life_saving(year, month, dt);
        }

        danya_car(year, month, dt);

        danya_consumer_credit(year, month, dt);

        danya_food(year);

        danya_home_bills(year);

        danya_pet(year, month);

        danya_random_expenses(year, month);

        cover_cash_shortfall();
        if (month == 12) {
            finish_tax_year(dt);
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
    danya = Person{};

    danya.cash = 20'000;
    danya.salary = 80'000;
    danya_food_init();
    danya_car_init();
    danya_consumer_credit_init();
    danya_pet_init();
}


int danya_mortgage_actual_months()
{
    if (danya.purchase_year == 0 || danya.mortgage_close_year == 0) {
        return 0;
    }

    int start = danya.purchase_year * 12 + danya.purchase_month;
    int finish = danya.mortgage_close_year * 12 + danya.mortgage_close_month;

    return finish - start;
}



const char* category_name(Category category)
{
    if (category == food) return "food";
    if (category == utilities) return "utilities";
    if (category == rent) return "rent";
    if (category == pet) return "pet";
    if (category == car) return "car";
    if (category == salary) return "salary";
    if (category == realestate) return "realestate";
    return "unknown";
}


RUB danya_total_remaining_debt()
{
    RUB debt = 0;

    if (danya.mortgage.months_left > 0) {
        debt += (RUB)danya.mortgage.remaining;
    }

    if (danya.car.loan.months_left > 0) {
        debt += (RUB)danya.car.loan.remaining;
    }

    if (danya.consumer_credit.loan.months_left > 0) {
        debt += (RUB)danya.consumer_credit.loan.remaining;
    }

    return debt;
}


void danya_print_inflation_report()
{
    printf("\n----------- INFLATION -----------\n");

    for (int i = 0; i < COUNT; ++i) {
        Category category = (Category)i;

        printf("%-12s base=%4.1f%% current=%4.1f%% factor=%.3f\n",
               category_name(category),
               inflation_annual[i] * 100.0,
               inflation_current_rate[i] * 100.0,
               inflation_random_factor[i]);
    }
}


void danya_print_credit_summary()
{
    RUB interest = danya.total_mortgage_interest +
                   danya.car.total_loan_interest +
                   danya.consumer_credit.total_loan_interest;

    printf("All loan interest: %llu\n", interest);
    printf("Remaining debt:    %llu\n", danya_total_remaining_debt());
}


void danya_print_apartment_section()
{
    if (danya.owns_apartment) {
        printf("Apartment bought:  %d-%02d for %llu\n",
               danya.purchase_year, danya.purchase_month,
               danya.apartment_price_paid);
    } else if (danya.mortgage.months_total > 0 && danya.mortgage.months_left == 0) {
        printf("Apartment:         own (mortgage paid off)\n");
    } else {
        printf("Apartment:         none\n");
    }
}


void danya_print_refinance_section()
{
    if (!danya.mortgage_refinanced) return;

    printf("Refinanced:        %d-%02d\n",
           danya.refinance_year, danya.refinance_month);
    printf("Old mortgage rate: %.1f%%\n",
           danya.mortgage_rate_before_refinance * 100.0);
    printf("New mortgage rate: %.1f%%\n",
           danya.mortgage.annual_rate * 100.0);
    printf("Old payment:       %llu\n",
           danya.mortgage_payment_before_refinance);
    printf("New payment:       %.0f\n",
           danya.mortgage.monthly_payment);
    printf("Refinance cost:    %llu\n",
           danya.refinance_cost_paid);
    printf("Balance at refin.: %llu\n",
           danya.mortgage_balance_at_refinance);
    printf("Projected saving:  %llu\n",
           danya.refinance_projected_savings);
}


void danya_print_car_section()
{
    if (!danya.car.owns) {
        printf("Car:               none\n");
        return;
    }

    printf("Car bought:        %d-%02d\n",
           danya.car.purchase_year, danya.car.purchase_month);
    printf("Car price:         %llu\n",
           danya.car.purchase_price_paid);
    printf("Car downpayment:   %llu\n",
           danya.car.loan_downpayment_paid);

    if (!danya.car.bought_on_credit) {
        printf("Car total spent:   %llu\n",
               danya.car.total_spent);
        return;
    }

    printf("Car loan amount:   %.0f\n",
           danya.car.loan.principal);
    printf("Car loan payment:  %.0f\n",
           danya.car.loan.monthly_payment);
    printf("Car loan interest: %llu\n",
           danya.car.total_loan_interest);
    printf("Car early repaid:  %.0f\n",
           danya.car.loan.total_early_paid);

    if (danya.car.loan.months_left == 0) {
        printf("Car loan closed:   %d-%02d\n",
               danya.car.loan_close_year, danya.car.loan_close_month);
    } else {
        printf("Car loan remaining: %.0f\n",
               danya.car.loan.remaining);
        printf("Car loan months:    %d\n",
               danya.car.loan.months_left);
    }

    printf("Car total spent:   %llu\n",
           danya.car.total_spent);
}


void danya_print_pet_section()
{
    if (!danya.pet.had_pet) {
        printf("Pet:               none\n");
        return;
    }

    printf("Pet bought:        %d-%02d\n",
           danya.pet.purchase_year, danya.pet.purchase_month);

    if (danya.pet.alive) {
        printf("Pet status:        alive\n");
    } else {
        printf("Pet died:          %d-%02d\n",
               danya.pet.death_year, danya.pet.death_month);
    }

    printf("Pet total spent:   %llu\n",
           danya.pet.total_spent);
}


void danya_print_consumer_credit_section()
{
    if (!danya.consumer_credit.expense_required) return;

    printf("Emergency expense: %llu\n",
           danya.consumer_credit.expense_amount);

    if (!danya.consumer_credit.loan_taken) {
        printf("Consumer loan:     not used\n");
    } else {
        printf("Consumer loan:     %.0f\n",
               danya.consumer_credit.loan.principal);
        printf("Consumer payment:  %.0f\n",
               danya.consumer_credit.loan.monthly_payment);
        printf("Consumer interest: %llu\n",
               danya.consumer_credit.total_loan_interest);
        printf("Consumer early:    %.0f\n",
               danya.consumer_credit.loan.total_early_paid);

        if (danya.consumer_credit.loan.months_left == 0) {
            printf("Consumer closed:   %d-%02d\n",
                   danya.consumer_credit.loan_close_year,
                   danya.consumer_credit.loan_close_month);
        } else {
            printf("Consumer balance:  %.0f\n",
                   danya.consumer_credit.loan.remaining);
        }
    }

    printf("Bank refusals:      %d\n",
           danya.consumer_credit.bank_refusals);
}


void danya_print_random_events_section()
{
    printf("Random car repairs: %d (%llu RUB)\n",
           danya.random_car_repairs, danya.random_car_repairs_spent);
    printf("Random pet visits:  %d (%llu RUB)\n",
           danya.random_pet_vet_visits, danya.random_pet_vet_spent);
    printf("Random home repair: %d (%llu RUB)\n",
           danya.random_home_repairs, danya.random_home_repairs_spent);
    printf("Random expenses:    %llu\n",
           danya.total_random_expenses);
}


void danya_print_career_section()
{
    printf("Career breaks:      %d (%d months)\n",
           danya.career_break_count, danya.career_break_months_total);
    printf("Career bonuses:     %d (%llu RUB)\n",
           danya.career_bonus_count, danya.career_bonus_total);
}


void danya_print_totals_section(DeepTopic& dt)
{
    printf("Total earned:      %llu\n", danya.total_earned);
    printf("Total spent:       %llu\n", danya.total_spent);
    printf("Wage tax withheld: %llu\n", danya.taxes.total_wages_tax);
    printf("Deposit tax paid:  %llu\n", danya.taxes.total_deposit_tax_paid);
    printf("Tax refunded:      %llu\n", danya.taxes.total_refund);
    printf("Housing base used: %llu\n", danya.taxes.housing_base_used);
    printf("Interest base used:%llu\n", danya.taxes.mortgage_interest_base_used);

    if (strategy == mortgage) {
        printf("Interest paid:     %llu\n",
               danya.total_mortgage_interest);
        printf("Early repaid:      %.0f\n",
               danya.mortgage.total_early_paid);
        printf("Mortgage amount:   %.0f\n",
               danya.mortgage.principal);

        if (danya.mortgage_close_year != 0) {
            int actual_months = danya_mortgage_actual_months();

            printf("Mortgage closed:   %d-%02d\n",
                   danya.mortgage_close_year,
                   danya.mortgage_close_month);
            printf("Mortgage term:     %d months\n", actual_months);
            printf("Mortgage term:     %d years %d months\n",
                   actual_months / 12, actual_months % 12);
        }
    } else {
        printf("Interest earned:   %llu\n",
               danya.total_deposit_interest);
        printf("Deposit rate now:  %.1f%%\n",
               dt.deposit_rate * 100.0);
        printf("Deposit rate range: %.1f%% - %.1f%%\n",
               dt.deposit_rate_min * 100.0,
               dt.deposit_rate_max * 100.0);
        printf("Rate changes:      %d\n",
               dt.deposit_rate_changes);
    }
}


void danya_print_net_worth_section(DeepTopic& dt)
{
    RUB apt_now = 0;
    bool has_apartment = danya.owns_apartment ||
        (danya.mortgage.months_total > 0 && danya.mortgage.months_left == 0);

    if (has_apartment) {
        apt_now = apply_inflation_year(dt.apartment_price_base,
                                       realestate, 2076 - 2026);
    }
    // Капитал на конец августа включает начисленный налог по вкладу.
    RUB deposit_tax_pending = danya.taxes.deposit_tax_due_in_november +
        deposit_income_tax(danya.taxes.deposit_interest_this_year, dt.assumed_key_rate);
    long long net = danya.cash + static_cast<long long>(danya.deposit)
                  + static_cast<long long>(apt_now)
                  - static_cast<long long>(danya_total_remaining_debt())
                  - static_cast<long long>(deposit_tax_pending);

    printf("Deposit tax pending: %llu\n", deposit_tax_pending);

    printf("Net worth:         %lld\n", net);
}


void danya_print_report(DeepTopic& dt)
{
    printf("\n=============== REPORT ===============\n");
    printf("Strategy:          %s\n",
           strategy == mortgage ? "mortgage" : "saving");
    printf("Final cash:        %lld\n", danya.cash);
    printf("Final deposit:     %llu\n", danya.deposit);

    danya_print_apartment_section();
    danya_print_refinance_section();
    danya_print_car_section();
    danya_print_pet_section();
    danya_print_consumer_credit_section();
    danya_print_random_events_section();
    danya_print_career_section();
    danya_print_totals_section(dt);
    danya_print_net_worth_section(dt);

    danya_print_inflation_report();
    danya_print_credit_summary();

    printf("======================================\n");
}


int main()
{
    DeepTopic dt;
    deep_topic_init(dt);
    danya_init();

    srand(42);                         // фиксированный seed для повторяемого сценария

    simulation(dt);

    danya_print_report(dt);

    return 0;
}
