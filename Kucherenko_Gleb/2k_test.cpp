#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <random>

using RUB = int;

int year = 2026;
int month = 9;

std::mt19937 rng(42);

int rand_range(int min_value, int max_value)
{
    std::uniform_int_distribution<int> dist(min_value, max_value);
    return dist(rng);
}

bool chance(int percent)
{
    std::uniform_int_distribution<int> dist(1, 100);
    return dist(rng) <= percent;
}

struct Job
{
    RUB salary = 0;
    bool active = false;
};

struct Person
{
    Job job1;
    Job job2;

    RUB cash = 0;
    RUB deposit = 0;
    RUB petexp = 0;
    RUB transport_exp = 0;
    RUB gasoline_exp = 0;
    RUB food_exp = 0;
    RUB utilities_exp = 0;
    RUB rent_exp = 0;

    RUB taxes_paid = 0;
    RUB taxes_refund = 0;
    RUB yearly_salary = 0;

    RUB car_loan_balance = 0;
    RUB car_loan_payment = 0;
    int car_loan_months_left = 0;

    RUB mortgage_balance = 0;
    RUB mortgage_payment = 0;
    int mortgage_months_left = 0;

    RUB debt_balance = 0;
    int job2_search_months_left = 0;

    int age = 0;
    int exp_age = 0;

    RUB protein_food_exp = 0;
    RUB grains_food_exp = 0;
    RUB vegetables_food_exp = 0;
    RUB fruits_food_exp = 0;
    RUB dairy_food_exp = 0;
    RUB healthy_fats_exp = 0;
    RUB other_food_exp = 0;

    RUB gym_exp = 0;
    RUB gym_pass_cost = 18'000;

    bool saving_mode = false;

    RUB total_salary() const
    {
        RUB result = 0;
        if (job1.active) result += job1.salary;
        if (job2.active) result += job2.salary;
        return result;
    }
};

Person alice;
Person ivan;

RUB base_monthly_expenses(const Person& person)
{
    return person.food_exp
         + person.transport_exp
         + person.gasoline_exp
         + person.utilities_exp
         + person.rent_exp
         + person.car_loan_payment
         + person.mortgage_payment
         + person.gym_exp
         + person.petexp;
}

void alice_jobs()
{
    switch (year)
    {
        case 2026:
            alice.job1.salary = 80'000;
            alice.job1.active = true;
            break;
        case 2027:
            alice.job1.salary = 95'000;
            alice.job1.active = true;
            break;
        case 2028:
            alice.job1.salary = 110'000;
            alice.job1.active = true;
            break;
        case 2029:
            alice.job1.salary = 120'000;
            alice.job1.active = true;
            break;
        case 2030:
            alice.job1.salary = 140'000;
            alice.job1.active = true;
            break;
        case 2031:
            alice.job1.salary = 90'000;
            alice.job1.active = true;
            break;
        case 2032:
            alice.job1.salary = 67'000;
            alice.job1.active = true;
            break;
        case 2033:
            alice.job1.salary = 69'000;
            alice.job1.active = true;
            break;
        case 2034:
            alice.job1.salary = 99'000;
            alice.job1.active = true;
            break;
        case 2035:
            alice.job1.salary = 125'000;
            alice.job1.active = true;
            break;
        case 2036:
            alice.job1.salary = 140'000;
            alice.job1.active = true;
            break;
        default:
            break;
    }
}

void ivan_jobs()
{
    switch (year)
    {
        case 2026:
            ivan.job1.salary = 75'000;
            ivan.job1.active = true;
            break;
        case 2027:
            ivan.job1.salary = 80'000;
            ivan.job1.active = true;
            break;
        case 2028:
            ivan.job1.salary = 55'000;
            ivan.job1.active = true;
            break;
        case 2029:
            ivan.job1.salary = 95'000;
            ivan.job1.active = true;
            break;
        case 2030:
            ivan.job1.salary = 95'000;
            ivan.job1.active = true;
            break;
        case 2031:
            ivan.job1.salary = 120'000;
            ivan.job1.active = true;
            break;
        case 2032:
            ivan.job1.salary = 125'000;
            ivan.job1.active = true;
            break;
        case 2033:
            ivan.job1.salary = 135'000;
            ivan.job1.active = true;
            break;
        case 2034:
            ivan.job1.salary = 135'000;
            ivan.job1.active = true;
            break;
        case 2035:
            ivan.job1.salary = 135'000;
            ivan.job1.active = true;
            break;
        case 2036:
            ivan.job1.salary = 140'000;
            ivan.job1.active = true;
            break;
        default:
            if (year > 2036)
            {
                ivan.job1.salary = 135'000;
                ivan.job1.active = true;
            }
            break;
    }
}

void alice_deposit()
{
    const double monthly_rate = 0.05 / 12.0;
    const double ndfl = 0.13;

    RUB interest = static_cast<RUB>(
        alice.deposit * monthly_rate);
    RUB tax = static_cast<RUB>(interest * ndfl);

    alice.deposit += interest - tax;
}

void ivan_deposit()
{
    const double monthly_rate = 0.05 / 12.0;
    const double ndfl = 0.13;

    RUB interest = static_cast<RUB>(
        ivan.deposit * monthly_rate);
    RUB tax = static_cast<RUB>(interest * ndfl);

    ivan.deposit += interest - tax;
}

void alice_pet()
{
    switch (year)
    {
        case 2026:
            alice.petexp = 0;
            break;
        case 2027:
            alice.petexp = 4'000;
            break;
        case 2028:
            alice.petexp = 2'000;
            break;
        case 2029:
            alice.petexp = 3'000;
            break;
        case 2030:
            alice.petexp = 3'500;
            break;
        case 2031:
            alice.petexp = 3'500;
            break;
        case 2032:
            alice.petexp = 5'000;
            break;
        case 2033:
            alice.petexp = 3'500;
            break;
        case 2034:
            alice.petexp = 3'500;
            break;
        case 2035:
            alice.petexp = 4'000;
            break;
        case 2036:
            alice.petexp = 0;
            break;
        default:
            alice.petexp = 0;
            break;
    }
}

void ivan_pet()
{
    switch (year)
    {
        case 2026:
            ivan.petexp = 0;
            break;
        case 2027:
            ivan.petexp = 0;
            break;
        case 2028:
            ivan.petexp = 0;
            break;
        case 2029:
            ivan.petexp = 10'000;
            break;
        case 2030:
            ivan.petexp = 5'000;
            break;
        case 2031:
            ivan.petexp = 5'000;
            break;
        case 2032:
            ivan.petexp = 5'000;
            break;
        case 2033:
            ivan.petexp = 7'000;
            break;
        case 2034:
            ivan.petexp = 0;
            break;
        case 2035:
            ivan.petexp = 9'000;
            break;
        case 2036:
            ivan.petexp = 5'000;
            break;
        default:
            ivan.petexp = 0;
            break;
    }
}

void alice_transport()
{
    switch (year)
    {
        case 2026:
            alice.transport_exp = 0;
            break;
        case 2027:
            alice.transport_exp = 4'000;
            break;
        case 2028:
            alice.transport_exp = 4'500;
            break;
        case 2029:
            alice.transport_exp = 5'000;
            break;
        case 2030:
            alice.transport_exp = 5'500;
            break;
        case 2031:
            alice.transport_exp = 0;
            break;
        case 2032:
            alice.transport_exp = 5'500;
            break;
        case 2033:
            alice.transport_exp = 6'000;
            break;
        case 2034:
            alice.transport_exp = 6'500;
            break;
        case 2035:
            alice.transport_exp = 7'000;
            break;
        case 2036:
            alice.transport_exp = 0;
            break;
        default:
            alice.transport_exp = 0;
            break;
    }
}

void alice_gasoline()
{
    if (year < 2027 || year > 2035)
    {
        alice.gasoline_exp = 0;
        return;
    }

    const double annual_inflation = 0.05;
    const int years_passed = year - 2027;

    alice.gasoline_exp = static_cast<RUB>(
        6'000 * std::pow(1.0 + annual_inflation, years_passed));
}

void ivan_transport()
{
    if (year < 2027)
    {
        ivan.transport_exp = 0;
    }
    else
    {
        const RUB monthly_transport_in_2027 = 5'000;
        const RUB annual_transport_increase = 500;

        ivan.transport_exp = monthly_transport_in_2027 +
            annual_transport_increase * (year - 2027);
    }
}

void ivan_food()
{
    switch (year)
    {
        case 2026:
            ivan.food_exp = 24'000;
            break;
        case 2027:
            ivan.food_exp = 25'200;
            break;
        case 2028:
            ivan.food_exp = 26'460;
            break;
        case 2029:
            ivan.food_exp = 27'783;
            break;
        case 2030:
            ivan.food_exp = 29'172;
            break;
        case 2031:
            ivan.food_exp = 30'631;
            break;
        case 2032:
            ivan.food_exp = 32'162;
            break;
        case 2033:
            ivan.food_exp = 33'770;
            break;
        case 2034:
            ivan.food_exp = 35'459;
            break;
        case 2035:
            ivan.food_exp = 37'232;
            break;
        case 2036:
            ivan.food_exp = 39'093;
            break;
        default:
            ivan.food_exp = static_cast<RUB>(
                39'093 * std::pow(1.05, year - 2036));
            break;
    }
}

void alice_utilities()
{
    switch (year)
    {
        case 2026:
            alice.utilities_exp = 10'000;
            break;
        case 2027:
            alice.utilities_exp = 10'450;
            break;
        case 2028:
            alice.utilities_exp = 10'920;
            break;
        case 2029:
            alice.utilities_exp = 11'411;
            break;
        case 2030:
            alice.utilities_exp = 11'925;
            break;
        case 2031:
            alice.utilities_exp = 12'462;
            break;
        case 2032:
            alice.utilities_exp = 13'023;
            break;
        case 2033:
            alice.utilities_exp = 13'609;
            break;
        case 2034:
            alice.utilities_exp = 14'221;
            break;
        case 2035:
            alice.utilities_exp = 14'861;
            break;
        case 2036:
            alice.utilities_exp = 15'530;
            break;
        default:
            alice.utilities_exp = static_cast<RUB>(
                15'530 * std::pow(1.045, year - 2036));
            break;
    }
}

void ivan_utilities()
{
    switch (year)
    {
        case 2026:
            ivan.utilities_exp = 9'000;
            break;
        case 2027:
            ivan.utilities_exp = 9'405;
            break;
        case 2028:
            ivan.utilities_exp = 9'828;
            break;
        case 2029:
            ivan.utilities_exp = 10'270;
            break;
        case 2030:
            ivan.utilities_exp = 10'732;
            break;
        case 2031:
            ivan.utilities_exp = 11'215;
            break;
        case 2032:
            ivan.utilities_exp = 11'720;
            break;
        case 2033:
            ivan.utilities_exp = 12'247;
            break;
        case 2034:
            ivan.utilities_exp = 12'798;
            break;
        case 2035:
            ivan.utilities_exp = 13'374;
            break;
        case 2036:
            ivan.utilities_exp = 13'976;
            break;
        default:
            ivan.utilities_exp = static_cast<RUB>(
                13'976 * std::pow(1.045, year - 2036));
            break;
    }
}

void alice_rent()
{
    const RUB monthly_rent_in_2026 = 32'000;
    const double annual_rent_increase = 0.05;

    alice.rent_exp = static_cast<RUB>(
        monthly_rent_in_2026 *
        std::pow(1.0 + annual_rent_increase, year - 2026));
}

void ivan_rent()
{
    const RUB monthly_rent_in_2026 = 20'000;
    const double annual_rent_increase = 0.05;

    if (year < 2029)
    {
        ivan.rent_exp = static_cast<RUB>(
            monthly_rent_in_2026 *
            std::pow(1.0 + annual_rent_increase, year - 2026));
    }
    else
    {
        ivan.rent_exp = 0;
    }
}

void alice_start_car_loan()
{
    const RUB car_price = 1'500'000;
    const int loan_duration_months = 60;
    const double annual_interest_rate = 0.10;
    const double monthly_interest_rate = annual_interest_rate / 12.0;

    if (year == 2027 && month == 1 &&
        alice.car_loan_balance == 0)
    {
        RUB down_payment = alice.cash + alice.deposit;
        if (down_payment > car_price)
        {
            down_payment = car_price;
        }

        RUB from_cash = std::min(alice.cash, down_payment);
        alice.cash -= from_cash;
        RUB from_deposit = down_payment - from_cash;
        alice.deposit -= from_deposit;

        RUB loan_amount = car_price - down_payment;
        alice.car_loan_balance = loan_amount;
        alice.car_loan_months_left = loan_duration_months;

        const double payment_factor =
            std::pow(1.0 + monthly_interest_rate,
                     loan_duration_months);

        alice.car_loan_payment = static_cast<RUB>(
            loan_amount * monthly_interest_rate * payment_factor /
            (payment_factor - 1.0));

        std::cout << "  [Car purchase] down payment = "
                  << down_payment << ", loan = "
                  << loan_amount << " RUB\n";
    }
}

void alice_pay_car_loan()
{
    const double annual_interest_rate = 0.10;
    const double monthly_interest_rate = annual_interest_rate / 12.0;

    if (alice.car_loan_balance > 0 &&
        alice.car_loan_months_left > 0)
    {
        RUB monthly_interest = static_cast<RUB>(
            alice.car_loan_balance * monthly_interest_rate);

        alice.car_loan_balance += monthly_interest;

        RUB actual_payment = alice.car_loan_payment;

        if (actual_payment > alice.car_loan_balance)
        {
            actual_payment = alice.car_loan_balance;
        }

        alice.car_loan_balance -= actual_payment;
        alice.car_loan_months_left--;

        if (alice.car_loan_months_left == 0 ||
            alice.car_loan_balance <= 0)
        {
            alice.car_loan_balance = 0;
            alice.car_loan_payment = 0;
            alice.car_loan_months_left = 0;
        }
    }
}

void ivan_start_mortgage()
{
    const RUB apartment_price = 4'500'000;
    const int mortgage_duration_months = 180;
    const double annual_interest_rate = 0.09;
    const double monthly_interest_rate = annual_interest_rate / 12.0;

    if (year == 2029 && month == 1 &&
        ivan.mortgage_balance == 0)
    {
        RUB down_payment = ivan.cash + ivan.deposit;
        if (down_payment > apartment_price)
        {
            down_payment = apartment_price;
        }

        RUB from_cash = std::min(ivan.cash, down_payment);
        ivan.cash -= from_cash;
        RUB from_deposit = down_payment - from_cash;
        ivan.deposit -= from_deposit;

        RUB loan_amount = apartment_price - down_payment;
        ivan.mortgage_balance = loan_amount;
        ivan.mortgage_months_left = mortgage_duration_months;

        const double payment_factor =
            std::pow(1.0 + monthly_interest_rate,
                     mortgage_duration_months);

        ivan.mortgage_payment = static_cast<RUB>(
            loan_amount * monthly_interest_rate * payment_factor /
            (payment_factor - 1.0));

        std::cout << "  [Mortgage] down payment = "
                  << down_payment << ", loan = "
                  << loan_amount << " RUB\n";
    }
}

void ivan_pay_mortgage()
{
    const double annual_interest_rate = 0.09;
    const double monthly_interest_rate = annual_interest_rate / 12.0;

    if (ivan.mortgage_balance > 0 &&
        ivan.mortgage_months_left > 0)
    {
        RUB monthly_interest = static_cast<RUB>(
            ivan.mortgage_balance * monthly_interest_rate);

        ivan.mortgage_balance += monthly_interest;

        RUB actual_payment = ivan.mortgage_payment;

        if (actual_payment > ivan.mortgage_balance)
        {
            actual_payment = ivan.mortgage_balance;
        }

        ivan.mortgage_balance -= actual_payment;
        ivan.mortgage_months_left--;

        if (ivan.mortgage_months_left == 0 ||
            ivan.mortgage_balance <= 0)
        {
            ivan.mortgage_balance = 0;
            ivan.mortgage_payment = 0;
            ivan.mortgage_months_left = 0;
        }
    }
}

RUB calculate_progressive_tax(RUB annual_income)
{
    const RUB first_limit = 2'400'000;
    const RUB second_limit = 5'000'000;
    const RUB third_limit = 20'000'000;
    const RUB fourth_limit = 50'000'000;

    if (annual_income <= 0)
    {
        return 0;
    }

    if (annual_income <= first_limit)
    {
        return static_cast<RUB>(
            annual_income * 0.13);
    }

    if (annual_income <= second_limit)
    {
        return static_cast<RUB>(
            312'000 +
            (annual_income - first_limit) * 0.15);
    }

    if (annual_income <= third_limit)
    {
        return static_cast<RUB>(
            702'000 +
            (annual_income - second_limit) * 0.18);
    }

    if (annual_income <= fourth_limit)
    {
        return static_cast<RUB>(
            3'402'000 +
            (annual_income - third_limit) * 0.20);
    }

    return static_cast<RUB>(
        9'402'000 +
        (annual_income - fourth_limit) * 0.22);
}

void alice_tax()
{
    RUB total_tax = calculate_progressive_tax(
        alice.yearly_salary);

    RUB tax = total_tax - alice.taxes_paid;

    if (tax < 0)
    {
        tax = 0;
    }

    alice.cash -= tax;
    alice.taxes_paid += tax;
}

void ivan_tax()
{
    RUB total_tax = calculate_progressive_tax(
        ivan.yearly_salary);

    RUB tax = total_tax - ivan.taxes_paid;

    if (tax < 0)
    {
        tax = 0;
    }

    ivan.cash -= tax;
    ivan.taxes_paid += tax;
}

void alice_tax_refund()
{
    const double refund_rate = 0.07;

    RUB refund = static_cast<RUB>(
        alice.yearly_salary * refund_rate);

    if (refund > alice.taxes_paid)
    {
        refund = alice.taxes_paid;
    }

    alice.taxes_refund = refund;
    alice.cash += refund;
    alice.taxes_paid = 0;
    alice.yearly_salary = 0;
}

void ivan_tax_refund()
{
    const double refund_rate = 0.10;

    RUB refund = static_cast<RUB>(
        ivan.yearly_salary * refund_rate);

    if (refund > ivan.taxes_paid)
    {
        refund = ivan.taxes_paid;
    }

    ivan.taxes_refund = refund;
    ivan.cash += refund;
    ivan.taxes_paid = 0;
    ivan.yearly_salary = 0;
}

void update_saving_mode(Person& person, RUB base_expenses)
{
    RUB reserves = person.cash + person.deposit;
    RUB enter_threshold = base_expenses * 2;
    RUB exit_threshold = base_expenses * 4;

    if (!person.saving_mode && reserves < enter_threshold)
    {
        person.saving_mode = true;
    }
    else if (person.saving_mode && reserves > exit_threshold)
    {
        person.saving_mode = false;
    }
}

RUB actual_monthly_expenses(const Person& person)
{
    if (!person.saving_mode)
    {
        return base_monthly_expenses(person);
    }

    RUB food_spent = person.food_exp * 75 / 100;
    RUB transport_spent = person.transport_exp * 60 / 100;
    RUB gasoline_spent = person.gasoline_exp * 60 / 100;
    RUB pet_spent = person.petexp * 85 / 100;

    return food_spent
         + transport_spent
         + gasoline_spent
         + person.utilities_exp
         + person.rent_exp
         + person.car_loan_payment
         + person.mortgage_payment
         + person.gym_exp
         + pet_spent;
}

void withdraw_from_deposit_if_needed(
    Person& person,
    RUB monthly_expenses)
{
    if (person.cash < monthly_expenses)
    {
        RUB shortage = monthly_expenses - person.cash;
        RUB withdrawal = shortage;

        if (withdrawal > person.deposit)
        {
            withdrawal = person.deposit;
        }

        person.deposit -= withdrawal;
        person.cash += withdrawal;
    }

    person.cash -= monthly_expenses;

    if (person.cash < 0)
    {
        person.debt_balance += -person.cash;
        person.cash = 0;
    }
}

void apply_surplus(Person& person, RUB base_expenses)
{
    RUB reserve = base_expenses * 2;

    if (person.cash <= reserve)
    {
        return;
    }

    RUB surplus = person.cash - reserve;
    person.cash -= surplus;

    bool has_loan = (person.car_loan_balance > 0)
                 || (person.mortgage_balance > 0);

    if (!has_loan)
    {
        person.deposit += surplus;
        return;
    }

    RUB prepay = surplus / 2;
    RUB to_deposit = surplus - prepay;

    if (person.car_loan_balance > 0 && prepay > 0)
    {
        RUB amount = std::min(prepay, person.car_loan_balance);
        person.car_loan_balance -= amount;
        prepay -= amount;

        if (person.car_loan_balance <= 0)
        {
            person.car_loan_balance = 0;
            person.car_loan_months_left = 0;
            person.car_loan_payment = 0;
        }
    }

    if (person.mortgage_balance > 0 && prepay > 0)
    {
        RUB amount = std::min(prepay, person.mortgage_balance);
        person.mortgage_balance -= amount;
        prepay -= amount;

        if (person.mortgage_balance <= 0)
        {
            person.mortgage_balance = 0;
            person.mortgage_months_left = 0;
            person.mortgage_payment = 0;
        }
    }

    person.deposit += to_deposit + prepay;
}

void apply_sudden_illness(Person& person, const char* name)
{
    if (!chance(2))
    {
        return;
    }

    RUB bill = rand_range(15'000, 60'000);
    person.cash -= bill;

    std::cout << "  [Illness] " << name
              << " paid " << bill << " RUB\n";
}

void apply_car_repair(Person& person, const char* name)
{
    if (!chance(4))
    {
        return;
    }

    RUB bill = rand_range(10'000, 50'000);
    person.cash -= bill;

    std::cout << "  [Car repair] " << name
              << " paid " << bill << " RUB\n";
}

void apply_sudden_events()
{
    apply_sudden_illness(alice, "Alice");
    apply_sudden_illness(ivan, "Ivan");

    if (year >= 2027 && year <= 2035)
    {
        apply_car_repair(alice, "Alice");
    }
}

void update_extra_job(Person& person, RUB base_expenses)
{
    RUB reserves = person.cash + person.deposit;

    bool too_rich_for_part_time = person.job2.active
        && person.debt_balance == 0
        && reserves > base_expenses * 4;

    if (too_rich_for_part_time)
    {
        person.job2.active = false;
        person.job2.salary = 0;
        person.job2_search_months_left = 0;
        return;
    }

    bool need_extra_money = (person.debt_balance > 0)
        || (reserves < base_expenses * 2);

    if (need_extra_money && !person.job2.active)
    {
        if (person.job2_search_months_left <= 0)
        {
            person.job2_search_months_left = 2;
        }

        person.job2_search_months_left--;

        if (person.job2_search_months_left == 0)
        {
            RUB base = person.job1.active ? person.job1.salary : 0;
            RUB part_time = base * 35 / 100;

            if (part_time < 20'000)
            {
                part_time = 20'000;
            }

            person.job2.salary = part_time;
            person.job2.active = true;
        }
    }
}

void handle_debt(Person& person)
{
    if (person.debt_balance > 0 && person.cash > 0)
    {
        RUB payment = person.cash;
        if (payment > person.debt_balance)
        {
            payment = person.debt_balance;
        }

        person.cash -= payment;
        person.debt_balance -= payment;
    }

    if (person.debt_balance > 0)
    {
        const double monthly_penalty = 0.005;
        person.debt_balance += static_cast<RUB>(
            person.debt_balance * monthly_penalty);
    }
}

void alice_print()
{
    std::cout << "\nFinal report: Alice\n";
    std::cout << "Cash: " << alice.cash << " RUB\n";
    std::cout << "Bank deposit: " << alice.deposit << " RUB\n";
    std::cout << "Car loan balance: "
              << alice.car_loan_balance << " RUB\n";
    std::cout << "Debt balance: "
              << alice.debt_balance << " RUB\n";
    std::cout << "Job1 active: "
              << (alice.job1.active ? "yes" : "no") << "\n";
    std::cout << "Job2 active: "
              << (alice.job2.active ? "yes" : "no") << "\n";
    if (alice.job2.active)
    {
        std::cout << "Job2 salary: "
                  << alice.job2.salary << " RUB\n";
    }
    std::cout << "Saving mode at finish: "
              << (alice.saving_mode ? "ON" : "off") << "\n";
    std::cout << "Total capital: "
              << alice.cash + alice.deposit
                 - alice.car_loan_balance - alice.debt_balance
              << " RUB\n";
}

void ivan_print()
{
    std::cout << "\nFinal report: Ivan\n";
    std::cout << "Cash: " << ivan.cash << " RUB\n";
    std::cout << "Bank deposit: " << ivan.deposit << " RUB\n";
    std::cout << "Mortgage balance: "
              << ivan.mortgage_balance << " RUB\n";
    std::cout << "Debt balance: "
              << ivan.debt_balance << " RUB\n";
    std::cout << "Job1 active: "
              << (ivan.job1.active ? "yes" : "no") << "\n";
    std::cout << "Job2 active: "
              << (ivan.job2.active ? "yes" : "no") << "\n";
    if (ivan.job2.active)
    {
        std::cout << "Job2 salary: "
                  << ivan.job2.salary << " RUB\n";
    }
    std::cout << "Saving mode at finish: "
              << (ivan.saving_mode ? "ON" : "off") << "\n";
    std::cout << "Total capital: "
              << ivan.cash + ivan.deposit
                 - ivan.mortgage_balance - ivan.debt_balance
              << " RUB\n";
}

void alice_gym()
{
    const double annual_inflation = 0.05;
    const int years_passed = year - 2026;

    alice.gym_pass_cost = static_cast<RUB>(
        18'000 * std::pow(1.0 + annual_inflation, years_passed));

    alice.gym_exp = 0;

    if (month == 9 && !alice.saving_mode)
    {
        alice.gym_exp = alice.gym_pass_cost;
    }
}

void alice_healthy_food()
{
    const double annual_inflation = 0.05;
    const int years_passed = year - 2026;
    const double inflation_factor =
        std::pow(1.0 + annual_inflation, years_passed);

    static const double vegetable_season[12] =
        {1.20, 1.18, 1.12, 1.05, 0.95, 0.85,
         0.80, 0.78, 0.85, 0.95, 1.05, 1.15};

    static const double fruit_season[12] =
        {1.15, 1.12, 1.08, 1.02, 0.92, 0.80,
         0.75, 0.72, 0.82, 0.95, 1.08, 1.12};

    static const double dairy_season[12] =
        {1.05, 1.05, 1.03, 1.00, 0.98, 0.97,
         0.97, 0.97, 0.98, 1.00, 1.03, 1.05};

    const int m = month - 1;

    alice.protein_food_exp = static_cast<RUB>(
        6'000 * inflation_factor);

    alice.grains_food_exp = static_cast<RUB>(
        3'000 * inflation_factor);

    alice.vegetables_food_exp = static_cast<RUB>(
        4'000 * inflation_factor * vegetable_season[m]);

    alice.fruits_food_exp = static_cast<RUB>(
        3'000 * inflation_factor * fruit_season[m]);

    alice.dairy_food_exp = static_cast<RUB>(
        3'000 * inflation_factor * dairy_season[m]);

    alice.healthy_fats_exp = static_cast<RUB>(
        1'500 * inflation_factor);

    alice.other_food_exp = static_cast<RUB>(
        1'500 * inflation_factor);

    alice.food_exp =
        alice.protein_food_exp +
        alice.grains_food_exp +
        alice.vegetables_food_exp +
        alice.fruits_food_exp +
        alice.dairy_food_exp +
        alice.healthy_fats_exp +
        alice.other_food_exp;
}

void simulation()
{
    year = 2026;
    month = 9;

    alice.cash = 300'000;
    alice.deposit = 500'000;
    alice.taxes_paid = 0;
    alice.taxes_refund = 0;
    alice.yearly_salary = 0;
    alice.car_loan_balance = 0;
    alice.car_loan_payment = 0;
    alice.car_loan_months_left = 0;
    alice.debt_balance = 0;
    alice.job2_search_months_left = 0;
    alice.job1.active = false;
    alice.job2.active = false;
    alice.saving_mode = false;

    ivan.cash = 100'000;
    ivan.deposit = 200'000;
    ivan.taxes_paid = 0;
    ivan.taxes_refund = 0;
    ivan.yearly_salary = 0;
    ivan.mortgage_balance = 0;
    ivan.mortgage_payment = 0;
    ivan.mortgage_months_left = 0;
    ivan.debt_balance = 0;
    ivan.job2_search_months_left = 0;
    ivan.job1.active = false;
    ivan.job2.active = false;
    ivan.saving_mode = false;

    alice_jobs();
    ivan_jobs();

    alice_pet();
    ivan_pet();

    alice_transport();
    alice_gasoline();
    ivan_transport();

    ivan_food();

    alice_utilities();
    ivan_utilities();

    alice_rent();
    ivan_rent();

    while (!(year == 2044 && month == 9))
    {
        if (year == 2027 && month == 1)
        {
            alice_start_car_loan();
        }

        if (year == 2029 && month == 1)
        {
            ivan_start_mortgage();
        }

        RUB alice_pay = alice.total_salary();
        RUB ivan_pay = ivan.total_salary();

        alice.cash += alice_pay;
        ivan.cash += ivan_pay;

        alice.yearly_salary += alice_pay;
        ivan.yearly_salary += ivan_pay;

        alice_tax();
        ivan_tax();

        alice_deposit();
        ivan_deposit();

        alice_pay_car_loan();
        ivan_pay_mortgage();

        alice_healthy_food();
        alice_gym();
        alice_gasoline();

        alice_rent();
        ivan_rent();

        RUB alice_base = base_monthly_expenses(alice);
        RUB ivan_base = base_monthly_expenses(ivan);

        update_saving_mode(alice, alice_base);
        update_saving_mode(ivan, ivan_base);

        RUB alice_monthly_expenses = actual_monthly_expenses(alice);
        RUB ivan_monthly_expenses = actual_monthly_expenses(ivan);

        apply_sudden_events();

        withdraw_from_deposit_if_needed(
            alice, alice_monthly_expenses);

        withdraw_from_deposit_if_needed(
            ivan, ivan_monthly_expenses);

        update_extra_job(alice, alice_base);
        update_extra_job(ivan, ivan_base);

        handle_debt(alice);
        handle_debt(ivan);

        apply_surplus(alice, alice_base);
        apply_surplus(ivan, ivan_base);

        std::cout << "\nYear: " << year
                  << ", Month: " << month << '\n';

        std::cout << "Alice:\n";
        std::cout << "  Job1 salary: "
                  << (alice.job1.active ? alice.job1.salary : 0)
                  << " RUB\n";
        std::cout << "  Job2 salary: "
                  << (alice.job2.active ? alice.job2.salary : 0)
                  << " RUB\n";
        std::cout << "  Cash: " << alice.cash << " RUB\n";
        std::cout << "  Bank deposit: " << alice.deposit << " RUB\n";
        std::cout << "  Debt balance: "
                  << alice.debt_balance << " RUB\n";
        std::cout << "  Food expenses: " << alice.food_exp << " RUB\n";
        std::cout << "  Utilities: " << alice.utilities_exp << " RUB\n";
        std::cout << "  Rent: " << alice.rent_exp << " RUB\n";
        std::cout << "  Transport: " << alice.transport_exp << " RUB\n";
        std::cout << "  Gasoline: " << alice.gasoline_exp << " RUB\n";
        std::cout << "  Pet expenses: " << alice.petexp << " RUB\n";
        std::cout << "  Car loan payment: "
                  << alice.car_loan_payment << " RUB\n";
        std::cout << "  Car loan balance: "
                  << alice.car_loan_balance << " RUB\n";
        std::cout << "  Gym expense: "
                  << alice.gym_exp << " RUB\n";
        std::cout << "  Total monthly expenses: "
                  << alice_monthly_expenses << " RUB\n";
        std::cout << "  Saving mode: "
                  << (alice.saving_mode ? "ON" : "off") << "\n";

        std::cout << "Ivan:\n";
        std::cout << "  Job1 salary: "
                  << (ivan.job1.active ? ivan.job1.salary : 0)
                  << " RUB\n";
        std::cout << "  Job2 salary: "
                  << (ivan.job2.active ? ivan.job2.salary : 0)
                  << " RUB\n";
        std::cout << "  Cash: " << ivan.cash << " RUB\n";
        std::cout << "  Bank deposit: " << ivan.deposit << " RUB\n";
        std::cout << "  Debt balance: "
                  << ivan.debt_balance << " RUB\n";
        std::cout << "  Food expenses: " << ivan.food_exp << " RUB\n";
        std::cout << "  Utilities: " << ivan.utilities_exp << " RUB\n";
        std::cout << "  Rent: " << ivan.rent_exp << " RUB\n";
        std::cout << "  Transport: " << ivan.transport_exp << " RUB\n";
        std::cout << "  Pet expenses: " << ivan.petexp << " RUB\n";
        std::cout << "  Mortgage payment: "
                  << ivan.mortgage_payment << " RUB\n";
        std::cout << "  Mortgage balance: "
                  << ivan.mortgage_balance << " RUB\n";
        std::cout << "  Total monthly expenses: "
                  << ivan_monthly_expenses << " RUB\n";
        std::cout << "  Saving mode: "
                  << (ivan.saving_mode ? "ON" : "off") << "\n";

        month++;

        if (month > 12)
        {
            month = 1;
            year++;

            alice_tax_refund();
            ivan_tax_refund();

            alice_jobs();
            ivan_jobs();

            alice_pet();
            ivan_pet();

            alice_transport();
            alice_gasoline();
            ivan_transport();

            ivan_food();

            alice_utilities();
            ivan_utilities();

            alice_rent();
            ivan_rent();
        }
    }
}

int main()
{
    simulation();
    alice_print();
    ivan_print();

    return 0;
}