#include "log.h"
#include "flat.h"
#include "peter.h"
#include "world.h"
#include "mortage.h"
#include "time.h"

#include <cmath>
#include <cstdarg>

extern Person peter;
extern World world;
extern Mortage mortage;
extern Time time;

FILE *log_file = NULL;
FILE *event_log_file = NULL;

const char *month_name(unsigned int m)
{
    switch (m) {
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


void log_event(const char *format, ...)
{
    if (event_log_file == NULL) return;

    fprintf(event_log_file, "[%02u.%u] ", time.month, time.year);

    va_list arguments;
    va_start(arguments, format);
    vfprintf(event_log_file, format, arguments);
    va_end(arguments);

    fprintf(event_log_file, "\n");
}


void log_finance()
{
    if (log_file == NULL) return;

    fprintf(log_file, "-Финансы\n");

    if (peter.month_pension > 0) {
        fprintf(log_file, "    пенсия: +%llu\n", peter.month_pension);
    }
    else if (peter.month_salary_income > 0) {
        fprintf(log_file, "    зп: +%llu\n", peter.month_salary_income);
    }
    else if (rental_portfolio.month_rent_income == 0) {
        fprintf(log_file, "    доход: 0 (безработный)\n");
    }

    if (rental_portfolio.month_rent_income > 0) {
        fprintf(
            log_file,
            "    доход от аренды: +%llu\n",
            rental_portfolio.month_rent_income
        );
    }

    if (peter.month_expenses_on_food > 0) {
        fprintf(log_file, "    расходы на еду: -%llu\n", peter.month_expenses_on_food);
    }

    if (peter.month_expenses_playing_airsoft > 0) {
        fprintf(log_file, "    расходы на страйкбол: -%llu\n", peter.month_expenses_playing_airsoft);
    }

    if (peter.month_expenses_dating > 0 and peter.girlfriend) {
        fprintf(log_file, "    расходы на свидания с девушкой: -%llu\n", peter.month_expenses_dating);
    }

    if (peter.month_expenses_dating > 0 and peter.married) {
        fprintf(log_file, "    расходы на свидания с женой: -%llu\n", peter.month_expenses_dating);
    }

    if (peter.month_expenses_chids_entertainment > 0) {
        fprintf(log_file, "    расходы на развлечения детей: -%llu\n", peter.month_expenses_chids_entertainment);
    }

    if (peter.month_expenses_birthdays > 0) {
        fprintf(log_file, "    расходы на дни рождения: -%llu\n", peter.month_expenses_birthdays);
    }

    if (peter.month_mortgage_payment > 0) {
        fprintf(log_file, "    личная ипотека: -%llu\n", peter.month_mortgage_payment);
    }

    if (rental_portfolio.month_mortgage_payment > 0) {
        fprintf(
            log_file,
            "    ипотеки арендных квартир: -%llu\n",
            rental_portfolio.month_mortgage_payment
        );
    }

    if (rental_portfolio.month_maintenance_expenses > 0) {
        fprintf(
            log_file,
            "    содержание арендных квартир: -%llu\n",
            rental_portfolio.month_maintenance_expenses
        );
    }

    if (rental_portfolio.month_repair_expenses > 0) {
        fprintf(
            log_file,
            "    ремонт после ущерба: -%llu\n",
            rental_portfolio.month_repair_expenses
        );
    }

    if (rental_portfolio.month_purchase_down_payment > 0) {
        fprintf(
            log_file,
            "    взнос за арендную квартиру: -%llu\n",
            rental_portfolio.month_purchase_down_payment
        );
    }

    if (rental_portfolio.month_early_repayment > 0) {
        fprintf(
            log_file,
            "    досрочное погашение арендной ипотеки: -%llu\n",
            rental_portfolio.month_early_repayment
        );
    }
    
    fprintf(log_file, "    общий расход на месяц: -%llu\n", peter.month_expenses);
    if (peter.month_income >= peter.month_expenses) {
        fprintf(
            log_file,
            "    итог за месяц: +%llu\n",
            peter.month_income - peter.month_expenses
        );
    }
    else {
        fprintf(
            log_file,
            "    итог за месяц: -%llu\n",
            peter.month_expenses - peter.month_income
        );
    }

    if (peter.month_mortgage_paid_off) {
        fprintf(log_file, "    !!! ипотека полностью погашена !!!\n");
    }

    fprintf(log_file, "    наличные: %llu\n", peter.cash);

    if (mortage.principal_amount > 0) {
        fprintf(log_file, "    остаток ипотеки: %llu\n", mortage.principal_amount);
    }

    fprintf(log_file, "    личных комнат в квартире: %u\n", peter.flat);
}


void log_rental_flats()
{
    if (log_file == NULL) return;

    fprintf(log_file, "-Арендные квартиры\n");
    fprintf(log_file, "    в собственности: %zu\n", rental_portfolio.flats.size());
    fprintf(
        log_file,
        "    с жильцами: %u; в поиске жильцов: %zu\n",
        rental_flats_with_tenants(),
        rental_portfolio.flats.size() - rental_flats_with_tenants()
    );
    fprintf(
        log_file,
        "    стоимость портфеля: %llu; остаток долга: %llu\n",
        rental_portfolio_market_value(),
        rental_portfolio_debt()
    );
    fprintf(
        log_file,
        "    аренда за месяц: +%llu; ипотека: -%llu; содержание: -%llu; ремонт: -%llu\n",
        rental_portfolio.month_rent_income,
        rental_portfolio.month_mortgage_payment,
        rental_portfolio.month_maintenance_expenses,
        rental_portfolio.month_repair_expenses
    );
    fprintf(
        log_file,
        "    всего аренды: %llu; ипотеки: %llu; содержание: %llu; ремонты: %llu; досрочно: %llu\n",
        rental_portfolio.total_rent_income,
        rental_portfolio.total_mortgage_payment,
        rental_portfolio.total_maintenance_expenses,
        rental_portfolio.total_repair_expenses,
        rental_portfolio.total_early_repayment
    );
    fprintf(
        log_file,
        "    найдено жильцов: %u; съехало жильцов: %u; случаев ущерба: %u\n",
        rental_portfolio.total_tenants_found,
        rental_portfolio.total_tenants_evicted,
        rental_portfolio.total_damage_cases
    );
    fprintf(
        log_file,
        "    за месяц: найдено %u; выселено %u; ущерб %u\n",
        rental_portfolio.month_tenants_found,
        rental_portfolio.month_tenants_evicted,
        rental_portfolio.month_damage_cases
    );

    for (const RentalFlat &flat : rental_portfolio.flats) {
        fprintf(
            log_file,
            "    №%u: %u м2, %s, аренда %llu, платёж %llu, долг %llu, "
            "жильцов %u, выселений %u, ущербов %u\n",
            flat.id,
            flat.quad_meters,
            flat.tenant ? "жилец" : "поиск жильца",
            flat.monthly_rent,
            flat.mortgage.payment,
            flat.mortgage.principal_amount,
            flat.tenant_changes,
            flat.evictions,
            flat.damage_cases
        );
    }
}


void log_health()
{
    if (log_file == NULL) return;

    fprintf(log_file, "-Здоровье\n");

    fprintf(log_file, "    показатель: %.2f\n", peter.health);

    if (peter.month_disease) {
        fprintf(log_file, "    болезнь: %s (урон %.1f)\n", 
            peter.month_disease_name.c_str(),
            peter.month_disease_damage
        );
    }

    fprintf(log_file, "    простуд за жизнь:        %d\n", peter.count_cold);

    fprintf(log_file, "    ангин за жизнь:          %d\n", peter.count_angina);

    fprintf(log_file, "    переломов за жизнь:      %d\n", peter.count_broken_bone);

    fprintf(log_file, "    инфарктов за жизнь:      %d\n", peter.count_heart_attack);
}


void log_age()
{
    if (log_file == NULL) return;

    fprintf(log_file, "-Возраст\n");

    fprintf(log_file, "    %u лет\n", peter.age);

    if (peter.month_promotion) {
        fprintf( log_file, "    повышение на работе (всего: %u)\n",
            peter.number_of_promotions
        );
    }

    if (peter.month_dismissed){
        fprintf(log_file, "    уволен с работы\n");
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
        "    %d / 120\n",
        peter.mental
    );

    fprintf(log_file, "    приобретено за месяц: +%d\n", peter.month_mental_plus);
    for (const std::string &reason : peter.month_mental_pluses) {
        fprintf(log_file, "        %s\n", reason.c_str());
    }

    fprintf(log_file, "    потеряно за месяц: -%d\n", peter.month_mental_loss);
    for (const std::string &reason : peter.month_mental_losses) {
        fprintf(log_file, "        %s\n", reason.c_str());
    }

    if (peter.month_mental >= 0) {
        fprintf(log_file, "    итог за месяц: +%d\n", peter.month_mental);
    }
    else {
        fprintf(log_file, "    итог за месяц: %d\n", peter.month_mental);
    }

    if (peter.mental >= 80) {
        fprintf(log_file, "    состояние: отличное\n");
    }
    else if (peter.mental >= 60) {
        fprintf(log_file, "    состояние: хорошее\n");
    }
    else if (peter.mental >= 40) {
        fprintf(log_file, "    состояние: нормальное\n");
    }
    else if (peter.mental >= 20) {
        fprintf(log_file, "    состояние: плохое\n");
    }
    else {
        fprintf(log_file, "    состояние: критическое\n");
    }
}


void log_month_header()
{
    if (log_file == NULL) return;

    fprintf(log_file, "\n");

    fprintf(log_file,
        "================= %s %u ===================\n",
        month_name(time.month),
        time.year
    );
}


void log_family()
{
    if (log_file == NULL) return;

    fprintf(log_file, "-Семейный статус\n");

    if (peter.girlfriend){
        fprintf(log_file, "    в отношениях\n");
    }

    if (peter.married){
        fprintf(log_file, "    в браке\n");
    }

    if (peter.childs == 1){
        fprintf(log_file, "    1 ребёнок\n");
    }

    if (peter.childs == 2){
        fprintf(log_file, "    2 детей\n");
    }
}


void log_month_report()
{
    if (log_file == NULL) return;

    log_month_header();
    log_finance();
    log_rental_flats();
    log_health();
    log_age();
    log_family();
    log_mental();
}
