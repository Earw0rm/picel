#ifndef _DARRAY_H_
#define _DARRAY_H_

#include "defines.h"

#define DARRAY_BASIC_CAPACITY 2
#define DARRAY_SCALE_FACTOR 2

typedef struct darray_intr* darray;
#define darray_alloc(type) _darray_alloc(sizeof(type));
#define darray_alloc_fix(sz) _darray_alloc(sz);


API darray _darray_alloc(uint64_t stride);
API void  darray_free(darray da);
/**
 * unsafe
 * Return a reference to the nth cell of the array.
 * A value can be written to or read from this cell.
 */
API void* darray_at(darray da, uint64_t n);

API void  darray_reserve(darray da, uint64_t new_capacity);
API void  darray_clear(darray da);

API uint64_t darray_stride(darray da);
API uint64_t darray_lenght(darray da);
API uint64_t darray_capacity(darray da);


API void _darray_push_back(darray da, void* elem);
#define darray_push_back(dq, elem) do{       \
    typeof(elem) val = elem;                 \
    _darray_push_back(dq, (void*) &val);     \
} while(0)       

#define darray_emplace_back(dq, pointer) do{ \
    _darray_push_back(dq, (void*) pointer);  \
} while(0)

#endif 