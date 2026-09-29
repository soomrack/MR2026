#pragma once

#include <cstdio>

// Глобальный файл лога (определён в log.cpp)
extern FILE *log_file;
extern FILE *event_log_file;

const char *month_name(unsigned int m);

void log_event(const char *format, ...);

void log_finance();
void log_health();
void log_age();
void log_mental();
void log_family();
void log_month_header();
void log_month_report();
