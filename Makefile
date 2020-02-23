CXX              ?= g++
CC               ?= gcc
BUILD_DIR        ?= build
BUILD_TYPE       ?= Release

BUILD_STATIC     ?= OFF
BUILD_EXAMPLES   ?= OFF

VERBOSE_MAKEFILE ?= OFF

CMAKE_FLAGS  = -DCMAKE_C_COMPILER=$(CC) -DCMAKE_CXX_COMPILER=$(CXX)
CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DGE_BUILD_EXAMPLES=$(BUILD_EXAMPLES)
CMAKE_FLAGS += -DGE_STATIC=$(BUILD_STATIC) -DCMAKE_VERBOSE_MAKEFILE=$(VERBOSE_MAKEFILE)

# Build project
.PHONY: all
all: build_project

.PHONY: build_project
build_project:
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..
	$(MAKE) -C $(BUILD_DIR) $(MAKE_TARGET)

# Clean project
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
