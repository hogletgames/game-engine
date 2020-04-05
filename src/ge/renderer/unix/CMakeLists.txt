set(GE_RENDERER_UNIX_SRC
    opengl_context.cpp
)

add_library(ge-renderer-unix STATIC ${GE_RENDERER_UNIX_SRC})
target_link_libraries(ge-renderer-unix PUBLIC
    glad
    SDL2
)
target_include_directories(ge-renderer-unix SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/glad/include
    ${CMAKE_SOURCE_DIR}/third-party/SDL2/include
)

