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

#define print_spacer printf("\n\n===================================================\n\n")

#ifdef _WIN32
#include <windows.h>
#endif

Person* person = nullptr;
World* world = nullptr;

void setup_person() {
    person->age = scan_input_in_range(16, 30, "\nВведите возраст от 16 до 30 лет: ");
    char name[50];
    printf("Введите имя: ");
    scanf("%s", name);
    person->name = malloc(strlen(name) + 1);
    strcpy(person->name, name);

    person->money = (int)dice() * (person->age - 15) * 500;
    person->salary = 60000 + (int)dice() * 10000;
    person->mortgage_debt = (int)dice() * 500000;
    person->mortgage_years_left = 10;
    person->mortgage_overdue = 0;
    person->utilities_tariff = 8000 + (int)dice() * 1000;
    person->utilities_debt = 0;
    person->has_cat = dice() >= LUCK_NORMAL;
    person->cat_age = 0;
    person->cat_lifespan = 12 + (int)dice() * 2;
    person->health = 40 + (int)dice() * 15;
    person->mood = (MoodType) dice();

    printf("\nВашего персонажа зовут: %s\nЕму/ей %d лет. С рождения по здоровью он/она был(а) %s\nВ кармане оставалось %d рублей. \
        \nНа момент начала событий его/её настроение: %s", person->name, person->age, get_health_description(person->health), person->money, get_mood_description(person->mood));
}

void setup_world() {
    world->year = scan_input_in_range(2000, 2026, "\nВведите желемый год старта от 2000 до 2026: ");
    
    world->end_year = world->year + scan_input_in_range(1, 30, "\nУкажите какое количество лет вы желаете симулировать (не меньше 1 и не больше 30): ");

    world->events_per_year = scan_input_in_range(1, 10, "\nУкажите желаемое количество событий в год (не меньше 1 и не больше 10): ");

    world->economy = (EconomyStatus) dice();
    world->status = (WorldStatus) dice();

    printf("\nСейчас %d год.\nВаш персонаж проживет в мире еще %d счастливых или не очень лет, и каждый год с ним будет происходить %d событий.\
        \nСам мир находится в %s состоянии.\nЭкономика мира переживает %s", world->year, world->end_year-world->year, world->events_per_year,\
    get_world_status_description(world->status), get_economy_status_description(world->economy));
}

void declare_events(Node* event_list) {
    call_signals(event_list);
}

Event** sorted_events = nullptr; // array
int teenage_offset, youth_offset, middleage_offset, old_offset;
int teenage_amount, youth_amount, middleage_amount, old_amount;
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
            case STAGE_TEENAGE:
                teenage_offset = offset - added;
                teenage_amount = added;
                break;
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
        case STAGE_TEENAGE:
            amount = teenage_amount;
            offset = teenage_offset;
            break;
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
    int pos = offset + (rand() % amount);
    return sorted_events[pos];
}


void print_signed(char* label, int before, int after) {
    if (before == after) {
        return;
    }
    printf("\n    %s: %d -> %d (%+d)", label, before, after, after - before);
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

void print_summary(Person* start, int start_year, int start_world_economy, bool died) {
    print_spacer;
    printf("\nИтоги симуляции: %s, %d - %d годы%s", person->name, start_year, world->year,
        died ? " (закончилась досрочно из-за смерти)" : "");
    printf("\n\nБыло в начале (%d лет):", start->age);
    printf("\n  деньги: %d, зарплата в год: %d, долг по ипотеке: %d, просрочка: %d", start->money, start->salary, start->mortgage_debt, start->mortgage_overdue);
    printf("\n  здоровье: %s, настроение: %s", get_health_description(start->health), get_mood_description(start->mood));
    printf("\n\nСтало в конце (%d лет):", person->age);
    printf("\n  деньги: %d, зарплата в год: %d, долг по ипотеке: %d, просрочка: %d", person->money, person->salary, person->mortgage_debt, person->mortgage_overdue);
    printf("\n  здоровье: %s, настроение: %s", get_health_description(person->health), get_mood_description(person->mood));
    printf("\n\nИзменения: деньги %+d, зарплата %+d, долг по ипотеке %+d, здоровье %+d",
        person->money - start->money, person->salary - start->salary,
        person->mortgage_debt - start->mortgage_debt, person->health - start->health);
    printf("\nЭкономика в начале: %s, в конце: %s",
        get_economy_status_description(start_world_economy), get_economy_status_description(world->economy));

    if (person->money > start->money) {
        printf("\n\nЗа эти годы %s стал(а) богаче на %d рублей.", person->name, person->money - start->money);
    } else if (person->money < start->money) {
        printf("\n\nЗа эти годы %s обеднел(а) на %d рублей.", person->name, start->money - person->money);
    } else {
        printf("\n\nФинансовое положение %s не изменилось.", person->name);
    }
    printf("\nВсего событий произошло: %d.\n", events_happened);
}

void simulate() {
    if(person == nullptr || world == nullptr) {
        return;
    }

    Person start = *person;
    int start_year = world->year;
    int start_world_economy = world->economy;
    bool died = false;

    while(world->year < world->end_year) {
        printf("\n\n--- %d год, возраст %d ---", world->year, person->age);

        Person before_constants = *person;
        apply_constants(person, world);
        print_changes(&before_constants, person);

        for(int event_n = 0; event_n <= world->events_per_year && person->health > 0; event_n++) {
            Event* e = pick_event(get_stage_by_age(person->age));
            if(e->check(person, world)) {
                Person before = *person;
                e->result(person, world);
                print_changes(&before, person);
                events_happened++;
            }
        }

        if(person->health <= 0) {
            died = true;
            print_spacer;
            printf("\nЗдоровье %s полностью исчерпано. В %d году, в возрасте %d лет, жизнь оборвалась.\nСимуляция завершена досрочно (планировалось до %d года).",
                person->name, world->year, person->age, world->end_year);
            break;
        }
        world->year++;
        person->age++;
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