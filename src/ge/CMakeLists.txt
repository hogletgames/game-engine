set(GE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ge)

list(APPEND GE_SRC
    app_properties.cpp
    application.cpp
    layer_stack.cpp
    manager.cpp
    thread_pool.cpp
)

list(APPEND GE_HEADERS
    ${GE_INCLUDE_DIR}/app_properties.h
    ${GE_INCLUDE_DIR}/application.h
    ${GE_INCLUDE_DIR}/empty_layer.h
    ${GE_INCLUDE_DIR}/ge.h
    ${GE_INCLUDE_DIR}/layer.h
    ${GE_INCLUDE_DIR}/layer_stack.h
    ${GE_INCLUDE_DIR}/manager.h
    ${GE_INCLUDE_DIR}/thread_pool.h
)

add_library(ge ${GE_LIB_TYPE} ${GE_SRC} ${GE_HEADERS})
target_link_libraries(ge PUBLIC
    ge-core
    ge-ecs
    ge-gui
    ge-renderer
    ge-window
)
target_include_directories(ge PRIVATE
    ${GE_INCLUDE_DIR}
)
target_include_directories(ge SYSTEM PRIVATE
    ${Boost_INCLUDE_DIR}
)

install(TARGETS ge
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

include_directories(utils)

add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(gui)
add_subdirectory(renderer)
add_subdirectory(window)
