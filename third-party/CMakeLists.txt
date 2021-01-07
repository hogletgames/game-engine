# spdlog
add_subdirectory(spdlog)

# google
if(GE_BUILD_TESTS)
    add_subdirectory(googletest)
endif()

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
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)
target_compile_options(imgui PRIVATE
    -Wno-error=unused-variable
    -Wno-error=unused-but-set-variable
    -Wno-error=unused-parameter
)

# Dear ImGui examples
add_library(imgui-examples STATIC
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_sdl.cpp
)
target_include_directories(imgui-examples SYSTEM PRIVATE
    imgui
    glad/include
    SDL2/include
)
target_compile_definitions(imgui-examples PRIVATE
    -DIMGUI_IMPL_OPENGL_LOADER_GLAD
)

# docopt
add_subdirectory(docopt)

# yaml-cpp
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Enable testing" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Enable parse tools" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Enable contrib stuff in library" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "Enable generation of install target" FORCE)
add_subdirectory(yaml-cpp)
target_compile_options(yaml-cpp PRIVATE -Wno-error=effc++)

# nativefiledialog
list(APPEND NATIVEFILEDIALOG_SRC
    nativefiledialog/src/nfd_common.c
)
if (GE_PLATFORM_UNIX)
    list(APPEND NATIVEFILEDIALOG_SRC
        nativefiledialog/src/nfd_gtk.c
    )
endif()
add_library(nativefiledialog ${NATIVEFILEDIALOG_SRC})
target_include_directories(nativefiledialog PUBLIC
    nativefiledialog/src/include
)
if (GE_PLATFORM_UNIX)
    target_include_directories(nativefiledialog PUBLIC ${GTK3_INCLUDE_DIRS})
    target_link_libraries(nativefiledialog PUBLIC ${GTK3_LIBRARIES})
endif ()
