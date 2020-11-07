set(GE_RENDERER_OPENGL_SRC
    buffers.cpp
    framebuffer.cpp
    renderer_api.cpp
    shader_program.cpp
    shader.cpp
    texture.cpp
    vertex_array.cpp
)

add_library(ge-renderer-opengl STATIC ${GE_RENDERER_OPENGL_SRC})
target_link_libraries(ge-renderer-opengl PUBLIC glad)
target_include_directories(ge-renderer-opengl SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/glad/include
    ${CMAKE_SOURCE_DIR}/third-party/stb
)
target_compile_definitions(ge-renderer-opengl PRIVATE
    STB_IMAGE_IMPLEMENTATION
)
