#include <iostream>




constexpr int months_in_simulation = 15 * 12;


struct person {
    double cash = 0.0;    // баланс, руб. (с копейками)
    double salary = 0.0;  // текущий месячный оклад
};


struct person alice;


void alice_init() {
    alice.cash = 20000.0;
    alice.salary = 60000.0; // временно фиксированный оклад,
                             // потом это будет считаться через career_level
}


void alice_salary(int month) {
    alice.cash += alice.salary;
}


void simulation() {
    std::cout << "Начало симуляции\n";

    for (int month = 0; month < months_in_simulation; ++month) {
        alice_salary(month);

        
    }
}

// Финальный отчёт.
void alice_report() {
    std::cout << "Итог симуляции\n";
    std::cout << "Финальный баланс: " << alice.cash << " руб.\n";
}

int main() {
    alice_init();
    simulation();
    alice_report();
    return 0;
}
