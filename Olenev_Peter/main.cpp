#include "struct_peter.h"
#include "struct_flat.h"
#include "struct_world.h"
#include "struct_mortgage.h"
#include "struct_time.h"
#include "struct_log.h"
#include <cmath>


Person peter;
World world;
Time time;

void peter_die()
{
    fprintf(log_file, "\n");
    fprintf(log_file, "===========================================\n");
    fprintf(log_file, "                 СМЕРТЬ\n");
    fprintf(log_file, "===========================================\n");
    fprintf(log_file, "    причина:    %s\n",
        peter.last_damage_source.c_str());

    fprintf(log_file, "    возраст:    %u лет\n", peter.age);
}

void simulation()
{
    do
    {
        peter_reset_month_stats();
        
        peter_dismissial_from_work();
        peter_find_work();
        peter_promotion_at_work();
        peter_salary();
        peter_pension();
        peter_month_rent_income();
        peter_month_income();
    

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

        peter_agama();
        peter_car();

        peter_taxes();
        peter_insurance();
        peter_month_expenses();
        peter_mortgage();
        checking_readiness();

        peter_month_mental_end();
        log_month_report();

        world_tick();

    } while (peter.health > 0.0);

    peter_die();

}


// ================== MAIN ==================

int main()
{
    log_file = fopen("statistics.txt", "w");
    event_log_file = fopen("events.txt", "w");

    peter_init();
    world_init();

    simulation();

    fclose(log_file);
    log_file = NULL;
    fclose(event_log_file);
    event_log_file = NULL;

    return 0;
}
