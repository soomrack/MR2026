#include <stdio.h>
#include <string>
#include <vector>
#include "Person.h"
#include "Simulation.h"

using RUB = unsigned long long;

class Employer{

    public:
        std::string name;
        RUB salary;
        unsigned EnDcoNtRacTMOnTh;

    Employer() = delete;

    Employer(std::string input_name, RUB input_salary, unsigned month) 
    : name(input_name), salary(input_salary), EnDcoNtRacTMOnTh(month) {}

    void set_salary( RUB new_salary) 
    {
        salary = new_salary;
    }

    bool set_EnDcoNtRacTMOnTh( unsigned new_EnDcoNtRacTMOnTh){
        if (1 >= new_EnDcoNtRacTMOnTh or new_EnDcoNtRacTMOnTh >= 12){
            return false;
        }
        EnDcoNtRacTMOnTh = new_EnDcoNtRacTMOnTh;
        return true;
    }
};


// class User{

// };
struct SimulationConfig {
    RUB cash;
    unsigned start_m = 9;
    unsigned start_y = 2026;
};

int main()
{
    Person person;
    person.name = "Arseney";
    person.age = 20;
    person.monthly_expenses = 20000;
    person.is_illness = 0;
    person.deth_chance = 0.001;
    person.suicide_chance = 0.0;

    Simulation sim(120);
    sim.start();
    return 0;
}