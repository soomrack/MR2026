#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "events/__event.h"

Luck dice() {
    return (Luck) rand() % 5;
}

char* get_health_description(int health) {
    if(health > 90) {
        return "здоров как бык";
    } else if (health > 60) {
        return "средненький по здоровью";
    } else if (health > 40) {
        return "часто болеющий";
    } else {
        return "совсем хиленький и слабенький";
    }
}

char* get_economy_status_description(EconomyStatus status) {
    char* returns;
    switch (status)
    {
    case ESTATUS_DEFOLT:
        returns = "дефолт";
        break;
    case ESTATUS_CRISIS:
        returns = "падение";
        break;
    case ESTATUS_NORMAL:
        returns = "нормальное состояние";
        break;
    case ESTATUS_RISE:
        returns = "рост";
        break;
    case ESTATUS_PERFECT:
        returns = "лучшие времени";
        break;
    }
    return returns;
}


char* get_world_status_description(WorldStatus status) {
    char* returns;
    switch (status)
    {
    case STATUS_WAR:
        returns = "наихудшем";
        break;
    case STATUS_CRISIS:
        returns = "кризисном";
        break;
    case STATUS_NORMAL:
        returns = "нормальном";
        break;
    case STATUS_RISE:
        returns = "растущем";
        break;
    case STATUS_PERFECT_WORLD:
        returns = "лучшем";
        break;
    }
    return returns;
}

char* get_mood_description(MoodType mood) {
    char* returns;
    switch (mood)
    {
    case MOOD_AWFUL:
        returns = "отвратительное";
        break;
    case MOOD_BAD:
        returns = "плоховатое";
        break;
    case MOOD_NORMAL:
        returns = "обычное";
        break;
    case MOOD_GOOD:
        returns = "приподнятное";
        break;
    case MOOD_PERFECT:
        returns = "отличное";
        break;
    }
    return returns;
}


EventStage get_stage_by_age(int age) {
    if (age < 20) {
        return STAGE_TEENAGE;
    } else if (age < 30) {
        return STAGE_YOUTH;
    } else if (age < 50) {
        return STAGE_MIDDLEAGE;
    } else {
        return STAGE_OLD;
    }
}

int scan_input_in_range(int min, int max, char* msg) {
    bool runflag = true;
    int dest = 0;
    while (runflag) {
        printf(msg);
        scanf("%d", &dest);
        if (dest < min || dest > max) {
            printf("Неверно!");
            dest = min-1;
            continue;
        }
        runflag = false;
    }
    return dest;
}