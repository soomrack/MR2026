#ifndef TYPES
#define TYPES

typedef enum Luck {
    LUCK_CRITICAL,
    LUCK_BAD,
    LUCK_NORMAL,
    LUCK_GOOD,
    LUCK_PERFECT
} Luck;

typedef enum MoodType {
    MOOD_AWFUL,
    MOOD_BAD,
    MOOD_NORMAL,
    MOOD_GOOD,
    MOOD_PERFECT
} MoodType;

typedef enum WorldStatus {
    STATUS_WAR,
    STATUS_CRISIS,
    STATUS_NORMAL,
    STATUS_RISE,
    STATUS_PERFECT_WORLD
} WorldStatus;

typedef enum EconomyStatus {
    ESTATUS_DEFOLT,
    ESTATUS_CRISIS,
    ESTATUS_NORMAL,
    ESTATUS_RISE,
    ESTATUS_PERFECT
} EconomyStatus;

typedef struct World {
    int year;
    int end_year;
    int events_per_year;
    EconomyStatus economy;
    WorldStatus status;
} World;

typedef struct Person {
    char* name;
    int age;
    Luck luck;
    int money;
    int salary;
    int mortgage_debt;
    int mortgage_years_left;
    int mortgage_overdue;
    int utilities_tariff;
    int utilities_debt;
    int has_cat;
    int cat_age;
    int cat_lifespan;
    int health;
    MoodType mood;
} Person;

#endif