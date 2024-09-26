#include "logger.h"
#include "window.h"
#include <stdio.h>

static window win;

int main(int argc, char const *argv[])
{
    win_init("my first c window", 1024, 1024, &win);
    while(!win_should_close(&win)){
        win_poll_events(&win);
    }
    win_destroy(&win);
    return 0;
}
