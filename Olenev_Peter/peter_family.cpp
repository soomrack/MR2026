#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
#include "log.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;
#include <random>



void peter_girlfriend()
{
    if (!peter.married && peter.girlfriend == false and
        int_number_generator(1, 50) == 1 and
        peter.girlfriend_possibility == true)
    {
        peter.girlfriend = true;
        peter.mental += 10;
        peter.girlfriend_possibility = false;
        log_event("начал встречаться");
    }

    if (peter.mental < 30)
    {
        if (peter.girlfriend) {
            log_event("расстался из-за ухудшения ментального состояния");
        }
        peter.girlfriend = false;
        peter.girlfriend_possibility = false;
    }
    else if (!peter.married && peter.mental >= 30 and
             peter.girlfriend == false)
    {
        peter.girlfriend_possibility = true;
    }

    if (peter.girlfriend == true and
        int_number_generator(1, 500) == 1.0)
    {
        peter.girlfriend = false;
        peter.mental -= 10;
        peter.girlfriend_possibility = true;
        peter.girlfriend_time = 0;
        log_event("расстался");
    }

    if (peter.girlfriend == true)
    {
        peter.mental += 1;
        peter.girlfriend_time += 1;
    }
}


void peter_married()
{
    if (!peter.married and
        peter.girlfriend_time > int_number_generator(24, 36) and
        peter.salary >= 80000)
    {
        peter.married = true;
        peter.girlfriend = false;
        peter.girlfriend_possibility = false;
        log_event("вступил в брак");
    }

    if (peter.mental < 30)
    {
        if (peter.married) {
            log_event("развелся из-за ухудшения ментального состояния");
        }
        peter.married = false;
    }

    if (peter.married == true)
    {
        peter.mental += 1;
        peter.married_time += 1;
    }
}


void peter_childrens()
{
    unsigned int ch = peter.childs;

    if (peter.married and peter.age < 40 and peter.childs < 2) {
        if (peter.married_time > int_number_generator(12, 24) and ch == 0) {
            peter.childs += 1;
            log_event("родился ребёнок (всего: %d)", peter.childs);
        }

        // Второй ребёнок появляется только после переезда в трёхкомнатную квартиру.
        if (peter.married_time > int_number_generator(72, 144) and
            ch == 1 and peter.flat >= 3) {
            peter.childs += 1;
            log_event("родился ребёнок (всего: %d)", peter.childs);
        }
    }
}


void peter_grandchildrens()
{
    // пусто
}


void peter_family()
{
    peter_girlfriend();
    peter_married();
    peter_childrens();
}
