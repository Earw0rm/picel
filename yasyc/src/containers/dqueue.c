
#include "containers/dqueue.h"


#include <stdlib.h>
#include <string.h>


#define BASIC_CAPACITY 2
#define SCALE_FACTOR 2


typedef struct dqueue_header{
    uint64_t capacity;
    uint64_t lenght; // lenght it is tail idx 
    uint64_t stride;

    uint64_t head_idx;
}dqueue_header;

typedef struct dqueue{
    dqueue_header header;
    void* values;
} dqueue;



dqueue* _dqueue_alloc(uint64_t stride){
    dqueue* dqueue = malloc(sizeof(dqueue_header) 
                            + stride * BASIC_CAPACITY);
    dqueue->header.capacity = BASIC_CAPACITY;
    dqueue->header.stride   = stride;
    dqueue->header.lenght   = 0;
    dqueue->header.head_idx = 0;
    return dqueue;
}

void _dqueue_free(dqueue* deq){
    free(deq);
}

dqueue* _dqueue_scale(dqueue* dqueue){
    uint64_t new_capacity = dqueue->header.capacity * SCALE_FACTOR;
    struct dqueue* new_dqueue = malloc(sizeof(dqueue_header) 
                                       + dqueue->header.stride * new_capacity);

    new_dqueue = memcpy(new_dqueue, 
                        dqueue, 
                        dqueue->header.lenght * dqueue->header.stride 
                        + sizeof(dqueue_header));
    new_dqueue->header.capacity = new_capacity;
    _dqueue_free(dqueue);
    return new_dqueue;
}

dqueue* _dqueue_push_back(dqueue* dqueue, void* elem){
    if(dqueue->header.capacity == dqueue->header.lenght){
        dqueue_scale(dqueue);
    }
    memcpy(&dqueue->values[dqueue->header.lenght], elem, dqueue->header.stride);
    dqueue->header.lenght += 1;

    return dqueue;
}

void* _dqeue_pop_front(dqueue* dqueue){
    if(dqueue->header.lenght == 0)return 0;

    void* elem = dqueue->values + dqueue->header.head_idx * dqueue->header.stride;
    dqueue->header.head_idx += 1;

    // rebase alternative 
    if(dqueue->header.lenght == dqueue->header.head_idx){
        dqueue->header.lenght   = 0;
        dqueue->header.head_idx = 0;
    }
    return elem;
}

bool _dqueue_rebase(dqueue* dqueue){
    uint64_t diff;
    if((diff = dqueue->header.lenght - dqueue->header.head_idx) > 0){
        memmove(dqueue->values, dqueue->values + dqueue->header.head_idx * dqueue->header.stride, diff * dqueue->header.stride);
        dqueue->header.lenght -= dqueue->header.head_idx;
        dqueue->header.head_idx = 0;
        return true;
    }
    return false;
}

uint64_t dqueue_stride(dqueue* dqueue){
    return dqueue->header.stride;
}
uint64_t dqueue_lenght(dqueue* dqueue){
    return dqueue->header.lenght;
}
uint64_t dqueue_capacity(dqueue* dqueue){
    return dqueue->header.capacity;
}
uint64_t dqueue_head_idx(dqueue* dqueue){
    return dqueue->header.head_idx;
}



