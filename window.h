//
// Created by zach on 2021-04-24.
//

#ifndef MINECRAFT_WINDOW_H
#define MINECRAFT_WINDOW_H

#include <stdbool.h>
#include <GLFW/glfw3.h>

typedef void (*PFNUPDATEFRAMEPROC)(double);
typedef void (*PFNRENDERFRAMEPROC)();
typedef void (*PFNINITGRAPHICSAPIPROC)(GLFWglproc (*)(const char*), float, float);
typedef void (*PFNRESIZEVIEWPORTPROC)(int, int, int, int);

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
} window_t;

extern window_t app_window;

extern bool glfw_is_initialized;

void setup_window(const char* title, int width, int height, PFNINITGRAPHICSAPIPROC init_graphics_api, PFNUPDATEFRAMEPROC update_frame, PFNRENDERFRAMEPROC render_frame, PFNRESIZEVIEWPORTPROC resize_viewport);
void main_loop();
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void resize_callback(GLFWwindow* window, int width, int height);

#endif //MINECRAFT_WINDOW_H
