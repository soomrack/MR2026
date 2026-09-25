#include <stdio.h>
#include <random>
#include <cmath>

using RUB = unsigned long long int;
using YEARS = unsigned int;
using PERCENT = unsigned int;

FILE* log_file = NULL;

// ================== СТРУКТУРЫ ==================

struct Person{

    // Здоровье, возраст, менталка
    YEARS age;
    double health;
    unsigned int mental;
    int count_cold;
    int count_angina;
    int count_broken_bone;
    int count_heart_attack;
    bool month_disease;
    const char* last_damage_source;
    const char* month_disease_name;
    double month_disease_damage;

    // Работа, зарплата
    RUB cash;
    RUB salary;
    RUB base_salary;
    RUB month_income;
    unsigned int number_of_promotions;
    bool month_promotion;
    bool dismission;
    unsigned int dismissions_count;

    // Расходы
    RUB month_mortgage_payment;
    RUB month_expenses;
    RUB expenses_on_healing;
    bool month_dismissed;
    bool month_mortgage_paid_off;

    // Семья
    bool girlfriend;
    bool girlfriend_possibility;
    bool married;
    unsigned int girlfriend_time;
    unsigned int married_time;
    int childs;
    bool wife;

    // Имущество
    bool car;
    bool flat;
};

struct World{
    PERCENT min_inflation;
    PERCENT max_inflation;
    PERCENT inflation;
    RUB base_month_expenses;
    RUB cost_per_quad_meter;
    PERCENT cost_per_quad_meter_grow;
    PERCENT min_cost_per_quad_meter_grow;
    PERCENT max_cost_per_quad_meter_grow;  
};

struct Mortage{
    RUB debt;
    RUB payment;
    RUB down_payment;
    RUB principal_amount;
    unsigned int month;
    double interest_rate;
};

struct Time{
    unsigned int month;
    unsigned int year;
};

struct Person peter;
struct Mortage mortage;
struct Time time;
struct World world;


// ================== ИНИЦИАЛИЗАЦИЯ ==================

void peter_init()
{
    // Здоровье, возраст, менталка
    peter.age = 21;
    peter.mental = 100;
    peter.health = 60.0;
    peter.count_cold = 0;
    peter.count_angina = 0;
    peter.count_broken_bone = 0;
    peter.count_heart_attack = 0;
    peter.month_disease = false;
    peter.month_disease_name = "";
    peter.month_disease_damage = 0.0;
    peter.last_damage_source = "старость";

    // Работа, зарплата
    peter.cash = 0;
    peter.salary = 40000;
    peter.base_salary = 40000;
    peter.month_income = 0;
    peter.number_of_promotions = 0;
    peter.month_promotion = false;
    peter.dismission = false;
    peter.dismissions_count = 0;

    // Расходы
    peter.month_mortgage_payment = 0;
    peter.month_expenses = 0;
    peter.month_dismissed = false;
    peter.month_mortgage_paid_off = false;
    peter.expenses_on_healing = 0;


    // Семья
    peter.girlfriend = false;
    peter.girlfriend_possibility = true;
    peter.married = false;
    peter.girlfriend_time = 0;
    peter.married_time=0;
}


void peter_reset_month_stats()
{
    peter.month_income = 0;
    peter.month_mortgage_payment = 0;
    peter.month_expenses = 0;
    peter.month_promotion = false;
    peter.month_dismissed = false;
    peter.month_disease = false;
    peter.month_mortgage_paid_off = false;
    peter.month_disease_name = "";
    peter.month_disease_damage = 0.0;
}


void mortage_init()
{
    mortage.debt = 6500000;
    mortage.down_payment = 2500000;
    mortage.principal_amount = mortage.debt - mortage.down_payment;

    mortage.interest_rate = 0.155 / 12;
    mortage.month = 12 * 10;

    RUB K = mortage.principal_amount;
    double r = mortage.interest_rate;
    double t = std::pow(1.0 + r, mortage.month);
    mortage.payment = static_cast<RUB>((K) * (r * t) / (t - 1));
}


void time_init()
{
    time.year = 2027;
    time.month = 1;
}


void world_init()
{
    world.min_inflation = 4;
    world.max_inflation = 10;
    world.inflation = 7;
    world.base_month_expenses = 1000;
    world.cost_per_quad_meter = 286000;
    world.cost_per_quad_meter_grow = 11;
    world.min_cost_per_quad_meter_grow = 10;
    world.max_cost_per_quad_meter_grow = 35;
}


// ================== ГЕНЕРАТОР ЧИСЕЛ ==================

int number_generator(unsigned int min, unsigned int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(min, max);
    return distr(gen);
}


// ================== МИР ==================

void inflation_in_this_year()
{
    world.inflation = number_generator(world.min_inflation, world.max_inflation);
    world.base_month_expenses=(1.0 + world.inflation / 100.0);
    world.cost_per_quad_meter_grow = number_generator(world.min_inflation, world.max_inflation)
}


void world_tick()
{
    if (time.month == 12){
        ++(time.year);
        time.month = 1;
        peter.age += 1;
        inflation_in_this_year();
        peter_salary_indexation();
    }
    else{
        ++(time.month);
    }

    if (peter.health <= 0.0) return;

    peter.health -= 1.0 / 12.0;
    peter.mental -= 1;

    if (peter.dismission == true){
        peter.mental -= 1;
    }

    if (peter.health <= 0.0){
        peter.health = 0.0;
        peter.last_damage_source = "старость";
    }
}

// ================== СЕМЬЯ =====================

void peter_girlfriend()
{
    if (peter.girlfriend == false and number_generator(1, 50)==1 
    and peter.girlfriend_possibility == true){
        peter.girlfriend = true;
        peter.mental+=10;
        peter.girlfriend_possibility=false;
    }

    if (peter.mental<30){
        peter.girlfriend = false;
        peter.girlfriend_possibility == false;
    }

    if (peter.girlfriend == true and number_generator(1, 500)==1){
        peter.girlfriend = false;
        peter.mental-=10;
        peter.girlfriend_possibility=true;
    }
    if (peter.girlfriend=true){
        peter.mental+=1;
    }
}


void peter_married()
{
    if (peter.girlfriend_time > number_generator(24, 36) and peter.salary>=80000){
        peter.married=true;
        peter.girlfriend=false;
        peter.girlfriend_possibility=false;
    }

    if (peter.mental<30){
        peter.married = false;
    } 

    if (peter.married = true){
        peter.mental+=1;
    }
}

void peter_childrens()
{
    unsigned int ch=peter.childs;
    if (peter.married_time > number_generator(12*ch, 24*ch) and peter.age<40){
        peter.childs+=1;
    }
}


void peter_grandchildrens()
{
    void;
}

void peter_family(){
    peter_girlfriend();
    peter_married();
    peter_childrens();
}

// ================== РАБОТА ==================

void peter_salary()
{
    if (peter.dismission){
        peter.salary = 0;
        return;
    }

}


void peter_vacation()
{
    peter.mental+=5;
}


void peter_salary_after_promotion()
{
    unsigned int x = peter.number_of_promotions;
    RUB new_base = 0;

    if (x == 0){
        new_base = static_cast<RUB>(number_generator(30, 50) * 1000ULL);
    }
    else if (x == 1){
        new_base = static_cast<RUB>(number_generator(70, 90) * 1000ULL);
    }
    else if (x == 2){
        new_base = static_cast<RUB>(number_generator(110, 130) * 1000ULL);
    }
    else if (x == 3){
        new_base = static_cast<RUB>(number_generator(150, 170) * 1000ULL);
    }
    else if (x == 4){
        new_base = static_cast<RUB>(number_generator(190, 210) * 1000ULL);
    }
    else {
        new_base = static_cast<RUB>(number_generator(230, 300) * 1000ULL);
    }

    peter.base_salary = new_base;
    peter.salary = peter.dismission ? 0 : peter.base_salary;
}


void peter_salary_indexation()
{
    if (peter.base_salary == 0) return;

    peter.base_salary = static_cast<RUB>(
        peter.base_salary * (1.0 + world.inflation / 100.0)
    );

    if (!peter.dismission){
        peter.salary = peter.base_salary;
    }
}


void peter_promotion_at_work()
{
    if (number_generator(1, 12 * 60 - peter.mental) == 1){
        peter.number_of_promotions++;
        peter.month_promotion = true;
        peter_salary_after_promotion();
    }
}


void peter_dismissial_from_work()
{
    if (peter.dismission){
        peter.mental-=5;
        return;
    }

    if (number_generator(1, peter.mental*6) == 1){
        peter.dismission = true;
        peter.dismissions_count += 1;
        peter.month_dismissed = true;
        peter.salary = 0;
    }
}


void peter_find_work()
{
    if (!peter.dismission){
        return;
    }
    
    if (number_generator(1, 12 * 60) == 1){
        peter.dismission = false;
        peter.salary = peter.base_salary;
    }
}


void peter_month_income()
{
    peter_dismissial_from_work();
    peter_find_work();
    peter_promotion_at_work();
    peter_salary();

    peter.month_income = peter.salary;
    peter.cash += peter.month_income;

}

// ================== РАСХОДЫ ==================

void peter_mortage()
{
    if (mortage.principal_amount <= 0){
        return;
        peter.flat = true;
    }

    if (peter.cash >= mortage.payment){
        peter.cash -= mortage.payment;
        peter.month_mortgage_payment += mortage.payment;

        RUB interest = static_cast<RUB>(mortage.principal_amount * mortage.interest_rate);
        if (mortage.payment > interest){
            RUB principal_part = mortage.payment - interest;
            if (principal_part > mortage.principal_amount){
                principal_part = mortage.principal_amount;
            }
            mortage.principal_amount -= principal_part;
            peter.mental-=1;
        }

        if (mortage.principal_amount == 0){
            peter.month_mortgage_paid_off = true;
        }
    }
}

void peter_mortage()
{
    if peter.flat == 
    if (peter.flat == 1)
}

void peter_food()
{
    void;
}


void peter_expenses()
{
    peter_mortage();
    peter_food();
}
// ================== БОЛЕЗНИ ==================

void peter_damage(double amount, const char* source)
{
    peter.health -= amount;
    if (peter.health < 0.0){
        peter.health = 0.0;
    }
    peter.last_damage_source = source;
}


void peter_disease_cold()
{
    if (number_generator(1, 36) == 1){
        peter.count_cold++;
        peter.month_disease = true;
        peter.month_disease_name = "простуда";
        peter.month_disease_damage = 0.1;
        peter_damage(0.1, "простуда");
    }
}


void peter_disease_angina()
{
    if (number_generator(1, 720) == 1){
        peter.count_angina++;
        peter.month_disease = true;
        peter.month_disease_name = "ангина";
        peter.month_disease_damage = 0.5;
        peter_damage(0.5, "ангина");
    }
}


void peter_disease_broken_bone()
{
    if (number_generator(1, 1440) == 1){
        peter.count_broken_bone++;
        peter.month_disease = true;
        peter.month_disease_name = "перелом кости";
        peter.month_disease_damage = 0.3;
        peter_damage(0.3, "перелом кости");
    }
}


void peter_disease_heart_attack()
{
    if (number_generator(1, 7200) == 1){
        peter.count_heart_attack++;
        peter.month_disease = true;
        peter.month_disease_name = "сердечный приступ";
        peter.month_disease_damage = 100.0;
        peter_damage(100.0, "сердечный приступ");
    }
}


void peter_disease()
{
    if (peter.health <= 0.0) return;
    peter_disease_cold();

    if (peter.health <= 0.0) return;
    peter_disease_angina();

    if (peter.health <= 0.0) return;
    peter_disease_broken_bone();

    if (peter.health <= 0.0) return;
    peter_disease_heart_attack();
}

void peter_expences_on_healing()
{
    RUB k = world.base_month_expenses;
    double d = number_generator(80, 120) / 100.0;
    peter.expenses_on_healing=peter.month_disease_damage*k*10*d;
}

void peter_health()
{
    peter_disease();
    peter_expences_on_healing();
}

// ================= МЕНТАЛЬНОЕ ЗДОРОВЬЕ ====================

void peter_mantality()
{
    void;
}

// ================== ЛОГ ==================

const char* month_name(unsigned int m)
{
    switch (m){
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


void log_finance()
{
    fprintf(log_file, "-Финансы\n");

    if (peter.month_income > 0){
        fprintf(log_file, "  зп: +%llu\n", peter.month_income);
    }
    else{
        fprintf(log_file, "  зп: 0 (безработный)\n");
    }

    if (peter.month_mortgage_payment > 0){
        fprintf(log_file, "  списание по ипотеке: -%llu\n",
                peter.month_mortgage_payment);
    }

    if (peter.month_mortgage_paid_off){
        fprintf(log_file, "  !!! ипотека полностью погашена !!!\n");
    }

    fprintf(log_file, "  наличные: %llu\n", peter.cash);

    if (mortage.principal_amount > 0){
        fprintf(log_file, "  остаток ипотеки: %llu\n", mortage.principal_amount);
    }
}


void log_health()
{
    fprintf(log_file, "-Здоровье\n");
    fprintf(log_file, "  показатель: %.2f\n", peter.health);

    if (peter.month_disease){
        fprintf(log_file, "  болезнь: %s (урон %.1f)\n",
                peter.month_disease_name,
                peter.month_disease_damage);
    }
    fprintf(log_file, "  простуд за жизнь:    %d\n", peter.count_cold);
    fprintf(log_file, "  ангин за жизнь:      %d\n", peter.count_angina);
    fprintf(log_file, "  переломов за жизнь:  %d\n", peter.count_broken_bone);
    fprintf(log_file, "  инфарктов за жизнь:  %d\n", peter.count_heart_attack);
}


void log_age()
{
    fprintf(log_file, "-Возраст\n");
    fprintf(log_file, "  %u лет\n", peter.age);

    if (peter.month_promotion){
        fprintf(log_file, "  повышение на работе (всего: %u)\n",
                peter.number_of_promotions);
    }

    if (peter.month_dismissed){
        fprintf(log_file, "  уволен с работы\n");
    }
}


void log_mental()
{
    fprintf(log_file, "-Ментальное состояние\n");
    fprintf(log_file, "  %u / 100\n", peter.mental);

    if (peter.mental >= 80){
        fprintf(log_file, "  состояние: отличное\n");
    }
    else if (peter.mental >= 60){
        fprintf(log_file, "  состояние: хорошее\n");
    }
    else if (peter.mental >= 40){
        fprintf(log_file, "  состояние: нормальное\n");
    }
    else if (peter.mental >= 20){
        fprintf(log_file, "  состояние: плохое\n");
    }
    else{
        fprintf(log_file, "  состояние: критическое\n");
    }
}


void log_month_header()
{
    fprintf(log_file, "\n");
    fprintf(log_file, "================= %s %u ===================\n",
            month_name(time.month), time.year);
}


void log_month_report()
{
    log_month_header();
    log_finance();
    log_health();
    log_age();
    log_mental();
}


// ================== СИМУЛЯЦИЯ ==================

void simulation()
{
    do {
        peter_reset_month_stats();

        peter_month_income();

        peter_expenses();

        peter_health();

        peter_family();

        log_month_report();

        world_tick();
    } while (peter.health > 0.0);

    fprintf(log_file, "\n");
    fprintf(log_file, "===========================================\n");
    fprintf(log_file, "                 СМЕРТЬ\n");
    fprintf(log_file, "===========================================\n");
    fprintf(log_file, "  причина:  %s\n", peter.last_damage_source);
    fprintf(log_file, "  возраст:  %u лет\n", peter.age);
}


int main()
{
    log_file = fopen("statistics.txt", "w");
    if (log_file == NULL){
        printf("Не удалось открыть файл для записи\n");
        return 1;
    }

    peter_init();
    mortage_init();
    time_init();
    world_init();
    simulation();

    fclose(log_file);
    log_file = NULL;

    return 0;
}