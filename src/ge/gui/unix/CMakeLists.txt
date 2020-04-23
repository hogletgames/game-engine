set(GE_GUI_UNIX_SRC
    gui.cpp
)

add_library(ge-gui-unix STATIC ${GE_GUI_UNIX_SRC})
target_link_libraries(ge-gui-unix PUBLIC
    glad
    imgui-examples
    SDL2
)
target_include_directories(ge-gui-unix SYSTEM PRIVATE
    ${GE_IMGUI_DIR}/examples
    ${CMAKE_SOURCE_DIR}/third-party/glad/include
    ${CMAKE_SOURCE_DIR}/third-party/SDL2/include
)
