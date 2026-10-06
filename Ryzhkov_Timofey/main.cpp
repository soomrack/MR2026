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
};


struct Mortgage {
    double annual_rate;
    unsigned int term_years;
    RUB loan_amount;
    RUB monthly_payment;
    RUB remaining_debt;
    bool active;
};


Person person;
Expenses expenses;
Car car;
Deposit deposit;
Investment investment;
Apartment apartment;
Mortgage mortgage;

RUB month_income = 0;
RUB month_expenses = 0;
RUB free_money = 0;


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
    srand(time(NULL));

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

    mortgage.annual_rate = 15;
    mortgage.term_years = 15;
    mortgage.loan_amount = apartment.price * (100 - apartment.down_payment_percent) / 100;
    mortgage.monthly_payment = calculate_mortgage_payment();
    mortgage.remaining_debt = 0;
    mortgage.active = false;
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


//Повышение зарплаты
void update_salary(int year)
{
    if (year == 4) {
        person.salary = 200000;
        printf("Salary inscreased to %llu\n", person.salary);
    }
}


void person_income()
{
    month_income = calculate_month_income();
    person.cash += month_income;
}


RUB calculate_holiday_expenses(int month)
{
    RUB holiday_expenses = 0;

    //Подарок на 14 февраля
    if (month == 2) {
        holiday_expenses += 5000;
    }

    //Подарки на 8 марта
    if (month == 3) {
        holiday_expenses += 5000;
    }

    //Подарки на годовщину
    if (month == 9) {
        holiday_expenses += 10000;
    }

    //Подарки на день Рождения
    if (month == 11) {
        holiday_expenses += 10000;
    }

    //Подарки на Новый год
    if (month == 12) {
        holiday_expenses += 10000;
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
        car_expenses += 5000;
    }

    //Страховка и налог на автомобиль
    if (month == 7) {
        car_expenses += 20500;
    }

    return car_expenses;
}


void person_expenses(int month)
{
    month_expenses = calculate_month_expenses();

    if (apartment.purchased) {
        month_expenses -= expenses.rent;
    }

    month_expenses += calculate_holiday_expenses(month);
    month_expenses += calculate_car_expenses(month);

    if (person.cash >= month_expenses) {
        person.cash -= month_expenses;
    }

    free_money = 0;

    if (month_income >= month_expenses) {
        free_money = month_income - month_expenses;
    }
}


//Совместные накопления на вкладе для оплаты ипотеки
void update_deposit()
{
    deposit.balance += deposit.girlfriend_contribution;

    RUB contribution = free_money * 50 / 100;
    person.cash -= contribution;
    deposit.balance += contribution;
}


//Накопление на инвестиционный счет
void update_investment()
{
    RUB investment_contribution = free_money * 50 / 100;
    RUB stocks_contribution = investment_contribution / 2;
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
void check_apartment_purchase()
{
    if (apartment.purchased) {
        return;
    }

    RUB down_payment = apartment.price * apartment.down_payment_percent / 100;
    RUB required_money = down_payment + apartment.emergency_fund;

    if (deposit.balance >= required_money) {
        deposit.balance -= down_payment;

        apartment.purchased = true;
        mortgage.active = true;
        mortgage.remaining_debt = mortgage.loan_amount;

        printf("Apartment purchased\n");
        printf("Down payment: %llu\n", down_payment);
        printf("Mortgage debt: %llu\n", mortgage.remaining_debt);
    }
}


//Расчет ежемесячного платежа


void pay_mortgage_from_deposit(RUB& payment)
{
    if (deposit.balance >= payment) {
        deposit.balance -= payment;
        payment = 0;
    }
    else {
        payment -= deposit.balance;
        deposit.balance = 0;
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


//Оплата ипотеки
void pay_mortgage()
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

    RUB payment_remainder = payment;

    pay_mortgage_from_deposit(payment_remainder);
    pay_mortgage_from_investments(payment_remainder);

    if (payment_remainder > 0) {
        printf("Not enough money for mortgage payment\n");
        return;
    }

    if (principal_payment >= mortgage.remaining_debt) {
        mortgage.remaining_debt = 0;
        mortgage.active = false;

        printf("Mortgage fully paid\n");
    }
    else {
        mortgage.remaining_debt -= principal_payment;
    }
}


void annual_events(int year)
{
    person.age += 1;

    update_salary(year + 1);
    update_stocks_rate();
}


void simulation()
{
    int year = 1;
    int month = 1;

    update_salary(year);
    update_stocks_rate();

    while (year <= 10) {

        person_income();
        person_expenses(month);

        update_deposit();
        update_investment();

        calculate_deposit_interest();
        calculate_investment_income();

        check_apartment_purchase();
        pay_mortgage();

        ++month;

        if (month == 13) {
            annual_events(year);

            ++year;
            month = 1;
        }
    }
}


void print()
{
    printf("Age: %u\n", person.age);
    printf("Salary: %llu\n", person.salary);
    printf("Additional Income: %llu\n", person.additional_income);
    printf("Cash: %llu\n", person.cash);

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
}


int main()
{
    init();
    simulation();
    print();

    return 0;
}