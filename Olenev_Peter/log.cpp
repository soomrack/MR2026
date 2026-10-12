#include "log.h"
#include "flat.h"
#include "peter.h"
#include "world.h"
#include "mortgage.h"
#include "time.h"

#include <cmath>
#include <cstdarg>

extern Person peter;
extern World world;
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
    else {
        fprintf(log_file, "    доход от работы: 0 (безработный)\n");
    }
    fprintf(log_file, "    аренда: +%llu\n", peter.month_rent_income);

    fprintf(log_file, "    НДФЛ с зарплаты: -%llu\n", peter.month_salary_tax);
    if (peter.month_rent_tax > 0) {
        fprintf(log_file, "    НДФЛ с аренды за прошлый год: -%llu\n", peter.month_rent_tax);
    }
    fprintf(log_file, "    транспортный налог (доля за месяц): -%llu\n", peter.month_transport_tax);
    if (peter.month_flat_insurance > 0) {
        fprintf(log_file, "    страховка квартир на год: -%llu\n", peter.month_flat_insurance);
    }
    if (peter.month_car_insurance > 0) {
        fprintf(log_file, "    страховка машины на год: -%llu\n", peter.month_car_insurance);
    }

    if (peter.month_agama_died)
        fprintf(log_file, "    бородатая агама умерла\n");
    if (peter.month_car_purchase > 0)
        fprintf(log_file, "    %s: -%llu\n", peter.month_car_replaced ? "заменил машину" : "купил машину", peter.month_car_purchase);

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

    fprintf(log_file, "    первоначальные взносы: -%llu\n", peter.month_down_payment);
    fprintf(log_file, "    помощь родителей: +%llu\n", peter.month_parent_help);

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

    RUB debt = 0;
    for (const Mortgage &mortgage : peter.mortgages) {
        debt += mortgage.principal_amount;
    }
    fprintf(log_file, "    остаток всех ипотек: %llu\n", debt);

    fprintf(log_file, "    личных комнат в квартире: %u\n", peter.flat);
}


void log_flats()
{
    if (log_file == NULL) return;
    fprintf(log_file, "-Квартиры\n");
    fprintf(log_file, "    в собственности: %zu\n", peter.flats.size());
    for (std::size_t i = 0; i < peter.flats.size(); i++) {
        const Flat &flat = peter.flats[i];
        fprintf(log_file, "    №%zu: %u-комн., %u м2, стоимость %llu\n",
            i + 1, flat.room_count, flat.quad_meters, flat.cost);
        if (flat.used_by_peter) {
            fprintf(log_file, "        живёт Пётр\n");
        }
        else if (flat.tenant_stay_months > 0) {
            fprintf(log_file, "        сдана, плата %llu в месяц, срок съёмщиков: %u мес.\n",
                flat.rent, flat.tenant_stay_months);
        }
        else {
            fprintf(log_file, "        поиск съёмщиков\n");
        }
    }
}


void log_health()
{
    if (log_file == NULL) return;

    fprintf(log_file, "-Здоровье\n");

    fprintf(log_file, "    показатель: %.2f\n", peter.health);

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
    log_flats();
    log_health();
    log_age();
    log_family();
    log_mental();
}
