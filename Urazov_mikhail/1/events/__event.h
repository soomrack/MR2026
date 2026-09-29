#ifndef EVENT
#define EVENT
#include "../types.h"
#include "../linked_list.h"
#include "../signal.h"

typedef enum EventStage {
    STAGE_YOUTH,
    STAGE_MIDDLEAGE,
    STAGE_OLD
} EventStage;

typedef void (*CB)(Node* l);
typedef void (*Result)(Person* p, World* w);
typedef bool (*Check)(Person* p, World* w);

typedef struct Event {
    Result result;
    Check check;
    EventStage stage;
} Event;

#define EVENT_RESULT(name)  void name##_result(Person* p, World* w)
#define EVENT_CHECK(name)   bool name##_check(Person* p, World* w) 

#define EVENT_REGISTRATION(name, st) \
EVENT_RESULT(name); \
EVENT_CHECK(name); \
void setup_##name(Node* event_list) {Event* e=malloc(sizeof(Event));e->stage=st;e->check=name##_check;e->result=name##_result;push_back(event_list,e);}\
__attribute__((constructor)) \
static void pre_setup_##name(){try_init_signals();register_signal(setup_##name);}

#endif