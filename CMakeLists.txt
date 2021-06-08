cmake_minimum_required(VERSION 3.19)
project(minecraft C)

set(CMAKE_C_STANDARD 11)

add_executable(minecraft main.c window.h window.c graphics.c graphics.h sprite.h scene.c scene.h vec.h renderer.c renderer.h entity.h player.h font.c font.h ui.h)

add_subdirectory(lib/glad/cmake)
glad_add_library(glad_glcore_46 API gl:core=4.6)

add_subdirectory(lib/glfw)
add_subdirectory(lib/cglm)

target_include_directories(minecraft PRIVATE lib/stb)

target_link_libraries(minecraft PRIVATE glad_glcore_46 glfw cglm)

add_custom_command(TARGET minecraft PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:minecraft>/res)