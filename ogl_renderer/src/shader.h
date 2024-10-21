#ifndef _SHADER_H_
#define _SHADER_H_
#include <glad/glad.h>

typedef struct shader{
    GLint program;

    GLint uniform_location_model;
    GLint uniform_location_view;
    GLint uniform_location_projection;
    GLint uniform_location_g_sampler;
    bool is_initialized;
} shader;


enum shader_status{
    SHADER_STATUS_BAD_IS_UNINITIALIZED       = -13,
    SHADER_STATUS_BAD_UNIFORM            = -12,    
    SHADER_STATUS_BAD_PROGRAM_VALIDATION     = -11,    
    SHADER_STATUS_BAD_PROGRAM_LINKAGE        = -10,    
    SHADER_STATUS_BAD_PROGRAM_CREATE         = -9,
    SHADER_STATUS_BAD_COMPILE                = -8,
    SHADER_STATUS_BAD_FCLOSE                 = -7,
    SHADER_STATUS_BAD_EXTENSION              = -6,
    SHADER_STATUS_BAD_FREAD                  = -5,
    SHADER_STATUS_BAD_ALLOC                  = -4,
    SHADER_STATUS_BAD_FTELL                  = -3,
    SHADER_STATUS_BAD_FSEEK                  = -2,
    SHADER_STATUS_BAD_OPEN_FILE              = -1,
    SHADER_STATUS_OK                         =  0,
    SHADER_STATUS_MAX_VALUE
};

int8_t shader_init(const char* vert_path, 
                   const char* frag_path, 
                   bool check_location,
                   shader* sp);


void shader_destroy(shader* sp);






#endif 