#include <stdio.h>

//typedef int RUB;
using RUB = int;

struct Car {
    bool bought;        // куплена ли машина
    RUB price;          // цена машины
    RUB down_payment;   // первоначальный взнос
    RUB fuel;           // бензин в месяц
    RUB maintenance;    // ТО раз в год
    RUB repair;         // ремонт раз в 3 года
    int horse_power;    // мощность нужна для налога
};

struct Home {
    RUB rent;           // аренда, пока нет своей квартиры
    RUB bills;          // ЖКХ в месяц
    RUB goods;          // товары для дома в месяц
    RUB repair;         // косметический ремонт раз в год в своей квартире
};

struct Flat {
    bool bought;        // куплена ли квартира
    RUB price;          // цена (кадастровая)
    RUB down_payment;   // первоначальный взнос
    RUB rent_income;    // сколько платят жильцы в месяц (0, если живет ден)
};

struct Loan {
    RUB debt;           // остаток долга
    RUB payment;        // ежемесячный платеж
    int rate;           // % годовых
};

struct Deposit {
    int amount;          // сумма в валюте вклада (рубли, евро или юани)
    int rate;            // % годовых
    int currency_rate;   // курс валюты в копейках (1 евро = 9500 коп.)
    int currency_growth; // на сколько % валюта дорожает за год
};

struct Bank {
    Loan car_loan; // на все кредиты одна и та же математика, поэтому объеденю (нейронка)
    Loan mortgage;
    Loan second_mortgage;
    Deposit deposit_rub; // то же с депозитами
    Deposit deposit_eur;
    Deposit deposit_cny;
};

struct Health {
    RUB checkup;        // чек ап раз в год
    RUB dentist;        // стоматолог раз в год
    RUB illness;        // лекарства при болезни, раз в полгода
};

struct Tax {
    RUB car_per_hp;         // транспортный налог за 1 лс
    int property_permille;  // налог на квартиру в промилле (0.1% = 1)
    int rent_percent;       // налог с аренды для самозанятого (6%)
};

struct Person {
    RUB cash;
    RUB salary;
    RUB food;
    Car car;
    Home home;
    Flat flat;
    Flat second_flat;
    Bank bank;
    Health health;
    Tax tax;
};

struct Person den;

void den_salary(const int year, const int month) {
    if (month == 1) // индексация зарплаты 7% в год
    {
        den.salary = den.salary * 107 / 100;
    }

    if (year == 2030 and month == 3) // Повышение
    {
        den.salary += 30'000;
    }

    den.cash += den.salary;
}

void den_food(const int year, const int month) {
    if (month == 1) // инфляция на еду 9% в год
    {
        den.food = den.food * 109 / 100;
    }

    den.cash -= den.food;
}

void den_rent(const int year, const int month) {
    if (month == 9) // хозяин поднимает аренду 5% в год
    {
        den.home.rent = den.home.rent * 105 / 100;
    }

    if (not den.flat.bought) // съем, пока нет своего жилья
    {
        den.cash -= den.home.rent;
    }
}

void den_home(const int year, const int month) {
    if (month == 7) // индексация тарифов ЖКХ 10% в год
    {
        den.home.bills = den.home.bills * 110 / 100;
    }

    if (month == 6 and den.flat.bought) // косметический ремонт в своей квартире
    {
        den.cash -= den.home.repair;
    }

    den.cash -= den.home.bills;
    den.cash -= den.home.goods;
}

void den_car(const int year, const int month) {
    if (month == 1) // бензин дорожает 8% в год
    {
        den.car.fuel = den.car.fuel * 108 / 100;
    }

    if (not den.car.bought) //  если машины нет
    {
        return;
    }

    if (month == 4) // ТО
    {
        den.cash -= den.car.maintenance;
    }

    if (month == 10 and year % 3 == 0) // ремонт
    {
        den.cash -= den.car.repair;
    }

    den.cash -= den.car.fuel;
}

void den_health(const int year, const int month) {
    if (month == 1) // медицина дорожает 10% в год
    {
        den.health.checkup = den.health.checkup * 110 / 100;
        den.health.dentist = den.health.dentist * 110 / 100;
        den.health.illness = den.health.illness * 110 / 100;
    }

    if (month == 2) // ежегодное обследование
    {
        den.cash -= den.health.checkup;
    }

    if (month == 8) // стоматолог
    {
        den.cash -= den.health.dentist;
    }

    if (month == 3 or month == 11) // простуда весной и осенью
    {
        den.cash -= den.health.illness;
    }
}

void den_rent_income(const int year, const int month) {
    if (not den.second_flat.bought) // вторую квартиру еще не купил
    {   
        return;
    }

    RUB tax = den.second_flat.rent_income * den.tax.rent_percent / 100; // считаем налог и затем вычитаем его
    den.cash += den.second_flat.rent_income - tax;
}

void den_loan(Loan& loan) {
    if (loan.debt <= 0) // кредита нет или он выплачен
    {
        return;
    }

    RUB interest = loan.debt * loan.rate / 100 / 12;
    RUB payment = loan.payment;
    if (payment > loan.debt + interest) // последний платеж
    {
        payment = loan.debt + interest;
    }

    loan.debt += interest - payment;
    den.cash -= payment;
}

void den_deposit(Deposit& deposit, const int month) {
    if (month == 1) // курс валюты меняется раз в год
    {
        deposit.currency_rate = deposit.currency_rate * (100 + deposit.currency_growth) / 100;
    }

    // проценты начисляются раз в месяц и остаются на вкладе
    deposit.amount += deposit.amount * deposit.rate / 100 / 12;
}

RUB deposit_in_rub(const Deposit deposit) {
    return deposit.amount * deposit.currency_rate / 100; // курс в копейках
}

void den_taxes(const int year, const int month) {
    if (month == 11 and den.car.bought) // налог на машину, до 1 декабря
    {
        den.cash -= den.car.horse_power * den.tax.car_per_hp;
    }

    if (month == 11 and den.flat.bought) // налог на первую квартиру
    {
        den.cash -= den.flat.price / 1000 * den.tax.property_permille;
    }

    if (month == 11 and den.second_flat.bought) // налог на вторую квартиру
    {
        den.cash -= den.second_flat.price / 1000 * den.tax.property_permille;
    }
}

void den_buy_car(const int year, const int month) {
    if (den.car.bought or den.cash < den.car.down_payment) // уже есть или еще не накопил
    {
        return;
    }

    den.cash -= den.car.down_payment;
    den.bank.car_loan.debt = den.car.price - den.car.down_payment;
    den.car.bought = true;
    printf("%d.%02d: Den bought a car\n", year, month);
}

void den_buy_flat(const int year, const int month) {
    if (den.flat.bought or den.cash < den.flat.down_payment) // уже есть или еще не накопил
    {
        return;
    }

    den.cash -= den.flat.down_payment;
    den.bank.mortgage.debt = den.flat.price - den.flat.down_payment;
    den.flat.bought = true;
    printf("%d.%02d: Den bought a flat\n", year, month);
}

void den_buy_second_flat(const int year, const int month) {
    if (not den.flat.bought) // сначала своя квартира
    {
        return;
    }

    if (den.second_flat.bought or den.cash < den.second_flat.down_payment)
    {
        return;
    }

    den.cash -= den.second_flat.down_payment;
    den.bank.second_mortgage.debt = den.second_flat.price - den.second_flat.down_payment;
    den.second_flat.bought = true;
    printf("%d.%02d: Den bought a second flat for rent\n", year, month);
}

void simulation() {
    int year = 2026;
    int month = 9;

    while ( not ( year == 2038 and month == 1 ) ) // до выплаты обеих ипотек
    {
        den_salary(year, month);
        den_rent_income(year, month);
        den_food(year, month);
        den_rent(year, month);
        den_home(year, month);
        den_car(year, month);
        den_health(year, month);
        den_loan(den.bank.car_loan);
        den_loan(den.bank.mortgage);
        den_loan(den.bank.second_mortgage);
        den_deposit(den.bank.deposit_rub, month);
        den_deposit(den.bank.deposit_eur, month);
        den_deposit(den.bank.deposit_cny, month);
        den_taxes(year, month);

        // покупки в конце месяца, когда все траты уже сделаны
        den_buy_car(year, month);
        den_buy_flat(year, month);
        den_buy_second_flat(year, month);
        // den_dog();
        // den_tax_refund();

        ++month;
        if (month == 13)
        {
            ++year;
            month = 1;
        }
    }
}

void den_init() {
    den.cash = 100'000;
    den.salary = 180'000;
    den.food = 25'000;

    // подержанная машина: 300 000 взнос + 400 000 в кредит
    den.car = {.bought = false, .price = 700'000, .down_payment = 300'000,
               .fuel = 8'000, .maintenance = 25'000, .repair = 40'000, .horse_power = 106};

    den.home = {.rent = 40'000, .bills = 7'000, .goods = 3'000, .repair = 50'000};

    // своя квартира: взнос 500 000 (20%) + 2 000 000 ипотека
    den.flat = {.bought = false, .price = 2'500'000, .down_payment = 500'000, .rent_income = 0};

    // квартира под сдачу: взнос 500 000 + 2 150 000 ипотека
    den.second_flat = {.bought = false, .price = 2'650'000, .down_payment = 500'000, .rent_income = 50'000};

    // платежи посчитаны по формуле аннуитета
    den.bank.car_loan = {.debt = 0, .payment = 14'063, .rate = 16};         // 3 года
    den.bank.mortgage = {.debt = 0, .payment = 37'335, .rate = 19};         // 10 лет
    den.bank.second_mortgage = {.debt = 0, .payment = 40'135, .rate = 19};  // 10 лет

    den.bank.deposit_rub = {.amount = 200'000, .rate = 12, .currency_rate = 100,  .currency_growth = 0};
    den.bank.deposit_eur = {.amount = 1'000,   .rate = 0,  .currency_rate = 9500, .currency_growth = 5};
    den.bank.deposit_cny = {.amount = 10'000,  .rate = 3,  .currency_rate = 1200, .currency_growth = 4};

    den.health = {.checkup = 15'000, .dentist = 12'000, .illness = 5'000};

    den.tax = {.car_per_hp = 35, .property_permille = 1, .rent_percent = 4};
}

void den_print() {
    RUB deposit_rub = deposit_in_rub(den.bank.deposit_rub);
    RUB deposit_eur = deposit_in_rub(den.bank.deposit_eur);
    RUB deposit_cny = deposit_in_rub(den.bank.deposit_cny);

    printf("\nDen salary = %d\n", den.salary);
    printf("Den cash = %d\n", den.cash);
    printf("Den car loan debt = %d\n", den.bank.car_loan.debt);
    printf("Den mortgage debt = %d\n", den.bank.mortgage.debt);
    printf("Den second mortgage debt = %d\n", den.bank.second_mortgage.debt);
    printf("Den deposit RUB = %d RUB\n", deposit_rub);
    printf("Den deposit EUR = %d RUB\n", deposit_eur);
    printf("Den deposit CNY = %d RUB\n", deposit_cny);
    printf("Den deposits total = %d RUB\n", deposit_rub + deposit_eur + deposit_cny);
}

int main() {
    den_init();

    simulation();

    den_print();
}
