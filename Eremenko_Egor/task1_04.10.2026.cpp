#include <stdio.h>
#include <cstdio>
#include <random>
#include <vector>
#include <algorithm>
#include <array>

/////////////////////////////////////////////////////////////////////////

using RUB = unsigned long long;

struct Person {
    RUB cash = 0;
    RUB income = 0;
    RUB scholarship = 0;
    RUB side_job = 0;
    RUB salary = 0;
    RUB parents_support = 0;
    RUB food = 0;
    RUB entertainment = 0;
    RUB dormitory = 0;
    RUB base_spend = 0;
    RUB mortgage = 0;   
    RUB rent = 0;
    RUB finance_reserve = 0;
    bool has_mortgage = false;  
    bool has_rent = false;      
};

struct Exam {
    const char* name;
    int score;
    int number;
};

/////////////////////////////////////////////////////////////////////////

enum class Life {
    UNKNOWN,
    IT_AND_MATH,        // 3 + 4 — программист (1)
    PHYSICS_AND_IT,     // 2 + 3 — инженер (2)
    PHYSICS_AND_MATH,   // 2 + 4 — физик теоретик (3)
    HUMANITIES_AND_IT,  // 1 + 3 — веб/гейм-дизайнер (4)
    HUMANITIES_AND_MATH,// 1 + 4 — экономист (5)
    HUMANITIES_AND_PHYSICS // 1 + 2 — преподаватель (6)
};

/////////////////////////////////////////////////////////////////////////

Person ilya;

/////////////////////////////////////////////////////////////////////////

// генератор источника случайных чисел
std::mt19937& get_gen() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

// результат проверки (успех или провал)
bool success(int difficulty, int modificator) {
    std::uniform_int_distribution<> dist(1, 20);
    int result = dist(get_gen());

    if (result == 20) {
        return true; // критический успех
    }
    if (result == 1) {
        return false;  // критический провал
    }
    return result >= difficulty + modificator; // остальные результаты
}

/////////////////////////////////////////////////////////////////////////


Life choose_future_life(const std::vector<int>& best_numbers) {
    int a = std::min(best_numbers[0], best_numbers[1]);
    int b = std::max(best_numbers[0], best_numbers[1]);

    // 3 + 4
    if (a == 3 && b == 4) return Life::IT_AND_MATH;
    // 2 + 3
    if (a == 2 && b == 3) return Life::PHYSICS_AND_IT;
    // 2 + 4
    if (a == 2 && b == 4) return Life::PHYSICS_AND_MATH;
    // 1 + 3
    if (a == 1 && b == 3) return Life::HUMANITIES_AND_IT;
    // 1 + 4
    if (a == 1 && b == 4) return Life::HUMANITIES_AND_MATH;
    // 1 + 2
    if (a == 1 && b == 2) return Life::HUMANITIES_AND_PHYSICS;

    return Life::UNKNOWN;
}

/////////////////////////////////////////////////////////////////////////

// результаты экзаменов после школы
Life school_exam_results() {   // ← было void, стало Life (возвращаем профессию)
    std::uniform_int_distribution<> dist(40, 100);
    Exam exams[4] = { // создаем список из результатов 4 экзаменов
        {"humanities", dist(get_gen()), 1}, // гуманитарные науки
        {"physics",    dist(get_gen()), 2}, // физика
        {"IT",         dist(get_gen()), 3}, // информатика
        {"math",       dist(get_gen()), 4} // математика
    };

    // сортируем по убыванию оценки
    std::sort(exams, exams + 4,
        [](const Exam& a, const Exam& b) {
            return a.score > b.score;
        });

    // создаем список из кодов 2 лучших экзаменов, где:
    // 1 - гуманитарные науки
    // 2 - физика
    // 3 - информатика
    // 4 - математика
    std::vector<int> best_numbers = { exams[0].number, exams[1].number };

    Life profession = choose_future_life(best_numbers);

    
    printf("best exams: %s (%d), %s (%d)\n",
        exams[0].name, exams[0].score,
        exams[1].name, exams[1].score);

    printf("chosen profession %d\n", profession);

    return profession;
}

/////////////////////////////////////////////////////////////////////////

void ilya_income(const int year, const int month, const Life profession, const double k_inflation)
{
    if (year == 2024 and month == 5) {
        ilya.parents_support = 20'000; // карманные деньги от родителей
    }
    if (year == 2024 and month == 9) { // поступление в вуз
        ilya.scholarship = 6'000; // стипендия
    }
    if (year == 2025 and month == 1) {
        ilya.side_job = 30'000; // подработка
    }
    if (year == 2026 and month == 9) {
        ilya.parents_support = 0; // родители перестают давать деньги
        ilya.side_job = 40'000;
        ilya.scholarship = 8'000;
    }
    if (year == 2028 and month == 5) { // выпуск из вуза
        ilya.side_job = 50'000;
        ilya.scholarship = 0;
    }
    if (year == 2028 and month == 9) {
        ilya.side_job = 0;
        ilya.scholarship = 0;
        if (profession == Life::IT_AND_MATH) {
            ilya.salary = 75'000;
        }
        if (profession == Life::PHYSICS_AND_IT) {
            ilya.salary = 55'000;
        }
        if (profession == Life::PHYSICS_AND_MATH) {
            ilya.salary = 35'000;
        }
        if (profession == Life::HUMANITIES_AND_IT) {
            ilya.salary = 65'000;
        }
        if (profession == Life::HUMANITIES_AND_MATH) {
            ilya.salary = 65'000;
        }
        if (profession == Life::HUMANITIES_AND_PHYSICS) {
            ilya.salary = 40'000;
        }

    }
    if (year == 2029 and month == 9) {
        ilya.side_job = 0;
        ilya.scholarship = 0;
        if (profession == Life::IT_AND_MATH) {
            ilya.salary = 140'000;
        }
        if (profession == Life::PHYSICS_AND_IT) {
            ilya.salary = 75'000;
        }
        if (profession == Life::PHYSICS_AND_MATH) {
            ilya.salary = 45'000;
        }
        if (profession == Life::HUMANITIES_AND_IT) {
            ilya.salary = 100'000;
        }
        if (profession == Life::HUMANITIES_AND_MATH) {
            ilya.salary = 90'000;
        }
        if (profession == Life::HUMANITIES_AND_PHYSICS) {
            ilya.salary = 65'000;
        }

    }
    if (year == 2031 and month == 9) {
        ilya.side_job = 0;
        ilya.scholarship = 0;
        if (profession == Life::IT_AND_MATH) {
            ilya.salary = 190'000;
        }
        if (profession == Life::PHYSICS_AND_IT) {
            ilya.salary = 110'000;
        }
        if (profession == Life::PHYSICS_AND_MATH) {
            ilya.salary = 75'000;
        }
        if (profession == Life::HUMANITIES_AND_IT) {
            ilya.salary = 100'000;
        }
        if (profession == Life::HUMANITIES_AND_MATH) {
            ilya.salary = 140'000;
        }
        if (profession == Life::HUMANITIES_AND_PHYSICS) {
            ilya.salary = 85'000;
        }

    }
        // весь доход
    ilya.income = ilya.scholarship + ilya.parents_support + ilya.salary + ilya.side_job;
    if (ilya.cash > 10000000000000000000) {
        ilya.side_job = 40'000 * k_inflation;
        ilya.income = ilya.scholarship + ilya.parents_support + ilya.salary + ilya.side_job;
     }
    else {
        ilya.side_job = 0;
        ilya.income = ilya.scholarship + ilya.parents_support + ilya.salary + ilya.side_job;
    }
}

/////////////////////////////////////////////////////////////////////////

void ilya_base_spend(const int year, const int month, const Life profession, const double k_inflation) {
    if (year == 2024 and month == 5) {
        ilya.food = 0;
        ilya.entertainment = 10'000;
    }
    if (year == 2024 and month == 9) {
        ilya.dormitory = 3'000;
        ilya.food = 15'000;
        ilya.entertainment = 10'000;
    }
    if (year == 2028 and month == 5) {  // выпуск — общежитие больше не нужно
        ilya.dormitory = 0;
    }
    ilya.base_spend = RUB (ilya.dormitory + (ilya.food + ilya.entertainment) * k_inflation);
}

/////////////////////////////////////////////////////////////////////////

bool can_afford_mortgage(const double k_inflation) {
    RUB first_payment = 100'000;
    RUB mortgage_payment = 40'000 * k_inflation;

    bool can_afford_first = (ilya.cash >= first_payment);
    bool can_afford_monthly = (ilya.income >= ilya.base_spend + mortgage_payment - ilya.rent + 10'000);

    return can_afford_first and can_afford_monthly;
}

void take_mortgage(const double k_inflation) {
    ilya.cash -= 100'000;
    ilya.mortgage = 40'000* k_inflation;
    ilya.has_mortgage = true;
    ilya.rent = 0;
    ilya.has_rent = false;
}

void take_rent() {
    ilya.rent = 30'000;
    ilya.has_rent = true;
    ilya.mortgage = 0;
    ilya.has_mortgage = false;
}

/////////////////////////////////////////////////////////////////////////

// Траты на жильё: ипотека или аренда
void ilya_mortgage(const int year, const int month, const Life profession, const double k_inflation) {
    if (year == 2028 and month == 5) {
        if (can_afford_mortgage(k_inflation)) {
            take_mortgage(k_inflation);
        }
        else {
            take_rent();
        }
    }

    if (ilya.has_rent) {
        if (month == 1 && ilya.rent <= 50'000) ilya.rent += 3'000;

        if (can_afford_mortgage(k_inflation)) {
            take_mortgage(k_inflation);
        }
    }
}

void simulation(Life profession) {
    double k_inflation = 1.0;
    int year = 2024, month = 5;
    while (!(year == 2036 && month == 9)) {
                
        ilya_base_spend(year, month, profession, k_inflation);
        ilya_mortgage(year, month, profession, k_inflation);
        ilya_income(year, month, profession, k_inflation);
        
        ilya.cash += ilya.income;
        ilya.cash -= ilya.base_spend;
        ilya.cash -= ilya.mortgage;
        ilya.cash -= ilya.rent;

        ++month;
        if (month == 13) {
            ++year;
            k_inflation *= 1.07;
            month = 1;
        }
    }

    std::printf("final cash: %llu\n", ilya.cash);
}

/////////////////////////////////////////////////////////////////////////

void ilya_init() {
    ilya.cash = 10'000;
}

int main() {
    //bool ok = success(20, 10);
    ilya_init();                                      
    Life profession = school_exam_results();          
    simulation(profession);                           
    //std::printf("%d\n", ok ? 1 : 0);
    printf("has mortgage %d\n", ilya.has_mortgage ? 1 : 0);
}
