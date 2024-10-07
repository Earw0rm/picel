#include "containers/darray.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "logger.h"


typedef struct darray_header{
    uint64_t capacity;
    uint64_t lenght; 
    uint64_t stride;
}darray_header;

struct darray_intr{
    darray_header header;
    void* values;
};


darray _darray_alloc(uint64_t stride){
    darray da  = malloc(sizeof(struct darray_intr));
    assert(da != nullptr && 
        "cannot allocate darray" 
    );

    da->values = malloc(stride * DARRAY_BASIC_CAPACITY);
    assert(da->values != nullptr && 
        "cannot allocate darray" 
    );

    da->header.capacity = DARRAY_BASIC_CAPACITY;
    da->header.stride   = stride;
    da->header.lenght   = 0;
    return da;
}

void darray_free(darray da){
    free(da->values);
    free(da);
}

void* darray_at(darray da, uint64_t n){
    if(n >= da->header.lenght) return nullptr;
    return &da->values[da->header.stride * n];
}

void darray_reserve(darray da, uint64_t new_capacity){

    if(da->header.capacity >= new_capacity) return;
    void* new_value_ptr = realloc(da->values, new_capacity * da->header.stride);
    assert(new_value_ptr != nullptr && 
        "cannot reallocate darray" 
    );
    da->values = new_value_ptr;
    da->header.capacity = new_capacity;
}

static void darray_scale(darray da){
    darray_reserve(da, da->header.capacity * DARRAY_SCALE_FACTOR);
}

void _darray_push_back(darray da, void* elem){
    if(da->header.lenght == da->header.capacity){
        darray_scale(da);
    }
    memcpy(&da->values[da->header.lenght * da->header.stride], elem, da->header.stride);
    da->header.lenght += 1;
}

void darray_clear(darray da){
    da->header.lenght = 0;
}

uint64_t darray_stride(darray da){
    return da->header.stride;
}
uint64_t darray_lenght(darray da){
    return da->header.lenght;
}
uint64_t darray_capacity(darray da){
    return da->header.capacity;
}