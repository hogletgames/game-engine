set(GE_WINDOW_UNIX_SRC
    input.cpp
    window.cpp
)

add_library(ge-window-unix STATIC ${GE_WINDOW_UNIX_SRC})
target_link_libraries(ge-window-unix PUBLIC
    ge-core
    SDL2
    glad
)
target_include_directories(ge-window-unix SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/SDL2/include
    ${CMAKE_SOURCE_DIR}/third-party/glad/include
)
