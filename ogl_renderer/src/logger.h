#ifndef _LOGGER_H_
#define _LOGGER_H_

typedef enum{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_INFO  = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,                    
} log_level;

void log_output(log_level lvl, const char* msg, ...);

#define LOG_WARN_ENABLED  1
#define LOG_INFO_ENABLED  1 
#define LOG_DEBUG_ENABLED 1 
#define LOG_TRACE_ENABLED 1

#if PRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0 
#endif 


#define LOG_ERROR(msg, ...) log_output(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...) log_output(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__)

#if (LOG_WARN_ENABLED == 1)
#define LOG_WARN(msg, ...) log_output(LOG_LEVEL_WARN, msg, ##__VA_ARGS__)
#else
#define LOG_WARN(msg, ...)
#endif

#if (LOG_INFO_ENABLED == 1)
#define LOG_INFO(msg, ...) log_output(LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#else 
#define LOG_INFO(msg, ...)
#endif

#if (LOG_DEBUG_ENABLED == 1)
#define LOG_DEBUG(msg, ...) log_output(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
#else 
#define LOG_DEBUG(msg, ...)
#endif

#if (LOG_TRACE_ENABLED == 1)
#define LOG_TRACE(msg, ...) log_output(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)
#else 
#define LOG_TRACE(msg, ...)
#endif
#endif 