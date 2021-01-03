include_directories(
    utils
)

set(GE_SRC
    app_properties.cpp
    application.cpp
    layer_stack.cpp
    manager.cpp
    thread_pool.cpp
)

add_library(ge ${GE_LIB_TYPE} ${GE_SRC})
target_link_libraries(ge PUBLIC
    ge-core
    ge-ecs
    ge-gui
    ge-renderer
    ge-window
)
target_include_directories(ge PRIVATE
    ${CMAKE_SOURCE_DIR}/include/ge
)

install(TARGETS ge
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(gui)
add_subdirectory(renderer)
add_subdirectory(window)
