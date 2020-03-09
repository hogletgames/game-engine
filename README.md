## Game Engine

### Dependencies
- [spdlog](https://github.com/gabime/spdlog)

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

### How to build examples
```bash
make CC=gcc CXX=g++ BUILD_EXAMPLES=ON -j$(nproc)
```

### License
Game Engine is licensed under the [BSD 3-Clause license](LICENSE).
