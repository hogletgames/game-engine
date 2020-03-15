cmake_minimum_required(VERSION 3.9)
project(game_engine)

# Project options
option(GE_STATIC            "Build static library"  OFF)
option(GE_BUILD_EXAMPLES    "Build examples"        OFF)
option(GE_BUILD_TEST        "Build tests"           OFF)
option(GE_ENABLE_ASAN       "Build with ASAN"       OFF)
option(GE_ENABLE_USAN       "Build with USAN"       OFF)
option(GE_ENABLE_TSAN       "Build with TSAN"       OFF)
option(GE_ENABLE_ASSERTS    "Enable asserts"        OFF)

# Compile options
set(GE_COMPILER_FLAGS "-Wall -Wextra -Werror -pedantic")

if(GE_ENABLE_ASAN)
    message("Build with ASAN")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -g")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -O1")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fuse-ld=gold")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fno-sanitize-recover")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fsanitize=address")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fsanitize=leak")
    set(CMAKE_CXX_FLAGS ${GE_COMPILER_FLAGS})
elseif(GE_ENABLE_USAN)
    message("Build with USAN")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fuse-ld=gold")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fno-sanitize-recover")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS ${GE_COMPILER_FLAGS})
elseif(GE_ENABLE_TSAN)
    message("Build with TSAN")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fuse-ld=gold")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fno-sanitize-recover")
    set(GE_COMPILER_FLAGS "${GE_COMPILER_FLAGS} -fsanitize=thread")
    set(CMAKE_CXX_FLAGS ${GE_COMPILER_FLAGS})
else()
    message("Build type: '${CMAKE_BUILD_TYPE}'")
    set(CMAKE_CXX_FLAGS_RELEASE "${GE_COMPILER_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${GE_COMPILER_FLAGS} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${GE_COMPILER_FLAGS} -O3 -g")
endif()

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

if(GE_INSTALL_PREFIX)
    message("Install prefix: '${GE_INSTALL_PREFIX}'")
    set(CMAKE_INSTALL_PREFIX ${GE_INSTALL_PREFIX})
endif()

if(GE_ENABLE_ASSERTS)
    message("Enable asserts")
    add_definitions(-DGE_ENABLE_ASSERTS)
endif()

if(UNIX)
    message("Platform: unix")
    option(GE_PLATFORM_UNIX ON)
    add_definitions(-DGE_PLATFORM_UNIX)
elseif(WIN32)
    message("Platform: windows")
    option(GE_PLATFORM_WINDOWS ON)
    add_definitions(-DGE_PLATFORM_WINDOWS)
endif()

# Install
install(DIRECTORY include/ge
    DESTINATION include
)

# Include directories
include_directories(include)
include_directories(third-party/spdlog/include)

# Subdirectories
add_subdirectory(src)
if(GE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(GE_BUILD_TESTS)
    add_subdirectory(tests)
endif()
