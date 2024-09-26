#include "containers/dqueue.h"

#include <assert.h>
#include <stdio.h>


void test1(){
    dqueue* val1  = _dqueue_alloc(sizeof(int));
    
    _dqueue_free(val1);
    assert(val1 != 0);

    val1 = _dqueue_alloc(sizeof(int));
    dqueue_free(val1);
    assert(val1 == 0);
}

void test2(){
    dqueue* val1 = dqueue_alloc(int);
    uint64_t cap1 = dqueue_capacity(val1);
    assert(cap1 == 2);

    dqueue_scale(val1);
    uint64_t cap2 = dqueue_capacity(val1);
    assert(cap2 == 4);
}

int main(int argc, char const *argv[])
{
    test1();
    test2();

    printf("All test successufl \n");
    return 0;
}
