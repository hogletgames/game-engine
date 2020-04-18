set(GE_IMGUI_UNIX_SRC
    platform_imgui.cpp
)

add_library(ge-imgui-unix STATIC ${GE_IMGUI_UNIX_SRC})
target_link_libraries(ge-imgui-unix PUBLIC
    glad
    imgui-examples
    SDL2
)
target_include_directories(ge-imgui-unix SYSTEM PRIVATE
    ${GE_IMGUI_DIR}/examples
    ${CMAKE_SOURCE_DIR}/third-party/glad/include
    ${CMAKE_SOURCE_DIR}/third-party/SDL2/include
)
