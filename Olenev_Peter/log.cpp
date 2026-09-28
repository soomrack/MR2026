#include "log.h"
#include "peter.h"
#include "world.h"
#include "mortage.h"
#include "time.h"

#include <cmath>

extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;

FILE *log_file = NULL;

const char *month_name(unsigned int m)
{
    switch (m)
    {
        case 1:  return "январь";
        case 2:  return "февраль";
        case 3:  return "март";
        case 4:  return "апрель";
        case 5:  return "май";
        case 6:  return "июнь";
        case 7:  return "июль";
        case 8:  return "август";
        case 9:  return "сентябрь";
        case 10: return "октябрь";
        case 11: return "ноябрь";
        case 12: return "декабрь";
    }

    return "";
}

void log_finance()
{
    if (log_file == NULL) return;

    fprintf(log_file, "-Финансы\n");

    if (peter.month_income > 0)
    {
        fprintf(
            log_file,
            "    зп: +%llu\n",
            peter.month_income
        );
    }
    else
    {
        fprintf(
            log_file,
            "    зп: 0 (безработный)\n"
        );
    }

    if (peter.month_mortgage_payment > 0)
    {
        fprintf(
            log_file,
            "    списание по ипотеке: -%llu\n",
            peter.month_mortgage_payment
        );
    }

    if (peter.month_mortgage_paid_off)
    {
        fprintf(
            log_file,
            "    !!! ипотека полностью погашена !!!\n"
        );
    }

    fprintf(
        log_file,
        "    наличные: %llu\n",
        peter.cash
    );

    if (mortage.principal_amount > 0)
    {
        fprintf(
            log_file,
            "    остаток ипотеки: %llu\n",
            mortage.principal_amount
        );
    }

    fprintf(
        log_file,
        "    квартир: %u\n",
        peter.flat
    );
}

void log_health()
{
    if (log_file == NULL) return;

    fprintf(log_file, "-Здоровье\n");

    fprintf(
        log_file,
        "    показатель: %.2f\n",
        peter.health
    );

    if (peter.month_disease)
    {
        fprintf(
            log_file,
            "    болезнь: %s (урон %.1f)\n",
            peter.month_disease_name.c_str(),
            peter.month_disease_damage
        );
    }

    fprintf(
        log_file,
        "    простуд за жизнь:        %d\n",
        peter.count_cold
    );

    fprintf(
        log_file,
        "    ангин за жизнь:          %d\n",
        peter.count_angina
    );

    fprintf(
        log_file,
        "    переломов за жизнь:      %d\n",
        peter.count_broken_bone
    );

    fprintf(
        log_file,
        "    инфарктов за жизнь:      %d\n",
        peter.count_heart_attack
    );
}

void log_age()
{
    if (log_file == NULL) return;

    fprintf(log_file, "-Возраст\n");

    fprintf(
        log_file,
        "    %u лет\n",
        peter.age
    );

    if (peter.month_promotion)
    {
        fprintf(
            log_file,
            "    повышение на работе (всего: %u)\n",
            peter.number_of_promotions
        );
    }

    if (peter.month_dismissed)
    {
        fprintf(
            log_file,
            "    уволен с работы\n"
        );
    }
}

void log_mental()
{
    if (log_file == NULL) return;

    fprintf(
        log_file,
        "-Ментальное состояние\n"
    );

    fprintf(
        log_file,
        "    %d / 100\n",
        peter.mental
    );

    if (peter.mental >= 80)
    {
        fprintf(log_file, "    состояние: отличное\n");
    }
    else if (peter.mental >= 60)
    {
        fprintf(log_file, "    состояние: хорошее\n");
    }
    else if (peter.mental >= 40)
    {
        fprintf(log_file, "    состояние: нормальное\n");
    }
    else if (peter.mental >= 20)
    {
        fprintf(log_file, "    состояние: плохое\n");
    }
    else
    {
        fprintf(log_file, "    состояние: критическое\n");
    }
}

void log_month_header()
{
    if (log_file == NULL) return;

    fprintf(log_file, "\n");

    fprintf(
        log_file,
        "================= %s %u ===================\n",
        month_name(time.month),
        time.year
    );
}

void log_month_report()
{
    if (log_file == NULL) return;

    log_month_header();
    log_finance();
    log_health();
    log_age();
    log_mental();
}