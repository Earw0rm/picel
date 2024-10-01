#ifndef _DQUEUE_H_
#define _DQUEUE_H_
#include <stdint.h>

typedef struct dqueue_intr* dqueue;
#define dqueue_alloc(type) _dqueue_alloc(sizeof(type));

#define dqueue_push_back(dq, elem) do{       \
    typeof(elem) val = elem;                 \
    _dqueue_push_back(dq, (void*) &val);     \
} while(0)                                   \

dqueue _dqueue_alloc(uint64_t stride);
void dqueue_free(dqueue dq);
void* dqueue_pop_front(dqueue dq);
void _dqueue_push_back(dqueue dq, void* elem);
void dqueue_scale(dqueue dq);

uint64_t dqueue_stride(dqueue dq);
uint64_t dqueue_lenght(dqueue dq);
uint64_t dqueue_capacity(dqueue dq);
uint64_t dqueue_head_idx(dqueue dq);

#endif 
