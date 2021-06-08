//
// Created by zach on 2021-06-06.
//

#ifndef MINECRAFT_FONT_H
#define MINECRAFT_FONT_H

#include <stdint.h>
#include "scene.h"

typedef struct tag_glyph {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    int16_t x_offset;
    int16_t y_offset;
    uint16_t x_advance;
    // TODO: going to ignore pages for now, in the future (if needed), rework scene.c to support 3D textures, and have \
    //          page be an index into the 3D texture.
    // uint16_t page;
} glyph_t;

typedef struct tag_font {
    uint64_t texture_id;
    uint16_t line_height;
    uint16_t base;
    uint16_t scale_w;
    uint16_t scale_h;
    // TODO: going to ignore pages for now, in the future (if needed), rework scene.c to support 3D textures, and have \
    //          page be an index into the 3D texture.
    // uint16_t pages;

    // TODO: UTF-8 support...I know this is on everyone's todo list, but I just want to be able to finish this demo...
    glyph_t chars[256];
} font_t;

// TODO: for future support of pages, will need to pass a const char** image_filenames (_0.png, _1.png, _2.png, etc)
// TODO: should I really be coupling this class to scene_t? A naive solution to this would be having the user pass in \
//              a previously-added texture ID
font_t load_font(scene_t* scene, const char* font_filename, const char* image_filename);

inline vec4f_t get_font_glyph_texcoord_bounds(font_t font, int glyph) {
    vec4f_t result = {
            (float)font.chars[glyph].x / (float)font.scale_w,
            (float)font.chars[glyph].y / (float)font.scale_h,
            ((float)font.chars[glyph].x + (float)font.chars[glyph].width) / (float)font.scale_w,
            ((float)font.chars[glyph].y + (float)font.chars[glyph].height) / (float)font.scale_h
    };
    return result;
}

#endif //MINECRAFT_FONT_H
