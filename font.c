//
// Created by zach on 2021-06-06.
//

#include <string.h>
#include "font.h"
#include "graphics.h"
#include "scene.h"

#include <cJSON.h>

// TODO: maybe want to break out UI element texture and font texture loading?
font_t font_load_bmfont(scene_t* scene, const char* font_filename, const char** image_filenames, uint8_t num_images) {
    font_t result = {0};

//    texture_t texture = create_texture_2d(image_filename, true, false);
//    result.texture_id = scene_add_texture(scene, texture);

    texture_t texture = create_texture_2d_array(image_filenames, num_images, false, false);
    result.texture_id = scene_add_texture(scene, texture);

    FILE* font_file_pointer = NULL;

    if (fopen_s(&font_file_pointer, font_filename, "r") != 0) {
        printf("Could not open font file for reading with filename %s.\n", font_filename);
        // TODO: error handling
        return result;
    }

    if (!font_file_pointer) {
        printf("Unknown error in font_load_bmfont, font_file_pointer was NULL after fopen_s.\n");
        // TODO: error handling
        return result;
    }

    fseek(font_file_pointer, 0, SEEK_END);
    int64_t length = ftell(font_file_pointer);
    fseek(font_file_pointer, 0, SEEK_SET);
    char* font_file = (char*)malloc(sizeof(char) * length + 1);

    if (!font_file) {
        printf("Failed to malloc %llu bytes for reading font file with filename %s.\n", sizeof(char) * length + 1, font_filename);
        fclose(font_file_pointer);
        // TODO: error handling
        return result;
    }

    fread_s(font_file, sizeof(char) * length, sizeof(char), length, font_file_pointer);
    fclose(font_file_pointer);

    font_file[length] = '\0';

    const char* newline = "\n";
    char* next_token;
    char* line = strtok_s(font_file, newline, &next_token);

    while (line) {
        if (strncmp(line, "common", 6) == 0) {
            char* line_height = strstr(line, "lineHeight=");
            char* base = strstr(line, "base=");
            char* scale_w = strstr(line, "scaleW=");
            char* scale_h = strstr(line, "scaleH=");
            // char* pages = strstr(line, "pages=");

            if (!line_height || !base || !scale_w || !scale_h /*|| !pages*/) {
                // TODO: error handling
                printf_s("Malformed font file with filename %s. common line did not contain one or more required values - lineHeight, base, scaleW, scaleH.\n", font_filename);
                break;
            }

            // TODO: either check for a space after each item, or loop until isdigit() returns false - for each property. \
            //       this way, the properties do not need to be in a specific order, and we're not depending on a specific file format.
            // hmm, maybe I can bypass this just by using strtoul? will definitely want to fix narrowing cast to uint16_t
            result.line_height = strtoul(line_height + 11, NULL, 10);
            result.base = strtoul(base + 5, NULL, 10);
            result.scale_w = strtoul(scale_w + 7, NULL, 10);
            result.scale_h = strtoul(scale_h + 7, NULL, 10);
            // result.pages = strtoul(pages + 6, NULL, 10);
        } else if (strncmp(line, "char ", 5) == 0) {
            char* id = strstr(line, "id=");
            char* x = strstr(line, "x=");
            char* y = strstr(line, "y=");
            char* width = strstr(line, "width=");
            char* height = strstr(line, "height=");
            char* x_offset = strstr(line, "xoffset=");
            char* y_offset = strstr(line, "yoffset=");
            char* x_advance = strstr(line, "xadvance=");
            // char* page = strstr(line, "page=");

            if (!id || !x || !y || !width || !height || !x_offset || !y_offset || !x_advance /*|| !page*/) {
                // TODO: error handling
                printf_s("Malformed font file with filename %s. char line did not contain one or more required values - id, x, y, width, height, xoffset, yoffset, xadvance.\n", font_filename);
                break;
            }

            // TODO: either check for a space after each item, or loop until isdigit() returns false - for each property. \
            //       this way, the properties do not need to be in a specific order, and we're not depending on a specific file format.
            // hmm, maybe I can bypass this just by using strtoul? will definitely want to fix narrowing cast to uint16_t

            uint64_t index = strtoul(id + 3, NULL, 10);
            result.chars[index].x = strtoul(x + 2, NULL, 10);
            result.chars[index].y = strtoul(y + 2, NULL, 10);
            result.chars[index].width = strtoul(width + 6, NULL, 10);
            result.chars[index].height = strtoul(height + 7, NULL, 10);
            result.chars[index].x_offset = strtol(x_offset + 8, NULL, 10);
            result.chars[index].y_offset = strtol(y_offset + 8, NULL, 10);
            result.chars[index].x_advance = strtoul(x_advance + 9, NULL, 10);
            // result.chars[index].page = strtoul(page + 5, NULL, 10);
        }
        line = strtok_s(NULL, newline, &next_token);
    }

    free(font_file);
    return result;
}

font_t font_load_msdf(scene_t *scene, const char *font_filename, const char **image_filenames, uint8_t num_images) {
    font_t result = {0};
    texture_t texture = create_texture_2d_array(image_filenames, num_images, false, true);
    result.texture_id = scene_add_texture(scene, texture);

    FILE* font_file_pointer = NULL;

    if (fopen_s(&font_file_pointer, font_filename, "r") != 0) {
        printf("Could not open font file for reading with filename %s.\n", font_filename);
        // TODO: error handling
        return result;
    }

    if (!font_file_pointer) {
        printf("Unknown error in font_load_msdf, font_file_pointer was NULL after fopen_s.\n");
        // TODO: error handling
        return result;
    }

    fseek(font_file_pointer, 0, SEEK_END);
    int64_t length = ftell(font_file_pointer);
    fseek(font_file_pointer, 0, SEEK_SET);
    char* font_file = (char*)malloc(sizeof(char) * length + 1);

    if (!font_file) {
        printf("Failed to malloc %llu bytes for reading font file with filename %s.\n", sizeof(char) * length + 1, font_filename);
        fclose(font_file_pointer);
        // TODO: error handling
        return result;
    }

    fread_s(font_file, sizeof(char) * length, sizeof(char), length, font_file_pointer);
    fclose(font_file_pointer);

    font_file[length] = '\0';
    cJSON* json = cJSON_Parse(font_file);

    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error in parsing font file json with filename %s, error is before: %s\n", font_filename, error_ptr);
        }
    } else {
        // TODO: much better JSON parsing error handling
        cJSON* atlas = cJSON_GetObjectItem(json, "atlas");
        cJSON* width = cJSON_GetObjectItem(json, "width");
        result.scale_w = width->valueint;
        cJSON* height = cJSON_GetObjectItem(json, "height");
        result.scale_h = height->valueint;
        cJSON* metrics = cJSON_GetObjectItem(json, "metrics");
        cJSON* line_height = cJSON_GetObjectItem(metrics, "lineHeight");
        result.line_height = (float)line_height->valuedouble;
    }

    cJSON_Delete(json);

    free(font_file);
    return result;
}
