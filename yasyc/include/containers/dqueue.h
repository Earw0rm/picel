#ifndef _DQUEUE_H_
#define _DQUEUE_H_
#include <stdint.h>

typedef struct dqueue_intr* dqueue;

dqueue  _dqueue_alloc(uint64_t stride);
void    _dqueue_free(dqueue dq);
void*   _dqeue_pop_front(dqueue dq);
dqueue  _dqueue_push_back(dqueue dq, void* elem);
dqueue  _dqueue_scale(dqueue dq);

#define dqueue_push_back(dq, elem) do{       \
    typeof(elem) val = elem;                 \
    dq = _dqueue_push_back(dq, (void*) &val);\
} while(0)                                   \

#define dqueue_free(dq)    do{ \
    _dqueue_free(dq);          \
    dq = nullptr;              \
} while(0)                     \

#define dqueue_alloc(type) _dqueue_alloc(sizeof(type));

#define dqueue_scale(dq)   do{ \
    dq =_dqueue_scale(dq);     \
} while(0)                     \

uint64_t dqueue_stride(dqueue dq);
uint64_t dqueue_lenght(dqueue dq);
uint64_t dqueue_capacity(dqueue dq);
uint64_t dqueue_head_idx(dqueue dq);

#endif 
