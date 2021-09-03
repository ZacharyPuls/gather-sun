//
// Created by zach on 2021-06-19.
//

#ifndef GATHER_SUN_STATE_H
#define GATHER_SUN_STATE_H

#include "scene.h"
#include "renderer.h"

typedef struct tag_state {
    scene_t* scene;
    renderer_t* renderer;
    ui_t* ui;
    // TODO: do I want to include window_t (app_window) here?
    
    uint64_t main_menu_id;
    uint64_t pause_menu_id;
    uint64_t options_menu_id;

    // TODO: implement this as a commandbuffer, like an undo stack
    uint64_t prev_active_menu_id;

    bool tick_active;
    bool render_scene;
} state_t;

extern state_t game_state;

// TODO: do I want to fully manage scene from within state?
void state_init(scene_t* scene);

// State change events
void state_handle_mouse_input(float x, float y, bool left_button_pressed, bool right_button_pressed);
void state_handle_keyboard_input(int key, int scancode, int action, int mods);
void state_handle_pause_menu_resume_button(void* menu_ptr);
void state_handle_pause_menu_new_game_button(void* menu_ptr);
void state_handle_pause_menu_load_button(void* menu_ptr);
void state_handle_pause_menu_options_button(void* menu_ptr);
void state_handle_pause_menu_exit_button(void* menu_ptr);

void state_deinit();

#endif //GATHER_SUN_STATE_H
