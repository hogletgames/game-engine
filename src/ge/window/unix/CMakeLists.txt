set(GE_WINDOW_UNIX_SRC
    window_unix.cpp
)

add_library(ge-window-unix STATIC ${GE_WINDOW_UNIX_SRC})
target_link_libraries(ge-window-unix PUBLIC
    ge-core
    SDL2
)
target_include_directories(ge-window-unix PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/SDL2/include
)
