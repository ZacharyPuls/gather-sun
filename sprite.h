//
// Created by zach on 2021-05-15.
//

#ifndef GATHER_SUN_SPRITE_H
#define GATHER_SUN_SPRITE_H

#include <stdint.h>
#include "vec.h"

typedef struct tag_sprite {
    uint64_t texture_id;
    vec3f_t position;
    vec2f_t size;
    uint8_t keyframe;
    uint8_t num_keyframes;
    uint8_t cycle;
    uint8_t num_cycles;
    // Number of seconds between each keyframe
    double animation_speed;
} sprite_t;

inline uint8_t get_sprite_num_keyframes_per_cycle(sprite_t sprite) {
    return (sprite.num_keyframes / sprite.num_cycles);
}

inline vec4f_t get_sprite_texcoord_bounds(sprite_t sprite) {
    uint8_t num_keyframes_per_cycle = get_sprite_num_keyframes_per_cycle(sprite);
    float s0 = (float) (sprite.keyframe % num_keyframes_per_cycle) / (float) num_keyframes_per_cycle;
    float t0 = (float) sprite.cycle / (float) sprite.num_cycles;
    float s1 = s0 + 1.0f / (float) num_keyframes_per_cycle;
    float t1 = t0 + 1.0f / (float) sprite.num_cycles;
    vec4f_t result = {
            s0,
            t0,
            s1,
            t1
    };
    return result;
}

#endif //GATHER_SUN_SPRITE_H
