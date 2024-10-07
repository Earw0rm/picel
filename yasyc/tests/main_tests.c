
#include "containers/dqueue.h"
#include "containers/darray.h"

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

#include "yasyc_test.h"
#include "defines.h"

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


void dqueue_tests(){
    TEST("basic alloc/free test", test1);
    TEST("basic capasity/scale test", test2);
    TEST("basic push/pop test", test3);
    TEST("massive push test", test4);
    TEST("massive push/pop test", test5);
    TEST("capacity and adresses test", test6);
}

typedef struct foof{
    float a;
} foof;

typedef struct food{
    double a;
} food;

typedef struct fooc{
    char a;
} fooc;


static size_t sizes[3] = {
    sizeof(food), sizeof(foof), sizeof(fooc)
};

int _true_macro_test(uint64_t num, ...){
    va_list ap;
    uint32_t a = 0;
    va_start(ap, num);

    a = va_arg(ap, uint32_t);
    ASSERT(sizes[0] == a, "true macros should return sizeof for all typec correctly");
    a = va_arg(ap, uint32_t);    
    ASSERT(sizes[1] == a, "true macros should return sizeof for all typec correctly");
    a = va_arg(ap, uint32_t);    
    ASSERT(sizes[2] == a, "true macros should return sizeof for all typec correctly");   
    va_end(ap);
    return 1;
}

typedef struct test_substruct_1{
    uint64_t a;
} test_substruct_1;

typedef struct test_substruct_2{
    uint64_t b;
    char a[10];
} test_substruct_2;

struct test_struct{
    test_substruct_1 t;
    test_substruct_2 t2;
    test_substruct_2 t3;    
};

static size_t offsets[3] = {
    offsetof(struct test_struct, t), 
    offsetof(struct test_struct, t2),
    offsetof(struct test_struct, t3)
};

int _true_macro_test2(uint64_t num, ...){
    va_list ap;
    uint32_t a = 0;
    va_start(ap, num);

    a = va_arg(ap, uint32_t);
    ASSERT(offsets[0] == a, "true macros 2 should return offsetof for all names correctly");
    a = va_arg(ap, uint32_t);    
    ASSERT(offsets[1] == a, "true macros 2 should return offsetof for all names correctly");
    a = va_arg(ap, uint32_t);    
    ASSERT(offsets[2] == a, "true macros 2 should return offsetof for all names correctly");   
    va_end(ap);
    return 1;
}

#define true_macro_test(...) _true_macro_test(VA_NARGS(__VA_ARGS__), SIZEOF_ALL(__VA_ARGS__))
#define true_macro_test2(...) _true_macro_test2(VA_NARGS(__VA_ARGS__), OFFSETOF_ALL(struct test_struct, __VA_ARGS__))

int tt_test(){
    return true_macro_test(food, foof, fooc);
}

int tt_test2(){
    return true_macro_test2(t, t2, t3);
}



/////////////////////////////////////


int darray_test1(){
    darray val1 = darray_alloc(int);
    uint64_t cap1    = darray_capacity(val1);
    uint64_t stride1 = darray_stride(val1);
    uint64_t lenght1 = darray_lenght(val1);

    ASSERT(cap1 == 2, "basic capacity should be equal to 2");
    ASSERT(stride1 == 4, "basic int stride should be equal to 4");
    ASSERT(lenght1 == 0, "basic lenght should be equal to 2");    


    darray_reserve(val1, 42);
 
    uint64_t cap2    = darray_capacity(val1);
    uint64_t stride2 = darray_stride(val1);
    uint64_t lenght2 = darray_lenght(val1);

    ASSERT(cap2 == 42, "capacity after scaling should be 42");
    ASSERT(stride2 == 4, "stride after scaling should no change");
    ASSERT(lenght2 == 0, "lenght after scaling without insert should not change");

    darray_reserve(val1, 43);

    uint64_t cap3 = darray_capacity(val1);
    ASSERT(cap3 == 43, "capacity after scaling should be 43");


    darray_reserve(val1, 42);

    uint64_t cap4 = darray_capacity(val1);
    ASSERT(cap4 == 43, "capacity after downscaling should not changed ");

    darray_free(val1);

    return 1;
}


int darray_test2(){
    

    darray val1 = darray_alloc(float);
    darray_push_back(val1, 10.0f);

    uint64_t cap1    = darray_capacity(val1);
    uint64_t stride1 = darray_stride(val1);
    uint64_t lenght1 = darray_lenght(val1);

    ASSERT(cap1    == 2, "one element push, capasity should not change");
    ASSERT(stride1 == sizeof(float), "stride after push should not change");
    ASSERT(lenght1 == 1, "lenght after push should be += 1");

    darray_push_back(val1, 11.0f);

    uint64_t cap2    = darray_capacity(val1);
    uint64_t stride2 = darray_stride(val1);
    uint64_t lenght2 = darray_lenght(val1);

    ASSERT(cap2    == 2, "one element push, capasity should not change");
    ASSERT(stride2 == sizeof(float), "stride after push should not change");
    ASSERT(lenght2 == 2, "lenght after push should be += 1");


    darray_push_back(val1, 12.0f);

    uint64_t cap3    = darray_capacity(val1);
    uint64_t stride3 = darray_stride(val1);
    uint64_t lenght3 = darray_lenght(val1);

    ASSERT(cap3    == 4, "capasity should change after 3 basic push back");
    ASSERT(stride3 == sizeof(float), "stride after push should not change");
    ASSERT(lenght3 == 3, "lenght after push should be += 1");


    float* i1 = (float*) darray_at(val1, 0);
    float* i2 = (float*)darray_at(val1, 1);
    float* i3 = (float*)darray_at(val1, 2);

    ASSERT(*i1 == 10, "value inside array at n position must be correct after push some value");
    ASSERT(*i2 == 11, "value inside array at n position must be correct after push some value");
    ASSERT(*i3 == 12, "value inside array at n position must be correct after push some value");

    return 1;
}

int darray_test3(){
    const int test_arr_sz = 1000;
    float test_arr[test_arr_sz];
 
    darray da = darray_alloc(float);
    darray_reserve(da, 1001);
    for(float i = 0.0f; (int)i < test_arr_sz; i += 1.0f){
        test_arr[(int)i] = i;
        darray_push_back(da, i);
    }

    bool all_eq = true;

    for(float i = 0.0f; (int)i < test_arr_sz; i += 1.0f){
        float* elem = (float*)darray_at(da, (int)i);
        if(test_arr[(int)i] != *elem){
            LOG_ERROR("MPT %f = %f", test_arr[(int)i], *elem);
            all_eq = false;
        }
    }
    ASSERT(all_eq == true, "element in darr need to be in correct push order");
    ASSERT(1001 == darray_capacity(da), "capacity should not change");    
    return 1;
}


void darray_tests(){
    TEST("darray basic capasity/scale test", darray_test1);
    TEST("darray basic push/pop test", darray_test2);
    TEST("darray massive push test", darray_test3);
}

int main(int argc, char const *argv[])
{
    dqueue_tests();
    TEST("This is mad macro test. If u need debug this use -E for preprocessor only and -P to delete #line", tt_test);
    TEST("This is test for offsetof_all macros. If u need debug this use -E for preprocessor only and -P to delete #line", tt_test2);
    darray_tests();
    return 0;
}
