#pragma once

class Simulation{
    public:
        unsigned duration;

        Simulation(unsigned duration);

        void RandomEventsChance();
        void step();
        void DesigionMake();
        void start();
};