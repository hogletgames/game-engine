set(GE_CORE_SRC
    log.cpp
)

add_library(ge-core STATIC ${GE_CORE_SRC})
target_link_libraries(ge-core PUBLIC spdlog)
target_include_directories(ge-core PRIVATE
    ${CMAKE_SOURCE_DIR}/include/ge/core
)
