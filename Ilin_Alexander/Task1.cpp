#include <iostream>
#include <stdio.h>
#include <random>
#include <string>

using RUB = unsigned long int;

namespace Config {
    // Дата
    constexpr int END_MONTH = 7;
    constexpr int END_YEAR  = 2036;
    // Банк
    constexpr double BASE_BANK_PERCENT   = 0.10;
    constexpr double SPEND_BONUS_PERCENT = 0.07;
    constexpr RUB    SPEND_BONUS_THRESHOLD = 5'000;
    // Интерес (границы)
    constexpr double INTEREST_HIGH   = 0.10;
    constexpr double INTEREST_MEDIUM = 0.30;
    constexpr double INTEREST_LOW    = 0.50;
    // Интерес (шансы)
    constexpr int CHANCE_MEDIUM = 50; // %
    constexpr int CHANCE_LOW    = 10; // %
}

class random_int {
    std::mt19937 gen_;
public:
    random_int() : gen_(std::random_device{}()) {}                              // Случайное число

    RUB integer(RUB low, RUB upp) {
        return std::uniform_int_distribution<RUB>(low, upp)(gen_);              // Случайное число в пределах
    }

    bool chance(int percent) {
        return std::uniform_int_distribution<int>(1, 100)(gen_) <= percent;     // случайное число в процентах шанса
    }
};

class Person {
public:

    // Состояние
    bool alive  = true;

    // Деньги
    RUB money         = 10'000;
    RUB safety_pillow = 50'000;
    RUB bank_money    = 0;
    RUB month_spent   = 0;

    // Доходы
    RUB salary = 10'000;

    // Обязательные траты
    RUB spents_food     = 8'000;
    RUB spents_internet = 1'500;
    bool spents_internet_b = true;
    bool internet = true; 

    // Животное
    RUB animal_cost = 10'000;
    bool animal = false;

    Person() = default;
    explicit Person(RUB safety_pillow_new) : safety_pillow(safety_pillow_new) 
    {

    }

    void earn(const RUB amount)                             // Заработок
    { 
        money += amount;   
    }

    void spend(const RUB amount)                            // Трата
    { 
        money -= amount; 
    }

    bool can_buy(const RUB cost)                            // Проверка на хватку денег
    {
        return (cost <= money + bank_money);
    }

    void receive_salary() {                                 // Зарплата
        earn(salary);
    }

    void do_shopping(random_int& r_int) {                   //Покупка
        RUB spent = 0;

        // Обязательные траты

        RUB cost = spents_food;
        if (spents_internet_b) {
            cost += spents_internet;
            internet = true;
        }
        else {
            internet = false;
        }

        if (can_buy(cost)) {
            spend(cost);
            spent += cost;
        } else {
            alive = false;
            return;
        }

        // Необязательные траты

        if (!animal && can_buy(animal_cost) && interested_in(animal_cost, r_int)) {
            spend(animal_cost);
            animal = true;
            spent += animal_cost;
        }

        month_spent = spent;
    }

    void save_to_bank() {                                               // Накопление в банк
        if (money > safety_pillow) {
            RUB transfer = money - safety_pillow;
            money      -= transfer;
            bank_money += transfer;
        }
    }

    void bank_percents() {                                              // Начисление процентов за месяц
        double percent = Config::BASE_BANK_PERCENT;
        if (month_spent >= Config::SPEND_BONUS_THRESHOLD) {
            percent += Config::SPEND_BONUS_PERCENT;
        }
        bank_money += static_cast<RUB>(bank_money * percent / 12);
    }

    void print_report() const {                                         // Сводка результатов
        if (!alive) {
            printf("You are bankrupt :(\n");
            return;
        }
        printf("My money         : %llu\n", money);
        printf("My money in bank : %llu\n", bank_money);
        printf("Total            : %llu\n", money + bank_money);
        if (animal) {
            printf("You have an animal\n");
        }
    }

private:

    bool interested_in(const RUB cost, random_int& r_int){                                     // Интерес в покупке
        if (bank_money == 0) {
            return false;
        }

        double interest = static_cast<double>(cost) / static_cast<double>(bank_money);

        if (interest <= Config::INTEREST_HIGH) {
            return true;
        }
        if (interest <= Config::INTEREST_MEDIUM) {
            if (r_int.integer(0, 100) < Config::CHANCE_MEDIUM) {
                return true;
            }
            else {
                return false;
            }
        }
        if (interest <= Config::INTEREST_LOW){
            if (r_int.integer(0, 100) < Config::CHANCE_LOW) {
                return true;
            }
            else {
                return false;
            }  
        }
        else {
            return false;
        }    
    }
};

void simulation(Person& person) {
    random_int r_int;

    for (int year = 2026; year <= Config::END_YEAR; ++year) {
        for (int month = 1; month <= 12; ++month) {
            if (year == Config::END_YEAR && month == Config::END_MONTH) {
                person.print_report();
                return;
            }

            if (!person.alive) {
                person.print_report();
                return;
            }

            person.receive_salary();
            person.do_shopping(r_int);
            person.save_to_bank();
            person.bank_percents();
        }
    }
}

int main() {
    Person Alice;
    Person Bob{20'000};

    printf("Alice\n");
    simulation(Alice);

    printf("\nBob\n");
    simulation(Bob);
}
