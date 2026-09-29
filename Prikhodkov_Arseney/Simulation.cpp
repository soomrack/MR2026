#include "Simulation.h"
#include <iostream>
#include <random>
#include <string>


std::mt19937 gen(std::random_device{}());

int RandomDisease()
{
    std::vector<double> weights(100);

    weights[0] = 473.89;

    for (int i = 1; i <= 20; i++)
        weights[i] = 9.397-i*0.463;

    for (int i = 21; i <= 60; i++)
        weights[i] = 2.439-(i-21)*0.061;

    for (int i = 61; i <= 80; i++)
        weights[i] = 0.0699-(i-61)*0.0021;

    for (int i = 81; i <= 90; i++)
        weights[i] = 0.0204-(i-81)*0.0016;

    for (int i = 91; i <= 99; i++)
        weights[i] = 0.0164-(i-91)*0.00013;

    std::discrete_distribution<int> dist(weights.begin(), weights.end());

    return dist(gen);
}

Simulation::Simulation(unsigned duration) {
    this->duration = duration;
}

void Simulation::RandomEventsChance(){
    int Disease = RandomDisease();
    if (Disease==0){ std::cout<<"This month you're healthy"<<std::endl;}
    else if (1<=Disease<=20){std::cout<<"Just cold never mind"<<std::endl;}
    else if (21<=Disease<=61){std::cout<<"Well, it's unpleasent, you should take some days off and go to doctor"<<std::endl;}
    else if (61<=Disease<=80){std::cout<<"It's realy seriously! You should go to hospital!"<<std::endl;}
    else if (81<=Disease<=90){std::cout<<"F*ck! It's terrible! May be . . . May be you will die . . ."<<std::endl;}
    else if (91<=Disease<=99){std::cout<<"Have you already choosen your casket? Or you'll make drugs?"<<std::endl;}
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    if (dist(gen) < Disease/100){
        std::cout<<"Game ower you die"<<std::endl;
        duration = 0;
        return;
    }
}

void Simulation::step(){
    Simulation::RandomEventsChance();
}

void Simulation::DesigionMake(){
    return;
}

void Simulation::start() {
    for (unsigned i = 0; i < duration; i++) {
        step();
    }
}