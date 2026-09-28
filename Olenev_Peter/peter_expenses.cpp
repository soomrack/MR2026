#include "peter.h"
#include "world.h"
#include "time.h"
#include "mortage.h"
extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;
#include <random>


void peter_food()
{
    // пусто
}


void peter_expenses()
{
    peter_mortage_readiness();
    peter_food();
}

