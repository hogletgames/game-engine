[![Build Status](https://travis-ci.com/hogletgames/game-engine.svg?branch=master)](https://travis-ci.com/hogletgames/game-engine)

## Game Engine

### Dependencies
- [spdlog](https://github.com/gabime/spdlog)
- [Google Test](https://github.com/google/googletest)
- [SDL](https://www.libsdl.org/)
- [OpenGL v4.6 generated by GLAD](https://glad.dav1d.de/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [glm](https://github.com/g-truc/glm)
- [docopt.cpp](https://github.com/docopt/docopt.cpp)

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
make test_valgrind
```

### How to build examples
```bash
make CC=gcc CXX=g++ BUILD_EXAMPLES=ON -j$(nproc)
```

### Clang tools
clang-format:
```bash
bash tools/clang_format.sh --clang-format-bin clang-format-9 --fix
```

clang-tidy:
``` bash
make clang-tidy RUN_CLANG_TIDY_BIN=run-clang-tidy-9
```

### License
Game Engine is licensed under the [BSD 3-Clause license](LICENSE).
