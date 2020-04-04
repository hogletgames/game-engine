add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

set(GE_IMGUI_UNIX_SRC
    imgui_unix.cpp
    ${GE_IMGUI_DIR}/examples/imgui_impl_opengl3.cpp
)

add_library(ge-imgui-unix STATIC ${GE_IMGUI_UNIX_SRC})
target_link_libraries(ge-imgui-unix PUBLIC
    glad
)
target_include_directories(ge-imgui-unix PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/glad/include
)
