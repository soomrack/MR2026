#include <stdio.h>
#include <stdlib.h>

using RUB = long long int;
using USD = long long int;

// курсор времени симуляции (обновляется в simulation)
int cur_year = 2026;
int cur_month = 9;

struct Inflation { // индивидуальная инфляция по статьям, в базисных пунктах в год
    RUB food;
    RUB utilities;
    RUB pet;
    RUB car;
    RUB medicine;
    RUB rent;
    RUB rental_income;
    RUB salary;
    RUB tax;
    RUB clothes;
    RUB home_repair;
};

struct Car { // ремонт, топливо, обслуживание, автокредит
    RUB repair;          // база мелких неполадок в месяц
    RUB fuel;            // база топлива в месяц
    RUB service;         // цена одного ТО (списывается раз в полгода)
    RUB credit;
    RUB credit_payment;
    RUB credit_rate_bp;
    int credit_months;
};

struct Tax { // транспортный, имущественный, земельный (раз в год)
    RUB car_tax;
    RUB home_tax;
    RUB land_tax;
};

struct Family {  // еда, собака, одежда
    RUB food;      // базовая продуктовая корзина в месяц
    RUB dog;       // базовое содержание собаки в месяц
    RUB clothes;   // базовый уход/мелочи в месяц
    int dog_life_months;
};

struct medicine { // лекарства, зубы, болезни
    RUB remedy;    // аптечка/витамины база в месяц
    RUB teeth;     // цена одного визита к стоматологу
    RUB disease;   // резерв на болезнь (база)
};

struct Home { // коммунальные услуги, ремонт, ипотека, аренда, сдача
    RUB utilities;          // база коммуналки в месяц (зимой/летом корректируется)
    RUB repair;             // мелкие хозяйственные расходы в месяц
    RUB mortgage;
    RUB rent;               // если бы снимала сама
    RUB rental;             // расходы на сдаваемую квартиру
    RUB mortgage_payment;
    RUB mortgage_rate_bp;
    int mortgage_months;
    RUB flat_value;
};

struct Bank { // накопления, валюта, долговая карта, потребкредит
    RUB account_RUB;
    USD account_USD;
    RUB check;              // долг по долговой карте / овердрафт
    RUB credit;
    RUB credit_payment;
    RUB credit_rate_bp;
    int credit_months;
};

struct Person { // зарплата, наличные, доход от аренды
    RUB cash;
    RUB salary;
    RUB rental_income;
};

struct Inflation infl;
struct Car alice_car;
struct Tax alice_taxes;        // переименовано, чтобы не конфликтовало с alice_tax()
struct Family alice_family;
struct medicine alice_meds;    // переименовано, чтобы не конфликтовало с alice_medicine()
struct Home alice_home;
struct Bank alice_bank;
struct Person alice;

RUB total_income = 0;
RUB total_outflow = 0;
RUB total_refund = 0;
RUB total_interest = 0;

RUB year_mortgage_interest = 0;
RUB year_car_interest = 0;
RUB year_consumer_interest = 0;

// --- вспомогательные: случайность и рост цен ---

int rand_range(int lo, int hi) // целое включительно в [lo; hi]
{
    if (hi <= lo)
    {
        return lo;
    }

    return lo + rand() % (hi - lo + 1);
}

bool chance(int percent) // истина с вероятностью percent из 100
{
    return rand() % 100 < percent;
}

RUB inflate(RUB value, RUB yearly_bp) // месячный прирост годовой ставки
{
    if (value <= 0)
    {
        return value;
    }

    return value + value * yearly_bp / 120000;
}

RUB inflate_year(RUB value, RUB yearly_bp) // годовой прирост
{
    if (value <= 0)
    {
        return value;
    }

    return value + value * yearly_bp / 10000;
}

void add_income(RUB sum)
{
    alice.cash += sum;
    total_income += sum;
}

void add_outflow(RUB sum)
{
    alice.cash -= sum;
    total_outflow += sum;
}

void alice_init()
{
    infl.food = 1000;          // 10% в год
    infl.utilities = 1200;     // 12%
    infl.pet = 900;
    infl.car = 1100;
    infl.medicine = 1300;      // медицина дорожает быстрее всего
    infl.rent = 800;
    infl.rental_income = 700;
    infl.salary = 600;         // индексация зарплаты отстаёт от расходов
    infl.tax = 700;
    infl.clothes = 850;
    infl.home_repair = 950;

    alice_car.repair = 800;
    alice_car.fuel = 5000;
    alice_car.service = 6000;  // одно ТО
    alice_car.credit = 400'000;
    alice_car.credit_payment = 9000;
    alice_car.credit_rate_bp = 1200;
    alice_car.credit_months = 60;

    alice_taxes.car_tax = 6000;
    alice_taxes.home_tax = 6000;
    alice_taxes.land_tax = 1500;

    alice_family.food = 15000;
    alice_family.dog = 3500;
    alice_family.clothes = 1200;   // база, сезонное сверху
    alice_family.dog_life_months = 96;

    alice_meds.remedy = 1000;
    alice_meds.teeth = 4000;       // за визит
    alice_meds.disease = 800;

    alice_home.utilities = 6000;
    alice_home.repair = 1000;      // база, поломки сверху
    alice_home.mortgage = 2000'000;
    alice_home.rent = 0;
    alice_home.rental = 1000;
    alice_home.mortgage_payment = 24500;
    alice_home.mortgage_rate_bp = 800;
    alice_home.mortgage_months = 120;
    alice_home.flat_value = 2300'000;

    alice_bank.account_RUB = 300'000;
    alice_bank.account_USD = 2000;
    alice_bank.check = 0;
    alice_bank.credit = 100'000;
    alice_bank.credit_payment = 3800;
    alice_bank.credit_rate_bp = 2000;  // 20% - самый дорогой долг
    alice_bank.credit_months = 36;

    alice.cash = 20'000;
    alice.salary = 80'000;
    alice.rental_income = 20'000;
}

// --- доходы ---

void alice_salary()
{
    if (cur_month == 1) // январь: индексация и оценка квартиры
    {
        alice_home.flat_value = inflate_year(alice_home.flat_value, 500);

        if (cur_year == 2027)
        {
            alice.salary = 100'000; // promotion
        }
        else if (cur_year > 2027)
        {
            alice.salary = inflate_year(alice.salary, infl.salary);
        }
    }

    add_income(alice.salary);

    // квартальная премия - не гарантирована
    if (cur_month == 3 or cur_month == 6 or cur_month == 9 or cur_month == 12)
    {
        if (chance(60))
        {
            add_income(rand_range(10'000, 40'000));
        }
    }

    // годовой бонус
    if (cur_month == 12 and chance(70))
    {
        add_income(alice.salary / 2);
    }
}

void alice_rental_income()
{
    add_income(alice.rental_income);
}

void alice_bank_income()
{
    RUB rub_interest = alice_bank.account_RUB * 700 / 120000; // 7% годовых
    alice_bank.account_RUB += rub_interest;
    total_income += rub_interest;

    USD usd_interest = alice_bank.account_USD * 200 / 120000; // 2% годовых
    alice_bank.account_USD += usd_interest;
}

// --- долговая карта / овердрафт ---

void alice_check_interest()
{
    if (alice_bank.check <= 0)
    {
        return;
    }

    RUB interest = alice_bank.check * 3000 / 120000; // 30% годовых
    alice_bank.check += interest;
    total_interest += interest;
}

void alice_check_payment()
{
    if (alice.cash < 0) // ушли в минус - фиксируем долг по карте
    {
        alice_bank.check += -alice.cash;
        alice.cash = 0;
    }

    if (alice_bank.check <= 0)
    {
        return;
    }

    RUB min_pay = alice_bank.check * 5 / 100;

    if (min_pay < 1000)
    {
        min_pay = 1000;
    }

    if (min_pay > alice_bank.check)
    {
        min_pay = alice_bank.check;
    }

    if (alice.cash >= min_pay)
    {
        alice.cash -= min_pay;
        alice_bank.check -= min_pay;
    }
    else
    {
        alice_bank.check -= alice.cash;
        alice.cash = 0;
    }
}

// --- кредиты (детальная тема) ---

void pay_loan(
    RUB &balance,
    RUB payment,
    RUB rate_bp,
    int &months,
    RUB &year_interest
)
{
    if (balance <= 0)
    {
        balance = 0;
        months = 0;
        return;
    }

    RUB interest = balance * rate_bp / 120000;
    RUB total_due = balance + interest;
    RUB due = payment;

    if (due > total_due)
    {
        due = total_due;
    }

    add_outflow(due);

    balance = total_due - due;
    year_interest += interest;
    total_interest += interest;

    if (balance <= 0)
    {
        balance = 0;
        months = 0;
    }
    else if (months > 0)
    {
        --months;
    }
}

void alice_mortgage()
{
    pay_loan(
        alice_home.mortgage,
        alice_home.mortgage_payment,
        alice_home.mortgage_rate_bp,
        alice_home.mortgage_months,
        year_mortgage_interest
    );
}

void alice_car_credit()
{
    pay_loan(
        alice_car.credit,
        alice_car.credit_payment,
        alice_car.credit_rate_bp,
        alice_car.credit_months,
        year_car_interest
    );
}

void alice_consumer_credit()
{
    pay_loan(
        alice_bank.credit,
        alice_bank.credit_payment,
        alice_bank.credit_rate_bp,
        alice_bank.credit_months,
        year_consumer_interest
    );
}

// --- расходы со случайными и сезонными тратами ---

void alice_food()
{
    RUB sum = alice_family.food; // база

    if (chance(40)) // кафе / доставка
    {
        sum += rand_range(500, 2500);
    }

    if (chance(8)) // праздник / гости
    {
        sum += rand_range(2000, 6000);
    }

    add_outflow(sum);
}

void alice_dog()
{
    if (alice_family.dog_life_months <= 0)
    {
        return;
    }

    RUB sum = alice_family.dog; // база

    if (chance(15)) // прививка / плановый визит
    {
        sum += rand_range(800, 3000);
    }

    if (chance(3)) // серьёзная ветеринария
    {
        sum += rand_range(5000, 20000);
    }

    add_outflow(sum);
    --alice_family.dog_life_months;
}

void alice_clothes()
{
    RUB sum = alice_family.clothes; // база

    if (cur_month == 3 or cur_month == 9) // сезонная смена гардероба
    {
        sum += rand_range(4000, 15000);
    }

    add_outflow(sum);
}

void alice_medicine()
{
    RUB sum = alice_meds.remedy + alice_meds.disease; // аптечка + резерв

    if (chance(20)) // ОРВИ / простуда
    {
        sum += rand_range(500, 2500);
    }

    if (cur_month == 6 or cur_month == 12) // плановый стоматолог
    {
        sum += alice_meds.teeth * rand_range(1, 3);
    }

    if (chance(2)) // крупное лечение / диагностика
    {
        sum += rand_range(10000, 50000);
    }

    add_outflow(sum);
}

void alice_utilities()
{
    RUB sum = alice_home.utilities; // база

    if (cur_month >= 10 or cur_month <= 4) // отопительный сезон
    {
        sum = sum * 130 / 100; // зимой +30%
    }
    else
    {
        sum = sum * 85 / 100;  // летом дешевле
    }

    add_outflow(sum);
}

void alice_home_repair()
{
    RUB sum = alice_home.repair; // база

    if (chance(10)) // сломалась техника / сантехника
    {
        sum += rand_range(2000, 15000);
    }

    if (chance(2)) // крупный ремонт / замена окна, бойлера
    {
        sum += rand_range(20000, 80000);
    }

    add_outflow(sum);
}

void alice_rent()
{
    if (alice_home.rent > 0)
    {
        add_outflow(alice_home.rent);
    }
}

void alice_rental_expense()
{
    if (alice_home.rental > 0)
    {
        add_outflow(alice_home.rental);
    }
}

void alice_car_costs()
{
    RUB sum = alice_car.fuel; // база топлива

    if (chance(30)) // extra поездки / дальняя дорога
    {
        sum += rand_range(1000, 4000);
    }

    if (cur_month == 4 or cur_month == 10) // ТО раз в полгода
    {
        sum += alice_car.service;
    }

    if (cur_month == 4 or cur_month == 10) // сезонная резина / шиномонтаж
    {
        sum += rand_range(6000, 12000);
    }

    if (chance(8)) // незапланированный ремонт
    {
        sum += rand_range(3000, 25000);
    }

    if (chance(2)) // крупная поломка
    {
        sum += rand_range(15000, 60000);
    }

    add_outflow(sum);
}

void alice_tax()
{
    if (cur_month != 11) // налоги раз в год, в ноябре
    {
        return;
    }

    add_outflow(alice_taxes.car_tax);
    add_outflow(alice_taxes.home_tax);
    add_outflow(alice_taxes.land_tax);
}

void alice_tax_refund()
{
    if (cur_month != 12)
    {
        return;
    }

    RUB refund = 15600; // упрощённый стандартный возврат

    RUB mortgage_refund = year_mortgage_interest * 13 / 100; // вычет по процентам

    if (mortgage_refund > 390000)
    {
        mortgage_refund = 390000;
    }

    refund += mortgage_refund;

    add_income(refund);
    total_refund += refund;

    year_mortgage_interest = 0;
    year_car_interest = 0;
    year_consumer_interest = 0;
}

// --- управление наличными и долгами ---

void alice_save_excess()
{
    const RUB cash_buffer = 50'000;

    if (alice.cash <= cash_buffer)
    {
        return;
    }

    RUB free_money = alice.cash - cash_buffer;

    if (alice_bank.check > 0) // сначала закрываем дорогой долг по карте
    {
        RUB pay_check = free_money;

        if (pay_check > alice_bank.check)
        {
            pay_check = alice_bank.check;
        }

        alice_bank.check -= pay_check;
        alice.cash -= pay_check;
        free_money -= pay_check;
    }

    if (free_money > 0)
    {
        alice_bank.account_RUB += free_money;
        alice.cash -= free_money;
    }
}

void alice_cover_deficit()
{
    if (alice.cash >= 0)
    {
        return;
    }

    RUB need = -alice.cash;

    if (alice_bank.account_RUB > 0) // сначала подушка безопасности
    {
        RUB take = need;

        if (take > alice_bank.account_RUB)
        {
            take = alice_bank.account_RUB;
        }

        alice_bank.account_RUB -= take;
        alice.cash += take;
        need -= take;
    }

    if (need > 0) // остаток уходит в долговую карту
    {
        alice_bank.check += need;
        alice.cash = 0;
    }
}

void alice_prepay_debts()
{
    const RUB savings_reserve = 100'000; // подушку не трогаем

    if (alice_bank.account_RUB <= savings_reserve)
    {
        return;
    }

    RUB free = alice_bank.account_RUB - savings_reserve;

    if (free <= 0)
    {
        return;
    }

    // гасим тела кредитов от самого дорогого к самому дешёвому
    RUB pay = free;

    if (pay > alice_bank.credit)
    {
        pay = alice_bank.credit;
    }

    alice_bank.credit -= pay;
    alice_bank.account_RUB -= pay;
    free -= pay;

    if (free > 0)
    {
        pay = free;

        if (pay > alice_car.credit)
        {
            pay = alice_car.credit;
        }

        alice_car.credit -= pay;
        alice_bank.account_RUB -= pay;
        free -= pay;
    }

    if (free > 50'000) // ипотеку трогаем только если осталось много сверх буфера
    {
        pay = free - 50'000;

        if (pay > alice_home.mortgage)
        {
            pay = alice_home.mortgage;
        }

        alice_home.mortgage -= pay;
        alice_bank.account_RUB -= pay;
    }
}

void apply_inflation()
{
    alice_family.food = inflate(alice_family.food, infl.food);
    alice_family.clothes = inflate(alice_family.clothes, infl.clothes);
    alice_family.dog = inflate(alice_family.dog, infl.pet);

    alice_meds.remedy = inflate(alice_meds.remedy, infl.medicine);
    alice_meds.teeth = inflate(alice_meds.teeth, infl.medicine);
    alice_meds.disease = inflate(alice_meds.disease, infl.medicine);

    alice_home.utilities = inflate(alice_home.utilities, infl.utilities);
    alice_home.repair = inflate(alice_home.repair, infl.home_repair);
    alice_home.rent = inflate(alice_home.rent, infl.rent);
    alice_home.rental = inflate(alice_home.rental, infl.rent);

    alice.rental_income = inflate(alice.rental_income, infl.rental_income);

    alice_car.repair = inflate(alice_car.repair, infl.car);
    alice_car.fuel = inflate(alice_car.fuel, infl.car);
    alice_car.service = inflate(alice_car.service, infl.car);

    alice_taxes.car_tax = inflate(alice_taxes.car_tax, infl.tax);
    alice_taxes.home_tax = inflate(alice_taxes.home_tax, infl.tax);
    alice_taxes.land_tax = inflate(alice_taxes.land_tax, infl.tax);
}

void simulation()
{
    while ( not ( cur_year == 2036 and cur_month == 9 ) )
    {
        alice_check_interest();

        alice_salary();
        alice_rental_income();
        alice_bank_income();

        alice_check_payment();

        alice_mortgage();
        alice_car_credit();
        alice_consumer_credit();

        alice_food();
        alice_dog();
        alice_clothes();
        alice_medicine();

        alice_utilities();
        alice_home_repair();

        alice_rent();
        alice_rental_expense();

        alice_car_costs();

        alice_tax();
        alice_tax_refund();

        alice_save_excess();
        alice_cover_deficit();
        alice_prepay_debts();

        apply_inflation();

        ++cur_month;

        if (cur_month == 13)
        {
            ++cur_year;
            cur_month = 1;
        }
    }
}

void alice_print()
{
    RUB debts =
        alice_home.mortgage +
        alice_car.credit +
        alice_bank.credit +
        alice_bank.check;

    RUB usd_in_rub = alice_bank.account_USD * 90;

    RUB assets =
        alice.cash +
        alice_bank.account_RUB +
        alice_home.flat_value +
        usd_in_rub;

    RUB net_worth = assets - debts;

    printf("Alice salary = %lld\n", alice.salary);
    printf("Alice cash = %lld\n", alice.cash);

    printf("Savings RUB = %lld\n", alice_bank.account_RUB);
    printf("Savings USD = %lld\n", alice_bank.account_USD);

    printf("Mortgage left = %lld\n", alice_home.mortgage);
    printf("Mortgage months left = %d\n", alice_home.mortgage_months);

    printf("Car credit left = %lld\n", alice_car.credit);
    printf("Car credit months left = %d\n", alice_car.credit_months);

    printf("Consumer credit left = %lld\n", alice_bank.credit);
    printf("Consumer credit months left = %d\n", alice_bank.credit_months);

    printf("Check debt = %lld\n", alice_bank.check);

    printf("Flat value = %lld\n", alice_home.flat_value);

    printf("Total income = %lld\n", total_income);
    printf("Total outflow = %lld\n", total_outflow);
    printf("Total refund = %lld\n", total_refund);
    printf("Total interest = %lld\n", total_interest);

    printf("Assets = %lld\n", assets);
    printf("Debts = %lld\n", debts);
    printf("Net worth = %lld\n", net_worth);

    if (net_worth > 0 and debts == 0)
    {
        printf(" вывод: модель хорошая, кредиты закрыты, есть активы.\n");
    }
    else if (net_worth > 0)
    {
        printf(" вывод: модель средняя, актив есть, но остались долги.\n");
    }
    else
    {
        printf(" вывод: модель плохая, долги больше активов.\n");
    }
}

int main()
{
    // фиксированный сид = воспроизводимость (один и тот же расклад каждый запуск).
    // Для "разных жизней" при A/B замени на: srand((unsigned)time(0)); (+ #include <time.h>)
    srand(20260901u);

    alice_init();
    simulation();
    alice_print();

    return 0;
}