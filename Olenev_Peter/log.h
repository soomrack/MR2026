#pragma once

#include <cstdio>

// Глобальный файл лога (определён в log.cpp)
extern FILE *log_file;

// Вспомогательная функция: имя месяца по номеру (1..12)
const char *month_name(unsigned int m);

// Отдельные секции отчёта
void log_finance();
void log_health();
void log_age();
void log_mental();

// Заголовок месяца и полный отчёт
void log_month_header();
void log_month_report();