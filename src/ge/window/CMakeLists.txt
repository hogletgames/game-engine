set(GE_WINDOW_SRC
    input.cpp
    window.cpp
)

add_library(ge-window STATIC ${GE_WINDOW_SRC})
target_include_directories(ge-window PRIVATE
    ${CMAKE_SOURCE_DIR}/include/ge/window
)
if(GE_PLATFORM_UNIX)
    target_link_libraries(ge-window ge-window-unix)
    add_subdirectory(unix)
endif()
