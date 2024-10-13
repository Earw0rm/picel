#include "ecs_tests.h"
#include "math_tests.h"



int main(int argc, char const *argv[])
{

    ecs_run_tests();
    math_run_tests();
    return 0;
}
