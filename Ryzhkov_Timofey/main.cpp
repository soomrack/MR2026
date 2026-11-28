#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

using RUB = unsigned long long int;


struct Person {
    unsigned int age;
    RUB salary;
    RUB additional_income;
    RUB cash;
};

struct Expenses {
    RUB rent;
    RUB food;
    RUB utilities;
    RUB transport;
    RUB other;
    RUB flowers;
    RUB date;
};

struct Car {
    unsigned int mileage;           //Пробег
    unsigned int annual_mileage;
    unsigned int service_interval;
    RUB service_cost;
    unsigned int last_service_mileage;
};

struct Deposit {
    RUB balance;
    RUB girlfriend_contribution;
    double annual_rate;
};

struct Investment {
    RUB stocks;
    RUB bonds;
    int stocks_annual_rate;
    double bonds_annual_rate;
};

struct Apartment {
    RUB price;
    unsigned int down_payment_percent;
    RUB emergency_fund;
    bool purchased;
    unsigned int purchase_year;
    unsigned int purchase_month;
    unsigned int purchase_age;
};

struct Mortgage {
    double annual_rate;
    unsigned int term_years;
    RUB loan_amount;
    RUB monthly_payment;
    RUB remaining_debt;
    bool active;
    unsigned int repayment_year;
    unsigned int repayment_month;
    unsigned int repayment_age;
};

struct Taxes {
    RUB annual_gross_income;
    RUB annual_net_income;
    RUB annual_tax_paid;
    RUB total_tax_paid;
    RUB total_tax_refund;
    RUB property_deduction_limit;
    RUB property_deduction_used;
    RUB property_tax_refund;
    RUB mortgage_interest_paid;
    RUB mortgage_deduction_limit;
    RUB mortgage_deduction_used;
    RUB mortgage_tax_refund;
    RUB annual_deduction_used;
};

struct Pet {
    bool owned;
    bool alive;

    unsigned int purchase_age;
    unsigned int age_months;
    unsigned int lifespan_months;

    RUB purchase_cost;
    RUB food_cost;
    RUB care_cost;
    RUB vaccination_cost;
    RUB veterinary_cost;

    RUB total_expenses;
};


Person person;
Expenses expenses;
Car car;
Deposit deposit;
Investment investment;
Apartment apartment;
Mortgage mortgage;
Taxes taxes;
Pet pet;
RUB month_income = 0;
RUB month_expenses = 0;
RUB free_money = 0;
unsigned int deposit_percent;
unsigned int stocks_percent;
unsigned int min_simulation_years;
unsigned int max_simulation_years;
RUB holiday_february = 5000;
RUB holiday_march = 5000;
RUB holiday_september = 10000;
RUB holiday_november = 10000;
RUB holiday_december = 10000;

RUB tire_change_cost = 5000;
RUB insurance_tax_cost = 20500;


RUB calculate_mortgage_payment()
{
    double monthly_rate = mortgage.annual_rate / 100 / 12;
    unsigned int total_month = mortgage.term_years * 12;
    double coefficient = pow(1 + monthly_rate, total_month);
    double payment = mortgage.loan_amount * monthly_rate * coefficient / (coefficient - 1);

    return static_cast<RUB>(payment);
}


void init()
{
    srand(42);

    person.age = 22;
    person.salary = 140000;
    person.additional_income = 40000;
    person.cash = 0;

    expenses.rent = 45000;
    expenses.food = 30000;
    expenses.utilities = 5000;
    expenses.transport = 7000;
    expenses.other = 10000;
    expenses.flowers = 5000;
    expenses.date = 5000;

    car.mileage = 111000;
    car.annual_mileage = 18000;
    car.service_interval = 10000;
    car.service_cost = 4000;
    car.last_service_mileage = 110000;

    deposit.balance = 150000;
    deposit.girlfriend_contribution = 30000;
    deposit.annual_rate = 14;

    investment.stocks = 75000;
    investment.bonds = 75000;
    investment.stocks_annual_rate = 0;
    investment.bonds_annual_rate = 14;

    apartment.price = 12000000;
    apartment.down_payment_percent = 25;
    apartment.emergency_fund = 400000;
    apartment.purchased = false;
    apartment.purchase_year = 0;
    apartment.purchase_month = 0;
    apartment.purchase_age = 0;

    mortgage.annual_rate = 15;
    mortgage.term_years = 15;
    mortgage.loan_amount = apartment.price * (100 - apartment.down_payment_percent) / 100;
    mortgage.monthly_payment = calculate_mortgage_payment();
    mortgage.remaining_debt = 0;
    mortgage.active = false;
    mortgage.repayment_year = 0;
    mortgage.repayment_month = 0;
    mortgage.repayment_age = 0;
    deposit_percent = 50;
    stocks_percent = 50;
    min_simulation_years = 10;
    max_simulation_years = 50;

    taxes.annual_gross_income = 0;
    taxes.annual_net_income = 0;
    taxes.annual_tax_paid = 0;
    taxes.total_tax_paid = 0;
    taxes.total_tax_refund = 0;
    taxes.property_deduction_limit = 2000000;
    taxes.property_deduction_used = 0;
    taxes.property_tax_refund = 0;
    taxes.mortgage_interest_paid = 0;
    taxes.mortgage_deduction_limit = 3000000;
    taxes.mortgage_deduction_used = 0;
    taxes.mortgage_tax_refund = 0;
    taxes.annual_deduction_used = 0;

    pet.owned = false;
    pet.alive = false;

    pet.purchase_age = 24;
    pet.age_months = 0;
    pet.lifespan_months = 12 * 12;

    pet.purchase_cost = 30000;
    pet.food_cost = 4000;
    pet.care_cost = 1000;
    pet.vaccination_cost = 5000;
    pet.veterinary_cost = 3000;

    pet.total_expenses = 0;
}


RUB calculate_month_expenses()
{
    return expenses.rent + expenses.food + expenses.utilities +
           expenses.transport + expenses.other + expenses.flowers +
           expenses.date;
}


RUB calculate_month_income()
{
    return person.salary + person.additional_income;
}

//Расчет НДФЛ по прогрессивной шкале
RUB calculate_income_tax(RUB income)
{
    if (income <= 2400000) {
        return income * 13 / 100;
    }

    if (income <= 5000000) {
        return 312000 + (income - 2400000) * 15 / 100;
    }

    if (income <= 20000000) {
        return 702000 + (income - 5000000) * 18 / 100;
    }

    if (income <= 50000000) {
        return 3402000 + (income - 20000000) * 20 / 100;
    }

    return 9402000 + (income - 50000000) * 22 / 100;
}

//Расчет начисленного дохода до уплаты НДФЛ
RUB calculate_gross_income(RUB net_income)
{
    RUB low = net_income;
    RUB high = net_income * 2;

    while (low < high) {

        RUB middle = low + (high - low) / 2;
        RUB tax = calculate_income_tax(middle);

        if (middle - tax < net_income) {
            low = middle + 1;
        }
        else {
            high = middle;
        }
    }

    return low;
}

void update_expenses()
{
    expenses.rent *= 1.06;
    expenses.food *= 1.06;
    expenses.utilities *= 1.06;
    expenses.transport *= 1.06;
    expenses.other *= 1.06;
    expenses.flowers *= 1.06;
    expenses.date *= 1.06;
}

//Повышение зарплаты
void update_salary(int year)
{
    if (year == 4) {
        person.salary = 200000;
        printf("Salary inscreased to %llu\n", person.salary);
    }
}

//Учет уплаченного НДФЛ
void update_income_tax()
{
    RUB previous_tax = taxes.annual_tax_paid;

    taxes.annual_net_income += month_income;

    taxes.annual_gross_income =
        calculate_gross_income(taxes.annual_net_income);

    taxes.annual_tax_paid =
        calculate_income_tax(taxes.annual_gross_income);

    RUB monthly_tax = taxes.annual_tax_paid - previous_tax;

    taxes.total_tax_paid += monthly_tax;
}

void person_income()
{
    month_income = calculate_month_income();
    person.cash += month_income;

    update_income_tax();
}


RUB calculate_holiday_expenses(int month)
{
    RUB holiday_expenses = 0;

    //Подарок на 14 февраля
    if (month == 2) {
        holiday_expenses += holiday_february;
    }

    //Подарки на 8 марта
    if (month == 3) {
        holiday_expenses += holiday_march;
    }

    //Подарки на годовщину
    if (month == 9) {
        holiday_expenses += holiday_september;
    }

    //Подарки на день Рождения
    if (month == 11) {
        holiday_expenses += holiday_november;
    }

    //Подарки на Новый год
    if (month == 12) {
        holiday_expenses += holiday_december;
    }

    return holiday_expenses;
}


RUB calculate_car_expenses(int month)
{
    RUB car_expenses = 0;

    //Увеличение пробега за месяц
    car.mileage += car.annual_mileage / 12;

    //Когда нужно проходить ТО
    if (car.mileage - car.last_service_mileage >= car.service_interval) {
        car_expenses += car.service_cost;
        car.last_service_mileage = car.mileage;
    }

    //Смена колес на автомобиле
    if (month == 4 || month == 10) {
        car_expenses += tire_change_cost;
    }

    //Страховка и налог на автомобиль
    if (month == 7) {
        car_expenses += insurance_tax_cost;
    }

    return car_expenses;
}

//Приобретение домашнего животного
RUB purchase_pet(RUB current_expenses)
{
    if (pet.owned) {
        return 0;
    }

    if (person.age < pet.purchase_age) {
        return 0;
    }

    RUB required_money = current_expenses +
                         pet.purchase_cost +
                         pet.food_cost +
                         pet.care_cost +
                         pet.vaccination_cost +
                         pet.veterinary_cost;

    if (person.cash < required_money) {
        return 0;
    }

    pet.owned = true;
    pet.alive = true;
    pet.age_months = 0;

    printf("Dog purchased at age %u\n", person.age);
    printf("Dog purchase cost: %llu RUB\n", pet.purchase_cost);

    return pet.purchase_cost;
}

//Расчет расходов на домашнее животное
RUB calculate_pet_expenses(RUB current_expenses)
{
    RUB pet_expenses = purchase_pet(current_expenses);

    if (!pet.alive) {
        return 0;
    }

    //Питание
    pet_expenses += pet.food_cost;

    //Гигиена и уход
    pet_expenses += pet.care_cost;

    //Ежегодная вакцинация
    if (pet.age_months % 12 == 0) {
        pet_expenses += pet.vaccination_cost;
    }

    //Ежегодный осмотр ветеринара
    if (pet.age_months % 12 == 0) {
        pet_expenses += pet.veterinary_cost;
    }

    pet.total_expenses += pet_expenses;

    return pet_expenses;
}

//Увеличение возраста домашнего животного
void update_pet_age()
{
    if (!pet.alive) {
        return;
    }

    ++pet.age_months;

    if (pet.age_months >= pet.lifespan_months) {
        pet.alive = false;

        printf("Dog died at age %u years\n",
               pet.age_months / 12);
    }
}

void person_expenses(int month)
{
    month_expenses = calculate_month_expenses();

    if (apartment.purchased) {
        month_expenses -= expenses.rent;
    }

    month_expenses += calculate_holiday_expenses(month);
    month_expenses += calculate_car_expenses(month);
    month_expenses += calculate_pet_expenses(month_expenses);

    if (person.cash >= month_expenses) {
        person.cash -= month_expenses;
    }

    free_money = 0;

    if (month_income >= month_expenses) {
        free_money = month_income - month_expenses;
    }
}

void girlfriend_income()
{
    deposit.balance += deposit.girlfriend_contribution;
}

//Совместные накопления на вкладе для оплаты ипотеки
void update_deposit()
{
    RUB contribution = free_money * deposit_percent / 100;
    person.cash -= contribution;
    deposit.balance += contribution;
}


//Накопление на инвестиционный счет
void update_investment()
{
    RUB investment_contribution = free_money -
                                  free_money * deposit_percent / 100;

    RUB stocks_contribution = investment_contribution * stocks_percent / 100;
    RUB bonds_contribution = investment_contribution - stocks_contribution;

    person.cash -= investment_contribution;
    investment.stocks += stocks_contribution;
    investment.bonds += bonds_contribution;
}


//Начисление дохода по акциям
void update_stocks_rate()
{
    int chance = rand() % 100;

    if (chance < 70) {
        investment.stocks_annual_rate = 8 + rand() % 10;
    }
    else {
        investment.stocks_annual_rate = -(3 + rand() % 5);
    }

    printf("Stocks annual rate: %d%%\n", investment.stocks_annual_rate);
}


//Начисление процентов по вкладу
void calculate_deposit_interest()
{
    RUB interest = deposit.balance * deposit.annual_rate / 100 / 12;
    deposit.balance += interest;
}


//Начисление процентов по инвестициям
void calculate_investment_income()
{
    RUB bonds_income = investment.bonds * investment.bonds_annual_rate / 100 / 12;
    investment.bonds += bonds_income;

    if (investment.stocks_annual_rate >= 0) {
        RUB stocks_income = investment.stocks * investment.stocks_annual_rate / 100 / 12;
        investment.stocks += stocks_income;
    }
    else {
        RUB stocks_loss = investment.stocks * (-investment.stocks_annual_rate) / 100 / 12;
        investment.stocks -= stocks_loss;
    }
}


//Проверка: хватает ли средств для покупки квартиры
void check_apartment_purchase(int year, int month)
{
    if (apartment.purchased) {
        return;
    }

    RUB down_payment = apartment.price * apartment.down_payment_percent / 100;
    RUB required_money = down_payment + apartment.emergency_fund;

    if (deposit.balance >= required_money) {
        deposit.balance -= down_payment;

        apartment.purchased = true;
        apartment.purchase_year = year;
        apartment.purchase_month = month;
        apartment.purchase_age = person.age;
        mortgage.active = true;
        mortgage.loan_amount = apartment.price - down_payment;
        mortgage.monthly_payment = calculate_mortgage_payment();
        mortgage.remaining_debt = mortgage.loan_amount;

        printf("Apartment purchased\n");
        printf("Down payment: %llu\n", down_payment);
        printf("Mortgage debt: %llu\n", mortgage.remaining_debt);
    }
}

void pay_mortgage_from_cash(RUB& payment)
{
    if (free_money >= payment) {
        person.cash -= payment;
        free_money -= payment;
        payment = 0;
    }
    else {
        person.cash -= free_money;
        payment -= free_money;
        free_money = 0;
    }
}

RUB available_deposit()
{
    if (deposit.balance > apartment.emergency_fund) {
        return deposit.balance - apartment.emergency_fund;
    }

    return 0;
}

//Расчет ежемесячного платежа
void pay_mortgage_from_deposit(RUB& payment)
{
    RUB available = available_deposit();

    if (available >= payment) {
        deposit.balance -= payment;
        payment = 0;
    }
    else {
        deposit.balance -= available;
        payment -= available;
    }
}


void pay_mortgage_from_investments(RUB& payment)
{
    if (payment == 0) {
        return;
    }

    RUB total_investments = investment.stocks + investment.bonds;

    if (total_investments < payment) {
        return;
    }

    RUB from_stocks = payment / 2;
    RUB from_bonds = payment - from_stocks;

    if (investment.stocks >= from_stocks &&
        investment.bonds >= from_bonds) {

        investment.stocks -= from_stocks;
        investment.bonds -= from_bonds;
    }
    else {
        if (investment.stocks < from_stocks) {
            from_stocks = investment.stocks;
            from_bonds = payment - from_stocks;
        }
        else {
            from_bonds = investment.bonds;
            from_stocks = payment - from_bonds;
        }

        investment.stocks -= from_stocks;
        investment.bonds -= from_bonds;
    }

    payment = 0;
}

//Фиксация даты погашения ипотеки
void save_mortgage_repayment_date(int year, int month)
{
    mortgage.repayment_year = year;
    mortgage.repayment_month = month;
    mortgage.repayment_age = person.age;
}

//Оплата ипотеки
void pay_mortgage(int year, int month)
{
    if (!mortgage.active) {
        return;
    }

    double monthly_rate = mortgage.annual_rate / 100 / 12;
    RUB interest = mortgage.remaining_debt * monthly_rate;
    RUB principal_payment = mortgage.monthly_payment - interest;
    RUB payment = mortgage.monthly_payment;

    if (principal_payment >= mortgage.remaining_debt) {
        payment = mortgage.remaining_debt + interest;
    }

    RUB available_money = free_money + available_deposit() +
                      investment.stocks + investment.bonds;

    if (available_money < payment) {
        printf("Not enough money for mortgage payment\n");
        return;
    }

    RUB payment_remainder = payment;

    pay_mortgage_from_cash(payment_remainder);
    pay_mortgage_from_deposit(payment_remainder);
    pay_mortgage_from_investments(payment_remainder);

    if (payment_remainder > 0) {
        printf("Not enough money for mortgage payment\n");
        return;
    }
    taxes.mortgage_interest_paid += interest;

    if (principal_payment >= mortgage.remaining_debt) {
        mortgage.remaining_debt = 0;
        mortgage.active = false;
        save_mortgage_repayment_date(year, month);

        printf("Mortgage fully paid\n");
    }
    else {
        mortgage.remaining_debt -= principal_payment;
    }
}

//Досрочное погашение ипотеки
void early_mortgage_repayment(int year, int month)
{
    if (!mortgage.active) {
        return;
    }

    if (available_deposit() >= mortgage.remaining_debt) {

        deposit.balance -= mortgage.remaining_debt;

        mortgage.remaining_debt = 0;
        mortgage.active = false;
        save_mortgage_repayment_date(year, month);

        printf("Mortgage repaid early\n");
    }
}

void update_apartment_price()
{
    if (!apartment.purchased) {
        apartment.price *= 1.06;
    }
}

void update_additional_expenses()
{
    holiday_february *= 1.06;
    holiday_march *= 1.06;
    holiday_september *= 1.06;
    holiday_november *= 1.06;
    holiday_december *= 1.06;

    car.service_cost *= 1.06;
    tire_change_cost *= 1.06;
    insurance_tax_cost *= 1.06;
}

//Ежегодная индексация доходов
void update_income()
{
    person.salary *= 1.05;
    person.additional_income *= 1.05;
    deposit.girlfriend_contribution *= 1.05;
}


//Индексация финансовой подушки
void update_emergency_fund()
{
    apartment.emergency_fund *= 1.06;
}

//Имущественный налоговый вычет
void calculate_property_deduction()
{
    if (!apartment.purchased) {
        return;
    }

    RUB remaining_deduction =
        taxes.property_deduction_limit -
        taxes.property_deduction_used;

    if (remaining_deduction == 0) {
        return;
    }

    RUB deduction = remaining_deduction;

    if (deduction > taxes.annual_gross_income) {
        deduction = taxes.annual_gross_income;
    }

    RUB taxable_income =
        taxes.annual_gross_income - deduction;

    RUB corrected_tax =
        calculate_income_tax(taxable_income);

    RUB refund = taxes.annual_tax_paid - corrected_tax;

    taxes.property_deduction_used += deduction;
    taxes.annual_deduction_used += deduction;
    taxes.property_tax_refund += refund;
    taxes.total_tax_refund += refund;

    deposit.balance += refund;

    printf("Property tax refund: %llu RUB\n", refund);
}

//Налоговый вычет по ипотечным процентам
void calculate_mortgage_deduction()
{
    if (!apartment.purchased) {
        return;
    }

    RUB eligible_interest = taxes.mortgage_interest_paid;

    if (eligible_interest > taxes.mortgage_deduction_limit) {
        eligible_interest = taxes.mortgage_deduction_limit;
    }

    RUB remaining_deduction =
        eligible_interest - taxes.mortgage_deduction_used;

    RUB taxable_income =
        taxes.annual_gross_income - taxes.annual_deduction_used;

    RUB deduction = remaining_deduction;

    if (deduction > taxable_income) {
        deduction = taxable_income;
    }

    if (deduction == 0) {
        return;
    }

    RUB tax_before = calculate_income_tax(taxable_income);

    RUB tax_after =
        calculate_income_tax(taxable_income - deduction);

    RUB refund = tax_before - tax_after;

    taxes.mortgage_deduction_used += deduction;
    taxes.annual_deduction_used += deduction;
    taxes.mortgage_tax_refund += refund;
    taxes.total_tax_refund += refund;

    deposit.balance += refund;

    printf("Mortgage tax refund: %llu RUB\n", refund);
}

//Ежегодный возврат налогов
void annual_tax_refund()
{
    calculate_property_deduction();
    calculate_mortgage_deduction();
}

//Сброс годовых налоговых показателей
void reset_annual_taxes()
{
    taxes.annual_gross_income = 0;
    taxes.annual_net_income = 0;
    taxes.annual_tax_paid = 0;
    taxes.annual_deduction_used = 0;
}

void annual_events(int year)
{
    reset_annual_taxes();
    person.age += 1;

    update_expenses();
    update_additional_expenses();
    update_apartment_price();
    update_emergency_fund();
    update_income();
    update_salary(year + 1);
    update_stocks_rate();
}

//Информация о домашнем животном
void print_pet()
{
    if (!pet.owned) {
        printf("Dog not purchased\n");
        return;
    }

    if (pet.alive) {
        printf("Dog is alive\n");
    }
    else {
        printf("Dog is dead\n");
    }

    printf("Dog age: %u years, %u months\n",
           pet.age_months / 12,
           pet.age_months % 12);

    printf("Total dog expenses: %llu RUB\n",
           pet.total_expenses);
}

void print_year(int year)
{
    printf("\n========== YEAR %d ==========\n", year);
    printf("Age: %u\n", person.age);
    printf("Monthly salary: %llu RUB\n", person.salary);
    printf("Monthly expenses: %llu RUB\n", month_expenses);
    printf("Annual gross income: %llu RUB\n", taxes.annual_gross_income);
    printf("Annual net income: %llu RUB\n", taxes.annual_net_income);
    printf("Annual income tax: %llu RUB\n", taxes.annual_tax_paid);
    printf("Total mortgage interest paid: %llu RUB\n",
       taxes.mortgage_interest_paid);
    printf("Total property tax refund: %llu RUB\n",
       taxes.property_tax_refund);
    printf("Total mortgage tax refund: %llu RUB\n",
       taxes.mortgage_tax_refund);

    printf("Deposit: %llu RUB\n", deposit.balance);
    printf("Stocks: %llu RUB\n", investment.stocks);
    printf("Bonds: %llu RUB\n", investment.bonds);

    printf("Apartment price: %llu RUB\n", apartment.price);

    if (apartment.purchased) {
    printf("Apartment purchased\n");

    if (mortgage.active) {
        printf("Mortgage debt: %llu RUB\n", mortgage.remaining_debt);
        printf("Mortgage payment: %llu RUB\n", mortgage.monthly_payment);
    }
    else {
        printf("Mortgage fully paid\n");
    }
}
else {
    printf("Apartment not purchased\n");
}
    print_pet();
}

//Проверка окончания симуляции
bool simulation_finished(int year, int month)
{
    unsigned int elapsed_months = (year - 1) * 12 + month - 1;

    if (elapsed_months >= max_simulation_years * 12) {
        return true;
    }

    if (elapsed_months >= min_simulation_years * 12 &&
        apartment.purchased &&
        !mortgage.active) {

        return true;
    }

    return false;
}

void simulation()
{
    int year = 1;
    int month = 1;

    update_salary(year);
    update_stocks_rate();

    while (!simulation_finished(year, month)) {

        person_income();
        person_expenses(month);
        update_pet_age();

        girlfriend_income();
        pay_mortgage(year, month);

        update_deposit();
        update_investment();

        calculate_deposit_interest();
        calculate_investment_income();

        early_mortgage_repayment(year, month);
        check_apartment_purchase(year, month);

        ++month;

        if (month == 13) {
            annual_tax_refund();
            print_year(year);

            if (!simulation_finished(year + 1, 1)) {
                annual_events(year);
            }
            else {
                person.age += 1;
            }

            ++year;
            month = 1;
        }
    }

    if (month != 1) {
        printf("\nFinal incomplete year:\n");
        printf("Completed months: %d\n", month - 1);
        print_year(year);
    }

    if (!apartment.purchased || mortgage.active) {
        printf("Simulation time limit reached\n");
    }
}

void print()
{
    printf("Age: %u\n", person.age);
    printf("Salary: %llu\n", person.salary);
    printf("Additional Income: %llu\n", person.additional_income);
    printf("Cash: %llu\n", person.cash);
    printf("Total income tax paid: %llu RUB\n", taxes.total_tax_paid);
    printf("Total tax refund: %llu RUB\n",
       taxes.total_tax_refund);
    printf("Property tax refund: %llu RUB\n",
       taxes.property_tax_refund);
    printf("Mortgage tax refund: %llu RUB\n",
       taxes.mortgage_tax_refund);
    printf("Total mortgage interest paid: %llu RUB\n",
       taxes.mortgage_interest_paid);
    printf("Net income tax paid: %llu RUB\n",
       taxes.total_tax_paid - taxes.total_tax_refund);

    printf("Mileage: %u km\n", car.mileage);
    printf("Deposit balance: %llu\n", deposit.balance);
    printf("Stocks: %llu\n", investment.stocks);
    printf("Bonds: %llu\n", investment.bonds);

    if (apartment.purchased) {
        printf("Apartment purchased\n");
    }

    if (mortgage.active) {
        printf("Mortgage debt: %llu\n", mortgage.remaining_debt);
    }
    else if (apartment.purchased) {
        printf("Mortgage fully paid\n");
    }

    if (apartment.purchased) {
        printf("\nApartment purchase:\n");
        printf("Year: %u\n", apartment.purchase_year);
        printf("Month: %u\n", apartment.purchase_month);
        printf("Age: %u\n", apartment.purchase_age);
    }

    if (apartment.purchased && !mortgage.active) {
        printf("\nMortgage repayment:\n");
        printf("Year: %u\n", mortgage.repayment_year);
        printf("Month: %u\n", mortgage.repayment_month);
        printf("Age: %u\n", mortgage.repayment_age);
    }
    print_pet();
}

int main()
{
    init();
    simulation();
    print();

    return 0;
}