#include "constants.h"

#define MAX_CONSTANTS 64

typedef struct ConstantEntry {
    ConstantApply apply;
    int order;
} ConstantEntry;

static ConstantEntry constants[MAX_CONSTANTS];
static int constant_amount = 0;

void register_constant(ConstantApply apply, int order) {
    if (constant_amount >= MAX_CONSTANTS) {
        return;
    }
    int pos = constant_amount;
    while (pos > 0 && constants[pos - 1].order > order) {
        constants[pos] = constants[pos - 1];
        pos--;
    }
    constants[pos].apply = apply;
    constants[pos].order = order;
    constant_amount++;
}

void apply_constants(Person* p, World* w) {
    for (int i = 0; i < constant_amount; i++) {
        constants[i].apply(p, w);
    }
}
