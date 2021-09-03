//
// Created by zach on 2021-05-16.
//

#include <stb_ds.h>
#include "renderer.h"
#include "font.h"
#include "ui.h"
#include "window.h"

// TODO: wrap this in NDEBUG ifndef
void GLAPIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                      const GLchar *message, const void *user_param) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

void renderer_init(renderer_t* renderer, scene_t* scene) {
    shader_t default_shader = create_shader("res/shader/default/default.vs", "res/shader/default/default.fs");
    renderer->default_shader_id = scene_add_shader(scene, default_shader);

    shader_t font_shader = create_shader("res/shader/text/text.vs", "res/shader/text/text.fs");
    renderer->text_shader_id = scene_add_shader(scene, font_shader);

    vbo_t sprite_vbo = create_vbo(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
    renderer->sprite_vbo_id = scene_add_vbo(scene, sprite_vbo);

    vao_t sprite_vao = create_vao();
    renderer->sprite_vao_id = scene_add_vao(scene, sprite_vao);

    bind_vao(sprite_vao);
    bind_vbo(sprite_vbo);
    vbo_set_vertex_attrib(sprite_vbo, 0, 3, GL_FLOAT, sizeof(float) * 5, 0);
    vbo_set_vertex_attrib(sprite_vbo, 1, 2, GL_FLOAT, sizeof(float) * 5, sizeof(float) * 3);

//    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(opengl_debug_callback, NULL);

    vbo_t ui_vbo = create_vbo(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
    renderer->ui_vbo_id = scene_add_vbo(scene, ui_vbo);

    vao_t ui_vao = create_vao();
    renderer->ui_vao_id = scene_add_vao(scene, ui_vao);

    bind_vao(ui_vao);
    bind_vbo(ui_vbo);
    vbo_set_vertex_attrib(ui_vbo, 0, 3, GL_FLOAT, sizeof(float) * 9, 0);
    vbo_set_vertex_attrib(ui_vbo, 1, 3, GL_FLOAT, sizeof(float) * 9, sizeof(float) * 3);
    vbo_set_vertex_attrib(ui_vbo, 2, 3, GL_FLOAT, sizeof(float) * 9, sizeof(float) * 6);
}

void regenerate_vertices(scene_t* scene, renderer_t* renderer, ui_t* ui) {
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
    vbo_t sprite_vbo = scene_get_vbo(scene, renderer->sprite_vbo_id);
    bind_vbo(sprite_vbo);
    fill_vbo(sprite_vbo, sizeof(float) * num_sprites * 30, buffer);
    free(buffer);
    
    // TODO: remove this, temporary test

    size_t ui_vbo_size = sizeof(float) * 9 * ui_get_num_vertices(ui);
    buffer = (float*)malloc(ui_vbo_size);
//    printf("malloc %lli bytes for ui_vbo\n", ui_vbo_size);
    // TODO: definitely don't break separation of concerns by using app_window like this, outside this test
    ui_generate_vertices(ui, buffer, app_window.width, app_window.height);

    vbo_t ui_vbo = scene_get_vbo(scene, renderer->ui_vbo_id);
    bind_vbo(ui_vbo);
    fill_vbo(ui_vbo, ui_vbo_size, buffer);
    free(buffer);
}

void renderer_render_frame(scene_t* scene, renderer_t* renderer, ui_t* ui) {
    regenerate_vertices(scene, renderer, ui);
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO: error handling for no active camera?
    camera_t active_camera = scene_get_active_camera(scene);
    vec3s translation = {-active_camera.position.x, -active_camera.position.y, -active_camera.position.z};
    model_matrix = glms_translate(glms_mat4_identity(), translation);

    bind_shader(scene_get_shader(scene, renderer->default_shader_id));
    shader_uniform_mat4(0, get_model_projection_matrix());

    vao_t sprite_vao = scene_get_vao(scene, renderer->sprite_vao_id);
    bind_vao(sprite_vao);

    for (uint64_t i = 0; i < hmlenu(scene->sprite_map); i++) {
        sprite_t sprite = scene_get_sprite(scene, i);
        shader_uniform_texture(1, scene_get_texture(scene, sprite.texture_id));

        // TODO: bounds checking (UINT32_MAX) and narrowing cast to GLsizei
        draw_vao(sprite_vao, GL_TRIANGLES, 6 * (int32_t)i, 6);
    }

    bind_shader(scene_get_shader(scene, renderer->text_shader_id));
    model_matrix = glms_mat4_identity();
    shader_uniform_mat4(0, get_model_projection_matrix());
    vao_t ui_vao = scene_get_vao(scene, renderer->ui_vao_id);
    bind_vao(ui_vao);
    // TODO: this is not going to work if there are multiple fonts for different menus. \
    //  Consider setting this in a UBO.
    shader_uniform_texture(1, scene_get_texture(scene, ui_get_font(ui, ui_get_menu(ui, 0).font_id).texture_id));
    draw_vao(ui_vao, GL_TRIANGLES, 0, ui_get_num_vertices(ui));
}

void renderer_deinit(renderer_t* renderer) {

}
