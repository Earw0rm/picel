#ifndef _YASYC_TEST_H_
#define _YASYC_TEST_H_
#include "logger.h"



#define ASSERT(cond, msg) do {                                                          \
    if(!(cond)){                                                                        \
        LOG_ERROR("Assertion failed(%s): %s (in %s:%d)", #cond, msg, __FILE__, __LINE__);          \
        return 0;                                                                       \
    }} while(0)



#define TEST(description, body) do {           \
    LOG_INFO("Running test: %s", description); \
    if(body()) {                               \
        LOG_INFO("Passed");                    \
    } else LOG_ERROR("Error");                 \
} while(0)
#endif 