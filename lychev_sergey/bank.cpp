// ============================================================
// bank.cpp
//
// Реализация банковской модели: счета, проценты, кредиты, МФО.
// Здесь собраны операции над банком и его счетами и ежемесячное
// начисление процентов по вкладам и кредитам.
// ============================================================

#include "bank.h"

#include <cmath>
#include <cstdio>

// ============================================================
// Вспомогательные функции
// ============================================================

// Переводит годовую процентную ставку в месячную.
// Считаем, что за год ставка начисляется как сложный процент,
// поэтому месячная ставка r удовлетворяет (1 + r)^12 = 1 + R,
// где R — годовая ставка 

double monthly_rate_from_year_percent(double rate_year_percent)
{
    // При ставке -100% и меньше степень теряет смысл, возвращаем 0 и не ломаем pow (byfxt выдаст NaN)
    if (rate_year_percent <= -100.0) {
        return 0.0;
    }

    const double r = rate_year_percent / 100.0;
    return std::pow(1.0 + r, 1.0 / 12.0) - 1.0;
}

// Аннуитетный платёж: сколько нужно платить в месяц, чтобы закрыть
// долг body за months месяцев при ставке rate_year_percent
//
// Формула: payment = body * r * (1 + r)^n / ((1 + r)^n - 1),
// где r — месячная ставка, n — число месяцев
//
// При нулевой (или отрицательной) ставке формула вырождается,
// поэтому этот случай обрабатываем отдельно — простым делением
RUB bank_annuity(RUB body, double rate_year_percent, int months)
{
    // Без положительного срока и тела кредита платежа не существует
    if (months <= 0 || body <= 0) {
        return 0;
    }

    const double r = monthly_rate_from_year_percent(rate_year_percent);

    // Нет процентов — платёж равен телу, поделённому на срок.
    // static_cast<double> нужен, чтобы деление было дробным:
    // иначе RUB / int обрежет копейки
    if (r <= 0.0) {
        double per_month = static_cast<double>(body) / months;
        long long rounded = std::llround(per_month);
        return static_cast<RUB>(rounded);
    }

    // Считаем (1 + r)^n один раз — оно нужно дважды в дроби ниже.
    const double pow_rn = std::pow(1.0 + r, static_cast<double>(months));

    // Вся дробь r*(1+r)^n / ((1+r)^n - 1) целиком. Умножив её на тело,
    // получим ежемесячный платёж.
    const double coef = (r * pow_rn) / (pow_rn - 1.0);

    // Округляем вверх: переплата минимальна, потому что последний
    // платёж ограничивается остатком долга (см. pay_one_loan
    // в person.cpp). std::ceil возвращает double, поэтому приводим
    // к RUB явно
    double raw = static_cast<double>(body) * coef;
    double ceiled = std::ceil(raw);
    return static_cast<RUB>(ceiled);
}

// ============================================================
// Создание и поиск счетов
// ============================================================

// Инициализирует банк: задаёт имя и обнуляет счётчики процентов
void bank_init(Bank& b, const std::string& name)
{
    b.name = name;
    b.accounts.clear();
    b.total_interest_earned = 0;
    b.total_interest_paid   = 0;
}

// Открывает новый счёт в банке и возвращает его индекс
//
// Для кредита сразу задаётся срок по умолчанию. Конкретный размер
// платежа станет известен позже — в момент первого bank_withdraw,
// когда появится сумма тела
int bank_open_account(Bank& b, AccountKind kind,
                      double rate_year_percent, RUB limit,
                      int year, int month)
{
    Account a;
    a.kind         = kind;
    a.limit        = limit;
    a.rate_year    = rate_year_percent;
    a.opened_year  = year;
    a.opened_month = month;

    // Кредит рождается с пустым телом и сроком по умолчанию
    // Платёж рассчитается в bank_withdraw, когда сумма станет известна
    if (kind == AccountKind::Loan) {
        a.term_months_left = LOAN_TERM_MONTHS;
    }

    b.accounts.push_back(a);
    return static_cast<int>(b.accounts.size()) - 1;
}

// Возвращает указатель на счёт по индексу
// Если индекс вне диапазона — nullptr: вызывающий код обязан проверить
Account* bank_get(Bank& b, int acc)
{
    const int total = static_cast<int>(b.accounts.size());
    if (acc < 0 || acc >= total) {
        return nullptr;
    }
    return &b.accounts[acc];
}

// ============================================================
// Внутренние хелперы — общая логика для deposit / withdraw / transfer
// Помещены в анонимный namespace, чтобы иметь внутреннее связывание
// и не «светить» имена за пределами этого файла.
// ============================================================

namespace {

// Можно ли снять (или перевести) сумму со счёта?
//
// Для дебета/вклада: хватает ли баланса.
// Для кредита: balance — это долг, principal — сколько уже взято,
// значит проверяем, что новое тело principal + amount не превысит лимит.
bool can_withdraw(const Account* a, RUB amount)
{
    if (a->kind == AccountKind::Loan) {
        return a->principal + amount <= a->limit;
    }
    return a->balance >= amount;
}

// Можно ли положить (или перевести) сумму на счёт?
//
// Для кредита нельзя переплатить: сумма не должна превышать текущий
// долг. Для остальных типов ограничений нет
bool can_deposit(const Account* a, RUB amount)
{
    if (a->kind == AccountKind::Loan) {
        return amount <= a->balance;
    }
    return true;
}

// Применяет снятие. Вызывать ТОЛЬКО после успешной can_withdraw.
//
// Для кредита: увеличиваем тело и долг, пересчитываем аннуитет на
// оставшийся срок. Для дебета/вклада: уменьшаем баланс
void apply_withdraw(Account* a, RUB amount)
{
    if (a->kind == AccountKind::Loan) {
        a->principal += amount;
        a->balance   += amount;
        a->monthly_payment = bank_annuity(a->balance,
                                          a->rate_year,
                                          a->term_months_left);
    } else {
        a->balance -= amount;
    }
}

// Применяет пополнение. Вызывать ТОЛЬКО после успешной can_deposit
//
// Для кредита balance — это долг, поэтому пополнение его уменьшает
// Для дебета/вклада — увеличивает баланс
void apply_deposit(Account* a, RUB amount)
{
    if (a->kind == AccountKind::Loan) {
        a->balance -= amount;
    } else {
        a->balance += amount;
    }
}

} // namespace

// ============================================================
// Публичные операции
// ============================================================

// Положить деньги на счёт
//
// Возвращает true при успехе. false — если счёт неактивен, сумма
// не положительна или нарушено ограничение (переплата по кредиту)
bool bank_deposit(Bank& b, int acc, RUB amount)
{
    if (amount <= 0) {
        return false;
    }

    Account* a = bank_get(b, acc);
    if (a == nullptr || !a->active) {
        return false;
    }
    if (!can_deposit(a, amount)) {
        return false;
    }

    apply_deposit(a, amount);
    return true;
}

// Снять деньги со счёта. Для кредита это означает «взять ещё»:
// увеличиваются и тело (principal), и текущий долг (balance)
//
// Возвращает true при успехе. false — если счёт неактивен, сумма
// не положительна или превышает допустимый лимит
bool bank_withdraw(Bank& b, int acc, RUB amount)
{
    if (amount <= 0) {
        return false;
    }

    Account* a = bank_get(b, acc);
    if (a == nullptr || !a->active) {
        return false;
    }
    if (!can_withdraw(a, amount)) {
        return false;
    }

    apply_withdraw(a, amount);
    return true;
}

// Перевести деньги между счетами
//
// Обе проверки (списание со src и зачисление на dst) выполняются
// ДО любых мутаций. Иначе при отказе на второй проверке можно
// оставить систему в промежуточном состоянии (списали, но не
// зачислили)
//
// Возвращает true при успехе. false — если счета неактивны, совпадают
// или хотя бы одна из проверок не прошла
bool bank_transfer(Bank& b, int from, int to, RUB amount)
{
    if (amount <= 0) {
        return false;
    }
    if (from == to) {
        return false;
    }

    Account* src = bank_get(b, from);
    Account* dst = bank_get(b, to);
    if (src == nullptr || dst == nullptr || !src->active || !dst->active) {
        return false;
    }

    if (!can_withdraw(src, amount)) {
        return false;
    }
    if (!can_deposit(dst, amount)) {
        return false;
    }

    apply_withdraw(src, amount);
    apply_deposit(dst, amount);
    return true;
}

// Закрывает счёт: обнуляет баланс, тело, платёж, срок и помечает
// счёт неактивным. Закрытый счёт нельзя ни пополнить, ни снять с него
void bank_close_account(Bank& b, int acc)
{
    Account* a = bank_get(b, acc);
    if (a == nullptr) {
        return;
    }

    a->balance          = 0;
    a->principal        = 0;
    a->monthly_payment  = 0;
    a->term_months_left = 0;
    a->active           = false;
}

// ============================================================
// Начисление процентов — по одному хелперу на тип счёта.
// ============================================================

namespace {

// Начисляет проценты по вкладу: увеличивает баланс
// Сумма процентов учитывается в статистике банка (total_interest_paid)
void accrue_deposit(Account& a, Bank& b, double r_month)
{
    const double raw      = static_cast<double>(a.balance) * r_month;
    const RUB    interest = static_cast<RUB>(std::llround(raw));

    a.balance             += interest;
    b.total_interest_paid += interest;
}

// Начисляет проценты по кредиту: увеличивает текущий долг (balance)
//
// Для МФО применяется законный потолок переплаты: balance не должен
// превышать principal * MFO_CAP_FACTOR. У банка такого потолка нет
//
// Также уменьшает оставшийся срок. Если срок кончился, а долг
// остался — запускает реструктуризацию (продление)
void accrue_loan(Account& a, Bank& b, double r_month)
{
    const double raw      = static_cast<double>(a.balance) * r_month;
    RUB          interest = static_cast<RUB>(std::llround(raw));

    // У МФО — законный потолок переплаты. У банка — нет
    if (b.is_mfo && a.principal > 0) {
        const double raw_cap = static_cast<double>(a.principal) * MFO_CAP_FACTOR;
        const RUB    cap     = static_cast<RUB>(std::llround(raw_cap));
        const RUB    headroom    = cap - a.balance;

        if (headroom <= 0) {
            // Долг уже на потолке — проценты не начисляем
            interest = 0;
        } else if (interest > headroom) {
            // Начисление упирается в потолок — обрезаем до headroom
            interest = headroom;
        }
    }

    a.balance               += interest;
    b.total_interest_earned += interest;

    if (a.term_months_left > 0) {
        a.term_months_left--;
    }

    // Срок вышел, а долг остался — реструктуризация (продление)
    // Но если остаток меньше двух платежей — это хвост округления,
    // растягивать его на годы не имеет смысла
    if (a.term_months_left <= 0 && a.balance > a.monthly_payment * 2) {
        a.term_months_left = LOAN_RESTRUCTURE_MONTHS;
        a.monthly_payment  = bank_annuity(a.balance,
                                          a.rate_year,
                                          a.term_months_left);
    }
}

} // namespace

// Ежемесячное начисление процентов по всем активным счетам
//
// Проценты начисляются ДО того, как клиент вносит платёж, — иначе
// аннуитетный график не сойдётся (см. порядок вызовов
// в world_step_month, simulation.cpp).
void bank_accrue_monthly(Bank& b)
{
    const int total = static_cast<int>(b.accounts.size());
    for (int i = 0; i < total; ++i) {
        Account& a = b.accounts[i];

        // Неактивные счета и счета с нулевым балансом не трогаем
        if (!a.active || a.balance == 0) {
            continue;
        }

        const double r_month = monthly_rate_from_year_percent(a.rate_year);

        if (a.kind == AccountKind::Deposit) {
            accrue_deposit(a, b, r_month);
        } else if (a.kind == AccountKind::Loan) {
            accrue_loan(a, b, r_month);
        }
        // Debit — проценты не начисляются
    }
}

// ============================================================
// Запросы и печать
// ============================================================

// Возвращает баланс счёта. Если индекс некорректен — 0
RUB bank_balance(const Bank& b, int acc)
{
    const int total = static_cast<int>(b.accounts.size());
    if (acc < 0 || acc >= total) {
        return 0;
    }
    return b.accounts[acc].balance;
}

// Сумма балансов всех счетов указанного типа
// Полезна для сводок (например, «сколько всего лежит на вкладах»)
RUB bank_total_by_kind(const Bank& b, AccountKind kind)
{
    RUB sum = 0;

    const int total = static_cast<int>(b.accounts.size());
    for (int i = 0; i < total; ++i) {
        if (b.accounts[i].kind == kind) {
            sum += b.accounts[i].balance;
        }
    }
    return sum;
}

// Печатает состояние банка и всех его счетов
//
// Формат строки для кредита: дополнительно выводятся тело, лимит,
// ежемесячный платёж и оставшийся срок
void bank_print(const Bank& b)
{
    const char* mfo_suffix = b.is_mfo ? " (МФО)" : "";

    std::printf("\n=== Банк \"%s\"%s ===\n", b.name.c_str(), mfo_suffix);
    std::printf("Счетов: %d\n", static_cast<int>(b.accounts.size()));

    const int total = static_cast<int>(b.accounts.size());
    for (int i = 0; i < total; ++i) {
        const Account& a = b.accounts[i];

        const char* kind_name = "?";
        switch (a.kind) {
            case AccountKind::Debit:   kind_name = "дебет";  break;
            case AccountKind::Deposit: kind_name = "вклад";  break;
            case AccountKind::Loan:    kind_name = "кредит"; break;
        }

        std::printf("  #%d %-7s баланс=%lld",
                    i,
                    kind_name,
                    static_cast<long long>(a.balance));

        // Для кредита дополняем строку специфичными полями
        if (a.kind == AccountKind::Loan) {
            std::printf(" тело=%lld лимит=%lld платёж=%lld срок=%d",
                        static_cast<long long>(a.principal),
                        static_cast<long long>(a.limit),
                        static_cast<long long>(a.monthly_payment),
                        a.term_months_left);
        }

        const char* active_str = a.active ? "да" : "нет";
        std::printf(" ставка=%.2f%% активен=%s\n",
                    a.rate_year,
                    active_str);
    }

    std::printf("Заработано на процентах: %lld\n",
                static_cast<long long>(b.total_interest_earned));
    std::printf("Выплачено по вкладам:    %lld\n",
                static_cast<long long>(b.total_interest_paid));
}
