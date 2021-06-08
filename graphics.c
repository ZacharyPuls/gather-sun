//
// Created by zach on 2021-04-24.
//

#include "graphics.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

mat4s model_matrix;
mat4s projection_matrix;

void initialize_graphics(GLADloadfunc load, float initial_viewport_width, float initial_viewport_height) {
    int version = gladLoadGL(load);
    if (version == 0) {
        printf("GLAD error - Failed to initialize OpenGL context\n");
        return;
    }

    printf("GLAD - Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    model_matrix = glms_mat4_identity();
    projection_matrix = glms_mat4_identity();
//    projection_matrix = glms_ortho_default(initial_viewport_width / initial_viewport_height);
}

void resize_viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
    projection_matrix = glms_ortho_default((float) width / (float) height);
}

vbo_t create_vbo(GLenum target, GLenum usage) {
    vbo_t result;
    glCreateBuffers(1, &result.id);
    result.target = target;
    result.usage = usage;
    return result;
}

void bind_vbo(vbo_t vbo) {
    glBindBuffer(vbo.target, vbo.id);
}

void fill_vbo(vbo_t vbo, GLsizeiptr size, void *data) {
    bind_vbo(vbo);
    glBufferData(vbo.target, size, data, vbo.usage);
}

void fill_sub_vbo(vbo_t vbo, GLintptr offset, GLsizeiptr size, void* data) {
    glBufferSubData(vbo.target, offset, size, data);
}

void vbo_set_vertex_attrib(vbo_t vbo, GLint index, GLint size, GLenum type, GLsizei stride, GLsizei offset) {
    bind_vbo(vbo);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, ((char*)NULL + offset));
    glEnableVertexAttribArray(index);
}

vao_t create_vao() {
    vao_t result;
    glCreateVertexArrays(1, &result.id);
    return result;
}

void bind_vao(vao_t vao) {
    glBindVertexArray(vao.id);
}

void draw_vao(vao_t vao, GLenum mode, GLint first, GLsizei count) {
    bind_vao(vao);
    glDrawArrays(mode, first, count);
}

texture_t create_texture(const char *image_filename) {
    texture_t result = {0};

    int width;
    int height;
    int num_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(image_filename, &width, &height, &num_channels, 0);

    if (pixels == NULL) {
        printf("Failed to load image file with filename %s.", image_filename);
        return result;
    }

    // TODO: just hackily adding num_channels == 1 to ternary, really should make this cover all cases.
    GLint internal_format = num_channels == 4 ? GL_RGBA : num_channels == 1 ? GL_RED : GL_RGB;
    GLenum format = num_channels == 4 ? GL_RGBA : num_channels == 1 ? GL_RED : GL_RGB;

    glCreateTextures(GL_TEXTURE_2D, 1, &result.id);
    glBindTexture(GL_TEXTURE_2D, result.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
//    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);

    return result;
}

void bind_texture(texture_t texture) {
    glBindTexture(GL_TEXTURE_2D, texture.id);
}

GLuint compile_shader(GLenum type, const char *source_filename) {

    char *source;
    uint64_t length;
    FILE *fp;

    if (fopen_s(&fp, source_filename, "rb") != 0) {
        printf("Could not open shader file for reading with filename %s.\n", source_filename);
        return 0;
    }

    if (!fp) {
        printf("Unknown error in compile_shader, fp was NULL after fopen_s.\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    source = (char*)malloc(sizeof(char) * length + 1);

    if (!source) {
        printf("Failed to malloc %llu bytes for reading shader file with filename %s.\n", sizeof(char) * length + 1, source_filename);
        fclose(fp);
        return 0;
    }

    fread_s(source, sizeof(char) * length, sizeof(char), length, fp);
    fclose(fp);

    source[length] = '\0';

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const GLchar* const*)&source, NULL);
    glCompileShader(id);

    GLint status = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = (char*)malloc(infoLogLength);
        glGetShaderInfoLog(id, infoLogLength, &infoLogLength, infoLog);

        printf("Failed to compile shader. Error log: \n%s\n", infoLog);

        free(infoLog);
        return 0;
    }

    free(source);
    return id;
}

shader_t create_shader(const char *vs, const char *fs) {
    shader_t result;

    GLuint vs_id = compile_shader(GL_VERTEX_SHADER, vs);
    GLuint fs_id = compile_shader(GL_FRAGMENT_SHADER, fs);

    result.id = glCreateProgram();

    glAttachShader(result.id, vs_id);
    glAttachShader(result.id, fs_id);
    glLinkProgram(result.id);

    GLint status = GL_FALSE;
    glGetProgramiv(result.id, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength = 0;
        glGetProgramiv(result.id, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = (char*)malloc(infoLogLength);
        glGetProgramInfoLog(result.id, infoLogLength, &infoLogLength, infoLog);

        printf("Failed to link shader program. Error log: \n%s\n", infoLog);

        free(infoLog);
    }

    glDeleteShader(vs_id);
    glDeleteShader(fs_id);

    return result;
}

void bind_shader(shader_t shader) {
    glUseProgram(shader.id);
}

void shader_uniform_mat4(GLint location, mat4s value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *) &value.raw);
}

void shader_uniform_texture(GLint location, texture_t value) {
    bind_texture(value);
    glUniform1i(location, GL_TEXTURE0);
}

mat4s get_model_projection_matrix() {
    return glms_mat4_mul(projection_matrix, model_matrix);
}
