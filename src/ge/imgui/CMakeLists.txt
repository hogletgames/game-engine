set(GE_IMGUI_DIR ${CMAKE_SOURCE_DIR}/third-party/imgui)

include_directories(
    ${GE_IMGUI_DIR}
    ${CMAKE_SOURCE_DIR}/include/ge/imgui
)

set(GE_IMGUI_SRC
    imgui_layer.cpp
)

add_library(ge-imgui STATIC ${GE_IMGUI_SRC})
target_link_libraries(ge-imgui PUBLIC imgui)
if(GE_PLATFORM_UNIX)
    target_link_libraries(ge-imgui PUBLIC ge-imgui-unix)
    add_subdirectory(unix)
endif()
