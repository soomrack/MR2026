#include "log.h"
#include "mortgage.h"
#include "peter.h"
#include "time.h"
#include "world.h"
extern Person peter;
extern World world;
extern Time time;


void peter_month_rent_income()
{
    for (std::size_t i = 0; i < peter.flats.size(); ++i) {
        Flat &flat = peter.flats[i];
        if (flat.used_by_peter) {
            continue;
        }

        if (flat.tenant_stay_months > 0) {
            --flat.tenant_stay_months;
        
            if (flat.tenant_stay_months == 0 or int_number_generator(1, 100) <= 2) {

                flat.tenant_stay_months = 0;
                flat.tenant_search_months = 0;
                peter_remove_mental(5, "съезд съёмщиков");
                continue;
            }
        }
        else {
            if (flat.tenant_search_months == 0) {
                flat.tenant_search_months = int_number_generator(
                    world.rental_min_tenant_search_months,
                    world.rental_max_tenant_search_months);
            }

            --flat.tenant_search_months;
            if (flat.tenant_search_months > 0) {
                peter_remove_mental(1, "нет съёмщиков");
                continue;
            }

            flat.tenant_stay_months = int_number_generator(
                world.rental_min_tenant_stay_months,
                world.rental_max_tenant_stay_months);
            peter_add_mental(5, "найдены съёмщики");
            log_event("квартира №%zu: найдены съёмщики на %u мес.", i + 1, flat.tenant_stay_months);
        }

        peter.month_rent_income += flat.rent;
        peter_add_mental(1, "получена арендная плата");
    }
}
