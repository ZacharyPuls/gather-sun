//
// Created by zach on 2021-05-16.
//

#ifndef MINECRAFT_VEC_H
#define MINECRAFT_VEC_H

typedef struct tag_vec2f {
    float x;
    float y;
} vec2f_t;

typedef struct tag_vec3f {
    float x;
    float y;
    float z;
} vec3f_t;

typedef struct tag_vec4f {
    float x;
    float y;
    float z;
    float w;
} vec4f_t;

typedef struct tag_vec2u {
    uint32_t x;
    uint32_t y;
} vec2u_t;

#endif //MINECRAFT_VEC_H
