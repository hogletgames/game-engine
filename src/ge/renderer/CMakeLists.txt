set(GE_RENDERER_SRC
    buffer_layout.cpp
    buffers.cpp
    framebuffer.cpp
    graphics_context.cpp
    ortho_camera_controller.cpp
    orthographic_camera.cpp
    render_command.cpp
    renderer.cpp
    renderer_2d.cpp
    renderer_api.cpp
    shader_program.cpp
    shader.cpp
    texture.cpp
    vertex_array.cpp
)

add_library(ge-renderer STATIC ${GE_RENDERER_SRC})
target_link_libraries(ge-renderer PUBLIC
    ge-ecs
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
