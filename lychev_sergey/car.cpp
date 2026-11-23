// ============================================================
// Модель личного автомобиля: амортизация, ежемесячные расходы,
// поломки, ремонт и замена на новую машину
// ============================================================

#include "car.h"
#include "events.h"
#include <cmath>

// ============================================================
// Внутренние константы и хелперы
// ============================================================

namespace {

// Через сколько месяцев машина считается «старой» (15 лет)
constexpr int CAR_MAX_AGE_MONTHS = 15 * 12;

// Списывает сумму с наличных и учитывает её в общих расходах
// и в персональном счётчике расходов на машину
void car_pay(RUB amount, RUB& cash, RUB& total_expense, Car& c)
{
    cash          -= amount;
    total_expense += amount;
    c.total_spent += amount;
}

// Инфлирует значение на месячный множитель и округляет до рубля
void car_inflate_value(RUB& value, double monthly_rate)
{
    double raw = static_cast<double>(value) * monthly_rate;
    long long rounded = std::llround(raw);
    value = static_cast<RUB>(rounded);
}

// Покупка новой машины: списываем цену, обнуляем возраст,
// сбрасываем состояние поломки и счёт за ремонт
void car_buy_new(Car& c, RUB& cash, RUB& total_expense)
{
    car_pay(c.new_car_price, cash, total_expense, c);
    c.age_months  = 0;
    c.broken      = false;
    c.repair_cost = 0;
}

// Случайная стоимость ремонта в диапазоне [repair_min, repair_max]
// roll_range возвращает int, а нам нужен RUB — приводим явно
RUB car_roll_repair_cost(const Car& c)
{
    int lo = static_cast<int>(c.repair_min);
    int hi = static_cast<int>(c.repair_max);
    int rolled = roll_range(lo, hi);
    return static_cast<RUB>(rolled);
}

} // namespace


// Задаёт начальное состояние машины: возраст, расходы, цены
// repair_cost = 0 и broken = false — машина рабочая
void car_init(Car& c)
{
    c.has_car       = true;
    c.age_months    = roll_range(12, 60);
    c.monthly_cost  = 10000;
    c.taxi_cost     = 15000;
    c.repair_min    = 24000;
    c.repair_max    = 90000;
    c.new_car_price = 1500000;
    c.repair_cost   = 0;
    c.broken        = false;
    c.repairs_total = 0;
    c.total_spent   = 0;
}

// Ежемесячное обновление состояния машины
//
// Порядок действий:
//   1. Увеличиваем возраст
//   2. Инфлируем все цены на месячный множитель
//   3. Если машина сломана — платим за такси и пытаемся починить
//      (или заменить, если машина старая)
//   4. Если рабочая — платим за топливо и проверяем шанс поломки
//
// Параметры year/month/owner сейчас не используются, но оставлены
// в сигнатуре — на случай будущего логирования
void car_monthly(Car& c, RUB& cash, RUB& total_expense,
                 int year, int month, const char* owner)
{
    (void)year;
    (void)month;
    (void)owner;

    if (!c.has_car) {
        return;
    }

    c.age_months++;

    // Годовая инфляция -> месячный множитель:
    // (1 + infl_year/100)^(1/12)
    const double monthly_infl =
        std::pow(1.0 + c.infl_year / 100.0, 1.0 / 12.0);

    car_inflate_value(c.monthly_cost,  monthly_infl);
    car_inflate_value(c.taxi_cost,     monthly_infl);
    car_inflate_value(c.repair_min,    monthly_infl);
    car_inflate_value(c.repair_max,    monthly_infl);
    car_inflate_value(c.new_car_price, monthly_infl);

    // Сломана: платим за такси, пока не починим
    if (c.broken) {
        car_pay(c.taxi_cost, cash, total_expense, c);

        // Сначала пытаемся починить — если денег на ремонт хватает
        if (cash >= c.repair_cost) {
            car_pay(c.repair_cost, cash, total_expense, c);
            c.broken = false;
            c.repairs_total++;
        }
        // Иначе, если машина совсем старая и на новую есть деньги — покупаем новую
        else if (c.age_months >= CAR_MAX_AGE_MONTHS &&
                 cash >= c.new_car_price) {
            car_buy_new(c, cash, total_expense);
        }
        return;
    }

    // Рабочая: платим за топливо и проверяем шанс поломки
    car_pay(c.monthly_cost, cash, total_expense, c);

    if (roll_chance(c.break_chance)) {
        c.broken      = true;
        c.repair_cost = car_roll_repair_cost(c);
    }
}
