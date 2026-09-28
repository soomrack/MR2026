#ifndef PERSON_H
#define PERSON_H

#include <string>
#include "bank.h"
#include "car.h"

#define MAX_CREDITS 3

struct Person {
    std::string name;

    RUB cash = 0;

    Bank* bank = nullptr;
    Bank* mfo  = nullptr;

    int debit_acc    = -1;
    int deposit_acc  = -1;
    int mortgage_acc = -1;

    /* массивы кредитов — до 3 одновременно в банке и до 3 в МФО */
    int loan_accs[MAX_CREDITS];
    int loan_count = 0;

    int mfo_accs[MAX_CREDITS];
    int mfo_count = 0;

    RUB    salary  = 0;
    RUB    salary2 = 0;
    double salary_indexation = 6.0;

    /* подработка */
    bool has_side_job         = false;
    int  side_job_months_left = 0;
    int  months_in_deficit    = 0;
    RUB  side_job_income      = 30000;

    RUB food      = 0;
    RUB utilities = 0;
    RUB rent      = 0;
    RUB pet       = 0;

    double infl_food      = 6.0;
    double infl_utilities = 8.0;
    double infl_rent      = 6.0;
    double infl_pet       = 7.0;

    int health     = 90;
    int health_max = 100;
    int happiness  = 80;

    bool alive    = true;
    bool bankrupt = false;

    int unemployed_months = 0;
    int sick_months       = 0;

    bool depressed          = false;
    int  depressed_months   = 0;
    RUB  total_therapy_spent = 0;

    int cold_count   = 0;
    int flu_count    = 0;
    int injury_count = 0;
    int chronic_count = 0;
    int hospital_count = 0;
    bool has_chronic_disease = false;
    RUB chronic_monthly_cost = 0;
    RUB total_medical_spent  = 0;

    RUB medical_spent_this_year = 0;
    RUB mortgage_interest_this_year = 0;

    int credit_score = 700;

    bool has_apartment   = false;
    RUB  apartment_price = 8000000;

    int current_year  = 2026;
    int current_month = 9;

    bool has_mortgage         = false;
    RUB  mortgage_debt        = 0;
    RUB  mortgage_payment     = 0;
    int  mortgage_months_left = 0;
    RUB  total_mortgage_interest = 0;

    bool has_cat            = true;
    int  cat_age_months     = 36;
    int  months_without_cat = 0;
    int  months_until_new_cat = 0;
    int  cats_total         = 1;

    Car  car;

    RUB total_income        = 0;
    RUB total_expense       = 0;
    RUB total_interest_paid = 0;

    int credits_taken = 0;
    int micro_taken   = 0;
    RUB total_borrowed = 0;
};

void person_init(Person& p, const std::string& name, Bank& bank);
void person_attach_accounts(Person& p, int debit, int deposit, int mortgage);
void person_attach_mfo(Person& p, Bank& mfo);

void person_apply_salary(Person& p, int month);
void person_pay_food(Person& p);
void person_pay_utilities(Person& p);
void person_pay_rent(Person& p);
void person_pay_car(Person& p, int year, int month);
void person_pay_pet(Person& p, int year, int month);
void person_pay_chronic(Person& p);

void person_move_to_deposit(Person& p, RUB amount);
void person_ensure_cash(Person& p, RUB target);
void person_bankrupt_austerity(Person& p);

void person_update_side_job(Person& p, int month);
void person_change_health(Person& p, int delta);
void person_change_happiness(Person& p, int delta);
void person_change_credit_score(Person& p, int delta);

RUB  person_monthly_expenses(const Person& p);
RUB  person_apartment_price(const Person& p, int year, int month);
RUB  person_liquid_capital(const Person& p);
RUB  person_net_worth(const Person& p);
void person_sell_apartment(Person& p);
void person_print(const Person& p);

#endif