//
// Created by zach on 2021-06-19.
//

#include <stb_ds.h>
#include "state.h"
#include "event.h"
#include "window.h"

state_t game_state;

void state_init(scene_t *scene) {
    game_state.scene = scene;

    game_state.renderer = malloc(sizeof(renderer_t));
    renderer_init(game_state.renderer, game_state.scene);

    game_state.ui = malloc(sizeof(ui_t));
    ui_init(game_state.ui, game_state.scene);

    const char* default_font_image_filenames[3] = {"res/font/fira-code-medium_0.png", "res/img/ui-active.png", "res/img/ui-inactive.png"};
    font_t default_font = font_load_bmfont(scene, "res/font/fira-code-medium.fnt", default_font_image_filenames, 3);
    uint64_t default_font_id = ui_add_font(game_state.ui, default_font);

    menu_t pause_menu = {0};
    pause_menu.button_map = NULL;
    pause_menu.active_button_id = UINT64_MAX;
    pause_menu.font_id = default_font_id;

    textbox_t resume = {
            .base.texture_id = default_font.texture_id,
            .base.position = {-0.25f, 0.375f},
            .base.size = {0.5f, 0.15f},
            .base.texcoord = {0.0f, 0.0f, 1.0f},
            .base.texcoord_size = {0.5f, 0.25f},
            .text = "Resume",
            .padding = {140, 24},
            .scale = 1.0f
    };
    textbox_t new_game = {
            .base.texture_id = default_font.texture_id,
            .base.position = {-0.25f, 0.225f},
            .base.size = {0.5f, 0.15f},
            .base.texcoord = {0.0f, 0.0f, 1.0f},
            .base.texcoord_size = {0.5f, 0.25f},
            .text = "New Game",
            .padding = {115, 24},
            .scale = 1.0f
    };
    textbox_t load = {
            .base.texture_id = default_font.texture_id,
            .base.position = {-0.25f, 0.075f},
            .base.size = {0.5f, 0.15f},
            .base.texcoord = {0.0f, 0.0f, 1.0f},
            .base.texcoord_size = {0.5f, 0.25f},
            .text = "Load",
            .padding = {170, 24},
            .scale = 1.0f
    };
    textbox_t options = {
            .base.texture_id = default_font.texture_id,
            .base.position = {-0.25f, -0.075f},
            .base.size = {0.5f, 0.15f},
            .base.texcoord = {0.0f, 0.0f, 1.0f},
            .base.texcoord_size = {0.5f, 0.25f},
            .text = "Options",
            .padding = {125, 24},
            .scale = 1.0f
    };
    textbox_t exit = {
            .base.texture_id = default_font.texture_id,
            .base.position = {-0.25f, -0.225f},
            .base.size = {0.5f, 0.15f},
            .base.texcoord = {0.0f, 0.0f, 1.0f},
            .base.texcoord_size = {0.5f, 0.25f},
            .text = "Exit",
            .padding = {170, 24},
            .scale = 1.0f
    };
    button_t resume_button = {.textbox = resume, .state = false, .callback = state_handle_pause_menu_resume_button};
    button_t new_game_button = {.textbox = new_game, .state = false, .callback = state_handle_pause_menu_new_game_button};
    button_t load_button = {.textbox = load, .state = false, .callback = state_handle_pause_menu_load_button};
    button_t options_button = {.textbox = options, .state = false, .callback = state_handle_pause_menu_options_button};
    button_t exit_button = {.textbox = exit, .state = false, .callback = state_handle_pause_menu_exit_button};

    menu_add_button(&pause_menu, resume_button);
    menu_add_button(&pause_menu, new_game_button);
    menu_add_button(&pause_menu, load_button);
    menu_add_button(&pause_menu, options_button);
    menu_add_button(&pause_menu, exit_button);

    game_state.pause_menu_id = ui_add_menu(game_state.ui, pause_menu);

    menu_t options_menu = {0};
    options_menu.button_map = NULL;
    options_menu.active_button_id = UINT64_MAX;
    options_menu.font_id = default_font_id;

    textbox_t resolution = {
            .base.texture_id = default_font.texture_id,
            .base.position = {-0.75f, 0.675f},
            .base.size = {0.5f, 0.15f},
            .base.texcoord = {0.0f, 0.0f, 1.0f},
            .base.texcoord_size = {0.5f, 0.25f},
            .text = "Resolution: ",
            .padding = {0, 24},
            .scale = 1.0f
    };
    menu_add_textbox(&options_menu, resolution);

    game_state.options_menu_id = ui_add_menu(game_state.ui, options_menu);

    game_state.prev_active_menu_id = UINT64_MAX;
    game_state.tick_active = true;
    game_state.render_scene = false;
}

// TODO: handle more than just left/right mouse button input
void state_handle_mouse_input(float x, float y, bool left_button_pressed, bool right_button_pressed) {
    for (uint64_t i = 0; i < ui_get_num_menus(game_state.ui); ++i) {
        menu_t menu = ui_get_menu(game_state.ui, i);
        if (!menu.active) {
            continue;
        }

        if (!left_button_pressed && menu.active_button_id != UINT64_MAX) {
            button_t active_button = menu_get_button(&menu, menu.active_button_id);
            active_button.state = false;
            menu_update_button(&menu, menu.active_button_id, active_button);
            menu.active_button_id = UINT64_MAX;
            active_button.callback(&menu);
            ui_update_menu(game_state.ui, i, menu);
        } else if (left_button_pressed) {
            uint64_t button_id = menu_find_button(&menu, x, y);
            if (button_id != UINT64_MAX) {
                button_t button = menu_get_button(&menu, button_id);
                button.state = true;
                menu_update_button(&menu, button_id, button);
                menu.active_button_id = button_id;
                ui_update_menu(game_state.ui, i, menu);
            }
        }
    }
}

void state_handle_keyboard_input(int key, int scancode, int action, int mods) {
    // TODO: do I want to pass this to a menu keyboard callback? \
    //  menu_handle_keyboard_input(&pause_menu, key, scancode, action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        uint64_t menu_to_modify = ui_get_active_menu(game_state.ui);
        menu_to_modify = menu_to_modify == UINT64_MAX ? game_state.pause_menu_id : menu_to_modify;

        menu_t menu = ui_get_menu(game_state.ui, menu_to_modify);
        menu.active = !menu.active;
        if (!menu.active) {
            menu.visible = false;
        } else {
            menu.visible = true;
        }
        ui_update_menu(game_state.ui, menu_to_modify, menu);

        if (game_state.prev_active_menu_id != UINT64_MAX) {
            menu_t prev_active_menu = ui_get_menu(game_state.ui, game_state.prev_active_menu_id);
            prev_active_menu.active = true;
            ui_update_menu(game_state.ui, game_state.prev_active_menu_id, prev_active_menu);
            game_state.prev_active_menu_id = UINT64_MAX;
        } else {
            game_state.prev_active_menu_id = menu_to_modify;
        }

        if (menu.active) {
            game_state.tick_active = false;
        }
    }
}

void state_handle_pause_menu_resume_button(void* menu_ptr) {
    menu_t* menu = (menu_t*)menu_ptr;
    menu->active = false;
    menu->visible = false;
    game_state.tick_active = true;
}

void state_handle_pause_menu_new_game_button(void* menu_ptr) {

}

void state_handle_pause_menu_load_button(void* menu_ptr) {

}

void state_handle_pause_menu_options_button(void* menu_ptr) {
    menu_t options_menu = ui_get_menu(game_state.ui, game_state.options_menu_id);
    options_menu.active = true;
    options_menu.visible = true;
    ui_update_menu(game_state.ui, game_state.options_menu_id, options_menu);

    menu_t pause_menu = ui_get_menu(game_state.ui, game_state.pause_menu_id);
    pause_menu.active = false;
    ui_update_menu(game_state.ui, game_state.pause_menu_id, pause_menu);

    game_state.prev_active_menu_id = game_state.pause_menu_id;
}

void state_handle_pause_menu_exit_button(void* menu_ptr) {
    menu_t* menu = (menu_t*)menu_ptr;
    menu->active = false;
    menu->visible = false;
    app_window.running = false;
}

void state_deinit() {
    renderer_deinit(game_state.renderer);
    free(game_state.renderer);
    game_state.renderer = NULL;

    ui_deinit(game_state.ui);
    free(game_state.ui);
    game_state.ui = NULL;
}
