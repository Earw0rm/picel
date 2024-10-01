#include "containers/dqueue.h"

#include <assert.h>
#include <stdio.h>
#include "yasyc_test.h"

int test1(){
    dqueue val1  = _dqueue_alloc(sizeof(int));
    dqueue_free(val1);
    ASSERT(val1 != nullptr, "free should delete the pointer correctly");

    return 1;
}

int test2(){
    dqueue val1 = dqueue_alloc(int);
    uint64_t cap1    = dqueue_capacity(val1);
    uint64_t stride1 = dqueue_stride(val1);
    uint64_t lenght1 = dqueue_lenght(val1);

    ASSERT(cap1 == 2, "basic capacity should be equal to 2");
    ASSERT(stride1 == 4, "basic int stride should be equal to 4");
    ASSERT(lenght1 == 0, "basic lenght should be equal to 2");    


    dqueue_scale(val1);
 
    uint64_t cap2    = dqueue_capacity(val1);
    uint64_t stride2 = dqueue_stride(val1);
    uint64_t lenght2 = dqueue_lenght(val1);

    ASSERT(cap2 == 4, "capacity after scaling should be x2");
    ASSERT(stride2 == 4, "stride after scaling should no change");
    ASSERT(lenght2 == 0, "lenght after scaling without insert should not change");

    dqueue_scale(val1);

    uint64_t cap3 = dqueue_capacity(val1);
    ASSERT(cap3 == 8, "capacity after scaling should be x2");

    dqueue_free(val1);

    return 1;
}

int test3(){
    dqueue val1 = dqueue_alloc(float);
    dqueue_push_back(val1, 10.0f);

    uint64_t cap1    = dqueue_capacity(val1);
    uint64_t stride1 = dqueue_stride(val1);
    uint64_t lenght1 = dqueue_lenght(val1);

    ASSERT(cap1    == 2, "one element push, capasity should not change");
    ASSERT(stride1 == sizeof(float), "stride after push should not change");
    ASSERT(lenght1 == 1, "lenght after push should be += 1");

    dqueue_push_back(val1, 11.0f);

    uint64_t cap2    = dqueue_capacity(val1);
    uint64_t stride2 = dqueue_stride(val1);
    uint64_t lenght2 = dqueue_lenght(val1);

    ASSERT(cap2    == 2, "one element push, capasity should not change");
    ASSERT(stride2 == sizeof(float), "stride after push should not change");
    ASSERT(lenght2 == 2, "lenght after push should be += 1");


    dqueue_push_back(val1, 12.0f);

    uint64_t cap3    = dqueue_capacity(val1);
    uint64_t stride3 = dqueue_stride(val1);
    uint64_t lenght3 = dqueue_lenght(val1);

    ASSERT(cap3    == 4, "one element push, capasity should not change");
    ASSERT(stride3 == sizeof(float), "stride after push should not change");
    ASSERT(lenght3 == 3, "lenght after push should be += 1");


    return 1;
}
int test4(){
    const int test_arr_sz = 1000;
    float test_arr[test_arr_sz];
 
    dqueue dq = dqueue_alloc(float);

    for(float i = 0.0f; (int)i < test_arr_sz; i += 1.0f){
        test_arr[(int)i] = i;
        dqueue_push_back(dq, i);
    }

    bool all_eq = true;
    for(float i = 0.0f; (int)i < test_arr_sz; i += 1.0f){
        float* elem = (float*)dqeue_pop_front(dq);
        if(test_arr[(int)i] != *elem){
            LOG_ERROR("MPT %f = %f", test_arr[(int)i], *elem);
            all_eq = false;
        }
    }
    ASSERT(all_eq == true, "element in deq need to be in correct push order");
    return 1;
}
int test5(){

    return 0;
}
int main(int argc, char const *argv[])
{
    TEST("basic alloc/free test", test1);
    TEST("basic capasity/scale test", test2);
    TEST("basic push/pop test", test3);
    TEST("massive push test", test4);
    TEST("massive push/pop test", test5);
    return 0;
}
