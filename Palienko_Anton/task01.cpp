#include <stdio.h>


using RUB = unsigned long long int;


struct Person {
    RUB cash;
    RUB salary;
    RUB credit;
};

struct Person alice;


void alice_init()
{
    alice.cash = 20'000;
    alice.salary = 80'000;
    alice.credit = 0;
}


void alice_salary(const int year, const int month)
{
    if (year == 2026 and month == 12) {  // promotion
        alice.salary = 120'000;
    }

    alice.cash += alice.salary;
}


void alice_car(const int year, const int month)
{
    if (year == 2027 and month == 1) {    // кредит на машину
        alice.credit += 2'000'000;
    }
    else if ((year >= 2027 and month >= 1) and (year <= 2035 and month <= 1)) {
        alice.cash -= 30'000;    // текущие расходы на машину
    }
}

void alice_print()
{
    printf("Alice cash = %llu\n", alice.cash);
}


void simulation()
{
    int year = 2026;
    int month = 9;
    while (not (year == 2047 and month == 9)) {

        // ===== ALICE =====

        alice_salary(year, month);

        // alice_bank_income();

        // alice_food();
        // alice_utilities();
        // alice_transport();
        // alice_mobile_and_internet();
        // alice_clothes();
        // alice_entertainment();
        // alice_health();

        // alice_car();
        // alice_cat();

        // alice_mortgage();
        // alice_mortgage_payment();
        // alice_mortgage_interest();
        // alice_mortgage_insurance();
        // alice_mortgage_early_payment();

        // alice_tax();
        // alice_property_tax_deduction();

        // alice_vacation();
        // alice_gifts();

        // alice_random_event();

        // alice_emergency_fund();
        // alice_financial_decision();

        // alice_statistics();


        // ===== BOB =====

        // bob_salary();

        // bob_bank_income();
        // bob_deposit_interest();

        // bob_food();
        // bob_utilities();
        // bob_transport();
        // bob_mobile_and_internet();
        // bob_clothes();
        // bob_entertainment();
        // bob_health();

        // bob_car();
        // bob_cat();

        // bob_rent();
        // bob_rent_indexation();

        // bob_apartment_savings();
        // bob_check_apartment_price();
        // bob_buy_apartment();

        // bob_tax();

        // bob_vacation();
        // bob_gifts();

        // bob_random_event();

        // bob_emergency_fund();
        // bob_financial_decision();

        // bob_statistics();


        // ===== ECONOMY =====

        // update_food_inflation();
        // update_utilities_inflation();
        // update_transport_inflation();
        // update_rent_inflation();
        // update_apartment_price();
        // update_bank_rate();

        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
    }
}


int main()
{
    alice_init();

    simulation();

    alice_print();
}
