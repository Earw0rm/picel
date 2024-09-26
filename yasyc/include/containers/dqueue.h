#ifndef _YASYS_DQUEUE_H_
#define _YASYS_DQUEUE_H_

/*
* TODO change to platform independent 
*/
#include <stdint.h>
#include <stdbool.h>
typedef struct dqueue dqueue;

dqueue* _dqueue_alloc(uint64_t stride);
void    _dqueue_free(dqueue* dqueue);
dqueue* _dqueue_push_back(dqueue* dqueue, void* elem);
void*   _dqeue_pop_front(dqueue* deque);
dqueue* _dqueue_scale(dqueue* dqueue);
bool _dqueue_rebase(dqueue* dqueue);

#define dqueue_push_back(dqueue, elem) do{   \
    dqueue = _dqueue_push_back(dqueue, elem) \
}                                            \

#define dqueue_free(dqueue) do{ \
    _dqueue_free(dqueue);       \
    dqueue = 0;                 \
} while(0)                      \

#define dqueue_alloc(type) _dqueue_alloc(sizeof(type));

#define dqueue_scale(dqueue) do {   \
    dqueue = _dqueue_scale(dqueue); \
}while(0)                           \

uint64_t dqueue_stride(dqueue* dqueue);
uint64_t dqueue_lenght(dqueue* dqueue);
uint64_t dqueue_capacity(dqueue* dqueue);
uint64_t dqueue_head_idx(dqueue* dqueue);






#endif 