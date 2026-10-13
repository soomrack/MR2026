#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// ===== Основные структуры данных и параметры модели =====
// PS: С английским у меня плохо, поэтому пишу перевод больше для себя


namespace alice_bob {
    // Модель работает помесячно: каждый шаг начисляет доходы и проценты,
    // учитывает расходы/события, погашает долги и сохраняет результат.
    constexpr int months_in_year = 12;      // месяц
    constexpr double percent_base = 100.0;  // процент в доли
    constexpr double rubles_epsilon = 0.01; // рубль (задаем точность до копейки)

// Перечисление значений каждого класса
enum class housing_mode {   // жилье
    rent,                   // аренда
    mortgage,               // ипотека
    owned_home              // собств. квартира
};


enum class loan_kind {      // кредит
    mortgage,               // ипотека
    car_loan,               // автокредит
    consumer_loan           // кредит классический - потребительский
};


enum class event_kind {     // события (траты/прибыль)  
    none,                   // 
    salary_bonus,           // премии (дословно зарпалата бонус)
    temporary_job_loss,     // подработка (временная работа)
    medical_expense,        // мед. расходы
    car_repair,             // ремонт авто
    pet_treatment,          // лечение животных
    home_repair,            // ремонт дома
    food_discount,          // скидка на еду
    rent_increase,          // арендная плата (увеличение)
    freelance_order         // подработка (фриланс - тип личные не стабильно)
};

// приняла решение расходы сделать отдельно (стабильные) от событий
enum class expense_kind {   // расходы
    food,                   // еда
    utilities,              // коммуналка
    rent,                   // аренда (обычно коммуналка не в счет)
    mortgage,               // ипотека
    pet,                    // дом. животные
    car,                    // авто
    transport,              // трансопрт
    communication,          // связь
    health,                 // здоровье
    leisure,                // досуг
    household,              // дом. хозяйство
    insurance,              // страхование
    loan_payment,           // выплата кредита
    event_expense           // расходы на мероприятия
};


struct simulation_date {    // структура (группировка данных об одном объекте)
    int year = 2026;
    int month = 1;
    // nodiscard - возврат значений, без игнора, в целом можно не писать - это больше для проверки себя
    // const - не меняет состояние текущего объекта
    [[nodiscard]] int serial_month() const {                                // номер месяца
        return year * months_in_year + month - 1;                           // мы присваеваем тут как раз таки индивидуальный номер 
    }

    [[nodiscard]] int year_index(const simulation_date& start) const {      // индекс года
        return serial_month() - start.serial_month();                       // разница в месяцах
    }

    [[nodiscard]] bool january() const {                                    // true или false (остальное)
        return month == 1;
    }

    [[nodiscard]] bool december() const {
        return month == 12;
    }

    [[nodiscard]] std::string text() const {                                // перевод на читаемые названия через массив
        static const std::array<std::string, 12> names = {
            "январь", "февраль", "март", "апрель", "май", "июнь",
            "июль", "август", "сентябрь", "октябрь", "ноябрь", "декабрь"
        };
        return names.at(static_cast<std::size_t>(month - 1)) + "    " + std::to_string(year);
    }

    void advance_one_month() {
        ++month;                                                            // цикл, по новому после 12
        if (month > months_in_year) {
            month = 1;
            ++year;
        }
    }
};


struct indexed_amount {                                 // индексация месячное значение, годовой %
    std::string name;
    double monthly_value = 0.0;
    double annual_inflation_percent = 0.0;

    void apply_annual_inflation() {                     // увеличение на годовой %
        monthly_value *= 1.0 + annual_inflation_percent / percent_base;
    }
};


struct job {                                            // работа
    std::string title;
    double monthly_salary = 0.0;
    double annual_salary_growth_percent = 0.0;
    bool enabled = true;
    int lost_months_left = 0;

    [[nodiscard]] double salary_for_month() const {     // зарплата
        if (!enabled || lost_months_left > 0) {
            return 0.0;
        }
        return monthly_salary;
    }

    void apply_annual_growth() {                        // повышение зарплаты
        monthly_salary *= 1.0 + annual_salary_growth_percent / percent_base;
    }
};


struct tax_rule {                                       // НАЛОГИ ВЫЧЕТЫ И ТД
    double income_tax_percent = 13.0;                   // ПОДОХОДНЫЙ НАЛОВГ В %
    double property_deduction_limit = 2'000'000.0;      // ЛИМИТ
    double mortgage_interest_deduction_limit = 3'000'000.0;
    double property_deduction_used = 0.0;               // ИСПОЛЬЗОВАННЫЙ
    double interest_deduction_used = 0.0;               
    bool use_property_deduction = true;                 // ВЫЧЕТ НА НЕДВИЖИМОСТЬ
    bool use_interest_deduction = true;                 // ВЫЧЕТ ПО ИПОТЕЧНЫМ %   
};


struct savings_account {                                // накопительный счет
    double balance = 0.0;
    double annual_rate_percent = 12.0;
    double monthly_interest_received = 0.0;
    double minimum_balance = 0.0;

    [[nodiscard]] double monthly_rate() const {         // годовая ставка в месячную
        return annual_rate_percent / percent_base / months_in_year;
    }

    double accrue_interest() {                          // проценты за месяц и добавление
        monthly_interest_received = std::max(0.0, balance) * monthly_rate();
        balance += monthly_interest_received;
        return monthly_interest_received;
    }

    bool withdraw(double amount) {                      // снятие ограничение, чтоб не уйти ниже 
        if (amount < 0.0 || balance - amount + rubles_epsilon < minimum_balance) {
            return false;
        }
        balance -= amount;
        return true;
    }

    void deposit(double amount) {                       // пополнение
        if (amount > 0.0) {
            balance += amount;
        }
    }
};


struct loan {                                         // кредит
    loan_kind kind = loan_kind::consumer_loan;
    std::string name;
    double initial_principal = 0.0;
    double remaining_principal = 0.0;
    double annual_rate_percent = 0.0;
    int original_term_months = 0;
    int remaining_term_months = 0;
    double required_monthly_payment = 0.0;
    double total_interest_paid = 0.0;
    double total_principal_paid = 0.0;
    double total_paid = 0.0;
    bool active = false;

    [[nodiscard]] double monthly_rate() const {
        return annual_rate_percent / percent_base / months_in_year;
    }

    [[nodiscard]] bool paid_off() const {
        return !active || remaining_principal <= rubles_epsilon;
    }
};


struct pet {
    std::string name;
    int lifespan_months = 0;
    int age_months = 0;
    indexed_amount monthly_cost;
    bool enabled = false;

    [[nodiscard]] bool alive() const {
        return enabled && age_months < lifespan_months;
    }

    void grow_one_month() {
        if (alive()) {
            ++age_months;
        }
    }
};


struct car {
    bool enabled = false;
    std::string model;
    indexed_amount fuel;
    indexed_amount maintenance;
    indexed_amount parking;
    indexed_amount insurance;
    int insurance_month = 3;
    double insurance_annual_cost = 0.0;
    double annual_insurance_inflation_percent = 0.0;

    void apply_annual_inflation() {
        fuel.apply_annual_inflation();
        maintenance.apply_annual_inflation();
        parking.apply_annual_inflation();
        insurance_annual_cost *= 1.0 + annual_insurance_inflation_percent / percent_base;
    }
};


struct housing {                                        // жилье
    housing_mode mode = housing_mode::rent;
    indexed_amount monthly_rent;
    indexed_amount monthly_utilities;
    indexed_amount maintenance_reserve;
    double property_price = 0.0;
    double down_payment = 0.0;
    double purchase_month = 0.0;
    double annual_property_growth_percent = 0.0;
    bool property_owned = false;

    void apply_annual_inflation() {
        monthly_rent.apply_annual_inflation();
        monthly_utilities.apply_annual_inflation();
        maintenance_reserve.apply_annual_inflation();
        property_price *= 1.0 + annual_property_growth_percent / percent_base;
    }
};


struct person_plan {
    std::string name;
    std::string strategy_name;
    std::string strategy_description;
    simulation_date start_date;
    int simulation_months = 240;
    job primary_job;
    job secondary_job;
    savings_account savings;
    housing home;
    loan mortgage;
    std::vector<loan> additional_loans;
    pet domestic_pet;
    car personal_car;
    tax_rule taxes;
    std::vector<indexed_amount> regular_expenses;
    double cash_balance = 0.0;
    double credit_limit = 250'000.0;
    double credit_debt = 0.0;
    double credit_card_monthly_rate_percent = 3.5;
    double emergency_fund_months = 6.0;
    double planned_savings_share_percent = 15.0;
    double mortgage_prepayment_share_percent = 50.0;
    bool make_mortgage_prepayment = true;
    bool make_car_prepayment = false;
    bool receive_tax_refund = true;
    bool allow_secondary_job = true;
    bool stop_when_mortgage_paid = false;
};


struct random_event {                               // рандом событие (доход/расход/условия)
    event_kind kind = event_kind::none;
    std::string title;
    double income_change = 0.0;
    double expense_change = 0.0;
    int affects_primary_job_for_months = 0;
    int affects_secondary_job_for_months = 0;
    bool changes_rent = false;
    double rent_multiplier = 1.0;
};


struct month_record {                               // результат месяца, ниже года (хранение)
    simulation_date date;
    double primary_salary = 0.0;
    double secondary_salary = 0.0;
    double savings_interest = 0.0;
    double tax_refund = 0.0;
    double other_income = 0.0;
    double total_income = 0.0;
    std::map<expense_kind, double> expenses;
    double regular_expenses_total = 0.0;
    double loan_principal_paid = 0.0;
    double loan_interest_paid = 0.0;
    double mortgage_prepayment = 0.0;
    double total_expenses = 0.0;
    double net_cashflow = 0.0;
    double savings_balance = 0.0;
    double cash_balance = 0.0;
    double credit_debt = 0.0;
    double mortgage_balance = 0.0;
    double net_worth = 0.0;
    std::vector<std::string> notes;
};


struct year_record {
    int year = 0;
    double income = 0.0;
    double expenses = 0.0;
    double savings_interest = 0.0;
    double tax_refund = 0.0;
    double loan_interest = 0.0;
    double loan_principal = 0.0;
    double prepayment = 0.0;
    double net_cashflow = 0.0;
    double savings_balance_end = 0.0;
    double mortgage_balance_end = 0.0;
    double net_worth_end = 0.0;
    int negative_cashflow_months = 0;
    int events_count = 0;
};


struct simulation_result {                          // итог структуры симуляции
    person_plan final_plan;
    std::vector<month_record> months;
    std::vector<year_record> years;
    double total_income = 0.0;
    double total_expenses = 0.0;
    double total_interest_income = 0.0;
    double total_tax_refund = 0.0;
    double total_loan_interest = 0.0;
    double total_loan_principal = 0.0;
    double total_prepayment = 0.0;
    double final_net_worth = 0.0;
    int negative_cashflow_months = 0;
    int credit_limit_breaches = 0;
    int mortgage_paid_month = -1;
    bool financially_stable = false;
    std::vector<std::string> conclusions;
};


struct comparison_result {                          // сравнение
    simulation_result alice;
    simulation_result bob;
    std::string winner_name;
    double net_worth_difference = 0.0;
    std::vector<std::string> conclusions;
};


struct monte_carlo_summary {                       // для множественных запусков???? риск/устойчивость
    int runs = 0;
    int alice_wins = 0;
    int bob_wins = 0;
    int draws = 0;
    double alice_average_net_worth = 0.0;
    double bob_average_net_worth = 0.0;
    double alice_min_net_worth = std::numeric_limits<double>::max();
    double alice_max_net_worth = std::numeric_limits<double>::lowest();
    double bob_min_net_worth = std::numeric_limits<double>::max();
    double bob_max_net_worth = std::numeric_limits<double>::lowest();
    double alice_average_negative_months = 0.0;
    double bob_average_negative_months = 0.0;
};


inline std::string money(double value) {
    std::ostringstream stream;
    stream.imbue(std::locale::classic());
    stream << std::fixed << std::setprecision(2) << value << "  руб.";
    return stream.str();
}


inline std::string percent(double value) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << value << " %";
    return stream.str();
}


inline std::string housing_text(housing_mode mode) {
    switch (mode) {
        case housing_mode::rent:
            return "аренда";
        case housing_mode::mortgage:
            return "ипотека";
        case housing_mode::owned_home:
            return "собственное жильё";
    }
    return "неизвестно";
}


inline std::string loan_text(loan_kind kind) {
    switch (kind) {
        case loan_kind::mortgage:
            return "ипотека";
        case loan_kind::car_loan:
            return "автокредит";
        case loan_kind::consumer_loan:
            return "потребительский кредит";
    }
    return "кредит";
}


inline std::string expense_text(expense_kind kind) {
    switch (kind) {
        case expense_kind::food:
            return "питание";
        case expense_kind::utilities:
            return "коммунальные услуги";
        case expense_kind::rent:
            return "аренда";
        case expense_kind::mortgage:
            return "ипотека";
        case expense_kind::pet:
            return "домашнее животное";
        case expense_kind::car:
            return "автомобиль";
        case expense_kind::transport:
            return "транспорт";
        case expense_kind::communication:
            return "связь";
        case expense_kind::health:
            return "здоровье";
        case expense_kind::leisure:
            return "досуг";
        case expense_kind::household:
            return "бытовые расходы";
        case expense_kind::insurance:
            return "страхование";
        case expense_kind::loan_payment:
            return "прочие кредиты";
        case expense_kind::event_expense:
            return "случайные события";
    }
    return "расход";
}


inline double sum_expenses(const std::map<expense_kind, double>& expenses) {
    double total = 0.0;
    for (const auto& [kind, value] : expenses) {
        static_cast<void>(kind);
        total += value;
    }
    return total;
}


inline void add_expense(month_record& record, expense_kind kind, double value) {
    if (value > rubles_epsilon) {
        record.expenses[kind] += value;
    }
}

}  // namespace alice_bob


// ===== Объявления финансовых формул и работы с кредитом =====
namespace alice_bob {

struct loan_payment {
    double scheduled_payment = 0.0;
    double interest_part = 0.0;
    double principal_part = 0.0;
    double extra_principal = 0.0;
    bool paid_off = false;
};


double annuity_payment(double principal, double annual_rate_percent, int term_months);

double total_annuity_cost(double principal, double annual_rate_percent, int term_months);

double annual_effective_rate(double nominal_annual_rate_percent);

double monthly_payment_for_income_ratio(double income, double maximum_ratio_percent);

int months_to_reach_target(double initial_savings,
                           double monthly_contribution,
                           double annual_rate_percent,
                           double target);

double future_value_of_savings(double initial_savings,
                               double monthly_contribution,
                               double annual_rate_percent,
                               int months);

double discounted_value(double future_sum, double annual_discount_percent, int months);

double debt_to_income_ratio(double monthly_debt_payment, double monthly_income);

double housing_cost_ratio(double housing_cost, double monthly_income);

double safe_emergency_fund(double fixed_monthly_expenses, double reserve_months);

double calculate_income_tax(double gross_income, double tax_percent);

double calculate_property_tax_refund(tax_rule& rule,
                                     double property_cost,
                                     double available_income_tax);

double calculate_interest_tax_refund(tax_rule& rule,
                                     double mortgage_interest_paid,
                                     double available_income_tax);

void initialize_loan(loan& value,
                     loan_kind kind,
                     const std::string& name,
                     double principal,
                     double annual_rate_percent,
                     int term_months);

loan_payment pay_loan_month(loan& value, double additional_principal_payment);

double payoff_amount(const loan& value);

double remaining_total_payments(const loan& value);

double estimated_months_to_payoff(const loan& value, double monthly_extra_payment);

bool can_make_prepayment(const loan& value, double amount);

double round_money(double value);

bool nearly_equal(double left, double right, double tolerance = 0.01);

}  // namespace alice_bob

// ===== Финансовые формулы и ежемесячный платёж =====


namespace alice_bob {

namespace {

constexpr double zero_rate_tolerance = 0.0000001;


double positive_or_zero(double value) {
    return std::max(0.0, value);
}


double limited(double value, double lower, double upper) {
    return std::clamp(value, lower, upper);
}


double loan_payment_without_rounding(double principal,
                                     double monthly_rate,
                                     int term_months) {
    if (principal <= 0.0 || term_months <= 0) {
        return 0.0;
    }

    if (std::abs(monthly_rate) < zero_rate_tolerance) {
        return principal / static_cast<double>(term_months);
    }

    const double power = std::pow(1.0 + monthly_rate, term_months);
    return principal * monthly_rate * power / (power - 1.0);
}


double available_deduction_base(double limit, double used) {
    return positive_or_zero(limit - used);
}

}  // namespace


double annuity_payment(double principal, double annual_rate_percent, int term_months) {
    const double monthly_rate = annual_rate_percent / percent_base / months_in_year;
    return round_money(loan_payment_without_rounding(principal, monthly_rate, term_months));
}


double total_annuity_cost(double principal, double annual_rate_percent, int term_months) {
    return annuity_payment(principal, annual_rate_percent, term_months)
           * static_cast<double>(std::max(0, term_months));
}


double annual_effective_rate(double nominal_annual_rate_percent) {
    const double monthly_rate = nominal_annual_rate_percent / percent_base / months_in_year;
    return (std::pow(1.0 + monthly_rate, months_in_year) - 1.0) * percent_base;
}


double monthly_payment_for_income_ratio(double income, double maximum_ratio_percent) {
    return positive_or_zero(income) * limited(maximum_ratio_percent, 0.0, 100.0) / percent_base;
}


int months_to_reach_target(double initial_savings,
                           double monthly_contribution,
                           double annual_rate_percent,
                           double target) {
    if (target <= initial_savings + rubles_epsilon) {
        return 0;
    }

    if (monthly_contribution <= 0.0 && annual_rate_percent <= 0.0) {
        return -1;
    }

    double balance = positive_or_zero(initial_savings);
    const double monthly_rate = annual_rate_percent / percent_base / months_in_year;

    for (int month = 1; month <= 1'200; ++month) {
        balance *= 1.0 + monthly_rate;
        balance += positive_or_zero(monthly_contribution);
        if (balance + rubles_epsilon >= target) {
            return month;
        }
    }

    return -1;
}


double future_value_of_savings(double initial_savings,
                               double monthly_contribution,
                               double annual_rate_percent,
                               int months) {
    const int safe_months = std::max(0, months);
    const double monthly_rate = annual_rate_percent / percent_base / months_in_year;
    double value = positive_or_zero(initial_savings);

    for (int month = 0; month < safe_months; ++month) {
        value *= 1.0 + monthly_rate;
        value += monthly_contribution;
    }

    return round_money(value);
}


double discounted_value(double future_sum, double annual_discount_percent, int months) {
    const double monthly_rate = annual_discount_percent / percent_base / months_in_year;
    const int safe_months = std::max(0, months);
    return future_sum / std::pow(1.0 + monthly_rate, safe_months);
}


double debt_to_income_ratio(double monthly_debt_payment, double monthly_income) {
    if (monthly_income <= rubles_epsilon) {
        return monthly_debt_payment > rubles_epsilon ? 100.0 : 0.0;
    }
    return positive_or_zero(monthly_debt_payment) / monthly_income * percent_base;
}


double housing_cost_ratio(double housing_cost, double monthly_income) {
    return debt_to_income_ratio(housing_cost, monthly_income);
}


double safe_emergency_fund(double fixed_monthly_expenses, double reserve_months) {
    return positive_or_zero(fixed_monthly_expenses) * positive_or_zero(reserve_months);
}


double calculate_income_tax(double gross_income, double tax_percent) {
    return positive_or_zero(gross_income) * limited(tax_percent, 0.0, 100.0) / percent_base;
}


double calculate_property_tax_refund(tax_rule& rule,
                                     double property_cost,
                                     double available_income_tax) {
    if (!rule.use_property_deduction || available_income_tax <= rubles_epsilon) {
        return 0.0;
    }

    const double unused_base = available_deduction_base(rule.property_deduction_limit,
                                                        rule.property_deduction_used);
    const double possible_base = std::min(positive_or_zero(property_cost), unused_base);
    const double possible_refund = calculate_income_tax(possible_base, rule.income_tax_percent);
    const double refund = std::min(possible_refund, positive_or_zero(available_income_tax));
    const double used_base = refund / (rule.income_tax_percent / percent_base);
    rule.property_deduction_used += used_base;
    return round_money(refund);
}


double calculate_interest_tax_refund(tax_rule& rule,
                                     double mortgage_interest_paid,
                                     double available_income_tax) {
    if (!rule.use_interest_deduction || available_income_tax <= rubles_epsilon) {
        return 0.0;
    }

    const double unused_base = available_deduction_base(rule.mortgage_interest_deduction_limit,
                                                        rule.interest_deduction_used);
    const double possible_base = std::min(positive_or_zero(mortgage_interest_paid), unused_base);
    const double possible_refund = calculate_income_tax(possible_base, rule.income_tax_percent);
    const double refund = std::min(possible_refund, positive_or_zero(available_income_tax));
    const double used_base = refund / (rule.income_tax_percent / percent_base);
    rule.interest_deduction_used += used_base;
    return round_money(refund);
}


void initialize_loan(loan& value,
                     loan_kind kind,
                     const std::string& name,
                     double principal,
                     double annual_rate_percent,
                     int term_months) {
    value.kind = kind;
    value.name = name;
    value.initial_principal = positive_or_zero(principal);
    value.remaining_principal = positive_or_zero(principal);
    value.annual_rate_percent = positive_or_zero(annual_rate_percent);
    value.original_term_months = std::max(0, term_months);
    value.remaining_term_months = std::max(0, term_months);
    value.required_monthly_payment = annuity_payment(value.initial_principal,
                                                     value.annual_rate_percent,
                                                     value.original_term_months);
    value.total_interest_paid = 0.0;
    value.total_principal_paid = 0.0;
    value.total_paid = 0.0;
    value.active = value.initial_principal > rubles_epsilon && value.original_term_months > 0;
}


loan_payment pay_loan_month(loan& value, double additional_principal_payment) {
    loan_payment result;

    if (!value.active || value.remaining_principal <= rubles_epsilon) {
        value.active = false;
        value.remaining_principal = 0.0;
        result.paid_off = true;
        return result;
    }

    const double monthly_interest = value.remaining_principal * value.monthly_rate();
    const double scheduled_payment = std::min(value.required_monthly_payment,
                                              value.remaining_principal + monthly_interest);
    const double scheduled_principal = positive_or_zero(scheduled_payment - monthly_interest);
    const double remaining_after_scheduled = positive_or_zero(value.remaining_principal
                                                               - scheduled_principal);
    const double extra = std::min(positive_or_zero(additional_principal_payment),
                                  remaining_after_scheduled);

    result.scheduled_payment = round_money(scheduled_payment);
    result.interest_part = round_money(monthly_interest);
    result.principal_part = round_money(scheduled_principal);
    result.extra_principal = round_money(extra);

    value.remaining_principal = positive_or_zero(value.remaining_principal
                                                 - scheduled_principal
                                                 - extra);
    value.total_interest_paid += monthly_interest;
    value.total_principal_paid += scheduled_principal + extra;
    value.total_paid += scheduled_payment + extra;
    value.remaining_term_months = std::max(0, value.remaining_term_months - 1);

    if (value.remaining_principal <= rubles_epsilon || value.remaining_term_months == 0) {
        value.remaining_principal = 0.0;
        value.active = false;
        result.paid_off = true;
    }

    return result;
}


double payoff_amount(const loan& value) {
    return positive_or_zero(value.remaining_principal);
}


double remaining_total_payments(const loan& value) {
    if (!value.active) {
        return 0.0;
    }
    return value.required_monthly_payment * static_cast<double>(value.remaining_term_months);
}


double estimated_months_to_payoff(const loan& value, double monthly_extra_payment) {
    if (!value.active || value.remaining_principal <= rubles_epsilon) {
        return 0.0;
    }

    loan copy = value;
    const int maximum_months = std::max(1, value.remaining_term_months + 360);
    for (int month = 1; month <= maximum_months; ++month) {
        const loan_payment payment = pay_loan_month(copy, monthly_extra_payment);
        if (payment.paid_off) {
            return static_cast<double>(month);
        }
    }

    return static_cast<double>(maximum_months);
}


bool can_make_prepayment(const loan& value, double amount) {
    return value.active && amount > rubles_epsilon && value.remaining_principal > rubles_epsilon;
}


double round_money(double value) {
    return std::round(value * 100.0) / 100.0;
}


bool nearly_equal(double left, double right, double tolerance) {
    return std::abs(left - right) <= std::abs(tolerance);
}

}  // namespace alice_bob

// ===== Объявления сценариев и симуляции =====



namespace alice_bob {

person_plan make_alice_plan();

person_plan make_bob_plan();

std::vector<random_event> generate_events_for_month(std::mt19937& engine,
                                                     const person_plan& plan,
                                                     const simulation_date& date,
                                                     bool enabled);

simulation_result run_simulation(person_plan plan,
                                 bool random_events_enabled,
                                 std::uint32_t seed);

comparison_result compare_default_strategies(bool random_events_enabled,
                                             std::uint32_t seed);

monte_carlo_summary run_ab_experiment(int runs, std::uint32_t base_seed);

double calculate_net_worth(const person_plan& plan);

double calculate_monthly_regular_cost(const person_plan& plan, const simulation_date& date);

std::vector<std::string> assess_financial_health(const simulation_result& result);

std::string describe_event(const random_event& event);

}  // namespace alice_bob

// ===== Месячная симуляция Алисы и Боба =====



namespace alice_bob {

namespace {

constexpr double apartment_purchase_cost_percent = 1.5;
constexpr double credit_card_minimum_payment_percent = 10.0;
constexpr double maximum_safe_debt_ratio_percent = 45.0;
constexpr int retirement_buffer_months = 24;


indexed_amount make_expense(const std::string& name,
                            double amount,
                            double inflation_percent) {
    return indexed_amount{name, amount, inflation_percent};
}


loan make_loan(loan_kind kind,
               const std::string& name,
               double principal,
               double rate,
               int months) {
    loan result;
    initialize_loan(result, kind, name, principal, rate, months);
    return result;
}


double net_salary_for_month(const job& value, const tax_rule& tax) {
    const double gross = value.salary_for_month();
    return gross - calculate_income_tax(gross, tax.income_tax_percent);
}


double gross_salary_for_month(const person_plan& plan) {
    return plan.primary_job.salary_for_month() + plan.secondary_job.salary_for_month();
}


void apply_inflation_for_new_year(person_plan& plan) {
    plan.primary_job.apply_annual_growth();
    plan.secondary_job.apply_annual_growth();
    plan.home.apply_annual_inflation();
    plan.personal_car.apply_annual_inflation();

    for (indexed_amount& expense : plan.regular_expenses) {
        expense.apply_annual_inflation();
    }

    if (plan.domestic_pet.enabled) {
        plan.domestic_pet.monthly_cost.apply_annual_inflation();
    }
}


double car_cost_for_month(const person_plan& plan, const simulation_date& date) {
    if (!plan.personal_car.enabled) {
        return 0.0;
    }

    double cost = plan.personal_car.fuel.monthly_value
                  + plan.personal_car.maintenance.monthly_value
                  + plan.personal_car.parking.monthly_value;

    if (date.month == plan.personal_car.insurance_month) {
        cost += plan.personal_car.insurance_annual_cost;
    }

    return cost;
}


double regular_expenses_for_month(const person_plan& plan,
                                  month_record& record,
                                  const simulation_date& date) {
    double total = 0.0;

    for (const indexed_amount& expense : plan.regular_expenses) {
        total += expense.monthly_value;

        if (expense.name == "Питание") {
            add_expense(record, expense_kind::food, expense.monthly_value);
        } else if (expense.name == "Транспорт") {
            add_expense(record, expense_kind::transport, expense.monthly_value);
        } else if (expense.name == "Связь") {
            add_expense(record, expense_kind::communication, expense.monthly_value);
        } else if (expense.name == "Здоровье") {
            add_expense(record, expense_kind::health, expense.monthly_value);
        } else if (expense.name == "Досуг") {
            add_expense(record, expense_kind::leisure, expense.monthly_value);
        } else {
            add_expense(record, expense_kind::household, expense.monthly_value);
        }
    }

    if (plan.home.mode == housing_mode::rent) {
        total += plan.home.monthly_rent.monthly_value;
        add_expense(record, expense_kind::rent, plan.home.monthly_rent.monthly_value);
    }

    if (plan.home.mode != housing_mode::rent) {
        total += plan.home.monthly_utilities.monthly_value;
        total += plan.home.maintenance_reserve.monthly_value;
        add_expense(record, expense_kind::utilities, plan.home.monthly_utilities.monthly_value);
        add_expense(record, expense_kind::household, plan.home.maintenance_reserve.monthly_value);
    } else {
        const double tenant_utilities = plan.home.monthly_utilities.monthly_value * 0.55;
        total += tenant_utilities;
        add_expense(record, expense_kind::utilities, tenant_utilities);
    }

    if (plan.domestic_pet.alive()) {
        total += plan.domestic_pet.monthly_cost.monthly_value;
        add_expense(record, expense_kind::pet, plan.domestic_pet.monthly_cost.monthly_value);
    }

    const double monthly_car_cost = car_cost_for_month(plan, date);
    total += monthly_car_cost;
    add_expense(record, expense_kind::car, monthly_car_cost);

    return total;
}


double pay_required_loans(person_plan& plan, month_record& record) {
    double total = 0.0;

    if (plan.mortgage.active) {
        const loan_payment payment = pay_loan_month(plan.mortgage, 0.0);
        total += payment.scheduled_payment;
        record.loan_interest_paid += payment.interest_part;
        record.loan_principal_paid += payment.principal_part;
        add_expense(record, expense_kind::mortgage, payment.scheduled_payment);

        if (payment.paid_off) {
            plan.home.mode = housing_mode::owned_home;
            record.notes.push_back("Ипотека полностью погашена");
        }
    }

    for (loan& current_loan : plan.additional_loans) {
        if (!current_loan.active) {
            continue;
        }

        const loan_payment payment = pay_loan_month(current_loan, 0.0);
        total += payment.scheduled_payment;
        record.loan_interest_paid += payment.interest_part;
        record.loan_principal_paid += payment.principal_part;
        add_expense(record, expense_kind::loan_payment, payment.scheduled_payment);

        if (payment.paid_off) {
            record.notes.push_back("Погашен кредит: " + current_loan.name);
        }
    }

    return total;
}


double calculate_credit_card_interest(const person_plan& plan) {
    const double monthly_rate = plan.credit_card_monthly_rate_percent
                                / percent_base
                                / months_in_year;
    return plan.credit_debt * monthly_rate;
}


double pay_credit_card_from_surplus(person_plan& plan,
                                    month_record& record,
                                    double available_cash) {
    if (plan.credit_debt <= rubles_epsilon || available_cash <= rubles_epsilon) {
        return 0.0;
    }

    const double minimum_payment = plan.credit_debt
                                   * credit_card_minimum_payment_percent
                                   / percent_base;
    const double payment = std::min(plan.credit_debt,
                                    std::max(std::min(available_cash, minimum_payment), 0.0));
    plan.credit_debt -= payment;
    add_expense(record, expense_kind::loan_payment, payment);
    record.loan_principal_paid += payment;
    return payment;
}


double calculate_reserve_target(const person_plan& plan, const simulation_date& date) {
    return safe_emergency_fund(calculate_monthly_regular_cost(plan, date),
                               plan.emergency_fund_months);
}


double apply_mortgage_prepayment(person_plan& plan,
                                 month_record& record,
                                 double available_cash) {
    if (!plan.make_mortgage_prepayment || !plan.mortgage.active) {
        return 0.0;
    }

    const double planned_from_surplus = available_cash
                                        * plan.mortgage_prepayment_share_percent
                                        / percent_base;
    const double available = std::min(planned_from_surplus,
                                      plan.mortgage.remaining_principal);

    if (!can_make_prepayment(plan.mortgage, available)) {
        return 0.0;
    }

    const double payment_amount = available;
    const double actual_extra = std::min(payment_amount, plan.mortgage.remaining_principal);
    plan.mortgage.remaining_principal -= actual_extra;
    plan.mortgage.total_principal_paid += actual_extra;
    plan.mortgage.total_paid += actual_extra;

    record.mortgage_prepayment += actual_extra;
    record.loan_principal_paid += actual_extra;
    add_expense(record, expense_kind::mortgage, actual_extra);

    if (plan.mortgage.remaining_principal <= rubles_epsilon) {
        plan.mortgage.remaining_principal = 0.0;
        plan.mortgage.active = false;
        plan.home.mode = housing_mode::owned_home;
        record.notes.push_back("Ипотека погашена досрочно");
    }

    return actual_extra;
}


void apply_event(person_plan& plan,
                 const random_event& event,
                 month_record& record) {
    if (event.kind == event_kind::none) {
        return;
    }

    record.notes.push_back(describe_event(event));
    record.other_income += event.income_change;
    if (std::abs(event.expense_change) > rubles_epsilon) {
        record.expenses[expense_kind::event_expense] += event.expense_change;
    }

    if (event.affects_primary_job_for_months > 0) {
        plan.primary_job.lost_months_left = std::max(plan.primary_job.lost_months_left,
                                                     event.affects_primary_job_for_months);
    }

    if (event.affects_secondary_job_for_months > 0) {
        plan.secondary_job.lost_months_left = std::max(plan.secondary_job.lost_months_left,
                                                       event.affects_secondary_job_for_months);
    }

    if (event.changes_rent) {
        plan.home.monthly_rent.monthly_value *= event.rent_multiplier;
    }
}


void decrease_job_loss_counters(person_plan& plan) {
    if (plan.primary_job.lost_months_left > 0) {
        --plan.primary_job.lost_months_left;
    }

    if (plan.secondary_job.lost_months_left > 0) {
        --plan.secondary_job.lost_months_left;
    }
}


bool try_to_buy_home_without_loan(person_plan& plan,
                                  month_record& record) {
    if (plan.home.mode != housing_mode::rent) {
        return false;
    }

    const double purchase_cost = plan.home.property_price
                                 * (1.0 + apartment_purchase_cost_percent / percent_base);
    const double reserve = calculate_reserve_target(plan, record.date);

    if (plan.savings.balance < purchase_cost + reserve) {
        return false;
    }

    if (!plan.savings.withdraw(purchase_cost)) {
        return false;
    }

    plan.home.mode = housing_mode::owned_home;
    plan.home.property_owned = true;
    plan.home.purchase_month = static_cast<double>(record.date.serial_month());
    record.notes.push_back("Накоплений достаточно: квартира куплена без кредита");
    return true;
}


void fill_year_record(year_record& year,
                      const month_record& month) {
    year.income += month.total_income;
    year.expenses += month.total_expenses;
    year.savings_interest += month.savings_interest;
    year.tax_refund += month.tax_refund;
    year.loan_interest += month.loan_interest_paid;
    year.loan_principal += month.loan_principal_paid;
    year.prepayment += month.mortgage_prepayment;
    year.net_cashflow += month.net_cashflow;

    if (month.net_cashflow < -rubles_epsilon) {
        ++year.negative_cashflow_months;
    }

    if (!month.notes.empty()) {
        year.events_count += static_cast<int>(month.notes.size());
    }

    year.savings_balance_end = month.savings_balance;
    year.mortgage_balance_end = month.mortgage_balance;
    year.net_worth_end = month.net_worth;
}


void add_final_conclusions(simulation_result& result) {
    result.final_net_worth = calculate_net_worth(result.final_plan);
    result.financially_stable = result.negative_cashflow_months == 0
                                && result.final_plan.credit_debt <= rubles_epsilon
                                && result.final_net_worth > 0.0;
    result.conclusions = assess_financial_health(result);
}


}  // namespace


person_plan make_alice_plan() {
    person_plan plan;
    plan.name = "Алиса";
    plan.strategy_name = "Покупка квартиры с ипотекой";
    plan.strategy_description = "Первоначальный взнос вносится сразу. Свободные средства "
                                "направляются на резерв и частичное досрочное погашение.";
    plan.start_date = simulation_date{2026, 1};
    plan.simulation_months = 240;
    plan.primary_job = job{"Инженер-программист", 230'000.0, 7.0, true, 0};
    plan.secondary_job = job{"Преподаватель курсов", 45'000.0, 5.0, true, 0};
    plan.savings = savings_account{280'000.0, 12.0, 0.0, 150'000.0};

    plan.home.mode = housing_mode::mortgage;
    plan.home.monthly_rent = make_expense("Аренда", 0.0, 7.0);
    plan.home.monthly_utilities = make_expense("Коммунальные услуги", 10'500.0, 6.0);
    plan.home.maintenance_reserve = make_expense("Резерв на ремонт", 4'500.0, 7.0);
    plan.home.property_price = 8'000'000.0;
    plan.home.down_payment = 1'700'000.0;
    plan.home.annual_property_growth_percent = 6.0;
    plan.home.property_owned = true;

    initialize_loan(plan.mortgage,
                    loan_kind::mortgage,
                    "Ипотека на квартиру",
                    6'300'000.0,
                    15.5,
                    15 * months_in_year);

    plan.domestic_pet.enabled = true;
    plan.domestic_pet.name = "кот Барсик";
    plan.domestic_pet.lifespan_months = 16 * months_in_year;
    plan.domestic_pet.age_months = 4 * months_in_year;
    plan.domestic_pet.monthly_cost = make_expense("Содержание питомца", 5'300.0, 8.0);

    plan.personal_car.enabled = true;
    plan.personal_car.model = "семейный автомобиль";
    plan.personal_car.fuel = make_expense("Топливо", 8'500.0, 8.0);
    plan.personal_car.maintenance = make_expense("Обслуживание", 4'000.0, 8.0);
    plan.personal_car.parking = make_expense("Парковка", 1'500.0, 5.0);
    plan.personal_car.insurance_month = 4;
    plan.personal_car.insurance_annual_cost = 48'000.0;
    plan.personal_car.annual_insurance_inflation_percent = 7.0;

    plan.regular_expenses = {
        make_expense("Питание", 34'000.0, 8.0),
        make_expense("Транспорт", 2'500.0, 7.0),
        make_expense("Связь", 1'800.0, 4.0),
        make_expense("Здоровье", 3'500.0, 7.0),
        make_expense("Досуг", 7'000.0, 6.0),
        make_expense("Бытовые расходы", 4'000.0, 7.0)
    };

    plan.cash_balance = 0.0;
    plan.credit_limit = 250'000.0;
    plan.emergency_fund_months = 6.0;
    plan.planned_savings_share_percent = 30.0;
    plan.mortgage_prepayment_share_percent = 35.0;
    plan.make_mortgage_prepayment = true;
    plan.receive_tax_refund = true;
    return plan;
}


person_plan make_bob_plan() {
    person_plan plan;
    plan.name = "Боб";
    plan.strategy_name = "Аренда и накопление на квартиру";
    plan.strategy_description = "Жильё арендуется, а весь доступный денежный поток "
                                "накапливается на вкладе до покупки квартиры без кредита.";
    plan.start_date = simulation_date{2026, 1};
    plan.simulation_months = 240;
    plan.primary_job = job{"Инженер-программист", 230'000.0, 7.0, true, 0};
    plan.secondary_job = job{"Преподаватель курсов", 45'000.0, 5.0, true, 0};
    plan.savings = savings_account{1'700'000.0, 12.0, 0.0, 150'000.0};

    plan.home.mode = housing_mode::rent;
    plan.home.monthly_rent = make_expense("Аренда", 55'000.0, 7.0);
    plan.home.monthly_utilities = make_expense("Коммунальные услуги", 10'500.0, 6.0);
    plan.home.maintenance_reserve = make_expense("Резерв на ремонт", 4'500.0, 7.0);
    plan.home.property_price = 8'000'000.0;
    plan.home.down_payment = 0.0;
    plan.home.annual_property_growth_percent = 6.0;
    plan.home.property_owned = false;

    plan.domestic_pet.enabled = true;
    plan.domestic_pet.name = "кот Барсик";
    plan.domestic_pet.lifespan_months = 16 * months_in_year;
    plan.domestic_pet.age_months = 4 * months_in_year;
    plan.domestic_pet.monthly_cost = make_expense("Содержание питомца", 5'300.0, 8.0);

    plan.personal_car.enabled = true;
    plan.personal_car.model = "семейный автомобиль";
    plan.personal_car.fuel = make_expense("Топливо", 8'500.0, 8.0);
    plan.personal_car.maintenance = make_expense("Обслуживание", 4'000.0, 8.0);
    plan.personal_car.parking = make_expense("Парковка", 1'500.0, 5.0);
    plan.personal_car.insurance_month = 4;
    plan.personal_car.insurance_annual_cost = 48'000.0;
    plan.personal_car.annual_insurance_inflation_percent = 7.0;

    plan.regular_expenses = {
        make_expense("Питание", 34'000.0, 8.0),
        make_expense("Транспорт", 2'500.0, 7.0),
        make_expense("Связь", 1'800.0, 4.0),
        make_expense("Здоровье", 3'500.0, 7.0),
        make_expense("Досуг", 7'000.0, 6.0),
        make_expense("Бытовые расходы", 4'000.0, 7.0)
    };

    plan.cash_balance = 280'000.0;
    plan.credit_limit = 250'000.0;
    plan.emergency_fund_months = 6.0;
    plan.planned_savings_share_percent = 100.0;
    plan.mortgage_prepayment_share_percent = 0.0;
    plan.make_mortgage_prepayment = false;
    plan.receive_tax_refund = false;
    return plan;
}


std::vector<random_event> generate_events_for_month(std::mt19937& engine,
                                                     const person_plan& plan,
                                                     const simulation_date& date,
                                                     bool enabled) {
    static_cast<void>(plan);
    static_cast<void>(date);

    if (!enabled) {
        return {};
    }

    std::uniform_int_distribution<int> probability(1, 10'000);
    const int roll = probability(engine);
    std::vector<random_event> events;

    if (roll <= 110) {
        events.push_back(random_event{event_kind::temporary_job_loss,
                                      "Временная потеря основной работы",
                                      0.0,
                                      0.0,
                                      2,
                                      0,
                                      false,
                                      1.0});
    } else if (roll <= 430) {
        events.push_back(random_event{event_kind::medical_expense,
                                      "Незапланированные медицинские расходы",
                                      0.0,
                                      45'000.0,
                                      0,
                                      0,
                                      false,
                                      1.0});
    } else if (roll <= 720) {
        events.push_back(random_event{event_kind::car_repair,
                                      "Ремонт автомобиля",
                                      0.0,
                                      35'000.0,
                                      0,
                                      0,
                                      false,
                                      1.0});
    } else if (roll <= 870) {
        events.push_back(random_event{event_kind::pet_treatment,
                                      "Лечение домашнего животного",
                                      0.0,
                                      22'000.0,
                                      0,
                                      0,
                                      false,
                                      1.0});
    } else if (roll <= 990) {
        events.push_back(random_event{event_kind::salary_bonus,
                                      "Премия по основной работе",
                                      60'000.0,
                                      0.0,
                                      0,
                                      0,
                                      false,
                                      1.0});
    } else if (roll <= 1'180) {
        events.push_back(random_event{event_kind::freelance_order,
                                      "Разовый заказ на фрилансе",
                                      35'000.0,
                                      0.0,
                                      0,
                                      0,
                                      false,
                                      1.0});
    } else if (roll <= 1'240) {
        events.push_back(random_event{event_kind::rent_increase,
                                      "Внеплановый рост арендной платы",
                                      0.0,
                                      0.0,
                                      0,
                                      0,
                                      true,
                                      1.10});
    } else if (roll <= 1'360) {
        events.push_back(random_event{event_kind::home_repair,
                                      "Небольшой ремонт жилья",
                                      0.0,
                                      40'000.0,
                                      0,
                                      0,
                                      false,
                                      1.0});
    } else if (roll <= 1'520) {
        events.push_back(random_event{event_kind::food_discount,
                                      "Экономия на продуктах по акции",
                                      0.0,
                                      -8'000.0,
                                      0,
                                      0,
                                      false,
                                      1.0});
    }

    return events;
}


simulation_result run_simulation(person_plan plan,
                                 bool random_events_enabled,
                                 std::uint32_t seed) {
    simulation_result result;
    result.final_plan = plan;
    std::mt19937 engine(seed);
    simulation_date date = plan.start_date;
    double previous_year_income_tax = 0.0;
    double previous_year_mortgage_interest = 0.0;
    double previous_year_property_cost = plan.home.down_payment + plan.mortgage.initial_principal;
    double current_year_income_tax = 0.0;
    year_record current_year;
    current_year.year = date.year;

    for (int step = 0; step < plan.simulation_months; ++step) {
        if (date.january() && step > 0) {
            apply_inflation_for_new_year(plan);
        }

        month_record record;
        record.date = date;

        const std::vector<random_event> events = generate_events_for_month(engine,
                                                                            plan,
                                                                            date,
                                                                            random_events_enabled);
        for (const random_event& event : events) {
            apply_event(plan, event, record);
        }

        record.primary_salary = net_salary_for_month(plan.primary_job, plan.taxes);
        record.secondary_salary = net_salary_for_month(plan.secondary_job, plan.taxes);
        const double gross_salary = gross_salary_for_month(plan);
        const double income_tax = calculate_income_tax(gross_salary, plan.taxes.income_tax_percent);
        current_year_income_tax += income_tax;
        record.savings_interest = plan.savings.accrue_interest();

        if (date.january() && step > 0 && plan.receive_tax_refund) {
            const double property_refund = calculate_property_tax_refund(plan.taxes,
                                                                          previous_year_property_cost,
                                                                          previous_year_income_tax);
            const double remaining_tax = std::max(0.0, previous_year_income_tax - property_refund);
            const double interest_refund = calculate_interest_tax_refund(plan.taxes,
                                                                          previous_year_mortgage_interest,
                                                                          remaining_tax);
            record.tax_refund = property_refund + interest_refund;
            if (record.tax_refund > rubles_epsilon) {
                record.notes.push_back("Получен налоговый вычет за предыдущий год");
            }
        }

        record.total_income = record.primary_salary
                              + record.secondary_salary
                              + record.savings_interest
                              + record.tax_refund
                              + record.other_income;

        record.regular_expenses_total = regular_expenses_for_month(plan, record, date);
        pay_required_loans(plan, record);
        const double credit_card_interest = calculate_credit_card_interest(plan);
        if (credit_card_interest > rubles_epsilon) {
            plan.credit_debt += credit_card_interest;
            add_expense(record, expense_kind::loan_payment, credit_card_interest);
            record.loan_interest_paid += credit_card_interest;
        }

        record.total_expenses = sum_expenses(record.expenses);
        record.net_cashflow = record.total_income - record.total_expenses;

        if (record.net_cashflow > rubles_epsilon) {
            double surplus = record.net_cashflow;
            const double credit_card_payment = pay_credit_card_from_surplus(plan, record, surplus);
            surplus -= credit_card_payment;
            record.total_expenses += credit_card_payment;
            record.net_cashflow -= credit_card_payment;

            const double prepayment = apply_mortgage_prepayment(plan, record, surplus);
            surplus -= prepayment;
            record.total_expenses += prepayment;
            record.net_cashflow -= prepayment;

            // Проценты уже зачислены на вклад в accrue_interest().
            // В новый взнос направляется только внешний денежный остаток.
            const double new_savings_deposit = std::max(0.0,
                                                        surplus - record.savings_interest);
            plan.savings.deposit(new_savings_deposit);
            try_to_buy_home_without_loan(plan, record);
        } else if (record.net_cashflow < -rubles_epsilon) {
            double shortage = -record.net_cashflow;
            const double from_cash = std::min(plan.cash_balance, shortage);
            plan.cash_balance -= from_cash;
            shortage -= from_cash;

            const double reserve = calculate_reserve_target(plan, date);
            const double available_savings = std::max(0.0, plan.savings.balance - reserve);
            const double from_savings = std::min(available_savings, shortage);
            plan.savings.withdraw(from_savings);
            shortage -= from_savings;

            if (shortage > rubles_epsilon) {
                const double available_credit = std::max(0.0, plan.credit_limit - plan.credit_debt);
                const double borrowed = std::min(shortage, available_credit);
                plan.credit_debt += borrowed;
                shortage -= borrowed;
            }

            if (shortage > rubles_epsilon) {
                ++result.credit_limit_breaches;
                record.notes.push_back("Недостаточно средств даже с учётом кредитного лимита");
            }
        }

        decrease_job_loss_counters(plan);
        plan.domestic_pet.grow_one_month();

        record.savings_balance = plan.savings.balance;
        record.cash_balance = plan.cash_balance;
        record.credit_debt = plan.credit_debt;
        record.mortgage_balance = plan.mortgage.remaining_principal;
        record.net_worth = calculate_net_worth(plan);

        result.total_income += record.total_income;
        result.total_expenses += record.total_expenses;
        result.total_interest_income += record.savings_interest;
        result.total_tax_refund += record.tax_refund;
        result.total_loan_interest += record.loan_interest_paid;
        result.total_loan_principal += record.loan_principal_paid;
        result.total_prepayment += record.mortgage_prepayment;

        if (record.net_cashflow < -rubles_epsilon) {
            ++result.negative_cashflow_months;
        }

        if (result.mortgage_paid_month < 0
            && plan.mortgage.initial_principal > rubles_epsilon
            && plan.mortgage.paid_off()) {
            result.mortgage_paid_month = step + 1;
        }

        fill_year_record(current_year, record);
        result.months.push_back(record);

        if (date.december() || step == plan.simulation_months - 1) {
            result.years.push_back(current_year);
            previous_year_income_tax = current_year_income_tax;
            previous_year_mortgage_interest = current_year.loan_interest;
            previous_year_property_cost = plan.home.property_price;
            current_year_income_tax = 0.0;
            current_year = year_record{};
            current_year.year = date.year + 1;
        }

        if (plan.stop_when_mortgage_paid && plan.mortgage.paid_off()) {
            break;
        }

        date.advance_one_month();
    }

    result.final_plan = plan;
    add_final_conclusions(result);
    return result;
}


comparison_result compare_default_strategies(bool random_events_enabled,
                                             std::uint32_t seed) {
    comparison_result comparison;
    comparison.alice = run_simulation(make_alice_plan(), random_events_enabled, seed);
    comparison.bob = run_simulation(make_bob_plan(), random_events_enabled, seed);
    comparison.net_worth_difference = comparison.alice.final_net_worth
                                      - comparison.bob.final_net_worth;

    if (std::abs(comparison.net_worth_difference) < 10'000.0) {
        comparison.winner_name = "Ничья";
        comparison.conclusions.push_back("За 20 лет стратегии дают сопоставимый чистый капитал.");
    } else if (comparison.net_worth_difference > 0.0) {
        comparison.winner_name = comparison.alice.final_plan.name;
        comparison.conclusions.push_back("Ипотечная стратегия даёт больший чистый капитал в заданных условиях.");
    } else {
        comparison.winner_name = comparison.bob.final_plan.name;
        comparison.conclusions.push_back("Стратегия аренды и накопления даёт больший чистый капитал в заданных условиях.");
    }

    const bool alice_owns_home = comparison.alice.final_plan.home.property_owned;
    const bool bob_owns_home = comparison.bob.final_plan.home.property_owned;
    comparison.conclusions.push_back("К концу периода жильё Алисы: "
                                     + housing_text(comparison.alice.final_plan.home.mode)
                                     + "; жильё Боба: "
                                     + housing_text(comparison.bob.final_plan.home.mode) + ".");

    if (alice_owns_home && !bob_owns_home) {
        comparison.conclusions.push_back("Алиса фиксирует покупку жилья раньше; Боб продолжает зависеть от арендной платы.");
    }

    if (bob_owns_home && !alice_owns_home) {
        comparison.conclusions.push_back("Боб смог приобрести жильё без кредита до окончания горизонта моделирования.");
    }

    if (comparison.alice.negative_cashflow_months > comparison.bob.negative_cashflow_months) {
        comparison.conclusions.push_back("Ипотечная стратегия требует более крупного резерва: в ней больше месяцев с дефицитом бюджета.");
    }

    if (comparison.bob.negative_cashflow_months > comparison.alice.negative_cashflow_months) {
        comparison.conclusions.push_back("Арендная стратегия хуже переносит заданные случайные события из-за роста арендной платы.");
    }

    return comparison;
}


monte_carlo_summary run_ab_experiment(int runs, std::uint32_t base_seed) {
    monte_carlo_summary summary;
    summary.runs = std::max(0, runs);

    for (int index = 0; index < summary.runs; ++index) {
        const comparison_result comparison = compare_default_strategies(true,
                                                                         base_seed
                                                                         + static_cast<std::uint32_t>(index));
        const simulation_result& alice = comparison.alice;
        const simulation_result& bob = comparison.bob;

        summary.alice_average_net_worth += alice.final_net_worth;
        summary.bob_average_net_worth += bob.final_net_worth;
        summary.alice_average_negative_months += alice.negative_cashflow_months;
        summary.bob_average_negative_months += bob.negative_cashflow_months;
        summary.alice_min_net_worth = std::min(summary.alice_min_net_worth, alice.final_net_worth);
        summary.alice_max_net_worth = std::max(summary.alice_max_net_worth, alice.final_net_worth);
        summary.bob_min_net_worth = std::min(summary.bob_min_net_worth, bob.final_net_worth);
        summary.bob_max_net_worth = std::max(summary.bob_max_net_worth, bob.final_net_worth);

        if (comparison.winner_name == alice.final_plan.name) {
            ++summary.alice_wins;
        } else if (comparison.winner_name == bob.final_plan.name) {
            ++summary.bob_wins;
        } else {
            ++summary.draws;
        }
    }

    if (summary.runs > 0) {
        const double divisor = static_cast<double>(summary.runs);
        summary.alice_average_net_worth /= divisor;
        summary.bob_average_net_worth /= divisor;
        summary.alice_average_negative_months /= divisor;
        summary.bob_average_negative_months /= divisor;
    } else {
        summary.alice_min_net_worth = 0.0;
        summary.alice_max_net_worth = 0.0;
        summary.bob_min_net_worth = 0.0;
        summary.bob_max_net_worth = 0.0;
    }

    return summary;
}


double calculate_net_worth(const person_plan& plan) {
    double liabilities = plan.mortgage.remaining_principal + plan.credit_debt;
    for (const loan& current_loan : plan.additional_loans) {
        liabilities += current_loan.remaining_principal;
    }

    const double property_asset = plan.home.property_owned ? plan.home.property_price : 0.0;
    return plan.savings.balance + plan.cash_balance + property_asset - liabilities;
}


double calculate_monthly_regular_cost(const person_plan& plan, const simulation_date& date) {
    double total = 0.0;
    for (const indexed_amount& expense : plan.regular_expenses) {
        total += expense.monthly_value;
    }

    total += plan.home.monthly_utilities.monthly_value;
    total += plan.home.maintenance_reserve.monthly_value;

    if (plan.home.mode == housing_mode::rent) {
        total += plan.home.monthly_rent.monthly_value;
    }

    if (plan.domestic_pet.alive()) {
        total += plan.domestic_pet.monthly_cost.monthly_value;
    }

    total += car_cost_for_month(plan, date);
    return total;
}


std::vector<std::string> assess_financial_health(const simulation_result& result) {
    std::vector<std::string> conclusions;
    const person_plan& plan = result.final_plan;
    const double reserve = calculate_reserve_target(plan, plan.start_date);

    if (result.financially_stable) {
        conclusions.push_back("Модель финансово устойчива: дефицитных месяцев и непогашенного потребительского долга нет.");
    } else {
        conclusions.push_back("Модель требует корректировки: встречается дефицит бюджета, долг по карте или отрицательный капитал.");
    }

    if (plan.savings.balance >= reserve) {
        conclusions.push_back("Резерв превышает целевой уровень " + money(reserve) + ".");
    } else {
        conclusions.push_back("Резерв ниже целевого уровня " + money(reserve) + ".");
    }

    if (result.mortgage_paid_month > 0) {
        const int years = result.mortgage_paid_month / months_in_year;
        const int months = result.mortgage_paid_month % months_in_year;
        conclusions.push_back("Ипотека закрыта за " + std::to_string(years)
                              + " лет " + std::to_string(months) + " месяцев.");
    } else if (plan.mortgage.active) {
        conclusions.push_back("К концу горизонта ипотека ещё не погашена; остаток составляет "
                              + money(plan.mortgage.remaining_principal) + ".");
    }

    if (result.total_tax_refund > rubles_epsilon) {
        conclusions.push_back("Налоговые вычеты вернули в бюджет " + money(result.total_tax_refund) + ".");
    }

    if (result.total_prepayment > rubles_epsilon) {
        conclusions.push_back("На досрочное погашение ипотеки направлено "
                              + money(result.total_prepayment) + ".");
    }

    if (result.credit_limit_breaches > 0) {
        conclusions.push_back("Зафиксированы случаи превышения доступного кредитного лимита: "
                              + std::to_string(result.credit_limit_breaches) + ".");
    }

    return conclusions;
}


std::string describe_event(const random_event& event) {
    return event.title;
}

}  // namespace alice_bob

// ===== Объявления диагностики и стресс-тестов =====



namespace alice_bob {

enum class risk_level {
    low,
    moderate,
    high,
    critical
};


struct loan_schedule_row {
    int month_number = 0;
    double starting_balance = 0.0;
    double payment = 0.0;
    double interest = 0.0;
    double principal = 0.0;
    double extra_payment = 0.0;
    double ending_balance = 0.0;
};


struct affordability_metrics {
    double gross_monthly_income = 0.0;
    double net_monthly_income = 0.0;
    double monthly_housing_cost = 0.0;
    double monthly_debt_payment = 0.0;
    double regular_monthly_cost = 0.0;
    double debt_to_income_percent = 0.0;
    double housing_to_income_percent = 0.0;
    double emergency_fund_target = 0.0;
    double actual_reserve = 0.0;
    double reserve_coverage_months = 0.0;
    double maximum_safe_debt_payment = 0.0;
    double maximum_safe_mortgage_principal = 0.0;
    risk_level risk = risk_level::moderate;
    std::vector<std::string> explanations;
};


struct prepayment_analysis {
    double monthly_extra_payment = 0.0;
    int months_without_prepayment = 0;
    int months_with_prepayment = 0;
    double interest_without_prepayment = 0.0;
    double interest_with_prepayment = 0.0;
    double saved_interest = 0.0;
    int saved_months = 0;
};


struct stress_case {
    std::string title;
    std::string description;
    double primary_salary_multiplier = 1.0;
    double secondary_salary_multiplier = 1.0;
    double mortgage_rate_delta_percent = 0.0;
    double rent_multiplier = 1.0;
    double expense_inflation_delta_percent = 0.0;
    double deposit_rate_delta_percent = 0.0;
    bool random_events_enabled = true;
};


struct stress_result {
    stress_case configuration;
    simulation_result result;
    double net_worth_change = 0.0;
    bool remains_stable = false;
    std::vector<std::string> notes;
};


struct strategy_diagnostics {
    affordability_metrics affordability;
    prepayment_analysis prepayment;
    std::vector<stress_result> stress_results;
};


std::vector<loan_schedule_row> build_loan_schedule(const loan& initial_loan,
                                                    double monthly_extra_payment = 0.0);

double calculate_schedule_interest(const std::vector<loan_schedule_row>& schedule);

double calculate_schedule_cost(const std::vector<loan_schedule_row>& schedule);

double calculate_maximum_principal(double affordable_payment,
                                   double annual_rate_percent,
                                   int term_months);

double calculate_net_monthly_income(const person_plan& plan);

double calculate_required_monthly_debt_payment(const person_plan& plan);

double calculate_monthly_housing_cost(const person_plan& plan);

double calculate_actual_reserve(const person_plan& plan);

risk_level classify_risk(double debt_to_income_percent,
                         double reserve_coverage_months,
                         bool has_credit_card_debt);

std::string risk_level_text(risk_level level);

affordability_metrics analyze_affordability(const person_plan& plan);

prepayment_analysis analyze_prepayment(const loan& value,
                                       double monthly_extra_payment);

person_plan apply_stress_case(person_plan plan, const stress_case& stress);

std::vector<stress_case> make_default_stress_cases();

stress_result run_stress_case(const person_plan& base_plan,
                              const simulation_result& base_result,
                              const stress_case& stress,
                              std::uint32_t seed);

std::vector<stress_result> run_default_stress_tests(const person_plan& base_plan,
                                                    const simulation_result& base_result,
                                                    std::uint32_t seed);

strategy_diagnostics diagnose_strategy(const person_plan& plan,
                                       const simulation_result& base_result,
                                       std::uint32_t seed);

}  // namespace alice_bob

// ===== Долговая нагрузка, досрочное погашение и стресс-тесты =====



namespace alice_bob {

namespace {

constexpr double safe_debt_to_income_percent = 35.0;
constexpr double caution_debt_to_income_percent = 45.0;
constexpr double high_debt_to_income_percent = 60.0;
constexpr double low_reserve_coverage_months = 3.0;
constexpr double good_reserve_coverage_months = 6.0;
constexpr double diagnostics_credit_card_minimum_payment_percent = 10.0;


double non_negative(double value) {
    return std::max(0.0, value);
}


double total_additional_loan_payment(const person_plan& plan) {
    double payment = 0.0;
    for (const loan& value : plan.additional_loans) {
        if (value.active) {
            payment += value.required_monthly_payment;
        }
    }
    return payment;
}


double total_regular_expenses_without_housing(const person_plan& plan) {
    double sum = 0.0;
    for (const indexed_amount& value : plan.regular_expenses) {
        sum += value.monthly_value;
    }

    if (plan.domestic_pet.alive()) {
        sum += plan.domestic_pet.monthly_cost.monthly_value;
    }

    if (plan.personal_car.enabled) {
        sum += plan.personal_car.fuel.monthly_value;
        sum += plan.personal_car.maintenance.monthly_value;
        sum += plan.personal_car.parking.monthly_value;
        sum += plan.personal_car.insurance_annual_cost / months_in_year;
    }

    return sum;
}


double calculate_loan_interest_by_simulation(const loan& initial_loan,
                                             double monthly_extra_payment) {
    const std::vector<loan_schedule_row> schedule = build_loan_schedule(initial_loan,
                                                                         monthly_extra_payment);
    return calculate_schedule_interest(schedule);
}


int schedule_months_by_simulation(const loan& initial_loan,
                                  double monthly_extra_payment) {
    return static_cast<int>(build_loan_schedule(initial_loan, monthly_extra_payment).size());
}


void add_affordability_explanations(affordability_metrics& result,
                                    const person_plan& plan) {
    if (result.debt_to_income_percent <= safe_debt_to_income_percent) {
        result.explanations.push_back("Долговая нагрузка не превышает 35% дохода после налога.");
    } else if (result.debt_to_income_percent <= caution_debt_to_income_percent) {
        result.explanations.push_back("Долговая нагрузка находится в зоне внимания: от 35% до 45% дохода.");
    } else {
        result.explanations.push_back("Долговая нагрузка выше рекомендуемого порога 45% дохода.");
    }

    if (result.reserve_coverage_months < low_reserve_coverage_months) {
        result.explanations.push_back("Ликвидный резерв меньше трёх месяцев регулярных расходов.");
    } else if (result.reserve_coverage_months < good_reserve_coverage_months) {
        result.explanations.push_back("Резерв частично защищает бюджет, но пока меньше шести месяцев расходов.");
    } else {
        result.explanations.push_back("Ликвидный резерв покрывает не менее шести месяцев регулярных расходов.");
    }

    if (plan.home.mode == housing_mode::rent) {
        result.explanations.push_back("Арендная плата зависит от рыночной инфляции и может меняться вне плана.");
    }

    if (plan.mortgage.active) {
        result.explanations.push_back("При досрочном погашении уменьшается основной долг, а не очередной процентный платёж.");
    }

    if (plan.credit_debt > rubles_epsilon) {
        result.explanations.push_back("Долг по кредитной карте должен погашаться раньше дополнительных вложений и досрочной ипотеки.");
    }
}


void add_stress_notes(stress_result& stress, const simulation_result& base_result) {
    if (stress.result.final_net_worth < 0.0) {
        stress.notes.push_back("Чистый капитал стал отрицательным: стратегия в таком сценарии неустойчива.");
    }

    if (stress.result.negative_cashflow_months > base_result.negative_cashflow_months) {
        stress.notes.push_back("Количество месяцев с дефицитом бюджета увеличилось.");
    }

    if (stress.result.credit_limit_breaches > 0) {
        stress.notes.push_back("Модель исчерпала доступный кредитный лимит хотя бы в одном месяце.");
    }

    if (stress.result.final_plan.credit_debt > rubles_epsilon) {
        stress.notes.push_back("К концу периода остался долг по кредитной карте.");
    }

    if (stress.notes.empty()) {
        stress.notes.push_back("Стратегия сохраняет положительный капитал и выдерживает этот стресс-сценарий.");
    }
}


}  // namespace


std::vector<loan_schedule_row> build_loan_schedule(const loan& initial_loan,
                                                    double monthly_extra_payment) {
    std::vector<loan_schedule_row> schedule;
    loan current_loan = initial_loan;
    const int maximum_months = std::max(1, initial_loan.remaining_term_months + 360);

    for (int month = 1; month <= maximum_months && current_loan.active; ++month) {
        loan_schedule_row row;
        row.month_number = month;
        row.starting_balance = current_loan.remaining_principal;
        const loan_payment payment = pay_loan_month(current_loan, monthly_extra_payment);
        row.payment = payment.scheduled_payment;
        row.interest = payment.interest_part;
        row.principal = payment.principal_part;
        row.extra_payment = payment.extra_principal;
        row.ending_balance = current_loan.remaining_principal;
        schedule.push_back(row);

        if (payment.paid_off) {
            break;
        }
    }

    return schedule;
}


double calculate_schedule_interest(const std::vector<loan_schedule_row>& schedule) {
    return std::accumulate(schedule.begin(), schedule.end(), 0.0,
                           [](double total, const loan_schedule_row& row) {
                               return total + row.interest;
                           });
}


double calculate_schedule_cost(const std::vector<loan_schedule_row>& schedule) {
    return std::accumulate(schedule.begin(), schedule.end(), 0.0,
                           [](double total, const loan_schedule_row& row) {
                               return total + row.payment + row.extra_payment;
                           });
}


double calculate_maximum_principal(double affordable_payment,
                                   double annual_rate_percent,
                                   int term_months) {
    if (affordable_payment <= rubles_epsilon || term_months <= 0) {
        return 0.0;
    }

    const double monthly_rate = annual_rate_percent / percent_base / months_in_year;
    if (std::abs(monthly_rate) < rubles_epsilon) {
        return affordable_payment * term_months;
    }

    const double factor = 1.0 - std::pow(1.0 + monthly_rate, -term_months);
    return round_money(affordable_payment * factor / monthly_rate);
}


double calculate_net_monthly_income(const person_plan& plan) {
    const double gross = plan.primary_job.salary_for_month() + plan.secondary_job.salary_for_month();
    return gross - calculate_income_tax(gross, plan.taxes.income_tax_percent);
}


double calculate_required_monthly_debt_payment(const person_plan& plan) {
    double payment = total_additional_loan_payment(plan);
    if (plan.mortgage.active) {
        payment += plan.mortgage.required_monthly_payment;
    }

    if (plan.credit_debt > rubles_epsilon) {
        payment += plan.credit_debt * diagnostics_credit_card_minimum_payment_percent / percent_base;
    }

    return payment;
}


double calculate_monthly_housing_cost(const person_plan& plan) {
    double cost = plan.home.monthly_utilities.monthly_value + plan.home.maintenance_reserve.monthly_value;
    if (plan.home.mode == housing_mode::rent) {
        cost += plan.home.monthly_rent.monthly_value;
    }

    if (plan.home.mode == housing_mode::mortgage && plan.mortgage.active) {
        cost += plan.mortgage.required_monthly_payment;
    }

    return cost;
}


double calculate_actual_reserve(const person_plan& plan) {
    return std::max(0.0, plan.savings.balance - plan.savings.minimum_balance) + plan.cash_balance;
}


risk_level classify_risk(double debt_to_income_percent,
                         double reserve_coverage_months,
                         bool has_credit_card_debt) {
    if (debt_to_income_percent > high_debt_to_income_percent
        || reserve_coverage_months < 1.0
        || has_credit_card_debt) {
        return risk_level::critical;
    }

    if (debt_to_income_percent > caution_debt_to_income_percent
        || reserve_coverage_months < low_reserve_coverage_months) {
        return risk_level::high;
    }

    if (debt_to_income_percent > safe_debt_to_income_percent
        || reserve_coverage_months < good_reserve_coverage_months) {
        return risk_level::moderate;
    }

    return risk_level::low;
}


std::string risk_level_text(risk_level level) {
    switch (level) {
        case risk_level::low:
            return "низкий";
        case risk_level::moderate:
            return "умеренный";
        case risk_level::high:
            return "высокий";
        case risk_level::critical:
            return "критический";
    }
    return "не определён";
}


affordability_metrics analyze_affordability(const person_plan& plan) {
    affordability_metrics result;
    result.gross_monthly_income = plan.primary_job.salary_for_month()
                                  + plan.secondary_job.salary_for_month();
    result.net_monthly_income = calculate_net_monthly_income(plan);
    result.monthly_housing_cost = calculate_monthly_housing_cost(plan);
    result.monthly_debt_payment = calculate_required_monthly_debt_payment(plan);
    result.regular_monthly_cost = total_regular_expenses_without_housing(plan)
                                  + result.monthly_housing_cost;
    result.debt_to_income_percent = debt_to_income_ratio(result.monthly_debt_payment,
                                                          result.net_monthly_income);
    result.housing_to_income_percent = housing_cost_ratio(result.monthly_housing_cost,
                                                          result.net_monthly_income);
    result.emergency_fund_target = safe_emergency_fund(result.regular_monthly_cost,
                                                       plan.emergency_fund_months);
    result.actual_reserve = calculate_actual_reserve(plan);
    result.reserve_coverage_months = result.regular_monthly_cost > rubles_epsilon
                                         ? result.actual_reserve / result.regular_monthly_cost
                                         : 0.0;
    result.maximum_safe_debt_payment = monthly_payment_for_income_ratio(result.net_monthly_income,
                                                                         safe_debt_to_income_percent);
    result.maximum_safe_mortgage_principal = calculate_maximum_principal(result.maximum_safe_debt_payment,
                                                                          plan.mortgage.annual_rate_percent,
                                                                          plan.mortgage.original_term_months);
    result.risk = classify_risk(result.debt_to_income_percent,
                                result.reserve_coverage_months,
                                plan.credit_debt > rubles_epsilon);
    add_affordability_explanations(result, plan);
    return result;
}


prepayment_analysis analyze_prepayment(const loan& value,
                                       double monthly_extra_payment) {
    prepayment_analysis result;
    result.monthly_extra_payment = non_negative(monthly_extra_payment);
    result.months_without_prepayment = schedule_months_by_simulation(value, 0.0);
    result.months_with_prepayment = schedule_months_by_simulation(value, result.monthly_extra_payment);
    result.interest_without_prepayment = calculate_loan_interest_by_simulation(value, 0.0);
    result.interest_with_prepayment = calculate_loan_interest_by_simulation(value,
                                                                             result.monthly_extra_payment);
    result.saved_interest = std::max(0.0,
                                     result.interest_without_prepayment
                                     - result.interest_with_prepayment);
    result.saved_months = std::max(0,
                                   result.months_without_prepayment
                                   - result.months_with_prepayment);
    return result;
}


person_plan apply_stress_case(person_plan plan, const stress_case& stress) {
    plan.primary_job.monthly_salary *= non_negative(stress.primary_salary_multiplier);
    plan.secondary_job.monthly_salary *= non_negative(stress.secondary_salary_multiplier);
    plan.savings.annual_rate_percent = non_negative(plan.savings.annual_rate_percent
                                                    + stress.deposit_rate_delta_percent);
    plan.home.monthly_rent.monthly_value *= non_negative(stress.rent_multiplier);

    for (indexed_amount& expense : plan.regular_expenses) {
        expense.annual_inflation_percent = non_negative(expense.annual_inflation_percent
                                                        + stress.expense_inflation_delta_percent);
    }

    plan.home.monthly_rent.annual_inflation_percent = non_negative(
        plan.home.monthly_rent.annual_inflation_percent + stress.expense_inflation_delta_percent);
    plan.home.monthly_utilities.annual_inflation_percent = non_negative(
        plan.home.monthly_utilities.annual_inflation_percent + stress.expense_inflation_delta_percent);
    plan.home.maintenance_reserve.annual_inflation_percent = non_negative(
        plan.home.maintenance_reserve.annual_inflation_percent + stress.expense_inflation_delta_percent);

    if (plan.mortgage.active) {
        const double new_rate = non_negative(plan.mortgage.annual_rate_percent
                                             + stress.mortgage_rate_delta_percent);
        initialize_loan(plan.mortgage,
                        loan_kind::mortgage,
                        plan.mortgage.name,
                        plan.mortgage.initial_principal,
                        new_rate,
                        plan.mortgage.original_term_months);
    }

    return plan;
}


std::vector<stress_case> make_default_stress_cases() {
    return {
        {"Базовый сценарий",
         "Исходные параметры без дополнительного ухудшения условий.",
         1.0, 1.0, 0.0, 1.0, 0.0, 0.0, false},
        {"Снижение основной зарплаты на 20%",
         "Проверка способности бюджета пережить сокращение заработка.",
         0.8, 1.0, 0.0, 1.0, 0.0, 0.0, true},
        {"Рост ипотечной ставки на 3 пункта",
         "Стресс для кредитной стратегии при рефинансировании или новом договоре.",
         1.0, 1.0, 3.0, 1.0, 0.0, 0.0, true},
        {"Инфляционный шок расходов",
         "Ежегодная инфляция регулярных расходов повышается на 5 процентных пунктов.",
         1.0, 1.0, 0.0, 1.0, 5.0, -2.0, true},
        {"Снижение доходности вклада на 5 п.п.",
         "Проверка финансового резерва при уменьшении банковской ставки.",
         1.0, 1.0, 0.0, 1.0, 0.0, -5.0, true},
        {"Одновременный неблагоприятный сценарий",
         "Зарплата ниже на 15%, расходы дорожают, а доходность вклада уменьшается.",
         0.85, 0.9, 2.0, 1.15, 4.0, -4.0, true}
    };
}


stress_result run_stress_case(const person_plan& base_plan,
                              const simulation_result& base_result,
                              const stress_case& stress,
                              std::uint32_t seed) {
    stress_result result;
    result.configuration = stress;
    const person_plan stressed_plan = apply_stress_case(base_plan, stress);
    result.result = run_simulation(stressed_plan, stress.random_events_enabled, seed);
    result.net_worth_change = result.result.final_net_worth - base_result.final_net_worth;
    result.remains_stable = result.result.financially_stable;
    add_stress_notes(result, base_result);
    return result;
}


std::vector<stress_result> run_default_stress_tests(const person_plan& base_plan,
                                                    const simulation_result& base_result,
                                                    std::uint32_t seed) {
    std::vector<stress_result> results;
    const std::vector<stress_case> cases = make_default_stress_cases();

    for (std::size_t index = 0; index < cases.size(); ++index) {
        results.push_back(run_stress_case(base_plan,
                                          base_result,
                                          cases[index],
                                          seed + static_cast<std::uint32_t>(index)));
    }

    return results;
}


strategy_diagnostics diagnose_strategy(const person_plan& plan,
                                       const simulation_result& base_result,
                                       std::uint32_t seed) {
    strategy_diagnostics diagnostics;
    diagnostics.affordability = analyze_affordability(plan);
    diagnostics.prepayment = analyze_prepayment(plan.mortgage,
                                                plan.mortgage.required_monthly_payment * 0.2);
    diagnostics.stress_results = run_default_stress_tests(plan, base_result, seed);
    return diagnostics;
}

}  // namespace alice_bob

// ===== Объявления статистики и оценки качества =====



namespace alice_bob {

struct value_range {
    double minimum = 0.0;
    double maximum = 0.0;
    double average = 0.0;
    double median = 0.0;
};


struct category_summary {
    expense_kind category = expense_kind::food;
    double total = 0.0;
    double monthly_average = 0.0;
    double share_percent = 0.0;
    int non_zero_months = 0;
};


struct event_summary {
    std::string title;
    int occurrences = 0;
};


struct capital_milestone {
    double target = 0.0;
    int month_number = -1;
    simulation_date date;
};


struct life_quality_score {
    int total = 0;
    int budget_stability = 0;
    int reserve_quality = 0;
    int debt_quality = 0;
    int housing_quality = 0;
    int growth_quality = 0;
    std::string grade;
    std::vector<std::string> explanations;
};


struct model_statistics {
    value_range monthly_income;
    value_range monthly_expenses;
    value_range monthly_cashflow;
    value_range monthly_net_worth;
    double total_regular_expenses = 0.0;
    double total_event_expenses = 0.0;
    double total_housing_expenses = 0.0;
    double total_food_expenses = 0.0;
    double average_monthly_savings_change = 0.0;
    double annualized_net_worth_growth_percent = 0.0;
    int months_with_positive_cashflow = 0;
    int months_with_negative_cashflow = 0;
    int months_with_zero_cashflow = 0;
    month_record best_month;
    month_record worst_month;
    std::vector<category_summary> categories;
    std::vector<event_summary> events;
    std::vector<capital_milestone> milestones;
    life_quality_score quality;
};


value_range calculate_range(const std::vector<double>& values);

double calculate_median(std::vector<double> values);

double calculate_total_for_category(const simulation_result& result, expense_kind category);

double calculate_average_for_category(const simulation_result& result, expense_kind category);

double calculate_category_share(const simulation_result& result, expense_kind category);

std::vector<category_summary> summarize_expense_categories(const simulation_result& result);

std::vector<event_summary> summarize_events(const simulation_result& result);

month_record find_best_cashflow_month(const simulation_result& result);

month_record find_worst_cashflow_month(const simulation_result& result);

double calculate_annualized_growth(double start_value, double finish_value, int months);

std::vector<capital_milestone> find_capital_milestones(const simulation_result& result,
                                                       const std::vector<double>& targets);

life_quality_score calculate_life_quality_score(const simulation_result& result);

model_statistics calculate_model_statistics(const simulation_result& result);

std::vector<std::string> compare_cashflow_profiles(const simulation_result& alice,
                                                   const simulation_result& bob);

}  // namespace alice_bob

// ===== Сводная статистика результатов =====



namespace alice_bob {

namespace {

constexpr int score_per_dimension = 20;
constexpr int excellent_score = 85;
constexpr int good_score = 70;
constexpr int satisfactory_score = 50;


std::vector<double> extract_income(const simulation_result& result) {
    std::vector<double> values;
    values.reserve(result.months.size());
    for (const month_record& record : result.months) {
        values.push_back(record.total_income);
    }
    return values;
}


std::vector<double> extract_expenses(const simulation_result& result) {
    std::vector<double> values;
    values.reserve(result.months.size());
    for (const month_record& record : result.months) {
        values.push_back(record.total_expenses);
    }
    return values;
}


std::vector<double> extract_cashflow(const simulation_result& result) {
    std::vector<double> values;
    values.reserve(result.months.size());
    for (const month_record& record : result.months) {
        values.push_back(record.net_cashflow);
    }
    return values;
}


std::vector<double> extract_net_worth(const simulation_result& result) {
    std::vector<double> values;
    values.reserve(result.months.size());
    for (const month_record& record : result.months) {
        values.push_back(record.net_worth);
    }
    return values;
}


std::string grade_for_score(int score) {
    if (score >= excellent_score) {
        return "отличная";
    }

    if (score >= good_score) {
        return "хорошая";
    }

    if (score >= satisfactory_score) {
        return "удовлетворительная";
    }

    return "неустойчивая";
}


int budget_stability_score(const simulation_result& result, std::vector<std::string>& notes) {
    const int months = static_cast<int>(result.months.size());
    if (months == 0) {
        notes.push_back("Нет данных для оценки стабильности бюджета.");
        return 0;
    }

    const double deficit_share = static_cast<double>(result.negative_cashflow_months)
                                 / static_cast<double>(months);
    if (result.negative_cashflow_months == 0) {
        notes.push_back("Ежемесячный бюджет остаётся положительным в течение всего моделирования.");
        return score_per_dimension;
    }

    if (deficit_share <= 0.05) {
        notes.push_back("Дефицит бюджета редкий и может покрываться резервом.");
        return 15;
    }

    if (deficit_share <= 0.15) {
        notes.push_back("Дефицит бюджета встречается регулярно; необходим более крупный резерв.");
        return 10;
    }

    notes.push_back("Дефицит бюджета повторяется слишком часто.");
    return 3;
}


int reserve_quality_score(const simulation_result& result, std::vector<std::string>& notes) {
    if (result.months.empty()) {
        return 0;
    }

    const month_record& last_month = result.months.back();
    const double average_expenses = result.total_expenses
                                    / static_cast<double>(result.months.size());
    const double coverage = average_expenses > rubles_epsilon
                                ? last_month.savings_balance / average_expenses
                                : 0.0;

    if (coverage >= 6.0) {
        notes.push_back("Накопления в конце периода покрывают не менее шести месяцев расходов.");
        return score_per_dimension;
    }

    if (coverage >= 3.0) {
        notes.push_back("Накопления формируют приемлемый, но не максимальный резерв.");
        return 14;
    }

    if (coverage >= 1.0) {
        notes.push_back("Резерв слишком мал: он покрывает менее трёх месяцев расходов.");
        return 8;
    }

    notes.push_back("Ликвидного резерва практически нет.");
    return 2;
}


int debt_quality_score(const simulation_result& result, std::vector<std::string>& notes) {
    const person_plan& plan = result.final_plan;
    if (plan.credit_debt > rubles_epsilon || result.credit_limit_breaches > 0) {
        notes.push_back("Есть непогашенный дорогой долг или превышение кредитного лимита.");
        return 0;
    }

    if (plan.mortgage.active) {
        const double original = std::max(plan.mortgage.initial_principal, 1.0);
        const double remaining_share = plan.mortgage.remaining_principal / original;
        if (remaining_share < 0.4) {
            notes.push_back("Ипотека существенно сокращена и обслуживается без просрочек.");
            return 16;
        }

        notes.push_back("Ипотека сохраняется, поэтому финансовая свобода ограничена долговой нагрузкой.");
        return 11;
    }

    notes.push_back("К концу периода все обязательные кредиты погашены.");
    return score_per_dimension;
}


int housing_quality_score(const simulation_result& result, std::vector<std::string>& notes) {
    if (result.final_plan.home.mode == housing_mode::owned_home) {
        notes.push_back("В конце периода жильё находится в собственности и не требует арендной платы.");
        return score_per_dimension;
    }

    if (result.final_plan.home.mode == housing_mode::mortgage) {
        notes.push_back("Жильё приобретено, но ипотека ещё не погашена.");
        return 14;
    }

    notes.push_back("Стратегия сохраняет зависимость от арендного рынка.");
    return 8;
}


int growth_quality_score(const simulation_result& result, std::vector<std::string>& notes) {
    if (result.months.empty()) {
        return 0;
    }

    const double first = result.months.front().net_worth;
    const double last = result.months.back().net_worth;
    const double growth = calculate_annualized_growth(first, last,
                                                       static_cast<int>(result.months.size()));

    if (growth >= 10.0) {
        notes.push_back("Чистый капитал уверенно растёт в реальном денежном выражении модели.");
        return score_per_dimension;
    }

    if (growth >= 4.0) {
        notes.push_back("Чистый капитал растёт, но умеренным темпом.");
        return 14;
    }

    if (growth >= 0.0) {
        notes.push_back("Чистый капитал почти не растёт, необходимо пересмотреть долю сбережений.");
        return 8;
    }

    notes.push_back("Чистый капитал уменьшается на горизонте моделирования.");
    return 0;
}


}  // namespace


value_range calculate_range(const std::vector<double>& values) {
    value_range result;
    if (values.empty()) {
        return result;
    }

    const auto [minimum, maximum] = std::minmax_element(values.begin(), values.end());
    result.minimum = *minimum;
    result.maximum = *maximum;
    result.average = std::accumulate(values.begin(), values.end(), 0.0)
                     / static_cast<double>(values.size());
    result.median = calculate_median(values);
    return result;
}


double calculate_median(std::vector<double> values) {
    if (values.empty()) {
        return 0.0;
    }

    std::sort(values.begin(), values.end());
    const std::size_t middle = values.size() / 2;
    if (values.size() % 2 == 1) {
        return values[middle];
    }

    return (values[middle - 1] + values[middle]) / 2.0;
}


double calculate_total_for_category(const simulation_result& result, expense_kind category) {
    double total = 0.0;
    for (const month_record& record : result.months) {
        const auto iterator = record.expenses.find(category);
        if (iterator != record.expenses.end()) {
            total += iterator->second;
        }
    }
    return total;
}


double calculate_average_for_category(const simulation_result& result, expense_kind category) {
    if (result.months.empty()) {
        return 0.0;
    }
    return calculate_total_for_category(result, category)
           / static_cast<double>(result.months.size());
}


double calculate_category_share(const simulation_result& result, expense_kind category) {
    if (result.total_expenses <= rubles_epsilon) {
        return 0.0;
    }
    return calculate_total_for_category(result, category)
           / result.total_expenses
           * percent_base;
}


std::vector<category_summary> summarize_expense_categories(const simulation_result& result) {
    std::vector<category_summary> summaries;
    const std::array<expense_kind, 14> kinds = {
        expense_kind::food,
        expense_kind::utilities,
        expense_kind::rent,
        expense_kind::mortgage,
        expense_kind::pet,
        expense_kind::car,
        expense_kind::transport,
        expense_kind::communication,
        expense_kind::health,
        expense_kind::leisure,
        expense_kind::household,
        expense_kind::insurance,
        expense_kind::loan_payment,
        expense_kind::event_expense
    };

    for (expense_kind kind : kinds) {
        category_summary summary;
        summary.category = kind;
        summary.total = calculate_total_for_category(result, kind);
        summary.monthly_average = calculate_average_for_category(result, kind);
        summary.share_percent = calculate_category_share(result, kind);

        for (const month_record& record : result.months) {
            const auto iterator = record.expenses.find(kind);
            if (iterator != record.expenses.end() && std::abs(iterator->second) > rubles_epsilon) {
                ++summary.non_zero_months;
            }
        }

        if (std::abs(summary.total) > rubles_epsilon) {
            summaries.push_back(summary);
        }
    }

    std::sort(summaries.begin(), summaries.end(), [](const category_summary& left,
                                                      const category_summary& right) {
        return left.total > right.total;
    });
    return summaries;
}


std::vector<event_summary> summarize_events(const simulation_result& result) {
    std::map<std::string, int> counter;
    for (const month_record& record : result.months) {
        for (const std::string& note : record.notes) {
            ++counter[note];
        }
    }

    std::vector<event_summary> summary;
    for (const auto& [title, occurrences] : counter) {
        summary.push_back(event_summary{title, occurrences});
    }

    std::sort(summary.begin(), summary.end(), [](const event_summary& left,
                                                  const event_summary& right) {
        if (left.occurrences != right.occurrences) {
            return left.occurrences > right.occurrences;
        }
        return left.title < right.title;
    });
    return summary;
}


month_record find_best_cashflow_month(const simulation_result& result) {
    if (result.months.empty()) {
        return {};
    }

    return *std::max_element(result.months.begin(), result.months.end(),
                             [](const month_record& left, const month_record& right) {
                                 return left.net_cashflow < right.net_cashflow;
                             });
}


month_record find_worst_cashflow_month(const simulation_result& result) {
    if (result.months.empty()) {
        return {};
    }

    return *std::min_element(result.months.begin(), result.months.end(),
                             [](const month_record& left, const month_record& right) {
                                 return left.net_cashflow < right.net_cashflow;
                             });
}


double calculate_annualized_growth(double start_value, double finish_value, int months) {
    if (months <= 0 || start_value <= rubles_epsilon || finish_value < 0.0) {
        return 0.0;
    }

    const double years = static_cast<double>(months) / months_in_year;
    return (std::pow(finish_value / start_value, 1.0 / years) - 1.0) * percent_base;
}


std::vector<capital_milestone> find_capital_milestones(const simulation_result& result,
                                                       const std::vector<double>& targets) {
    std::vector<capital_milestone> milestones;
    for (double target : targets) {
        capital_milestone milestone;
        milestone.target = target;
        for (std::size_t index = 0; index < result.months.size(); ++index) {
            if (result.months[index].net_worth + rubles_epsilon >= target) {
                milestone.month_number = static_cast<int>(index + 1);
                milestone.date = result.months[index].date;
                break;
            }
        }
        milestones.push_back(milestone);
    }
    return milestones;
}


life_quality_score calculate_life_quality_score(const simulation_result& result) {
    life_quality_score score;
    score.budget_stability = budget_stability_score(result, score.explanations);
    score.reserve_quality = reserve_quality_score(result, score.explanations);
    score.debt_quality = debt_quality_score(result, score.explanations);
    score.housing_quality = housing_quality_score(result, score.explanations);
    score.growth_quality = growth_quality_score(result, score.explanations);
    score.total = score.budget_stability
                  + score.reserve_quality
                  + score.debt_quality
                  + score.housing_quality
                  + score.growth_quality;
    score.grade = grade_for_score(score.total);
    return score;
}


model_statistics calculate_model_statistics(const simulation_result& result) {
    model_statistics statistics;
    statistics.monthly_income = calculate_range(extract_income(result));
    statistics.monthly_expenses = calculate_range(extract_expenses(result));
    statistics.monthly_cashflow = calculate_range(extract_cashflow(result));
    statistics.monthly_net_worth = calculate_range(extract_net_worth(result));
    statistics.categories = summarize_expense_categories(result);
    statistics.events = summarize_events(result);
    statistics.best_month = find_best_cashflow_month(result);
    statistics.worst_month = find_worst_cashflow_month(result);
    statistics.milestones = find_capital_milestones(result, {1'000'000.0,
                                                             5'000'000.0,
                                                             10'000'000.0,
                                                             25'000'000.0});
    statistics.quality = calculate_life_quality_score(result);

    for (const category_summary& category : statistics.categories) {
        if (category.category == expense_kind::event_expense) {
            statistics.total_event_expenses = category.total;
        } else {
            statistics.total_regular_expenses += category.total;
        }

        if (category.category == expense_kind::food) {
            statistics.total_food_expenses = category.total;
        }

        if (category.category == expense_kind::rent
            || category.category == expense_kind::mortgage
            || category.category == expense_kind::utilities) {
            statistics.total_housing_expenses += category.total;
        }
    }

    for (const month_record& record : result.months) {
        if (record.net_cashflow > rubles_epsilon) {
            ++statistics.months_with_positive_cashflow;
        } else if (record.net_cashflow < -rubles_epsilon) {
            ++statistics.months_with_negative_cashflow;
        } else {
            ++statistics.months_with_zero_cashflow;
        }
    }

    if (result.months.size() > 1) {
        const double initial_savings = result.months.front().savings_balance;
        const double final_savings = result.months.back().savings_balance;
        statistics.average_monthly_savings_change = (final_savings - initial_savings)
                                                    / static_cast<double>(result.months.size() - 1);
        statistics.annualized_net_worth_growth_percent = calculate_annualized_growth(
            result.months.front().net_worth,
            result.months.back().net_worth,
            static_cast<int>(result.months.size()));
    }

    return statistics;
}


std::vector<std::string> compare_cashflow_profiles(const simulation_result& alice,
                                                   const simulation_result& bob) {
    const model_statistics alice_statistics = calculate_model_statistics(alice);
    const model_statistics bob_statistics = calculate_model_statistics(bob);
    std::vector<std::string> conclusions;

    if (alice_statistics.monthly_cashflow.minimum < bob_statistics.monthly_cashflow.minimum) {
        conclusions.push_back("Минимальный месячный поток у ипотечной стратегии ниже: она чувствительнее к шокам дохода.");
    } else {
        conclusions.push_back("Минимальный месячный поток у стратегии аренды ниже: рост арендной платы создаёт больший риск.");
    }

    if (alice_statistics.total_housing_expenses < bob_statistics.total_housing_expenses) {
        conclusions.push_back("За выбранный горизонт затраты Алисы на жильё ниже совокупных затрат Боба.");
    } else {
        conclusions.push_back("За выбранный горизонт затраты Боба на жильё ниже совокупных затрат Алисы.");
    }

    if (alice_statistics.quality.total > bob_statistics.quality.total) {
        conclusions.push_back("По интегральной оценке качества финансовой жизни выше балл у Алисы.");
    } else if (alice_statistics.quality.total < bob_statistics.quality.total) {
        conclusions.push_back("По интегральной оценке качества финансовой жизни выше балл у Боба.");
    } else {
        conclusions.push_back("Интегральная оценка качества финансовой жизни у стратегий одинакова.");
    }

    if (alice_statistics.months_with_negative_cashflow == bob_statistics.months_with_negative_cashflow) {
        conclusions.push_back("Обе стратегии имеют одинаковое число месяцев с дефицитом бюджета.");
    } else if (alice_statistics.months_with_negative_cashflow < bob_statistics.months_with_negative_cashflow) {
        conclusions.push_back("Ипотечная стратегия показывает меньше месяцев с дефицитом бюджета.");
    } else {
        conclusions.push_back("Накопительная стратегия показывает меньше месяцев с дефицитом бюджета.");
    }

    return conclusions;
}

}  // namespace alice_bob

// ===== Объявления вывода в консоль =====

namespace alice_bob {

// Объявление тестов нужно консольному меню; реализация находится ниже.
bool run_all_tests(std::ostream& output);

void print_program_title(std::ostream& output);

void print_plan(const person_plan& plan, std::ostream& output);

void print_month_record(const month_record& record, std::ostream& output);

void print_year_table(const simulation_result& result, std::ostream& output);

void print_result_summary(const simulation_result& result, std::ostream& output);

void print_comparison(const comparison_result& comparison, std::ostream& output);

void print_ab_summary(const monte_carlo_summary& summary, std::ostream& output);

void print_affordability(const affordability_metrics& metrics, std::ostream& output);

void print_prepayment_analysis(const prepayment_analysis& analysis, std::ostream& output);

void print_stress_results(const std::vector<stress_result>& results, std::ostream& output);

void print_model_statistics(const simulation_result& result, std::ostream& output);

void print_life_quality_score(const life_quality_score& score, std::ostream& output);

bool export_month_records_csv(const simulation_result& result,
                              const std::filesystem::path& path,
                              std::string& error);

bool export_comparison_csv(const comparison_result& comparison,
                           const std::filesystem::path& path,
                           std::string& error);

int read_menu_choice(std::istream& input, std::ostream& output, int min, int max);

void run_interactive_console(std::istream& input, std::ostream& output);

}  // namespace alice_bob

// ===== Печать результатов и экспорт CSV =====



namespace alice_bob {

namespace {

constexpr int console_width = 118;


void print_line(std::ostream& output, char symbol = '-') {
    output << std::string(console_width, symbol) << '\n';
}


void print_centered(std::ostream& output, const std::string& text) {
    const int padding = std::max(0, (console_width - static_cast<int>(text.size())) / 2);
    output << std::string(static_cast<std::size_t>(padding), ' ') << text << '\n';
}


std::string yes_no(bool value) {
    return value ? "да" : "нет";
}


std::string rounded_number(double value) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(0) << value;
    return stream.str();
}


double salary_after_income_tax(const job& current_job, const tax_rule& taxes) {
    const double gross_salary = current_job.salary_for_month();
    return gross_salary - calculate_income_tax(gross_salary, taxes.income_tax_percent);
}


void print_key_value(std::ostream& output,
                     const std::string& name,
                     const std::string& value) {
    output << "  " << std::left << std::setw(38) << name << value << '\n';
}


void print_loan(std::ostream& output, const loan& value) {
    output << "  " << value.name << " (" << loan_text(value.kind) << ")\n";
    print_key_value(output, "сумма", money(value.initial_principal));
    print_key_value(output, "ставка", percent(value.annual_rate_percent));
    print_key_value(output, "срок", std::to_string(value.original_term_months) + " мес.");
    print_key_value(output, "платёж", money(value.required_monthly_payment));
}


void print_expense(std::ostream& output, const indexed_amount& item) {
    output << "  " << std::left << std::setw(28) << item.name
           << std::right << std::setw(16) << money(item.monthly_value)
           << "  инфляция " << percent(item.annual_inflation_percent) << '\n';
}


void print_result_numbers(const simulation_result& result, std::ostream& output) {
    print_key_value(output, "накопления в конце", money(result.final_plan.savings.balance));
    print_key_value(output, "остаток ипотеки", money(result.final_plan.mortgage.remaining_principal));
    print_key_value(output, "долг по карте", money(result.final_plan.credit_debt));
    print_key_value(output, "чистый капитал", money(result.final_net_worth));
    print_key_value(output, "месяцев с дефицитом", std::to_string(result.negative_cashflow_months));
    print_key_value(output, "проценты по кредитам", money(result.total_loan_interest));
    print_key_value(output, "проценты по накоплениям", money(result.total_interest_income));
    print_key_value(output, "налоговый вычет", money(result.total_tax_refund));
    if (result.final_plan.home.purchase_month > 0.0) {
        const int relative_month = static_cast<int>(result.final_plan.home.purchase_month)
                                   - result.final_plan.start_date.serial_month() + 1;
        print_key_value(output, "месяц покупки квартиры", std::to_string(relative_month));
    }
}


void print_menu(std::ostream& output) {
    print_line(output, '=');
    output << "1. Показать исходные параметры Алисы и Боба\n";
    output << "2. Сравнить ипотеку и накопление без случайных событий\n";
    output << "3. Показать первые 12 месяцев модели Алисы\n";
    output << "4. Провести A/B-тест со случайными событиями (50 запусков)\n";
    output << "5. Экспортировать результаты сравнения в CSV\n";
    output << "6. Выполнить внутренние тесты модели\n";
    output << "7. Проверить доступность ипотечной нагрузки для Алисы\n";
    output << "8. Выполнить стресс-тест стратегии Алисы\n";
    output << "9. Оценить качество финансовой жизни Алисы\n";
    output << "0. Завершить работу\n";
    print_line(output, '=');
}


void pause_console(std::istream& input, std::ostream& output) {
    output << "Нажмите Enter, чтобы продолжить...";
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


std::string csv_escape(const std::string& value) {
    std::string result = "\"";
    for (char symbol : value) {
        if (symbol == '\"') {
            result += "\"\"";
        } else {
            result += symbol;
        }
    }
    result += "\"";
    return result;
}


std::string csv_number(double value) {
    std::ostringstream stream;
    stream.imbue(std::locale::classic());
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}


}  // namespace


void print_program_title(std::ostream& output) {
    print_line(output, '=');
    print_centered(output, "ALICE AND BOB");
    print_centered(output, "Модель личных финансов: ипотека против накоплений");
    print_centered(output, "Шаг симуляции: один месяц; горизонт: 20 лет");
    print_line(output, '=');
}


void print_plan(const person_plan& plan, std::ostream& output) {
    print_line(output);
    output << plan.name << ": " << plan.strategy_name << '\n';
    output << plan.strategy_description << '\n';
    print_line(output);

    print_key_value(output, "первичная работа", plan.primary_job.title);
    print_key_value(output, "зарплата после налога", money(salary_after_income_tax(plan.primary_job, plan.taxes)));
    print_key_value(output, "рост зарплаты", percent(plan.primary_job.annual_salary_growth_percent));
    print_key_value(output, "вторая работа", plan.secondary_job.title);
    print_key_value(output, "доход со второй работы", money(salary_after_income_tax(plan.secondary_job, plan.taxes)));
    print_key_value(output, "стартовые накопления", money(plan.savings.balance));
    print_key_value(output, "ставка по вкладу", percent(plan.savings.annual_rate_percent));
    print_key_value(output, "жилищная стратегия", housing_text(plan.home.mode));
    print_key_value(output, "стоимость квартиры", money(plan.home.property_price));
    print_key_value(output, "рост цены квартиры", percent(plan.home.annual_property_growth_percent));

    if (plan.home.mode == housing_mode::rent) {
        print_key_value(output, "арендная плата", money(plan.home.monthly_rent.monthly_value));
    } else {
        print_key_value(output, "первоначальный взнос", money(plan.home.down_payment));
    }

    print_key_value(output, "коммунальные услуги", money(plan.home.monthly_utilities.monthly_value));
    print_key_value(output, "резерв на ремонт", money(plan.home.maintenance_reserve.monthly_value));
    print_key_value(output, "налоговый вычет", yes_no(plan.receive_tax_refund));
    print_key_value(output, "досрочное погашение", yes_no(plan.make_mortgage_prepayment));

    if (plan.mortgage.active) {
        output << "\nКредит на жильё:\n";
        print_loan(output, plan.mortgage);
    }

    if (!plan.additional_loans.empty()) {
        output << "\nДополнительные кредиты:\n";
        for (const loan& value : plan.additional_loans) {
            print_loan(output, value);
        }
    }

    output << "\nРегулярные расходы:\n";
    for (const indexed_amount& item : plan.regular_expenses) {
        print_expense(output, item);
    }

    if (plan.domestic_pet.enabled) {
        output << "\nДомашнее животное:\n";
        print_key_value(output, "питомец", plan.domestic_pet.name);
        print_key_value(output, "срок жизни в модели", std::to_string(plan.domestic_pet.lifespan_months / 12) + " лет");
        print_key_value(output, "расход в месяц", money(plan.domestic_pet.monthly_cost.monthly_value));
    }

    if (plan.personal_car.enabled) {
        output << "\nАвтомобиль:\n";
        print_key_value(output, "модель", plan.personal_car.model);
        print_key_value(output, "топливо", money(plan.personal_car.fuel.monthly_value));
        print_key_value(output, "обслуживание", money(plan.personal_car.maintenance.monthly_value));
        print_key_value(output, "страховка в год", money(plan.personal_car.insurance_annual_cost));
    }
}


void print_month_record(const month_record& record, std::ostream& output) {
    print_line(output);
    output << record.date.text() << '\n';
    print_line(output);
    print_key_value(output, "зарплата по основной работе", money(record.primary_salary));
    print_key_value(output, "доход от второй работы", money(record.secondary_salary));
    print_key_value(output, "проценты по вкладу", money(record.savings_interest));
    print_key_value(output, "налоговый вычет", money(record.tax_refund));
    print_key_value(output, "прочий доход", money(record.other_income));
    print_key_value(output, "доходы всего", money(record.total_income));

    output << "\nРасходы:\n";
    for (const auto& [kind, value] : record.expenses) {
        output << "  " << std::left << std::setw(28) << expense_text(kind)
               << std::right << money(value) << '\n';
    }

    print_key_value(output, "расходы всего", money(record.total_expenses));
    print_key_value(output, "денежный поток", money(record.net_cashflow));
    print_key_value(output, "накопления", money(record.savings_balance));
    print_key_value(output, "остаток ипотеки", money(record.mortgage_balance));
    print_key_value(output, "чистый капитал", money(record.net_worth));

    if (!record.notes.empty()) {
        output << "\nСобытия и изменения:\n";
        for (const std::string& note : record.notes) {
            output << "  - " << note << '\n';
        }
    }
}


void print_year_table(const simulation_result& result, std::ostream& output) {
    print_line(output);
    output << "Динамика по годам: " << result.final_plan.name << '\n';
    print_line(output);
    output << std::left << std::setw(7) << "Год"
           << std::right << std::setw(15) << "Доходы"
           << std::setw(15) << "Расходы"
           << std::setw(15) << "Накопления"
           << std::setw(15) << "Ипотека"
           << std::setw(15) << "Капитал"
           << std::setw(8) << "Деф." << '\n';
    print_line(output);

    for (const year_record& year : result.years) {
        output << std::left << std::setw(7) << year.year
               << std::right << std::setw(15) << rounded_number(year.income)
               << std::setw(15) << rounded_number(year.expenses)
               << std::setw(15) << rounded_number(year.savings_balance_end)
               << std::setw(15) << rounded_number(year.mortgage_balance_end)
               << std::setw(15) << rounded_number(year.net_worth_end)
               << std::setw(8) << year.negative_cashflow_months << '\n';
    }
}


void print_result_summary(const simulation_result& result, std::ostream& output) {
    print_line(output, '=');
    output << "Итог модели: " << result.final_plan.name << " — "
           << result.final_plan.strategy_name << '\n';
    print_line(output, '=');
    print_result_numbers(result, output);

    output << "\nВыводы:\n";
    for (const std::string& conclusion : result.conclusions) {
        output << "  - " << conclusion << '\n';
    }
}


void print_comparison(const comparison_result& comparison, std::ostream& output) {
    print_program_title(output);
    output << "Сравнение одинаковых исходных доходов на горизонте 20 лет\n";
    print_line(output);
    output << std::left << std::setw(42) << "Показатель"
           << std::right << std::setw(34) << "Алиса"
           << std::setw(34) << "Боб" << '\n';
    print_line(output);

    output << std::left << std::setw(42) << "Чистый капитал"
           << std::right << std::setw(34) << rounded_number(comparison.alice.final_net_worth)
           << std::setw(34) << rounded_number(comparison.bob.final_net_worth) << '\n';
    output << std::left << std::setw(42) << "Накопления на вкладе"
           << std::right << std::setw(34) << rounded_number(comparison.alice.final_plan.savings.balance)
           << std::setw(34) << rounded_number(comparison.bob.final_plan.savings.balance) << '\n';
    output << std::left << std::setw(42) << "Остаток ипотеки"
           << std::right << std::setw(34) << rounded_number(comparison.alice.final_plan.mortgage.remaining_principal)
           << std::setw(34) << rounded_number(comparison.bob.final_plan.mortgage.remaining_principal) << '\n';
    output << std::left << std::setw(42) << "Проценты по кредитам"
           << std::right << std::setw(34) << rounded_number(comparison.alice.total_loan_interest)
           << std::setw(34) << rounded_number(comparison.bob.total_loan_interest) << '\n';
    output << std::left << std::setw(42) << "Проценты по накоплениям"
           << std::right << std::setw(34) << rounded_number(comparison.alice.total_interest_income)
           << std::setw(34) << rounded_number(comparison.bob.total_interest_income) << '\n';
    output << std::left << std::setw(42) << "Налоговый вычет"
           << std::right << std::setw(34) << rounded_number(comparison.alice.total_tax_refund)
           << std::setw(34) << rounded_number(comparison.bob.total_tax_refund) << '\n';
    output << std::left << std::setw(42) << "Месяцев с дефицитом"
           << std::right << std::setw(34) << comparison.alice.negative_cashflow_months
           << std::setw(34) << comparison.bob.negative_cashflow_months << '\n';
    output << std::left << std::setw(42) << "Итоговый тип жилья"
           << std::right << std::setw(34) << housing_text(comparison.alice.final_plan.home.mode)
           << std::setw(34) << housing_text(comparison.bob.final_plan.home.mode) << '\n';
    output << std::left << std::setw(42) << "Месяц покупки квартиры"
           << std::right << std::setw(34)
           << (comparison.alice.final_plan.home.purchase_month > 0.0
                   ? std::to_string(static_cast<int>(comparison.alice.final_plan.home.purchase_month)
                                    - comparison.alice.final_plan.start_date.serial_month() + 1)
                   : (comparison.alice.final_plan.home.property_owned ? "с начала" : "-"))
           << std::setw(34)
           << (comparison.bob.final_plan.home.purchase_month > 0.0
                   ? std::to_string(static_cast<int>(comparison.bob.final_plan.home.purchase_month)
                                    - comparison.bob.final_plan.start_date.serial_month() + 1)
                   : "-") << '\n';
    print_line(output);

    output << "Победитель по чистому капиталу: " << comparison.winner_name << '\n';
    output << "Разница чистого капитала: " << money(std::abs(comparison.net_worth_difference)) << '\n';
    output << "\nИнтерпретация:\n";
    for (const std::string& conclusion : comparison.conclusions) {
        output << "  - " << conclusion << '\n';
    }
}


void print_ab_summary(const monte_carlo_summary& summary, std::ostream& output) {
    print_line(output, '=');
    output << "A/B-тест: одинаковые случайные события для обеих стратегий\n";
    print_line(output, '=');
    print_key_value(output, "количество запусков", std::to_string(summary.runs));
    print_key_value(output, "побед Алисы", std::to_string(summary.alice_wins));
    print_key_value(output, "побед Боба", std::to_string(summary.bob_wins));
    print_key_value(output, "ничьих", std::to_string(summary.draws));
    output << '\n';
    output << std::left << std::setw(36) << "Показатель"
           << std::right << std::setw(22) << "Алиса"
           << std::setw(22) << "Боб" << '\n';
    print_line(output);
    output << std::left << std::setw(36) << "Средний чистый капитал"
           << std::right << std::setw(22) << rounded_number(summary.alice_average_net_worth)
           << std::setw(22) << rounded_number(summary.bob_average_net_worth) << '\n';
    output << std::left << std::setw(36) << "Минимальный капитал"
           << std::right << std::setw(22) << rounded_number(summary.alice_min_net_worth)
           << std::setw(22) << rounded_number(summary.bob_min_net_worth) << '\n';
    output << std::left << std::setw(36) << "Максимальный капитал"
           << std::right << std::setw(22) << rounded_number(summary.alice_max_net_worth)
           << std::setw(22) << rounded_number(summary.bob_max_net_worth) << '\n';
    output << std::left << std::setw(36) << "Среднее число дефицитных месяцев"
           << std::right << std::setw(22) << std::fixed << std::setprecision(2)
           << summary.alice_average_negative_months
           << std::setw(22) << summary.bob_average_negative_months << '\n';
}


void print_affordability(const affordability_metrics& metrics, std::ostream& output) {
    print_line(output, '=');
    output << "Проверка доступности финансовой нагрузки\n";
    print_line(output, '=');
    print_key_value(output, "доход до налога", money(metrics.gross_monthly_income));
    print_key_value(output, "доход после налога", money(metrics.net_monthly_income));
    print_key_value(output, "ежемесячная стоимость жилья", money(metrics.monthly_housing_cost));
    print_key_value(output, "обязательные платежи по долгам", money(metrics.monthly_debt_payment));
    print_key_value(output, "долговая нагрузка", percent(metrics.debt_to_income_percent));
    print_key_value(output, "доля расходов на жильё", percent(metrics.housing_to_income_percent));
    print_key_value(output, "целевой резерв", money(metrics.emergency_fund_target));
    print_key_value(output, "фактический ликвидный резерв", money(metrics.actual_reserve));
    print_key_value(output, "покрытие регулярных расходов", rounded_number(metrics.reserve_coverage_months) + " мес.");
    print_key_value(output, "максимальный безопасный платёж", money(metrics.maximum_safe_debt_payment));
    print_key_value(output, "максимальная безопасная сумма кредита", money(metrics.maximum_safe_mortgage_principal));
    print_key_value(output, "уровень риска", risk_level_text(metrics.risk));

    output << "\nИнтерпретация:\n";
    for (const std::string& text : metrics.explanations) {
        output << "  - " << text << '\n';
    }
}


void print_prepayment_analysis(const prepayment_analysis& analysis, std::ostream& output) {
    print_line(output);
    output << "Анализ досрочного погашения ипотеки\n";
    print_line(output);
    print_key_value(output, "дополнительный платёж в месяц", money(analysis.monthly_extra_payment));
    print_key_value(output, "срок без досрочных платежей", std::to_string(analysis.months_without_prepayment) + " мес.");
    print_key_value(output, "срок с досрочными платежами", std::to_string(analysis.months_with_prepayment) + " мес.");
    print_key_value(output, "сокращение срока", std::to_string(analysis.saved_months) + " мес.");
    print_key_value(output, "проценты без досрочного платежа", money(analysis.interest_without_prepayment));
    print_key_value(output, "проценты с досрочным платежом", money(analysis.interest_with_prepayment));
    print_key_value(output, "экономия на процентах", money(analysis.saved_interest));
}


void print_stress_results(const std::vector<stress_result>& results, std::ostream& output) {
    print_line(output, '=');
    output << "Стресс-тест стратегии\n";
    print_line(output, '=');
    output << std::left << std::setw(50) << "Сценарий"
           << std::right << std::setw(18) << "Капитал"
           << std::setw(18) << "Изменение"
           << std::setw(12) << "Дефицит"
           << std::setw(14) << "Устойчивость" << '\n';
    print_line(output);

    for (const stress_result& result : results) {
        output << std::left << std::setw(50) << result.configuration.title
               << std::right << std::setw(18) << rounded_number(result.result.final_net_worth)
               << std::setw(18) << rounded_number(result.net_worth_change)
               << std::setw(12) << result.result.negative_cashflow_months
               << std::setw(14) << yes_no(result.remains_stable) << '\n';

        for (const std::string& note : result.notes) {
            output << "  " << note << '\n';
        }
    }
}


void print_life_quality_score(const life_quality_score& score, std::ostream& output) {
    print_line(output);
    output << "Оценка качества финансовой жизни: " << score.grade
           << " (" << score.total << " из 100)\n";
    print_line(output);
    print_key_value(output, "стабильность бюджета", std::to_string(score.budget_stability) + " из 20");
    print_key_value(output, "качество резерва", std::to_string(score.reserve_quality) + " из 20");
    print_key_value(output, "управление долгом", std::to_string(score.debt_quality) + " из 20");
    print_key_value(output, "жилищная устойчивость", std::to_string(score.housing_quality) + " из 20");
    print_key_value(output, "рост капитала", std::to_string(score.growth_quality) + " из 20");

    output << "\nОснования оценки:\n";
    for (const std::string& explanation : score.explanations) {
        output << "  - " << explanation << '\n';
    }
}


void print_model_statistics(const simulation_result& result, std::ostream& output) {
    const model_statistics statistics = calculate_model_statistics(result);
    print_line(output, '=');
    output << "Статистика модели: " << result.final_plan.name << '\n';
    print_line(output, '=');
    print_key_value(output, "средний доход в месяц", money(statistics.monthly_income.average));
    print_key_value(output, "медианный доход в месяц", money(statistics.monthly_income.median));
    print_key_value(output, "средние расходы в месяц", money(statistics.monthly_expenses.average));
    print_key_value(output, "средний денежный поток", money(statistics.monthly_cashflow.average));
    print_key_value(output, "минимальный денежный поток", money(statistics.monthly_cashflow.minimum));
    print_key_value(output, "максимальный денежный поток", money(statistics.monthly_cashflow.maximum));
    print_key_value(output, "среднее изменение накоплений", money(statistics.average_monthly_savings_change));
    print_key_value(output, "годовой рост чистого капитала", percent(statistics.annualized_net_worth_growth_percent));
    print_key_value(output, "месяцев с положительным потоком", std::to_string(statistics.months_with_positive_cashflow));
    print_key_value(output, "месяцев с дефицитом", std::to_string(statistics.months_with_negative_cashflow));

    output << "\nКрупнейшие категории расходов:\n";
    output << std::left << std::setw(28) << "Категория"
           << std::right << std::setw(18) << "Всего"
           << std::setw(18) << "В месяц"
           << std::setw(13) << "Доля" << '\n';
    print_line(output);
    const std::size_t rows = std::min<std::size_t>(statistics.categories.size(), 8);
    for (std::size_t index = 0; index < rows; ++index) {
        const category_summary& category = statistics.categories[index];
        output << std::left << std::setw(28) << expense_text(category.category)
               << std::right << std::setw(18) << rounded_number(category.total)
               << std::setw(18) << rounded_number(category.monthly_average)
               << std::setw(13) << percent(category.share_percent) << '\n';
    }

    output << "\nЛучший месяц по потоку: " << statistics.best_month.date.text()
           << " (" << money(statistics.best_month.net_cashflow) << ")\n";
    output << "Худший месяц по потоку: " << statistics.worst_month.date.text()
           << " (" << money(statistics.worst_month.net_cashflow) << ")\n";

    output << "\nДостижение порогов чистого капитала:\n";
    for (const capital_milestone& milestone : statistics.milestones) {
        output << "  " << money(milestone.target) << ": ";
        if (milestone.month_number > 0) {
            output << milestone.date.text() << " (" << milestone.month_number << "-й месяц)\n";
        } else {
            output << "не достигнут за период моделирования\n";
        }
    }

    print_life_quality_score(statistics.quality, output);
}


bool export_month_records_csv(const simulation_result& result,
                              const std::filesystem::path& path,
                              std::string& error) {
    std::ofstream file(path);
    if (!file) {
        error = "Невозможно открыть файл для записи: " + path.string();
        return false;
    }

    file << "year;month;primary_salary;secondary_salary;savings_interest;tax_refund;total_income;total_expenses;net_cashflow;savings;mortgage;net_worth;notes\n";

    for (const month_record& record : result.months) {
        std::ostringstream notes;
        for (std::size_t index = 0; index < record.notes.size(); ++index) {
            if (index > 0) {
                notes << " | ";
            }
            notes << record.notes[index];
        }

        file << record.date.year << ';'
             << record.date.month << ';'
             << csv_number(record.primary_salary) << ';'
             << csv_number(record.secondary_salary) << ';'
             << csv_number(record.savings_interest) << ';'
             << csv_number(record.tax_refund) << ';'
             << csv_number(record.total_income) << ';'
             << csv_number(record.total_expenses) << ';'
             << csv_number(record.net_cashflow) << ';'
             << csv_number(record.savings_balance) << ';'
             << csv_number(record.mortgage_balance) << ';'
             << csv_number(record.net_worth) << ';'
             << csv_escape(notes.str()) << '\n';
    }

    if (!file) {
        error = "Ошибка во время записи CSV-файла: " + path.string();
        return false;
    }

    return true;
}


bool export_comparison_csv(const comparison_result& comparison,
                           const std::filesystem::path& path,
                           std::string& error) {
    std::ofstream file(path);
    if (!file) {
        error = "Невозможно открыть файл для записи: " + path.string();
        return false;
    }

    file << "year;alice_net_worth;bob_net_worth;alice_savings;bob_savings;alice_mortgage;bob_mortgage\n";
    const std::size_t count = std::min(comparison.alice.years.size(), comparison.bob.years.size());

    for (std::size_t index = 0; index < count; ++index) {
        const year_record& alice = comparison.alice.years[index];
        const year_record& bob = comparison.bob.years[index];
        file << alice.year << ';'
             << csv_number(alice.net_worth_end) << ';'
             << csv_number(bob.net_worth_end) << ';'
             << csv_number(alice.savings_balance_end) << ';'
             << csv_number(bob.savings_balance_end) << ';'
             << csv_number(alice.mortgage_balance_end) << ';'
             << csv_number(bob.mortgage_balance_end) << '\n';
    }

    if (!file) {
        error = "Ошибка во время записи CSV-файла: " + path.string();
        return false;
    }

    return true;
}


int read_menu_choice(std::istream& input, std::ostream& output, int min, int max) {
    int choice = min - 1;
    while (true) {
        output << "Выберите пункт: ";
        if (input >> choice && choice >= min && choice <= max) {
            return choice;
        }

        output << "Введите целое число от " << min << " до " << max << ".\n";
        input.clear();
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}


void run_interactive_console(std::istream& input, std::ostream& output) {
    print_program_title(output);

    bool active = true;
    while (active) {
        print_menu(output);
        const int choice = read_menu_choice(input, output, 0, 9);

        switch (choice) {
            case 0:
                output << "Работа программы завершена.\n";
                active = false;
                break;

            case 1:
                print_plan(make_alice_plan(), output);
                print_plan(make_bob_plan(), output);
                pause_console(input, output);
                break;

            case 2: {
                const comparison_result comparison = compare_default_strategies(false, 42U);
                print_comparison(comparison, output);
                print_year_table(comparison.alice, output);
                print_year_table(comparison.bob, output);
                pause_console(input, output);
                break;
            }

            case 3: {
                simulation_result alice = run_simulation(make_alice_plan(), true, 2026U);
                const int months_to_show = std::min(12, static_cast<int>(alice.months.size()));
                for (int index = 0; index < months_to_show; ++index) {
                    print_month_record(alice.months.at(static_cast<std::size_t>(index)), output);
                }
                print_result_summary(alice, output);
                pause_console(input, output);
                break;
            }

            case 4: {
                const monte_carlo_summary summary = run_ab_experiment(50, 1'000U);
                print_ab_summary(summary, output);
                pause_console(input, output);
                break;
            }

            case 5: {
                const comparison_result comparison = compare_default_strategies(false, 42U);
                std::string error;
                const bool saved = export_comparison_csv(comparison, "comparison_results.csv", error);
                if (saved) {
                    output << "Файл comparison_results.csv сохранён в текущей папке.\n";
                } else {
                    output << error << '\n';
                }
                pause_console(input, output);
                break;
            }

            case 6:
                run_all_tests(output);
                pause_console(input, output);
                break;

            case 7: {
                const person_plan alice = make_alice_plan();
                const affordability_metrics metrics = analyze_affordability(alice);
                const prepayment_analysis analysis = analyze_prepayment(alice.mortgage,
                                                                         alice.mortgage.required_monthly_payment * 0.2);
                print_affordability(metrics, output);
                print_prepayment_analysis(analysis, output);
                pause_console(input, output);
                break;
            }

            case 8: {
                const person_plan alice = make_alice_plan();
                const simulation_result base = run_simulation(alice, false, 42U);
                print_stress_results(run_default_stress_tests(alice, base, 5'000U), output);
                pause_console(input, output);
                break;
            }

            case 9: {
                const simulation_result alice = run_simulation(make_alice_plan(), true, 2'026U);
                print_model_statistics(alice, output);
                pause_console(input, output);
                break;
            }

            default:
                break;
        }
    }
}

}  // namespace alice_bob

// ===== Объявления встроенных проверок =====



namespace alice_bob {

bool run_all_tests(std::ostream& output);

}  // namespace alice_bob

// ===== 25 встроенных проверок =====


namespace alice_bob {

namespace {

struct test_case {
    std::string name;
    std::function<bool(std::string&)> action;
};


bool require_true(bool condition, const std::string& message, std::string& error) {
    if (condition) {
        return true;
    }
    error = message;
    return false;
}


bool require_equal(double expected,
                   double actual,
                   double tolerance,
                   const std::string& message,
                   std::string& error) {
    if (nearly_equal(expected, actual, tolerance)) {
        return true;
    }

    error = message + "; ожидалось " + std::to_string(expected)
            + ", получено " + std::to_string(actual);
    return false;
}


bool test_date_transition(std::string& error) {
    simulation_date date{2026, 12};
    date.advance_one_month();
    return require_true(date.year == 2027 && date.month == 1,
                        "переход с декабря на январь выполнен неверно",
                        error);
}


bool test_annuity_payment(std::string& error) {
    const double payment = annuity_payment(1'000'000.0, 12.0, 12);
    return require_equal(88'848.79,
                         payment,
                         0.05,
                         "аннуитетный платёж рассчитан неверно",
                         error);
}


bool test_zero_rate_loan(std::string& error) {
    const double payment = annuity_payment(120'000.0, 0.0, 12);
    return require_equal(10'000.0,
                         payment,
                         0.01,
                         "кредит с нулевой ставкой рассчитан неверно",
                         error);
}


bool test_loan_payment_reduces_debt(std::string& error) {
    loan value;
    initialize_loan(value, loan_kind::consumer_loan, "Проверочный кредит", 100'000.0, 10.0, 12);
    const double before = value.remaining_principal;
    const loan_payment payment = pay_loan_month(value, 0.0);

    return require_true(payment.interest_part > 0.0
                        && payment.principal_part > 0.0
                        && value.remaining_principal < before,
                        "очередной платёж не уменьшил основной долг",
                        error);
}


bool test_extra_payment_reduces_debt(std::string& error) {
    loan value;
    initialize_loan(value, loan_kind::mortgage, "Проверочная ипотека", 1'000'000.0, 12.0, 120);
    loan without_extra = value;
    loan with_extra = value;
    pay_loan_month(without_extra, 0.0);
    pay_loan_month(with_extra, 20'000.0);

    return require_true(with_extra.remaining_principal < without_extra.remaining_principal,
                        "досрочный платёж не уменьшил остаток долга",
                        error);
}


bool test_savings_interest(std::string& error) {
    savings_account account{120'000.0, 12.0, 0.0, 0.0};
    const double interest = account.accrue_interest();
    return require_equal(1'200.0,
                         interest,
                         0.01,
                         "месячные проценты по вкладу рассчитаны неверно",
                         error);
}


bool test_savings_cannot_break_minimum(std::string& error) {
    savings_account account{100'000.0, 10.0, 0.0, 80'000.0};
    const bool withdrawn = account.withdraw(25'000.0);
    return require_true(!withdrawn && nearly_equal(account.balance, 100'000.0),
                        "вклад разрешил снять средства ниже минимального остатка",
                        error);
}


bool test_savings_interest_is_not_counted_twice(std::string& error) {
    person_plan plan;
    plan.simulation_months = 1;
    plan.primary_job.enabled = false;
    plan.secondary_job.enabled = false;
    plan.savings = savings_account{120'000.0, 12.0, 0.0, 0.0};
    plan.home.mode = housing_mode::owned_home;
    plan.home.property_owned = false;
    plan.home.monthly_utilities = indexed_amount{"Коммунальные услуги", 0.0, 0.0};
    plan.home.maintenance_reserve = indexed_amount{"Резерв на ремонт", 0.0, 0.0};
    plan.regular_expenses.clear();
    plan.domestic_pet.enabled = false;
    plan.personal_car.enabled = false;

    const simulation_result result = run_simulation(plan, false, 1U);
    return require_true(nearly_equal(result.total_interest_income, 1'200.0)
                        && nearly_equal(result.final_plan.savings.balance, 121'200.0),
                        "проценты по вкладу начислены дважды",
                        error);
}


bool test_tax_deduction_limit(std::string& error) {
    tax_rule rule;
    const double refund = calculate_property_tax_refund(rule, 4'000'000.0, 1'000'000.0);
    const double second_refund = calculate_property_tax_refund(rule, 4'000'000.0, 1'000'000.0);

    return require_true(nearly_equal(refund, 260'000.0)
                        && nearly_equal(second_refund, 0.0)
                        && nearly_equal(rule.property_deduction_used, 2'000'000.0),
                        "ограничение имущественного вычета не соблюдено",
                        error);
}


bool test_interest_deduction_limit(std::string& error) {
    tax_rule rule;
    const double refund = calculate_interest_tax_refund(rule, 4'000'000.0, 1'000'000.0);
    return require_true(nearly_equal(refund, 390'000.0)
                        && nearly_equal(rule.interest_deduction_used, 3'000'000.0),
                        "ограничение вычета по ипотечным процентам не соблюдено",
                        error);
}


bool test_future_value(std::string& error) {
    const double value = future_value_of_savings(0.0, 10'000.0, 0.0, 12);
    return require_equal(120'000.0,
                         value,
                         0.01,
                         "будущая стоимость накоплений рассчитана неверно",
                         error);
}


bool test_target_months(std::string& error) {
    const int months = months_to_reach_target(0.0, 10'000.0, 0.0, 120'000.0);
    return require_true(months == 12,
                        "срок достижения цели по накоплениям рассчитан неверно",
                        error);
}


bool test_income_ratio(std::string& error) {
    const double ratio = debt_to_income_ratio(45'000.0, 100'000.0);
    return require_equal(45.0,
                         ratio,
                         0.01,
                         "долговая нагрузка рассчитана неверно",
                         error);
}


bool test_no_events_is_repeatable(std::string& error) {
    const simulation_result first = run_simulation(make_alice_plan(), false, 1U);
    const simulation_result second = run_simulation(make_alice_plan(), false, 987U);

    return require_true(nearly_equal(first.final_net_worth, second.final_net_worth)
                        && first.negative_cashflow_months == second.negative_cashflow_months,
                        "модель без случайных событий зависит от начального значения генератора",
                        error);
}


bool test_events_are_repeatable_by_seed(std::string& error) {
    const simulation_result first = run_simulation(make_bob_plan(), true, 44U);
    const simulation_result second = run_simulation(make_bob_plan(), true, 44U);

    return require_true(nearly_equal(first.final_net_worth, second.final_net_worth)
                        && first.total_expenses == second.total_expenses,
                        "модель со случайными событиями не воспроизводится по seed",
                        error);
}


bool test_alice_horizon(std::string& error) {
    const simulation_result result = run_simulation(make_alice_plan(), false, 1U);
    return require_true(result.months.size() == 240
                        && result.years.size() == 20
                        && result.final_net_worth > 0.0,
                        "модель Алисы не завершила двадцатилетний горизонт",
                        error);
}


bool test_bob_horizon(std::string& error) {
    const simulation_result result = run_simulation(make_bob_plan(), false, 1U);
    return require_true(result.months.size() == 240
                        && result.years.size() == 20
                        && result.final_net_worth > 0.0,
                        "модель Боба не завершила двадцатилетний горизонт",
                        error);
}


bool test_month_records_have_balances(std::string& error) {
    const simulation_result result = run_simulation(make_alice_plan(), false, 1U);
    const bool all_valid = std::all_of(result.months.begin(), result.months.end(), [](const month_record& record) {
        return record.savings_balance >= -rubles_epsilon
               && record.mortgage_balance >= -rubles_epsilon
               && std::isfinite(record.net_worth);
    });

    return require_true(all_valid,
                        "в месячных записях найдены недопустимые остатки или нечисловой капитал",
                        error);
}


bool test_comparison_has_winner(std::string& error) {
    const comparison_result comparison = compare_default_strategies(false, 1U);
    return require_true(!comparison.winner_name.empty()
                        && !comparison.conclusions.empty()
                        && std::isfinite(comparison.net_worth_difference),
                        "сравнение стратегий не сформировало вывод",
                        error);
}


bool test_ab_counts_runs(std::string& error) {
    const monte_carlo_summary summary = run_ab_experiment(10, 100U);
    return require_true(summary.alice_wins + summary.bob_wins + summary.draws == 10,
                        "A/B-тест неверно посчитал количество запусков",
                        error);
}


bool test_pet_lifespan(std::string& error) {
    pet value;
    value.enabled = true;
    value.lifespan_months = 2;
    value.age_months = 1;
    value.grow_one_month();
    return require_true(!value.alive(),
                        "питомец не прекращает учитываться после срока жизни",
                        error);
}


bool test_rent_purchase(std::string& error) {
    person_plan plan = make_bob_plan();
    plan.savings.balance = 20'000'000.0;
    plan.simulation_months = 1;
    const simulation_result result = run_simulation(plan, false, 1U);
    return require_true(result.final_plan.home.mode == housing_mode::owned_home,
                        "накопительная стратегия не покупает жильё при достаточных средствах",
                        error);
}


bool test_bob_purchase_month(std::string& error) {
    const simulation_result result = run_simulation(make_bob_plan(), false, 42U);
    const int relative_month = static_cast<int>(result.final_plan.home.purchase_month)
                               - result.final_plan.start_date.serial_month() + 1;
    return require_true(result.final_plan.home.property_owned
                        && relative_month == 57,
                        "месяц покупки квартиры Бобом рассчитан неверно",
                        error);
}


bool test_credit_card_interest(std::string& error) {
    person_plan plan = make_alice_plan();
    plan.credit_debt = 100'000.0;
    plan.simulation_months = 1;
    const simulation_result result = run_simulation(plan, false, 1U);
    return require_true(result.months.front().loan_interest_paid > 0.0,
                        "проценты по кредитной карте не учтены",
                        error);
}


bool test_rental_property_price_growth(std::string& error) {
    person_plan plan = make_bob_plan();
    const double before = plan.home.property_price;
    plan.home.apply_annual_inflation();
    const double expected = before * 1.06;
    return require_equal(expected,
                         plan.home.property_price,
                         0.01,
                         "цена квартиры не индексируется до покупки жилья",
                         error);
}


std::vector<test_case> build_tests() {
    return {
        {"Переход календарного месяца", test_date_transition},
        {"Аннуитетный платёж", test_annuity_payment},
        {"Кредит с нулевой ставкой", test_zero_rate_loan},
        {"Снижение долга очередным платежом", test_loan_payment_reduces_debt},
        {"Влияние досрочного платежа", test_extra_payment_reduces_debt},
        {"Проценты по накоплениям", test_savings_interest},
        {"Минимальный остаток вклада", test_savings_cannot_break_minimum},
        {"Отсутствие двойного начисления процентов", test_savings_interest_is_not_counted_twice},
        {"Лимит имущественного вычета", test_tax_deduction_limit},
        {"Лимит вычета по ипотечным процентам", test_interest_deduction_limit},
        {"Будущая стоимость накоплений", test_future_value},
        {"Срок достижения финансовой цели", test_target_months},
        {"Показатель долговой нагрузки", test_income_ratio},
        {"Повторяемость модели без событий", test_no_events_is_repeatable},
        {"Повторяемость модели по seed", test_events_are_repeatable_by_seed},
        {"Двадцатилетний горизонт Алисы", test_alice_horizon},
        {"Двадцатилетний горизонт Боба", test_bob_horizon},
        {"Корректность месячных остатков", test_month_records_have_balances},
        {"Формирование сравнения", test_comparison_has_winner},
        {"Количество прогонов A/B-теста", test_ab_counts_runs},
        {"Учёт срока жизни питомца", test_pet_lifespan},
        {"Покупка квартиры без кредита", test_rent_purchase},
        {"Месяц покупки квартиры Бобом", test_bob_purchase_month},
        {"Проценты по кредитной карте", test_credit_card_interest},
        {"Рост цены квартиры до покупки", test_rental_property_price_growth}
    };
}


}  // namespace


bool run_all_tests(std::ostream& output) {
    const std::vector<test_case> tests = build_tests();
    int passed = 0;

    output << "Внутреннее тестирование модели\n";
    output << std::string(70, '-') << '\n';

    for (const test_case& current_test : tests) {
        std::string error;
        const bool successful = current_test.action(error);

        if (successful) {
            ++passed;
            output << "[PASS] " << current_test.name << '\n';
        } else {
            output << "[FAIL] " << current_test.name << ": " << error << '\n';
        }
    }

    output << std::string(70, '-') << '\n';
    output << "Пройдено: " << passed << " из " << tests.size() << '\n';
    return passed == static_cast<int>(tests.size());
}

}  // namespace alice_bob

// ===== Точка входа и параметры запуска =====


namespace {

int run_command_line_mode(const std::string& argument) {
    using namespace alice_bob;

    if (argument == "--self-test") {
        return run_all_tests(std::cout) ? 0 : 1;
    }

    if (argument == "--compare") {
        print_comparison(compare_default_strategies(false, 42U), std::cout);
        return 0;
    }

    if (argument == "--events") {
        print_comparison(compare_default_strategies(true, 2026U), std::cout);
        return 0;
    }

    if (argument == "--ab-test") {
        print_ab_summary(run_ab_experiment(50, 1'000U), std::cout);
        return 0;
    }

    if (argument == "--alice") {
        const simulation_result result = run_simulation(make_alice_plan(), true, 2026U);
        print_result_summary(result, std::cout);
        print_year_table(result, std::cout);
        return 0;
    }

    if (argument == "--diagnostics") {
        const person_plan alice = make_alice_plan();
        const simulation_result base = run_simulation(alice, false, 42U);
        print_affordability(analyze_affordability(alice), std::cout);
        print_prepayment_analysis(analyze_prepayment(alice.mortgage,
                                                      alice.mortgage.required_monthly_payment * 0.2),
                                  std::cout);
        print_stress_results(run_default_stress_tests(alice, base, 5'000U), std::cout);
        return 0;
    }

    if (argument == "--quality") {
        const simulation_result alice = run_simulation(make_alice_plan(), true, 2'026U);
        print_model_statistics(alice, std::cout);
        return 0;
    }

    std::cerr << "Неизвестный параметр: " << argument << '\n';
    std::cerr << "Поддерживаются: --self-test, --compare, --events, --ab-test, --alice, --diagnostics, --quality\n";
    return 2;
}

}  // namespace


int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    if (argc > 1) {
        return run_command_line_mode(argv[1]);
    }

    alice_bob::run_interactive_console(std::cin, std::cout);
    return 0;
}