#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <string>
#include <algorithm>

using namespace std;

using kopecks = long long int; // считаем в копейках, чтобы избежать проблем с плавающей запятой

kopecks rubles(long long value) // перевод рублей в копейки
{
    return value * 100;
}

kopecks percent_of(kopecks value, double percent) // вычисление процента от суммы
{
    double result = static_cast<double>(value) * percent;
    return static_cast<kopecks>(llround(result));
}

kopecks monthly_percent(kopecks value, double annual_percent) // вычисление ежемесячного процента от суммы
{
    return percent_of(value, annual_percent / 12.0);
}

kopecks increase_by_inflation(kopecks value, double annual_inflation) // вычисление увеличения суммы с учетом инфляции
{
    double monthly_inflation =
        pow(1.0 + annual_inflation, 1.0 / 12.0) - 1.0;

    double new_value =
        static_cast<double>(value) * (1.0 + monthly_inflation);

    return static_cast<kopecks>(llround(new_value));
}

void print_money(kopecks value) // перевод в рубли и копейки и вывод на экран
{
    bool negative = value < 0;

    if (negative)
    {
        value = -value;
        cout << '-';
    }

    long long ruble_part = value / 100;
    long long kopeck_part = value % 100;

    cout << ruble_part
         << '.'
         << setw(2)
         << setfill('0')
         << kopeck_part
         << setfill(' ')
         << " RUB";
}


namespace config // константы симуляции
{
    constexpr int start_year = 2026;
    constexpr int start_month = 9;

    constexpr int simulation_years = 10;

    constexpr double bank_annual_percent = 0.10;

    constexpr double salary_annual_growth = 0.05;

    constexpr double food_inflation = 0.07;
    constexpr double utility_inflation = 0.06;
    constexpr double rent_inflation = 0.06;
    constexpr double car_inflation = 0.05;
    constexpr double pet_inflation = 0.06;

    constexpr int car_repair_chance = 5;
    constexpr int car_repair_cost = 30'000;

    constexpr int unexpected_chance = 3;
    constexpr int unexpected_cost = 20'000;

    constexpr int kirill_cash_reserve = 300'000;
    constexpr int kirill_salary = 90'000;
    constexpr int bob_cash_reserve = 150'000;
    constexpr int bob_salary = 75'000;

    constexpr int kirill_mortgage = 15'000'000;
    constexpr double kirill_mortgage_percent = 0.085;
    constexpr int kirill_mortgage_years = 20;
}

struct Date
{
    int year = config::start_year;
    int month = config::start_month;

    void next_month()
    {
        ++month;

        if (month > 12)
        {
            month = 1;
            ++year;
        }
    }

class random_generator
{
private:
    mt19937 generator;

public:
    random_generator()
        : generator(random_device{}())
    {
    }

    int integer(int low, int high)
    {
        uniform_int_distribution<int> distribution(low, high);

        return distribution(generator);
    }

    bool chance(int percent)
    {
        return integer(1, 100) <= percent;
    }
};

    void print() const
    {
        cout << year << '-'
             << setw(2)
             << setfill('0')
             << month
             << setfill(' ');
    }
};

struct mortgage
{
    bool active = false;

    kopecks balance = 0;

    kopecks monthly_payment = 0;

    double annual_percent = 0.0;

    int months_left = 0;

    void setup(kopecks initial_balance,
               double new_annual_percent,
               int term_months)
    {
        balance = initial_balance;
        annual_percent = new_annual_percent;
        months_left = term_months;

        active = balance > 0 && months_left > 0;

        double monthly_rate = annual_percent / 12.0;

        double principal =
            static_cast<double>(balance);

        if (monthly_rate == 0.0)
        {
            monthly_payment =
                balance / months_left;

            return;
        }

        double payment =
            principal
            * monthly_rate
            * pow(1.0 + monthly_rate, months_left)
            / (pow(1.0 + monthly_rate, months_left) - 1.0);

        monthly_payment =
            static_cast<kopecks>(llround(payment));
    }

    kopecks current_payment() const
    {
        if (!active)
        {
            return 0;
        }

        kopecks interest =
            monthly_percent(balance, annual_percent);

        kopecks total_debt =
            balance + interest;

        return min(monthly_payment, total_debt);
    }

    void make_payment()
    {
        if (!active)
        {
            return;
        }

        kopecks interest =
            monthly_percent(balance, annual_percent);

        balance += interest;

        kopecks payment =
            min(monthly_payment, balance);

        balance -= payment;

        --months_left;

        if (balance <= 0 || months_left <= 0)
        {
            balance = 0;
            months_left = 0;
            active = false;
        }
    }
};

struct car
{
    bool owned = false;

    kopecks monthly_cost = 0;

    void apply_inflation()
    {
        if (owned)
        {
            monthly_cost =
                increase_by_inflation(
                    monthly_cost,
                    config::car_inflation
                );
        }
    }
};

struct pet
{
    bool alive = false;

    kopecks monthly_cost = 0;

    int age_months = 0;

    int lifetime_months = 0;

    void grow_one_month()
    {
        if (!alive)
        {
            return;
        }

        ++age_months;

        if (age_months >= lifetime_months)
        {
            alive = false;
        }
    }

    void apply_inflation()
    {
        if (alive)
        {
            monthly_cost =
                increase_by_inflation(
                    monthly_cost,
                    config::pet_inflation
                );
        }
    }
}

struct Person // структура для представления человека
{
    string name;

    kopecks cash = 0;

    kopecks savings = 0;

    kopecks salary = 0;

    kopecks food_expense = 0;

    kopecks utility_expense = 0;

    kopecks rent_expense = 0;

    kopecks cash_reserve = 0;

    car personal_car;

    pet personal_pet;

    mortgage home_mortgage;

    void setup(const string& new_name, // установка параметров человека
               kopecks new_cash,
               kopecks new_salary,
               kopecks new_food,
               kopecks new_utility,
               kopecks new_rent,
               kopecks new_cash_reserve)
    {
        name = new_name;

        cash = new_cash;

        salary = new_salary;

        food_expense = new_food;

        utility_expense = new_utility;

        rent_expense = new_rent;

        cash_reserve = new_cash_reserve;
    }

    void receive_salary() // получение зарплаты
    {
        cash += salary;
    }

    void receive_bank_interest() // получение процентов по банковским вкладам
    {
        kopecks interest =
            monthly_percent(
                savings,
                config::bank_annual_percent
            );

        savings += interest;
    }

    bool can_pay(kopecks amount) const // проверка наличия денег для оплаты
    {
        return amount <= cash + savings;
    }

    bool pay(kopecks amount) // оплата расходов
    {
        if (amount <= 0)
        {
            return true;
        }

        if (!can_pay(amount))
        {
            return false;
        }

        if (cash >= amount)
        {
            cash -= amount;

            return true;
        }

        kopecks remaining =
            amount - cash;

        cash = 0;

        savings -= remaining;

        return true;
    }

    void save_surplus() // сохранение излишков на вклад
    {
        if (cash <= cash_reserve)
        {
            return;
        }

        kopecks transfer =
            cash - cash_reserve;

        cash -= transfer;

        savings += transfer;
    }

    bool pay_regular_expenses() // оплата регулярных расходов
    {
        kopecks expenses = 0;

        expenses += food_expense;

        expenses += utility_expense;

        expenses += rent_expense;

        expenses +=
            personal_car.owned
            ? personal_car.monthly_cost
            : 0;

        expenses +=
            personal_pet.alive
            ? personal_pet.monthly_cost
            : 0;

        if (!pay(expenses))
        {
            return false;
        }

        return true;
    }

    bool pay_mortgage() // оплата ипотеки
    {
        if (!home_mortgage.active)
        {
            return true;
        }

        kopecks payment =
            home_mortgage.current_payment();

        if (!pay(payment))
        {
            return false;
        }

        home_mortgage.make_payment();

        return true;
    }

    bool process_random_events( // обработка случайных событий
        random_generator& random)
    {
        if (personal_car.owned &&
            random.chance(
                config::car_repair_chance))
        {
            kopecks repair_cost =
                rubles(
                    config::car_repair_cost
                );

            if (!pay(repair_cost))
            {
                return false;
            }

            cout << "      Car repair: ";

            print_money(repair_cost);

            cout << '\n';
        }

        if (random.chance(
                config::unexpected_chance))
        {
            kopecks unexpected_cost =
                rubles(
                    config::unexpected_cost
                );

            if (!pay(unexpected_cost))
            {
                return false;
            }

            cout << "      Unexpected expense: ";

            print_money(unexpected_cost);

            cout << '\n';
        }

        return true;
    }

    void apply_inflation() // инфляция
    {
        food_expense =
            increase_by_inflation(
                food_expense,
                config::food_inflation
            );

        utility_expense =
            increase_by_inflation(
                utility_expense,
                config::utility_inflation
            );

        if (rent_expense > 0)
        {
            rent_expense =
                increase_by_inflation(
                    rent_expense,
                    config::rent_inflation
                );
        }

        personal_car.apply_inflation();

        personal_pet.apply_inflation();
    }

    void update_life_state() // обновление состояния жизни питомца
    {
        personal_pet.grow_one_month();
    }

    void print_short_report( // вывод отчета о состоянии человека
        const Date& date) const
    {
        cout << "    ";

        date.print();

        cout << " | "
             << name
             << " | cash = ";

        print_money(cash);

        cout << " | savings = ";

        print_money(savings);

        cout << '\n';
    }

    void print_final_report() const // вывод итогового отчета
    {
        cout
            << "\n============================================\n";

        cout
            << name
            << " FINAL REPORT\n";

        cout
            << "============================================\n";

        cout << "Cash:             ";

        print_money(cash);

        cout << '\n';

        cout << "Savings:          ";

        print_money(savings);

        cout << '\n';

        cout << "Total money:      ";

        print_money(cash + savings);

        cout << '\n';

        cout << "Salary:           ";

        print_money(salary);

        cout << '\n';

        cout << "Car:              "
             << (personal_car.owned
                    ? "yes"
                    : "no")
             << '\n';

        cout << "Pet alive:        "
             << (personal_pet.alive
                    ? "yes"
                    : "no")
             << '\n';

        cout << "Mortgage balance: ";

        print_money(
            home_mortgage.balance
        );

        cout << '\n';
    }
};

bool simulate_one_month( // симуляция одного месяца жизни человека
    Person& person,
    Date& date,
    random_generator& random)
{
    if (date.month == config::start_month)
    {
        cout
            << "  Year: "
            << date.year
            << '\n';
    }

    person.receive_salary();

    person.receive_bank_interest();

    if (!person.pay_mortgage())
    {
        return false;
    }

    if (!person.pay_regular_expenses())
    {
        return false;
    }

    if (!person.process_random_events(random))
    {
        return false;
    }

    person.save_surplus();

    person.apply_inflation();

    person.salary =
        increase_by_inflation(
            person.salary,
            config::salary_annual_growth
        );

    person.update_life_state();

    return true;
}

void simulation(Person& person) // функция симуляции жизни человека
{
    random_generator random;

    Date date;

    int total_months =
        config::simulation_years * 12;

    cout
        << "\nStarting simulation for "
        << person.name
        << '\n';

    for (int month_number = 0;
         month_number < total_months;
         ++month_number)
    {
        bool successful =
            simulate_one_month(
                person,
                date,
                random
            );

        if (!successful)
        {
            cout
                << "    "
                << person.name
                << " cannot pay expenses anymore.\n";

            break;
        }

        date.next_month();
    }
}

Person create_kirill() // задание параметров Кирилла
{
    Person kirill;

    kirill.setup(
        "Kirill",
        rubles(300'000),
        rubles(90'000),
        rubles(23'000),
        rubles(6'000),
        rubles(30'000),
        rubles(config::kirill_cash_reserve)
    );

    return kirill;
}

Person create_bob() // задание параметров Боба
{
    Person bob;

    bob.setup(
        "Bob",
        rubles(150'000),
        rubles(75'000),
        rubles(20'000),
        rubles(6'000),
        rubles(28'000),
        rubles(config::bob_cash_reserve)
    );

    return bob;
}

void compare_people( // сравнение двух людей по их финансовому состоянию, в будущем по здоровью и другим параметрам
    const Person& kirill,
    const Person& bob)
{
    cout
        << "\n============================================\n";

    cout
        << "FINAL COMPARISON\n";

    cout
        << "============================================\n";

    cout << "Kirill total money: ";

    print_money(
        kirill.cash + kirill.savings
    );

    cout << '\n';

    cout << "Bob total money:   ";

    print_money(
        bob.cash + bob.savings
    );

    cout << '\n';

    cout
        << "\nImportant: property value is not included yet.\n";

    cout
        << "Therefore this version compares liquidity, "
        << "not net worth.\n";
}


int main() // главная функция программы
{
    Person kirill =
        create_kirill();

    Person bob =
        create_bob();

    cout
        << "============================================\n";

    cout
        << "KIRILL AND BOB - LIFE SIMULATION\n";

    cout
        << "============================================\n";

    cout
        << "Start date: "
        << config::start_year
        << '-'
        << config::start_month
        << '\n';

    cout
        << "Simulation length: "
        << config::simulation_years
        << " years\n";

    cout
        << "\n----------- KIRILL -----------\n";

    simulation(kirill);

    kirill.print_final_report();

    cout
        << "\n------------ BOB ------------\n";

    simulation(bob);

    bob.print_final_report();

    compare_people(
        kirill,
        bob
    );

    return 0;
}