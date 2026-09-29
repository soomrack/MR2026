#include <stdio.h>

#include "../types.h"
#include "../constants.h"

CONSTANT_REGISTRATION(salary, CONSTANT_ORDER_INCOME)

CONSTANT_APPLY(salary) {
    p->money += p->salary;
    printf("\nЗарплата %s за год: +%d рублей.", p->name, p->salary);
}
