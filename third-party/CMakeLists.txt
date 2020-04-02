# SDL2
set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL2)

# GLAD
add_library(glad SHARED glad/src/glad.c)
target_include_directories(glad PRIVATE glad/include)
install(TARGETS glad LIBRARY DESTINATION lib)
