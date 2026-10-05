#include <stdio.h>

//typedef int RUB;
using RUB = int;


// Государство: курсы валют и инфляция


enum class Currency {
    rub,
    eur,
    cny,
};

struct Exchange_rate {
    double rub;         // сколько рублей стоит единица валюты (1 евро = 95.0 руб.)
    int growth;         // на сколько % валюта дорожает за год
};

struct Inflation {      // на сколько % в год дорожает каждая статья расходов
    int food;
    int rent;           // аренда жилья (и плата наших жильцов)
    int bills;          // тарифы ЖКХ
    int home_goods;     // товары для дома
    int mobile;
    int subscriptions;
    int transport;
    int fun;
    int gym;
    int haircut;
    int clothes;
    int gifts;
    int vacation;
    int fuel;
    int medicine;
};

struct Government {
    Exchange_rate eur;
    Exchange_rate cny;
    Inflation inflation;
};


// Человек


struct Ndfl_counter {   // свой счетчик НДФЛ у каждого работодателя
    RUB income_year;    // выплачено с начала года (gross)
    RUB withheld_year;  // удержано НДФЛ с начала года
};

struct Job {
    bool is_working;    // работает ли человек на данной работе
    RUB salary;         // оклад до вычета НДФЛ (на руки) - gross
    RUB bonus;          // годовая премия в декабре, до вычета
    int indexation;     // индексация оклада, % в год
    Ndfl_counter ndfl;
};

struct Ndfl {           // все налоги
    int withhold_percent;   // сколько % удерживает работодатель с каждой выплаты

    RUB income;             // доход за прошлый год по всем работам
    RUB withheld;           // сколько НДФЛ удержали все работодатели
    RUB extra;              // доплата за прошлый год
    RUB extra_total;        // сколько доплачено за все время

    // имущественные налоги и НПД
    RUB car_per_hp;         // транспортный налог за 1 л.с.
    int property_permille;  // налог на квартиру в промилле
    int rent_percent;       // НПД самозанятого со сдачи жилья физлицам, %
};

struct Life {
    RUB mobile;         // связь и интернет в месяц
    RUB subscriptions;  // онлайн-кинотеатр, музыка, облако в месяц
    RUB transport;      // проезд в месяц, пока нет машины
    RUB fun;            // кафе, кино, бары в месяц
    RUB gym;            // абонемент в зал в месяц
    RUB haircut;        // парикмахерская раз в два месяца
    RUB clothes;        // одежда два раза в год: весна осень
    RUB gifts;          // подарки на дни рождения и нг
    RUB vacation;       // отпуск раз в год
};

struct Car {
    bool is_bought;     // куплена ли машина
    RUB price;          // цена машины
    RUB down_payment;   // первоначальный взнос
    RUB fuel;           // бензин в месяц
    RUB maintenance;    // ТО раз в год
    RUB repair;         // ремонт раз в 3 года
    int horse_power;    // нужна для транспортного налога
};

struct Home {
    RUB rent;           // аренда, пока нет своей квартиры
    RUB bills;          // ЖКХ в месяц
    RUB goods;          // товары для дома в месяц
    RUB repair;         // косметический ремонт раз в год в своей квартире
};

struct Flat {
    bool is_bought;     // куплена ли квартира
    RUB price;          // цена (для простоты она же кадастровая стоимость)
    RUB down_payment;   // первоначальный взнос
    RUB rent_income;    // сколько платят жильцы в месяц (0, если живем сами)
};

struct Credit {         // потребительский кредит или авто
    RUB debt;           // остаток долга
    RUB payment;        // ежемесячный платеж
    int rate;           // % годовых
    int months;         // срок кредита
};

struct Mortgage {       // ипотека
    RUB debt;           // остаток долга
    RUB payment;        // ежемесячный платеж
    int rate;           // % годовых
    int years;          // срок ипотеки
    RUB interest_paid;  // уплачено процентов за все время, пригодится для вычета по процентам
};

struct Deposit {
    Currency currency;  // курс берем у государства
    int amount;         // сумма в валюте вклада (рубли, евро или юани)
    int rate;           // % годовых
};

struct Bank {
    RUB account;        // текущий счет: сюда приходят доходы, отсюда все траты
    Deposit deposit_rub;
    Deposit deposit_eur;
    Deposit deposit_cny;
    Credit car_credit;
    Mortgage mortgage;
    Mortgage second_mortgage;
};

struct Health {
    RUB checkup;        // обследование раз в год
    RUB dentist;        // стоматолог раз в год
    RUB illness;        // лекарства при болезни, раз в полгода
};

struct Person {
    Job main_job;
    Job side_job;
    Ndfl ndfl;
    RUB food;
    Life life;
    Car car;
    Home home;
    Flat flat;
    Flat second_flat;
    Bank bank;
    Health health;
};

struct Government government;
struct Person den;       // подробная модель: две работы, НДФЛ, машина, две квартиры, валюта
struct Person hanna;     // упрощенная модель: одна работа, одна квартира, рублевый вклад


RUB add_percent(const RUB value, const int percent) {
    return value * (100 + percent) / 100;
}


// государство


double currency_rate(const Currency currency) { // сколько рублей стоит единица валюты
    switch (currency)
    {
        case Currency::rub: return 1.0;
        case Currency::eur: return government.eur.rub;
        case Currency::cny: return government.cny.rub;
    }
    return 1.0;
}

void exchange_rate_growth(Exchange_rate& exchange_rate) {
    exchange_rate.rub = exchange_rate.rub * (100 + exchange_rate.growth) / 100;
}

void government_currency(const int year, const int month) {
    if (month != 1) // курсы валют меняются раз в год
    {
        return;
    }

    exchange_rate_growth(government.eur);
    exchange_rate_growth(government.cny);
}


// НДФЛ


RUB ndfl(const RUB income) {
    if (income <= 2'400'000) // первая ступень: 13%
    {
        return income * 13 / 100;
    }

    RUB tax = 2'400'000 * 13 / 100; // 312 000 руб. с первой ступени

    if (income <= 5'000'000) // вторая ступень: 15% > над 2,4 млн
    {
        return tax + (income - 2'400'000) * 15 / 100;
    }

    tax += (5'000'000 - 2'400'000) * 15 / 100; // 390 000 руб. со второй ступени

    if (income <= 20'000'000) // третья ступень: 18% > 5 млн
    {
        return tax + (income - 5'000'000) * 18 / 100;
    }

    tax += (20'000'000 - 5'000'000) * 18 / 100; // 2 700 000 руб. с третьей ступени

    return tax + (income - 20'000'000) * 20 / 100; // дальше 20%
}


// каждый работодатель ведет свой счетчик и удерживает 13% с каждой выплаты,
// о доходах Дена у другого работодателя он не знает
// прогрессивная шкала применяется один раз при годовом перерасчете в январе


RUB job_income(Job& job, const RUB gross) {
    RUB tax = gross * den.ndfl.withhold_percent / 100;

    job.ndfl.income_year += gross;
    job.ndfl.withheld_year += tax;

    return gross - tax;
}



// Доходы


void den_main_job(const int year, const int month) {
    if (not den.main_job.is_working) // уволился или еще не устроился
    {
        return;
    }

    if (month == 1) // индексация оклада, 13-я зарплата растет вместе с ним
    {
        den.main_job.salary = add_percent(den.main_job.salary, den.main_job.indexation);
        den.main_job.bonus = add_percent(den.main_job.bonus, den.main_job.indexation);
    }

    if (year == 2030 and month == 3) // повышение
    {
        den.main_job.salary += 30'000;
        printf("%d.%02d: Den got a promotion\n", year, month);
    }

    den.bank.account += job_income(den.main_job, den.main_job.salary);
}

void den_side_job(const int year, const int month) {
    if (year == 2027 and month == 3) // нашел подработку
    {
        den.side_job.is_working = true;
        printf("%d.%02d: Den started a second job\n", year, month);
    }

    if (not den.side_job.is_working) // подработки  нет
    {
        return;
    }

    if (month == 1) // индексация подработки
    {
        den.side_job.salary = add_percent(den.side_job.salary, den.side_job.indexation);
    }

    den.bank.account += job_income(den.side_job, den.side_job.salary);
}

void den_bonus(const int year, const int month) {
    if (month != 12) // премию платят в декабре
    {
        return;
    }

    if (den.main_job.is_working and den.main_job.bonus > 0)
    {
        den.bank.account += job_income(den.main_job, den.main_job.bonus);
        printf("%d.%02d: Den got an annual bonus\n", year, month);
    }

    if (den.side_job.is_working and den.side_job.bonus > 0) // на подработке премии нет
    {
        den.bank.account += job_income(den.side_job, den.side_job.bonus);
    }
}

void den_rent_income(const int year, const int month) {
    if (not den.second_flat.is_bought) // вторую квартиру еще не купил
    {
        return;
    }

    if (month == 8) // раз в год поднимаем плату жильцам вслед за рынком аренды
    {
        den.second_flat.rent_income = add_percent(den.second_flat.rent_income, government.inflation.rent);
    }

    // НПД платится самим самозанятым и в базу по НДФЛ не попадает
    RUB tax = den.second_flat.rent_income * den.ndfl.rent_percent / 100;
    den.bank.account += den.second_flat.rent_income - tax;
}


// Годовой перерасчет НДФЛ


void den_ndfl_year_close(const int year, const int month) {
    if (month != 1) // год закрывается один раз, в январе
    {
        return;
    }

    // ФНС складывает счетчики всех работодателей и применяет шкалу к общей сумме
    den.ndfl.income = den.main_job.ndfl.income_year + den.side_job.ndfl.income_year;
    den.ndfl.withheld = den.main_job.ndfl.withheld_year + den.side_job.ndfl.withheld_year;
    den.ndfl.extra = ndfl(den.ndfl.income) - den.ndfl.withheld;

    if (den.ndfl.extra > 0)
    {
        printf("%d.%02d: tax notice for %d: income %d, withheld %d, to pay %d\n",
               year, month, year - 1, den.ndfl.income, den.ndfl.withheld, den.ndfl.extra);
    }

    // начинается новый налоговый год, каждый работодатель обнуляет свой счетчик
    den.main_job.ndfl = {.income_year = 0, .withheld_year = 0};
    den.side_job.ndfl = {.income_year = 0, .withheld_year = 0};
}

void den_ndfl_extra(const int year, const int month) {
    if (month != 11) // доплатить ндфл нужно до 1 декабря
    {
        return;
    }

    if (den.ndfl.extra <= 0) // работодатели удержали все сами
    {
        return;
    }

    den.bank.account -= den.ndfl.extra;
    den.ndfl.extra_total += den.ndfl.extra;
    printf("%d.%02d: Den paid extra ndfl = %d\n", year, month, den.ndfl.extra);
}


// Расходы: еда и жилье

void den_food(const int year, const int month) {
    if (month == 1) // продукты дорожают раз в год
    {
        den.food = add_percent(den.food, government.inflation.food);
    }

    den.bank.account -= den.food;
}

void den_rent(const int year, const int month) {
    if (month == 9) // хозяин поднимает аренду раз в год
    {
        den.home.rent = add_percent(den.home.rent, government.inflation.rent);
    }

    if (not den.flat.is_bought) // снимаем, пока нет своей квартиры
    {
        den.bank.account -= den.home.rent;
    }
}

void den_home(const int year, const int month) {
    if (month == 7) // индексация тарифов ЖКХ
    {
        den.home.bills = add_percent(den.home.bills, government.inflation.bills);
    }

    if (month == 2) // товары для дома
    {
        den.home.goods = add_percent(den.home.goods, government.inflation.home_goods);
    }

    if (month == 6 and den.flat.is_bought) // косметический ремонт в своей квартире
    {
        den.bank.account -= den.home.repair;
    }

    den.bank.account -= den.home.bills;
    den.bank.account -= den.home.goods;
}


// Расходы: бытовые, у каждой своя инфляция и свой месяц пересмотра цены

void den_mobile(const int year, const int month) {
    if (month == 2) // оператор поднимает тариф
    {
        den.life.mobile = add_percent(den.life.mobile, government.inflation.mobile);
    }

    den.bank.account -= den.life.mobile;
}

void den_subscriptions(const int year, const int month) {
    if (month == 1) // подписки дорожают заметнее всего
    {
        den.life.subscriptions = add_percent(den.life.subscriptions, government.inflation.subscriptions);
    }

    den.bank.account -= den.life.subscriptions;
}

void den_transport(const int year, const int month) {
    if (month == 1) // проездной дорожает вместе с тарифами
    {
        den.life.transport = add_percent(den.life.transport, government.inflation.transport);
    }

    if (den.car.is_bought) // пересел на свою машину, бензин считается отдельно
    {
        return;
    }

    den.bank.account -= den.life.transport;
}

void den_fun(const int year, const int month) {
    if (month == 1) // кафе и кино
    {
        den.life.fun = add_percent(den.life.fun, government.inflation.fun);
    }

    den.bank.account -= den.life.fun;
}

void den_gym(const int year, const int month) {
    if (month == 9) // клуб пересматривает абонементы к началу сезона
    {
        den.life.gym = add_percent(den.life.gym, government.inflation.gym);
    }

    den.bank.account -= den.life.gym;
}

void den_haircut(const int year, const int month) {
    if (month == 5) // мастер поднимает цену раз в год
    {
        den.life.haircut = add_percent(den.life.haircut, government.inflation.haircut);
    }

    if (month % 2 == 1) // стрижется раз в два месяца
    {
        den.bank.account -= den.life.haircut;
    }
}

void den_clothes(const int year, const int month) {
    if (month == 3) // одежда дорожает к новому сезону
    {
        den.life.clothes = add_percent(den.life.clothes, government.inflation.clothes);
    }

    if (month == 4 or month == 10) // обновляет гардероб к весне и к зиме
    {
        den.bank.account -= den.life.clothes;
    }
}

void den_gifts(const int year, const int month) {
    if (month == 1) // подарки дорожают вместе со всем остальным
    {
        den.life.gifts = add_percent(den.life.gifts, government.inflation.gifts);
    }

    if (month == 3 or month == 6 or month == 9) // дни рождения близких
    {
        den.bank.account -= den.life.gifts;
    }

    if (month == 12) // Новый год обходится дороже обычного дня рождения
    {
        den.bank.account -= den.life.gifts * 2;
    }
}

void den_vacation(const int year, const int month) {
    if (month == 2) // путевки дорожают быстрее прочего
    {
        den.life.vacation = add_percent(den.life.vacation, government.inflation.vacation);
    }

    if (month == 7) // отпуск летом, раз в год
    {
        den.bank.account -= den.life.vacation;
    }
}


// Расходы: машина и здоровье


void den_car(const int year, const int month) {
    if (month == 1) // бензин дорожает раз в год
    {
        den.car.fuel = add_percent(den.car.fuel, government.inflation.fuel);
    }

    if (not den.car.is_bought) // пока машины нет, тратить на нее нечего
    {
        return;
    }

    if (month == 4) // ТО
    {
        den.bank.account -= den.car.maintenance;
    }

    if (month == 10 and year % 3 == 0) // ремонт: подвеска, тормоза и т.д.
    {
        den.bank.account -= den.car.repair;
    }

    den.bank.account -= den.car.fuel;
}

void den_health(const int year, const int month) {
    if (month == 1) // медицина дорожает раз в год
    {
        den.health.checkup = add_percent(den.health.checkup, government.inflation.medicine);
        den.health.dentist = add_percent(den.health.dentist, government.inflation.medicine);
        den.health.illness = add_percent(den.health.illness, government.inflation.medicine);
    }

    if (month == 2) // ежегодное обследование
    {
        den.bank.account -= den.health.checkup;
    }

    if (month == 8) // стоматолог
    {
        den.bank.account -= den.health.dentist;
    }

    if (month == 3 or month == 11) // простуда весной и осенью
    {
        den.bank.account -= den.health.illness;
    }
}


// Банк: кредиты и вклады


RUB month_interest(const RUB debt, const int rate) { // проценты за месяц
    return debt * rate / 100 / 12;
}

RUB month_payment(const RUB debt, const RUB interest, const RUB payment) {
    if (payment > debt + interest) // последний платеж
    {
        return debt + interest;
    }

    return payment;
}

void bank_credit(Bank& bank, Credit& credit) {
    if (credit.debt <= 0) // кредита нет или он выплачен
    {
        return;
    }

    RUB interest = month_interest(credit.debt, credit.rate);
    RUB payment = month_payment(credit.debt, interest, credit.payment);

    credit.debt += interest - payment;
    bank.account -= payment; // платеж списывается со счета в том же банке
}

void bank_mortgage(Bank& bank, Mortgage& mortgage) {
    if (mortgage.debt <= 0) // ипотеки нет или она выплачена
    {
        return;
    }

    RUB interest = month_interest(mortgage.debt, mortgage.rate);
    RUB payment = month_payment(mortgage.debt, interest, mortgage.payment);

    mortgage.debt += interest - payment;
    mortgage.interest_paid += interest;
    bank.account -= payment; // платеж списывается со счета в том же банке
}

void den_car_credit(const int year, const int month) {
    bank_credit(den.bank, den.bank.car_credit);
}

void den_mortgage(const int year, const int month) {
    bank_mortgage(den.bank, den.bank.mortgage);
}

void den_second_mortgage(const int year, const int month) {
    bank_mortgage(den.bank, den.bank.second_mortgage);
}

void bank_deposit(Deposit& deposit) {
    // проценты начисляются раз в месяц и остаются на вкладе
    deposit.amount += deposit.amount * deposit.rate / 100 / 12;
}

void den_deposit_rub(const int year, const int month) {
    bank_deposit(den.bank.deposit_rub);
}

void den_deposit_eur(const int year, const int month) {
    bank_deposit(den.bank.deposit_eur);
}

void den_deposit_cny(const int year, const int month) {
    bank_deposit(den.bank.deposit_cny);
}

RUB deposit_in_rub(const Deposit& deposit) {
    return (RUB)(deposit.amount * currency_rate(deposit.currency)); // копейки отбрасываем
}


// Имущественные налоги


void den_taxes(const int year, const int month) {
    if (month == 11 and den.car.is_bought) // налог на машину, до 1 декабря
    {
        den.bank.account -= den.car.horse_power * den.ndfl.car_per_hp;
    }

    if (month == 11 and den.flat.is_bought) // налог на первую квартиру
    {
        den.bank.account -= den.flat.price / 1000 * den.ndfl.property_permille;
    }

    if (month == 11 and den.second_flat.is_bought) // налог на вторую квартиру
    {
        den.bank.account -= den.second_flat.price / 1000 * den.ndfl.property_permille;
    }
}


// Крупные покупки


void den_buy_car(const int year, const int month) {
    if (den.car.is_bought or den.bank.account < den.car.down_payment) // уже есть или не накопили
    {
        return;
    }

    den.bank.account -= den.car.down_payment;
    den.bank.car_credit.debt = den.car.price - den.car.down_payment;
    den.car.is_bought = true;
    printf("%d.%02d: Den bought a car\n", year, month);
}

void den_buy_flat(const int year, const int month) {
    if (den.flat.is_bought or den.bank.account < den.flat.down_payment) // уже есть или не накопили
    {
        return;
    }

    den.bank.account -= den.flat.down_payment;
    den.bank.mortgage.debt = den.flat.price - den.flat.down_payment;
    den.flat.is_bought = true;
    printf("%d.%02d: Den bought a flat\n", year, month);
}

void den_buy_second_flat(const int year, const int month) {
    if (not den.flat.is_bought) // сначала своя квартира
    {
        return;
    }

    if (den.second_flat.is_bought or den.bank.account < den.second_flat.down_payment)
    {
        return;
    }

    den.bank.account -= den.second_flat.down_payment;
    den.bank.second_mortgage.debt = den.second_flat.price - den.second_flat.down_payment;
    den.second_flat.is_bought = true;
    printf("%d.%02d: Den bought a second flat for rent\n", year, month);
}


// Отчет в конце каждого года


void den_year_report(const int year, const int month) {
    if (month != 12) // отчет раз в год, когда все выплаты уже прошли
    {
        return;
    }

    RUB income = den.main_job.ndfl.income_year + den.side_job.ndfl.income_year;
    RUB withheld = den.main_job.ndfl.withheld_year + den.side_job.ndfl.withheld_year;

    printf("%d: Den gross = %d, ndfl = %d, account = %d\n", year, income, withheld, den.bank.account);
}



// ============================================================
// Ханна: упрощенная модель
// одна работа без премии, НДФЛ только 13% у работодателя (без перерасчета),
// нет машины, второй квартиры, валютных вкладов и имущественных налогов
// ============================================================


// Ханна: доходы


void hanna_job(const int year, const int month) {
    if (not hanna.main_job.is_working)
    {
        return;
    }

    if (month == 1) // индексация оклада
    {
        hanna.main_job.salary = add_percent(hanna.main_job.salary, hanna.main_job.indexation);
    }

    // работодатель удерживает 13%, до прогрессивной шкалы Ханна не дорастает
    RUB tax = hanna.main_job.salary * hanna.ndfl.withhold_percent / 100;
    hanna.bank.account += hanna.main_job.salary - tax;
}


// Ханна: еда и жилье


void hanna_food(const int year, const int month) {
    if (month == 1) // продукты дорожают раз в год
    {
        hanna.food = add_percent(hanna.food, government.inflation.food);
    }

    hanna.bank.account -= hanna.food;
}

void hanna_rent(const int year, const int month) {
    if (month == 3) // у Ханны договор аренды продлевается весной
    {
        hanna.home.rent = add_percent(hanna.home.rent, government.inflation.rent);
    }

    if (not hanna.flat.is_bought) // снимает, пока нет своей квартиры
    {
        hanna.bank.account -= hanna.home.rent;
    }
}

void hanna_home(const int year, const int month) {
    if (month == 7) // индексация тарифов ЖКХ
    {
        hanna.home.bills = add_percent(hanna.home.bills, government.inflation.bills);
    }

    if (month == 2) // товары для дома
    {
        hanna.home.goods = add_percent(hanna.home.goods, government.inflation.home_goods);
    }

    if (month == 5 and hanna.flat.is_bought) // косметический ремонт в своей квартире
    {
        hanna.bank.account -= hanna.home.repair;
    }

    hanna.bank.account -= hanna.home.bills;
    hanna.bank.account -= hanna.home.goods;
}


// Ханна: бытовые расходы


void hanna_mobile(const int year, const int month) {
    if (month == 2) // оператор поднимает тариф
    {
        hanna.life.mobile = add_percent(hanna.life.mobile, government.inflation.mobile);
    }

    hanna.bank.account -= hanna.life.mobile;
}

void hanna_transport(const int year, const int month) {
    if (month == 1) // проездной дорожает раз в год
    {
        hanna.life.transport = add_percent(hanna.life.transport, government.inflation.transport);
    }

    hanna.bank.account -= hanna.life.transport; // машины нет, всегда ездит на транспорте
}

void hanna_fun(const int year, const int month) {
    if (month == 1) // кафе и кино
    {
        hanna.life.fun = add_percent(hanna.life.fun, government.inflation.fun);
    }

    hanna.bank.account -= hanna.life.fun;
}

void hanna_haircut(const int year, const int month) {
    if (month == 5) // салон поднимает цены раз в год
    {
        hanna.life.haircut = add_percent(hanna.life.haircut, government.inflation.haircut);
    }

    hanna.bank.account -= hanna.life.haircut; // салон каждый месяц
}

void hanna_clothes(const int year, const int month) {
    if (month == 3) // одежда дорожает к новому сезону
    {
        hanna.life.clothes = add_percent(hanna.life.clothes, government.inflation.clothes);
    }

    if (month == 4 or month == 10) // обновляет гардероб к весне и к зиме
    {
        hanna.bank.account -= hanna.life.clothes;
    }
}

void hanna_vacation(const int year, const int month) {
    if (month == 2) // путевки дорожают быстрее прочего
    {
        hanna.life.vacation = add_percent(hanna.life.vacation, government.inflation.vacation);
    }

    if (month == 8) // отпуск в конце лета
    {
        hanna.bank.account -= hanna.life.vacation;
    }
}

void hanna_health(const int year, const int month) {
    if (month == 1) // медицина дорожает раз в год
    {
        hanna.health.checkup = add_percent(hanna.health.checkup, government.inflation.medicine);
        hanna.health.dentist = add_percent(hanna.health.dentist, government.inflation.medicine);
    }

    if (month == 3) // ежегодное обследование
    {
        hanna.bank.account -= hanna.health.checkup;
    }

    if (month == 10) // стоматолог
    {
        hanna.bank.account -= hanna.health.dentist;
    }
}


// Ханна: банк


void hanna_mortgage(const int year, const int month) {
    bank_mortgage(hanna.bank, hanna.bank.mortgage);
}

void hanna_deposit_rub(const int year, const int month) {
    bank_deposit(hanna.bank.deposit_rub);
}


// Ханна: покупка квартиры


void hanna_buy_flat(const int year, const int month) {
    if (hanna.flat.is_bought or hanna.bank.account < hanna.flat.down_payment) // уже есть или не накопила
    {
        return;
    }

    hanna.bank.account -= hanna.flat.down_payment;
    hanna.bank.mortgage.debt = hanna.flat.price - hanna.flat.down_payment;
    hanna.flat.is_bought = true;
    printf("%d.%02d: Hanna bought a flat\n", year, month);
}


// Ханна: отчет в конце года


void hanna_year_report(const int year, const int month) {
    if (month != 12)
    {
        return;
    }

    printf("%d: Hanna account = %d, mortgage debt = %d\n",
           year, hanna.bank.account, hanna.bank.mortgage.debt);
}


void simulation() {
    int year = 2026;
    int month = 9;

    while ( not ( year == 2038 and month == 1 ) ) // до выплаты обеих ипотек
    {
        government_currency(year, month);

        den_ndfl_year_close(year, month);
        den_main_job(year, month);
        den_side_job(year, month);
        den_bonus(year, month);
        den_rent_income(year, month);

        den_food(year, month);
        den_rent(year, month);
        den_home(year, month);

        den_mobile(year, month);
        den_subscriptions(year, month);
        den_transport(year, month);
        den_fun(year, month);
        den_gym(year, month);
        den_haircut(year, month);
        den_clothes(year, month);
        den_gifts(year, month);
        den_vacation(year, month);

        den_car(year, month);
        den_health(year, month);

        den_car_credit(year, month);
        den_mortgage(year, month);
        den_second_mortgage(year, month);
        den_deposit_rub(year, month);
        den_deposit_eur(year, month);
        den_deposit_cny(year, month);

        den_taxes(year, month);
        den_ndfl_extra(year, month);

        // покупки в конце месяца, когда все траты уже учтены
        den_buy_car(year, month);
        den_buy_flat(year, month);
        den_buy_second_flat(year, month);
        // den_dog();
        // den_tax_refund();

        den_year_report(year, month);


        // Ханна

        hanna_job(year, month);

        hanna_food(year, month);
        hanna_rent(year, month);
        hanna_home(year, month);

        hanna_mobile(year, month);
        hanna_transport(year, month);
        hanna_fun(year, month);
        hanna_haircut(year, month);
        hanna_clothes(year, month);
        hanna_vacation(year, month);
        hanna_health(year, month);

        hanna_mortgage(year, month);
        hanna_deposit_rub(year, month);

        hanna_buy_flat(year, month);

        hanna_year_report(year, month);

        ++month;
        if (month == 13)
        {
            ++year;
            month = 1;
        }
    }
}


// Начальное состояние


void government_init() {
    government.eur = {.rub = 95.0, .growth = 5};
    government.cny = {.rub = 12.0, .growth = 4};

    government.inflation = {.food = 9, .rent = 5, .bills = 10, .home_goods = 8,
                            .mobile = 5, .subscriptions = 12, .transport = 8,
                            .fun = 9, .gym = 10, .haircut = 8, .clothes = 7,
                            .gifts = 9, .vacation = 12, .fuel = 8, .medicine = 10};
}

void den_init() {
    den.food = 25'000;

    // основная работа: оклад до вычета НДФЛ и 13-я зарплата в декабре
    den.main_job = {.is_working = true, .salary = 180'000, .bonus = 180'000,
                    .indexation = 7, .ndfl = {.income_year = 0, .withheld_year = 0}};

    // подработка появится в марте 2027
    den.side_job = {.is_working = false, .salary = 70'000, .bonus = 0,
                    .indexation = 5, .ndfl = {.income_year = 0, .withheld_year = 0}};

    den.ndfl = {.withhold_percent = 13,
                .income = 0, .withheld = 0, .extra = 0, .extra_total = 0,
                .car_per_hp = 35, .property_permille = 1, .rent_percent = 4};

    den.life = {.mobile = 1'200, .subscriptions = 1'000, .transport = 3'500,
                .fun = 9'000, .gym = 3'000, .haircut = 1'500,
                .clothes = 15'000, .gifts = 8'000, .vacation = 90'000};

    // машина: 300 000 взнос + 400 000 в кредит
    den.car = {.is_bought = false, .price = 700'000, .down_payment = 300'000,
               .fuel = 8'000, .maintenance = 25'000, .repair = 40'000, .horse_power = 106};

    den.home = {.rent = 40'000, .bills = 7'000, .goods = 3'000, .repair = 50'000};

    // своя квартира: взнос 500 000 (20%) + 2 000 000 ипотека
    den.flat = {.is_bought = false, .price = 2'500'000, .down_payment = 500'000, .rent_income = 0};

    // квартира под сдачу: взнос 500 000 + 2 150 000 ипотека
    den.second_flat = {.is_bought = false, .price = 2'650'000, .down_payment = 500'000, .rent_income = 50'000};

    den.bank.account = 100'000;

    den.bank.deposit_rub = {.currency = Currency::rub, .amount = 200'000, .rate = 12};
    den.bank.deposit_eur = {.currency = Currency::eur, .amount = 1'000,   .rate = 0};
    den.bank.deposit_cny = {.currency = Currency::cny, .amount = 10'000,  .rate = 3};

    // платежи посчитаны по аннуитетной формуле
    den.bank.car_credit = {.debt = 0, .payment = 14'063, .rate = 16, .months = 36};
    den.bank.mortgage = {.debt = 0, .payment = 37'335, .rate = 19, .years = 10, .interest_paid = 0};
    den.bank.second_mortgage = {.debt = 0, .payment = 40'135, .rate = 19, .years = 10, .interest_paid = 0};

    den.health = {.checkup = 15'000, .dentist = 12'000, .illness = 5'000};
}

void hanna_init() {
    hanna.food = 20'000;

    // одна работа, без премии и без подработки
    hanna.main_job = {.is_working = true, .salary = 120'000, .bonus = 0,
                      .indexation = 6, .ndfl = {.income_year = 0, .withheld_year = 0}};

    // из налогов у Ханны только 13%, которые удерживает работодатель
    hanna.ndfl = {.withhold_percent = 13,
                  .income = 0, .withheld = 0, .extra = 0, .extra_total = 0,
                  .car_per_hp = 0, .property_permille = 0, .rent_percent = 0};

    // спортзал, подарки и подписки Ханна не оплачивает, поэтому там нули
    hanna.life = {.mobile = 900, .subscriptions = 0, .transport = 3'000,
                  .fun = 7'000, .gym = 0, .haircut = 2'500,
                  .clothes = 20'000, .gifts = 0, .vacation = 70'000};

    hanna.home = {.rent = 35'000, .bills = 6'000, .goods = 2'500, .repair = 40'000};

    // своя квартира: взнос 440 000 (20%) + 1 760 000 ипотека
    hanna.flat = {.is_bought = false, .price = 2'200'000, .down_payment = 440'000, .rent_income = 0};

    hanna.bank.account = 50'000;
    hanna.bank.deposit_rub = {.currency = Currency::rub, .amount = 100'000, .rate = 12};

    // платеж посчитан по аннуитетной формуле
    hanna.bank.mortgage = {.debt = 0, .payment = 32'854, .rate = 19, .years = 10, .interest_paid = 0};

    hanna.health = {.checkup = 12'000, .dentist = 10'000, .illness = 0};
}



// Итог


void den_print() {
    RUB deposit_rub = deposit_in_rub(den.bank.deposit_rub);
    RUB deposit_eur = deposit_in_rub(den.bank.deposit_eur);
    RUB deposit_cny = deposit_in_rub(den.bank.deposit_cny);

    printf("\nDen main job salary = %d (gross)\n", den.main_job.salary);
    printf("Den side job salary = %d (gross)\n", den.side_job.salary);
    printf("Den extra ndfl paid total = %d\n", den.ndfl.extra_total);
    printf("Den bank account = %d\n", den.bank.account);
    printf("Den car credit debt = %d\n", den.bank.car_credit.debt);
    printf("Den mortgage debt = %d\n", den.bank.mortgage.debt);
    printf("Den second mortgage debt = %d\n", den.bank.second_mortgage.debt);
    printf("Den mortgage interest paid = %d\n", den.bank.mortgage.interest_paid + den.bank.second_mortgage.interest_paid);
    printf("Den deposit RUB = %d RUB\n", deposit_rub);
    printf("Den deposit EUR = %d RUB\n", deposit_eur);
    printf("Den deposit CNY = %d RUB\n", deposit_cny);
    printf("Den deposits total = %d RUB\n", deposit_rub + deposit_eur + deposit_cny);
}

void hanna_print() {
    printf("\nHanna salary = %d (gross)\n", hanna.main_job.salary);
    printf("Hanna bank account = %d\n", hanna.bank.account);
    printf("Hanna mortgage debt = %d\n", hanna.bank.mortgage.debt);
    printf("Hanna mortgage interest paid = %d\n", hanna.bank.mortgage.interest_paid);
    printf("Hanna deposit RUB = %d RUB\n", deposit_in_rub(hanna.bank.deposit_rub));
}

int main() {
    government_init();
    den_init();
    hanna_init();

    simulation();

    den_print();
    hanna_print();
}
