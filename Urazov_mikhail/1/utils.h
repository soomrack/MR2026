#include "types.h"
#include "events/__event.h"

#ifndef UTILS
#define UTILS

Luck dice();

char* get_health_description(int health);
char* get_economy_status_description(EconomyStatus status);
char* get_world_status_description(WorldStatus status);
char* get_mood_description(MoodType mood);
EventStage get_stage_by_age(int age);

int scan_input_in_range(int min, int max, char* msg);

#endif