using namespace std;                                    

using RUB = unsigned long long int;
using Percent = double;


struct Work
{
    RUB salary_month;
    RUB bonus;                 
    string position;
    int experience_years;
    bool has_remote;             
    int vacation_days_used;       
    int vacation_days_total;     
    RUB education_allowance_year;    //бюджет на обучение (от компании)
    RUB transport_compensation_month;
    RUB last_net_salary;               // c НДФЛ
};

struct Person ulya;
struct Person vasya;


const RUB ndfl_limit1 = 2'400'000;
const RUB ndfl_limit2 = 5'000'000;
const RUB ndfl_limit3 = 20'000'000;
const RUB ndfl_limit4 = 50'000'000;

const Percent ndfl_rate1 = 13.0;
const Percent ndfl_rate2 = 15.0;
const Percent ndfl_rate3 = 18.0;
const Percent ndfl_rate4 = 20.0;
const Percent ndfl_rate5 = 22.0;

const Percent dividend_tax_rate = 13.0;
const Percent rental_tax_rate = 13.0;

RUB calculate_ndfl_year(RUB annual_income)       // ндфл
{
    RUB tax = 0;
    RUB remaining = annual_income;

    RUB part1 = (remaining < ndfl_limit1) ? remaining : ndfl_limit1-1;
    tax += (RUB)((double)part1 * ndfl_rate1 / 100);
    remaining -= part1;
    if (remaining == 0) {
        return tax;
    }

    RUB range2 = ndfl_limit2 - ndfl_limit1;
    RUB part2 = (remaining < range2) ? remaining : range2 - 1;
    tax += (RUB)((double)part2 * ndfl_rate2 / 100);
    remaining -= part2;
    if (remaining == 0) {
        return tax;
    }

    RUB range3 = ndfl_limit3 - ndfl_limit2;
    RUB part3 = (remaining < range3) ? remaining : range3-1;
    tax += (RUB)((double)part3 * ndfl_rate3 / 100);
    remaining -= part3;
    if (remaining == 0) {
        return tax;
    }


    RUB range4 = ndfl_limit4 - ndfl_limit3;
    RUB part4 = (remaining < range4) ? remaining : range4-1;
    tax += (RUB)((double)part4 * ndfl_rate4 / 100);
    remaining -= part4;
    if (remaining == 0) {
        return tax;
    }

    tax += (RUB)((double)remaining * ndfl_rate5 / 100);
    return tax;
}

const RUB taxes_go_back_limit = 120'000;                           //deduction_fitness
const Percent taxes_go_back_rate = 13.0;

RUB calculate_taxes_go_back(RUB expences) {
    RUB taxable_base = (expences < taxes_go_back_limit) ? expences : taxes_go_back_limit;
    return (RUB)((double)taxable_base * taxes_go_back_rate / 100.0);
}

void apply_social_deduction(Person &p, int month)
{
    if (month != 12) return;
    RUB deduction = calculate_taxes_go_back(p.taxes.ytd_fitness_paid);
    if (deduction > p.taxes.ytd_tax_paid) {
        deduction = p.taxes.ytd_tax_paid;
    }
    p.bank.balance += deduction;
    p.taxes.ytd_social_deduction = deduction;
    p.taxes.ytd_tax_paid -= deduction;
}


RUB calculate_go_back_year(RUB annual_income)       // возврат
{
    RUB tax = 0;
    RUB remaining = annual_income - p.taxes.ytd_tax_paid;

    RUB part1 = (remaining < ndfl_limit1) ? remaining : ndfl_limit1-1;
    tax += (RUB)((double)part1 * ndfl_rate1 / 100);
    remaining -= part1;
    if (remaining == 0) {
        return tax;
    }

    RUB range2 = ndfl_limit2 - ndfl_limit1;
    RUB part2 = (remaining < range2) ? remaining : range2 - 1;
    tax += (RUB)((double)part2 * ndfl_rate2 / 100);
    remaining -= part2;
    if (remaining == 0) {
        return tax;
    }

    RUB range3 = ndfl_limit3 - ndfl_limit2;
    RUB part3 = (remaining < range3) ? remaining : range3-1;
    tax += (RUB)((double)part3 * ndfl_rate3 / 100);
    remaining -= part3;
    if (remaining == 0) {
        return tax;
    }

    RUB range4 = ndfl_limit4 - ndfl_limit3;
    RUB part4 = (remaining < range4) ? remaining : range4-1;
    tax += (RUB)((double)part4 * ndfl_rate4 / 100);
    remaining -= part4;
    if (remaining == 0) {
        return tax;
    }

    tax += (RUB)((double)remaining * ndfl_rate5 / 100);
    return tax;
}

RUB true_tax = calculate_ndfl_year(RUB annual_income) - calculate_go_back_year(RUB annual_income)


RUB apply_income_tax(Person &p, RUB gross_amount)           //для случаев когда сумма дохода становится > ndfl_limit
{
    p.taxes.ytd_taxable_income += gross_amount;
    const RUB tax_total = calculate_ndfl_year(p.taxes.ytd_taxable_income);
    const RUB tax_this_year = tax_total - p.taxes.ytd_tax_paid;
    p.taxes.ytd_tax_paid = tax_total;
    return gross_amount - tax_this_year;
}


RUB apply_apartment_tax(RUB gross_amount, Percent rate_percent)
{
    const RUB tax = (RUB)((double)gross_amount * rate_percent / 100.0);
    return gross_amount - tax;
}


void reset_yearly_taxes(Person &p)
{
    p.taxes.ytd_taxable_income = 0;
    p.taxes.ytd_tax_paid = 0;
}


bool random_event(double prob)
{
    return (rand() / (double)RAND_MAX) < prob;     //true с вер prob
}

void ulya_salary(const int month, const int year)
{
   
    if (month == 1) {
        if (year >= 2026 && year <= 2035) {
            double index = 1.0;
            if (year == 2026) index = 1.03;     
            if (year == 2027) index = 1.05;  
            if (year == 2028) index = 1.04; 
            if (year == 2029) index = 1.06; 
            if (year == 2030) index = 1.05;  
            if (year == 2031) index = 1.04; 
            if (year == 2032) index = 1.04;  
            if (year == 2033) index = 1.03;  
            if (year == 2034) index = 1.03;  
            if (year == 2035) index = 1.02;

            if (year == 2030 && month == 3) index = 2;
            
            ulya.work.salary_month = (RUB)(ulya.work.salary_month * index);
        }
    }
    
    if (year >= 2026 && year <= 2035) {
        if (month == 6) {
            double bonus_percent = 0.5;
            if (year == 2026) bonus_percent = 0.5;
            if (year == 2027) bonus_percent = 0.5;
            if (year == 2028) bonus_percent = 0.6;
            if (year == 2029) bonus_percent = 0.7;
            if (year >= 2030) bonus_percent = 0.6;
            
            ulya.bank.balance += apply_income_tax(ulya, (RUB)(ulya.work.salary_month * bonus_percent));
        }
        
        if (month == 12){
            double bonus_percent = 0.3;
            if (year == 2026) bonus_percent = 0.3;
            if (year == 2027) bonus_percent = 0.4;
            if (year == 2028) bonus_percent = 0.4;
            if (year == 2029) bonus_percent = 0.5;
            if (year >= 2030) bonus_percent = 0.35; 
            
            ulya.bank.balance += apply_income_tax(ulya, (RUB)(ulya.work.salary_month * bonus_percent));
        }
    }
    const RUB net_salary = apply_income_tax(ulya, ulya.work.salary_month);
    ulya.bank.balance += net_salary;
    ulya.work.last_net_salary = net_salary;
}

void vasya_salary(const int month, const int year)
{
    if (month == 1) {
        if (year >= 2026 && year <= 2035) {
            double index = 1.0;
            if (year == 2026) index = 1.02;    
            if (year == 2027) index = 1.1; 
            if (year == 2028) index = 1.08;
            if (year == 2029) index = 1.04; 
            if (year == 2030) index = 1.2;  
            if (year == 2031) index = 1.02; 
            if (year == 2032) index = 1.02;  
            if (year == 2033) index = 1.02;  
            if (year == 2034) index = 1.1;  
            if (year == 2035) index = 1.2;  
            
            vasya.work.salary_month = (RUB)(vasya.work.salary_month * index);
        }
    }
    if (year >= 2026 && year <= 2035) {
   
        if (month == 9) {
            double bonus_percent = 0.0;
            if (year == 2026) bonus_percent = 0.0;      
            if (year == 2027) bonus_percent = 0.2;  
            if (year == 2028) bonus_percent = 0.25; 
            if (year == 2029) bonus_percent = 0.3;  
            if (year >= 2030) bonus_percent = 0.25; 
            
            vasya.bank.balance += apply_income_tax(vasya, (RUB)(vasya.work.salary_month * bonus_percent));
        }
        
        if (month == 3) {
            double bonus_percent = 0.0;
            if (year == 2026) bonus_percent = 0.0;     
            if (year == 2027) bonus_percent = 0.3;  
            if (year == 2028) bonus_percent = 0.35; 
            if (year == 2029) bonus_percent = 0.4;  
            if (year >= 2030) bonus_percent = 0.3;  
            
            vasya.bank.balance += apply_income_tax(Bob, (RUB)(vasya.work.salary_month * bonus_percent));
        }
    }
    const RUB net_salary = apply_income_tax(vasya, vasya.work.salary_month);
    vasya.bank.balance += net_salary;
    vasya.work.last_net_salary = net_salary;
}