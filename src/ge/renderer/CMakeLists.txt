set(GE_RENDERER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ge/renderer)

list(APPEND GE_RENDERER_SRC
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

list(APPEND GE_RENDERER_HEADERS
    ${GE_RENDERER_INCLUDE_DIR}/buffer_layout.h
    ${GE_RENDERER_INCLUDE_DIR}/buffers.h
    ${GE_RENDERER_INCLUDE_DIR}/framebuffer.h
    ${GE_RENDERER_INCLUDE_DIR}/graphics_context.h
    ${GE_RENDERER_INCLUDE_DIR}/ortho_camera_controller.h
    ${GE_RENDERER_INCLUDE_DIR}/orthographic_camera.h
    ${GE_RENDERER_INCLUDE_DIR}/render_command.h
    ${GE_RENDERER_INCLUDE_DIR}/renderer_2d.h
    ${GE_RENDERER_INCLUDE_DIR}/renderer_api.h
    ${GE_RENDERER_INCLUDE_DIR}/renderer.h
    ${GE_RENDERER_INCLUDE_DIR}/shader.h
    ${GE_RENDERER_INCLUDE_DIR}/shader_program.h
    ${GE_RENDERER_INCLUDE_DIR}/texture.h
    ${GE_RENDERER_INCLUDE_DIR}/vertex_array.h
)

add_library(ge-renderer STATIC ${GE_RENDERER_SRC} ${GE_RENDERER_HEADERS})
target_link_libraries(ge-renderer PUBLIC
    ge-ecs
    ge-renderer-opengl
)
target_include_directories(ge-renderer PRIVATE
    ${GE_RENDERER_INCLUDE_DIR}
)
if(GE_PLATFORM_UNIX)
    target_link_libraries(ge-renderer PUBLIC ge-renderer-unix)
    add_subdirectory(unix)
endif()

add_subdirectory(opengl)
