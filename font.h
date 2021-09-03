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

// TODO: stop being lazy, and just merge these structs
typedef struct tag_msdf_font {
    float distance_range;
    float size;
    uint16_t width;
    uint16_t height;
    float em_size;
    float line_height;
    float ascent;
    float descent;

    // TODO: UTF-8 support...I know this is on everyone's todo list, but I just want to be able to finish this demo...
//    msdf_glyph_t chars[256];
} msdf_font_t;

// TODO: for future support of pages, will need to pass a const char** image_filenames (_0.png, _1.png, _2.png, etc)
// TODO: should I really be coupling this class to scene_t? A naive solution to this would be having the user pass in \
//              a previously-added texture ID
font_t font_load_bmfont(scene_t* scene, const char* font_filename, const char** image_filenames, uint8_t num_images);
font_t font_load_msdf(scene_t* scene, const char* font_filename, const char** image_filenames, uint8_t num_images);

inline vec4f_t get_font_glyph_texcoord_bounds(font_t font, int glyph) {
    glyph_t ch = font.chars[glyph];
    vec4f_t result = {
            (float)ch.x / (float)font.scale_w,
            (float)(ch.y + ch.height) / (float)font.scale_h,
            (float)(ch.x + ch.width) / (float)font.scale_w,
            (float)ch.y / (float)font.scale_h

    };
    return result;
}

inline vec4u_t get_font_glyph_position_bounds(font_t font, int glyph, vec2u_t position, float scale) {
    vec4u_t result = {
            position.x + (uint32_t)((float)font.chars[glyph].x_offset * scale),
            position.y + (uint32_t)((float)(font.base - font.chars[glyph].y_offset - font.chars[glyph].height) * scale),
            position.x + (uint32_t)((float)(font.chars[glyph].x_offset + font.chars[glyph].width) * scale),
            position.y + (uint32_t)((float)(font.base - font.chars[glyph].y_offset) * scale)
    };
    return result;
}

#endif //MINECRAFT_FONT_H
