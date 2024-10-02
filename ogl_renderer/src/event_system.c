#include "event_system.h"
#include "containers/dqueue.h"
#include "logger.h"


#include <memory.h>
#include <stdlib.h>



typedef struct event_subscription{
    struct event_subscription* next;
    void* listener;
    on_event_pfn callback;
}event_subscription;



typedef struct event{
    event_code code;
    void* sender;
    event_context ctx;
} event;

// all static 0 by default, mean that there is in bss section
static event_subscription* state[EVENT_CODE_MAX];
static bool is_initialized = false;
static dqueue dq;


void event_system_init(){
    if(is_initialized){
        LOG_ERROR("Event system double initialization");
        return;
    }

    //TODO check that dequeue not null and other stuff
    memset(state, 0, sizeof state);
    dq = dqueue_alloc(event);

    is_initialized = true;
}

void event_system_destroy(){
    if(is_initialized == false){
        LOG_ERROR("Event system calling destroy when not initialized");
        return;
    }

    is_initialized = false;
    //TODO zero all resources
    // we need to wait all messages handle?
    dqueue_free(dq);
}



bool event_system_register(event_code code, void* listener, on_event_pfn on_event){
    if(is_initialized == false){
        LOG_ERROR("Event system calling register when not initialized");
        return false;
    }

    //check that es != null
    event_subscription* es = malloc(sizeof(event_subscription));
    event_subscription* last = state[code];

    es->listener = listener;
    es->next = last;
    es->callback = on_event;
    state[code] = es;
    return true;
}



bool event_system_unregister(event_code code, void* listener, on_event_pfn on_event){
    if(is_initialized == false){
        LOG_ERROR("Event system calling unregister when not initialized");
        return false;
    }
    event_subscription* last = state[code];
    if(last == nullptr) return false;
    if(last->listener == listener && last->callback == on_event){
        state[code] = last->next;
        free(last);
        return true;
    }

    bool success = false;
    event_subscription* next = last->next;
    while(next != nullptr){
        if(next->listener == listener && next->callback == on_event){
            last->next = next->next;
            free(next);
            success = true;
            break;
        }
        last = next;
        next = next->next;
    }
    return success;
}

bool event_system_fire(event_code code, void* sender, event_context context){
    if(is_initialized == false){
        LOG_ERROR("Event system calling fire when not initialized");
        return false;
    }
    //TODO how i can do emplase hire?
    event event = {
        .code = code, 
        .sender = sender,
        .ctx = context
    };
    
    dqueue_push_back(dq, event);
    return true;
}


void event_system_process_one(){
    if(is_initialized == false){
        LOG_ERROR("Event system calling process_one when not initialized");
        return;
    }
    event* e = (event*)dqueue_pop_front(dq);
    if(e != nullptr){
        event_subscription* es = state[e->code];
        while(es != nullptr){
            es->callback(e->code, e->sender, es->listener, e->ctx);
            es = es->next;
        }
    }
}

void event_system_process_all(){
    if(is_initialized == false){
        LOG_ERROR("Event system calling process_all when not initialized");
        return;
    }    
    event* e;
    while((e = (event*)dqueue_pop_front(dq)) != nullptr){
        event_subscription* es = state[e->code];
        while(es != nullptr){
            es->callback(e->code, e->sender, es->listener, e->ctx);
            es = es->next;
        }
    }
}