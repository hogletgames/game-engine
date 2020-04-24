set(GE_RENDERER_SRC
    buffer_layout.cpp
    buffers.cpp
    graphics_context.cpp
    orthographic_camera.cpp
    render_command.cpp
    renderer_api.cpp
    renderer.cpp
    shader_program.cpp
    shader.cpp
    vertex_array.cpp
)

add_library(ge-renderer STATIC ${GE_RENDERER_SRC})
target_link_libraries(ge-renderer PUBLIC
    ge-renderer-opengl
)
target_include_directories(ge-renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/include/ge/renderer
)
if(GE_PLATFORM_UNIX)
    target_link_libraries(ge-renderer PUBLIC ge-renderer-unix)
    add_subdirectory(unix)
endif()

add_subdirectory(opengl)
