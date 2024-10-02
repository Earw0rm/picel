#include "containers/dqueue.h"
#include <stdlib.h>
#include <string.h>
#include "logger.h"



#define BASIC_CAPACITY 2
#define SCALE_FACTOR 2

typedef struct dqueue_header{
    uint64_t capacity;
    uint64_t lenght; 
    uint64_t stride;

    uint64_t head_idx;
}dqueue_header;

struct dqueue_intr{
    dqueue_header header;
    void* values;
};

void dqueue_scale(dqueue dq){
    uint64_t new_capacity = dq->header.capacity * SCALE_FACTOR ;
    uint64_t new_lenght = dq->header.lenght - dq->header.head_idx;

    void* old_values = dq->values;
    void* new_values = malloc(new_capacity * dq->header.stride);
    memcpy(new_values,
           &old_values[dq->header.head_idx * dq->header.stride],
           new_lenght * dq->header.stride);
    
    dq->values = new_values;
    dq->header.capacity = new_capacity;
    dq->header.lenght   = new_lenght;
    dq->header.head_idx = 0;
    free(old_values);
}

dqueue _dqueue_alloc(uint64_t stride){
    dqueue dq = malloc(sizeof(struct dqueue_intr));
    dq->values = malloc(stride * BASIC_CAPACITY);

    dq->header.capacity = BASIC_CAPACITY;
    dq->header.stride   = stride;
    dq->header.lenght   = 0;
    dq->header.head_idx = 0;
    return dq;
}

void _dqueue_push_back(dqueue dq, void* elem){
    if(dq->header.lenght == dq->header.capacity){
        dqueue_scale(dq);
    }
    memcpy(&dq->values[dq->header.lenght * dq->header.stride], elem, dq->header.stride);
    dq->header.lenght += 1;
}

void dqueue_free(dqueue dq){
    free(dq->values);
    free(dq);
}

void* dqueue_pop_front(dqueue dq){
    if(dq->header.lenght == 0) return nullptr;
    void* val = &dq->values[dq->header.head_idx * dq->header.stride];
    dq->header.head_idx += 1;
    dq->header.lenght   -= 1;   

    if(dq->header.lenght == 0){ // this is form of rebase (resize)
        dq->header.head_idx = 0;
    }

    return val;
}

uint64_t dqueue_stride(dqueue dq){
    return dq->header.stride;
}
uint64_t dqueue_lenght(dqueue dq){
    return dq->header.lenght;
}
uint64_t dqueue_capacity(dqueue dq){
    return dq->header.capacity;
}