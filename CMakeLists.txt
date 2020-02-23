cmake_minimum_required(VERSION 3.9)
project(game_engine)

# Project options
option(GE_STATIC "Build static library" OFF)
option(GE_BUILD_EXAMPLES "Build examples" OFF)

# Compile options
set(GE_COMPILER_FLAGS "-Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${GE_COMPILER_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${GE_COMPILER_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${GE_COMPILER_FLAGS} -O3 -g")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build options
if(GE_STATIC)
    message("Build static lib")
    add_definitions(-DGE_STATIC)
    set(GE_LIB_TYPE STATIC)
else()
    message("Build shared lib")
    set(GE_LIB_TYPE SHARED)
endif()

# Include directories
include_directories(include)

# Subdirectories
if(GE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
