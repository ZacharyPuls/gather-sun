//
// Created by zach on 2021-05-16.
//

#include <stb_ds.h>
#include "renderer.h"
#include "font.h"

uint64_t default_shader_id = 0UL;

// TODO: probably want to have a pool of VBOs/VAOs that get recycled each frame
uint64_t sprite_vbo_id = 0UL;
uint64_t sprite_vao_id = 0UL;
uint64_t ui_vbo_id = 0UL;
uint64_t ui_vao_id = 0UL;

font_t default_font;

void startup_renderer(scene_t* scene) {

    shader_t default_shader = create_shader("res/shader/default/default.vs", "res/shader/default/default.fs");
    default_shader_id = add_shader(scene, default_shader);

    vbo_t sprite_vbo = create_vbo(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
    sprite_vbo_id = add_vbo(scene, sprite_vbo);

    vao_t sprite_vao = create_vao();
    sprite_vao_id = add_vao(scene, sprite_vao);

    bind_vao(sprite_vao);
    bind_vbo(sprite_vbo);
    vbo_set_vertex_attrib(sprite_vbo, 0, 3, GL_FLOAT, sizeof(float) * 5, 0);
    vbo_set_vertex_attrib(sprite_vbo, 1, 2, GL_FLOAT, sizeof(float) * 5, sizeof(float) * 3);

//    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    default_font = load_font(scene, "res/font/fira-code-medium.fnt", "res/font/fira-code-medium_0.png");

    vbo_t ui_vbo = create_vbo(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
    ui_vbo_id = add_vbo(scene, ui_vbo);

    vao_t ui_vao = create_vao();
    ui_vao_id = add_vao(scene, ui_vao);

    bind_vao(ui_vao);
    bind_vbo(ui_vbo);
    vbo_set_vertex_attrib(ui_vbo, 0, 3, GL_FLOAT, sizeof(float) * 5, 0);
    vbo_set_vertex_attrib(ui_vbo, 1, 2, GL_FLOAT, sizeof(float) * 5, sizeof(float) * 3);
}

void regenerate_vertices(scene_t* scene) {
    size_t num_sprites = hmlenu(scene->sprite_map);
    float* buffer = (float*)malloc(sizeof(float) * num_sprites * 30);
    for (size_t i = 0; i < num_sprites; i++) {
        sprite_t sprite = hmget(scene->sprite_map, i);
        float x0 = sprite.position.x - sprite.size.x / 2.0f;
        float x1 = sprite.position.x + sprite.size.x / 2.0f;
        float y0 = sprite.position.y - sprite.size.y / 2.0f;
        float y1 = sprite.position.y + sprite.size.y / 2.0f;
        float z = sprite.position.z;
        vec4f_t bounds = get_sprite_texcoord_bounds(sprite);
        float s0 = bounds.x;
        float t0 = bounds.y;
        float s1 = bounds.z;
        float t1 = bounds.w;
        buffer[i * 30] = x0; buffer[i * 30 + 1] = y0; buffer[i * 30 + 2] = z; buffer[i * 30 + 3] = s0; buffer[i * 30 + 4] = t0;
        buffer[i * 30 + 5] = x1; buffer[i * 30 + 6] = y0; buffer[i * 30 + 7] = z; buffer[i * 30 + 8] = s1; buffer[i * 30 + 9] = t0;
        buffer[i * 30 + 10] = x0; buffer[i * 30 + 11] = y1; buffer[i * 30 + 12] = z; buffer[i * 30 + 13] = s0; buffer[i * 30 + 14] = t1;
        buffer[i * 30 + 15] = x1; buffer[i * 30 + 16] = y0; buffer[i * 30 + 17] = z; buffer[i * 30 + 18] = s1; buffer[i * 30 + 19] = t0;
        buffer[i * 30 + 20] = x1; buffer[i * 30 + 21] = y1; buffer[i * 30 + 22] = z; buffer[i * 30 + 23] = s1; buffer[i * 30 + 24] = t1;
        buffer[i * 30 + 25] = x0; buffer[i * 30 + 26] = y1; buffer[i * 30 + 27] = z; buffer[i * 30 + 28] = s0; buffer[i * 30 + 29] = t1;
    }
    vbo_t sprite_vbo = get_vbo(scene, sprite_vbo_id);
    bind_vbo(sprite_vbo);
    fill_vbo(sprite_vbo, sizeof(float) * num_sprites * 30, buffer);
    free(buffer);
}

void render_frame(scene_t* scene, /* TODO: Remove this parameter */ uint64_t player_sprite_id) {
    regenerate_vertices(scene);
    glClear(GL_COLOR_BUFFER_BIT);
    bind_shader(get_shader(scene, default_shader_id));

    // TODO: remove this, and put this logic in a camera struct/methods
    sprite_t player_sprite = get_sprite(scene, player_sprite_id);

    shader_uniform_mat4(0, get_model_projection_matrix());

    vao_t sprite_vao = get_vao(scene, sprite_vao_id);
    bind_vao(sprite_vao);

    for (uint64_t i = 0; i < hmlenu(scene->sprite_map); i++) {
        if (i == player_sprite_id) {
            vec3s translation = glms_vec3_zero();
            translation.x = -player_sprite.position.x;
            translation.y = -player_sprite.position.y;
            translation.z = -player_sprite.position.z;
            model_matrix = glms_translate(glms_mat4_identity(), translation);
        } else {
            model_matrix = glms_mat4_identity();
        }
        sprite_t sprite = get_sprite(scene, i);
        shader_uniform_texture(1, get_texture(scene, sprite.texture_id));

        // TODO: bounds checking (UINT32_MAX) and narrowing cast to GLsizei
        draw_vao(sprite_vao, GL_TRIANGLES, 6 * (int32_t)i, 6);
    }
}

void shutdown_renderer() {

}
