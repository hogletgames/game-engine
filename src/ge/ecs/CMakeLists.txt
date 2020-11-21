set(GE_ECS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ge/ecs)

list(APPEND GE_ECS_SRC
    entity.cpp
)

list(APPEND GE_ECS_HEADERS
    ${GE_ECS_INCLUDE_DIR}/entity.h
)

add_library(ge-ecs STATIC ${GE_ECS_SRC} ${GE_ECS_HEADERS})
target_include_directories(ge-ecs PRIVATE
    ${GE_ECS_INCLUDE_DIR}
)
