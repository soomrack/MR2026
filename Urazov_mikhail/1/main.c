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
    strcpy(person->name, name);

    person->money = (int)dice() * (person->age - 15) * 500;
    person->health = (int)dice() * 30;
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


void simulate() {
    if(person == nullptr || world == nullptr) {
        return;
    }

    while(world->year < world->end_year) {
        for(int event_n = 0; event_n <= world->events_per_year; event_n++) {
            Event* e = pick_event(get_stage_by_age(person->age));
            if(e->check(person, world)) {
                e->result(person, world);
            }
        }
        world->year++;
        person->age++;
    }

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