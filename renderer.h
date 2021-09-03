//
// Created by zach on 2021-05-16.
//

#ifndef MINECRAFT_RENDERER_H
#define MINECRAFT_RENDERER_H

#include "graphics.h"
#include "scene.h"
#include "font.h"
#include "ui.h"

typedef struct tag_renderer {
    uint64_t default_shader_id;
    uint64_t text_shader_id;

    // TODO: probably want to have a pool of VBOs/VAOs that get recycled each frame
    uint64_t sprite_vbo_id;
    uint64_t sprite_vao_id;
    uint64_t ui_vbo_id;
    uint64_t ui_vao_id;
} renderer_t;

void renderer_init(renderer_t* renderer, scene_t* scene);
void renderer_render_frame(scene_t* scene, renderer_t* renderer, ui_t* ui);
void renderer_deinit(renderer_t* renderer);

#endif //MINECRAFT_RENDERER_H
