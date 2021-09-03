//
// Created by zach on 2021-06-30.
//

#ifndef GATHER_SUN_CAMERA_H
#define GATHER_SUN_CAMERA_H

#include "vec.h"

typedef struct tag_camera {
    vec3f_t position;
    uint64_t following_entity_id;
} camera_t;

#endif //GATHER_SUN_CAMERA_H
