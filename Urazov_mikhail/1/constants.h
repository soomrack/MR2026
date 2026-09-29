#ifndef CONSTANTS
#define CONSTANTS
#include "types.h"


#define CONSTANT_ORDER_INCOME   10
#define CONSTANT_ORDER_TAX      20
#define CONSTANT_ORDER_LIVING   25
#define CONSTANT_ORDER_PAYMENT  30

typedef void (*ConstantApply)(Person* p, World* w);

void register_constant(ConstantApply apply, int order);

void apply_constants(Person* p, World* w);

#define CONSTANT_APPLY(name) void name##_constant(Person* p, World* w)

#define CONSTANT_REGISTRATION(name, order) \
CONSTANT_APPLY(name); \
__attribute__((constructor)) \
static void pre_setup_constant_##name(){register_constant(name##_constant, order);}

#endif
