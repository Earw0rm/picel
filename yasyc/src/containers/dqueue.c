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

dqueue  _dqueue_scale(dqueue dq){
    uint64_t new_capacity = dq->header.capacity * SCALE_FACTOR ;
    uint64_t new_lenght = dq->header.lenght - dq->header.head_idx;

    dqueue new_dq = malloc(sizeof(struct dqueue_intr));
    new_dq->values = malloc(new_capacity * dq->header.stride);


    new_dq->header.capacity = new_capacity;
    new_dq->header.lenght   = new_lenght;
    new_dq->header.stride   = dq->header.stride;
    new_dq->header.head_idx = 0;

    memcpy(new_dq->values,
           &dq->values[dq->header.head_idx * dq->header.stride],
           new_lenght * dq->header.stride);

    _dqueue_free(dq);
    return new_dq;
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

dqueue _dqueue_push_back(dqueue dq, void* elem){
    if(dq->header.lenght == dq->header.capacity){
        dq = _dqueue_scale(dq);
    }
    memcpy(&dq->values[dq->header.lenght * dq->header.stride], elem, dq->header.stride);
    dq->header.lenght += 1;
    return dq;
}

void _dqueue_free(dqueue dq){
    free(dq->values);
    free(dq);
}

void* _dqeue_pop_front(dqueue dq){
    if(dq->header.lenght == dq->header.capacity) return nullptr;
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