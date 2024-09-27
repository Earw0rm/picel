#include "logger.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

static const char* lvl2str[6] = {
    [LOG_LEVEL_FATAL] = "\033[0;41m[FATAL]: ",
    [LOG_LEVEL_ERROR] = "\033[1:31m[ERROR]: ",
    [LOG_LEVEL_WARN]  = "\033[1:33m[WARN]: " ,
    [LOG_LEVEL_INFO]  = "\033[1;32m[INFO]: " ,
    [LOG_LEVEL_DEBUG] = "\033[1;34m[DEBUG]: ",
    [LOG_LEVEL_TRACE] = "\033[1;30m[TRACE]: ",
};
#define ENDL "\033[0m\n"
#define LEVEL_STRING(log_level) lvl2str[log_level]

[[gnu::format(printf, 2, 3)]] 
void log_output(log_level lvl, const char* msg, ...){
   va_list args;
   va_start(args, msg);
   printf("%s", LEVEL_STRING(lvl));
   vprintf(msg, args);
   printf(ENDL);
}

