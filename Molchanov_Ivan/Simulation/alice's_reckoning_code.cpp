#include <stdio.h>  //не запускается через run; 
#include <stdlib.h>
#include <time.h>
#include <random.h>
using RUB = int;

struct Person{
    RUB cash;

    //INCOME
    RUB salary;

    //EXPENDITURE
    RUB rent;

    //HAVING
    bool having_car;
};

// FUNCTIONS 


    //INIT
void alice_init(Person& alice)
{
    alice.cash = 20'000; 
    
    //income init
    alice.salary = 80'000;

    //expenditure init
    alice.rent = 30'000;

    //having init
    alice.having_car = true;
}


    //PRINT RESULT
void print_output(Person& alice)        
{
    printf("Alice's cash: %d\n", alice.cash); 
}


    //RANDOM FUNCTIONS

void car_breaking(Person& alice) //CAR BREAKING RANDOM
{
    if (d(rng) <= 10) { //10% chance of breaking
        alice.cash -= 50'000;
    }
}


    //INCOME

void alice_salary(Person& alice) //SALARY CALCULATION
{
    alice.cash += alice.salary;
}

    //EXPENDITURE

void alice_rent(Person& alice) //RENT CALCULATION
{
    //rent
    alice.cash -= alice.rent;
}
    
void alice_car(Person& alice) //CAR CALCULATION
{
    if (alice.having_car == true){
        alice.cash -= 10'000;
        car_breaking(alice);
    }
}


// SIMULATION 

void simulation(Person& alice)
{
    char month = 9;
    unsigned short year = 2026;
    while (!(year == 2027 && month == 9)){  //не работает not и and
        
        //Alice's Functions + $$$

        alice_salary(alice);

        //Alice's Functions - $$$

        alice_rent(alice);
        alice_car(alice);
    
        month++;
        if (month == 13){
            year++;
            month = 1;
        }
    }

}

//SIMULATION 

// MAIN 
int main()
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> d(1, 100);
    
    struct Person alice;

    alice_init(alice);
    
    simulation(alice);

    print_output(alice);
}
