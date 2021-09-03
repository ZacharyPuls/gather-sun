//
// Created by zach on 2021-06-30.
//

#ifndef MINECRAFT_CAMERA_H
#define MINECRAFT_CAMERA_H

#include "vec.h"

typedef struct tag_camera {
    vec3f_t position;
    uint64_t following_entity_id;
} camera_t;

#endif //MINECRAFT_CAMERA_H
