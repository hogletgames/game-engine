set(GE_GUI_UNIX_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND GE_GUI_UNIX_SRC
    gui.cpp
)

list(APPEND GE_GUI_UNIX_HEADERS
    ${GE_GUI_UNIX_INCLUDE_DIR}/gui.h
)

add_library(ge-gui-unix STATIC ${GE_GUI_UNIX_SRC} ${GE_GUI_UNIX_HEADERS})
target_link_libraries(ge-gui-unix PUBLIC
    glad
    imgui-examples
    SDL2
)
target_include_directories(ge-gui-unix PRIVATE
    ${GE_GUI_UNIX_INCLUDE_DIR}
)
target_include_directories(ge-gui-unix SYSTEM PRIVATE
    ${GE_THIRD_PARTY_DIR}/glad/include
    ${GE_THIRD_PARTY_DIR}/imgui/backends
    ${GE_THIRD_PARTY_DIR}/SDL2/include
)
