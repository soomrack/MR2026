#include "peter.h"
#include "flat.h"
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
        peter_personal_flat();
        peter_investment_flats();

        peter_food();
        peter_entertainment();

        peter_mentality();
        peter_disease_cold();
        peter_disease_angina();
        peter_disease_broken_bone();
        peter_disease_heart_attack();
        peter_disease_caries();

        peter_girlfriend();
        peter_married();
        peter_childrens();

        peter_month_expenses();

        peter_month_mental_end();
        log_month_report();

        world_tick();

    } while (peter.health > 0.0);

    log_event("смерть; причина: %s; возраст: %u лет",
        peter.last_damage_source.c_str(), peter.age);

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
    event_log_file = fopen("events.txt", "w");

    if (log_file == NULL) {
        printf("Не удалось открыть файл для записи\n");
        return 1;
    }

    if (event_log_file == NULL) {
        printf("Не удалось открыть файл журнала событий\n");
        fclose(log_file);
        return 1;
    }

    peter_init();

    mortage = {};
    mortage.active = false;
    rental_portfolio_init();

    world_init();

    simulation();

    fclose(log_file);
    log_file = NULL;
    fclose(event_log_file);
    event_log_file = NULL;

    return 0;
}
