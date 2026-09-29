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

typedef struct Ipoteka {
    int active;
    int pogashena;
    int kvartira_izyata;
    int price;
    int pervonachalny_vznos;
    int home_price;
    int summa_kredita;
    int ostatok_dolga;
    int platezh;
    int srok_mesyatsev;
    int mesyatsev_ostalos;
    int mesyatsev_oplacheno;
    double nachalnaya_stavka;
    double stavka;
    int prosrochka;
    int propuskov_podryad;
    int protsenty_vyplacheno;
    int protsenty_za_god;
    int shtrafy;
    int dosrochno_vneseno;
    int vsego_vyplacheno;
    int strahovka_vyplacheno;
    int vychety_polucheno;
    int vychet_ostalos;
    int limit_protsentov_ostalos;
    int protsenty_dlya_vycheta;
    int chislo_refinansirovaniy;
    int tsena_prodazhi;
} Ipoteka;

typedef struct World {
    int year;
    int end_year;
    int month;
    int ipoteka_let;
    int dosrochnoe_vklyucheno;
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
    Ipoteka ipoteka;
    int nalog_uplachen_za_god;
    int nalog_uplachen_v_proshlom_godu;
    int utilities_tariff;
    int utilities_debt;
    int has_cat;
    int cat_age;
    int cat_lifespan;
    int health;
    MoodType mood;
} Person;

#endif