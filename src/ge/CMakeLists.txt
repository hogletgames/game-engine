set(GE_SRC
    application.cpp
    ge.cpp
    layer_stack.cpp
)

add_library(ge ${GE_LIB_TYPE} ${GE_SRC})
target_link_libraries(ge PUBLIC
    ge-core
    ge-imgui
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
add_subdirectory(imgui)
add_subdirectory(window)
