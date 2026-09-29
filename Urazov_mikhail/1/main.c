#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// #define DEBUG

#include "events/__event.h"
#include "types.h"
#include "signal.h"
#include "linked_list.h"
#include "utils.h"
#include "constants.h"
#include "kredit.h"

#define ZARPLATA_MIN_V_MESYATS 80000
#define ZARPLATA_SHAG_V_MESYATS 55000
#define ZARPLATA_RAZBROS 10000

#define print_spacer printf("\n\n===================================================\n\n")

#ifdef _WIN32
#include <windows.h>
#endif

Person* person = nullptr;
World* world = nullptr;

void print_predlozhenie_ipoteki() {
    Ipoteka* m = &person->ipoteka;
    printf("\n\nБанк одобрил ипотеку: квартира стоит %s рублей, первоначальный взнос %s рублей (из накоплений), кредит %s рублей на %d лет под %.1f%% годовых.",
        format_money(m->price), format_money(m->pervonachalny_vznos), format_money(m->summa_kredita), world->ipoteka_let, m->stavka);
    printf("\nЕжемесячный платеж: %s рублей при чистом доходе %s рублей в месяц.", format_money(m->platezh), format_money(kredit_dohod_v_mesyats(person)));
}

void setup_person() {
    person->age = scan_input_in_range(21, 45, "\nВведите возраст от 21 до 45 лет: ");
    char name[50];
    printf("Введите имя: ");
    scanf("%s", name);
    person->name = malloc(strlen(name) + 1);
    strcpy(person->name, name);

    person->salary = (ZARPLATA_MIN_V_MESYATS + (int)dice() * ZARPLATA_SHAG_V_MESYATS + rand() % (2 * ZARPLATA_RAZBROS + 1) - ZARPLATA_RAZBROS) * MONTHS_IN_YEAR;
    kredit_vydat(person, world);
    person->money = 2 * kredit_dohod_v_mesyats(person) + (int)dice() * (person->age - 15) * 50000;
    person->nalog_uplachen_za_god = 0;
    person->nalog_uplachen_v_proshlom_godu = 0;
    person->utilities_tariff = 36000 + (int)dice() * 6000;
    person->utilities_debt = 0;
    person->has_cat = dice() >= LUCK_NORMAL;
    person->cat_age = 0;
    person->cat_lifespan = 12 + (int)dice() * 2;
    person->health = 40 + (int)dice() * 15;
    person->mood = (MoodType) dice();

    printf("\nВашего персонажа зовут: %s\nЕму/ей %d лет. С рождения по здоровью он/она был(а) %s\nПосле покупки квартиры в кармане осталось %s рублей, зарплата %s рублей в год. \
        \nНа момент начала событий его/её настроение: %s", person->name, person->age, get_health_description(person->health), format_money(person->money), format_money(person->salary), get_mood_description(person->mood));
    print_predlozhenie_ipoteki();
}

void setup_world() {
    world->year = scan_input_in_range(2000, 2026, "\nВведите желемый год старта от 2000 до 2026: ");

    world->ipoteka_let = scan_input_in_range(10, 30, "\nУкажите срок ипотеки в годах (от 10 до 30), симуляция продлится до ее полной выплаты: ");
    world->end_year = world->year + world->ipoteka_let + IPOTEKA_DOP_LET;

    world->dosrochnoe_vklyucheno = scan_input_in_range(0, 1, "\nДосрочно гасить ипотеку из излишков денег каждый декабрь? (1 - да, 0 - нет): ");

    world->events_per_year = scan_input_in_range(1, 10, "\nУкажите желаемое количество событий в год (не меньше 1 и не больше 10): ");

    world->month = 1;
    world->economy = (EconomyStatus) dice();
    world->status = (WorldStatus) dice();

    printf("\nСейчас %d год.\nВаш персонаж будет выплачивать ипотеку %d лет (или пока она не закончится раньше), и каждый год с ним будет происходить в среднем %d событий.\
        \nДосрочное погашение: %s.\nСам мир находится в %s состоянии.\nЭкономика мира переживает %s", world->year, world->ipoteka_let, world->events_per_year,
        world->dosrochnoe_vklyucheno ? "включено" : "выключено",
        get_world_status_description(world->status), get_economy_status_description(world->economy));
}

void declare_events(Node* event_list) {
    call_signals(event_list);
}

Event** sorted_events = nullptr; // array
int youth_offset, middleage_offset, old_offset;
int youth_amount, middleage_amount, old_amount;
int event_amount;

void sort_events_by_stage(Node* event_list) {
    event_amount = len(event_list);

    sorted_events = malloc(sizeof(Event)*event_amount);
    int offset = 0;
    int added = 0;
    for(int i = 0; i <= STAGE_OLD; i++) { // ehhhh
        added = 0;
        for(int j = 0; j < event_amount; j++) {
            Event* e = get(event_list, j);
            if(e->stage == i) {
                sorted_events[offset] = e;
                offset += 1;
                added += 1;
            }
        }
        switch (i) {
            case STAGE_YOUTH:
                youth_offset = offset - added;
                youth_amount = added;
                break;
            case STAGE_MIDDLEAGE:
                middleage_offset = offset - added;
                middleage_amount = added;
                break;
            case STAGE_OLD:
                old_offset = offset - added;
                old_amount = added;
                break;
        }
    }
}

int events_happened = 0;

Event* pick_event(EventStage stage) {
    int offset;
    int amount;
    switch (stage){
        case STAGE_YOUTH:
            amount = youth_amount;
            offset = youth_offset;
            break;
        case STAGE_MIDDLEAGE:
            amount = middleage_amount;
            offset = middleage_offset;
            break;
        case STAGE_OLD:
            amount = old_amount;
            offset = old_offset;
            break;
    }
    int pos = rand() % (offset + amount);
    return sorted_events[pos];
}


void print_signed(char* label, int before, int after) {
    if (before == after) {
        return;
    }
    if (strcmp(label, "здоровье") == 0) {
        printf("\n    %s: %d -> %d (%+d)", label, before, after, after - before);
        return;
    }
    printf("\n    %s: %s -> %s (%s)", label, format_money(before), format_money(after), format_money_signed(after - before));
}

void print_changes(Person* before, Person* after) {
    print_signed("деньги", before->money, after->money);
    print_signed("зарплата в год", before->salary, after->salary);
    print_signed("коммуналка в год", before->utilities_tariff, after->utilities_tariff);
    print_signed("здоровье", before->health, after->health);
    if (before->mood != after->mood) {
        printf("\n    настроение: %s -> %s", get_mood_description(before->mood), get_mood_description(after->mood));
    }
}

void print_year_report(Person* year_start) {
    Ipoteka* m = &person->ipoteka;
    printf("\n\nИтоги года:");
    print_changes(year_start, person);
    if (m->active) {
        printf("\n    ипотека: остаток долга %s, платеж %s в месяц, ставка %.1f%%, осталось платить %d мес.",
            format_money(m->ostatok_dolga), format_money(m->platezh), m->stavka, m->mesyatsev_ostalos);
        printf("\n    процентов банку за год: %s, просрочка: %s, рыночная цена квартиры: %s", format_money(m->protsenty_za_god), format_money(m->prosrochka), format_money(m->home_price));
    }
}

void print_itogi_ipoteki() {
    Ipoteka* m = &person->ipoteka;
    int protsenty_i_sbory = m->protsenty_vyplacheno + m->shtrafy + m->strahovka_vyplacheno;
    int pereplata = protsenty_i_sbory - m->vychety_polucheno;

    printf("\n\nИпотека подробно:");
    printf("\n  квартира: %s рублей, первоначальный взнос: %s, кредит: %s, ставка при выдаче: %.1f%%", format_money(m->price), format_money(m->pervonachalny_vznos), format_money(m->summa_kredita), m->nachalnaya_stavka);
    printf("\n  платежей банку внесено: %s (из них досрочно: %s), проценты: %s, штрафы: %s, страховка: %s", format_money(m->vsego_vyplacheno), format_money(m->dosrochno_vneseno), format_money(m->protsenty_vyplacheno), format_money(m->shtrafy), format_money(m->strahovka_vyplacheno));
    printf("\n  налоговых вычетов возвращено: %s, рефинансирований: %d", format_money(m->vychety_polucheno), m->chislo_refinansirovaniy);
    printf("\n  выплачивалась %d мес. из планировавшихся %d", m->mesyatsev_oplacheno, m->srok_mesyatsev);
    printf("\n  реальная переплата (проценты, штрафы, страховка минус вычеты): %s рублей, это %d%% от цены квартиры", format_money(pereplata), (int)((long long)pereplata * 100 / m->price));
    if (m->pogashena) {
        printf("\n  ИПОТЕКА ПОЛНОСТЬЮ ВЫПЛАЧЕНА. Рыночная цена квартиры сейчас %s, она изменилась на %s относительно покупки.", format_money(m->home_price), format_money_signed(m->home_price - m->price));
    } else if (m->kvartira_izyata) {
        printf("\n  КВАРТИРА ОТОБРАНА ЗА ДОЛГИ и продана за %s рублей.", format_money(m->tsena_prodazhi));
    } else {
        printf("\n  ипотека не выплачена: остаток долга %s, просрочка %s, квартира стоит %s, собственный капитал в ней %s.",
            format_money(m->ostatok_dolga), format_money(m->prosrochka), format_money(m->home_price), format_money(m->home_price - kredit_dolg(person)));
    }
}

void print_summary(Person* start, int start_year, int start_world_economy, bool died) {
    print_spacer;
    printf("\nИтоги симуляции: %s, %d - %d годы%s", person->name, start_year, world->year,
        died ? " (закончилась досрочно из-за смерти)" : "");
    printf("\n\nБыло в начале (%d лет):", start->age);
    printf("\n  деньги: %s, зарплата в год: %s, долг по ипотеке: %s", format_money(start->money), format_money(start->salary), format_money(kredit_dolg(start)));
    printf("\n  здоровье: %s, настроение: %s", get_health_description(start->health), get_mood_description(start->mood));
    printf("\n\nСтало в конце (%d лет):", person->age);
    printf("\n  деньги: %s, зарплата в год: %s, долг по ипотеке: %s", format_money(person->money), format_money(person->salary), format_money(kredit_dolg(person)));
    printf("\n  здоровье: %s, настроение: %s", get_health_description(person->health), get_mood_description(person->mood));
    printf("\n\nИзменения: деньги %s, зарплата %s, долг по ипотеке %s, здоровье %+d",
        format_money_signed(person->money - start->money), format_money_signed(person->salary - start->salary),
        format_money_signed(kredit_dolg(person) - kredit_dolg(start)), person->health - start->health);
    printf("\nЭкономика в начале: %s, в конце: %s",
        get_economy_status_description(start_world_economy), get_economy_status_description(world->economy));
    print_itogi_ipoteki();

    int end_wealth = person->money + (person->ipoteka.kvartira_izyata ? 0 : person->ipoteka.home_price) - kredit_dolg(person);
    printf("\n\nЧистое имущество (деньги + квартира - долги): в начале %s (деньги %s + квартира %s - долг %s), в конце %s.",
        format_money(start->money + start->ipoteka.price - kredit_dolg(start)), format_money(start->money), format_money(start->ipoteka.price), format_money(kredit_dolg(start)), format_money(end_wealth));
    printf("\nВсего событий произошло: %d.\n", events_happened);
}

void simulate() {
    if(person == nullptr || world == nullptr) {
        return;
    }

    Person start = *person;
    Person year_start = *person;
    int start_year = world->year;
    int start_world_economy = world->economy;
    bool died = false;

    while(world->year < world->end_year) {
        if(world->month == 1) {
            printf("\n\n--- %d год, возраст %d ---", world->year, person->age);
            year_start = *person;
        }

        apply_constants(person, world);

        for(int event_n = 0; event_n < world->events_per_year && person->health > 0; event_n++) {
            if(rand() % MONTHS_IN_YEAR != 0) {
                continue;
            }
            Event* e = pick_event(get_stage_by_age(person->age));
            if(e->check(person, world)) {
                Person before = *person;
                printf("\n[месяц %d]", world->month);
                e->result(person, world);
                print_changes(&before, person);
                events_happened++;
            }
        }

        if(person->health <= 0) {
            died = true;
            print_spacer;
            printf("\nЗдоровье %s полностью исчерпано. В %d году, в возрасте %d лет, жизнь оборвалась.\nСимуляция завершена досрочно.",
                person->name, world->year, person->age);
            break;
        }
        if(!person->ipoteka.active) {
            print_spacer;
            printf("\nСимуляция завершена: %d год, месяц %d.", world->year, world->month);
            break;
        }

        if(world->month == MONTHS_IN_YEAR) {
            print_year_report(&year_start);
            person->ipoteka.protsenty_za_god = 0;
            world->month = 1;
            world->year++;
            person->age++;
        } else {
            world->month++;
        }
    }

    print_summary(&start, start_year, start_world_economy, died);
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    srand(time(NULL));

    world = malloc(sizeof(World));
    person = malloc(sizeof(Person));

    Node* event_list = create_list();

    declare_events(event_list);

    sort_events_by_stage(event_list);
    delete_list(event_list);

    print_spacer;
    printf("\nДобро пожаловать в симулятор жизни! Для начала вам требуется настроить игровой мир...");
    setup_world();

    print_spacer;
    printf("\nОтлично! Теперь настройте своего персонажа...");
    setup_person();

    print_spacer;
    printf("\nЗамечательно, все готово для симуляции! Приступаем...");
    simulate();

    return 0;
}