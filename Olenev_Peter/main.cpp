#include "peter.h"
#include "world.h"
#include "mortage.h"
#include "time.h"
#include "log.h"
#include <cmath>


Person peter;
World world;
Mortage mortage;
Time time;


void simulation()
{
    do
    {
        peter_reset_month_stats();
        peter_month_income();
        peter_expenses();
        peter_health();
        peter_family();

        log_month_report();

        world_tick();

    } while (peter.health > 0.0);

    fprintf(log_file, "\n");

    fprintf(log_file, "===========================================\n");

    fprintf(log_file, "                 СМЕРТЬ\n");

    fprintf(log_file, "===========================================\n");

    fprintf(log_file, "    причина:    %s\n",
        peter.last_damage_source.c_str());

    fprintf(log_file, "    возраст:    %u лет\n", peter.age);
}


// ================== MAIN ==================

int main()
{
    log_file = fopen("statistics.txt", "w");

    if (log_file == NULL)
    {
        printf("Не удалось открыть файл для записи\n");
        return 1;
    }

    peter_init();

    mortage = {};
    mortage.active = false;

    world_init();

    simulation();

    fclose(log_file);
    log_file = NULL;

    return 0;
}