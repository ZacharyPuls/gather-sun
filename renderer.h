//
// Created by zach on 2021-05-16.
//

#ifndef MINECRAFT_RENDERER_H
#define MINECRAFT_RENDERER_H

#include "graphics.h"
#include "scene.h"

void startup_renderer(scene_t* scene);
void render_frame(scene_t* scene, /* TODO: Remove this parameter */ uint64_t player_sprite_id);
void shutdown_renderer();

#endif //MINECRAFT_RENDERER_H
