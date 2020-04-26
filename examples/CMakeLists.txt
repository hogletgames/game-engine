set(GE_SANDBOX_SRC
    sandbox.cpp
    triangle_layer.cpp
)

add_executable(sandbox_example ${GE_SANDBOX_SRC})
target_link_libraries(sandbox_example
    docopt
    ge
)
