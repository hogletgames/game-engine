set(GE_RENDERER_UNIX_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND GE_RENDERER_UNIX_SRC
    opengl_context.cpp
)

list(APPEND GE_RENDERER_HEADERS
    ${GE_RENDERER_UNIX_INCLUDE_DIR}/opengl_context.h
)

add_library(ge-renderer-unix STATIC ${GE_RENDERER_UNIX_SRC} ${GE_RENDERER_HEADERS})
target_link_libraries(ge-renderer-unix PUBLIC
    glad
    SDL2
)
target_include_directories(ge-renderer-unix PRIVATE
    ${GE_RENDERER_UNIX_INCLUDE_DIR}
)
target_include_directories(ge-renderer-unix SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/glad/include
    ${CMAKE_SOURCE_DIR}/third-party/SDL2/include
)

