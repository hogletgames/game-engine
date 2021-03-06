set(GE_ECS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/ge/ecs)

list(APPEND GE_ECS_SRC
    camera_controller_script.cpp
    entity.cpp
    entity_registry.cpp
    scene.cpp
    scene_camera.cpp
)

list(APPEND GE_ECS_HEADERS
    ${GE_ECS_INCLUDE_DIR}/camera_controller_script.h
    ${GE_ECS_INCLUDE_DIR}/components.h
    ${GE_ECS_INCLUDE_DIR}/entity.h
    ${GE_ECS_INCLUDE_DIR}/entity_registry.h
    ${GE_ECS_INCLUDE_DIR}/scene.h
    ${GE_ECS_INCLUDE_DIR}/scene_camera.h
    ${GE_ECS_INCLUDE_DIR}/scriptable_entity.h
)

add_library(ge-ecs STATIC ${GE_ECS_SRC} ${GE_ECS_HEADERS})
target_include_directories(ge-ecs PRIVATE
    ${GE_ECS_INCLUDE_DIR}
)
