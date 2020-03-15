CXX              ?= g++
CC               ?= gcc
BUILD_DIR        ?= build
BUILD_TYPE       ?= Release
INSTALL_PREFIX   ?= /usr/local

BUILD_STATIC     ?= OFF
BUILD_EXAMPLES   ?= OFF
BUILD_TESTS      ?= OFF
ENABLE_ASSERTS   ?= ON

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
CMAKE_FLAGS += -DGE_ENABLE_ASSERTS=$(ENABLE_ASSERTS)

VALGRIND_BIN = valgrind --leak-check=full --error-exitcode=1

# Build project
.PHONY: all
all: build_project

.PHONY: install
install: MAKE_TARGET = install
install: build_project

.PHONY: build_project
build_project:
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..
	$(MAKE) -C $(BUILD_DIR) $(MAKE_TARGET)

# Clean project
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Tests
# Run GE core test
.PHONY: test_ge_core
test_ge_core:
	$(VALGRIND) ${BUILD_DIR}/tests/test_ge_core

# Run all tests
.PHONY: test
test: test_ge_core

# Run all tests with valgrind
.PHONY: test_valgrind
test_valgrind: VALGRIND = $(VALGRIND_BIN)
test_valgrind: test
