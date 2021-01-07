list(APPEND GE_TEST_SRC
    test_core.cpp
    test_scene_serializer.cpp
    test_window.cpp
)

add_executable(ge_test ${GE_TEST_SRC})
target_link_libraries(ge_test
    ge
    gmock
    gtest_main
)
