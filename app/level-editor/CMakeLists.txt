list(APPEND LE_LEVEL_EDITOR_SRC
    editor_layer.cpp
    editor_layer.h
    level_editor.cpp
)

add_executable(level_editor ${LE_LEVEL_EDITOR_SRC})
target_link_libraries(level_editor
    ge
    docopt
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
