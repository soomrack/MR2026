#include <stdio.h>

//typedef int RUB;
using RUB = int;

struct Job {
    bool works;         // работает ли Ден на этой работе
    RUB salary;         // оклад до вычета НДФЛ (gross)
    RUB bonus;          // годовая премия в декабре, тоже до вычета
    int indexation;     // индексация оклада, % в год
    RUB income_year;    // выплачено с начала года (gross)
    RUB withheld_year;  // удержано НДФЛ с начала года
};

struct Tax_notice {
    RUB income;         // доход за прошлый год по обеим работам
    RUB withheld;       // сколько НДФЛ удержали работодатели
    RUB extra;          // доплата по уведомлению ФНС за прошлый год
    RUB extra_total;    // сколько доплачено за все время симуляции
};

struct Life {
    RUB mobile;         // связь и интернет в месяц
    RUB subscriptions;  // онлайн-кинотеатр, музыка, облако в месяц
    RUB transport;      // проезд в месяц, пока нет машины
    RUB fun;            // кафе, кино, бары в месяц
    RUB gym;            // абонемент в зал в месяц
    RUB haircut;        // парикмахерская раз в два месяца
    RUB clothes;        // одежда два раза в год: весна и осень
    RUB gifts;          // подарки на дни рождения и Новый год
    RUB vacation;       // отпуск раз в год
};

struct Car {
    bool bought;        // куплена ли машина
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
    bool bought;        // куплена ли квартира
    RUB price;          // цена (для простоты она же кадастровая стоимость)
    RUB down_payment;   // первоначальный взнос
    RUB rent_income;    // сколько платят жильцы в месяц (0, если живем сами)
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
    Loan car_loan;
    Loan mortgage;
    Loan second_mortgage;
    Deposit deposit_rub;
    Deposit deposit_eur;
    Deposit deposit_cny;
};

struct Health {
    RUB checkup;        // обследование раз в год
    RUB dentist;        // стоматолог раз в год
    RUB illness;        // лекарства при болезни, раз в полгода
};

struct Tax {
    RUB car_per_hp;         // транспортный налог за 1 л.с.
    int property_permille;  // налог на квартиру в промилле (0.1% = 1)
    int rent_percent;       // НПД самозанятого со сдачи жилья физлицам, %
};

struct Person {
    RUB cash;
    Job main_job;
    Job side_job;
    Tax_notice notice;
    RUB food;
    Life life;
    Car car;
    Home home;
    Flat flat;
    Flat second_flat;
    Bank bank;
    Health health;
    Tax tax;
};

struct Person den;


// ---------------------------------------------------------------------------
// НДФЛ: прогрессивная шкала, действует с 2025 года.
// Ставка применяется не ко всему доходу, а к куску, попавшему в ступеньку.
// ---------------------------------------------------------------------------

RUB ndfl(const RUB income) {
    if (income <= 2'400'000) // первая ступенька: 13%
    {
        return income * 13 / 100;
    }

    RUB tax = 2'400'000 * 13 / 100; // 312 000 руб. с первой ступеньки

    if (income <= 5'000'000) // вторая ступенька: 15% с превышения над 2,4 млн
    {
        return tax + (income - 2'400'000) * 15 / 100;
    }

    tax += (5'000'000 - 2'400'000) * 15 / 100; // 390 000 руб. со второй ступеньки

    if (income <= 20'000'000) // третья ступенька: 18% с превышения над 5 млн
    {
        return tax + (income - 5'000'000) * 18 / 100;
    }

    tax += (20'000'000 - 5'000'000) * 18 / 100; // 2 700 000 руб. с третьей ступеньки

    return tax + (income - 20'000'000) * 20 / 100; // дальше 20%
}


// Упрощение: работодатель удерживает 13% с каждой выплаты.
// Прогрессивная шкала применяется один раз, при годовом перерасчете в январе.
// Функция возвращает сумму "на руки" и запоминает доход и удержанный НДФЛ.
RUB job_income(Job& job, const RUB gross) {
    RUB tax = gross * 13 / 100;

    job.income_year += gross;
    job.withheld_year += tax;

    return gross - tax;
}


// ---------------------------------------------------------------------------
// Доходы
// ---------------------------------------------------------------------------

void den_main_job(const int year, const int month) {
    if (not den.main_job.works) // уволился или еще не устроился
    {
        return;
    }

    if (month == 1) // индексация оклада, 13-я зарплата растет вместе с ним
    {
        den.main_job.salary = den.main_job.salary * (100 + den.main_job.indexation) / 100;
        den.main_job.bonus = den.main_job.bonus * (100 + den.main_job.indexation) / 100;
    }

    if (year == 2030 and month == 3) // повышение до senior
    {
        den.main_job.salary += 30'000;
        printf("%d.%02d: Den got a promotion\n", year, month);
    }

    den.cash += job_income(den.main_job, den.main_job.salary);
}

void den_side_job(const int year, const int month) {
    if (year == 2027 and month == 3) // нашел подработку по вечерам
    {
        den.side_job.works = true;
        printf("%d.%02d: Den started a second job\n", year, month);
    }

    if (not den.side_job.works) // подработки пока нет
    {
        return;
    }

    if (month == 1) // подработку индексируют скромнее основной работы
    {
        den.side_job.salary = den.side_job.salary * (100 + den.side_job.indexation) / 100;
    }

    den.cash += job_income(den.side_job, den.side_job.salary);
}

void den_bonus(const int year, const int month) {
    if (month != 12) // премию платят в декабре
    {
        return;
    }

    if (den.main_job.works and den.main_job.bonus > 0)
    {
        den.cash += job_income(den.main_job, den.main_job.bonus);
        printf("%d.%02d: Den got an annual bonus\n", year, month);
    }

    if (den.side_job.works and den.side_job.bonus > 0) // на подработке премии нет
    {
        den.cash += job_income(den.side_job, den.side_job.bonus);
    }
}

void den_rent_income(const int year, const int month) {
    if (not den.second_flat.bought) // вторую квартиру еще не купили
    {
        return;
    }

    if (month == 8) // раз в год поднимаем плату жильцам
    {
        den.second_flat.rent_income = den.second_flat.rent_income * 105 / 100;
    }

    // НПД платится самим самозанятым и в базу по НДФЛ не попадает
    RUB tax = den.second_flat.rent_income * den.tax.rent_percent / 100;
    den.cash += den.second_flat.rent_income - tax;
}


// ---------------------------------------------------------------------------
// Годовой перерасчет НДФЛ.
// В январе налоговая складывает доход обеих работ и применяет шкалу к сумме.
// Работодатели считали ступеньки каждый от своего нуля, поэтому возникает недобор.
// ---------------------------------------------------------------------------

void den_tax_year_close(const int year, const int month) {
    if (month != 1) // год закрывается один раз, в январе
    {
        return;
    }

    den.notice.income = den.main_job.income_year + den.side_job.income_year;
    den.notice.withheld = den.main_job.withheld_year + den.side_job.withheld_year;
    den.notice.extra = ndfl(den.notice.income) - den.notice.withheld;

    if (den.notice.extra > 0)
    {
        printf("%d.%02d: tax notice for %d: income %d, withheld %d, to pay %d\n",
               year, month, year - 1, den.notice.income, den.notice.withheld, den.notice.extra);
    }

    // счетчики обнуляются, начинается новый налоговый год
    den.main_job.income_year = 0;
    den.main_job.withheld_year = 0;
    den.side_job.income_year = 0;
    den.side_job.withheld_year = 0;
}

void den_ndfl_extra(const int year, const int month) {
    if (month != 11) // доплатить нужно до 1 декабря
    {
        return;
    }

    if (den.notice.extra <= 0) // работодатели удержали все сами
    {
        return;
    }

    den.cash -= den.notice.extra;
    den.notice.extra_total += den.notice.extra;
    printf("%d.%02d: Den paid extra ndfl = %d\n", year, month, den.notice.extra);
}


// ---------------------------------------------------------------------------
// Расходы: еда и жилье
// ---------------------------------------------------------------------------

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

    if (not den.flat.bought) // снимаем, пока нет своей квартиры
    {
        den.cash -= den.home.rent;
    }
}

void den_home(const int year, const int month) {
    if (month == 7) // индексация тарифов ЖКХ 10% в год
    {
        den.home.bills = den.home.bills * 110 / 100;
    }

    if (month == 2) // товары для дома дорожают вместе с продуктами
    {
        den.home.goods = den.home.goods * 108 / 100;
    }

    if (month == 6 and den.flat.bought) // косметический ремонт в своей квартире
    {
        den.cash -= den.home.repair;
    }

    den.cash -= den.home.bills;
    den.cash -= den.home.goods;
}


// ---------------------------------------------------------------------------
// Расходы: бытовые, у каждой своя инфляция и свой месяц пересмотра цены
// ---------------------------------------------------------------------------

void den_mobile(const int year, const int month) {
    if (month == 2) // оператор поднимает тариф 5% в год
    {
        den.life.mobile = den.life.mobile * 105 / 100;
    }

    den.cash -= den.life.mobile;
}

void den_subscriptions(const int year, const int month) {
    if (month == 1) // подписки дорожают заметнее всего
    {
        den.life.subscriptions = den.life.subscriptions * 112 / 100;
    }

    den.cash -= den.life.subscriptions;
}

void den_transport(const int year, const int month) {
    if (month == 1) // проездной дорожает вместе с тарифами
    {
        den.life.transport = den.life.transport * 108 / 100;
    }

    if (den.car.bought) // пересел на свою машину, бензин считается отдельно
    {
        return;
    }

    den.cash -= den.life.transport;
}

void den_fun(const int year, const int month) {
    if (month == 1) // кафе и кино дорожают 9% в год
    {
        den.life.fun = den.life.fun * 109 / 100;
    }

    den.cash -= den.life.fun;
}

void den_gym(const int year, const int month) {
    if (month == 9) // клуб пересматривает абонементы к началу сезона
    {
        den.life.gym = den.life.gym * 110 / 100;
    }

    den.cash -= den.life.gym;
}

void den_haircut(const int year, const int month) {
    if (month == 5) // мастер поднимает цену раз в год
    {
        den.life.haircut = den.life.haircut * 108 / 100;
    }

    if (month % 2 == 1) // стрижется раз в два месяца
    {
        den.cash -= den.life.haircut;
    }
}

void den_clothes(const int year, const int month) {
    if (month == 3) // одежда дорожает 7% в год
    {
        den.life.clothes = den.life.clothes * 107 / 100;
    }

    if (month == 4 or month == 10) // обновляет гардероб к весне и к зиме
    {
        den.cash -= den.life.clothes;
    }
}

void den_gifts(const int year, const int month) {
    if (month == 1) // подарки дорожают вместе со всем остальным
    {
        den.life.gifts = den.life.gifts * 109 / 100;
    }

    if (month == 3 or month == 6 or month == 9) // дни рождения близких
    {
        den.cash -= den.life.gifts;
    }

    if (month == 12) // Новый год обходится дороже обычного дня рождения
    {
        den.cash -= den.life.gifts * 2;
    }
}

void den_vacation(const int year, const int month) {
    if (month == 2) // путевки дорожают быстрее прочего
    {
        den.life.vacation = den.life.vacation * 112 / 100;
    }

    if (month == 7) // отпуск летом, раз в год
    {
        den.cash -= den.life.vacation;
    }
}


// ---------------------------------------------------------------------------
// Расходы: машина и здоровье
// ---------------------------------------------------------------------------

void den_car(const int year, const int month) {
    if (month == 1) // бензин дорожает 8% в год
    {
        den.car.fuel = den.car.fuel * 108 / 100;
    }

    if (not den.car.bought) // пока машины нет, тратить на нее нечего
    {
        return;
    }

    if (month == 4) // ТО
    {
        den.cash -= den.car.maintenance;
    }

    if (month == 10 and year % 3 == 0) // ремонт: подвеска, тормоза и т.д.
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


// ---------------------------------------------------------------------------
// Банк: кредиты и вклады
// ---------------------------------------------------------------------------

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

void den_car_loan(const int year, const int month) {
    den_loan(den.bank.car_loan);
}

void den_mortgage(const int year, const int month) {
    den_loan(den.bank.mortgage);
}

void den_second_mortgage(const int year, const int month) {
    den_loan(den.bank.second_mortgage);
}

void den_deposit(Deposit& deposit, const int month) {
    if (month == 1) // курс валюты меняется раз в год
    {
        deposit.currency_rate = deposit.currency_rate * (100 + deposit.currency_growth) / 100;
    }

    // проценты начисляются раз в месяц и остаются на вкладе
    deposit.amount += deposit.amount * deposit.rate / 100 / 12;
}

void den_deposit_rub(const int year, const int month) {
    den_deposit(den.bank.deposit_rub, month);
}

void den_deposit_eur(const int year, const int month) {
    den_deposit(den.bank.deposit_eur, month);
}

void den_deposit_cny(const int year, const int month) {
    den_deposit(den.bank.deposit_cny, month);
}

RUB deposit_in_rub(const Deposit& deposit) {
    return deposit.amount * deposit.currency_rate / 100; // курс в копейках
}


// ---------------------------------------------------------------------------
// Имущественные налоги (НДФЛ считается отдельно, выше)
// ---------------------------------------------------------------------------

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


// ---------------------------------------------------------------------------
// Крупные покупки
// ---------------------------------------------------------------------------

void den_buy_car(const int year, const int month) {
    if (den.car.bought or den.cash < den.car.down_payment) // уже есть или не накопили
    {
        return;
    }

    den.cash -= den.car.down_payment;
    den.bank.car_loan.debt = den.car.price - den.car.down_payment;
    den.car.bought = true;
    printf("%d.%02d: Den bought a car\n", year, month);
}

void den_buy_flat(const int year, const int month) {
    if (den.flat.bought or den.cash < den.flat.down_payment) // уже есть или не накопили
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


// ---------------------------------------------------------------------------
// Отчет в конце каждого года
// ---------------------------------------------------------------------------

void den_year_report(const int year, const int month) {
    if (month != 12) // отчет раз в год, когда все выплаты уже прошли
    {
        return;
    }

    RUB income = den.main_job.income_year + den.side_job.income_year;
    RUB withheld = den.main_job.withheld_year + den.side_job.withheld_year;

    printf("%d: gross = %d, ndfl = %d, cash = %d\n", year, income, withheld, den.cash);
}


// ---------------------------------------------------------------------------
// Симуляция
// ---------------------------------------------------------------------------

void simulation() {
    int year = 2026;
    int month = 9;

    while ( not ( year == 2038 and month == 1 ) ) // до выплаты обеих ипотек
    {
        den_tax_year_close(year, month);
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

        den_car_loan(year, month);
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

        ++month;
        if (month == 13)
        {
            ++year;
            month = 1;
        }
    }
}


// ---------------------------------------------------------------------------
// Начальное состояние
// ---------------------------------------------------------------------------

void den_init() {
    den.cash = 100'000;
    den.food = 25'000;

    // основная работа: оклад до вычета НДФЛ плюс 13-я зарплата в декабре
    den.main_job = {.works = true, .salary = 180'000, .bonus = 180'000,
                    .indexation = 7, .income_year = 0, .withheld_year = 0};

    // подработка появится в марте 2027, премий на ней не платят
    den.side_job = {.works = false, .salary = 70'000, .bonus = 0,
                    .indexation = 5, .income_year = 0, .withheld_year = 0};

    den.notice = {.income = 0, .withheld = 0, .extra = 0, .extra_total = 0};

    den.life = {.mobile = 1'200, .subscriptions = 1'000, .transport = 3'500,
                .fun = 9'000, .gym = 3'000, .haircut = 1'500,
                .clothes = 15'000, .gifts = 8'000, .vacation = 90'000};

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


// ---------------------------------------------------------------------------
// Итог
// ---------------------------------------------------------------------------

void den_print() {
    RUB deposit_rub = deposit_in_rub(den.bank.deposit_rub);
    RUB deposit_eur = deposit_in_rub(den.bank.deposit_eur);
    RUB deposit_cny = deposit_in_rub(den.bank.deposit_cny);

    printf("\nDen main job salary = %d (gross)\n", den.main_job.salary);
    printf("Den side job salary = %d (gross)\n", den.side_job.salary);
    printf("Den extra ndfl paid total = %d\n", den.notice.extra_total);
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
