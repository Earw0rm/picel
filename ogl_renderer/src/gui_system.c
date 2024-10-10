#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"
#include "gui_system.h"

//https://github.com/Immediate-Mode-UI/Nuklear/blob/master/demo/glfw_opengl3/main.c

static struct state{
    struct nk_glfw glfw;
    struct nk_context *ctx;

    struct nk_colorf bg;
    uint32_t max_vertex_buffer;
    uint32_t max_element_buffer;
}state;



void gui_system_init(window win, uint32_t max_vertex_buffer, uint32_t max_element_buffer){
    GLFWwindow* glfw_ctx = win_get_glfw_ctx(win);
    state.ctx = nk_glfw3_init(&state.glfw, glfw_ctx, NK_GLFW3_INSTALL_CALLBACKS);
    state.bg.r = 0.10f, state.bg.g = 0.18f, state.bg.b = 0.24f, state.bg.a = 1.0f;

    state.max_vertex_buffer = max_vertex_buffer;
    state.max_element_buffer = max_element_buffer;

    // nuklear font state initialization
    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&state.glfw, &atlas);
    /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
    /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
    /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
    /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
    /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
    /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
    nk_glfw3_font_stash_end(&state.glfw);
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    /*nk_style_set_font(ctx, &droid->handle);*/}
}

void gui_system_render(window win){
    nk_glfw3_new_frame(&state.glfw);
    if(nk_begin(state.ctx, "demo title", nk_rect(50, 50, 512, 512), 
    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE
    )){
        enum {EASY, HARD};
        static int op = EASY;
        static int property = 20;
        nk_layout_row_static(state.ctx, 30, 80, 1);
        if (nk_button_label(state.ctx, "button"))
            fprintf(stdout, "button pressed\n");

        nk_layout_row_dynamic(state.ctx, 30, 2);
        if (nk_option_label(state.ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(state.ctx, "hard", op == HARD)) op = HARD;

        nk_layout_row_dynamic(state.ctx, 25, 1);
        nk_property_int(state.ctx, "Compression:", 0, &property, 100, 10, 1);

        nk_layout_row_dynamic(state.ctx, 20, 1);
        nk_label(state.ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(state.ctx, 25, 1);
        if (nk_combo_begin_color(state.ctx, nk_rgb_cf(state.bg), nk_vec2(nk_widget_width(state.ctx),400))) {
            nk_layout_row_dynamic(state.ctx, 120, 1);
            state.bg = nk_color_picker(state.ctx, state.bg, NK_RGBA);
            nk_layout_row_dynamic(state.ctx, 25, 1);
            state.bg.r = nk_propertyf(state.ctx, "#R:", 0, state.bg.r, 1.0f, 0.01f,0.005f);
            state.bg.g = nk_propertyf(state.ctx, "#G:", 0, state.bg.g, 1.0f, 0.01f,0.005f);
            state.bg.b = nk_propertyf(state.ctx, "#B:", 0, state.bg.b, 1.0f, 0.01f,0.005f);
            state.bg.a = nk_propertyf(state.ctx, "#A:", 0, state.bg.a, 1.0f, 0.01f,0.005f);
            nk_combo_end(state.ctx);
        }        
    }
    nk_end(state.ctx);

    nk_glfw3_render(&state.glfw, NK_ANTI_ALIASING_ON, state.max_vertex_buffer, state.max_vertex_buffer);
}

void gui_system_destroy(void){
    nk_glfw3_shutdown(&state.glfw);
}