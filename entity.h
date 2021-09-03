//
// Created by zach on 2021-06-05.
//

#ifndef GATHER_SUN_ENTITY_H
#define GATHER_SUN_ENTITY_H

#include <stdint.h>
#include <stdbool.h>

#include "sprite.h"

typedef enum tag_direction {
    UP = 0,
    RIGHT = 1,
    LEFT = 2,
    DOWN = 3
} direction_t;

typedef struct tag_entity {
    uint64_t sprite_id;
    bool moving;
    direction_t direction;
    float speed;
} entity_t;

#endif //GATHER_SUN_ENTITY_H
