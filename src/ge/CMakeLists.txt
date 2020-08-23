find_package(Boost 1.65 REQUIRED)

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
    ge-gui
    ge-renderer
    ge-window
)
target_include_directories(ge PRIVATE
    ${CMAKE_SOURCE_DIR}/include/ge
)
target_include_directories(ge SYSTEM PRIVATE
    ${Boost_INCLUDE_DIR}
)

install(TARGETS ge
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(renderer)
add_subdirectory(window)
