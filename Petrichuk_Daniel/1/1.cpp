#include <stdio.h>

using RUB = unsigned long long int;                             // Важна точка управления       Безнаковый тип прописываем (больше пустых ячеек- экономия памяти)

struct Person {
    RUB cash;
    RUB salary;
};

enum Strategy {mortgage, saving};

void person_salary(Person* p, const int year, const int month)              // const - показываем, что переменная не меняется
{
    if (year == 2027 and month == 1) {   //Promotion}
        p->salary = 100'000;
    }
    p->cash += p->salary;
}

void simulate(Person* p, enum Strategy s)
{
    int year = 2005;
    int month = 5;

    while (not (year == 2036 and month == 9)) {                 // скобку { ставим именно здесь (как и в условном операторе)
        
        person_salary(p, year, month);

        // danya_car();                                         // Задаем ТЗ
        // danya_mortgage();                                    // Добавить вклады, налоги, инфлянцию
        // danya_rent();
        // danya_home_bills();
        // danya_food();
        // danya_dog();
        // danya_bank_income();

        if (s == mortgage) {
            // Действие
        } else if (s == saving) {
            // Действие
        }

        ++month;

        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}


void person_init(Person* p)
{
    p->cash = 20'000;
    p->salary = 80'000;
}


void person_print(Person* p)
{
    printf("Danya cash = %lld", p->cash);
}

int main()

{
    Person danya_mortgage;
    Person danya_saving;

    person_init(&danya_mortgage);
    person_init(&danya_saving);

    simulate(&danya_mortgage, mortgage);
    simulate(&danya_saving, saving);

    person_print(&danya_mortgage);
    person_print(&danya_saving);

    return 0;
}