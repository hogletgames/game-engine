set(GE_RENDERER_OPENGL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND GE_RENDERER_OPENGL_SRC
    buffers.cpp
    framebuffer.cpp
    renderer_api.cpp
    shader_program.cpp
    shader.cpp
    texture.cpp
    vertex_array.cpp
)

list(APPEND GE_RENDERER_OPENGL_HEADERS
    ${GE_RENDERER_OPENGL_INCLUDE_DIR}/buffers.h
    ${GE_RENDERER_OPENGL_INCLUDE_DIR}/framebuffer.h
    ${GE_RENDERER_OPENGL_INCLUDE_DIR}/renderer_api.h
    ${GE_RENDERER_OPENGL_INCLUDE_DIR}/shader.h
    ${GE_RENDERER_OPENGL_INCLUDE_DIR}/shader_program.h
    ${GE_RENDERER_OPENGL_INCLUDE_DIR}/texture.h
    ${GE_RENDERER_OPENGL_INCLUDE_DIR}/vertex_array.h
)

add_library(ge-renderer-opengl STATIC ${GE_RENDERER_OPENGL_SRC} ${GE_RENDERER_OPENGL_HEADERS})
target_link_libraries(ge-renderer-opengl PUBLIC glad)
target_include_directories(ge-renderer-opengl PRIVATE
    ${GE_RENDERER_OPENGL_INCLUDE_DIR}
)
target_include_directories(ge-renderer-opengl SYSTEM PRIVATE
    ${GE_THIRD_PARTY_DIR}/glad/include
    ${GE_THIRD_PARTY_DIR}/stb
)
target_compile_definitions(ge-renderer-opengl PRIVATE
    STB_IMAGE_IMPLEMENTATION
)
