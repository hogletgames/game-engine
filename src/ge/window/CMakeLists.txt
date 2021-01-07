set(GE_WINDOW_SRC
    dialogs.cpp
    input.cpp
    window.cpp
)

add_library(ge-window STATIC ${GE_WINDOW_SRC})
target_link_libraries(
    ge-window nativefiledialog
)
target_include_directories(ge-window PRIVATE
    ${CMAKE_SOURCE_DIR}/include/ge/window
)
target_include_directories(ge-window SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/nativefiledialog/src/include
    ${CMAKE_SOURCE_DIR}/third-party/nativefiledialog/src
)
if(GE_PLATFORM_UNIX)
    target_link_libraries(ge-window ge-window-unix)
    add_subdirectory(unix)
endif()
