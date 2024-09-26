#include "logger.h"
#include "window.h"
#include <stdio.h>



int main(int argc, char const *argv[])
{
    win_init("my first c window", 1024, 1024);
    while(!win_should_close()){
        
        win_poll_events();
        win_swap_buffers();
    }
    win_destroy();
    return 0;
}
