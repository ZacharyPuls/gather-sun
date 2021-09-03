//
// Created by zach on 2021-05-16.
//

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#include <time.h>

#include "scene.h"

void scene_init(scene_t* scene) {
    stbds_rand_seed(time(NULL));
    scene->texture_map = NULL;
    scene->texture_map_current_id = 0;
    scene->vbo_map = NULL;
    scene->vbo_map_current_id = 0;
    scene->vao_map = NULL;
    scene->vao_map_current_id = 0;
    scene->shader_map = NULL;
    scene->shader_map_current_id = 0;
    scene->sprite_map = NULL;
    scene->sprite_map_current_id = 0;
    scene->entity_map = NULL;
    scene->entity_map_current_id = 0;
    scene->camera_map = NULL;
    scene->camera_map_current_id = 0;
    scene->active_camera_id = UINT64_MAX;
}

uint64_t scene_add_texture(scene_t* scene, texture_t texture) {
    hmput(scene->texture_map, scene->texture_map_current_id, texture);
    return scene->texture_map_current_id++;
}

uint64_t scene_add_vbo(scene_t* scene, vbo_t vbo) {
    hmput(scene->vbo_map, scene->vbo_map_current_id, vbo);
    return scene->vbo_map_current_id++;
}

uint64_t scene_add_vao(scene_t* scene, vao_t vao) {
    hmput(scene->vao_map, scene->vao_map_current_id, vao);
    return scene->vao_map_current_id++;
}

uint64_t scene_add_shader(scene_t* scene, shader_t shader) {
    hmput(scene->shader_map, scene->shader_map_current_id, shader);
    return scene->shader_map_current_id++;
}

uint64_t scene_add_sprite(scene_t* scene, sprite_t sprite) {
    hmput(scene->sprite_map, scene->sprite_map_current_id, sprite);
    return scene->sprite_map_current_id++;
}

uint64_t scene_add_entity(scene_t* scene, entity_t entity) {
    hmput(scene->entity_map, scene->entity_map_current_id, entity);
    return scene->entity_map_current_id++;
}

uint64_t scene_add_camera(scene_t *scene, camera_t camera) {
    hmput(scene->camera_map, scene->camera_map_current_id, camera);
    return scene->camera_map_current_id++;
}

texture_t scene_get_texture(scene_t* scene, uint64_t texture_id) {
    return hmget(scene->texture_map, texture_id);
}

vbo_t scene_get_vbo(scene_t* scene, uint64_t vbo_id) {
    return hmget(scene->vbo_map, vbo_id);
}

vao_t scene_get_vao(scene_t* scene, uint64_t vao_id) {
    return hmget(scene->vao_map, vao_id);
}

shader_t scene_get_shader(scene_t* scene, uint64_t shader_id) {
    return hmget(scene->shader_map, shader_id);
}

sprite_t scene_get_sprite(scene_t* scene, uint64_t sprite_id) {
    return hmget(scene->sprite_map, sprite_id);
}

entity_t scene_get_entity(scene_t* scene, uint64_t entity_id) {
    return hmget(scene->entity_map, entity_id);
}

camera_t scene_get_camera(scene_t *scene, uint64_t camera_id) {
    return hmget(scene->camera_map, camera_id);
}

void scene_update_texture(scene_t *scene, uint64_t texture_id, texture_t texture) {
    hmput(scene->texture_map, texture_id, texture);
}

void scene_update_vbo(scene_t *scene, uint64_t vbo_id, vbo_t vbo) {
    hmput(scene->vbo_map, vbo_id, vbo);
}

void scene_update_vao(scene_t *scene, uint64_t vao_id, vao_t vao) {
    hmput(scene->vao_map, vao_id, vao);
}

void scene_update_shader(scene_t *scene, uint64_t shader_id, shader_t shader) {
    hmput(scene->shader_map, shader_id, shader);
}

void scene_update_sprite(scene_t *scene, uint64_t sprite_id, sprite_t sprite) {
    hmput(scene->sprite_map, sprite_id, sprite);
}

void scene_update_entity(scene_t *scene, uint64_t entity_id, entity_t entity) {
    hmput(scene->entity_map, entity_id, entity);
}

void scene_update_camera(scene_t *scene, uint64_t camera_id, camera_t camera) {
    hmput(scene->camera_map, camera_id, camera);
}

void scene_remove_texture(scene_t* scene, uint64_t texture_id) {
    hmdel(scene->texture_map, texture_id);
}

void scene_remove_vbo(scene_t* scene, uint64_t vbo_id) {
    hmdel(scene->vbo_map, vbo_id);
}

void scene_remove_vao(scene_t* scene, uint64_t vao_id) {
    hmdel(scene->vao_map, vao_id);
}

void scene_remove_shader(scene_t* scene, uint64_t shader_id) {
    hmdel(scene->shader_map, shader_id);
}

void scene_remove_sprite(scene_t* scene, uint64_t sprite_id) {
    hmdel(scene->sprite_map, sprite_id);
}

void scene_remove_entity(scene_t* scene, uint64_t entity_id) {
    hmdel(scene->entity_map, entity_id);
}

void scene_remove_camera(scene_t *scene, uint64_t camera_id) {
    hmdel(scene->camera_map, camera_id);
}

void scene_update_objects(scene_t *scene, float delta_time) {

}

void scene_sprite_set_idle(scene_t *scene, uint64_t sprite_id) {
    sprite_t sprite = scene_get_sprite(scene, sprite_id);
    sprite.keyframe = sprite.cycle * get_sprite_num_keyframes_per_cycle(sprite);
    scene_update_sprite(scene, sprite_id, sprite);
}

void scene_sprite_increment_keyframe(scene_t *scene, uint64_t sprite_id) {
    sprite_t sprite = scene_get_sprite(scene, sprite_id);
    if (++sprite.keyframe % get_sprite_num_keyframes_per_cycle(sprite) == 0) {
        // TODO: Possibly reverse direction, so it cycles from keyframe 0 - 1 - 2 - 1 - 0 - etc, to make a more believable walk cycle. \
        //       Could also do this in the texture itself, for sprites that need it.
        sprite.keyframe -= (get_sprite_num_keyframes_per_cycle(sprite) - 1);
    }
    scene_update_sprite(scene, sprite_id, sprite);
}

void scene_sprite_set_cycle(scene_t *scene, uint64_t sprite_id, uint8_t cycle) {
    sprite_t sprite = scene_get_sprite(scene, sprite_id);
    // TODO: bounds checking
    sprite.cycle = cycle;
    sprite.keyframe = cycle * get_sprite_num_keyframes_per_cycle(sprite);
    scene_update_sprite(scene, sprite_id, sprite);
}

void scene_sprite_update_position(scene_t *scene, uint64_t sprite_id, vec3f_t direction) {
    sprite_t sprite = scene_get_sprite(scene, sprite_id);
    sprite.position.x += direction.x;
    sprite.position.y += direction.y;
    sprite.position.z += direction.z;
    scene_update_sprite(scene, sprite_id, sprite);
}

void scene_entity_set_direction(scene_t *scene, uint64_t entity_id, direction_t direction) {
    entity_t entity = scene_get_entity(scene, entity_id);
    if (entity.direction == direction) {
        return;
    }
    entity.direction = direction;
    scene_sprite_set_cycle(scene, entity.sprite_id, direction);
    scene_update_entity(scene, entity_id, entity);
}

void scene_entity_set_moving(scene_t *scene, uint64_t entity_id, bool moving) {
    entity_t entity = scene_get_entity(scene, entity_id);
    if (entity.moving == moving) {
        return;
    }
    entity.moving = moving;
    if (!entity.moving) {
        scene_sprite_set_idle(scene, entity.sprite_id);
    }
    scene_update_entity(scene, entity_id, entity);
}

void scene_entity_update_position(scene_t *scene, uint64_t entity_id, double delta_time) {
    entity_t entity = scene_get_entity(scene, entity_id);

    if (!entity.moving) {
        return;
    }

    static double accumulated_frame_time = 0.0;
    // TODO: looks like static doesn't work in C like I thought it did (like in C++), there has to be a better way to do this.
    accumulated_frame_time += delta_time;

    sprite_t sprite = scene_get_sprite(scene, entity.sprite_id);
    if (accumulated_frame_time >= sprite.animation_speed) {
        scene_sprite_increment_keyframe(scene, entity.sprite_id);
        accumulated_frame_time = 0.0;
    }

    vec3f_t movement_vector = {0.0f, 0.0f, 0.0f};

    switch (entity.direction) {
        case UP:
            movement_vector.y = entity.speed * delta_time;
            break;
        case RIGHT:
            movement_vector.x = entity.speed * delta_time;
            break;
        case LEFT:
            movement_vector.x = -(entity.speed * delta_time);
            break;
        case DOWN:
            movement_vector.y = -(entity.speed * delta_time);
            break;
    }

    scene_sprite_update_position(scene, entity.sprite_id, movement_vector);

    // TODO: iterate through all cameras (possibly filtering on some new 'active' property?), instead of just pulling the active one for the scene
    camera_t active_camera = scene_get_active_camera(scene);
    if (active_camera.following_entity_id == entity_id) {
        sprite = scene_get_sprite(scene, entity.sprite_id);
        active_camera.position = sprite.position;
        scene_update_active_camera(scene, active_camera);
    }
}

void scene_set_active_camera(scene_t *scene, uint64_t camera_id) {
    scene->active_camera_id = camera_id;
}

void scene_deinit(scene_t* scene) {
    hmfree(scene->texture_map);
    hmfree(scene->vbo_map);
    hmfree(scene->vao_map);
    hmfree(scene->shader_map);
    hmfree(scene->sprite_map);
    hmfree(scene->entity_map);
    hmfree(scene->camera_map);
}
