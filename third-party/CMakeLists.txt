# SDL2
set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL2)

# GLAD
add_library(glad SHARED glad/src/glad.c)
target_include_directories(glad PRIVATE glad/include)
install(TARGETS glad LIBRARY DESTINATION lib)

# Dear ImGui
add_Library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_widgets.cpp
)
target_compile_options(imgui PRIVATE
    -Wno-error=unused-variable
    -Wno-error=unused-but-set-variable
)

# Dear ImGui examples
add_library(imgui-examples STATIC
    imgui/examples/imgui_impl_opengl3.cpp
    imgui/examples/imgui_impl_sdl.cpp
)
target_include_directories(imgui-examples SYSTEM PRIVATE
    imgui
    glad/include
    SDL2/include
)
target_compile_definitions(imgui-examples PRIVATE
    -DIMGUI_IMPL_OPENGL_LOADER_GLAD
)
