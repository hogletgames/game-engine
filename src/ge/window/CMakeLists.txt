include_directories(
    ${CMAKE_SOURCE_DIR}/include/ge/window
)

set(GE_WINDOW_SRC
    window.cpp
)

add_library(ge-window STATIC ${GE_WINDOW_SRC})
if(GE_PLATFORM_UNIX)
    target_link_libraries(ge-window ge-window-unix)
    add_subdirectory(unix)
endif()
