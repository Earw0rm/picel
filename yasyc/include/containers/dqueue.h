#ifndef _DQUEUE_H_
#define _DQUEUE_H_
#include <stdint.h>
#include "defines.h"

#define DQUEUE_BASIC_CAPACITY 2
#define DQUEUE_SCALE_FACTOR 2

typedef struct dqueue_intr* dqueue;
#define dqueue_alloc(type)   _dqueue_alloc(sizeof(type));
#define dqueue_alloc_fix(sz) _dqueue_alloc(sz);



#define dqueue_push_back(dq, elem) do{       \
    typeof(elem) val = elem;                 \
    _dqueue_push_back(dq, (void*) &val);     \
} while(0)                                   \


API dqueue _dqueue_alloc(uint64_t stride);
API void dqueue_free(dqueue dq);
API void* dqueue_pop_front(dqueue dq);
API void _dqueue_push_back(dqueue dq, void* elem);
API void dqueue_scale(dqueue dq);

API uint64_t dqueue_stride(dqueue dq);
API uint64_t dqueue_lenght(dqueue dq);
API uint64_t dqueue_capacity(dqueue dq);
API uint64_t dqueue_head_idx(dqueue dq);

#endif 
