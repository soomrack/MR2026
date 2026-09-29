#ifndef EVENTS_H
#define EVENTS_H

#include <string>
#include <vector>
#include "person.h"

struct EventLog {
    std::vector<std::string> lines;
    bool verbose = false;
    void add(const std::string& s) { if (verbose) lines.push_back(s); }
    void clear() { lines.clear(); }
    void print() const;
};

extern EventLog event_log;

bool roll_chance(double p);
int  roll_range(int lo, int hi);

void event_promotion(Person& p, int year, int month);
void event_job_loss(Person& p, int year, int month);
void event_new_job(Person& p, int year, int month);
void event_vacation(Person& p, int year, int month);
void event_bonus(Person& p, int year, int month);

void event_cold(Person& p, int year, int month);
void event_flu(Person& p, int year, int month);
void event_food_poisoning(Person& p, int year, int month);
void event_injury(Person& p, int year, int month);
void event_chronic(Person& p, int year, int month);
void event_hospital(Person& p, int year, int month);
void event_illness(Person& p, int year, int month);

void event_depression(Person& p, int year, int month);
void event_recovery(Person& p, int year, int month);
void event_tax_return(Person& p, int year, int month);
void event_life_drain(Person& p, int year, int month);

void process_all_events(Person& p, int year, int month);

#endif