CXX              ?= g++
CC               ?= gcc
BUILD_DIR        ?= build
BUILD_TYPE       ?= Release
INSTALL_PREFIX   ?= /usr/local

BUILD_STATIC     ?= OFF
BUILD_EXAMPLES   ?= OFF
BUILD_TESTS      ?= OFF
DISABLE_ASSERTS  ?= OFF
ENABLE_DEBUG     ?= ON
DISABLE_GL_ATTR  ?= ON

VERBOSE_MAKEFILE ?= OFF

ENABLE_ASAN      ?= OFF
ENABLE_USAN      ?= OFF
ENABLE_TSAN      ?= OFF

CMAKE_FLAGS  = -DCMAKE_C_COMPILER=$(CC) -DCMAKE_CXX_COMPILER=$(CXX)
CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DGE_BUILD_EXAMPLES=$(BUILD_EXAMPLES)
CMAKE_FLAGS += -DGE_STATIC=$(BUILD_STATIC) -DCMAKE_VERBOSE_MAKEFILE=$(VERBOSE_MAKEFILE)
CMAKE_FLAGS += -DGE_INSTALL_PREFIX=$(INSTALL_PREFIX)
CMAKE_FLAGS += -DGE_BUILD_TESTS=$(BUILD_TESTS) -DGE_ENABLE_ASAN=$(ENABLE_ASAN)
CMAKE_FLAGS += -DGE_ENABLE_USAN=$(ENABLE_USAN) CMAKE_FLAGS -DGE_ENABLE_TSAN=$(ENABLE_TSAN)
CMAKE_FLAGS += -DGE_DISABLE_ASSERTS=$(DISABLE_ASSERTS) -DGE_DEBUG=$(ENABLE_DEBUG)
CMAKE_FLAGS += -DGE_DISABLE_OPENGL_ATTR=$(DISABLE_GL_ATTR)

RUN_CLANG_TIDY_BIN      ?= run-clang-tidy

ifeq ($(VALGRIND),ON)
	VALGRIND_BIN = valgrind --leak-check=full --error-exitcode=1
endif

# Build project
.PHONY: all
all: build_project

.PHONY: install
install: MAKE_TARGET = install
install: build_project

.PHONY: clang-tidy
clang-tidy: generate_makefiles
clang-tidy: CMAKE_FLAGS += -DGE_EXPORT_COMPILE_CMD=ON -DGE_BUILD_EXAMPLES=ON -DGE_BUILD_TESTS=OFF
clang-tidy:
	$(RUN_CLANG_TIDY_BIN) -p build

.PHONY: build_project
build_project: generate_makefiles
	$(MAKE) -C $(BUILD_DIR) $(MAKE_TARGET)

.PHONY: generate_makefiles
generate_makefiles:
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..

# Clean project
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Tests
# Run GE core test
.PHONY: test_ge_core
test_ge_core:
	$(VALGRIND_BIN) ${BUILD_DIR}/tests/test_ge_core

# Run all tests
.PHONY: test
test: test_ge_core
