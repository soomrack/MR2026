#include <iostream>
#include <cstdint>
#include <cmath>

//  ТИПЫ И КОНСТАНТЫ

using RUB = std::int64_t;

namespace period {
    constexpr int START_YEAR = 2026;
    constexpr int START_MONTH = 9;
    constexpr int END_YEAR = 2036;
    constexpr int END_MONTH = 9;
}

namespace rate {
    constexpr double SALARY = 0.08;
    constexpr double SALARY2 = 0.06;
    constexpr double DEPOSIT = 0.08;
    constexpr double MORTGAGE = 0.12;
    constexpr double FOOD = 0.09;
    constexpr double UTILITIES = 0.07;
    constexpr double RENT = 0.10;
    constexpr double TRANSPORT = 0.08;
    constexpr double HEALTHCARE = 0.06;
    constexpr double EDUCATION = 0.08;
    constexpr double ENTERTAINMENT = 0.07;
    constexpr double CLOTHING = 0.06;
    constexpr double PETS = 0.09;
    constexpr double CAR = 0.05;
    constexpr double OTHER = 0.07;
}

namespace tax {
    constexpr double INCOME_RATE = 0.13;
}

namespace months {
    constexpr int IN_YEAR = 12;
}

//  СТРУКТУРЫ ДАННЫХ

struct Person {
    RUB cash = 0;
    RUB salary = 0;
    RUB salary2 = 0;
    RUB deposit = 0;
    RUB mortgage_debt = 0;
    RUB property_value = 0;
    RUB car_value = 0;
    RUB credit_debt = 0;

    RUB monthly_income = 0;
    RUB monthly_expense = 0;
    RUB total_income = 0;
    RUB total_expense = 0;
    RUB total_tax_paid = 0;
    RUB total_tax_refunded = 0;
    RUB total_interest = 0;

    int age = 0;
    int experience_years = 0;
    int experience_years2 = 0;

    bool pays_rent = false;
    bool pays_mortgage = false;
    bool owns_property = false;
    bool owns_car = false;
    bool uses_public_transport = false;
    bool owns_dog = false;
    bool owns_cat = false;
    bool pays_health_insurance = false;
    bool pays_gym = false;
    bool pays_subscriptions = false;
    bool owns_deposit_account = false;
    bool owes_car_credit = false;
    bool receives_second_salary = false;
};

Person alice;

//  ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ

int months_elapsed(int year, int month) {
    return (year - period::START_YEAR) * months::IN_YEAR
         + (month - period::START_MONTH);
}

RUB inflated(RUB base, double r, int year, int month) {
    const int m = months_elapsed(year, month);
    if (m <= 0) {
        return base;
    }
    const double years = m / 12.0;
    return static_cast<RUB>(base * std::pow(1.0 + r, years));
}

//  ИНИЦИАЛИЗАЦИЯ

void alice_init() {
    alice.cash = 30'000;
    alice.salary = 30'000;
    alice.age = 20;
    alice.pays_rent = true;
    alice.uses_public_transport = true;
}

void alice_monthly_reset() {
    alice.monthly_income = 0;
    alice.monthly_expense = 0;
}

//  ДОХОДЫ: ОСНОВНАЯ РАБОТА

void alice_salary(const int year, const int month) {
    if (year == 2027 && month == 1) { alice.salary = 60'000; }
    if (year == 2028 && month == 6) { alice.salary = 80'000; }
    if (year == 2029 && month == 3) { alice.salary = 120'000; }
    if (year == 2030 && month == 9) { alice.salary = 130'000; }
    if (year == 2031 && month == 1) { alice.salary = 140'000; }
    if (year == 2032 && month == 6) { alice.salary = 150'000; }
    if (year == 2033 && month == 3) { alice.salary = 160'000; }
    if (year == 2034 && month == 9) { alice.salary = 170'000; }
    if (year == 2035 && month == 1) { alice.salary = 180'000; }
    if (year == 2036 && month == 6) { alice.salary = 190'000; }

    const RUB actual = inflated(alice.salary, rate::SALARY, year, month);
    alice.cash += actual;
    alice.monthly_income += actual;
    alice.total_income += actual;
}

void alice_bonus(const int year, const int month) {
    if (month == 12) {
        const RUB bonus = alice.salary / 5;
        alice.cash += bonus;
        alice.monthly_income += bonus;
        alice.total_income += bonus;
    }

    if (year == 2028 && month == 7) {
        const RUB vacation_bonus = 30'000;
        alice.cash += vacation_bonus;
        alice.monthly_income += vacation_bonus;
        alice.total_income += vacation_bonus;
    }

    if (year == 2030 && month == 1) {
        const RUB annual_bonus = 50'000;
        alice.cash += annual_bonus;
        alice.monthly_income += annual_bonus;
        alice.total_income += annual_bonus;
    }
}

void alice_taxes(const int year, const int month) {
    const RUB salary_tax = static_cast<RUB>(alice.salary * tax::INCOME_RATE);
    alice.cash -= salary_tax;
    alice.monthly_expense += salary_tax;
    alice.total_expense += salary_tax;
    alice.total_tax_paid += salary_tax;

    if (year == 2029 && month == 1) {
        const RUB property_tax = 5'000;
        alice.cash -= property_tax;
        alice.monthly_expense += property_tax;
        alice.total_expense += property_tax;
    }

    if (year == 2030 && month == 1) {
        const RUB property_tax = 7'000;
        alice.cash -= property_tax;
        alice.monthly_expense += property_tax;
        alice.total_expense += property_tax;
    }
}

void alice_promotion(const int year, const int month) {
    if (month == 1 && year >= 2027) {
        alice.experience_years += 1;
    }
}

//  ДОХОДЫ: ВТОРАЯ РАБОТА

void alice_salary2(const int year, const int month) {
    if (year == 2027 && month == 3) {
        alice.receives_second_salary = true;
        alice.salary2 = 20'000;
    }

    if (!alice.receives_second_salary) {
        return;
    }

    if (year == 2028 && month == 9) { alice.salary2 = 40'000; }
    if (year == 2029 && month == 6) { alice.salary2 = 70'000; }
    if (year == 2030 && month == 6) { alice.salary2 = 90'000; }
    if (year == 2031 && month == 6) { alice.salary2 = 100'000; }
    if (year == 2032 && month == 6) { alice.salary2 = 110'000; }
    if (year == 2033 && month == 6) { alice.salary2 = 120'000; }
    if (year == 2034 && month == 6) { alice.salary2 = 130'000; }
    if (year == 2035 && month == 6) { alice.salary2 = 140'000; }

    const RUB actual = inflated(alice.salary2, rate::SALARY2, year, month);
    alice.cash += actual;
    alice.monthly_income += actual;
    alice.total_income += actual;
}

void alice_bonus2(const int year, const int month) {
    if (alice.receives_second_salary && month == 12) {
        const RUB bonus = alice.salary2 / 4;
        alice.cash += bonus;
        alice.monthly_income += bonus;
        alice.total_income += bonus;
    }
}

void alice_taxes2(const int year, const int month) {
    if (!alice.receives_second_salary) {
        return;
    }
    const RUB salary_tax = static_cast<RUB>(alice.salary2 * tax::INCOME_RATE);
    alice.cash -= salary_tax;
    alice.monthly_expense += salary_tax;
    alice.total_expense += salary_tax;
    alice.total_tax_paid += salary_tax;
}

void alice_promotion2(const int year, const int month) {
    if (alice.receives_second_salary && month == 3 && year >= 2028) {
        alice.experience_years2 += 1;
    }
}

//  НАЛОГОВЫЙ ВЫЧЕТ

void alice_tax_refund(const int year, const int month) {
    if (month != 4) {
        return;
    }

    RUB refund = 0;
    if (alice.pays_health_insurance) { refund += 12'000; }
    if (alice.experience_years >= 2) { refund += 8'000; }
    if (alice.pays_mortgage) { refund += 15'000; }
    if (alice.owns_deposit_account) { refund += 5'000; }

    alice.cash += refund;
    alice.monthly_income += refund;
    alice.total_income += refund;
    alice.total_tax_refunded += refund;
}

//  ЖИЛЬЁ: АРЕНДА

void alice_rent(const int year, const int month) {
    if (!alice.pays_rent) {
        return;
    }
    const RUB rent = inflated(30'000, rate::RENT, year, month);
    alice.cash -= rent;
    alice.monthly_expense += rent;
    alice.total_expense += rent;
}

//  ЖИЛЬЁ: КОММУНАЛКА

void alice_home_bills_electricity(const int year, const int month) {
    RUB base = 1'500;
    if (month == 1 || month == 2 || month == 12) { base = 2'200; }
    if (month == 7 || month == 8) { base = 1'800; }

    const RUB electricity = inflated(base, rate::UTILITIES, year, month);
    alice.cash -= electricity;
    alice.monthly_expense += electricity;
    alice.total_expense += electricity;
}

void alice_home_bills_water(const int year, const int month) {
    const RUB water = inflated(800, rate::UTILITIES, year, month);
    alice.cash -= water;
    alice.monthly_expense += water;
    alice.total_expense += water;
}

void alice_home_bills_gas(const int year, const int month) {
    const RUB gas = inflated(600, rate::UTILITIES, year, month);
    alice.cash -= gas;
    alice.monthly_expense += gas;
    alice.total_expense += gas;
}

void alice_home_bills_internet(const int year, const int month) {
    const RUB internet = inflated(700, rate::UTILITIES, year, month);
    alice.cash -= internet;
    alice.monthly_expense += internet;
    alice.total_expense += internet;
}

void alice_home_bills_heating(const int year, const int month) {
    RUB base = 1'200;
    if (month == 1 || month == 2 || month == 12) { base = 2'500; }

    const RUB heating = inflated(base, rate::UTILITIES, year, month);
    alice.cash -= heating;
    alice.monthly_expense += heating;
    alice.total_expense += heating;
}

void alice_home_bills_garbage(const int year, const int month) {
    const RUB garbage = inflated(300, rate::UTILITIES, year, month);
    alice.cash -= garbage;
    alice.monthly_expense += garbage;
    alice.total_expense += garbage;
}

void alice_home_bills_landline(const int year, const int month) {
    const RUB landline = inflated(400, rate::UTILITIES, year, month);
    alice.cash -= landline;
    alice.monthly_expense += landline;
    alice.total_expense += landline;
}

void alice_home_bills_total(const int year, const int month) {
    alice_home_bills_electricity(year, month);
    alice_home_bills_water(year, month);
    alice_home_bills_gas(year, month);
    alice_home_bills_internet(year, month);
    alice_home_bills_heating(year, month);
    alice_home_bills_garbage(year, month);
    alice_home_bills_landline(year, month);
}

//  ЕДА

void alice_food_groceries(const int year, const int month) {
    const RUB groceries = inflated(10'000, rate::FOOD, year, month);
    alice.cash -= groceries;
    alice.monthly_expense += groceries;
    alice.total_expense += groceries;
}

void alice_food_restaurants(const int year, const int month) {
    RUB base = 5'000;
    if (month == 12) { base = 10'000; }
    if (month == 1) { base = 3'000; }

    const RUB restaurants = inflated(base, rate::FOOD, year, month);
    alice.cash -= restaurants;
    alice.monthly_expense += restaurants;
    alice.total_expense += restaurants;
}

void alice_food_coffee(const int year, const int month) {
    const RUB coffee = inflated(2'000, rate::FOOD, year, month);
    alice.cash -= coffee;
    alice.monthly_expense += coffee;
    alice.total_expense += coffee;
}

void alice_food_delivery(const int year, const int month) {
    const RUB delivery = inflated(1'500, rate::FOOD, year, month);
    alice.cash -= delivery;
    alice.monthly_expense += delivery;
    alice.total_expense += delivery;
}

void alice_food_lunch_work(const int year, const int month) {
    const RUB lunch = inflated(4'000, rate::FOOD, year, month);
    alice.cash -= lunch;
    alice.monthly_expense += lunch;
    alice.total_expense += lunch;
}

void alice_food_snacks(const int year, const int month) {
    const RUB snacks = inflated(800, rate::FOOD, year, month);
    alice.cash -= snacks;
    alice.monthly_expense += snacks;
    alice.total_expense += snacks;
}

void alice_food_total(const int year, const int month) {
    alice_food_groceries(year, month);
    alice_food_restaurants(year, month);
    alice_food_coffee(year, month);
    alice_food_delivery(year, month);
    alice_food_lunch_work(year, month);
    alice_food_snacks(year, month);
}

//  ТРАНСПОРТ (ОБЩЕСТВЕННЫЙ)

void alice_transport_metro(const int year, const int month) {
    const RUB metro = inflated(1'500, rate::TRANSPORT, year, month);
    alice.cash -= metro;
    alice.monthly_expense += metro;
    alice.total_expense += metro;
}

void alice_transport_taxi(const int year, const int month) {
    RUB base = 3'000;
    if (month == 12) { base = 5'000; }

    const RUB taxi = inflated(base, rate::TRANSPORT, year, month);
    alice.cash -= taxi;
    alice.monthly_expense += taxi;
    alice.total_expense += taxi;
}

void alice_transport_bus(const int year, const int month) {
    const RUB bus = inflated(600, rate::TRANSPORT, year, month);
    alice.cash -= bus;
    alice.monthly_expense += bus;
    alice.total_expense += bus;
}

void alice_transport_scooter(const int year, const int month) {
    const RUB scooter = inflated(1'200, rate::TRANSPORT, year, month);
    alice.cash -= scooter;
    alice.monthly_expense += scooter;
    alice.total_expense += scooter;
}

void alice_transport_public(const int year, const int month) {
    if (!alice.uses_public_transport) {
        return;
    }
    alice_transport_metro(year, month);
    alice_transport_taxi(year, month);
    alice_transport_bus(year, month);
    alice_transport_scooter(year, month);
}

void alice_transport_total(const int year, const int month) {
    alice_transport_public(year, month);
}

//  АВТОМОБИЛЬ

void alice_car_buy(const int year, const int month) {
    if (!(year == 2028 && month == 5)) {
        return;
    }

    const RUB car_price = inflated(800'000, rate::CAR, year, month);
    const RUB down_payment = 300'000;

    if (alice.cash < down_payment) {
        return;
    }

    alice.cash -= down_payment;
    alice.car_value = car_price;
    alice.credit_debt += car_price - down_payment;
    alice.owns_car = true;
    alice.uses_public_transport = false;
    alice.owes_car_credit = true;
    alice.total_expense += down_payment;
}

void alice_car_fuel(const int year, const int month) {
    if (!alice.owns_car) {
        return;
    }
    RUB base = 6'000;
    if (month == 7 || month == 8) { base = 8'000; }

    const RUB fuel = inflated(base, rate::CAR, year, month);
    alice.cash -= fuel;
    alice.monthly_expense += fuel;
    alice.total_expense += fuel;
}

void alice_car_insurance(const int year, const int month) {
    if (!(alice.owns_car && month == 1)) {
        return;
    }
    const RUB insurance = inflated(12'000, rate::CAR, year, month);
    alice.cash -= insurance;
    alice.monthly_expense += insurance;
    alice.total_expense += insurance;
}

void alice_car_repair(const int year, const int month) {
    if (!alice.owns_car) {
        return;
    }
    RUB repair = 0;
    if (year == 2029 && month == 4) { repair = 25'000; }
    if (year == 2030 && month == 8) { repair = 40'000; }

    if (repair > 0) {
        alice.cash -= repair;
        alice.monthly_expense += repair;
        alice.total_expense += repair;
    }
}

void alice_car_tax(const int year, const int month) {
    if (!(alice.owns_car && month == 11)) {
        return;
    }
    const RUB car_tax = inflated(9'000, rate::CAR, year, month);
    alice.cash -= car_tax;
    alice.monthly_expense += car_tax;
    alice.total_expense += car_tax;
}

void alice_car_parking(const int year, const int month) {
    if (!alice.owns_car) {
        return;
    }
    const RUB parking = inflated(3'000, rate::CAR, year, month);
    alice.cash -= parking;
    alice.monthly_expense += parking;
    alice.total_expense += parking;
}

void alice_car_wash(const int year, const int month) {
    if (!alice.owns_car) {
        return;
    }
    const RUB wash = inflated(1'500, rate::CAR, year, month);
    alice.cash -= wash;
    alice.monthly_expense += wash;
    alice.total_expense += wash;
}

void alice_car_tires(const int year, const int month) {
    if (!(alice.owns_car && (month == 4 || month == 10))) {
        return;
    }
    const RUB tires = inflated(15'000, rate::CAR, year, month);
    alice.cash -= tires;
    alice.monthly_expense += tires;
    alice.total_expense += tires;
}

void alice_car_oil(const int year, const int month) {
    if (!(alice.owns_car && (month == 3 || month == 9))) {
        return;
    }
    const RUB oil = inflated(6'000, rate::CAR, year, month);
    alice.cash -= oil;
    alice.monthly_expense += oil;
    alice.total_expense += oil;
}

void alice_car_inspection(const int year, const int month) {
    if (!(alice.owns_car && month == 6)) {
        return;
    }
    const RUB inspection = inflated(4'000, rate::CAR, year, month);
    alice.cash -= inspection;
    alice.monthly_expense += inspection;
    alice.total_expense += inspection;
}

void alice_car_total(const int year, const int month) {
    alice_car_buy(year, month);
    alice_car_fuel(year, month);
    alice_car_insurance(year, month);
    alice_car_repair(year, month);
    alice_car_tax(year, month);
    alice_car_parking(year, month);
    alice_car_wash(year, month);
    alice_car_tires(year, month);
    alice_car_oil(year, month);
    alice_car_inspection(year, month);
}

//  АВТОКРЕДИТ

void alice_credit_payment(const int year, const int month) {
    if (!(alice.owes_car_credit && alice.credit_debt > 0)) {
        return;
    }

    RUB payment = 25'000;
    if (alice.credit_debt < payment) {
        payment = alice.credit_debt;
    }

    alice.cash -= payment;
    alice.credit_debt -= payment;
    alice.monthly_expense += payment;
    alice.total_expense += payment;

    if (alice.credit_debt == 0) {
        alice.owes_car_credit = false;
    }
}

//  ИПОТЕКА

void alice_mortgage_buy(const int year, const int month) {
    if (!(year == 2029 && month == 5)) {
        return;
    }

    const RUB property_price = inflated(5'000'000, rate::MORTGAGE, year, month);
    const RUB down_payment = 1'000'000;

    if (alice.cash < down_payment) {
        return;
    }

    alice.cash -= down_payment;
    alice.property_value = property_price;
    alice.mortgage_debt = property_price - down_payment;
    alice.pays_rent = false;
    alice.pays_mortgage = true;
    alice.owns_property = true;
    alice.total_expense += down_payment;
}

void alice_mortgage_payment(const int year, const int month) {
    if (!(alice.pays_mortgage && alice.mortgage_debt > 0)) {
        return;
    }

    RUB payment = 45'000;
    if (alice.mortgage_debt < payment) {
        payment = alice.mortgage_debt;
    }

    alice.cash -= payment;
    alice.mortgage_debt -= payment;
    alice.monthly_expense += payment;
    alice.total_expense += payment;

    if (alice.mortgage_debt == 0) {
        alice.pays_mortgage = false;
    }
}

void alice_mortgage_interest(const int year, const int month) {
    if (!(alice.pays_mortgage && alice.mortgage_debt > 0)) {
        return;
    }
    const RUB interest =
        static_cast<RUB>(alice.mortgage_debt * rate::MORTGAGE / 12.0);
    alice.mortgage_debt += interest;
    alice.monthly_expense += interest;
    alice.total_expense += interest;
}

void alice_mortgage_insurance(const int year, const int month) {
    if (!(alice.pays_mortgage && month == 1)) {
        return;
    }
    const RUB insurance = inflated(20'000, rate::MORTGAGE, year, month);
    alice.cash -= insurance;
    alice.monthly_expense += insurance;
    alice.total_expense += insurance;
}

void alice_mortgage_total(const int year, const int month) {
    alice_mortgage_buy(year, month);
    alice_mortgage_payment(year, month);
    alice_mortgage_interest(year, month);
    alice_mortgage_insurance(year, month);
}

//  СОБАКА

void alice_dog_buy(const int year, const int month) {
    if (!(year == 2027 && month == 3)) {
        return;
    }
    const RUB dog_price = 30'000;
    if (alice.cash < dog_price) {
        return;
    }
    alice.cash -= dog_price;
    alice.owns_dog = true;
    alice.monthly_expense += dog_price;
    alice.total_expense += dog_price;
}

void alice_dog_food(const int year, const int month) {
    if (!alice.owns_dog) {
        return;
    }
    const RUB dog_food = inflated(4'000, rate::PETS, year, month);
    alice.cash -= dog_food;
    alice.monthly_expense += dog_food;
    alice.total_expense += dog_food;
}

void alice_dog_vet(const int year, const int month) {
    if (!alice.owns_dog) {
        return;
    }
    RUB vet = 0;
    if (month == 4 || month == 10) { vet = 5'000; }
    if (year == 2029 && month == 6) { vet = 15'000; }

    if (vet > 0) {
        alice.cash -= vet;
        alice.monthly_expense += vet;
        alice.total_expense += vet;
    }
}

void alice_dog_toys(const int year, const int month) {
    if (!alice.owns_dog) {
        return;
    }
    const RUB toys = inflated(1'000, rate::PETS, year, month);
    alice.cash -= toys;
    alice.monthly_expense += toys;
    alice.total_expense += toys;
}

void alice_dog_grooming(const int year, const int month) {
    if (!alice.owns_dog) {
        return;
    }
    const RUB grooming = inflated(2'500, rate::PETS, year, month);
    alice.cash -= grooming;
    alice.monthly_expense += grooming;
    alice.total_expense += grooming;
}

void alice_dog_walking(const int year, const int month) {
    if (!alice.owns_dog) {
        return;
    }
    const RUB walking = inflated(3'000, rate::PETS, year, month);
    alice.cash -= walking;
    alice.monthly_expense += walking;
    alice.total_expense += walking;
}

void alice_dog_total(const int year, const int month) {
    alice_dog_buy(year, month);
    alice_dog_food(year, month);
    alice_dog_vet(year, month);
    alice_dog_toys(year, month);
    alice_dog_grooming(year, month);
    alice_dog_walking(year, month);
}

//  КОШКА

void alice_cat_buy(const int year, const int month) {
    if (!(year == 2030 && month == 2)) {
        return;
    }
    const RUB cat_price = 20'000;
    if (alice.cash < cat_price) {
        return;
    }
    alice.cash -= cat_price;
    alice.owns_cat = true;
    alice.monthly_expense += cat_price;
    alice.total_expense += cat_price;
}

void alice_cat_food(const int year, const int month) {
    if (!alice.owns_cat) {
        return;
    }
    const RUB cat_food = inflated(3'000, rate::PETS, year, month);
    alice.cash -= cat_food;
    alice.monthly_expense += cat_food;
    alice.total_expense += cat_food;
}

void alice_cat_litter(const int year, const int month) {
    if (!alice.owns_cat) {
        return;
    }
    const RUB litter = inflated(1'000, rate::PETS, year, month);
    alice.cash -= litter;
    alice.monthly_expense += litter;
    alice.total_expense += litter;
}

void alice_cat_vet(const int year, const int month) {
    if (!alice.owns_cat) {
        return;
    }
    RUB vet = 0;
    if (month == 5 || month == 11) { vet = 4'000; }
    if (vet > 0) {
        alice.cash -= vet;
        alice.monthly_expense += vet;
        alice.total_expense += vet;
    }
}

void alice_cat_toys(const int year, const int month) {
    if (!alice.owns_cat) {
        return;
    }
    const RUB toys = inflated(500, rate::PETS, year, month);
    alice.cash -= toys;
    alice.monthly_expense += toys;
    alice.total_expense += toys;
}

void alice_cat_total(const int year, const int month) {
    alice_cat_buy(year, month);
    alice_cat_food(year, month);
    alice_cat_litter(year, month);
    alice_cat_vet(year, month);
    alice_cat_toys(year, month);
}

//  ЗДОРОВЬЕ

void alice_health_insurance(const int year, const int month) {
    if (year == 2027 && month == 1) {
        alice.pays_health_insurance = true;
    }
    if (!alice.pays_health_insurance) {
        return;
    }
    const RUB insurance = inflated(5'000, rate::HEALTHCARE, year, month);
    alice.cash -= insurance;
    alice.monthly_expense += insurance;
    alice.total_expense += insurance;
}

void alice_health_medicine(const int year, const int month) {
    RUB base = 1'500;
    if (month == 1 || month == 2) { base = 3'000; }
    if (month == 10 || month == 11) { base = 2'500; }

    const RUB medicine = inflated(base, rate::HEALTHCARE, year, month);
    alice.cash -= medicine;
    alice.monthly_expense += medicine;
    alice.total_expense += medicine;
}

void alice_health_gym(const int year, const int month) {
    if (year == 2028 && month == 1) {
        alice.pays_gym = true;
    }
    if (!alice.pays_gym) {
        return;
    }
    const RUB gym = inflated(3'000, rate::HEALTHCARE, year, month);
    alice.cash -= gym;
    alice.monthly_expense += gym;
    alice.total_expense += gym;
}

void alice_health_dentist(const int year, const int month) {
    RUB dentist = 0;
    if (month == 5) { dentist = 8'000; }
    if (year == 2029 && month == 9) { dentist = 20'000; }

    if (dentist > 0) {
        alice.cash -= dentist;
        alice.monthly_expense += dentist;
        alice.total_expense += dentist;
    }
}

void alice_health_doctor(const int year, const int month) {
    RUB doctor = 0;
    if (month == 3 || month == 9) { doctor = 4'000; }
    if (doctor > 0) {
        alice.cash -= doctor;
        alice.monthly_expense += doctor;
        alice.total_expense += doctor;
    }
}

void alice_health_vitamins(const int year, const int month) {
    const RUB vitamins = inflated(1'500, rate::HEALTHCARE, year, month);
    alice.cash -= vitamins;
    alice.monthly_expense += vitamins;
    alice.total_expense += vitamins;
}

void alice_health_therapy(const int year, const int month) {
    const RUB therapy = inflated(6'000, rate::HEALTHCARE, year, month);
    alice.cash -= therapy;
    alice.monthly_expense += therapy;
    alice.total_expense += therapy;
}

void alice_health_total(const int year, const int month) {
    alice_health_insurance(year, month);
    alice_health_medicine(year, month);
    alice_health_gym(year, month);
    alice_health_dentist(year, month);
    alice_health_doctor(year, month);
    alice_health_vitamins(year, month);
    alice_health_therapy(year, month);
}

//  ОБРАЗОВАНИЕ

void alice_education_courses(const int year, const int month) {
    RUB courses = 0;
    if (year == 2027 && month == 2) { courses = 15'000; }
    if (year == 2028 && month == 9) { courses = 25'000; }
    if (year == 2030 && month == 3) { courses = 35'000; }

    if (courses > 0) {
        alice.cash -= courses;
        alice.monthly_expense += courses;
        alice.total_expense += courses;
    }
}

void alice_education_books(const int year, const int month) {
    RUB base = 1'000;
    if (month == 9) { base = 3'000; }

    const RUB books = inflated(base, rate::EDUCATION, year, month);
    alice.cash -= books;
    alice.monthly_expense += books;
    alice.total_expense += books;
}

void alice_education_conference(const int year, const int month) {
    if (!(year == 2029 && month == 11)) {
        return;
    }
    const RUB conference = 25'000;
    alice.cash -= conference;
    alice.monthly_expense += conference;
    alice.total_expense += conference;
}

void alice_education_total(const int year, const int month) {
    alice_education_courses(year, month);
    alice_education_books(year, month);
    alice_education_conference(year, month);
}

//  РАЗВЛЕЧЕНИЯ

void alice_entertainment_movies(const int year, const int month) {
    RUB base = 1'500;
    if (month == 12) { base = 3'000; }

    const RUB movies = inflated(base, rate::ENTERTAINMENT, year, month);
    alice.cash -= movies;
    alice.monthly_expense += movies;
    alice.total_expense += movies;
}

void alice_entertainment_netflix(const int year, const int month) {
    if (year == 2026 && month == 10) {
        alice.pays_subscriptions = true;
    }
    if (!alice.pays_subscriptions) {
        return;
    }
    const RUB netflix = inflated(1'200, rate::ENTERTAINMENT, year, month);
    alice.cash -= netflix;
    alice.monthly_expense += netflix;
    alice.total_expense += netflix;
}

void alice_entertainment_spotify(const int year, const int month) {
    if (!alice.pays_subscriptions) {
        return;
    }
    const RUB spotify = inflated(400, rate::ENTERTAINMENT, year, month);
    alice.cash -= spotify;
    alice.monthly_expense += spotify;
    alice.total_expense += spotify;
}

void alice_entertainment_cloud(const int year, const int month) {
    if (!alice.pays_subscriptions) {
        return;
    }
    const RUB cloud = inflated(300, rate::ENTERTAINMENT, year, month);
    alice.cash -= cloud;
    alice.monthly_expense += cloud;
    alice.total_expense += cloud;
}

void alice_entertainment_concerts(const int year, const int month) {
    RUB concert = 0;
    if (year == 2027 && month == 6) { concert = 8'000; }
    if (year == 2029 && month == 8) { concert = 12'000; }
    if (concert > 0) {
        alice.cash -= concert;
        alice.monthly_expense += concert;
        alice.total_expense += concert;
    }
}

void alice_entertainment_games(const int year, const int month) {
    const RUB games = inflated(2'000, rate::ENTERTAINMENT, year, month);
    alice.cash -= games;
    alice.monthly_expense += games;
    alice.total_expense += games;
}

void alice_entertainment_theater(const int year, const int month) {
    const RUB theater = inflated(3'000, rate::ENTERTAINMENT, year, month);
    alice.cash -= theater;
    alice.monthly_expense += theater;
    alice.total_expense += theater;
}

void alice_entertainment_total(const int year, const int month) {
    alice_entertainment_movies(year, month);
    alice_entertainment_netflix(year, month);
    alice_entertainment_spotify(year, month);
    alice_entertainment_cloud(year, month);
    alice_entertainment_concerts(year, month);
    alice_entertainment_games(year, month);
    alice_entertainment_theater(year, month);
}

//  ОДЕЖДА И ЛИЧНЫЕ РАСХОДЫ

void alice_clothes(const int year, const int month) {
    RUB base = 5'000;
    if (month == 3 || month == 9) { base = 12'000; }
    if (month == 12) { base = 15'000; }

    const RUB clothes = inflated(base, rate::CLOTHING, year, month);
    alice.cash -= clothes;
    alice.monthly_expense += clothes;
    alice.total_expense += clothes;
}

void alice_shoes(const int year, const int month) {
    const RUB shoes = inflated(3'000, rate::CLOTHING, year, month);
    alice.cash -= shoes;
    alice.monthly_expense += shoes;
    alice.total_expense += shoes;
}

void alice_cosmetics(const int year, const int month) {
    const RUB cosmetics = inflated(3'000, rate::CLOTHING, year, month);
    alice.cash -= cosmetics;
    alice.monthly_expense += cosmetics;
    alice.total_expense += cosmetics;
}

void alice_accessories(const int year, const int month) {
    const RUB accessories = inflated(1'000, rate::CLOTHING, year, month);
    alice.cash -= accessories;
    alice.monthly_expense += accessories;
    alice.total_expense += accessories;
}

void alice_laundry(const int year, const int month) {
    const RUB laundry = inflated(800, rate::CLOTHING, year, month);
    alice.cash -= laundry;
    alice.monthly_expense += laundry;
    alice.total_expense += laundry;
}

void alice_clothes_total(const int year, const int month) {
    alice_clothes(year, month);
    alice_shoes(year, month);
    alice_cosmetics(year, month);
    alice_accessories(year, month);
    alice_laundry(year, month);
}

//  УХОД ЗА СОБОЙ

void alice_haircut(const int year, const int month) {
    const RUB haircut = inflated(2'500, rate::OTHER, year, month);
    alice.cash -= haircut;
    alice.monthly_expense += haircut;
    alice.total_expense += haircut;
}

void alice_manicure(const int year, const int month) {
    const RUB manicure = inflated(2'000, rate::OTHER, year, month);
    alice.cash -= manicure;
    alice.monthly_expense += manicure;
    alice.total_expense += manicure;
}

void alice_spa(const int year, const int month) {
    const RUB spa = inflated(4'000, rate::OTHER, year, month);
    alice.cash -= spa;
    alice.monthly_expense += spa;
    alice.total_expense += spa;
}

void alice_personal_care_total(const int year, const int month) {
    alice_haircut(year, month);
    alice_manicure(year, month);
    alice_spa(year, month);
}

//  СВЯЗЬ

void alice_phone(const int year, const int month) {
    const RUB phone = inflated(800, rate::OTHER, year, month);
    alice.cash -= phone;
    alice.monthly_expense += phone;
    alice.total_expense += phone;

    if (year == 2028 && month == 11) {
        const RUB new_phone = 50'000;
        alice.cash -= new_phone;
        alice.monthly_expense += new_phone;
        alice.total_expense += new_phone;
    }

    if (year == 2030 && month == 11) {
        const RUB new_phone = 70'000;
        alice.cash -= new_phone;
        alice.monthly_expense += new_phone;
        alice.total_expense += new_phone;
    }
}

//  ПОДАРКИ И БЛАГОТВОРИТЕЛЬНОСТЬ

void alice_gifts(const int year, const int month) {
    RUB gifts = 0;
    if (month == 12) { gifts = 10'000; }
    if (month == 3) { gifts = 5'000; }
    if (month == 2) { gifts = 3'000; }

    if (gifts > 0) {
        alice.cash -= gifts;
        alice.monthly_expense += gifts;
        alice.total_expense += gifts;
    }
}

void alice_charity(const int year, const int month) {
    const RUB charity = inflated(500, rate::OTHER, year, month);
    alice.cash -= charity;
    alice.monthly_expense += charity;
    alice.total_expense += charity;
}

//  ПУТЕШЕСТВИЯ И ОТПУСК

void alice_travel(const int year, const int month) {
    RUB travel = 0;
    if (year == 2027 && month == 7) { travel = 40'000; }
    if (year == 2028 && month == 7) { travel = 60'000; }
    if (year == 2029 && month == 7) { travel = 80'000; }
    if (year == 2030 && month == 7) { travel = 100'000; }

    if (year == 2031 && month == 7) { travel = 120'000; }
    if (year == 2032 && month == 7) { travel = 140'000; }
    if (year == 2033 && month == 7) { travel = 160'000; }
    if (year == 2034 && month == 7) { travel = 180'000; }
    if (year == 2035 && month == 7) { travel = 200'000; }

    if (travel > 0) {
        alice.cash -= travel;
        alice.monthly_expense += travel;
        alice.total_expense += travel;
    }
}

void alice_vacation(const int year, const int month) {
    RUB vacation = 0;
    if (month == 1) { vacation = 15'000; }
    if (month == 5) { vacation = 10'000; }

    if (vacation > 0) {
        alice.cash -= vacation;
        alice.monthly_expense += vacation;
        alice.total_expense += vacation;
    }
}

//  ВКЛАД И ИНВЕСТИЦИИ

void alice_deposit_open(const int year, const int month) {
    if (year == 2027 && month == 6 && alice.cash >= 100'000) {
        alice.cash -= 100'000;
        alice.deposit += 100'000;
        alice.owns_deposit_account = true;
    }
    if (year == 2029 && month == 6 && alice.cash >= 150'000) {
        alice.cash -= 150'000;
        alice.deposit += 150'000;
    }
    if (year == 2030 && month == 6 && alice.cash >= 200'000) {
        alice.cash -= 200'000;
        alice.deposit += 200'000;
    }
}

void alice_bank_income(const int year, const int month) {
    if (!(alice.owns_deposit_account && alice.deposit > 0)) {
        return;
    }
    const RUB interest =
        static_cast<RUB>(alice.deposit * rate::DEPOSIT / 12.0);
    alice.deposit += interest;
    alice.monthly_income += interest;
    alice.total_income += interest;
    alice.total_interest += interest;
}

void alice_emergency_fund(const int year, const int month) {
    if (alice.cash < 50'000) { return; }
    alice.cash -= 5'000;
    alice.deposit += 5'000;
    alice.owns_deposit_account = true;
}

void alice_investment(const int year, const int month) {
    if (year >= 2029 && month == 3 && alice.cash >= 50'000) {
        alice.cash -= 50'000;
        alice.deposit += 50'000;
        alice.owns_deposit_account = true;
    }
}

//  ВОЗРАСТ

void alice_age_update(const int year, const int month) {
    if (month == 9) {
        alice.age += 1;
    }
}

//  ВЫВОД

void alice_print_monthly(const int year, const int month) {
    std::cout << "Year " << year
              << " Month " << month
              << ": income = " << alice.monthly_income
              << ", expense = " << alice.monthly_expense
              << ", cash = " << alice.cash
              << ", deposit = " << alice.deposit
              << '\n';
}

void alice_print_final() {
    std::cout << "Alice age = " << alice.age << '\n';
    std::cout << "Alice experience (job 1) = " << alice.experience_years << " years\n";
    std::cout << "Alice experience (job 2) = " << alice.experience_years2 << " years\n";
    std::cout << "Alice cash = " << alice.cash << '\n';
    std::cout << "Alice deposit = " << alice.deposit << '\n';
    std::cout << "Alice credit debt = " << alice.credit_debt << '\n';
    std::cout << "Alice mortgage debt = " << alice.mortgage_debt << '\n';
    std::cout << "Alice car value = " << alice.car_value << '\n';
    std::cout << "Alice property value = " << alice.property_value << '\n';
    std::cout << "Alice total income = " << alice.total_income << '\n';
    std::cout << "Alice total expense = " << alice.total_expense << '\n';
    std::cout << "Alice total tax paid = " << alice.total_tax_paid << '\n';
    std::cout << "Alice total tax refunded = " << alice.total_tax_refunded << '\n';
    std::cout << "Alice total interest = " << alice.total_interest << '\n';

    std::cout << std::boolalpha;
    std::cout << "Alice owns car = " << alice.owns_car << '\n';
    std::cout << "Alice uses public transport = " << alice.uses_public_transport << '\n';
    std::cout << "Alice owns dog = " << alice.owns_dog << '\n';
    std::cout << "Alice owns cat = " << alice.owns_cat << '\n';
    std::cout << "Alice owns deposit account = " << alice.owns_deposit_account << '\n';
    std::cout << "Alice owes car credit = " << alice.owes_car_credit << '\n';
    std::cout << "Alice pays mortgage = " << alice.pays_mortgage << '\n';
    std::cout << "Alice owns property = " << alice.owns_property << '\n';
    std::cout << "Alice pays rent = " << alice.pays_rent << '\n';
    std::cout << "Alice pays health insurance = " << alice.pays_health_insurance << '\n';
    std::cout << "Alice pays gym = " << alice.pays_gym << '\n';
    std::cout << "Alice pays subscriptions = " << alice.pays_subscriptions << '\n';
    std::cout << "Alice receives second salary = " << alice.receives_second_salary << '\n';
}

void alice_print_verdict() {
    const RUB net_worth = alice.cash
                        + alice.deposit
                        + alice.property_value
                        + alice.car_value
                        - alice.mortgage_debt
                        - alice.credit_debt;

    std::cout << "\nVERDICT\n";
    std::cout << "Net worth = " << net_worth << '\n';

    if (net_worth < 0) {
        std::cout << "Life model: BAD\n";
    } else if (net_worth < 1'000'000) {
        std::cout << "Life model: NOT BAD\n";
    } else if (net_worth < 5'000'000) {
        std::cout << "Life model: GOOD\n";
    } else {
        std::cout << "Life model: PERFECT\n";
    }
}

//  СИМУЛЯЦИЯ

void simulation() {
    int year = period::START_YEAR;
    int month = period::START_MONTH;

    while (!(year == period::END_YEAR && month == period::END_MONTH)) {

        alice_monthly_reset();

        // Доходы
        alice_salary(year, month);
        alice_bonus(year, month);
        alice_taxes(year, month);
        alice_promotion(year, month);

        alice_salary2(year, month);
        alice_bonus2(year, month);
        alice_taxes2(year, month);
        alice_promotion2(year, month);

        alice_tax_refund(year, month);

        // Обязательные расходы
        alice_rent(year, month);
        alice_home_bills_total(year, month);

        // Еда
        alice_food_total(year, month);

        // Транспорт и машина
        alice_transport_total(year, month);
        alice_car_total(year, month);
        alice_credit_payment(year, month);

        // Ипотека
        alice_mortgage_total(year, month);

        // Питомцы
        alice_dog_total(year, month);
        alice_cat_total(year, month);

        // Здоровье и образование
        alice_health_total(year, month);
        alice_education_total(year, month);

        // Развлечения
        alice_entertainment_total(year, month);

        // Одежда, уход, связь, подарки
        alice_clothes_total(year, month);
        alice_personal_care_total(year, month);
        alice_phone(year, month);
        alice_gifts(year, month);
        alice_charity(year, month);

        // Путешествия
        alice_travel(year, month);
        alice_vacation(year, month);

        // Вклады и инвестиции
        alice_deposit_open(year, month);
        alice_bank_income(year, month);
        alice_emergency_fund(year, month);
        alice_investment(year, month);

        // Возраст
        alice_age_update(year, month);

        // Отчёт за месяц
        alice_print_monthly(year, month);

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}

//  ТОЧКА ВХОДА

int main() {
    alice_init();
    simulation();
    alice_print_final();
    alice_print_verdict();
    return 0;
}