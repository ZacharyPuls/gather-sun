//
// Created by zach on 2021-04-24.
//

#ifndef GATHER_SUN_WINDOW_H
#define GATHER_SUN_WINDOW_H

#include <stdbool.h>
#include <GLFW/glfw3.h>

typedef void (*PFNUPDATEFRAMEPROC)(double);
typedef void (*PFNRENDERFRAMEPROC)();
typedef void (*PFNINITGRAPHICSAPIPROC)(GLFWglproc (*)(const char*), int, int);
typedef void (*PFNRESIZEVIEWPORTPROC)(int, int, int, int);
typedef void (*PFNHANDLEMOUSEINPUTPROC)(int, int, int); // TODO: possibly abstract out the GLFW enums?
typedef void (*PFNHANDLEKEYBOARDINPUTPROC)(int, int, int, int); // TODO: possibly abstract out the GLFW enums?

typedef struct tag_window {
    int width;
    int height;
    bool keys[GLFW_KEY_LAST];
    double mouse[2];
    bool running;
    GLFWwindow* handle;
    PFNUPDATEFRAMEPROC update_frame;
    PFNRENDERFRAMEPROC render_frame;
    PFNRESIZEVIEWPORTPROC resize_viewport;
    PFNHANDLEMOUSEINPUTPROC handle_mouse_input;
    PFNHANDLEKEYBOARDINPUTPROC handle_keyboard_input;
} window_t;

extern window_t app_window;

extern bool glfw_is_initialized;

void setup_window(const char *title, int width, int height, bool fullscreen, PFNINITGRAPHICSAPIPROC init_graphics_api,
                  PFNUPDATEFRAMEPROC update_frame, PFNRENDERFRAMEPROC render_frame,
                  PFNRESIZEVIEWPORTPROC resize_viewport, PFNHANDLEMOUSEINPUTPROC handle_mouse_input,
                  PFNHANDLEKEYBOARDINPUTPROC handle_keyboard_input);
void main_loop();
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void resize_callback(GLFWwindow *window, int width, int height);

#endif //GATHER_SUN_WINDOW_H
