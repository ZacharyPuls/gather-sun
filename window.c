//
// Created by zach on 2021-04-24.
//

#include "window.h"

#include <stdio.h>

window_t app_window;
bool glfw_is_initialized = false;

void glfw_error_callback(int error, const char* description) {
    printf("GLFW error with ID %i encountered: %s\n", error, description);
}

void setup_window(const char *title, const int width, const int height, bool fullscreen,
                  PFNINITGRAPHICSAPIPROC init_graphics_api, PFNUPDATEFRAMEPROC update_frame,
                  PFNRENDERFRAMEPROC render_frame, PFNRESIZEVIEWPORTPROC resize_viewport,
                  PFNHANDLEMOUSEINPUTPROC handle_mouse_input, PFNHANDLEKEYBOARDINPUTPROC handle_keyboard_input) {
    printf("Setting up window...\n");
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfw_is_initialized) {
        if (!glfwInit()) {
            printf("Could not initialize GLFW.\n");
            return;
        }
        glfw_is_initialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    app_window = (window_t) {0};
    app_window.width = width;
    app_window.height = height;
    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : NULL;
    app_window.handle = glfwCreateWindow(width, height, title, monitor, NULL);
    app_window.update_frame = update_frame;
    app_window.render_frame = render_frame;
    app_window.handle_mouse_input = handle_mouse_input;
    app_window.handle_keyboard_input = handle_keyboard_input;

    glfwSetKeyCallback(app_window.handle, keyboard_callback);
    glfwSetCursorPosCallback(app_window.handle, cursor_callback);
    glfwSetMouseButtonCallback(app_window.handle, mouse_button_callback);
    glfwSetWindowSizeCallback(app_window.handle, resize_callback);

    glfwMakeContextCurrent(app_window.handle);
    glfwSwapInterval(1);

    printf("Initializing graphics API...\n");
    init_graphics_api(glfwGetProcAddress, width, height);

    app_window.resize_viewport = resize_viewport;
    app_window.running = true;
}

void main_loop() {
    while (!glfwWindowShouldClose(app_window.handle) && app_window.running) {
        static double last_time = 0.0;
        double delta_time = glfwGetTime() - last_time;
        last_time = glfwGetTime();

        app_window.update_frame(delta_time);

        const int vw = 1920;
        const int vh = 1080;
        float scale_x = (float)app_window.width / (float)vw;
        float scale_y = (float)app_window.height / (float)vh;

        app_window.render_frame(scale_x, scale_y);

        glfwSwapBuffers(app_window.handle);
        glfwPollEvents();
    }

    glfwDestroyWindow(app_window.handle);
    glfwTerminate();
}

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    app_window.keys[key] = action == GLFW_PRESS || action == GLFW_REPEAT;
    app_window.handle_keyboard_input(key, scancode, action, mods);
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    app_window.mouse[0] = xpos;
    app_window.mouse[1] = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    app_window.handle_mouse_input(button, action, mods);
}

void resize_callback(GLFWwindow *window, int width, int height) {
    app_window.resize_viewport(0, 0, width, height);
    app_window.width = width;
    app_window.height = height;
}
