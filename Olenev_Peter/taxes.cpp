#include "peter.h"
#include "world.h"
#include "time.h"

#include <algorithm>
#include <cmath>


static RUB income_tax(RUB income)
{
    const RUB limits[] = {2400000, 5000000, 20000000, 50000000};
    const unsigned int rates[] = {13, 15, 18, 20, 22};
    long double tax = 0;
    RUB lower_limit = 0;

    for (unsigned int i = 0; i < 4; ++i) {
        RUB taxable_part = std::min(income, limits[i]) - lower_limit;
        tax += static_cast<long double>(taxable_part) * rates[i] / 100;
        if (income <= limits[i]) {
            return static_cast<RUB>(std::round(tax));
        }
        lower_limit = limits[i];
    }

    tax += static_cast<long double>(income - lower_limit) * rates[4] / 100;
    return static_cast<RUB>(std::round(tax));
}


void peter_taxes()
{
    if (peter.tax_year != time.year) {
        peter.rent_tax_due = income_tax(peter.year_taxable_income + peter.year_rent_income)
            - income_tax(peter.year_taxable_income);
        peter.tax_year = time.year;
        peter.year_taxable_income = 0;
        peter.year_rent_income = 0;
    }

    RUB previous_tax = income_tax(peter.year_taxable_income);
    peter.year_taxable_income += peter.month_salary_income;
    RUB salary_tax = income_tax(peter.year_taxable_income);
    peter.month_salary_tax = salary_tax - previous_tax;

    peter.year_rent_income += peter.month_rent_income;
    peter.month_rent_tax = 0;
    if (time.month == 7) {
        peter.month_rent_tax = peter.rent_tax_due;
        peter.rent_tax_due = 0;
    }

    if (peter.car) {
        RUB annual_tax = world.car_horsepower * world.transport_tax_per_horsepower;
        peter.month_transport_tax = annual_tax / 12;
    }

    if (peter.month_salary_tax + peter.month_rent_tax + peter.month_transport_tax > 0) {
        peter_remove_mental(1, "уплата налогов");
    }
}


void peter_insurance()
{
    for (Flat &flat : peter.flats) {
        if (flat.insurance_months_left == 0) {
            peter.month_flat_insurance += world.flat_annual_insurance;
            flat.insurance_months_left = 12;
        }
        --flat.insurance_months_left;
    }

    if (peter.car) {
        if (peter.car_insurance_months_left == 0) {
            peter.month_car_insurance = world.car_annual_insurance;
            peter.car_insurance_months_left = 12;
        }
        --peter.car_insurance_months_left;
    }

    if (peter.month_flat_insurance + peter.month_car_insurance > 0) {
        peter_remove_mental(1, "оплата страховок");
    }
}
