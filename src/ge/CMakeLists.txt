set(GE_SRC
    ge.cpp
)

add_library(ge ${GE_LIB_TYPE} ${GE_SRC})
target_link_libraries(ge PUBLIC
    ge-core
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
