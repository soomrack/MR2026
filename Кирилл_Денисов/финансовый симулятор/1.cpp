#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

class Rng 
{
public:
    unsigned long state;
public:
    Rng() { state = 1; }
    Rng(unsigned long seed) { state = seed; }

    unsigned long next()
    {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    double next_double()
    {
        return (next() % 1000000) / 1000000.0;
    }

    bool chance(double p)
    {
        return next_double() < p;
    }
};

class Inflation 
{
public:
    double food;     
    double utilities;   
    double housing;     
    double transport;   
    double vet;       
    double business;   
    double phone;      
    double clothing;  
    double leisure;    
    double health;      
private:
    double base_food;
    double base_utilities;
    double base_housing;
    double base_transport;
    double base_vet;
    double base_business;
    double base_phone;
    double base_clothing;
    double base_leisure;
    double base_health;
public:
    Inflation()
    {
        base_food = 0.008;
        base_utilities = 0.006;
        base_housing = 0.007;
        base_transport = 0.007;
        base_vet = 0.009;
        base_business = 0.008;
        base_phone = 0.003;    
        base_clothing = 0.006;
        base_leisure = 0.007;
        base_health = 0.009;

        food = base_food;
        utilities = base_utilities;
        housing = base_housing;
        transport = base_transport;
        vet = base_vet;
        business = base_business;
        phone = base_phone;
        clothing = base_clothing;
        leisure = base_leisure;
        health = base_health;
    }
public:
    void get_inflation(Rng& rng)
    {
        double noise;

        noise = (rng.next_double() - 0.5);
        food = 0.5 * base_food + 0.5 * food + noise * 0.004;

        noise = (rng.next_double() - 0.5);
        utilities = 0.5 * base_utilities + 0.5 * utilities + noise * 0.003;

        noise = (rng.next_double() - 0.5);
        housing = 0.5 * base_housing + 0.5 * housing + noise * 0.003;

        noise = (rng.next_double() - 0.5);
        transport = 0.5 * base_transport + 0.5 * transport + noise * 0.003;

        noise = (rng.next_double() - 0.5);
        vet = 0.5 * base_vet + 0.5 * vet + noise * 0.004;

        noise = (rng.next_double() - 0.5);
        business = 0.5 * base_business + 0.5 * business + noise * 0.003;

        noise = (rng.next_double() - 0.5);
        phone = 0.5 * base_phone + 0.5 * phone + noise * 0.002;

        noise = (rng.next_double() - 0.5);
        clothing = 0.5 * base_clothing + 0.5 * clothing + noise * 0.003;

        noise = (rng.next_double() - 0.5);
        leisure = 0.5 * base_leisure + 0.5 * leisure + noise * 0.003;

        noise = (rng.next_double() - 0.5);
        health = 0.5 * base_health + 0.5 * health + noise * 0.004;

        if (food < 0) food = 0;
        if (utilities < 0) utilities = 0;
        if (housing < 0) housing = 0;
        if (transport < 0) transport = 0;
        if (vet < 0) vet = 0;
        if (business < 0) business = 0;
        if (phone < 0) phone = 0;
        if (clothing < 0) clothing = 0;
        if (leisure < 0) leisure = 0;
        if (health < 0) health = 0;
    }
};

class TaxSystem 
{
public:
    double rate_low;
    double rate_high;
    double threshold;
public:
    TaxSystem()
    {
        rate_low = 0.13;
        rate_high = 0.15;
        threshold = 2.4e6;
    }
public:
    double get_ndfl(double yearly_gross) const
    {
        if (yearly_gross <= threshold) 
        {
            return yearly_gross * rate_low;
        }
        return threshold * rate_low
            + (yearly_gross - threshold) * rate_high;
    }
};

class Job 
{
public:
    double salary;
public:
    Job() { salary = 0; }
    Job(double s) { salary = s; }
public:
    void get_salary(const Inflation& infl, int month)
    {
        if (month % 12 == 0) {
            double yearly = infl.food * 12;
            salary *= (1.0 + yearly);
        }
    }
};

class Mortgage 
{
public:
    double left;             
    double monthly_rate;     
    double payment;          
    int months_left;         
    int original_months;     
    double last_interest;   
    double total_extra_paid; 
public:
    Mortgage()
    {
        left = 0; monthly_rate = 0; payment = 0;
        months_left = 0; original_months = 0;
        last_interest = 0; total_extra_paid = 0;
    }

    Mortgage(double debt, double yearly_rate, int years)
    {
        left = debt;
        monthly_rate = yearly_rate / 12.0;
        months_left = years * 12;
        original_months = years * 12;
        last_interest = 0;
        total_extra_paid = 0;

        if (monthly_rate == 0.0) 
        {
            payment = debt / months_left;
        }
        else 
        {
            payment = debt * monthly_rate /
                (1.0 - pow(1.0 + monthly_rate, -months_left));
        }
    }
public:
    void recalc_months()
    {
        if (left <= 0.0) 
        {
            left = 0.0;
            months_left = 0;
            return;
        }

        if (monthly_rate == 0.0) 
        {
            months_left = (int)(left / payment + 0.5);
        }
        else 
        {
            double ratio = left * monthly_rate / payment;
            if (ratio >= 1.0) 
            {
                months_left = 9999;
            }
            else 
            {
                double n = -log(1.0 - ratio) / log(1.0 + monthly_rate);
                months_left = (int)(n + 0.999); 
            }
        }
    }

    void pay_extra(double amount)
    {
        if (amount <= 0.0) return;
        if (amount > left) amount = left;

        left -= amount;
        total_extra_paid += amount;
        recalc_months();
    }

    void get_mortgage()
    {
        if (months_left <= 0) return;

        last_interest = left * monthly_rate;
        double principal = payment - last_interest;

        left -= principal;
        months_left -= 1;

        if (months_left == 0 || left < 0.01) left = 0.0;
    }

    bool is_paid() const { return left <= 0.0; }

    int saved_years(int months_passed) const
    {
        int saved = original_months - months_passed;
        if (saved < 0) saved = 0;
        return saved;
    }
};

class Pet 
{
public:
    bool alive;
    int age_months;
    int lifespan_months;
    double food_monthly;
    double vet_monthly;
    double last_event_cost;
public:
    Pet()
    {
        alive = false;
        age_months = 0;
        lifespan_months = 12 * 12;
        food_monthly = 4000;
        vet_monthly = 1500;
        last_event_cost = 0;
    }
public:
    void adopt()
    {
        alive = true;
        age_months = 0;
    }

    void random_events(Rng& rng)
    {
        last_event_cost = 0;
        if (!alive) return;

        if (rng.chance(0.03)) 
        {
            double age_factor = 1.0 + age_months / 24.0;
            last_event_cost = 15000 * age_factor * (0.5 + rng.next_double());
        }
    }

    void get_pet(const Inflation& infl)
    {
        if (!alive) return;

        food_monthly *= (1.0 + infl.vet);
        vet_monthly *= (1.0 + infl.vet);

        age_months += 1;
        if (age_months >= lifespan_months) alive = false;
    }

    double cost_this_month() const
    {
        if (!alive) return 0.0;

        double vet = vet_monthly;
        if (age_months > 8 * 12) vet *= 2.0;

        return food_monthly + vet + last_event_cost;
    }
};

class Car 
{
public:
    bool owned;
    double price;
    double fuel_monthly;
    double service_monthly;
    double insurance_yearly;
    int month_of_purchase;
    double last_event_cost;
public:
    Car()
    {
        owned = false;
        price = 1500000;
        fuel_monthly = 8000;
        service_monthly = 3000;
        insurance_yearly = 40000;
        month_of_purchase = -1;
        last_event_cost = 0;
    }
public:
    void random_events(Rng& rng)
    {
        last_event_cost = 0;
        if (!owned) return;

        if (rng.chance(0.04)) 
        {
            last_event_cost = 30000 + rng.next_double() * 90000;
        }
    }

    void get_car(const Inflation& infl)
    {
        if (owned) 
        {
            fuel_monthly *= (1.0 + infl.transport);
            service_monthly *= (1.0 + infl.transport);
            insurance_yearly *= (1.0 + infl.transport);
        }
        else 
        {
            price *= (1.0 + infl.transport);
        }
    }

    double cost_this_month(int month) const
    {
        if (!owned) return 0.0;

        double cost = fuel_monthly + service_monthly + last_event_cost;
        if ((month - month_of_purchase) % 12 == 0) cost += insurance_yearly;

        return cost;
    }
};

class PickupPoint 
{
public:
    bool is_open;
    int month_opened;
    int closed_months;

    double opening_cost;

    double parcels;
    double parcels_cap;
    double avg_order;
    double commission;
    double revenue;

    double rent;
    double staff_salary;
    double supplies;
    double expenses;

    double tax_rate;
    double tax_paid;

    double profit;
    double invested;
    double total_profit;

    double last_event_cost;
public:
    PickupPoint()
    {
        is_open = false;
        month_opened = -1;
        closed_months = 0;

        opening_cost = 900000;

        parcels = 1800;
        parcels_cap = 6000;
        avg_order = 1600;
        commission = 0.05;
        revenue = 0;

        rent = 35000;
        staff_salary = 55000;
        supplies = 8000;
        expenses = 0;

        tax_rate = 0.06;
        tax_paid = 0;

        profit = 0;
        invested = 0;
        total_profit = 0;

        last_event_cost = 0;
    }
public:
    double season(int month) const
    {
        int m = month % 12;
        if (m == 10 || m == 11) return 1.3;
        if (m == 6 || m == 7) return 0.85;
        return 1.0;
    }

    void open(int month)
    {
        is_open = true;
        month_opened = month;
        invested = opening_cost;
    }

    void random_events(Rng& rng)
    {
        last_event_cost = 0;
        if (!is_open) return;

        if (rng.chance(0.06)) 
        {
            last_event_cost = 10000 + rng.next_double() * 40000;
        }

        if (closed_months == 0 && rng.chance(0.02)) 
        {
            closed_months = 1;
        }
    }

    void get_business(const Inflation& infl, int month)
    {
        if (!is_open) return;

        if (closed_months > 0) 
        {
            closed_months -= 1;
            rent *= (1.0 + infl.business);
            staff_salary *= (1.0 + infl.business);
            supplies *= (1.0 + infl.business);
            revenue = 0;
            expenses = rent + staff_salary * 0.5 + supplies * 0.5;
            tax_paid = 0;
            profit = revenue - expenses - last_event_cost;
            total_profit += profit;
            return;
        }

        double growth = 1.02 * season(month);
        parcels *= growth;
        if (parcels > parcels_cap) parcels = parcels_cap;

        avg_order *= (1.0 + infl.business);

        revenue = parcels * avg_order * commission;

        rent *= (1.0 + infl.business);
        staff_salary *= (1.0 + infl.business);
        supplies *= (1.0 + infl.business);
        expenses = rent + staff_salary + supplies;

        tax_paid = revenue * tax_rate;

        profit = revenue - expenses - tax_paid - last_event_cost;
        total_profit += profit;
    }

    bool is_paid_back() const
    {
        return is_open && total_profit >= invested;
    }
};

class Person 
{
public:
    double savings;
    double bank_rate;
    Mortgage mortgage;
    Job job1;
    Job job2;
    Pet pet;
    Car car;
    PickupPoint pvz;
    TaxSystem tax;

    double yearly_gross;
    double yearly_tax_paid;
    double property_deduction_left;
    double interest_deduction_left;
    double min_savings;          
    double total_tax_paid;       
    double total_business_tax;   
    double total_interest_paid;  
    double total_refund;         
    double total_spent_food;
    double total_spent_utilities;
    double total_spent_transport;
    double total_spent_phone;
    double total_spent_clothing;
    double total_spent_leisure;
    double total_spent_health;
public:
    Person()
    {
        savings = 300000;
        bank_rate = 0.12 / 12.0;
        mortgage = Mortgage(6e6, 0.12, 20);
        job1 = Job(120000);
        job2 = Job(0);

        yearly_gross = 0;
        yearly_tax_paid = 0;
        property_deduction_left = 2e6;
        interest_deduction_left = 3e6;

        min_savings = savings;
        total_tax_paid = 0;
        total_business_tax = 0;
        total_interest_paid = 0;
        total_refund = 0;

        total_spent_food = 0;
        total_spent_utilities = 0;
        total_spent_transport = 0;
        total_spent_phone = 0;
        total_spent_clothing = 0;
        total_spent_leisure = 0;
        total_spent_health = 0;
    }
public:
    void get_income()
    {
        savings *= (1.0 + bank_rate);

        double gross = job1.salary + job2.salary;
        yearly_gross += gross;

        double tax_total = tax.get_ndfl(yearly_gross);
        double tax_now = tax_total - yearly_tax_paid;
        yearly_tax_paid = tax_total;

        total_tax_paid += tax_now;
        savings += gross - tax_now;

        if (savings < min_savings) min_savings = savings;
    }

    double get_refund()
    {
        double base_used = yearly_gross;
        if (base_used > property_deduction_left) base_used = property_deduction_left;

        double base_used_i = yearly_gross;
        if (base_used_i > interest_deduction_left) base_used_i = interest_deduction_left;

        double refund = base_used * tax.rate_low + base_used_i * tax.rate_low;
        if (refund > yearly_tax_paid) refund = yearly_tax_paid;

        property_deduction_left -= base_used;
        interest_deduction_left -= base_used_i;

        return refund;
    }

    void new_year()
    {
        yearly_gross = 0;
        yearly_tax_paid = 0;
    }
};

class Expenses 
{
public:
    double food;       
    double utilities;   
    double transport;   
    double phone;       
    double clothing;    
    double leisure;    
    double health;      
    double rent;        
public:
    Expenses()
    {
        food = 15000;
        utilities = 6000;
        transport = 3000;
        phone = 1000;
        clothing = 5000;
        leisure = 5000;
        health = 2000;
        rent = 0;
    }
public:
    void get_expenses(const Inflation& infl)
    {
        food *= (1.0 + infl.food);
        utilities *= (1.0 + infl.utilities);
        transport *= (1.0 + infl.transport);
        phone *= (1.0 + infl.phone);
        clothing *= (1.0 + infl.clothing);
        leisure *= (1.0 + infl.leisure);
        health *= (1.0 + infl.health);
    }

    double total() const
    {
        return food + utilities + transport + phone
            + clothing + leisure + health + rent;
    }
};

void control_pet(Person& person, int month)
{
    if (month == 24 && !person.pet.alive) 
    {
        person.pet.adopt();
        printf("  >> завели питомца\n");
    }

    person.savings -= person.pet.cost_this_month();
}

void control_car(Person& person, int month)
{
    if (!person.car.owned && person.savings > person.car.price + 100000) 
    {
        person.savings -= person.car.price;
        person.car.owned = true;
        person.car.month_of_purchase = month;
        printf("  >> купили машину за %.0f\n", person.car.price);
    }

    person.savings -= person.car.cost_this_month(month);
}

void control_expenses(Person& person, const Expenses& expenses)
{
    person.savings -= expenses.total();

    person.total_spent_food += expenses.food;
    person.total_spent_utilities += expenses.utilities;
    person.total_spent_transport += expenses.transport;
    person.total_spent_phone += expenses.phone;
    person.total_spent_clothing += expenses.clothing;
    person.total_spent_leisure += expenses.leisure;
    person.total_spent_health += expenses.health;
}

void control_mortgage(Person& person)
{
    person.savings -= person.mortgage.payment;
    person.mortgage.get_mortgage();
    person.total_interest_paid += person.mortgage.last_interest;

    if (!person.mortgage.is_paid()) 
    {
        double reserve = person.mortgage.payment * 6.0;
        double surplus = person.savings - reserve;

        if (surplus > 100000.0) 
        {
            double extra = surplus * 0.5;
            person.savings -= extra;
            person.mortgage.pay_extra(extra);
        }
    }

    if (person.savings < person.min_savings) person.min_savings = person.savings;
}

void control_tax(Person& person, int month)
{
    if (month % 12 == 11) 
    {
        double refund = person.get_refund();
        person.savings += refund;
        person.total_refund += refund;
        printf("  >> возврат налогов: %.0f\n", refund);
        person.new_year();
    }
}

void control_business(Person& person, int month)
{
    if (!person.pvz.is_open && month >= 48) 
    {
        double need = person.pvz.opening_cost + 200000;
        if (person.savings > need) 
        {
            person.savings -= person.pvz.opening_cost;
            person.pvz.open(month);
            printf("  >> открыли ПВЗ, вложено %.0f\n", person.pvz.invested);
        }
    }

    if (person.pvz.is_open) 
    {
        person.savings += person.pvz.profit;
        person.total_business_tax += person.pvz.tax_paid;

        if (person.savings < person.min_savings) person.min_savings = person.savings;
    }
}

void control_random_events(Person& person, Rng& rng, int month)
{
    if (month % 3 != 0) return;

    person.pet.random_events(rng);
    person.car.random_events(rng);
    person.pvz.random_events(rng);
}

void print_year(const Person& person, int month)
{
    printf("Год %2d: накопления = %10.0f, ипотека = %10.0f",
        month / 12, person.savings, person.mortgage.left);

    if (person.pvz.is_open) 
    {
        printf(", ПВЗ: прибыль/мес = %8.0f", person.pvz.profit);
        if (person.pvz.is_paid_back()) printf(" (окуплен)");
    }

    printf("\n");
}

void print_summary(const Person& person, int months)
{
    int years = months / 12;
    int saved_years = person.mortgage.saved_years(months);

    printf("----------------------------------------\n");
    printf("ИТОГИ ЖИЗНИ \n");
    printf("----------------------------------------\n");
    printf("Симуляция длилась %d лет (%d месяцев)\n", years, months);
    printf("Итоговые накопления: %.0f\n", person.savings);
    printf("Самый глубокий минус по счёту: %.0f\n", person.min_savings);
    printf("\n");

    printf("ИПОТЕКА:\n");
    printf("  Плановый срок:                  %12d лет\n", person.mortgage.original_months / 12);
    printf("  Фактический срок:               %12d лет\n", years);
    if (saved_years > 0) 
    {
        printf("  Досрочно погашено:              %12.0f\n", person.mortgage.total_extra_paid);
        printf("  Срок сокращён на:               %12d лет\n", saved_years);
    }
    printf("  Проценты банку за всё время:    %12.0f\n", person.total_interest_paid);
    printf("\n");

    printf("ТРАТЫ НА ЖИЗНЬ ЗА ВСЁ ВРЕМЯ (с учётом инфляции):\n");
    printf("  Еда:                            %12.0f\n", person.total_spent_food);
    printf("  Коммуналка:                     %12.0f\n", person.total_spent_utilities);
    printf("  Транспорт:                      %12.0f\n", person.total_spent_transport);
    printf("  Связь:                          %12.0f\n", person.total_spent_phone);
    printf("  Одежда:                         %12.0f\n", person.total_spent_clothing);
    printf("  Развлечения:                    %12.0f\n", person.total_spent_leisure);
    printf("  Медицина:                       %12.0f\n", person.total_spent_health);
    printf("\n");

    printf("Куда ещё ушли деньги:\n");
    printf("  НДФЛ (налог с зарплаты):        %12.0f\n", person.total_tax_paid);
    printf("  Налог с бизнеса (УСН 6%%):       %12.0f\n", person.total_business_tax);
    printf("  Возвращено через вычеты:        %12.0f\n", person.total_refund);
    printf("\n");

    if (person.pvz.is_open) 
    {
        printf("БИЗНЕС:\n");
        printf("  Вложено в открытие ПВЗ:         %12.0f\n", person.pvz.invested);
        printf("  Прибыль за всё время:           %12.0f\n", person.pvz.total_profit);
        if (person.pvz.is_paid_back()) 
        {
            printf("  ПВЗ окупился.\n");
        }
        else 
        {
            printf("  ПВЗ НЕ окупился (недобор %.0f).\n",
                person.pvz.invested - person.pvz.total_profit);
        }
        printf("\n");
    }
    else 
    {
        printf("ПВЗ так и не открылся (не хватило денег).\n\n");
    }

    printf("----------------------------------------\n");
    printf("ВЫВОД:\n");

    bool ok_business = !person.pvz.is_open || person.pvz.is_paid_back();
    bool ok_balance = person.min_savings > -200000;
    bool ok_result = person.savings > 0;

    if (ok_result && ok_balance && ok_business) 
    {
        printf("Модель жизни ХОРОШАЯ: ипотека выплачена, бизнес\n");
        printf("окупился, финансовая подушка не проседала критически.\n");
    }
    else 
    {
        printf("Модель жизни СПОРНАЯ:\n");
        if (!ok_result)
            printf("  - на финише нет накоплений;\n");
        if (!ok_balance)
            printf("  - счёт уходил в минус глубже 200 тыс. (риск);\n");
        if (!ok_business)
            printf("  - бизнес не окупился.\n");
    }

    if (saved_years >= 5) 
    {
        printf("\nДосрочное погашение сильно сократило срок (%d лет) —\n", saved_years);
        printf("при ставке 12%% гасить долг раньше выгоднее, чем копить на вкладе.\n");
    }
    else if (saved_years > 0) 
    {
        printf("\nДосрочное погашение сократило срок на %d лет.\n", saved_years);
    }
}

void setup_console()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
}


int run_simulation(Person& person, Inflation& inflation,
    Expenses& expenses, Rng& rng)
{
    int month;

    for (month = 0; month < 360; ++month) 
    {

        control_random_events(person, rng, month);

        inflation.get_inflation(rng);

        person.job1.get_salary(inflation, month);
        person.job2.get_salary(inflation, month);
        person.pet.get_pet(inflation);
        person.car.get_car(inflation);
        person.pvz.get_business(inflation, month);
        expenses.get_expenses(inflation);

        person.get_income();

        control_pet(person, month);
        control_car(person, month);
        control_expenses(person, expenses);
        control_mortgage(person);
        control_tax(person, month);
        control_business(person, month);

        if (month % 12 == 0) print_year(person, month);

        if (person.mortgage.is_paid()) break;
    }

    return month + 1;
}


int main()
{
    setup_console();

    unsigned long seed = (unsigned long)time(NULL);
    printf("Судьба №%lu\n\n", seed);

    Rng rng(seed);
    Inflation inflation;
    Expenses expenses;
    Person alice;

    int months = run_simulation(alice, inflation, expenses, rng);

    print_summary(alice, months);
}
