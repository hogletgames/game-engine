[![Build Status](https://travis-ci.com/hogletgames/game-engine.svg?branch=master)](https://travis-ci.com/hogletgames/game-engine)

## Game Engine

### Dependencies
- [spdlog](https://github.com/gabime/spdlog)
- [Google Test](https://github.com/google/googletest)
- [SDL](https://www.libsdl.org/)

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

### License
Game Engine is licensed under the [BSD 3-Clause license](LICENSE).
