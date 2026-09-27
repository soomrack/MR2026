#include <iostream>
#include <windows.h>
#include <string>
#include <cmath> 
#include <iomanip>

using namespace std;

// функция переключаем консоль Windows на UTF-8, чтобы русский текст выводился нормально.

void setup_console() 
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}


// Наш собственный тип: дата (год + месяц)

struct Date 
{
    int year = 2026;  // год
    int month = 1;    // месяц от 1 до 12

    // Перевести дату на следующий месяц
    void advance() 
    {
        ++month;            // увеличили месяц
        if (month > 12)  // если вышли за декабрь
        {  
            month = 1;      // начался январь
            ++year;         // новый год
        }
    }

    // Сколько месяцев прошло от даты start до этой даты
    int months_since(const Date& start) const 
    {
        return (year - start.year) * 12 + (month - start.month);
    }

    // Красивый текстовый вид: "2026-01"
    string str() const 
    {
        string m = (month < 10 ? "0" : "") + to_string(month);
        return to_string(year) + "-" + m;
    }
};

// Статья расходов: базовая цена + своя годовая инфляция

struct ExpenseCategory 
{
    string name;                // название, напр. "еда"
    double base_monthly = 0.0;  // цена в месяц на старте симуляции
    double annual_inflation = 0.0;  // доля в год: 0.08 = 8% годовых
    bool active = true;         // можно временно выключить статью

    // Сколько стоит эта статья на дату now
    double cost_on(const Date& start, const Date& now) const 
    {
        if (!active) 
        {
            return 0.0;
        }
        int n = now.months_since(start);          // сколько месяцев прошло
        double k = pow(1.0 + annual_inflation, n / 12.0);  // коэффициент роста
        return base_monthly * k;
    }
};


int main() 
{
    setup_console();

    Date start = { 2026, 1 };
    Date now = start;

    // создаём три статьи расходов
    ExpenseCategory food = { "еда",        25000.0, 0.08 };
    ExpenseCategory rent = { "аренда",     35000.0, 0.06 };
    ExpenseCategory transport = { "транспорт", 5000.0, 0.07 };

    // пройдём 25 месяцев и посмотрим, как растут цены
    for (int i = 0; i < 25; ++i) {
        cout << now.str()
            << " | еда: " << fixed << setprecision(2) << food.cost_on(start, now)
            << " | аренда: " << rent.cost_on(start, now)
            << " | транспорт: " << transport.cost_on(start, now)
            << "\n";
        now.advance();
    }

    return 0;
}
