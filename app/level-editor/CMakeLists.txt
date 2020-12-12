list(APPEND LE_LEVEL_EDITOR_SRC
    editor_layer.cpp
    level_editor.cpp
    panels/properties_panel.cpp
    panels/scene_hierarchy_panel.cpp
    panels/statistic_panel.cpp
    panels/viewport_panel.cpp
)

list(APPEND LE_LEVEL_EDITOR_HEADERS
    editor_layer.h
    editor_state.h
    panels/panel_base.h
    panels/properties_panel.h
    panels/scene_hierarchy_panel.h
    panels/statistic_panel.h
    panels/viewport_panel.h
)

add_executable(level_editor ${LE_LEVEL_EDITOR_SRC} ${LE_LEVEL_EDITOR_HEADERS})
target_link_libraries(level_editor
    ge
    docopt
)
target_include_directories(level_editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(level_editor SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third-party/docopt
)

# TODO: add assets and config installation
install(TARGETS level_editor
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
