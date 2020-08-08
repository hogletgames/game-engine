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
ENABLE_PROFILING ?= ON

ENABLE_ASAN      ?= OFF
ENABLE_USAN      ?= OFF
ENABLE_TSAN      ?= OFF

LOG_LEVEL        ?= GE_COMPILED_LOGLVL_TRACE

CMAKE_FLAGS ?= -DCMAKE_C_COMPILER=$(CC) -DCMAKE_CXX_COMPILER=$(CXX) \
               -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DGE_BUILD_EXAMPLES=$(BUILD_EXAMPLES) \
               -DGE_STATIC=$(BUILD_STATIC) -DGE_INSTALL_PREFIX=$(INSTALL_PREFIX) \
               -DGE_BUILD_TESTS=$(BUILD_TESTS) -DGE_ENABLE_ASAN=$(ENABLE_ASAN) \
               -DGE_ENABLE_USAN=$(ENABLE_USAN) -DGE_ENABLE_TSAN=$(ENABLE_TSAN) \
               -DGE_DISABLE_ASSERTS=$(DISABLE_ASSERTS) -DGE_DEBUG=$(ENABLE_DEBUG) \
               -DGE_PROFILING=$(ENABLE_PROFILING) -DGE_LOG_LEVEL=$(LOG_LEVEL)

RUN_CLANG_TIDY_BIN      ?= run-clang-tidy

DOCKER_IMAGE_NAME   = game-engine-image
DOCKER_RUN_CMD     ?= make -j$$(nproc)

ifeq ($(VALGRIND),ON)
	VALGRIND_BIN = valgrind --leak-check=full --error-exitcode=1
endif

# Build project
.PHONY: all
all: build_project

.PHONY: generate_makefiles
generate_makefiles:
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..

.PHONY: build_project
build_project: generate_makefiles
	$(MAKE) -C $(BUILD_DIR) $(MAKE_TARGET)

.PHONY: install
install: MAKE_TARGET = install
install: build_project

.PHONY: clang-tidy
clang-tidy: generate_makefiles
clang-tidy: CMAKE_FLAGS += -DGE_EXPORT_COMPILE_CMD=ON -DGE_BUILD_EXAMPLES=ON -DGE_BUILD_TESTS=OFF
clang-tidy:
	$(RUN_CLANG_TIDY_BIN) -p build

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

# Docker
.PHONY: docker_init
docker_init:
	docker build -t ${DOCKER_IMAGE_NAME} .

.PHONY: docker_shutdown
docker_shutdown:
	docker rm $(DOCKER_IMAGE_NAME)

.PHONY: docker_run
docker_run:
	docker run --rm \
               -t \
               -w ${PWD} \
               -v ${PWD}:${PWD} \
               -u $$(id -u):$$(id -g) \
               -e CMAKE_FLAGS="$(CMAKE_FLAGS)" \
               -e RUN_CLANG_TIDY_BIN=${RUN_CLANG_TIDY_BIN} \
               $(DOCKER_IMAGE_NAME) \
               bash -c "$(DOCKER_RUN_CMD)"

.PHONY: docker_build
docker_build: DOCKER_RUN_CMD = make -j$$(nproc)
docker_build: docker_run
