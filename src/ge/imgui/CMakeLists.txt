set(GE_IMGUI_DIR ${CMAKE_SOURCE_DIR}/third-party/imgui)

include_directories(SYSTEM
    ${GE_IMGUI_DIR}
)

set(GE_IMGUI_SRC
    imgui_layer.cpp
)

add_library(ge-imgui STATIC ${GE_IMGUI_SRC})
target_link_libraries(ge-imgui PUBLIC imgui)
target_include_directories(ge-imgui PRIVATE
    ${CMAKE_SOURCE_DIR}/include/ge/imgui
)
if(GE_PLATFORM_UNIX)
    target_link_libraries(ge-imgui PUBLIC ge-imgui-unix)
    add_subdirectory(unix)
endif()
