#include <stdio.h>
#include <math.h>

using RUB = unsigned long long int;                             // Важна точка управления       Безнаковый тип прописываем (больше пустых ячеек- экономия памяти)

enum Strategy {mortgage, saving};

enum LoanType {no_loan, mortgage_loan, car_loan, consumer_loan};

const Strategy strategy = mortgage;                           // mortgage или saving

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

    // параметры автокредита
    int loan_downpayment_percent;
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


struct Person {
    RUB cash;
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

    bool tax_refund_received;
    RUB total_tax_refund;
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

    // рефинансирование ипотеки
    double refinance_rate;
    int refinance_start_year;
    RUB refinance_cost;

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

    dc.refinance_rate = 0.12;
    dc.refinance_start_year = 2037;
    dc.refinance_cost = 50'000;

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
    danya.car.bought_on_credit = false;

    danya.car.purchase_price_base = 1'500'000;
    danya.car.purchase_price_paid = 0;

    danya.car.fuel_monthly_base = 8'000;
    danya.car.maintenance_monthly_base = 3'000;
    danya.car.insurance_annual_base = 40'000;

    danya.car.purchase_year = 0;
    danya.car.purchase_month = 0;

    // Автокредит:
    danya.car.loan_downpayment_percent = 30;                        // первоначальный взнос 30%
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

    if (danya.cash < dt.refinance_cost) return;

    danya.cash -= dt.refinance_cost;
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

    danya.cash        -= (RUB)to_pay;
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
        danya.salary +
        danya.salary_second_job;

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
    if (danya.cash < dt.early_repay_pad + dt.early_repay_amount) return;

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

    double monthly_rate = dt.deposit_rate / 12.0;

    double interest_gross =
        (double)danya.deposit * monthly_rate;

    double interest_net =
        interest_gross * (1.0 - dt.deposit_tax);

    RUB earned = (RUB)(interest_net + 0.5);

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

    danya.cash        -= price;
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

    danya.cash -= (RUB)payment;

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

        RUB available_money = danya.cash + danya.deposit;

        // Сначала пытаемся обойтись собственными деньгами.
        if (available_money >= credit.expense_amount + credit.reserve) {

            if (danya.cash >= credit.expense_amount) {
                danya.cash -= credit.expense_amount;
            } else {
                RUB from_deposit = credit.expense_amount - danya.cash;
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

    // Первый платёж — со следующего месяца.
    if (year == credit.loan_start_year &&
        month == credit.loan_start_month) {
        return;
    }

    double interest =
        credit.loan.remaining * (credit.loan.annual_rate / 12.0);

    credit.total_loan_interest += (RUB)(interest + 0.5);

    double payment = loan_pay_month(credit.loan);

    danya.cash -= (RUB)payment;
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


        RUB available_money =
            danya.cash +
            danya.deposit;


        // На первоначальный взнос денег должно хватать с сохранением денежной подушки
        if (available_money < downpayment + reserve) {
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


        // Проверяем долговую нагрузку:
        // ипотека + новый автокредит.
        if (!danya_can_get_new_loan(car_loan_payment,dt)) {
            return;
        }


        // ПЕРВОНАЧАЛЬНЫЙ ВЗНОС
        if (danya.cash >= downpayment) {

            danya.cash -= downpayment;

        } else {

            RUB from_deposit =
                downpayment -
                danya.cash;

            danya.cash = 0;

            danya.deposit -=
                from_deposit;
        }


        danya.total_spent +=
            downpayment;

        danya.car.total_spent +=
            downpayment;


        // СОЗДАНИЕ АВТОКРЕДИТА
        danya.car.loan.principal =
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

    // ЕЖЕМЕСЯЧНЫЙ ПЛАТЁЖ ПО АВТОКРЕДИТУ

    if (!car_bought_this_month) {

        danya_car_loan_payment(
            year,
            month
        );
    }

    // ТОПЛИВО

    RUB fuel =
        apply_inflation_year(
            danya.car.fuel_monthly_base,
            car,
            year - 2026
        );


    // ОБСЛУЖИВАНИЕ

    RUB maintenance =
        apply_inflation_year(
            danya.car.maintenance_monthly_base,
            car,
            year - 2026
        );


    RUB monthly_car_expenses =
        fuel +
        maintenance;


    danya.cash -=
        monthly_car_expenses;


    danya.car.total_spent +=
        monthly_car_expenses;

    danya.total_spent +=
        monthly_car_expenses;


    // СТРАХОВКА

    if (month ==
        danya.car.purchase_month) {

        RUB insurance =
            apply_inflation_year(
                danya.car.insurance_annual_base,
                car,
                year - 2026
            );


        danya.cash -=
            insurance;


        danya.car.total_spent +=
            insurance;

        danya.total_spent +=
            insurance;
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

        if (danya.cash < pet_price + reserve) {
            return;
        }

        danya.cash -= pet_price;

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

    danya.cash -= pet_food;

    danya.pet.total_spent += pet_food;
    danya.total_spent += pet_food;

    if (month == danya.pet.purchase_month) {

        RUB vet = apply_inflation_year(
            danya.pet.vet_annual_base,
            pet,
            year - 2026
        );

        danya.cash -= vet;

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


void danya_home_bills(int year)
{
    if (danya.mortgage.months_total == 0 && !danya.owns_apartment) return;

    const RUB base_price = 8'000;
    RUB price = apply_inflation_year(base_price, utilities, year - 2026);
    
    danya.cash -= price;
    danya.total_spent += price;      // ← добавь
}


void danya_tax_refund(int year, int month)
{
    if (danya.tax_refund_received) {
        return;
    }

    if (!danya.owns_apartment) {
        return;
    }

    if (year <= danya.purchase_year) {
        return;
    }

    if (month != 4) {
        return;
    }

    const RUB tax_refund = 260'000;

    danya.cash += tax_refund;
    danya.total_earned += tax_refund;
    danya.total_tax_refund += tax_refund;

    danya.tax_refund_received = true;
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
            danya_mortgage_refinance(year, month, dt);
            danya_early_repay_maybe(year, month, dt);
        }
        else {
            danya_life_saving(year, month, dt);
        }

        danya_car(year, month, dt);

        danya_consumer_credit(year, month, dt);

        danya_food(year);                                                   // Добавить вклады, налоги, инфлянцию

        danya_home_bills(year);

        danya_pet(year, month);

        danya_tax_refund(year, month);

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

    danya.mortgage_refinanced = false;
    danya.refinance_year = 0;
    danya.refinance_month = 0;
    danya.mortgage_rate_before_refinance = 0;
    danya.mortgage_payment_before_refinance = 0;
    danya.refinance_cost_paid = 0;

    danya.tax_refund_received = false;
    danya.total_tax_refund = 0;

    danya_food_init();
    danya_car_init();
    danya_consumer_credit_init();
    danya_pet_init();
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

    printf("Car price:         %llu\n",
        danya.car.purchase_price_paid);

    printf("Car downpayment:   %llu\n",
        danya.car.loan_downpayment_paid);


    if (danya.car.bought_on_credit) {

        printf("Car loan amount:   %.0f\n",
            danya.car.loan.principal);

        printf("Car loan payment:  %.0f\n",
            danya.car.loan.monthly_payment);

        printf("Car loan interest: %llu\n",
            danya.car.total_loan_interest);

        printf("Car early repaid:  %.0f\n",
            danya.car.loan.total_early_paid);

        if (danya.mortgage_refinanced) {
            printf("Refinanced:        %d-%02d\n",
                danya.refinance_year,
                danya.refinance_month);

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
        }


        if (danya.car.loan.months_left == 0) {

            printf("Car loan closed:   %d-%02d\n",
                danya.car.loan_close_year,
                danya.car.loan_close_month);

        } else {

            printf("Car loan remaining: %.0f\n",
                danya.car.loan.remaining);

            printf("Car loan months:    %d\n",
                danya.car.loan.months_left);
        }
    }


    printf("Car total spent:   %llu\n",
        danya.car.total_spent);

    } else {

        printf("Car:               none\n");
    }

    
    if (danya.pet.had_pet) {

    printf("Pet bought:        %d-%02d\n",
           danya.pet.purchase_year,
           danya.pet.purchase_month);

    if (danya.pet.alive) {

        printf("Pet status:        alive\n");

    } else {

        printf("Pet died:          %d-%02d\n",
               danya.pet.death_year,
               danya.pet.death_month);
    }

    printf("Pet total spent:   %llu\n",
           danya.pet.total_spent);

    } else {
        printf("Pet:               none\n");
    }

    if (danya.consumer_credit.expense_required) {
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

    printf("Total earned:      %llu\n", danya.total_earned);
    printf("Total spent:       %llu\n", danya.total_spent);
    printf("Tax refund:        %llu\n", danya.total_tax_refund);

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