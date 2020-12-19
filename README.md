![CI](https://github.com/hogletgames/game-engine/workflows/CI/badge.svg?branch=master)

## Game Engine

### Acknowledgements
- [spdlog](https://github.com/gabime/spdlog)
- [Google Test](https://github.com/google/googletest)
- [SDL](https://www.libsdl.org/)
- [OpenGL v4.6 generated by GLAD](https://glad.dav1d.de/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [glm](https://github.com/g-truc/glm)
- [docopt.cpp](https://github.com/docopt/docopt.cpp)
- [stb](https://github.com/nothings/stb)
- [entt](https://github.com/skypjack/entt)

### Dependencies
- [Boost v1.65](https://www.boost.org/doc/libs/1_65_0/)

### How to build library
Build shared library:
```bash
make CC=gcc CXX=g++ BUILD_TYPE=Release -j$(nproc)
```

Build static library:
```bash
make CC=gcc CXX=g++ BUILD_TYPE=Release BUILD_STATIC=ON -j$(nproc)
```

Install:
```bash
make install CC=gcc CXX=g++ BUILD_TUPE=Release INSTALL_PREFIX=~/.local -j$(nproc)
```

### How to build project using Docker

Creating Docker image:

```bash
make docker_init
```

Removing Docker image

```bash
make docker_shutdown
```

Building project inside Docker container:

```bash
make BUILD_TESTS=ON BUILD_EXAMPLES=ON docker_build
```

Running tests:

```bash
make DOCKER_RUN_CMD="make test" docker_run
```

### What about tests?
Build tests:
```bash
make CC=gcc CXX=g++ BUILD_TESTS=ON -j$(nproc)
```

Build tests with AddressSanitizer and LeakSanitizer:
```bash
make CC=gcc CXX=g++ BUILD_TESTS=ON ENABLE_ASAN=ON -j$(nproc)
```

Build tests with Undefined Behavior Sanitizer:
```bash
make CC=gcc CXX=g++ BUILD_TESTS=ON ENABLE_USAN=ON -j$(nproc)
```

Build tests with ThreadSanitizer:
```bash
make CC=gcc CXX=g++ BUILD_TESTS=ON ENABLE_TSAN=ON -j$(nproc)
```

Run tests:
```bash
make test
```

Run tests with Valgrind:
```bash
make VALGRIND=ON test
```

### Examples
Build examples:
```bash
make CC=gcc CXX=g++ BUILD_EXAMPLES=ON -j$(nproc)
```

Run example:
```bash
$BUILD_DIR/examples/sandbox -e triangle
```

Get more info:
```bash
$BUILD_DIR/examples/sandbox -h
```

### Clang tools
clang-format:
```bash
bash tools/clang_format.sh --clang-format-bin clang-format-10 --fix
```

clang-tidy:
``` bash
make clang-tidy RUN_CLANG_TIDY_BIN=run-clang-tidy-10
```

### Compile-time log levels
In order to configure compile-time log level
```bash
make LOG_LEVEL=${DESIRED_LOG_LEVEL} -j$(nproc)
```

where `${DESIRED_LOG_LEVEL}` can be one of
```
GE_COMPILED_LOGLVL_CRITICAL
GE_COMPILED_LOGLVL_ERROR
GE_COMPILED_LOGLVL_WARNING
GE_COMPILED_LOGLVL_INFO
GE_COMPILED_LOGLVL_DEBUG
GE_COMPILED_LOGLVL_TRACE
```

`GE_COMPILED_LOGLVL_TRACE` is configured by default

### License
Game Engine is licensed under the [BSD 3-Clause license](LICENSE).
