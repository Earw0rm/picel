#ifndef _DARRAY_H_
#define _DARRAY_H_

#include "defines.h"


typedef struct darray_intr* darray;

API darray _darray_alloc(uint64_t stride);
API void  darray_free(darray da);
/**
 * unsafe
 * Return a reference to the nth cell of the array.
 * A value can be written to or read from this cell.
 */
API void* darray_at(darray da, uint64_t n);
API void  darray_push_back(darray da, void* elem);
API void  darray_scale(darray da);

API uint64_t darray_stride(darray da);
API uint64_t darray_lenght(darray da);
API uint64_t darray_capacity(darray da);
#endif 