
// TODO: figure out a better way to enforce glad inclusion before GLFW
#include "graphics.h"
#include "window.h"
#include "scene.h"
#include "renderer.h"

scene_t* scene;

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
    const float speed = 0.01f;
    if (app_window.keys[GLFW_KEY_W]) {
        set_entity_direction(scene, player_entity_id, UP);
        update_entity_position(scene, player_entity_id, delta_time);
        set_entity_moving(scene, player_entity_id, true);
    }

    if (app_window.keys[GLFW_KEY_A]) {
        set_entity_direction(scene, player_entity_id, LEFT);
        update_entity_position(scene, player_entity_id, delta_time);
        set_entity_moving(scene, player_entity_id, true);
    }

    if (app_window.keys[GLFW_KEY_S]) {
        set_entity_direction(scene, player_entity_id, DOWN);
        update_entity_position(scene, player_entity_id, delta_time);
        set_entity_moving(scene, player_entity_id, true);
    }

    if (app_window.keys[GLFW_KEY_D]) {
        set_entity_direction(scene, player_entity_id, RIGHT);
        update_entity_position(scene, player_entity_id, delta_time);
        set_entity_moving(scene, player_entity_id, true);
    }

    if (!app_window.keys[GLFW_KEY_W] && !app_window.keys[GLFW_KEY_A] && !app_window.keys[GLFW_KEY_S] && !app_window.keys[GLFW_KEY_D]) {
        set_entity_moving(scene, player_entity_id, false);
    }
}

void render() {
    render_frame(scene, player_texture_id);
}

int main() {
    setup_window("Minecraft", 640, 480, initialize_graphics, update_frame, render, resize_viewport);

    scene = (scene_t*)malloc(sizeof(scene_t));
    begin_scene(scene);

    // temp background texture - 20x wide as player, 6x as tall as player (player is 1 tile wide, 2 tiles tall)
    texture_t background_texture = create_texture("res/img/temp-background.png");
    background_texture_id = add_texture(scene, background_texture);

    sprite_t background_sprite = {
            .texture_id = background_texture_id,
            .position = {-5.0f, -3.0f, 0.0f},
            .size = {10.0f, 6.0f},
            .keyframe = 0,
            .num_keyframes = 1,
            .cycle = 0,
            .num_cycles = 1,
            .animation_speed = 0.0
    };
    background_sprite_id = add_sprite(scene, background_sprite);

    texture_t player_texture = create_texture("res/img/Player.png");
    player_texture_id = add_texture(scene, player_texture);

    sprite_t player_sprite = {
            .texture_id = player_texture_id,
            .position = {-0.25f, -0.5f, 0.0f},
            .size = {0.5f, 1.0f},
            .keyframe = 0,
            .num_keyframes = 12,
            .cycle = 0,
            .num_cycles = 4,
            .animation_speed = 0.2
    };
    player_sprite_id = add_sprite(scene, player_sprite);

    entity_t player_entity = {
            .sprite_id = player_sprite_id,
            .moving = false,
            .direction = UP,
            .speed = 0.5f
    };
    player_entity_id = add_entity(scene, player_entity);

    // TODO: for some reason, calling create_font() in startup_renderer was causing the model matrix to not be updated \
    //  for the player sprite. Moving startup_renderer() down here to (hopefully) fix this until I implement a proper camera.
    startup_renderer(scene);


//    shader = create_shader("res/shader/default/default.vs", "res/shader/default/default.fs");
//
//    cobble = create_texture("res/img/cobble.png");
//
//    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
//                        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//                        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//                        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//                        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
//                        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};
//
//    vbo_t vbo = create_vbo(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
//    fill_vbo(vbo, sizeof(float) * (6 * 5), vertices);
//
//    vao = create_vao();
//    bind_vao(vao);
//    bind_vbo(vbo);
//    vbo_set_vertex_attrib(vbo, 0, 3, GL_FLOAT, sizeof(float) * 5, 0);
//    vbo_set_vertex_attrib(vbo, 1, 2, GL_FLOAT, sizeof(float) * 5, sizeof(float) * 3);

    main_loop();

    shutdown_renderer();

    end_scene(scene);
    free(scene);
    scene = NULL;

    return 0;
}
