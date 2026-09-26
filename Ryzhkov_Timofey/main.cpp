#include <stdio.h>
#include <cmath>

using RUB = unsigned long long int;

struct Person {
    unsigned int age = 22;
    RUB salary = 140000;
    RUB additional_income = 40000;
    RUB cash = 150000;
};

struct Expenses {
    RUB rent = 45000;
    RUB food = 30000;
    RUB utilities = 5000;
    RUB transport = 7000;
    RUB other = 10000;
    RUB flowers = 5000;
    RUB date = 5000;
};

struct Car {
    unsigned int mileage = 111000;           //Пробег
    unsigned int annual_mileage = 18000;
    unsigned int service_interval = 10000;
    RUB service_cost = 4000;
    unsigned int last_service_mileage = 110000;
};

struct Deposit {
    RUB balance = 150000;
    RUB girlfriend_contribution = 30000;
    double annual_rate = 14;
};

struct Apartment {
    RUB price = 12000000;
    unsigned int down_payment_percent = 25;
    RUB emergency_fund = 400000;
    bool purchased = false;
};

struct Mortgage {
    double annual_rate = 15;
    unsigned int term_years = 15;
    RUB loan_amount = 0;
    RUB monthly_payment = 0;
    RUB remaining_debt = 0;
    bool active = false;
};

RUB calculate_month_expenses(const Expenses& expenses) {
    return expenses.rent + expenses.food + expenses.utilities + expenses.transport + expenses.other + expenses.flowers + expenses.date;
}

RUB calculate_month_income(const Person& person) {
    return person.salary + person.additional_income;
}   

//Повышение зарплаты
void update_salary(Person& person, int year)
{
    if (year == 4) {
        person.salary = 200000;
        printf("Salary inscreased to %llu\n", person.salary);
    }
}

RUB calculate_holiday_expenses(int month) {
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

RUB calculate_car_expenses(Car& car, int month)
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

//Совместные накопления на вкладе для оплаты ипотеки
void update_deposit (Deposit& deposit, Person& person, RUB free_money) {
    deposit.balance += deposit.girlfriend_contribution;

    RUB contribution = free_money * 50 / 100;
    person.cash -= contribution;
    deposit.balance += contribution;

}

//Начисление процентов по вкладу
void calculate_deposit_interest(Deposit& deposit){
    RUB interest = deposit.balance * deposit.annual_rate / 100 / 12;
    deposit.balance += interest;
}

//Проверка: хватает ли средств для покупки квартиры
void check_apartment_purchase(Deposit& deposit, Apartment& apartment, Mortgage& mortgage) 
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
RUB calculate_mortgage_payment(const Mortgage& mortgage)
{
    double monthly_rate = mortgage.annual_rate / 100 / 12;
    unsigned int total_month = mortgage.term_years * 12;
    double coefficient = pow(1 + monthly_rate, total_month);
    double payment = mortgage.loan_amount * monthly_rate * coefficient / (coefficient - 1);

    return static_cast<RUB>(payment);
}

//Оплата ипотеки 
void pay_mortgage(Mortgage& mortgage, Deposit& deposit)
{
    if (!mortgage.active) {
        return;
    }

    double monthly_rate = mortgage.annual_rate / 100 / 12;
    RUB interest = mortgage.remaining_debt * monthly_rate;
    RUB principal_payment = mortgage.monthly_payment - interest;

    if (principal_payment >= mortgage.remaining_debt) {
        RUB last_payment = mortgage.remaining_debt + interest;

        if (deposit.balance >= last_payment) {
            deposit.balance -= last_payment;
            mortgage.remaining_debt = 0;
            mortgage.active = false;

            printf("Mortgage fully paid\n");
        }

        return;
    }

    if (deposit.balance >= mortgage.monthly_payment) {
        deposit.balance -= mortgage.monthly_payment;
        mortgage.remaining_debt -= principal_payment;
    }
}

void simulate_month(Person& person, const Expenses& expenses, int month, Car& car, Deposit& deposit, Apartment& apartment, Mortgage& mortgage)
{
    RUB month_income = calculate_month_income(person);
    RUB month_expenses = calculate_month_expenses(expenses);

    if (apartment.purchased) {
        month_expenses -= expenses.rent;
    }

    month_expenses += calculate_holiday_expenses(month);
    month_expenses += calculate_car_expenses(car, month);
 
    printf("Monthly expenses: %llu\n", month_expenses);
    person.cash += month_income;
    
    if (person.cash >= month_expenses) {
        person.cash -= month_expenses;
    }

    RUB free_money = 0;

    if (month_income >= month_expenses) {
        free_money = month_income - month_expenses;
    }

    update_deposit(deposit, person, free_money);
    calculate_deposit_interest(deposit);
    check_apartment_purchase(deposit, apartment, mortgage);
    pay_mortgage(mortgage, deposit);
}

int main()
{
    Person person;
    printf("Age: %u\n", person.age);
    printf("Salary: %llu\n", person.salary);
    printf("Additional Income: %llu\n", person.additional_income);
    printf("Cash: %llu\n", person.cash);
    RUB month_income = calculate_month_income(person);
    printf("Monthly Income: %llu\n", month_income);

    Expenses expenses;
    RUB month_expenses = calculate_month_expenses(expenses);
    printf("Monthly Expenses: %llu\n", month_expenses);

    Car car;

    Deposit deposit; 

    Apartment apartment;

    Mortgage mortgage;
    mortgage.loan_amount = apartment.price * (100 - apartment.down_payment_percent) / 100;
    mortgage.monthly_payment = calculate_mortgage_payment(mortgage);
    printf("Mortgage payment: %llu\n", mortgage.monthly_payment);
    
    RUB free_money = month_income - month_expenses;
    printf("Free Money: %llu\n", free_money); 

    for (int year = 1; year < 10; year++)
    {
        printf("\nYEAR %d\n", year);

        update_salary(person, year);
        
        for (int month = 1; month <= 12; month++)
        {
            simulate_month(person, expenses, month, car, deposit, apartment, mortgage);

            printf("Month %d: %llu\n", month, person.cash);
            printf("Mileage: %u km\n", car.mileage);
            printf("Deposit balance: %llu\n", deposit.balance);

            if (mortgage.active) {
                printf("Mortgage debt: %llu\n", mortgage.remaining_debt);
            }
    }
    }
}
