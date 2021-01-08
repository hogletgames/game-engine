set(GE_GUI_SRC
    gui.cpp
)

add_library(ge-gui STATIC ${GE_GUI_SRC})
target_link_libraries(ge-gui PUBLIC
    imgui
    ImGuizmo
)
target_include_directories(ge-gui PRIVATE
    ${CMAKE_SOURCE_DIR}/include/ge/gui
)
if(GE_PLATFORM_UNIX)
    target_link_libraries(ge-gui PUBLIC ge-gui-unix)
    add_subdirectory(unix)
endif()
