#include "peter.h"
#include "world.h"
#include "time.h"
// Исправлено: единое название заголовка ипотеки.
#include "mortgage.h"
#include "log.h"
extern Person peter;
extern World world;
// Изменено: ипотеки хранятся в peter.mortgages.
extern Time time;

#include <random>
#include <algorithm>

// Исправлено: сигнатура соответствует заголовку и строковым литералам.
void peter_add_mental(MP amount, const char *source)
{
    peter.mental += amount;
    // Изменено: положительный коэффициент исключает деление на ноль.
    peter.mental_factor = std::max(1, peter.mental) / 100.0;
    peter.month_mental_plus += amount;
    peter.month_mental += amount;
    peter.month_mental_pluses.push_back(std::string(source) + ": +" + std::to_string(amount));
}


void peter_remove_mental(MP amount, const char *source)
{
    peter.mental -= amount;
    // Изменено: положительный коэффициент исключает деление на ноль.
    peter.mental_factor = std::max(1, peter.mental) / 100.0;
    peter.month_mental_loss += amount;
    peter.month_mental -= amount;
    peter.month_mental_losses.push_back(std::string(source) + ": -" + std::to_string(amount));
}


void peter_damage(double amount, const char *source)
{
    peter.health -= amount;
    peter.month_damage.push_back(std::string(source) + ": -" + std::to_string(amount));
    if (peter.health < 0.0) {
        peter.health = 0.0;
    }
}


void peter_disease_cold()
{
    if (int_number_generator(1, 36 * peter.mental_factor) == 1) {
        peter.count_cold +=1 ;
        peter.month_expenses_on_healing += world.expenses_healing_cold;
        peter_damage(0.1, "простуда");
        log_event("заболел: простуда");
    }
}


void peter_disease_angina()
{
    if (int_number_generator(1, 720 * peter.mental_factor) == 1) {
        peter.count_angina += 1;
        peter.month_expenses_on_healing += world.expenses_healing_angina;
        peter_damage(0.5, "ангина");
        log_event("заболел: ангина");
    }
}


void peter_disease_broken_bone()
{
    if (int_number_generator(1, 1440 * peter.mental_factor) == 1) {
        peter.count_broken_bone += 1;
        peter.month_expenses_on_healing += world.expenses_healing_broken_bone;
        peter_damage(0.3, "перелом кости");
        log_event("получил травму: перелом кости");
    }
}


void peter_disease_caries()
{
    if (int_number_generator(1, 1440 * peter.mental_factor) == 1) {
        peter.count_caries += 1;
        peter.month_expenses_on_healing += world.expenses_healing_caries;
        peter_damage(0.3, "кариес");
        log_event("заболел: кариес");
    }
}


void peter_disease_heart_attack()
{
    if (int_number_generator(1, 7200 * peter.mental_factor) == 1) {
        peter.count_caries += 1;
        peter_damage(100.0, "сердечный приступ");
        log_event("сердечный приступ");
    }
}


void peter_mentality()
{
    // Влияние семьи на менталку
    if (peter.girlfriend == true) {
        peter_add_mental(1, "отношения");
        peter_remove_mental(4, "повседневные заботы в отношениях");
    }

    if (peter.married == true) {
        peter_add_mental(2, "брак");
        peter_remove_mental(5, "домашние обязанности");
    }
    
    unsigned int dependent_children = peter_dependent_children_count();
    if (dependent_children > 0) {
        peter_add_mental(dependent_children, "дети");
        peter_remove_mental(3 * dependent_children, "забота о детях");
    }

    // Влияние работы на менталку
    if (peter.dismissioned and !peter.retired) {
        peter_remove_mental(5, "без работы");
    }

    if (time.month == peter.vacation_month) {
        peter_add_mental(10, "отпуск");
    }

    // Влияние случайных событий не описываемых этой программой
    int mental_event = int_number_generator(1, 100);
    if (mental_event <= 8) {
        peter_add_mental(6, "хорошие новости");
        log_event("хорошие новости");
    }
    else if (mental_event <= 16) {
        peter_remove_mental(6, "стрессовое событие");
        log_event("стрессовое событие");
    }
    else if (mental_event == 17) {
        peter_add_mental(12, "большой успех");
        log_event("большой успех");
    }
    else if (mental_event == 18) {
        peter_remove_mental(12, "серьёзный кризис");
        log_event("серьёзный кризис");
    } 

    peter_remove_mental(1, "течение времени");
    peter_add_mental(3, "личный отдых");

    if (peter.mental <= 0) {
        peter_damage(100.0, "депрессия");
        log_event("депрессия");
    }
}



// Добавлено: завершение месяца соответствует вызову из simulation.
void peter_month_mental_end()
{
    peter.mental = std::min(120, peter.mental);
    peter.mental_factor = std::max(1, peter.mental) / 100.0;
    if (peter.mental <= 0 and peter.health > 0.0) {
        peter.last_damage_source = "депрессия";
        peter_damage(100.0, "депрессия");
        log_event("депрессия");
    }
}
