#pragma once

#include <vector>

using RUB = unsigned long long int;

// Ипотека одной инвестиционной квартиры хранится отдельно от личной ипотеки.
struct RentalMortgage {
    RUB principal_amount;
    RUB payment;
    RUB down_payment;
    RUB total_paid;
    RUB total_early_repayment;
    double interest_rate;
    unsigned int months_left;
    bool active;
};

// Квартира, которую Пётр купил для сдачи в аренду.
struct RentalFlat {
    unsigned int id;
    unsigned int quad_meters;
    unsigned int room_count;
    RUB purchase_price;
    RUB market_value;
    RUB monthly_rent;
    RUB total_rent_income;
    RUB total_repair_expenses;
    RUB total_maintenance_expenses;
    unsigned int tenant_months;
    unsigned int vacant_months;
    unsigned int tenant_changes;
    unsigned int evictions;
    unsigned int damage_cases;
    bool tenant;
    RentalMortgage mortgage;
};

// Общие показатели портфеля для помесячной статистики.
struct RentalPortfolio {
    std::vector<RentalFlat> flats;
    RUB month_rent_income;
    RUB month_mortgage_payment;
    RUB month_maintenance_expenses;
    RUB month_repair_expenses;
    RUB month_early_repayment;
    RUB month_purchase_down_payment;
    unsigned int month_tenants_found;
    unsigned int month_tenants_evicted;
    unsigned int month_damage_cases;
    RUB total_rent_income;
    RUB total_mortgage_payment;
    RUB total_repair_expenses;
    RUB total_maintenance_expenses;
    RUB total_early_repayment;
    unsigned int total_tenants_found;
    unsigned int total_tenants_evicted;
    unsigned int total_damage_cases;
};

extern RentalPortfolio rental_portfolio;

void rental_portfolio_init();
void rental_portfolio_reset_month_stats();
void peter_investment_flats();
void rental_portfolio_indexation();
unsigned int rental_flats_with_tenants();
RUB rental_portfolio_debt();
RUB rental_portfolio_market_value();
