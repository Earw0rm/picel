#include "containers/darray.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define BASIC_CAPACITY 2
#define SCALE_FACTOR 2


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
    da->values = malloc(stride * BASIC_CAPACITY);

    da->header.capacity = BASIC_CAPACITY;
    da->header.stride   = stride;
    da->header.lenght   = 0;
    return da;
}

void darray_free(darray da){
    free(da->values);
    free(da);
}

void* darray_at(darray da, uint64_t n){
    if(n > da->header.lenght) return nullptr;
    return &da->values[da->header.stride * n];
}

void darray_push_back(darray da, void* elem){
    if(da->header.lenght == da->header.capacity){
        darray_scale(da);
    }
    memcpy(&da->values[da->header.lenght * da->header.stride], elem, da->header.stride);
    da->header.lenght += 1;
}

void darray_scale(darray da){
    void* new_value_ptr = realloc(da->values, da->header.capacity * SCALE_FACTOR);
    assert(new_value_ptr != nullptr && 
        "cannot reallocate darray" 
    );
    da->values = new_value_ptr;
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