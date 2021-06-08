//
// Created by zach on 2021-06-06.
//

#ifndef MINECRAFT_UI_H
#define MINECRAFT_UI_H

#include "vec.h"
#include "scene.h"
#include <string.h>

typedef struct tag_ui_element {
    uint64_t texture_id;
    vec2u_t position;       // Position of UI element in screen space (px)
    vec2u_t size;           // Size of UI element in px
} ui_element_t;

typedef struct tag_textbox {
    ui_element_t;
    char* text;
} textbox_t;

inline uint32_t get_textbox_num_vertices(textbox_t textbox) {
    return strlen(textbox.text) * 6;
}

inline void generate_textbox_vertices(float* vertices, scene_t* scene, font_t font, textbox_t textbox) {
    vec2u_t cursor = textbox.position;
    for (uint32_t i = 0; i < strlen(textbox.text); i++) {

    }
}

#endif //MINECRAFT_UI_H
