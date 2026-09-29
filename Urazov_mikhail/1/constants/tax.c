#include <stdio.h>

#include "../types.h"
#include "../constants.h"

#define TAX_PERCENT 13

CONSTANT_REGISTRATION(tax, CONSTANT_ORDER_TAX)

CONSTANT_APPLY(tax) {
    int tax = p->salary * TAX_PERCENT / 100;
    p->money -= tax;
    printf("\nНалог (%d%%) с зарплаты %s: -%d рублей.", TAX_PERCENT, p->name, tax);
}
