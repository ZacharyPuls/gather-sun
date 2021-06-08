//
// Created by zach on 2021-05-16.
//

#ifndef MINECRAFT_SCENE_H
#define MINECRAFT_SCENE_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

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
} scene_t;

void begin_scene(scene_t* scene);

uint64_t add_texture(scene_t* scene, texture_t texture);
uint64_t add_vbo(scene_t* scene, vbo_t vbo);
uint64_t add_vao(scene_t* scene, vao_t vao);
uint64_t add_shader(scene_t* scene, shader_t shader);
uint64_t add_sprite(scene_t* scene, sprite_t sprite);
uint64_t add_entity(scene_t* scene, entity_t entity);

texture_t get_texture(scene_t* scene, uint64_t texture_id);
vbo_t get_vbo(scene_t* scene, uint64_t vbo_id);
vao_t get_vao(scene_t* scene, uint64_t vao_id);
shader_t get_shader(scene_t* scene, uint64_t shader_id);
sprite_t get_sprite(scene_t* scene, uint64_t sprite_id);
entity_t get_entity(scene_t* scene, uint64_t entity_id);

void update_texture(scene_t* scene, uint64_t texture_id, texture_t texture);
void update_vbo(scene_t* scene, uint64_t vbo_id, vbo_t vbo);
void update_vao(scene_t* scene, uint64_t vao_id, vao_t vao);
void update_shader(scene_t* scene, uint64_t shader_id, shader_t shader);
void update_sprite(scene_t* scene, uint64_t sprite_id, sprite_t sprite);
void update_entity(scene_t* scene, uint64_t entity_id, entity_t entity);

void remove_texture(scene_t* scene, uint64_t texture_id);
void remove_vbo(scene_t* scene, uint64_t vbo_id);
void remove_vao(scene_t* scene, uint64_t vao_id);
void remove_shader(scene_t* scene, uint64_t shader_id);
void remove_sprite(scene_t* scene, uint64_t sprite_id);
void remove_entity(scene_t* scene, uint64_t entity_id);

void update_scene_objects(scene_t* scene, float delta_time);

// Sprite update methods
void set_sprite_idle(scene_t* scene, uint64_t sprite_id);
void increment_sprite_keyframe(scene_t* scene, uint64_t sprite_id);
void set_sprite_cycle(scene_t* scene, uint64_t sprite_id, uint8_t cycle);
void update_sprite_position(scene_t* scene, uint64_t sprite_id, vec3f_t direction);

// Entity update methods
void set_entity_direction(scene_t* scene, uint64_t entity_id, direction_t direction);
void set_entity_moving(scene_t* scene, uint64_t entity_id, bool moving);
void update_entity_position(scene_t* scene, uint64_t entity_id, double delta_time);

void end_scene(scene_t* scene);

#endif //MINECRAFT_SCENE_H
