#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;
#include <random>



void peter_girlfriend()
{
    if (peter.girlfriend == false and
        number_generator(1, 50) == 1 and
        peter.girlfriend_possibility == true)
    {
        peter.girlfriend = true;
        peter.mental += 10;
        peter.girlfriend_possibility = false;
    }

    if (peter.mental < 30)
    {
        peter.girlfriend = false;
        peter.girlfriend_possibility = false;
    }
    else if (peter.mental >= 30 and
             peter.girlfriend == false)
    {
        peter.girlfriend_possibility = true;
    }

    if (peter.girlfriend == true and
        number_generator(1, 500) == 1)
    {
        peter.girlfriend = false;
        peter.mental -= 10;
        peter.girlfriend_possibility = true;
        peter.girlfriend_time = 0;
    }

    if (peter.girlfriend == true)
    {
        peter.mental += 1;
        peter.girlfriend_time += 1;
    }
}


void peter_married()
{
    if (peter.girlfriend_time > number_generator(24, 36) and
        peter.salary >= 80000)
    {
        peter.married = true;
        peter.girlfriend = false;
        peter.girlfriend_possibility = false;
    }

    if (peter.mental < 30)
    {
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

    if (peter.married and peter.age < 40)
    {
        if (peter.married_time >
            (unsigned)number_generator(
                12 * (ch + 1),
                24 * (ch + 1)
            ))
        {
            peter.childs += 1;
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

