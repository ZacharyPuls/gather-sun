//
// Created by zach on 2021-04-24.
//

#ifndef GATHER_SUN_GRAPHICS_H
#define GATHER_SUN_GRAPHICS_H

#include <glad/gl.h>
#include <cglm/struct.h>

void initialize_graphics(GLADloadfunc load, int initial_viewport_width, int initial_viewport_height);
void resize_viewport(int x, int y, int width, int height);

typedef struct tag_vbo {
    GLuint id;
    GLenum target;
    GLenum usage;
} vbo_t ;

vbo_t create_vbo(GLenum target, GLenum usage);
void bind_vbo(vbo_t vbo);
void fill_vbo(vbo_t vbo, GLsizeiptr size, void* data);
void fill_sub_vbo(vbo_t vbo, GLintptr offset, GLsizeiptr size, void* data);
void vbo_set_vertex_attrib(vbo_t vbo, GLint index, GLint size, GLenum type, GLsizei stride, GLsizei offset);

typedef struct tag_vao {
    GLuint id;
} vao_t;

vao_t create_vao();
void bind_vao(vao_t vao);
void draw_vao(vao_t vao, GLenum mode, GLint first, GLsizei count);

typedef struct tag_texture {
    GLenum target;
    GLuint id;
} texture_t;

texture_t create_texture_2d(const char* image_filename, bool generate_mipmaps, bool flip_vertically);
texture_t create_texture_2d_array(const char** image_filenames, uint8_t num_images, bool generate_mipmaps, bool flip_vertically);
void bind_texture(texture_t texture);

typedef struct tag_shader {
    GLuint id;
} shader_t;

shader_t create_shader(const char* vs, const char* fs);
void bind_shader(shader_t shader);
void shader_uniform_mat4(GLint location, mat4s value);
void shader_uniform_texture(GLint location, texture_t value);

extern mat4s model_matrix;
extern mat4s projection_matrix;

mat4s get_model_projection_matrix();

#endif //GATHER_SUN_GRAPHICS_H
