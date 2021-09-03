//
// Created by zach on 2021-06-16.
//

#include <stb_ds.h>
#include <time.h>
#include "ui.h"
#include "event.h"

menu_t main_menu;

uint64_t hud_add_textbox(hud_t *hud, textbox_t textbox) {
    return 0;
}

textbox_t hud_get_textbox(hud_t *hud, uint64_t textbox_id) {
    textbox_t result;
    return result;
}

void hud_update_textbox(hud_t *hud, uint64_t textbox_id, textbox_t textbox) {

}

void hud_remove_textbox(hud_t *hud, uint64_t textbox_id) {

}

uint64_t menu_add_button(menu_t *menu, button_t button) {
    hmput(menu->button_map, menu->button_map_current_id, button);
    return menu->button_map_current_id++;
}
uint64_t menu_add_textbox(menu_t *menu, textbox_t textbox) {
    hmput(menu->textbox_map, menu->textbox_map_current_id, textbox);
    return menu->textbox_map_current_id++;
}

button_t menu_get_button(menu_t *menu, uint64_t button_id) {
    return hmget(menu->button_map, button_id);
}

textbox_t menu_get_textbox(menu_t *menu, uint64_t textbox_id) {
    return hmget(menu->textbox_map, textbox_id);
}

uint64_t menu_get_num_buttons(menu_t *menu) {
    return hmlenu(menu->button_map);
}

uint64_t menu_get_num_textboxes(menu_t *menu) {
    return hmlenu(menu->textbox_map);
}

void menu_update_button(menu_t *menu, uint64_t button_id, button_t button) {
    hmput(menu->button_map, button_id, button);
}

void menu_update_textbox(menu_t *menu, uint64_t textbox_id, textbox_t textbox) {
    hmput(menu->textbox_map, textbox_id, textbox);
}

void menu_remove_button(menu_t *menu, uint64_t button_id) {
    hmdel(menu->button_map, button_id);
}

void menu_remove_textbox(menu_t *menu, uint64_t textbox_id) {
    hmdel(menu->textbox_map, textbox_id);
}

// TODO: handle finding multiple buttons?
uint64_t menu_find_button(menu_t *menu, float x, float y) {
    for (uint64_t i = 0; i < menu_get_num_buttons(menu); i++) {
        button_t button = menu_get_button(menu, i);
        float x0 = button.textbox.base.position.x;
        float y0 = button.textbox.base.position.y;
        float x1 = x0 + button.textbox.base.size.x;
        float y1 = y0 + button.textbox.base.size.y;
        if (x >= x0 && y >= y0 && x <= x1 && y <= y1) {
            return i;
        }
    }
    return UINT64_MAX;
}

// TODO: handle finding multiple textboxes?
uint64_t menu_find_textbox(menu_t *menu, float x, float y) {
    for (uint64_t i = 0; i < menu_get_num_textboxes(menu); i++) {
        textbox_t textbox = menu_get_textbox(menu, i);
        float x0 = textbox.base.position.x;
        float y0 = textbox.base.position.y;
        float x1 = x0 + textbox.base.size.x;
        float y1 = y0 + textbox.base.size.y;
        if (x >= x0 && y >= y0 && x <= x1 && y <= y1) {
            return i;
        }
    }
    return UINT64_MAX;
}

void ui_init(ui_t* ui, scene_t* scene) {
    stbds_rand_seed(time(NULL));
    ui->font_map = NULL;
    ui->font_map_current_id = 0;
    ui->hud_map = NULL;
    ui->hud_map_current_id = 0;
    ui->menu_map = NULL;
    ui->menu_map_current_id = 0;
}

uint64_t ui_add_font(ui_t *ui, font_t font) {
    hmput(ui->font_map, ui->font_map_current_id, font);
    return ui->font_map_current_id++;
}

uint64_t ui_add_hud(ui_t *ui, hud_t hud) {
    hmput(ui->hud_map, ui->hud_map_current_id, hud);
    return ui->hud_map_current_id++;
}

uint64_t ui_add_menu(ui_t *ui, menu_t menu) {
    hmput(ui->menu_map, ui->menu_map_current_id, menu);
    return ui->menu_map_current_id++;
}

font_t ui_get_font(ui_t *ui, uint64_t font_id) {
    return hmget(ui->font_map, font_id);
}

hud_t ui_get_hud(ui_t *ui, uint64_t hud_id) {
    return hmget(ui->hud_map, hud_id);
}

menu_t ui_get_menu(ui_t *ui, uint64_t menu_id) {
    return hmget(ui->menu_map, menu_id);
}

uint64_t ui_get_num_fonts(ui_t *ui) {
    return hmlenu(ui->font_map);
}

uint64_t ui_get_num_huds(ui_t *ui) {
    return hmlenu(ui->hud_map);
}

uint64_t ui_get_num_menus(ui_t *ui) {
    return hmlenu(ui->menu_map);
}

void ui_update_font(ui_t *ui, uint64_t font_id, font_t font) {
    hmput(ui->font_map, font_id, font);
}

void ui_update_hud(ui_t *ui, uint64_t hud_id, hud_t hud) {
    hmput(ui->hud_map, hud_id, hud);
}

void ui_update_menu(ui_t *ui, uint64_t menu_id, menu_t menu) {
//    printf("[ui_update_menu] Updating menu ID %llu to be %s.\n", menu_id, menu.active ? "active" : "inactive");
    hmput(ui->menu_map, menu_id, menu);
}

void ui_remove_font(ui_t *ui, uint64_t font_id) {
    hmdel(ui->font_map, font_id);
}

void ui_remove_hud(ui_t *ui, uint64_t hud_id) {
    hmdel(ui->hud_map, hud_id);
}

void ui_remove_menu(ui_t *ui, uint64_t menu_id) {
    hmdel(ui->menu_map, menu_id);
}

uint64_t ui_get_active_menu(ui_t *ui) {
    for (uint64_t i = 0; i < ui_get_num_menus(ui); ++i) {
        if (ui_get_menu(ui, i).active) {
            return i;
        }
    }
    return UINT64_MAX;
}

void ui_deinit(ui_t* ui) {

}
