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
        float* elem = (float*)dqueue_pop_front(dq);
        if(test_arr[(int)i] != *elem){
            LOG_ERROR("MPT %f = %f", test_arr[(int)i], *elem);
            all_eq = false;
        }
    }
    ASSERT(all_eq == true, "element in deq need to be in correct push order");
    return 1;
}

int test5(){
    dqueue dq = dqueue_alloc(int);

    bool all_eq = true;
    for(int i = 0; i < 1998; ++i){
        dqueue_push_back(dq, i);
        int* ii  = (int*)dqueue_pop_front(dq);
        int* ii2 = (int*)dqueue_pop_front(dq);

        if(ii2 != nullptr){
            all_eq = false;
            LOG_ERROR("Ptr is broken, i = %i", i);
        }
        if(*ii != i){
            all_eq = false;
            LOG_ERROR("Value is broken, i = %i, *ii = %i", i, *ii);
        }
    }
    uint64_t capacity = dqueue_capacity(dq);
    ASSERT(all_eq, "second pop after push should return nullptr");
    ASSERT(capacity == 2, "capacity need to be 2 in this case");
    return 1;
}
int test6(){
    dqueue dq = dqueue_alloc(int);
    dqueue_push_back(dq, 1);
    dqueue_push_back(dq, 1);
    dqueue_push_back(dq, 1);
    int* i1  = (int*)dqueue_pop_front(dq);
    int* i2  = (int*)dqueue_pop_front(dq);
    int* i3  = (int*)dqueue_pop_front(dq);
    dqueue_push_back(dq, 1);
    dqueue_push_back(dq, 1);
    dqueue_push_back(dq, 1);
    int* i11  = (int*)dqueue_pop_front(dq);
    int* i12  = (int*)dqueue_pop_front(dq);
    int* i13  = (int*)dqueue_pop_front(dq);

    uint64_t capacity = dqueue_capacity(dq);
    uint64_t lenght = dqueue_lenght(dq);

    ASSERT(capacity == 4, "capacity should not increase when we use push/pop sequentialy");
    ASSERT(lenght   == 0, "after 3 push and 3pop lenght should be 0");
    ASSERT(i1 == i11, "addreses of values in dqueue should be same when we use push/pop sequentialy");
    ASSERT(i2 == i12, "addreses of values in dqueue should be same when we use push/pop sequentialy");
    ASSERT(i3 == i13, "addreses of values in dqueue should be same when we use push/pop sequentialy");
    return 1;
}
int main(int argc, char const *argv[])
{
    TEST("basic alloc/free test", test1);
    TEST("basic capasity/scale test", test2);
    TEST("basic push/pop test", test3);
    TEST("massive push test", test4);
    TEST("massive push/pop test", test5);
    TEST("capacity and adresses test", test6);


    return 0;
}
