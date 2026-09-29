#include <stdlib.h>
#include <stdio.h>

#include "../../types.h"
#include "../../linked_list.h"

#include "../__event.h"

EVENT_REGISTRATION(example, STAGE_YOUTH)

// bool check(Person* p, World* w)
EVENT_CHECK(example) {
    printf("\nyouth Example check!");
}

// void result(Person* p, World* w)
EVENT_RESULT(example) {
    printf("\nyouth Example result!");
}
