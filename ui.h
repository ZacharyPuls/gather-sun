//
// Created by zach on 2021-06-06.
//

#ifndef MINECRAFT_UI_H
#define MINECRAFT_UI_H

#include "vec.h"
#include "scene.h"
#include "font.h"

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define VIEWPORT_TO_NDC(x, s) ((float)x / (float)s * 2.0f - 1.0f)
#define NDC_TO_VIEWPORT(x, s) ((uint32_t)((x + 1.0f) * ((float)s / 2.0f)))

#define CLAMP(x, min, max, newmin, newmax) ((((x - min) * (newmax - newmin)) / (max - min)) + newmin)

typedef struct tag_ui_element {
    uint64_t texture_id;
    vec2f_t position;
    vec2f_t size;
    vec3f_t texcoord;
    vec2f_t texcoord_size;
} ui_element_t;

inline uint8_t ui_element_get_num_vertices(ui_element_t element) {
    // TODO: this will vary for different element types
    return 6;
}

inline void ui_element_generate_vertices(float* vertices, ui_element_t element) {
    float x0 = element.position.x;
    float y0 = element.position.y;
    float x1 = x0 + element.size.x;
    float y1 = y0 + element.size.y;
    float s0 = element.texcoord.x;
    float t0 = element.texcoord.y;
    float r = element.texcoord.z;
    float s1 = s0 + element.texcoord_size.x;
    float t1 = t0 + element.texcoord_size.y;
    vertices[0] = x0;
    vertices[1] = y0;
    vertices[2] = 0.0f;
    vertices[3] = s0;
    vertices[4] = t0;
    vertices[5] = r;
    vertices[6] = 1.0f;
    vertices[7] = 1.0f;
    vertices[8] = 1.0f;
    vertices[9] = x1;
    vertices[10] = y0;
    vertices[11] = 0.0f;
    vertices[12] = s1;
    vertices[13] = t0;
    vertices[14] = r;
    vertices[15] = 1.0f;
    vertices[16] = 1.0f;
    vertices[17] = 1.0f;
    vertices[18] = x0;
    vertices[19] = y1;
    vertices[20] = 0.0f;
    vertices[21] = s0;
    vertices[22] = t1;
    vertices[23] = r;
    vertices[24] = 1.0f;
    vertices[25] = 1.0f;
    vertices[26] = 1.0f;
    vertices[27] = x1;
    vertices[28] = y0;
    vertices[29] = 0.0f;
    vertices[30] = s1;
    vertices[31] = t0;
    vertices[32] = r;
    vertices[33] = 1.0f;
    vertices[34] = 1.0f;
    vertices[35] = 1.0f;
    vertices[36] = x1;
    vertices[37] = y1;
    vertices[38] = 0.0f;
    vertices[39] = s1;
    vertices[40] = t1;
    vertices[41] = r;
    vertices[42] = 1.0f;
    vertices[43] = 1.0f;
    vertices[44] = 1.0f;
    vertices[45] = x0;
    vertices[46] = y1;
    vertices[47] = 0.0f;
    vertices[48] = s0;
    vertices[49] = t1;
    vertices[50] = r;
    vertices[51] = 1.0f;
    vertices[52] = 1.0f;
    vertices[53] = 1.0f;
}

typedef struct tag_textbox {
    ui_element_t base;
    char* text;
    vec2u_t padding;       // Text padding in px
    float scale;
} textbox_t;

inline uint32_t textbox_get_num_vertices(textbox_t textbox) {
    uint32_t num_chars = 0;
    for (uint32_t i = 0; i < strlen(textbox.text); ++i) {
        if (textbox.text[i] != ' ') {
            ++num_chars;
        }
    }
    return num_chars * 6 + 6;
}

inline void textbox_generate_vertices(float* vertices, font_t font, vec3f_t text_color, textbox_t textbox, uint32_t width, uint32_t height) {
    vec2u_t cursor = { NDC_TO_VIEWPORT(textbox.base.position.x, width), NDC_TO_VIEWPORT(textbox.base.position.y, height)};
    cursor.x += textbox.padding.x;
    cursor.y += textbox.padding.y;
    uint32_t i = 0;
    ui_element_generate_vertices(vertices, textbox.base);
    uint32_t offset = ui_element_get_num_vertices(textbox.base) * 9;
    for (i; i < strlen(textbox.text); i++) {
        if (textbox.text[i] == ' ') {
            // TODO: don't hardcode this (32px xadvance)
            cursor.x += (uint32_t)(32.0f * textbox.scale);
            continue;
        }
        vec4f_t texcoords = get_font_glyph_texcoord_bounds(font, textbox.text[i]);
        vec4u_t position = get_font_glyph_position_bounds(font, textbox.text[i], cursor, textbox.scale);
        cursor.x += (uint32_t)((float)font.chars[textbox.text[i]].x_advance * textbox.scale);
        float x0 = VIEWPORT_TO_NDC(position.x, width);
        float y0 = VIEWPORT_TO_NDC(position.y, height);
        float x1 = VIEWPORT_TO_NDC(position.z, width);
        float y1 = VIEWPORT_TO_NDC(position.w, height);
        vertices[offset] = x0;
        vertices[offset + 1] = y0;
        vertices[offset + 2] = 1.0f;
        vertices[offset + 3] = texcoords.x;
        vertices[offset + 4] = texcoords.y;
        vertices[offset + 5] = 0.0f;
        vertices[offset + 6] = text_color.x;
        vertices[offset + 7] = text_color.y;
        vertices[offset + 8] = text_color.z;
        vertices[offset + 9] = x1;
        vertices[offset + 10] = y0;
        vertices[offset + 11] = 1.0f;
        vertices[offset + 12] = texcoords.z;
        vertices[offset + 13] = texcoords.y;
        vertices[offset + 14] = 0.0f;
        vertices[offset + 15] = text_color.x;
        vertices[offset + 16] = text_color.y;
        vertices[offset + 17] = text_color.z;
        vertices[offset + 18] = x0;
        vertices[offset + 19] = y1;
        vertices[offset + 20] = 1.0f;
        vertices[offset + 21] = texcoords.x;
        vertices[offset + 22] = texcoords.w;
        vertices[offset + 23] = 0.0f;
        vertices[offset + 24] = text_color.x;
        vertices[offset + 25] = text_color.y;
        vertices[offset + 26] = text_color.z;
        vertices[offset + 27] = x1;
        vertices[offset + 28] = y0;
        vertices[offset + 29] = 1.0f;
        vertices[offset + 30] = texcoords.z;
        vertices[offset + 31] = texcoords.y;
        vertices[offset + 32] = 0.0f;
        vertices[offset + 33] = text_color.x;
        vertices[offset + 34] = text_color.y;
        vertices[offset + 35] = text_color.z;
        vertices[offset + 36] = x1;
        vertices[offset + 37] = y1;
        vertices[offset + 38] = 1.0f;
        vertices[offset + 39] = texcoords.z;
        vertices[offset + 40] = texcoords.w;
        vertices[offset + 41] = 0.0f;
        vertices[offset + 42] = text_color.x;
        vertices[offset + 43] = text_color.y;
        vertices[offset + 44] = text_color.z;
        vertices[offset + 45] = x0;
        vertices[offset + 46] = y1;
        vertices[offset + 47] = 1.0f;
        vertices[offset + 48] = texcoords.x;
        vertices[offset + 49] = texcoords.w;
        vertices[offset + 50] = 0.0f;
        vertices[offset + 51] = text_color.x;
        vertices[offset + 52] = text_color.y;
        vertices[offset + 53] = text_color.z;
        offset += 6 * 9;
    }
}

typedef void (*PFNBUTTONCALLBACKPROC)(void*);

typedef struct tag_button {
    textbox_t textbox;
    bool state;
    PFNBUTTONCALLBACKPROC callback;
} button_t;

typedef struct tag_hud_font {
    uint64_t key;
    font_t value;
} hud_font_t;

typedef struct tag_hud_textbox {
    uint64_t key;
    textbox_t value;
} hud_textbox_t;

typedef struct tag_hud {
    hud_textbox_t* textbox_map;
    uint64_t textbox_map_current_id;
} hud_t;

uint64_t hud_add_textbox(hud_t* hud, textbox_t textbox);

textbox_t hud_get_textbox(hud_t* hud, uint64_t textbox_id);

void hud_update_textbox(hud_t* hud, uint64_t textbox_id, textbox_t textbox);

void hud_remove_textbox(hud_t* hud, uint64_t textbox_id);

typedef struct tag_menu_button {
    uint64_t key;
    button_t value;
} menu_button_t;

typedef struct tag_menu_textbox {
    uint64_t key;
    textbox_t value;
} menu_textbox_t;

typedef struct tag_menu {
    menu_button_t* button_map;
    uint64_t button_map_current_id;

    uint64_t active_button_id;

    menu_textbox_t* textbox_map;
    uint64_t textbox_map_current_id;

    bool active;
    bool visible;
    uint64_t font_id;
} menu_t;

uint64_t menu_add_button(menu_t* menu, button_t button);
uint64_t menu_add_textbox(menu_t* menu, textbox_t textbox);

button_t menu_get_button(menu_t* menu, uint64_t button_id);
textbox_t menu_get_textbox(menu_t* menu, uint64_t textbox_id);

uint64_t menu_get_num_buttons(menu_t* menu);
uint64_t menu_get_num_textboxes(menu_t* menu);

void menu_update_button(menu_t* menu, uint64_t button_id, button_t button);
void menu_update_textbox(menu_t* menu, uint64_t textbox_id, textbox_t textbox);

void menu_remove_button(menu_t* menu, uint64_t button_id);
void menu_remove_textbox(menu_t* menu, uint64_t textbox_id);

uint64_t menu_find_button(menu_t* menu, float x, float y);
uint64_t menu_find_textbox(menu_t* menu, float x, float y);

typedef struct tag_ui_font {
    uint64_t key;
    font_t value;
} ui_font_t;

typedef struct tag_ui_hud {
    uint64_t key;
    hud_t value;
} ui_hud_t;

typedef struct tag_ui_menu {
    uint64_t key;
    menu_t value;
} ui_menu_t;

typedef struct tag_ui {
    ui_font_t* font_map;
    uint64_t font_map_current_id;

    ui_hud_t* hud_map;
    uint64_t hud_map_current_id;

    ui_menu_t* menu_map;
    uint64_t menu_map_current_id;
} ui_t;

void ui_init(ui_t* ui, scene_t* scene);

uint64_t ui_add_font(ui_t* ui, font_t font);
uint64_t ui_add_hud(ui_t* ui, hud_t hud);
uint64_t ui_add_menu(ui_t* ui, menu_t menu);

font_t ui_get_font(ui_t* ui, uint64_t font_id);
hud_t ui_get_hud(ui_t* ui, uint64_t hud_id);
menu_t ui_get_menu(ui_t* ui, uint64_t menu_id);

uint64_t ui_get_num_fonts(ui_t* ui);
uint64_t ui_get_num_huds(ui_t* ui);
uint64_t ui_get_num_menus(ui_t* ui);

void ui_update_font(ui_t* ui, uint64_t font_id, font_t font);
void ui_update_hud(ui_t* ui, uint64_t hud_id, hud_t hud);
void ui_update_menu(ui_t* ui, uint64_t menu_id, menu_t menu);

void ui_remove_font(ui_t* ui, uint64_t font_id);
void ui_remove_hud(ui_t* ui, uint64_t hud_id);
void ui_remove_menu(ui_t* ui, uint64_t menu_id);

uint64_t ui_get_active_menu(ui_t* ui);

inline uint32_t ui_get_num_vertices(ui_t* ui) {
    uint32_t num_vertices = 0;
    for (uint64_t menu_id = 0; menu_id < ui_get_num_menus(ui); ++menu_id) {
        menu_t menu = ui_get_menu(ui, menu_id);
//        printf("[ui_get_num_vertices] Getting number of vertices for (%s) menu ID %llu\n", menu.active ? "active" : "inactive", menu_id);
        if (!menu.active) {
            continue;
        }
        for (uint64_t button_id = 0; button_id < menu_get_num_buttons(&menu); ++button_id) {
            button_t button = menu_get_button(&menu, button_id);
            num_vertices += textbox_get_num_vertices(button.textbox);
        }
        for (uint64_t textbox_id = 0; textbox_id < menu_get_num_textboxes(&menu); ++textbox_id) {
            textbox_t textbox = menu_get_textbox(&menu, textbox_id);
            num_vertices += textbox_get_num_vertices(textbox);
        }
    }
    return num_vertices;
}

inline void ui_generate_vertices(ui_t* ui, float* vertices, uint32_t width, uint32_t height) {
    uint32_t num_vertices = 0;
    for (uint64_t menu_id = 0; menu_id < ui_get_num_menus(ui); ++menu_id) {
        menu_t menu = ui_get_menu(ui, menu_id);
        if (!menu.visible) {
            continue;
        }
        font_t font = ui_get_font(ui, menu.font_id);
        for (uint64_t button_id = 0; button_id < menu_get_num_buttons(&menu); ++button_id) {
            button_t button = menu_get_button(&menu, button_id);
            vec3f_t text_color = {0.0f, 0.0f, 0.0f};
            if (button.state) {
                button.textbox.base.texcoord.z += 1.0f;
                text_color.x = 1.0f;
                text_color.y = 1.0f;
                text_color.z = 1.0f;
            }
            textbox_generate_vertices(vertices + num_vertices * 9, font, text_color, button.textbox, width, height);
            num_vertices += textbox_get_num_vertices(button.textbox);
        }
        for (uint64_t textbox_id = 0; textbox_id < menu_get_num_textboxes(&menu); ++textbox_id) {
            textbox_t textbox = menu_get_textbox(&menu, textbox_id);
            vec3f_t text_color = {0.0f, 0.0f, 0.0f};
            textbox_generate_vertices(vertices + num_vertices * 9, font, text_color, textbox, width, height);
            num_vertices += textbox_get_num_vertices(textbox);
        }
    }
}

void ui_deinit(ui_t* ui);

#endif //MINECRAFT_UI_H
