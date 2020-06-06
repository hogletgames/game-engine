set(GE_SANDBOX_SRC
    gui_layer.cpp
    sandbox.cpp
    triangle_layer.cpp
)

add_executable(sandbox_example ${GE_SANDBOX_SRC})
target_link_libraries(sandbox_example
    docopt
    ge
)
target_include_directories(sandbox_example SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/docopt
)
