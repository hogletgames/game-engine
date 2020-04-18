set(GE_RENDERER_OPENGL_SRC
    buffers.cpp
    renderer_api.cpp
    shader.cpp
    vertex_array.cpp
)

add_library(ge-renderer-opengl STATIC ${GE_RENDERER_OPENGL_SRC})
target_link_libraries(ge-renderer-opengl PUBLIC glad)
target_include_directories(ge-renderer-opengl SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/glad/include
)
