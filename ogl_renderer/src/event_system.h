#ifndef _EVENT_SYSTEM_H_
#define _EVENT_SYSTEM_H_

#include <stdint.h>

typedef struct event_context{

    union{
        int64_t   i64[2];
        uint64_t ui64[2];
        double    d64[2];

        int32_t   i32[4];
        uint32_t ui32[4];

        int16_t   i16[8];
        uint16_t ui16[8];

        int8_t    i8[16];
        uint8_t  ui8[16];
    } data;
} event_context;

typedef enum event_code{
    // shuts the application down on the next frame
    EVENT_CODE_APPLICATION_QUIT = 0x01,

    /**
     * Context usage:
     * u16 key_code = data.u16[0]
     */
    EVENT_CODE_KEY_PRESSED = 0x02,

    /**
     * Context usage:
     * u16 key_code = data.u16[0]
     */
    EVENT_CODE_KEY_RELEASED = 0x03,

    /**
     * Context usage:
     * u16 button = data.u16[0]
     */
    EVENT_CODE_BUTTON_PRESSED = 0x04,

    /**
     * Context usage:
     * u16 button = data.u16[0]
     */
    EVENT_CODE_BUTTON_RELEASED = 0x05,

    /**
     * Context usage:
     * u16 x = data.u16[0]
     * u16 y = data.u16[1]
     */
    EVENT_CODE_MOUSE_MOVED = 0x06,    

    /**
     * Context usage:
     * u16 z_delta = data.u8[0]
     */
    EVENT_CODE_MOUSE_WHEEL = 0x07,

    /**
     * Context usage:
     * u16 width = data.u16[0]
     * u16 height = data.u16[1]
     */
    EVENT_CODE_MOUSE_RESIZED = 0x08,

    EVENT_CODE_MAX
} event_code;

// should return true if handle
typedef bool (*on_event_pfn)(event_code code, void* sender, void* listener_inst, event_context data);


void event_system_init();
void event_system_destroy();

void event_system_process_one();
void event_system_process_all();

/**
 *  @param code the event type to listen for
 *  @param listener pointer to listener instance. 
 *  @param on_event callback function pointer to be invoked when the event code is fired
 */
bool event_system_register(event_code code, void* listener, on_event_pfn on_event);


/**
 *  @param code the event type to listen for
 *  @param listener pointer to listener instance. 
 *  @param on_event the callback function pointer to be unregister
 */
bool event_system_unregister(event_code code, void* listener, on_event_pfn on_event);


// TODO fire is not a good name
/**
 * Fires an event to listeners of the given type. If an event handler returns TRUE,
 * the event is considered handled and is not passed on to any more listeners
 * @param code the event type to fire
 * @param sender a pointer to sender. Can be nullptr
 * @param context event context 
 * @returns true if handled.
 */
bool event_system_fire(event_code code, void* sender, event_context context);




#endif 