set(GE_GUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ge/gui)

list(APPEND GE_GUI_SRC
    gui.cpp
)

list(APPEND GE_GUI_HEADERS
    ${GE_GUI_INCLUDE_DIR}/gui.h
)

add_library(ge-gui STATIC ${GE_GUI_SRC})
target_link_libraries(ge-gui PUBLIC imgui)
target_include_directories(ge-gui PRIVATE
    ${GE_GUI_INCLUDE_DIR}
)
if(GE_PLATFORM_UNIX)
    target_link_libraries(ge-gui PUBLIC ge-gui-unix)
    add_subdirectory(unix)
endif()
