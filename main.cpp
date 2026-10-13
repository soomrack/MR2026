#include <stdio.h>

//typedef int RUB;
using RUB = int;

// Структуры данных

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
    int home_goods;     // все товары для дома
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
    int pet;            // корм, ветеринар и все для питомца
    int appliances;     // бытовая техника
    int car;            // новые машины
    int renovation;     // ремонт квартиры: материалы и работа
};

struct Government {
    Exchange_rate eur;
    Exchange_rate cny;
    Inflation inflation;
};

// Работа и налоги

struct Ndfl_counter {   //  у каждого работодателя свой счетчик НДФЛ
    RUB income_year;    // выплачено с начала года (gross)
    RUB withheld_year;  // удержано НДФЛ с начала года
};

struct Job {
    bool is_working;    // работает ли человек на данной работе
    RUB salary;         // оклад до вычета НДФЛ (на руки) - gross
    RUB bonus;          // премия до вычета (у Дена годовая, у Ханны квартальная)
    int indexation;     // индексация оклада, % в год
    Ndfl_counter ndfl;
};

struct Tax_refund {         // возврат НДФЛ по декларации 3-НДФЛ
    RUB property_left;      // остаток имущественного вычета за квартиру (лимит 2 000 000)
    RUB interest_left;      // остаток вычета по процентам ипотеки (лимит 3 000 000)
    RUB medical_year;       // потрачено на лечение с начала года (социальный вычет)
    RUB sport_year;         // потрачено на спорт с начала года (социальный вычет)
    RUB social_limit;       // лимит всех социальных вычетов за год (150 000)
    RUB amount;             // сколько вернут по последней декларации
    RUB total;              // сколько вернули за все время симуляции
};

struct Ndfl {               // все налоги человека
    int withhold_percent;   // сколько % удерживает работодатель с каждой выплаты

    // уведомление ФНС о доплате НДФЛ, раз в год
    RUB income;             // доход за прошлый год по всем работам
    RUB withheld;           // сколько НДФЛ удержали все работодатели
    RUB extra;              // доплата по уведомлению за прошлый год
    RUB extra_total;        // сколько доплачено за все время симуляции

    // имущественные налоги и НПД
    RUB car_per_hp;         // транспортный налог за 1 л.с.
    int property_permille;  // налог на квартиру в промилле
    int rent_percent;       // НПД самозанятого со сдачи жилья физлицам, %

    Tax_refund refund;  // налоговые вычеты
};

// Расходы

struct Life {
    RUB mobile;         // связь и интернет в месяц
    RUB subscriptions;  // онлайн-кинотеатр, музыка, облако в месяц
    RUB transport;      // проезд в месяц, пока нет машины
    RUB fun;            // кафе, кино, бары в месяц
    RUB gym;            // абонемент в зал в месяц
    RUB haircut;        // парикмахерская или салон
    RUB clothes;        // одежда два раза в год: весна и осень
    RUB gifts;          // подарки на дни рождения и Новый год
    RUB vacation;       // отпуск раз в год
};

struct Health {
    RUB checkup;        // обследование раз в год
    RUB dentist;        // стоматолог раз в год
    RUB illness;        // лекарства при болезни, два раза в год
};

struct Pet {
    const char* name;       // кличка
    bool is_alive;          // живет ли питомец у хозяина (false - еще не завели или уже умер)
    int age_months;         // возраст в месяцах
    int lifespan_years;     // сколько лет проживет
    int senior_age_years;   // с какого возраста ветеринар обходится вдвое дороже
    RUB price;              // заведение: покупка, лежанка, переноска, миски
    RUB food;               // корм в месяц
    RUB litter;             // наполнитель для лотка в месяц (у собаки 0)
    RUB vet;                // плановый осмотр у ветеринара раз в год
    RUB vaccination;        // прививки раз в год
    RUB parasites;          // обработка от клещей и блох
    RUB grooming;           // стрижка и мытье раз в квартал (у кошки 0)
    RUB toys;               // игрушки и амуниция два раза в год
    RUB pet_sitter;         // передержка на время отпуска хозяина
};

// Имущество

struct Car {
    bool is_bought;     // куплена ли машина
    RUB price;          // цена машины
    RUB down_payment;   // первоначальный взнос
    RUB fuel;           // бензин в месяц
    RUB maintenance;    // ТО раз в год
    RUB repair;         // ремонт раз в 3 года
    int horse_power;    // нужна для транспортного налога

    // трейд-ин: старую машину сдаем дилеру в зачет новой
    int bought_year;        // когда купили текущую машину
    int trade_in_years;     // через сколько лет менять
    int trade_in_percent;   // сколько % от цены дилер дает за старую
    RUB next_price;         // цена следующей машины
    int class_up_percent;   // на сколько % каждая следующая машина дороже (класс выше)
};

struct Upgrade {        // крупная трата, когда накопилось достаточно денег
    RUB price;          // стоимость
    int every_years;    // не чаще, чем раз в определенное количество лет
    int last_year;      // когда было в прошлый раз
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

// Банк

struct Credit {         // потребительский кредит или автокредит
    RUB debt;           // остаток долга
    RUB payment;        // ежемесячный платеж
    int rate;           // % годовых
    int months;         // срок кредита
};

struct Mortgage {       // ипотека под залог квартиры
    RUB debt;           // остаток долга
    RUB payment;        // ежемесячный платеж
    int rate;           // % годовых
    int years;          // срок ипотеки
    RUB interest_year;  // уплачено процентов с начала года, для вычета по процентам
    RUB interest_paid;  // уплачено процентов за все время
};

struct Deposit {
    Currency currency;  // курс берем у государства
    int amount;         // сумма в валюте вклада (рубли, евро или юани)
    int rate;           // % годовых
};

struct Bank {
    RUB account;        // текущий счет: сюда приходят доходы, отсюда идут все траты
    RUB reserve;        // подушка:  держим на счету, остальное на рублевый вклад
    Deposit deposit_rub;
    Deposit deposit_eur;
    Deposit deposit_cny;
    Credit car_credit;
    Mortgage mortgage;
    Mortgage second_mortgage;
};

// Человек

struct Person {
    const char* name;   // имя для вывода событий
    Job main_job;
    Job side_job;
    Ndfl ndfl;
    RUB food;
    Life life;
    Health health;
    Pet pet;
    Car car;
    Upgrade appliances;     // обновление бытовой техники
    Upgrade renovation;     // капитальный ремонт квартиры
    Home home;
    Flat flat;
    Flat second_flat;
    Bank bank;
};

struct Government government;
struct Person den;       
struct Person hanna; 

// Общие функции

RUB add_percent(const RUB value, const int percent) {
    return value * (100 + percent) / 100;
}

RUB min_rub(const RUB a, const RUB b) {
    if (a < b)
    {
        return a;
    }

    return b;
}

// Государство

double currency_rate(const Currency currency) { // рублей за 1 ед.
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
    if (month != 1) // раз в год
    {
        return;
    }

    exchange_rate_growth(government.eur);
    exchange_rate_growth(government.cny);
}

// НДФЛ

RUB ndfl(const RUB income) {
    if (income <= 2'400'000) // 13%
    {
        return income * 13 / 100;
    }

    RUB tax = 2'400'000 * 13 / 100; // 312 000

    if (income <= 5'000'000) // 15% свыше 2,4 млн
    {
        return tax + (income - 2'400'000) * 15 / 100;
    }

    tax += (5'000'000 - 2'400'000) * 15 / 100; // 390 000

    if (income <= 20'000'000) // 18% свыше 5 млн
    {
        return tax + (income - 5'000'000) * 18 / 100;
    }

    tax += (20'000'000 - 5'000'000) * 18 / 100; // 2 700 000

    return tax + (income - 20'000'000) * 20 / 100; // 20%
}

RUB job_income(Job& job, const RUB gross, const int withhold_percent) {
    RUB tax = gross * withhold_percent / 100;

    job.ndfl.income_year += gross;
    job.ndfl.withheld_year += tax;

    return gross - tax;
}

// Вычеты уменьшают налоговую базу, потом налог по шкале считается заново

RUB property_deduction(Tax_refund& refund, const Flat& flat, const RUB income) {
    if (not flat.is_bought) // только после покупки
    {
        return 0;
    }

    RUB deduction = min_rub(refund.property_left, income); // не больше дохода
    refund.property_left -= deduction;
    return deduction;
}

RUB interest_deduction(Tax_refund& refund, Mortgage& mortgage) {
    RUB deduction = min_rub(refund.interest_left, mortgage.interest_year);
    refund.interest_left -= deduction;
    mortgage.interest_year = 0;
    return deduction;
}

RUB social_deduction(Tax_refund& refund) {
    RUB deduction = min_rub(refund.medical_year + refund.sport_year, refund.social_limit);
    refund.medical_year = 0;
    refund.sport_year = 0;
    return deduction;
}

RUB tax_base(const RUB income, const RUB deductions) {
    if (deductions >= income) // база не может быть меньше нуля
    {
        return 0;
    }

    return income - deductions;
}

int ndfl_step(const RUB income) { // верхняя ступень шкалы, %
    if (income <= 2'400'000)
    {
        return 13;
    }

    if (income <= 5'000'000)
    {
        return 15;
    }

    if (income <= 20'000'000)
    {
        return 18;
    }

    return 20;
}

RUB ndfl_refund(const RUB tax_paid, const RUB base) {
    RUB overpaid = tax_paid - ndfl(base); // уплачено налогов - налог с базы после вычетов
    if (overpaid < 0)
    {
        return 0;
    }

    return overpaid;
}

// Банк

RUB month_interest(const RUB debt, const int rate) {
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
    if (credit.debt <= 0)
    {
        return;
    }

    RUB interest = month_interest(credit.debt, credit.rate);
    RUB payment = month_payment(credit.debt, interest, credit.payment);

    credit.debt += interest - payment;
    bank.account -= payment;
}

void bank_mortgage(Bank& bank, Mortgage& mortgage) {
    if (mortgage.debt <= 0)
    {
        return;
    }

    RUB interest = month_interest(mortgage.debt, mortgage.rate);
    RUB payment = month_payment(mortgage.debt, interest, mortgage.payment);

    mortgage.debt += interest - payment;
    mortgage.interest_year += interest;
    mortgage.interest_paid += interest;
    bank.account -= payment;
}

void bank_deposit(Deposit& deposit) {
    // капитализация
    deposit.amount += deposit.amount * deposit.rate / 100 / 12;
}

RUB bank_savings(const Bank& bank) { // счет + рублевый вклад
    return bank.account + bank.deposit_rub.amount;
}

void bank_pay(Bank& bank, const RUB amount) {
    bank.account -= amount;
    if (bank.account >= 0)
    {
        return;
    }

    bank.deposit_rub.amount += bank.account; // не хватило на счете - снимаем с вклада
    bank.account = 0;
}

void bank_save(Bank& bank) { // излишек сверх подушки - на вклад
    if (bank.account <= bank.reserve)
    {
        return;
    }

    bank.deposit_rub.amount += bank.account - bank.reserve;
    bank.account = bank.reserve;
}

bool upgrade_is_due(const Upgrade& upgrade, const Bank& bank, const int year) {
    if (year - upgrade.last_year < upgrade.every_years) // рано
    {
        return false;
    }

    return bank_savings(bank) - upgrade.price >= bank.reserve; // подушка должна остаться
}

RUB deposit_in_rub(const Deposit& deposit) {
    return (RUB)(deposit.amount * currency_rate(deposit.currency));
}

// Общее для всех людей

void ndfl_extra_payment(Person& person, const int year, const int month) {
    if (month != 11) // до 1 декабря
    {
        return;
    }

    if (person.ndfl.extra <= 0)
    {
        return;
    }

    person.bank.account -= person.ndfl.extra;
    person.ndfl.extra_total += person.ndfl.extra;
    printf("%d.%02d: %s paid extra ndfl = %d\n", year, month, person.name, person.ndfl.extra);
}

void tax_refund_payment(Person& person, const int year, const int month) {
    if (month != 5) // деньги приходят в мае
    {
        return;
    }

    if (person.ndfl.refund.amount <= 0)
    {
        return;
    }

    person.bank.account += person.ndfl.refund.amount;
    person.ndfl.refund.total += person.ndfl.refund.amount;
    printf("%d.%02d: %s got tax refund = %d\n", year, month, person.name, person.ndfl.refund.amount);
    person.ndfl.refund.amount = 0;
}

void pet_age(Person& person, const char* kind, const int year, const int month) {
    if (not person.pet.is_alive)
    {
        return;
    }

    ++person.pet.age_months;

    if (person.pet.age_months < person.pet.lifespan_years * 12)
    {
        return;
    }

    person.pet.is_alive = false;
    printf("%d.%02d: %s's %s %s died at the age of %d\n",
           year, month, person.name, kind, person.pet.name, person.pet.lifespan_years);
}

// техника или ремонт в своей квартире, когда накопилось
void home_upgrade(Person& person, Upgrade& upgrade, const int inflation, const int buy_month,
                  const char* what, const int year, const int month) {
    if (month == 1)
    {
        upgrade.price = add_percent(upgrade.price, inflation);
    }

    if (month != buy_month or not person.flat.is_bought)
    {
        return;
    }

    if (not upgrade_is_due(upgrade, person.bank, year))
    {
        return;
    }

    bank_pay(person.bank, upgrade.price);
    upgrade.last_year = year;
    printf("%d.%02d: %s %s for %d\n", year, month, person.name, what, upgrade.price);
}

void car_trade_in(Person& person, const int trade_month, const int year, const int month) {
    if (month == 1)
    {
        person.car.next_price = add_percent(person.car.next_price, government.inflation.car);
    }

    if (month != trade_month or not person.car.is_bought)
    {
        return;
    }

    if (year - person.car.bought_year < person.car.trade_in_years or person.bank.car_credit.debt > 0) // рано или кредит не закрыт
    {
        return;
    }

    RUB old_car = person.car.price * person.car.trade_in_percent / 100; // столько дает дилер
    RUB surcharge = person.car.next_price - old_car;

    if (bank_savings(person.bank) - surcharge < person.bank.reserve) // не хватает, ездим дальше
    {
        return;
    }

    bank_pay(person.bank, surcharge);
    person.car.price = person.car.next_price;
    person.car.next_price = add_percent(person.car.next_price, person.car.class_up_percent);
    person.car.bought_year = year;
    printf("%d.%02d: %s traded in the car: new one for %d, surcharge %d\n",
           year, month, person.name, person.car.price, surcharge);
}

// Ден

// Доходы

void den_main_job(const int year, const int month) {
    if (not den.main_job.is_working)
    {
        return;
    }

    if (month == 1) // индексация
    {
        den.main_job.salary = add_percent(den.main_job.salary, den.main_job.indexation);
        den.main_job.bonus = add_percent(den.main_job.bonus, den.main_job.indexation);
    }

    if (year == 2030 and month == 3) // повышение
    {
        den.main_job.salary += 30'000;
        printf("%d.%02d: Den got a promotion\n", year, month);
    }

    den.bank.account += job_income(den.main_job, den.main_job.salary, den.ndfl.withhold_percent);
}

void den_side_job(const int year, const int month) {
    if (year == 2027 and month == 3) // нашел подработку
    {
        den.side_job.is_working = true;
        printf("%d.%02d: Den started a second job\n", year, month);
    }

    if (not den.side_job.is_working)
    {
        return;
    }

    if (month == 1)
    {
        den.side_job.salary = add_percent(den.side_job.salary, den.side_job.indexation);
    }

    den.bank.account += job_income(den.side_job, den.side_job.salary, den.ndfl.withhold_percent);
}

void den_bonus(const int year, const int month) {
    if (month != 12) // в декабре
    {
        return;
    }

    if (den.main_job.is_working and den.main_job.bonus > 0)
    {
        den.bank.account += job_income(den.main_job, den.main_job.bonus, den.ndfl.withhold_percent);
        printf("%d.%02d: Den got an annual bonus\n", year, month);
    }

    if (den.side_job.is_working and den.side_job.bonus > 0) // на подработке нет
    {
        den.bank.account += job_income(den.side_job, den.side_job.bonus, den.ndfl.withhold_percent);
    }
}

void den_rent_income(const int year, const int month) {
    if (not den.second_flat.is_bought)
    {
        return;
    }

    if (month == 8) // поднимаем плату жильцам
    {
        den.second_flat.rent_income = add_percent(den.second_flat.rent_income, government.inflation.rent);
    }

    // НПД в базу НДФЛ не входит
    RUB tax = den.second_flat.rent_income * den.ndfl.rent_percent / 100;
    den.bank.account += den.second_flat.rent_income - tax;
}

// Перерасчет НДФЛ

void den_ndfl_year_close(const int year, const int month) {
    if (month != 1) // в январе
    {
        return;
    }

    // шкала к сумме по всем работам
    den.ndfl.income = den.main_job.ndfl.income_year + den.side_job.ndfl.income_year;
    den.ndfl.withheld = den.main_job.ndfl.withheld_year + den.side_job.ndfl.withheld_year;
    den.ndfl.extra = ndfl(den.ndfl.income) - den.ndfl.withheld;
    if (den.ndfl.extra < 300) // меньше 300 руб. уведомление не присылают
    {
        den.ndfl.extra = 0;
    }

    if (den.ndfl.extra > 0)
    {
        printf("%d.%02d: tax notice for %d: income %d, withheld %d, to pay %d\n",
               year, month, year - 1, den.ndfl.income, den.ndfl.withheld, den.ndfl.extra);
    }

    // новый год
    den.main_job.ndfl = {.income_year = 0, .withheld_year = 0};
    den.side_job.ndfl = {.income_year = 0, .withheld_year = 0};
}

// Возврат НДФЛ

void den_tax_refund_declaration(const int year, const int month) {
    if (month != 1) // в январе, после перерасчета
    {
        return;
    }

    RUB income = den.ndfl.income;
    RUB tax_paid = den.ndfl.withheld + den.ndfl.extra;

    RUB deductions = 0;
    deductions += property_deduction(den.ndfl.refund, den.flat, income);
    deductions += interest_deduction(den.ndfl.refund, den.bank.mortgage);
    deductions += social_deduction(den.ndfl.refund);

    // по второй квартире вычета нет
    den.bank.second_mortgage.interest_year = 0;

    RUB base = tax_base(income, deductions);
    den.ndfl.refund.amount = ndfl_refund(tax_paid, base);

    if (den.ndfl.refund.amount > 0)
    {
        printf("%d.%02d: Den filed 3-NDFL for %d: base %d -> %d, refund %d\n",
               year, month, year - 1, income, base, den.ndfl.refund.amount);
    }

    if (ndfl_step(base) < ndfl_step(income)) // вычет опустил базу на ступень ниже
    {
        printf("%d.%02d: Den's tax step for %d fell from %d%% to %d%%\n",
               year, month, year - 1, ndfl_step(income), ndfl_step(base));
    }
}

// Еда и жилье

void den_food(const int year, const int month) {
    if (month == 1)
    {
        den.food = add_percent(den.food, government.inflation.food);
    }

    den.bank.account -= den.food;
}

void den_rent(const int year, const int month) {
    if (month == 9)
    {
        den.home.rent = add_percent(den.home.rent, government.inflation.rent);
    }

    if (not den.flat.is_bought) // пока нет своей
    {
        den.bank.account -= den.home.rent;
    }
}

void den_home(const int year, const int month) {
    if (month == 7)
    {
        den.home.bills = add_percent(den.home.bills, government.inflation.bills);
    }

    if (month == 2)
    {
        den.home.goods = add_percent(den.home.goods, government.inflation.home_goods);
    }

    if (month == 6 and den.flat.is_bought) // ремонт в своей квартире
    {
        den.bank.account -= den.home.repair;
    }

    den.bank.account -= den.home.bills;
    den.bank.account -= den.home.goods;
}

// Бытовые расходы

void den_everyday(const int year, const int month) { // связь, подписки, развлечения
    if (month == 2) // оператор поднимает тариф
    {
        den.life.mobile = add_percent(den.life.mobile, government.inflation.mobile);
    }

    if (month == 1)
    {
        den.life.subscriptions = add_percent(den.life.subscriptions, government.inflation.subscriptions);
        den.life.fun = add_percent(den.life.fun, government.inflation.fun);
    }

    den.bank.account -= den.life.mobile + den.life.subscriptions + den.life.fun;
}

void den_transport(const int year, const int month) {
    if (month == 1)
    {
        den.life.transport = add_percent(den.life.transport, government.inflation.transport);
    }

    if (den.car.is_bought) // на машине
    {
        return;
    }

    den.bank.account -= den.life.transport;
}

void den_gym(const int year, const int month) {
    if (month == 9) // к сезону
    {
        den.life.gym = add_percent(den.life.gym, government.inflation.gym);
    }

    den.bank.account -= den.life.gym;
    den.ndfl.refund.sport_year += den.life.gym; // соц. вычет
}

void den_haircut(const int year, const int month) {
    if (month == 5)
    {
        den.life.haircut = add_percent(den.life.haircut, government.inflation.haircut);
    }

    if (month % 2 == 1) // раз в два месяца
    {
        den.bank.account -= den.life.haircut;
    }
}

void den_clothes(const int year, const int month) {
    if (month == 3)
    {
        den.life.clothes = add_percent(den.life.clothes, government.inflation.clothes);
    }

    if (month == 4 or month == 10) // весна и осень
    {
        den.bank.account -= den.life.clothes;
    }
}

void den_gifts(const int year, const int month) {
    if (month == 1)
    {
        den.life.gifts = add_percent(den.life.gifts, government.inflation.gifts);
    }

    if (month == 3 or month == 6 or month == 9) // дни рождения
    {
        den.bank.account -= den.life.gifts;
    }

    if (month == 12) // Новый год
    {
        den.bank.account -= den.life.gifts * 2;
    }
}

void den_vacation(const int year, const int month) {
    if (month == 2)
    {
        den.life.vacation = add_percent(den.life.vacation, government.inflation.vacation);
    }

    if (month == 7) // отпуск
    {
        den.bank.account -= den.life.vacation;
    }
}

// Здоровье (идет в вычет)

void den_health(const int year, const int month) {
    if (month == 1)
    {
        den.health.checkup = add_percent(den.health.checkup, government.inflation.medicine);
        den.health.dentist = add_percent(den.health.dentist, government.inflation.medicine);
        den.health.illness = add_percent(den.health.illness, government.inflation.medicine);
    }

    if (month == 2) // обследование
    {
        den.bank.account -= den.health.checkup;
        den.ndfl.refund.medical_year += den.health.checkup;
    }

    if (month == 8) // стоматолог
    {
        den.bank.account -= den.health.dentist;
        den.ndfl.refund.medical_year += den.health.dentist;
    }

    if (month == 3 or month == 11) // простуда
    {
        den.bank.account -= den.health.illness;
        den.ndfl.refund.medical_year += den.health.illness;
    }
}

// Собака

void den_pet_adopt(const int year, const int month) {
    if (not (year == 2027 and month == 5)) // из приюта
    {
        return;
    }

    den.pet.is_alive = true;
    den.bank.account -= den.pet.price;
    printf("%d.%02d: Den adopted a dog %s, age %d\n", year, month, den.pet.name, den.pet.age_months / 12);
}

void den_pet_prices(const int year, const int month) {
    if (month != 1) // все для собаки дорожает раз в год
    {
        return;
    }

    den.pet.food = add_percent(den.pet.food, government.inflation.pet);
    den.pet.vet = add_percent(den.pet.vet, government.inflation.pet);
    den.pet.vaccination = add_percent(den.pet.vaccination, government.inflation.pet);
    den.pet.parasites = add_percent(den.pet.parasites, government.inflation.pet);
    den.pet.grooming = add_percent(den.pet.grooming, government.inflation.pet);
    den.pet.toys = add_percent(den.pet.toys, government.inflation.pet);
    den.pet.pet_sitter = add_percent(den.pet.pet_sitter, government.inflation.pet);
}

void den_pet_care(const int year, const int month) {
    if (not den.pet.is_alive)
    {
        return;
    }

    den.bank.account -= den.pet.food;

    if (month == 6) // осмотр, пожилой собаке вдвое дороже
    {
        RUB vet = den.pet.vet;
        if (den.pet.age_months >= den.pet.senior_age_years * 12)
        {
            vet *= 2;
        }
        den.bank.account -= vet;
    }

    if (month == 4) // прививки
    {
        den.bank.account -= den.pet.vaccination;
    }

    if (month >= 4 and month <= 9) // сезон клещей
    {
        den.bank.account -= den.pet.parasites;
    }

    if (month % 3 == 0) // груминг раз в квартал
    {
        den.bank.account -= den.pet.grooming;
    }

    if (month == 5 or month == 11) // игрушки, амуниция
    {
        den.bank.account -= den.pet.toys;
    }

    if (month == 7) // передержка, пока Ден в отпуске
    {
        den.bank.account -= den.pet.pet_sitter;
    }
}

void den_pet_surgery(const int year, const int month) {
    if (not den.pet.is_alive)
    {
        return;
    }

    if (year == 2031 and month == 2) // порвала связку
    {
        den.bank.account -= 70'000;
        printf("%d.%02d: Den's dog %s had a surgery\n", year, month, den.pet.name);
    }
}

// Машина

void den_car(const int year, const int month) {
    if (month == 1)
    {
        den.car.fuel = add_percent(den.car.fuel, government.inflation.fuel);
    }

    if (not den.car.is_bought)
    {
        return;
    }

    if (month == 4) // ТО
    {
        den.bank.account -= den.car.maintenance;
    }

    if (month == 10 and year % 3 == 0) // ремонт
    {
        den.bank.account -= den.car.repair;
    }

    den.bank.account -= den.car.fuel;
}

// Налоги на имущество

void den_taxes(const int year, const int month) {
    if (month == 11 and den.car.is_bought) // до 1 декабря
    {
        den.bank.account -= den.car.horse_power * den.ndfl.car_per_hp;
    }

    if (month == 11 and den.flat.is_bought)
    {
        den.bank.account -= den.flat.price / 1000 * den.ndfl.property_permille;
    }

    if (month == 11 and den.second_flat.is_bought)
    {
        den.bank.account -= den.second_flat.price / 1000 * den.ndfl.property_permille;
    }
}

// Покупки

void den_buy_car(const int year, const int month) {
    if (den.car.is_bought or den.bank.account < den.car.down_payment) // есть или не накопил
    {
        return;
    }

    den.bank.account -= den.car.down_payment;
    den.bank.car_credit.debt = den.car.price - den.car.down_payment;
    den.car.is_bought = true;
    den.car.bought_year = year;
    printf("%d.%02d: Den bought a car\n", year, month);
}

void den_buy_flat(const int year, const int month) {
    if (den.flat.is_bought or den.bank.account < den.flat.down_payment) // есть или не накопил
    {
        return;
    }

    den.bank.account -= den.flat.down_payment;
    den.bank.mortgage.debt = den.flat.price - den.flat.down_payment;
    den.flat.is_bought = true;
    den.renovation.last_year = year; // при покупке ремонт свежий
    printf("%d.%02d: Den bought a flat\n", year, month);
}

void den_buy_second_flat(const int year, const int month) {
    if (not den.flat.is_bought) // сначала своя
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

// Отчет

void den_year_report(const int year, const int month) {
    if (month != 12)
    {
        return;
    }

    RUB income = den.main_job.ndfl.income_year + den.side_job.ndfl.income_year;
    RUB withheld = den.main_job.ndfl.withheld_year + den.side_job.ndfl.withheld_year;

    printf("%d: Den gross = %d, ndfl = %d, account = %d, deposit = %d\n",
           year, income, withheld, den.bank.account, den.bank.deposit_rub.amount);
}

// Ден за месяц

void den_month(const int year, const int month) {
    // налоги за прошлый год
    den_ndfl_year_close(year, month);
    den_tax_refund_declaration(year, month);

    // доходы
    den_main_job(year, month);
    den_side_job(year, month);
    den_bonus(year, month);
    den_rent_income(year, month);
    tax_refund_payment(den, year, month);

    // еда и жилье
    den_food(year, month);
    den_rent(year, month);
    den_home(year, month);

    // бытовые расходы
    den_everyday(year, month);
    den_transport(year, month);
    den_gym(year, month);
    den_haircut(year, month);
    den_clothes(year, month);
    den_gifts(year, month);
    den_vacation(year, month);

    // здоровье
    den_health(year, month);

    // собака
    pet_age(den, "dog", year, month); // сначала возраст, потом adopt
    den_pet_adopt(year, month);
    den_pet_prices(year, month);
    den_pet_care(year, month);
    den_pet_surgery(year, month);

    // машина
    den_car(year, month);

    // банк
    bank_credit(den.bank, den.bank.car_credit);
    bank_mortgage(den.bank, den.bank.mortgage);
    bank_mortgage(den.bank, den.bank.second_mortgage);
    bank_deposit(den.bank.deposit_rub);
    bank_deposit(den.bank.deposit_eur);
    bank_deposit(den.bank.deposit_cny);

    // налоги
    den_taxes(year, month);
    ndfl_extra_payment(den, year, month);

    // покупки в конце месяца, когда все траты уже учтены
    den_buy_car(year, month);
    den_buy_flat(year, month);
    den_buy_second_flat(year, month);

    // крупные траты и излишек на вклад
    home_upgrade(den, den.appliances, government.inflation.appliances, 11, "updated home appliances", year, month);
    home_upgrade(den, den.renovation, government.inflation.renovation, 4, "renovated the flat", year, month);
    car_trade_in(den, 3, year, month);

    if (month == 12 and den.second_flat.is_bought) // пока копит на квартиры, все на счету
    {
        bank_save(den.bank);
    }

    den_year_report(year, month);
}

// Ханна (попроще: одна работа, без валюты и второй квартиры)

// Доходы

void hanna_main_job(const int year, const int month) {
    if (not hanna.main_job.is_working)
    {
        return;
    }

    if (month == 1) // индексация
    {
        hanna.main_job.salary = add_percent(hanna.main_job.salary, hanna.main_job.indexation);
        hanna.main_job.bonus = add_percent(hanna.main_job.bonus, hanna.main_job.indexation);
    }

    if (year == 2029 and month == 9) // повышение
    {
        hanna.main_job.salary += 20'000;
        printf("%d.%02d: Hanna got a promotion\n", year, month);
    }

    hanna.bank.account += job_income(hanna.main_job, hanna.main_job.salary, hanna.ndfl.withhold_percent);
}

void hanna_bonus(const int year, const int month) {
    if (month % 3 != 0) // раз в квартал
    {
        return;
    }

    if (hanna.main_job.is_working and hanna.main_job.bonus > 0)
    {
        hanna.bank.account += job_income(hanna.main_job, hanna.main_job.bonus, hanna.ndfl.withhold_percent);
    }
}

// Перерасчет НДФЛ

void hanna_ndfl_year_close(const int year, const int month) {
    if (month != 1) // в январе
    {
        return;
    }

    hanna.ndfl.income = hanna.main_job.ndfl.income_year;
    hanna.ndfl.withheld = hanna.main_job.ndfl.withheld_year;
    hanna.ndfl.extra = ndfl(hanna.ndfl.income) - hanna.ndfl.withheld; // после 2,4 млн уже 15%
    if (hanna.ndfl.extra < 300) // меньше 300 руб. уведомление не присылают
    {
        hanna.ndfl.extra = 0;
    }

    if (hanna.ndfl.extra > 0)
    {
        printf("%d.%02d: Hanna's tax notice for %d: income %d, withheld %d, to pay %d\n",
               year, month, year - 1, hanna.ndfl.income, hanna.ndfl.withheld, hanna.ndfl.extra);
    }

    // новый год
    hanna.main_job.ndfl = {.income_year = 0, .withheld_year = 0};
}

// Возврат НДФЛ

void hanna_tax_refund_declaration(const int year, const int month) {
    if (month != 1) // в январе
    {
        return;
    }

    RUB income = hanna.ndfl.income;
    RUB tax_paid = hanna.ndfl.withheld + hanna.ndfl.extra;

    RUB deductions = 0;
    deductions += property_deduction(hanna.ndfl.refund, hanna.flat, income);
    deductions += interest_deduction(hanna.ndfl.refund, hanna.bank.mortgage);
    deductions += social_deduction(hanna.ndfl.refund);

    RUB base = tax_base(income, deductions);
    hanna.ndfl.refund.amount = ndfl_refund(tax_paid, base);

    if (hanna.ndfl.refund.amount > 0)
    {
        printf("%d.%02d: Hanna filed 3-NDFL for %d: base %d -> %d, refund %d\n",
               year, month, year - 1, income, base, hanna.ndfl.refund.amount);
    }

    if (ndfl_step(base) < ndfl_step(income)) // вычет опустил на ступень ниже
    {
        printf("%d.%02d: Hanna's tax step for %d fell from %d%% to %d%%\n",
               year, month, year - 1, ndfl_step(income), ndfl_step(base));
    }
}

// Еда и жилье

void hanna_food(const int year, const int month) {
    if (month == 1)
    {
        hanna.food = add_percent(hanna.food, government.inflation.food);
    }

    hanna.bank.account -= hanna.food;
}

void hanna_rent(const int year, const int month) {
    if (month == 3) // договор продлевается весной
    {
        hanna.home.rent = add_percent(hanna.home.rent, government.inflation.rent);
    }

    if (not hanna.flat.is_bought) // пока нет своей
    {
        hanna.bank.account -= hanna.home.rent;
    }
}

void hanna_home(const int year, const int month) {
    if (month == 7)
    {
        hanna.home.bills = add_percent(hanna.home.bills, government.inflation.bills);
    }

    if (month == 2)
    {
        hanna.home.goods = add_percent(hanna.home.goods, government.inflation.home_goods);
    }

    if (month == 5 and hanna.flat.is_bought) // ремонт в своей квартире
    {
        hanna.bank.account -= hanna.home.repair;
    }

    hanna.bank.account -= hanna.home.bills;
    hanna.bank.account -= hanna.home.goods;
}

// Бытовые расходы

void hanna_everyday(const int year, const int month) { // связь, подписки, развлечения
    if (month == 2) // оператор поднимает тариф
    {
        hanna.life.mobile = add_percent(hanna.life.mobile, government.inflation.mobile);
    }

    if (month == 1)
    {
        hanna.life.subscriptions = add_percent(hanna.life.subscriptions, government.inflation.subscriptions);
        hanna.life.fun = add_percent(hanna.life.fun, government.inflation.fun);
    }

    hanna.bank.account -= hanna.life.mobile + hanna.life.subscriptions + hanna.life.fun;
}

void hanna_transport(const int year, const int month) {
    if (month == 1)
    {
        hanna.life.transport = add_percent(hanna.life.transport, government.inflation.transport);
    }

    if (hanna.car.is_bought) // на машине
    {
        return;
    }

    hanna.bank.account -= hanna.life.transport;
}

void hanna_gym(const int year, const int month) {
    if (month == 1) // йога
    {
        hanna.life.gym = add_percent(hanna.life.gym, government.inflation.gym);
    }

    hanna.bank.account -= hanna.life.gym;
    hanna.ndfl.refund.sport_year += hanna.life.gym; // соц. вычет
}

void hanna_haircut(const int year, const int month) {
    if (month == 5)
    {
        hanna.life.haircut = add_percent(hanna.life.haircut, government.inflation.haircut);
    }

    hanna.bank.account -= hanna.life.haircut; // каждый месяц
}

void hanna_clothes(const int year, const int month) {
    if (month == 3)
    {
        hanna.life.clothes = add_percent(hanna.life.clothes, government.inflation.clothes);
    }

    if (month == 4 or month == 10) // весна и осень
    {
        hanna.bank.account -= hanna.life.clothes;
    }
}

void hanna_gifts(const int year, const int month) {
    if (month == 1)
    {
        hanna.life.gifts = add_percent(hanna.life.gifts, government.inflation.gifts);
    }

    if (month == 2 or month == 5 or month == 11) // дни рождения
    {
        hanna.bank.account -= hanna.life.gifts;
    }

    if (month == 12) // Новый год
    {
        hanna.bank.account -= hanna.life.gifts * 2;
    }
}

void hanna_vacation(const int year, const int month) {
    if (month == 2)
    {
        hanna.life.vacation = add_percent(hanna.life.vacation, government.inflation.vacation);
    }

    if (month == 8) // отпуск
    {
        hanna.bank.account -= hanna.life.vacation;
    }
}

// Здоровье (идет в вычет)

void hanna_health(const int year, const int month) {
    if (month == 1)
    {
        hanna.health.checkup = add_percent(hanna.health.checkup, government.inflation.medicine);
        hanna.health.dentist = add_percent(hanna.health.dentist, government.inflation.medicine);
        hanna.health.illness = add_percent(hanna.health.illness, government.inflation.medicine);
    }

    if (month == 3) // обследование
    {
        hanna.bank.account -= hanna.health.checkup;
        hanna.ndfl.refund.medical_year += hanna.health.checkup;
    }

    if (month == 10) // стоматолог
    {
        hanna.bank.account -= hanna.health.dentist;
        hanna.ndfl.refund.medical_year += hanna.health.dentist;
    }

    if (month == 2 or month == 12) // простуда
    {
        hanna.bank.account -= hanna.health.illness;
        hanna.ndfl.refund.medical_year += hanna.health.illness;
    }
}

// Кошка

void hanna_pet_adopt(const int year, const int month) {
    if (not (year == 2026 and month == 10)) // котенок от заводчика
    {
        return;
    }

    hanna.pet.is_alive = true;
    hanna.bank.account -= hanna.pet.price;
    printf("%d.%02d: Hanna got a kitten %s\n", year, month, hanna.pet.name);
}

void hanna_pet_prices(const int year, const int month) {
    if (month != 1) // все для кошки дорожает раз в год
    {
        return;
    }

    hanna.pet.food = add_percent(hanna.pet.food, government.inflation.pet);
    hanna.pet.litter = add_percent(hanna.pet.litter, government.inflation.pet);
    hanna.pet.vet = add_percent(hanna.pet.vet, government.inflation.pet);
    hanna.pet.vaccination = add_percent(hanna.pet.vaccination, government.inflation.pet);
    hanna.pet.parasites = add_percent(hanna.pet.parasites, government.inflation.pet);
    hanna.pet.toys = add_percent(hanna.pet.toys, government.inflation.pet);
    hanna.pet.pet_sitter = add_percent(hanna.pet.pet_sitter, government.inflation.pet);
}

void hanna_pet_care(const int year, const int month) {
    if (not hanna.pet.is_alive)
    {
        return;
    }

    hanna.bank.account -= hanna.pet.food;
    hanna.bank.account -= hanna.pet.litter;

    if (month == 10) // осмотр, пожилой кошке вдвое дороже
    {
        RUB vet = hanna.pet.vet;
        if (hanna.pet.age_months >= hanna.pet.senior_age_years * 12)
        {
            vet *= 2;
        }
        hanna.bank.account -= vet;
    }

    if (month == 11) // прививка
    {
        hanna.bank.account -= hanna.pet.vaccination;
    }

    if (month == 5 or month == 8) // от блох два раза в год
    {
        hanna.bank.account -= hanna.pet.parasites;
    }

    if (month == 2 or month == 8) // когтеточка, игрушки
    {
        hanna.bank.account -= hanna.pet.toys;
    }

    if (month == 8) // ситтер, пока Ханна в отпуске
    {
        hanna.bank.account -= hanna.pet.pet_sitter;
    }
}

void hanna_pet_sterilization(const int year, const int month) {
    if (not hanna.pet.is_alive)
    {
        return;
    }

    if (hanna.pet.age_months == 7) // стерилизация
    {
        hanna.bank.account -= 9'000;
        printf("%d.%02d: Hanna's cat %s was sterilized\n", year, month, hanna.pet.name);
    }
}

// Машина

void hanna_car(const int year, const int month) {
    if (month == 1)
    {
        hanna.car.fuel = add_percent(hanna.car.fuel, government.inflation.fuel);
    }

    if (not hanna.car.is_bought)
    {
        return;
    }

    if (month == 9) // ТО
    {
        hanna.bank.account -= hanna.car.maintenance;
    }

    if (month == 5 and year % 3 == 1) // ремонт
    {
        hanna.bank.account -= hanna.car.repair;
    }

    hanna.bank.account -= hanna.car.fuel;
}

// Налоги на имущество

void hanna_taxes(const int year, const int month) {
    if (month == 11 and hanna.car.is_bought) // до 1 декабря
    {
        hanna.bank.account -= hanna.car.horse_power * hanna.ndfl.car_per_hp;
    }

    if (month == 11 and hanna.flat.is_bought)
    {
        hanna.bank.account -= hanna.flat.price / 1000 * hanna.ndfl.property_permille;
    }
}

// Покупки: сначала квартира, потом машина

void hanna_buy_flat(const int year, const int month) {
    if (hanna.flat.is_bought or hanna.bank.account < hanna.flat.down_payment) // есть или не накопила
    {
        return;
    }

    hanna.bank.account -= hanna.flat.down_payment;
    hanna.bank.mortgage.debt = hanna.flat.price - hanna.flat.down_payment;
    hanna.flat.is_bought = true;
    hanna.renovation.last_year = year;
    printf("%d.%02d: Hanna bought a flat\n", year, month);
}

void hanna_buy_car(const int year, const int month) {
    if (not hanna.flat.is_bought) // сначала квартира
    {
        return;
    }

    if (hanna.car.is_bought or hanna.bank.account < hanna.car.down_payment) // есть или не накопила
    {
        return;
    }

    hanna.bank.account -= hanna.car.down_payment;
    hanna.bank.car_credit.debt = hanna.car.price - hanna.car.down_payment;
    hanna.car.is_bought = true;
    hanna.car.bought_year = year;
    printf("%d.%02d: Hanna bought a car\n", year, month);
}

// Отчет

void hanna_year_report(const int year, const int month) {
    if (month != 12)
    {
        return;
    }

    printf("%d: Hanna gross = %d, ndfl = %d, account = %d, deposit = %d, mortgage debt = %d\n",
           year, hanna.main_job.ndfl.income_year, hanna.main_job.ndfl.withheld_year,
           hanna.bank.account, hanna.bank.deposit_rub.amount, hanna.bank.mortgage.debt);
}

// Ханна за месяц

void hanna_month(const int year, const int month) {
    // налоги за прошлый год
    hanna_ndfl_year_close(year, month);
    hanna_tax_refund_declaration(year, month);

    // доходы
    hanna_main_job(year, month);
    hanna_bonus(year, month);
    tax_refund_payment(hanna, year, month);

    // еда и жилье
    hanna_food(year, month);
    hanna_rent(year, month);
    hanna_home(year, month);

    // бытовые расходы
    hanna_everyday(year, month);
    hanna_transport(year, month);
    hanna_gym(year, month);
    hanna_haircut(year, month);
    hanna_clothes(year, month);
    hanna_gifts(year, month);
    hanna_vacation(year, month);

    // здоровье
    hanna_health(year, month);

    // кошка
    pet_age(hanna, "cat", year, month); // сначала возраст, потом adopt
    hanna_pet_adopt(year, month);
    hanna_pet_prices(year, month);
    hanna_pet_care(year, month);
    hanna_pet_sterilization(year, month);

    // машина
    hanna_car(year, month);

    // банк
    bank_credit(hanna.bank, hanna.bank.car_credit);
    bank_mortgage(hanna.bank, hanna.bank.mortgage);
    bank_deposit(hanna.bank.deposit_rub);

    // налоги
    hanna_taxes(year, month);
    ndfl_extra_payment(hanna, year, month);

    // покупки в конце месяца
    hanna_buy_flat(year, month);
    hanna_buy_car(year, month);

    // крупные траты и излишек на вклад
    home_upgrade(hanna, hanna.appliances, government.inflation.appliances, 9, "updated home appliances", year, month);
    home_upgrade(hanna, hanna.renovation, government.inflation.renovation, 5, "renovated the flat", year, month);
    car_trade_in(hanna, 4, year, month);

    if (month == 12 and hanna.car.is_bought) // пока копит на квартиру и машину, все на счету
    {
        bank_save(hanna.bank);
    }

    hanna_year_report(year, month);
}

// Симуляция

void simulation() {
    int year = 2026;
    int month = 9;

    while ( not ( year == 2038 and month == 1 ) ) // пока Ден не закроет обе ипотеки
    {
        government_currency(year, month);

        den_month(year, month);
        hanna_month(year, month);

        ++month;
        if (month == 13)
        {
            ++year;
            month = 1;
        }
    }
}

// Начальные данные

void government_init() {
    government.eur = {
        .rub    = 95.0,
        .growth = 5,
    };

    government.cny = {
        .rub    = 12.0,
        .growth = 4,
    };

    government.inflation = {
        .food          = 9,
        .rent          = 5,
        .bills         = 10,
        .home_goods    = 8,
        .mobile        = 5,
        .subscriptions = 12,
        .transport     = 8,
        .fun           = 9,
        .gym           = 10,
        .haircut       = 8,
        .clothes       = 7,
        .gifts         = 9,
        .vacation      = 12,
        .fuel          = 8,
        .medicine      = 10,
        .pet           = 9,
        .appliances    = 6,
        .car           = 5,
        .renovation    = 9,
    };
}

void den_init() {
    den.name = "Den";
    den.food = 25'000;

    // 13-я зарплата в декабре
    den.main_job = {
        .is_working = true,
        .salary     = 180'000,
        .bonus      = 180'000,
        .indexation = 7,
        .ndfl       = {
            .income_year   = 0,
            .withheld_year = 0,
        },
    };

    // с марта 2027
    den.side_job = {
        .is_working = false,
        .salary     = 70'000,
        .bonus      = 0,
        .indexation = 5,
        .ndfl       = {
            .income_year   = 0,
            .withheld_year = 0,
        },
    };

    den.ndfl = {
        .withhold_percent  = 13,

        .income            = 0,
        .withheld          = 0,
        .extra             = 0,
        .extra_total       = 0,

        .car_per_hp        = 35,
        .property_permille = 1,
        .rent_percent      = 4,

        .refund = {
            .property_left = 2'000'000,
            .interest_left = 3'000'000,
            .medical_year  = 0,
            .sport_year    = 0,
            .social_limit  = 150'000,
            .amount        = 0,
            .total         = 0,
        },
    };

    den.life = {
        .mobile        = 1'200,
        .subscriptions = 1'000,
        .transport     = 3'500,
        .fun           = 9'000,
        .gym           = 3'000,
        .haircut       = 1'500,     // раз в два месяца
        .clothes       = 15'000,
        .gifts         = 8'000,
        .vacation      = 90'000,
    };

    den.health = {
        .checkup = 15'000,
        .dentist = 12'000,
        .illness = 5'000,
    };

    // из приюта, в мае 2027
    den.pet = {
        .name             = "Rex",
        .is_alive         = false,
        .age_months       = 60,         // 5 лет
        .lifespan_years   = 12,
        .senior_age_years = 8,
        .price            = 15'000,     // приют, лежанка, поводок
        .food             = 6'000,
        .litter           = 0,
        .vet              = 5'000,
        .vaccination      = 4'000,
        .parasites        = 1'500,      // апрель-сентябрь
        .grooming         = 3'000,      // раз в квартал
        .toys             = 4'000,
        .pet_sitter       = 14'000,     // две недели
    };

    // 300к взнос + 400к кредит
    den.car = {
        .is_bought    = false,
        .price        = 700'000,
        .down_payment = 300'000,
        .fuel         = 8'000,
        .maintenance  = 25'000,
        .repair       = 40'000,
        .horse_power  = 106,

        .bought_year      = 0,
        .trade_in_years   = 4,
        .trade_in_percent = 60,
        .next_price       = 1'800'000,  // кроссовер
        .class_up_percent = 20,
    };

    // холодильник, стиралка, посудомойка, телевизор, пылесос
    den.appliances = {
        .price       = 350'000,
        .every_years = 4,
        .last_year   = 2025,
    };

    // через 6 лет после покупки квартиры
    den.renovation = {
        .price       = 1'200'000,
        .every_years = 6,
        .last_year   = 0,
    };

    den.home = {
        .rent   = 40'000,
        .bills  = 7'000,
        .goods  = 3'000,
        .repair = 50'000,
    };

    // 500к взнос + 2 млн ипотека
    den.flat = {
        .is_bought    = false,
        .price        = 2'500'000,
        .down_payment = 500'000,
        .rent_income  = 0,
    };

    // 500к взнос + 2,15 млн ипотека
    den.second_flat = {
        .is_bought    = false,
        .price        = 2'650'000,
        .down_payment = 500'000,
        .rent_income  = 50'000,
    };

    den.bank.account = 100'000;
    den.bank.reserve = 600'000;

    den.bank.deposit_rub = {
        .currency = Currency::rub,
        .amount   = 200'000,
        .rate     = 12,
    };

    den.bank.deposit_eur = {
        .currency = Currency::eur,
        .amount   = 1'000,
        .rate     = 0,
    };

    den.bank.deposit_cny = {
        .currency = Currency::cny,
        .amount   = 10'000,
        .rate     = 3,
    };

    // аннуитет
    den.bank.car_credit = {
        .debt    = 0,
        .payment = 14'063,
        .rate    = 16,
        .months  = 36,
    };

    den.bank.mortgage = {
        .debt          = 0,
        .payment       = 37'335,
        .rate          = 19,
        .years         = 10,
        .interest_year = 0,
        .interest_paid = 0,
    };

    den.bank.second_mortgage = {
        .debt          = 0,
        .payment       = 40'135,
        .rate          = 19,
        .years         = 10,
        .interest_year = 0,
        .interest_paid = 0,
    };
}

void hanna_init() { // чего у Ханны нет (подработка, вторая квартира, валюта) - остается нулями
    hanna.name = "Hanna";
    hanna.food = 20'000;

    // премия раз в квартал
    hanna.main_job = {
        .is_working = true,
        .salary     = 120'000,
        .bonus      = 25'000,
        .indexation = 6,
        .ndfl       = {
            .income_year   = 0,
            .withheld_year = 0,
        },
    };

    // аренды нет
    hanna.ndfl = {
        .withhold_percent  = 13,

        .income            = 0,
        .withheld          = 0,
        .extra             = 0,
        .extra_total       = 0,

        .car_per_hp        = 35,
        .property_permille = 1,
        .rent_percent      = 0,

        .refund = {
            .property_left = 2'000'000,
            .interest_left = 3'000'000,
            .medical_year  = 0,
            .sport_year    = 0,
            .social_limit  = 150'000,
            .amount        = 0,
            .total         = 0,
        },
    };

    hanna.life = {
        .mobile        = 900,
        .subscriptions = 800,
        .transport     = 3'000,
        .fun           = 7'000,
        .gym           = 4'500,     // йога
        .haircut       = 2'500,     // каждый месяц
        .clothes       = 20'000,
        .gifts         = 6'000,
        .vacation      = 70'000,
    };

    hanna.health = {
        .checkup = 12'000,
        .dentist = 10'000,
        .illness = 4'000,
    };

    // котенок, в октябре 2026
    hanna.pet = {
        .name             = "Bonya",
        .is_alive         = false,
        .age_months       = 3,
        .lifespan_years   = 15,
        .senior_age_years = 10,
        .price            = 40'000,     // котенок, лоток, переноска
        .food             = 3'000,
        .litter           = 1'200,
        .vet              = 3'500,
        .vaccination      = 3'000,
        .parasites        = 900,        // два раза в год
        .grooming         = 0,
        .toys             = 3'000,
        .pet_sitter       = 7'000,      // ситтер на время отпуска
    };

    // после квартиры: 300к взнос + 600к кредит
    hanna.car = {
        .is_bought    = false,
        .price        = 900'000,
        .down_payment = 300'000,
        .fuel         = 6'000,
        .maintenance  = 20'000,
        .repair       = 30'000,
        .horse_power  = 98,

        .bought_year      = 0,
        .trade_in_years   = 5,
        .trade_in_percent = 55,
        .next_price       = 900'000,    // тот же класс
        .class_up_percent = 10,
    };

    hanna.appliances = {
        .price       = 200'000,
        .every_years = 5,
        .last_year   = 2026,
    };

    // через 7 лет после покупки квартиры
    hanna.renovation = {
        .price       = 700'000,
        .every_years = 7,
        .last_year   = 0,
    };

    hanna.home = {
        .rent   = 35'000,
        .bills  = 6'000,
        .goods  = 2'500,
        .repair = 40'000,
    };

    // 440к взнос + 1,76 млн ипотека
    hanna.flat = {
        .is_bought    = false,
        .price        = 2'200'000,
        .down_payment = 440'000,
        .rent_income  = 0,
    };

    hanna.bank.account = 50'000;
    hanna.bank.reserve = 250'000;

    hanna.bank.deposit_rub = {
        .currency = Currency::rub,
        .amount   = 100'000,
        .rate     = 12,
    };

    // аннуитет
    hanna.bank.car_credit = {
        .debt    = 0,
        .payment = 21'094,
        .rate    = 16,
        .months  = 36,
    };

    hanna.bank.mortgage = {
        .debt          = 0,
        .payment       = 32'854,
        .rate          = 19,
        .years         = 10,
        .interest_year = 0,
        .interest_paid = 0,
    };

}

// Итог

void den_print() {
    RUB deposit_rub = deposit_in_rub(den.bank.deposit_rub);
    RUB deposit_eur = deposit_in_rub(den.bank.deposit_eur);
    RUB deposit_cny = deposit_in_rub(den.bank.deposit_cny);

    printf("\n=== Den ===\n");

    printf("Den main job salary = %d (gross)\n", den.main_job.salary);
    printf("Den side job salary = %d (gross)\n", den.side_job.salary);

    printf("Den extra ndfl paid total = %d\n", den.ndfl.extra_total);
    printf("Den tax refund total = %d\n", den.ndfl.refund.total);
    printf("Den property deduction left = %d\n", den.ndfl.refund.property_left);
    printf("Den interest deduction left = %d\n", den.ndfl.refund.interest_left);

    printf("Den bank account = %d\n", den.bank.account);
    printf("Den car credit debt = %d\n", den.bank.car_credit.debt);
    printf("Den mortgage debt = %d\n", den.bank.mortgage.debt);
    printf("Den second mortgage debt = %d\n", den.bank.second_mortgage.debt);
    printf("Den mortgage interest paid = %d\n", den.bank.mortgage.interest_paid + den.bank.second_mortgage.interest_paid);

    printf("Den deposit RUB = %d RUB\n", deposit_rub);
    printf("Den deposit EUR = %d RUB\n", deposit_eur);
    printf("Den deposit CNY = %d RUB\n", deposit_cny);
    printf("Den deposits total = %d RUB\n", deposit_rub + deposit_eur + deposit_cny);

    printf("Den car price = %d\n", den.car.price);
    printf("Den appliances last updated = %d\n", den.appliances.last_year);
    printf("Den last renovation (or flat purchase) = %d\n", den.renovation.last_year);

    if (den.pet.is_alive)
    {
        printf("Den's dog %s is alive, age %d\n", den.pet.name, den.pet.age_months / 12);
    }
    else
    {
        printf("Den's dog %s is no longer with him\n", den.pet.name);
    }
}

void hanna_print() {
    RUB deposit_rub = deposit_in_rub(hanna.bank.deposit_rub);

    printf("\n=== Hanna ===\n");

    printf("Hanna salary = %d (gross)\n", hanna.main_job.salary);
    printf("Hanna quarterly bonus = %d (gross)\n", hanna.main_job.bonus);

    printf("Hanna extra ndfl paid total = %d\n", hanna.ndfl.extra_total);
    printf("Hanna tax refund total = %d\n", hanna.ndfl.refund.total);
    printf("Hanna property deduction left = %d\n", hanna.ndfl.refund.property_left);
    printf("Hanna interest deduction left = %d\n", hanna.ndfl.refund.interest_left);

    printf("Hanna bank account = %d\n", hanna.bank.account);
    printf("Hanna car credit debt = %d\n", hanna.bank.car_credit.debt);
    printf("Hanna mortgage debt = %d\n", hanna.bank.mortgage.debt);
    printf("Hanna mortgage interest paid = %d\n", hanna.bank.mortgage.interest_paid);

    printf("Hanna deposit RUB = %d RUB\n", deposit_rub);

    printf("Hanna car price = %d\n", hanna.car.price);
    printf("Hanna appliances last updated = %d\n", hanna.appliances.last_year);
    printf("Hanna last renovation (or flat purchase) = %d\n", hanna.renovation.last_year);

    if (hanna.pet.is_alive)
    {
        printf("Hanna's cat %s is alive, age %d\n", hanna.pet.name, hanna.pet.age_months / 12);
    }
    else
    {
        printf("Hanna's cat %s is no longer with her\n", hanna.pet.name);
    }
}

int main() {
    government_init();
    den_init();
    hanna_init();

    simulation();

    den_print();
    hanna_print();
}
