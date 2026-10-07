#include "peter.h"
#include "flat.h"
#include "world.h"
#include "mortgage.h"
#include "time.h"
#include "log.h"
#include <cmath>


Person peter;
World world;
// Изменено: все ипотеки находятся в Person.
Time time;


void simulation()
{
    do
    {
        peter_reset_month_stats();
// Изменено: сначала рассчитываем доход текущего месяца.
        
        peter_dismissial_from_work();
        peter_find_work();
        // Добавлено: повышение влияет на доход и возможность накопить взнос.
        if (!peter.dismissioned and !peter.retired and peter.age < 70) {
            peter_promotion_at_work();
        }
        peter_salary();
        peter_pension();
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

        // Изменено: расходы, действующие кредиты, затем новая покупка.
        peter_month_expenses();
        peter_mortgage();
        if (peter.health > 0.0) {
            checking_readiness();
        }

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

    // Изменено: списки квартир и ипотек уже очищены в peter_init.

    world_init();

    simulation();

    fclose(log_file);
    log_file = NULL;
    fclose(event_log_file);
    event_log_file = NULL;

    return 0;
}
