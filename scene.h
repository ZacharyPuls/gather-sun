//
// Created by zach on 2021-05-16.
//

#ifndef GATHER_SUN_SCENE_H
#define GATHER_SUN_SCENE_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"
#include "camera.h"

typedef struct tag_scene_texture {
    uint64_t key;
    texture_t value;
} scene_texture_t;

typedef struct tag_scene_vbo {
    uint64_t key;
    vbo_t value;
} scene_vbo_t;

typedef struct tag_scene_vao {
    uint64_t key;
    vao_t value;
} scene_vao_t;

typedef struct tag_scene_shader {
    uint64_t key;
    shader_t value;
} scene_shader_t;

typedef struct tag_scene_sprite {
    uint64_t key;
    sprite_t value;
} scene_sprite_t;

typedef struct tag_scene_entity {
    uint64_t key;
    entity_t value;
} scene_entity_t;

typedef struct tag_scene_camera {
    uint64_t key;
    camera_t value;
} scene_camera_t;

typedef struct tag_scene {
    // Hash maps containing scene objects
    scene_texture_t* texture_map;
    uint64_t texture_map_current_id;

    scene_vbo_t* vbo_map;
    uint64_t vbo_map_current_id;

    scene_vao_t* vao_map;
    uint64_t vao_map_current_id;

    scene_shader_t* shader_map;
    uint64_t shader_map_current_id;

    scene_sprite_t* sprite_map;
    uint64_t sprite_map_current_id;

    scene_entity_t* entity_map;
    uint64_t entity_map_current_id;

    scene_camera_t* camera_map;
    uint64_t camera_map_current_id;
    uint64_t active_camera_id;
} scene_t;

void scene_init(scene_t* scene);

uint64_t scene_add_texture(scene_t* scene, texture_t texture);
uint64_t scene_add_vbo(scene_t* scene, vbo_t vbo);
uint64_t scene_add_vao(scene_t* scene, vao_t vao);
uint64_t scene_add_shader(scene_t* scene, shader_t shader);
uint64_t scene_add_sprite(scene_t* scene, sprite_t sprite);
uint64_t scene_add_entity(scene_t* scene, entity_t entity);
uint64_t scene_add_camera(scene_t* scene, camera_t camera);

texture_t scene_get_texture(scene_t* scene, uint64_t texture_id);
vbo_t scene_get_vbo(scene_t* scene, uint64_t vbo_id);
vao_t scene_get_vao(scene_t* scene, uint64_t vao_id);
shader_t scene_get_shader(scene_t* scene, uint64_t shader_id);
sprite_t scene_get_sprite(scene_t* scene, uint64_t sprite_id);
entity_t scene_get_entity(scene_t* scene, uint64_t entity_id);
camera_t scene_get_camera(scene_t* scene, uint64_t camera_id);
inline camera_t scene_get_active_camera(scene_t* scene) { return scene_get_camera(scene, scene->active_camera_id); }

void scene_update_texture(scene_t* scene, uint64_t texture_id, texture_t texture);
void scene_update_vbo(scene_t* scene, uint64_t vbo_id, vbo_t vbo);
void scene_update_vao(scene_t* scene, uint64_t vao_id, vao_t vao);
void scene_update_shader(scene_t* scene, uint64_t shader_id, shader_t shader);
void scene_update_sprite(scene_t* scene, uint64_t sprite_id, sprite_t sprite);
void scene_update_entity(scene_t* scene, uint64_t entity_id, entity_t entity);
void scene_update_camera(scene_t* scene, uint64_t camera_id, camera_t camera);
inline void scene_update_active_camera(scene_t* scene, camera_t camera) { scene_update_camera(scene, scene->active_camera_id, camera); }

void scene_remove_texture(scene_t* scene, uint64_t texture_id);
void scene_remove_vbo(scene_t* scene, uint64_t vbo_id);
void scene_remove_vao(scene_t* scene, uint64_t vao_id);
void scene_remove_shader(scene_t* scene, uint64_t shader_id);
void scene_remove_sprite(scene_t* scene, uint64_t sprite_id);
void scene_remove_entity(scene_t* scene, uint64_t entity_id);
void scene_remove_camera(scene_t* scene, uint64_t camera_id);

void scene_update_objects(scene_t* scene, float delta_time);

// Sprite update methods
void scene_sprite_set_idle(scene_t* scene, uint64_t sprite_id);
void scene_sprite_increment_keyframe(scene_t* scene, uint64_t sprite_id);
void scene_sprite_set_cycle(scene_t* scene, uint64_t sprite_id, uint8_t cycle);
void scene_sprite_update_position(scene_t* scene, uint64_t sprite_id, vec3f_t direction);

// Entity update methods
void scene_entity_set_direction(scene_t* scene, uint64_t entity_id, direction_t direction);
void scene_entity_set_moving(scene_t* scene, uint64_t entity_id, bool moving);
void scene_entity_update_position(scene_t* scene, uint64_t entity_id, double delta_time);

// Camera update methods
void scene_set_active_camera(scene_t* scene, uint64_t camera_id);

void scene_deinit(scene_t* scene);

#endif //GATHER_SUN_SCENE_H
