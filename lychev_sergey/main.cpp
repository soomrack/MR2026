#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <windows.h>

#include "bank.h"
#include "person.h"
#include "events.h"
#include "mortgage.h"
#include "borrow.h"

static const int TOTAL_RUNS = 1000;
static const int YEARS      = 30;

struct Stats {
    double alice_net = 0, bob_net = 0;
    double alice_liquid = 0, bob_liquid = 0;
    double alice_income = 0, bob_income = 0;
    double alice_expense = 0, bob_expense = 0;
    double alice_interest = 0, bob_interest = 0;
    double alice_medical = 0, bob_medical = 0;
    double alice_happiness = 0, bob_happiness = 0;
    double alice_health = 0, bob_health = 0;
    double alice_score = 0, bob_score = 0;
    double alice_side_jobs = 0, bob_side_jobs = 0;
    int alice_wins = 0, bob_wins = 0, draws = 0;
    int alice_bankrupt = 0, bob_bankrupt = 0;
    int alice_depressed = 0, bob_depressed = 0;
    int alice_apartment = 0, bob_apartment = 0;
};

static Stats stats;

static void one_simulation(unsigned seed)
{
    std::srand(seed);
    event_log.clear();

    Bank alpha_bank, micro_bank;
    Person alice, bob;

    bank_init(alpha_bank, "AlphaBank");
    bank_init(micro_bank, "MicroFinance");
    person_init(alice, "Alice", alpha_bank);
    person_init(bob,   "Bob",   alpha_bank);

    int a_debit   = bank_open_account(alpha_bank, AccountKind::Debit,   0.0, 0, 2026, 9);
    int a_deposit = bank_open_account(alpha_bank, AccountKind::Deposit, 9.6, 0, 2026, 9);
    person_attach_accounts(alice, a_debit, a_deposit, -1);
    person_attach_mfo(alice, micro_bank);

    int b_debit   = bank_open_account(alpha_bank, AccountKind::Debit,   0.0, 0, 2026, 9);
    int b_deposit = bank_open_account(alpha_bank, AccountKind::Deposit, 9.6, 0, 2026, 9);
    person_attach_accounts(bob, b_debit, b_deposit, -1);
    person_attach_mfo(bob, micro_bank);

    bob.rent = 30000;
    alice.rent = 30000;

    int year = 2026, month = 9;
    int a_side_jobs_taken = 0;
    int b_side_jobs_taken = 0;
    bool a_had_side_job = false;
    bool b_had_side_job = false;

    for (int step = 0; step < YEARS * 12; ++step) {

        alice.current_year  = year;
        alice.current_month = month;
        bob.current_year    = year;
        bob.current_month   = month;

        if (!alice.has_apartment) {
            RUB price = person_apartment_price(alice, year, month);
            RUB down  = static_cast<RUB>(price * MORTGAGE_DOWN_PERCENT / 100.0);
            RUB total = alice.cash +
                (alice.deposit_acc >= 0 ? bank_balance(*alice.bank, alice.deposit_acc) : 0);
            if (total >= down) mortgage_take(alice, price, year, month);
        }

        person_apply_salary(alice, month);
        person_pay_food(alice);
        person_pay_utilities(alice);
        person_pay_rent(alice);
        person_pay_car(alice, year, month);
        person_pay_pet(alice, year, month);
        person_pay_chronic(alice);
        mortgage_pay(alice);
        process_all_events(alice, year, month);

        person_update_side_job(alice, month);
        if (alice.has_side_job && !a_had_side_job) a_side_jobs_taken++;
        a_had_side_job = alice.has_side_job;

        if (alice.cash < 0) borrow_handle_deficit(alice, year, month);
        borrow_try_early_repay(alice, year, month);
        if (alice.cash > 0 && alice.has_mortgage)
            mortgage_try_early_repay(alice, year, month);
        if (alice.cash > 50000) person_move_to_deposit(alice, alice.cash - 50000);
        if (!alice.bankrupt && alice.loan_count == 0)
            person_change_credit_score(alice, +1);

        person_apply_salary(bob, month);
        person_pay_food(bob);
        person_pay_utilities(bob);
        person_pay_rent(bob);
        person_pay_car(bob, year, month);
        person_pay_pet(bob, year, month);
        person_pay_chronic(bob);
        process_all_events(bob, year, month);

        person_update_side_job(bob, month);
        if (bob.has_side_job && !b_had_side_job) b_side_jobs_taken++;
        b_had_side_job = bob.has_side_job;

        if (bob.cash < 0) borrow_handle_deficit(bob, year, month);
        borrow_try_early_repay(bob, year, month);
        if (bob.cash > 50000) person_move_to_deposit(bob, bob.cash - 50000);
        if (!bob.bankrupt && bob.loan_count == 0)
            person_change_credit_score(bob, +1);

        if (!bob.has_apartment && bob.deposit_acc >= 0) {
            RUB price = person_apartment_price(bob, year, month);
            RUB dep = bank_balance(*bob.bank, bob.deposit_acc);
            if (dep >= price) {
                bank_withdraw(*bob.bank, bob.deposit_acc, price);
                bob.has_apartment = true;
                bob.apartment_price = price;
                bob.rent = 0;
            }
        }

        bank_accrue_monthly(alpha_bank);
        bank_accrue_monthly(micro_bank);

        if (month == 12) { year++; month = 1; }
        else month++;
    }

    alice.current_year  = year;
    alice.current_month = month;
    bob.current_year    = year;
    bob.current_month   = month;

    long long a_net = person_net_worth(alice);
    long long b_net = person_net_worth(bob);
    long long a_liq = person_liquid_capital(alice);
    long long b_liq = person_liquid_capital(bob);

    stats.alice_net += (double)a_net;
    stats.bob_net   += (double)b_net;
    stats.alice_liquid += (double)a_liq;
    stats.bob_liquid   += (double)b_liq;
    stats.alice_income  += (double)alice.total_income;
    stats.bob_income    += (double)bob.total_income;
    stats.alice_expense += (double)alice.total_expense;
    stats.bob_expense   += (double)bob.total_expense;
    stats.alice_interest += (double)alice.total_interest_paid;
    stats.bob_interest   += (double)bob.total_interest_paid;
    stats.alice_medical += (double)alice.total_medical_spent;
    stats.bob_medical   += (double)bob.total_medical_spent;
    stats.alice_happiness += alice.happiness;
    stats.bob_happiness   += bob.happiness;
    stats.alice_health += alice.health;
    stats.bob_health   += bob.health;
    stats.alice_score += alice.credit_score;
    stats.bob_score   += bob.credit_score;
    stats.alice_side_jobs += a_side_jobs_taken;
    stats.bob_side_jobs   += b_side_jobs_taken;

    if (a_net > b_net) stats.alice_wins++;
    else if (b_net > a_net) stats.bob_wins++;
    else stats.draws++;

    if (alice.bankrupt) stats.alice_bankrupt++;
    if (bob.bankrupt)   stats.bob_bankrupt++;
    if (alice.depressed) stats.alice_depressed++;
    if (bob.depressed)   stats.bob_depressed++;
    if (alice.has_apartment) stats.alice_apartment++;
    if (bob.has_apartment)   stats.bob_apartment++;
}

static void print_stats()
{
    std::printf("\n");
    std::printf("==============================================================\n");
    std::printf("  РЕЗУЛЬТАТЫ %d ПРОГОНОВ × %d лет (средние)\n", TOTAL_RUNS, YEARS);
    std::printf("==============================================================\n");
    std::printf("Alice: капитал (ср.):    %14.0f\n", stats.alice_net / TOTAL_RUNS);
    std::printf("Bob:   капитал (ср.):    %14.0f\n", stats.bob_net / TOTAL_RUNS);
    std::printf("Alice: ликвидный (ср.):  %14.0f\n", stats.alice_liquid / TOTAL_RUNS);
    std::printf("Bob:   ликвидный (ср.):  %14.0f\n", stats.bob_liquid / TOTAL_RUNS);
    std::printf("Alice: недвижимость:     %14.0f\n",
                (stats.alice_net - stats.alice_liquid) / TOTAL_RUNS);
    std::printf("Bob:   недвижимость:     %14.0f\n",
                (stats.bob_net - stats.bob_liquid) / TOTAL_RUNS);
    std::printf("Alice: доходы (ср.):     %14.0f\n", stats.alice_income / TOTAL_RUNS);
    std::printf("Bob:   доходы (ср.):     %14.0f\n", stats.bob_income / TOTAL_RUNS);
    std::printf("Alice: расходы (ср.):    %14.0f\n", stats.alice_expense / TOTAL_RUNS);
    std::printf("Bob:   расходы (ср.):    %14.0f\n", stats.bob_expense / TOTAL_RUNS);
    std::printf("Alice: проценты (ср.):   %14.0f\n", stats.alice_interest / TOTAL_RUNS);
    std::printf("Bob:   проценты (ср.):   %14.0f\n", stats.bob_interest / TOTAL_RUNS);
    std::printf("Alice: медицина (ср.):   %14.0f\n", stats.alice_medical / TOTAL_RUNS);
    std::printf("Bob:   медицина (ср.):   %14.0f\n", stats.bob_medical / TOTAL_RUNS);
    std::printf("Alice: подработок:       %14.1f\n", stats.alice_side_jobs / TOTAL_RUNS);
    std::printf("Bob:   подработок:       %14.1f\n", stats.bob_side_jobs / TOTAL_RUNS);
    std::printf("Alice: счастье (ср.):    %14.1f\n", stats.alice_happiness / TOTAL_RUNS);
    std::printf("Bob:   счастье (ср.):    %14.1f\n", stats.bob_happiness / TOTAL_RUNS);
    std::printf("Alice: здоровье (ср.):   %14.1f\n", stats.alice_health / TOTAL_RUNS);
    std::printf("Bob:   здоровье (ср.):   %14.1f\n", stats.bob_health / TOTAL_RUNS);
    std::printf("Alice: рейтинг (ср.):    %14.1f\n", stats.alice_score / TOTAL_RUNS);
    std::printf("Bob:   рейтинг (ср.):    %14.1f\n", stats.bob_score / TOTAL_RUNS);
    std::printf("--------------------------------------------------------------\n");
    std::printf("Побед Alice: %d, Bob: %d, ничьих: %d\n",
                stats.alice_wins, stats.bob_wins, stats.draws);
    std::printf("Банкротств: Alice=%d, Bob=%d\n",
                stats.alice_bankrupt, stats.bob_bankrupt);
    std::printf("Депрессий:  Alice=%d, Bob=%d\n",
                stats.alice_depressed, stats.bob_depressed);
    std::printf("С квартирой: Alice=%d, Bob=%d\n",
                stats.alice_apartment, stats.bob_apartment);
    std::printf("==============================================================\n");

    if (stats.alice_wins > stats.bob_wins)
        std::printf("ВЫВОД: ипотека выгоднее накоплений\n");
    else if (stats.bob_wins > stats.alice_wins)
        std::printf("ВЫВОД: накопления выгоднее ипотеки\n");
    else
        std::printf("ВЫВОД: стратегии равнозначны\n");
    std::printf("==============================================================\n");
}

int main(int argc, char** argv)
{
    SetConsoleOutputCP(CP_UTF8);

    bool verbose = false;
    bool single  = false;
    unsigned base_seed = static_cast<unsigned>(std::time(nullptr));

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-v") == 0) verbose = true;
        else if (std::strcmp(argv[i], "-1") == 0) single = true;
        else if (std::strcmp(argv[i], "-s") == 0 && i + 1 < argc)
            base_seed = static_cast<unsigned>(std::atoi(argv[++i]));
    }

    if (single) {
        std::printf("Один прогон, seed=%u, лог включён\n\n", base_seed);
        event_log.verbose = true;
        event_log.clear();
        one_simulation(base_seed);
        print_stats();
        event_log.print();
        std::printf("\nНажмите Enter...\n");
        std::getchar();
        return 0;
    }

    std::printf("Запускаем %d прогонов по %d лет...\n", TOTAL_RUNS, YEARS);
    std::printf("base_seed = %u\n", base_seed);

    event_log.verbose = false;

    for (int run = 0; run < TOTAL_RUNS; ++run) {
        one_simulation(base_seed + run);
        if ((run + 1) % 100 == 0)
            std::printf("  прогресс: %d / %d\n", run + 1, TOTAL_RUNS);
    }

    print_stats();

    if (verbose) {
        event_log.verbose = true;
        event_log.clear();
        one_simulation(base_seed);
        event_log.print();
        event_log.verbose = false;
    }

    std::printf("\nНажмите Enter...\n");
    std::getchar();
    return 0;
}