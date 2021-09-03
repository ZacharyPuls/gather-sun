
// TODO: figure out a better way to enforce glad inclusion before GLFW
#include "graphics.h"
#include "window.h"
#include "scene.h"
#include "renderer.h"
#include "event.h"
#include "ui.h"
#include "state.h"

uint64_t player_texture_id = 0UL;
uint64_t player_sprite_id = 0UL;
uint64_t player_entity_id = 0UL;

uint64_t background_texture_id = 0UL;
uint64_t background_sprite_id = 0UL;

void translate_model_matrix(float x, float y, float z) {
    vec3s translation = glms_vec3_zero();
    translation.x = x;
    translation.y = y;
    translation.z = z;
    model_matrix = glms_translate(model_matrix, translation);
}

void update_frame(double delta_time) {
    // TODO: maybe break the update function out into a separate module?
    if (!game_state.tick_active) {
        return;
    }
    // TODO: maybe break out user input into a 'controller' module?
    const float speed = 0.01f;
    if (app_window.keys[GLFW_KEY_W]) {
        scene_entity_set_direction(game_state.scene, player_entity_id, UP);
        scene_entity_update_position(game_state.scene, player_entity_id, delta_time);
        scene_entity_set_moving(game_state.scene, player_entity_id, true);
    }

    if (app_window.keys[GLFW_KEY_A]) {
        scene_entity_set_direction(game_state.scene, player_entity_id, LEFT);
        scene_entity_update_position(game_state.scene, player_entity_id, delta_time);
        scene_entity_set_moving(game_state.scene, player_entity_id, true);
    }

    if (app_window.keys[GLFW_KEY_S]) {
        scene_entity_set_direction(game_state.scene, player_entity_id, DOWN);
        scene_entity_update_position(game_state.scene, player_entity_id, delta_time);
        scene_entity_set_moving(game_state.scene, player_entity_id, true);
    }

    if (app_window.keys[GLFW_KEY_D]) {
        scene_entity_set_direction(game_state.scene, player_entity_id, RIGHT);
        scene_entity_update_position(game_state.scene, player_entity_id, delta_time);
        scene_entity_set_moving(game_state.scene, player_entity_id, true);
    }

    if (!app_window.keys[GLFW_KEY_W] && !app_window.keys[GLFW_KEY_A] && !app_window.keys[GLFW_KEY_S] && !app_window.keys[GLFW_KEY_D]) {
        scene_entity_set_moving(game_state.scene, player_entity_id, false);
    }
}

void render() {
    renderer_render_frame(game_state.scene, game_state.renderer, game_state.ui);
}

void mouse_input(int button, int action, int mods) {
    float x = (float)app_window.mouse[0] / (float)app_window.width * 2.0f - 1.0f;
    float y = ((float)app_window.height - (float)app_window.mouse[1]) / (float)app_window.height * 2.0f - 1.0f;
    bool left_mouse_pressed = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
    bool right_mouse_pressed = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS;
    state_handle_mouse_input(x, y, left_mouse_pressed, right_mouse_pressed);
}

void keyboard_input(int key, int scancode, int action, int mods) {
    state_handle_keyboard_input(key, scancode, action, mods);
}

int main() {
    setup_window("Minecraft", 1920, 1080, true, initialize_graphics, update_frame, render, resize_viewport, mouse_input, keyboard_input);

    game_state.scene = (scene_t*)malloc(sizeof(scene_t));
    scene_init(game_state.scene);

    // temp background texture - 20x wide as player, 6x as tall as player (player is 1 tile wide, 2 tiles tall)
    texture_t background_texture = create_texture_2d("res/img/temp-background.png", false, true);
    background_texture_id = scene_add_texture(game_state.scene, background_texture);

    sprite_t background_sprite = {
            .texture_id = background_texture_id,
            .position = {0.0f, 0.0f, 0.0f},
            .size = {10.0f, 5.0f},
            .keyframe = 0,
            .num_keyframes = 1,
            .cycle = 0,
            .num_cycles = 1,
            .animation_speed = 0.0
    };
    background_sprite_id = scene_add_sprite(game_state.scene, background_sprite);

    texture_t player_texture = create_texture_2d("res/img/Player.png", false, true);
    player_texture_id = scene_add_texture(game_state.scene, player_texture);

    sprite_t player_sprite = {
            .texture_id = player_texture_id,
            .position = {0.0f, 0.0f, 0.0f},
            .size = {0.5f, 1.0f},
            .keyframe = 0,
            .num_keyframes = 12,
            .cycle = 0,
            .num_cycles = 4,
            .animation_speed = 0.2
    };
    player_sprite_id = scene_add_sprite(game_state.scene, player_sprite);

    entity_t player_entity = {
            .sprite_id = player_sprite_id,
            .moving = false,
            .direction = UP,
            .speed = 0.15f
    };
    player_entity_id = scene_add_entity(game_state.scene, player_entity);

    camera_t player_camera = {
            .position = player_sprite.position,
            .following_entity_id = player_entity_id
    };
    uint64_t player_camera_id = scene_add_camera(game_state.scene, player_camera);
    scene_set_active_camera(game_state.scene, player_camera_id);

    state_init(game_state.scene);

    main_loop();

    scene_deinit(game_state.scene);
    free(game_state.scene);
    game_state.scene = NULL;

    state_deinit();

    return 0;
}
